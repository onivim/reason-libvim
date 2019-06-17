let getWidth = Native.vimWindowGetWidth;
let getHeight = Native.vimWindowGetHeight;
let getTopLine = Native.vimWindowGetTopLine;
let setWidth = Native.vimWindowSetWidth;
let setHeight = Native.vimWindowSetHeight;

let onTopLineChanged = f => Event.add(f, Listeners.topLineChanged);
