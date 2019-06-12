type t = Native.buffer;

module IntSet =
  Set.Make({
    let compare = Pervasives.compare;
    type t = int;
  });

let knownBuffers = ref(IntSet.empty);

let haveSeenBuffer = (buffer: t) => {
  let id = Native.vimBufferGetId(buffer);
  switch (IntSet.find_opt(id, knownBuffers^)) {
  | None => false
  | Some(_) => true
  };
};

let markBufferAsSeen = (buffer: t) => {
  let id = Native.vimBufferGetId(buffer);
  knownBuffers := IntSet.add(id, knownBuffers^);
};

let checkCurrentBufferForUpdate = () => {
  let buffer = Native.vimBufferGetCurrent();
  if (!haveSeenBuffer(buffer)) {
    let update = BufferUpdate.createInitial(buffer);
    markBufferAsSeen(buffer);
    Event.dispatch(buffer, Listeners.bufferEnter);
    Event.dispatch(update, Listeners.bufferUpdate);
  };
};

let openFile = (filePath: string) => {
  let ret = Native.vimBufferOpen(filePath);

  checkCurrentBufferForUpdate();
  ret;
};

let getFilename = (buffer: t) => {
  Native.vimBufferGetFilename(buffer);
};

let getVersion = (buffer: t) => {
  Native.vimBufferGetChangedTick(buffer);
};

let isModified = (buffer: t) => {
  Native.vimBufferGetModified(buffer);
};

let getLineCount = (buffer: t) => {
  Native.vimBufferGetLineCount(buffer);
};

let getLine = (buffer: t, line: int) => {
  Native.vimBufferGetLine(buffer, line);
};

let getId = (buffer: t) => {
  Native.vimBufferGetId(buffer);
};

let getById = (id: int) => {
  Native.vimBufferGetById(id);
};

let getCurrent = () => {
  Native.vimBufferGetCurrent();
};

let setCurrent = (buffer: t) => {
  Native.vimBufferSetCurrent(buffer);
  checkCurrentBufferForUpdate();
};

let onEnter = (f: Listeners.bufferListener) => {
  Event.add(f, Listeners.bufferEnter);
};

let onUpdate = (f: Listeners.bufferUpdateListener) => {
  Event.add(f, Listeners.bufferUpdate);
};
