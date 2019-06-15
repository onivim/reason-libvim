type t = Native.buffer;

module IntSet =
  Set.Make({
    let compare = Pervasives.compare;
    type t = int;
  });

let knownBuffers = ref(IntSet.empty);

let haveSeenBuffer = (buffer: t) => {
  let id = Native.vimBufferGetId(buffer);
  let ret = switch (IntSet.find_opt(id, knownBuffers^)) {
  | None => false
  | Some(_) => true
  };
  ret;
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
