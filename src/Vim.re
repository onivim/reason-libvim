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
  let prevVisualMode = Visual.getType();

  f();

  let newPosition = Cursor.getPosition();
  let newMode = Mode.getCurrent();
  let newRange = Visual.getRange();
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

let _onQuit = (q, f) => {
  queue(() => Event.dispatch2(q, f, Listeners.quit));
};

let init = () => {
  Callback.register("lv_onBufferChanged", _onBufferChanged);
  Callback.register("lv_onAutocommand", _onAutocommand);
  Callback.register("lv_onDirectoryChanged", _onDirectoryChanged);
  Callback.register("lv_onQuit", _onQuit);

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

let onQuit = f => {
  Event.add2(f, Listeners.quit);
};
