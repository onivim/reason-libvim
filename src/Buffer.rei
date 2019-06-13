type t = Native.buffer;

/**
[openFile(path)] opens a file, sets it as the active buffer, and returns a handle to the buffer.
*/
let openFile: string => t;

/**
[getFileName(buffer)] returns the full file path of the buffer [buffer]
*/
let getFilename: t => option(string);

/**
[getVersion(buffer)] returns the latest changedtick of the buffer [buffer].

The changedtick gets updated with each modification to the buffer
*/
let getVersion: t => int;

/**
[isModified(buffer)] returns [true] if the buffer has been modified since the last save, [false] otherwise.
*/
let isModified: t => bool;

/**
[getLineCount(buffer)] returns the number of lines in the buffer.
*/
let getLineCount:  t => int;

/**
[getline(buffer, line)] returns the text content at the one-based line number [line] for buffer [buffer].
*/
let getLine: (t, int) => string;

/**
[getId(buffer)] returns the id of buffer [buffer];
*/
let getId: t => int;

/**
[getById(id)] returns a buffer if one is available with the specified id if it exists, otherwise [None].
*/
let getById: int => option(t);

/**
[getCurrent()] returns the currently active buffer.
*/
let getCurrent: unit => t;

/**
[setCurrent(buffer)] sets the active buffer to [buffer].

This will trigger dispatching of autocommands, like [BufEnter].
*/
let setCurrent: t => unit;

/**
[onEnter(f)] adds a listener [f] that is called whenever a new buffer is entered.

This is more reliable than autocommands, as it will dispatch in any case the buffer
is changed, even in cases where [BufEnter] would not be dispatched.

Returns a function that can be called to unsubscribe.
*/
let onEnter: Listeners.bufferListener => Event.unsubscribe;

/**
[onUpdate(f)] adds a listener [f] that is called whenever a buffer is modified.

Returns a function that can be called to unsubscribe.
*/
let onUpdate: Listeners.bufferUpdateListener => Event.unsubscribe;
