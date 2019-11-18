type t = Native.buffer;

let setAutoClosingPairs = AutoClosingPairs.setEnabled;
let setTabSize = Native.vimOptionSetTabSize;
let setInsertSpaces = Native.vimOptionSetInsertSpaces;

let getAutoClosingPairs = AutoClosingPairs.getEnabled;
let getTabSize = Native.vimOptionGetTabSize;
let getInsertSpaces = Native.vimOptionGetInsertSpaces;
