open Types;

let bufferUpdateListeners: ref(list(bufferUpdateListener)) = ref([]);
let cursorPositionChangedListeners: ref(list(cursorPositionChangedListener)) = ref([]);
let modeChangedListeners: ref(list(modeChangedListener)) = ref([]);
