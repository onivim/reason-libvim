open Types;

type autocmdListener = (autocmd, Native.buffer) => unit;
type bufferListener = Native.buffer => unit;
type bufferUpdateListener = BufferUpdate.t => unit;
type cursorMovedListener = Position.t => unit;
type modeChangedListener = mode => unit;
type visualRangeChangedListener = VisualRange.t => unit;

let autocmd: ref(list(autocmdListener)) = ref([]);
let bufferEnter: ref(list(bufferListener)) = ref([]);
let bufferUpdate: ref(list(bufferUpdateListener)) = ref([]);
let bufferLeave: ref(list(bufferListener)) = ref([]);
let cursorMoved: ref(list(cursorMovedListener)) = ref([]);
let modeChanged: ref(list(modeChangedListener)) = ref([]);
let visualRangeChanged: ref(list(visualRangeChangedListener)) = ref([]);
