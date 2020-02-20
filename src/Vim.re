open EditorCoreTypes;

module AutoClosingPairs = AutoClosingPairs;
module AutoCommands = AutoCommands;
module Buffer = Buffer;
module BufferMetadata = BufferMetadata;
module BufferUpdate = BufferUpdate;
module Clipboard = Clipboard;
module CommandLine = CommandLine;
module Cursor = Cursor;
module Event = Event;
module Mode = Mode;
module Options = Options;
module Search = Search;
module Types = Types;
module Undo = Undo;
module Visual = Visual;
module VisualRange = VisualRange;
module Window = Window;
module Yank = Yank;

type fn = unit => unit;

let queuedFunctions: ref(list(fn)) = ref([]);

let queue = f => queuedFunctions := [f, ...queuedFunctions^];

let flushQueue = () => {
  queuedFunctions^ |> List.rev |> List.iter(f => f());

  queuedFunctions := [];
};

let checkAndUpdateState = f => {
  let oldBuf = Buffer.getCurrent();
  let prevMode = Mode.getCurrent();
  let prevLocation = Cursor.getLocation();
  let prevRange = Visual.getRange();
  let prevTopLine = Window.getTopLine();
  let prevLeftColumn = Window.getLeftColumn();
  let prevVisualMode = Visual.getType();
  let prevModified = Buffer.isModified(oldBuf);

  let ret = f();

  let newBuf = Buffer.getCurrent();
  let newLocation = Cursor.getLocation();
  let newMode = Mode.getCurrent();
  let newRange = Visual.getRange();
  let newLeftColumn = Window.getLeftColumn();
  let newTopLine = Window.getTopLine();
  let newVisualMode = Visual.getType();
  let newModified = Buffer.isModified(newBuf);

  BufferInternal.checkCurrentBufferForUpdate();

  if (newMode != prevMode) {
    Event.dispatch(newMode, Listeners.modeChanged);

    if (newMode == CommandLine) {
      Event.dispatch(
        CommandLineInternal.getState(),
        Listeners.commandLineEnter,
      );
    } else if (prevMode == CommandLine) {
      Event.dispatch((), Listeners.commandLineLeave);
    };
  } else if (newMode == CommandLine) {
    Event.dispatch(
      CommandLineInternal.getState(),
      Listeners.commandLineUpdate,
    );
  };

  if (!Location.(prevLocation == newLocation)) {
    Event.dispatch(newLocation, Listeners.cursorMoved);
  };

  if (prevTopLine != newTopLine) {
    Event.dispatch(newTopLine, Listeners.topLineChanged);
  };

  if (prevLeftColumn != newLeftColumn) {
    Event.dispatch(newLeftColumn, Listeners.leftColumnChanged);
  };

  if (!Range.equals(prevRange, newRange)
      || newMode == Visual
      && prevMode != Visual
      || prevVisualMode != newVisualMode) {
    let vr =
      VisualRange.create(~range=newRange, ~visualType=newVisualMode, ());
    Event.dispatch(vr, Listeners.visualRangeChanged);
  };

  if (prevModified != newModified) {
    let id = Buffer.getId(newBuf);
    Event.dispatch2(id, newModified, Listeners.bufferModifiedChanged);
  };

  flushQueue();
  ret;
};

let _onAutocommand = (autoCommand: Types.autocmd, buffer: Buffer.t) => {
  Event.dispatch2(autoCommand, buffer, Listeners.autocmd);

  switch (autoCommand) {
  | BufWritePost
  | FileWritePost =>
    let id = Buffer.getId(buffer);
    queue(() => Event.dispatch(id, Listeners.bufferWrite));
  | _ => ()
  };
};

let _onBufferChanged =
    (buffer: Buffer.t, startLine: int, endLine: int, extra: int) => {
  let update =
    BufferUpdate.createIncremental(~buffer, ~startLine, ~endLine, ~extra);

  BufferInternal.notifyUpdate(buffer);

  queue(() => Event.dispatch(update, Listeners.bufferUpdate));
};

let _onDirectoryChanged = _ => {
  queue(() => Event.dispatch(Sys.getcwd(), Listeners.directoryChanged));
};

let _onMessage = (priority, title, contents) => {
  queue(() => Event.dispatch3(priority, title, contents, Listeners.message));
};

let _onQuit = (q, f) => {
  queue(() => Event.dispatch2(q, f, Listeners.quit));
};

let _onWindowMovement = (mt, c) => {
  queue(() => Event.dispatch2(mt, c, Listeners.windowMovement));
};

let _onWindowSplit = (st, p) => {
  queue(() => Event.dispatch2(st, p, Listeners.windowSplit));
};

let _onWindowSplit = (st, p) => {
  queue(() => Event.dispatch2(st, p, Listeners.windowSplit));
};

let _onYank =
    (
      lines,
      yankTypeInt,
      operator,
      register,
      startLine,
      startColumn,
      endLine,
      endColumn,
    ) => {
  queue(() =>
    Event.dispatch(
      Yank.create(
        ~lines,
        ~yankTypeInt,
        ~operator,
        ~register,
        ~startLine,
        ~startColumn,
        ~endLine,
        ~endColumn,
        (),
      ),
      Listeners.yank,
    )
  );
};

let _onStopSearch = () => {
  queue(() => Event.dispatch((), Listeners.stopSearchHighlight));
};

let _onUnhandledEscape = () => {
  queue(() => Event.dispatch((), Listeners.unhandledEscape));
};

let _clipboardGet = (regname: int) => {
  switch (Clipboard._provider^) {
  | None => None
  | Some(v) => v(regname)
  };
};

let _onGoto = (line: int, column: int, gotoType: Types.gotoType) => {
  let location =
    Location.create(
      ~line=Index.fromOneBased(line),
      ~column=Index.fromZeroBased(column),
    );
  queue(() => Event.dispatch2(location, gotoType, Listeners.goto));
};

let init = () => {
  Callback.register("lv_clipboardGet", _clipboardGet);
  Callback.register("lv_onBufferChanged", _onBufferChanged);
  Callback.register("lv_onAutocommand", _onAutocommand);
  Callback.register("lv_onDirectoryChanged", _onDirectoryChanged);
  Callback.register("lv_onGoto", _onGoto);
  Callback.register("lv_onMessage", _onMessage);
  Callback.register("lv_onQuit", _onQuit);
  Callback.register("lv_onUnhandledEscape", _onUnhandledEscape);
  Callback.register("lv_onStopSearch", _onStopSearch);
  Callback.register("lv_onWindowMovement", _onWindowMovement);
  Callback.register("lv_onWindowSplit", _onWindowSplit);
  Callback.register("lv_onYank", _onYank);

  Native.vimInit();

  Event.dispatch(Mode.getCurrent(), Listeners.modeChanged);
  BufferInternal.checkCurrentBufferForUpdate();
};

let _getDefaultCursors = (cursors: list(Cursor.t)) =>
  if (cursors == []) {
    [Cursor.get()];
  } else {
    cursors;
  };

let input = (~autoClosingPairs=AutoClosingPairs.empty, ~cursors=[], v: string) => {
  checkAndUpdateState(() => {
    // Special auto-closing pairs handling...

    let runCursor = cursor => {
      Cursor.set(cursor);
      if (Mode.getCurrent() == Types.Insert) {
        let location = Cursor.getLocation();
        let line = Buffer.getLine(Buffer.getCurrent(), location.line);

        let isBetweenPairs = () => {
          AutoClosingPairs.isBetweenPairs(
            line,
            location.column,
            autoClosingPairs,
          );
        };

        let doesNextCharacterMatch = s => {
          AutoClosingPairs.doesNextCharacterMatch(line, location.column, s);
        };

        let canCloseBefore = () =>
          AutoClosingPairs.canCloseBefore(
            line,
            location.column,
            autoClosingPairs,
          );

        if (v == "<BS>" && isBetweenPairs()) {
          Native.vimInput("<DEL>");
          Native.vimInput("<BS>");
        } else if (v == "<CR>" && isBetweenPairs()) {
          Native.vimInput("<CR>");
          Native.vimInput("<CR>");
          Native.vimInput("<UP>");
          Native.vimInput("<TAB>");
        } else if (AutoClosingPairs.isClosingPair(v, autoClosingPairs)
                   && doesNextCharacterMatch(v)) {
          Native.vimInput("<RIGHT>");
        } else if (AutoClosingPairs.isOpeningPair(v, autoClosingPairs)
                   && canCloseBefore()) {
          let pair = AutoClosingPairs.getByOpeningPair(v, autoClosingPairs);
          Native.vimInput(v);
          Native.vimInput(pair.closing);
          Native.vimInput("<LEFT>");
        } else {
          Native.vimInput(v);
        };
      } else {
        Native.vimInput(v);
      };
      Cursor.get();
    };

    let mode = Mode.getCurrent();
    let cursors = _getDefaultCursors(cursors);
    if (mode == Types.Insert) {
      // Run first command, verify we don't go back to normal mode
      switch (cursors) {
      | [hd, ...tail] =>
        let newHead = runCursor(hd);

        let newMode = Mode.getCurrent();
        // If we're still in insert mode, run the command for all the rest of the characters too
        let remainingCursors =
          if (newMode == Types.Insert) {
            List.map(runCursor, tail);
          } else {
            tail;
          };

        [newHead, ...remainingCursors];
      // This should never happen...
      | [] => cursors
      };
    } else {
      switch (cursors) {
      | [hd, ..._] => Cursor.set(hd)
      | _ => ()
      };
      Native.vimInput(v);
      _getDefaultCursors([]);
    };
  });
};

let command = v => {
  checkAndUpdateState(() => Native.vimCommand(v));
};

let onDirectoryChanged = f => {
  Event.add(f, Listeners.directoryChanged);
};

let onGoto = f => {
  Event.add2(f, Listeners.goto);
};

let onMessage = f => {
  Event.add3(f, Listeners.message);
};

let onQuit = f => {
  Event.add2(f, Listeners.quit);
};

let onUnhandledEscape = f => {
  Event.add(f, Listeners.unhandledEscape);
};

let onYank = f => {
  Event.add(f, Listeners.yank);
};
