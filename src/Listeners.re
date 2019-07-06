open Types;

type autocmdListener = (autocmd, Native.buffer) => unit;
type bufferListener = Native.buffer => unit;
type bufferUpdateListener = BufferUpdate.t => unit;
type commandLineUpdateListener = Types.cmdline => unit;
type directoryChangedListener = string => unit;
type cursorMovedListener = Position.t => unit;
type messageListener = (Types.msgPriority, string, string) => unit;
type modeChangedListener = mode => unit;
type topLineChangedListener = int => unit;
type visualRangeChangedListener = VisualRange.t => unit;
type noopListener = unit => unit;

let autocmd: ref(list(autocmdListener)) = ref([]);
let bufferEnter: ref(list(bufferListener)) = ref([]);
let bufferUpdate: ref(list(bufferUpdateListener)) = ref([]);
let bufferLeave: ref(list(bufferListener)) = ref([]);
let commandLineEnter: ref(list(commandLineUpdateListener)) = ref([]);
let commandLineUpdate: ref(list(commandLineUpdateListener)) = ref([]);
let commandLineLeave: ref(list(noopListener)) = ref([]);
let directoryChanged: ref(list(directoryChangedListener)) = ref([]);
let cursorMoved: ref(list(cursorMovedListener)) = ref([]);
let message: ref(list(messageListener)) = ref([]);
let modeChanged: ref(list(modeChangedListener)) = ref([]);
let topLineChanged: ref(list(topLineChangedListener)) = ref([]);
let visualRangeChanged: ref(list(visualRangeChangedListener)) = ref([]);
