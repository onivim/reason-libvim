open Types;

type buffer;

external vimInit: unit => unit = "libvim_vimInit";
external vimInput: string => unit = "libvim_vimInput";
external vimCommand: string => unit = "libvim_vimCommand";

external vimGetMode: unit => Types.mode = "libvim_vimGetMode";

external vimBufferOpen: string => buffer = "libvim_vimBufferOpen";
external vimBufferGetId: buffer => int = "libvim_vimBufferGetId";
external vimBufferGetById: int => option(buffer) = "libvim_vimBufferGetById";
external vimBufferGetFilename: buffer => option(string) = "libvim_vimBufferGetFilename";
external vimBufferSetCurrent: buffer => unit = "libvim_vimBufferSetCurrent";

external vimCursorGetLine: unit => int = "libvim_vimCursorGetLine";
external vimCursorGetColumn: unit => int = "libvim_vimCursorGetColumn";

type bufferUpdateCallback = (buffer, int, int, int) => unit;
/* external vimSetBufferUpdateCallback: bufferUpdateCallback => unit; */
