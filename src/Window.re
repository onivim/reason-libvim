let getWidth = Native.vimWindowGetWidth;
let getHeight = Native.vimWindowGetHeight;
let getTopLine = Native.vimWindowGetTopLine;
let setWidth = Native.vimWindowSetWidth;
let setHeight = Native.vimWindowSetHeight;

let onLeftColumnChanged = f => Event.add(f, Listeners.leftColumnChanged);
let onTopLineChanged = f => Event.add(f, Listeners.topLineChanged);
let onSplit = f => Event.add2(f, Listeners.windowSplit);
let onMovement = f => Event.add2(f, Listeners.windowMovement);
