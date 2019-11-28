module AutoClosingPairs = AutoClosingPairs;
module AutoCommands = AutoCommands;
module Buffer = Buffer;
module BufferMetadata = BufferMetadata;
module BufferUpdate = BufferUpdate;
module Clipboard = Clipboard;
module CommandLine = CommandLine;
module Cursor = Cursor;
module Mode = Mode;
module Options = Options;
module Position = Position;
module Range = Range;
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
  let prevPosition = Cursor.getPosition();
  let prevRange = Visual.getRange();
  let prevTopLine = Window.getTopLine();
  let prevLeftColumn = Window.getLeftColumn();
  let prevVisualMode = Visual.getType();
  let prevModified = Buffer.isModified(oldBuf);

  let ret = f();

  let newBuf = Buffer.getCurrent();
  let newPosition = Cursor.getPosition();
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

  if (!Position.equals(prevPosition, newPosition)) {
    Event.dispatch(newPosition, Listeners.cursorMoved);
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

let init = () => {
  Callback.register("lv_clipboardGet", _clipboardGet);
  Callback.register("lv_onBufferChanged", _onBufferChanged);
  Callback.register("lv_onAutocommand", _onAutocommand);
  Callback.register("lv_onDirectoryChanged", _onDirectoryChanged);
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

let _createCursorFromCurrent = () => {
  Cursor.create(~line=Cursor.getLine(), ~column=Cursor.getColumn(), ());
};

let _getDefaultCursors = (cursors: list(Cursor.t)) =>
  if (cursors == []) {
    [_createCursorFromCurrent()];
  } else {
    cursors;
  };

let input = (~cursors=[], v: string) => {
  checkAndUpdateState(() => {
    // Special auto-closing pairs handling...

    let runCursor = (~first, cursor) => {
      print_endline ("!! RUNCURSOR - 1");
      Cursor.set(cursor);
      //if (!first) {
        Undo.saveCursor();
      //}
      if (AutoClosingPairs.getEnabled() && Mode.getCurrent() == Types.Insert) {
        let isBetweenPairs = () => {
          let position = Cursor.getPosition();
          let line = Buffer.getLine(Buffer.getCurrent(), position.line);
          AutoClosingPairs.isBetweenPairs(line, position.column);
        };

        let doesNextCharacterMatch = s => {
          let position = Cursor.getPosition();
          let line = Buffer.getLine(Buffer.getCurrent(), position.line);
          AutoClosingPairs.doesNextCharacterMatch(line, position.column, s);
        };
        if (v == "<BS>" && isBetweenPairs()) {
          Native.vimInput("<DEL>");
          Native.vimInput("<BS>");
        } else if (v == "<CR>" && isBetweenPairs()) {
          Native.vimInput("<CR>");
          Native.vimInput("<CR>");
          Native.vimInput("<UP>");
          Native.vimInput("<TAB>");
        } else if (AutoClosingPairs.isClosingPair(v)
                   && doesNextCharacterMatch(v)) {
          Native.vimInput("<RIGHT>");
        } else if (AutoClosingPairs.isOpeningPair(v)) {
          let pair = AutoClosingPairs.getByOpeningPair(v);
          Native.vimInput(v);
          Native.vimInput(pair.closing);
          Native.vimInput("<LEFT>");
        } else {
          Native.vimInput(v);
        };
      } else {
        Native.vimInput(v);
      };
      _createCursorFromCurrent();
    };

    let mode = Mode.getCurrent();
    let cursors = _getDefaultCursors(cursors);
    if (mode == Types.Insert) {
      // Run first command, verify we don't go back to insert mode
      switch (cursors) {
      | [hd, ...tail] =>
        let newHead = runCursor(~first=true, hd);

        let newMode = Mode.getCurrent();
        // If we're still in insert mode, run the command for all the rest of the characters too
        let remainingCursors =
          if (newMode == Types.Insert) {
            List.map(runCursor(~first=false), tail);
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
