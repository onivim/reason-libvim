type t = Native.buffer;

module IntMap =
  Map.Make({
    let compare = Pervasives.compare;
    type t = int;
  });

let knownBuffers: ref(IntMap.t(int)) = ref(IntMap.empty);
let currentBuffer: ref(option(Native.buffer)) = ref(None);

let notifyUpdate = (buffer: t) => {
  let id = Native.vimBufferGetId(buffer);
  let version = Native.vimBufferGetChangedTick(buffer);
  knownBuffers :=
    IntMap.update(
      id,
      curr =>
        switch (curr) {
        | None => Some(version)
        | Some(_) => Some(version)
        },
      knownBuffers^,
    );
};

let doFullUpdate = (buffer: t) => {
  let bu = BufferUpdate.createFull(buffer);
  notifyUpdate(buffer);
  Event.dispatch(bu, Listeners.bufferUpdate);
};

let checkCurrentBufferForUpdate = () => {
  let buffer = Native.vimBufferGetCurrent();
  let id = Native.vimBufferGetId(buffer);

  switch (IntMap.find_opt(id, knownBuffers^)) {
  | None =>
    let update = BufferUpdate.createInitial(buffer);
    notifyUpdate(buffer);
    Event.dispatch(buffer, Listeners.bufferEnter);
    Event.dispatch(update, Listeners.bufferUpdate);
  | Some(lastVersion) =>
    let newVersion = Native.vimBufferGetChangedTick(buffer);

    if (newVersion > lastVersion) {
      doFullUpdate(buffer);
    };

    /* Check if the current buffer changed */
    switch (currentBuffer^) {
    | Some(v) =>
      if (v != buffer) {
        Event.dispatch(v, Listeners.bufferLeave);
        Event.dispatch(buffer, Listeners.bufferEnter);
        currentBuffer := Some(buffer);
      }
    | None =>
      Event.dispatch(buffer, Listeners.bufferEnter);
      currentBuffer := Some(buffer);
    };
  };
};
