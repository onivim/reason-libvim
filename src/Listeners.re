open Types;

type autocmdListener = (autocmd, Native.buffer) => unit;
type bufferListener = Native.buffer => unit;
type bufferUpdateListener = BufferUpdate.t => unit;
type commandLineUpdateListener = Types.cmdline => unit;
type cursorMovedListener = Position.t => unit;
type modeChangedListener = mode => unit;
type visualRangeChangedListener = Range.t => unit;
type noopListener = unit => unit;

let autocmd: ref(list(autocmdListener)) = ref([]);
let bufferEnter: ref(list(bufferListener)) = ref([]);
let bufferUpdate: ref(list(bufferUpdateListener)) = ref([]);
let bufferLeave: ref(list(bufferListener)) = ref([]);
let commandLineEnter: ref(list(commandLineUpdateListener)) = ref([]);
let commandLineUpdate: ref(list(commandLineUpdateListener)) = ref([]);
let commandLineLeave: ref(list(noopListener)) = ref([]);
let cursorMoved: ref(list(cursorMovedListener)) = ref([]);
let modeChanged: ref(list(modeChangedListener)) = ref([]);
let visualRangeChanged: ref(list(visualRangeChangedListener)) = ref([]);
