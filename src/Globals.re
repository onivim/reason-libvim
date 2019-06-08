open Types;

let bufferUpdateListeners: ref(list(bufferUpdateListener)) = ref([]);
let modeChangedListeners: ref(list(modeChangedListener)) = ref([]);
