module AutoClosingPairs = AutoClosingPairs;
module AutoCommands = AutoCommands;
module Buffer = Buffer;
module BufferMetadata = BufferMetadata;
module BufferUpdate = BufferUpdate;
module CommandLine = CommandLine;
module Cursor = Cursor;
module Mode = Mode;
module Options = Options;
module Position = Position;
module Range = Range;
module Search = Search;
module Types = Types;
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
  let prevMode = Mode.getCurrent();
  let prevPosition = Cursor.getPosition();
  let prevRange = Visual.getRange();
  let prevTopLine = Window.getTopLine();
  let prevLeftColumn = Window.getLeftColumn();
  let prevVisualMode = Visual.getType();

  f();

  let newPosition = Cursor.getPosition();
  let newMode = Mode.getCurrent();
  let newRange = Visual.getRange();
  let newLeftColumn = Window.getLeftColumn();
  let newTopLine = Window.getTopLine();
  let newVisualMode = Visual.getType();

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

  flushQueue();
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

let init = () => {
  Callback.register("lv_onBufferChanged", _onBufferChanged);
  Callback.register("lv_onAutocommand", _onAutocommand);
  Callback.register("lv_onDirectoryChanged", _onDirectoryChanged);
  Callback.register("lv_onMessage", _onMessage);
  Callback.register("lv_onQuit", _onQuit);
  Callback.register("lv_onStopSearch", _onStopSearch);
  Callback.register("lv_onWindowMovement", _onWindowMovement);
  Callback.register("lv_onWindowSplit", _onWindowSplit);
  Callback.register("lv_onYank", _onYank);

  Native.vimInit();

  Event.dispatch(Mode.getCurrent(), Listeners.modeChanged);
  BufferInternal.checkCurrentBufferForUpdate();
};

let input = v => {
  checkAndUpdateState(() => Native.vimInput(v));
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

let onYank = f => {
  Event.add(f, Listeners.yank);
};
