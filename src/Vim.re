module AutoCommands = AutoCommands;
module Buffer = Buffer;
module BufferMetadata = BufferMetadata;
module BufferUpdate = BufferUpdate;
module Cursor = Cursor;
module Mode = Mode;
module Visual = Visual;
module VisualRange = VisualRange;

type fn = unit => unit;

let queuedFunctions: ref(list(fn)) = ref([]);

let queue = f => queuedFunctions := [f, ...queuedFunctions^];

let flushQueue = () => {
  List.iter(f => f(), queuedFunctions^);
  queuedFunctions := [];
};

let checkAndUpdateState = f => {
  let prevMode = Mode.getCurrent();
  let prevPosition = Cursor.getPosition();
  let prevRange = Visual.getRange();

  f();

  let newPosition = Cursor.getPosition();
  let newMode = Mode.getCurrent();
  let newRange = Visual.getRange();

  Buffer.checkCurrentBufferForUpdate();

  if (newMode != prevMode) {
    Event.dispatch(newMode, Listeners.modeChanged);
  };

  if (Position.equals(prevPosition, newPosition)) {
    Event.dispatch(newPosition, Listeners.cursorMoved);
  };

  if (!VisualRange.equals(prevRange, newRange)) {
    Event.dispatch(newRange, Listeners.visualRangeChanged);
  }

  flushQueue();
};

let _onAutocommand = (autoCommand: Types.autocmd, buffer: Buffer.t) => {
  Event.dispatch2(autoCommand, buffer, Listeners.autocmd);
};

let _onBufferChanged =
    (buffer: Buffer.t, startLine: int, endLine: int, extra: int) => {
  let update = BufferUpdate.create(~buffer, ~startLine, ~endLine, ~extra);

  queue(() => Event.dispatch(update, Listeners.bufferUpdate));
};

let init = () => {
  Callback.register("lv_onBufferChanged", _onBufferChanged);
  Callback.register("lv_onAutocommand", _onAutocommand);

  Native.vimInit();

  Event.dispatch(Mode.getCurrent(), Listeners.modeChanged);
};

let input = v => {
  checkAndUpdateState(() => Native.vimInput(v));
};

let command = v => {
  checkAndUpdateState(() => Native.vimCommand(v));
};
