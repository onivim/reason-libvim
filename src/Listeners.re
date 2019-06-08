open Types;

type modeChangedListener = mode => unit;
type bufferListener = Native.buffer => unit;
type bufferUpdateListener = BufferUpdate.t => unit;
type cursorMovedListener = Position.t => unit;

let bufferEnter: ref(list(bufferListener)) = ref([]);
let bufferUpdate: ref(list(bufferUpdateListener)) = ref([]);
let bufferLeave: ref(list(bufferListener)) = ref([]);
let cursorMoved: ref(list(cursorMovedListener)) = ref([]);
let modeChanged: ref(list(modeChangedListener)) = ref([]);
