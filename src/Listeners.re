open Types;

type modeChangedListener = mode => unit;
type bufferUpdateListener = BufferUpdate.t => unit;
type cursorMovedListener = Position.t => unit;

let bufferUpdate: ref(list(bufferUpdateListener)) = ref([]);
let cursorMoved: ref(list(cursorMovedListener)) = ref([]);
let modeChanged: ref(list(modeChangedListener)) = ref([]);
