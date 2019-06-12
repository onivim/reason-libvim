type t = Native.buffer;

module IntSet =
  Set.Make({
    let compare = Pervasives.compare;
    type t = int;
  });

let knownBuffers = ref(IntSet.empty);

let haveSeenBuffer = (buffer: t) => {
  let id = Native.vimBufferGetId(buffer);
  switch (IntMap.find_opt(id, knownBuffers^)) {
  | None => false
  | Some(_) => true
  };
};

let markBufferAsSeen = (buffer: t) => {
  let id = Native.vimBufferGetId(buffer);
  knownBuffers := IntSet.add(id, knownBuffers^);
};

let checkCurrentBufferForUpdate = () => {
  print_endline("!!!CHECKING CURRENT BUFFER...");

  let buffer = Native.vimBufferGetCurrent();
  if (!haveSeenBuffer(buffer)) {
    print_endline("SENDING FIRST TIME UPDATE!");
    let update = BufferUpdate.createInitial(buffer);
    markBufferAsSeen(buffer);
    Event.dispatch(buffer, Listeners.bufferEnter);
    Event.dispatch(update, Listeners.bufferUpdate);
  };
};

let openFile = (filePath: string) => {
  print_endline("Start open....\n");
  Native.vimBufferOpen(filePath);
  print_endline("End open....\n");

  checkCurrentBufferForUpdate();
};

let getFilename = (buffer: t) => {
  Native.vimBufferGetFilename(buffer);
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
