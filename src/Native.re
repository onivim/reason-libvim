open Types;

type buffer;

external vimInit: unit => unit = "libvim_vimInit";
external vimInput: string => unit = "libvim_vimInput";
external vimCommand: string => unit = "libvim_vimCommand";

external vimGetMode: unit => Types.mode = "libvim_vimGetMode";

external vimBufferOpen: string => buffer = "libvim_vimBufferOpen";
external vimBufferGetId: buffer => int = "libvim_vimBufferGetId";
external vimBufferGetById: int => option(buffer) = "libvim_vimBufferGetById";
external vimBufferGetCurrent: unit => buffer = "libvim_vimBufferGetCurrent";
external vimBufferGetFilename: buffer => option(string) =
  "libvim_vimBufferGetFilename";
external vimBufferGetFiletype: buffer => option(string) =
  "libvim_vimBufferGetFiletype";
external vimBufferGetLine: (buffer, int) => string = "libvim_vimBufferGetLine";
external vimBufferGetLineCount: buffer => int = "libvim_vimBufferGetLineCount";
external vimBufferGetModified: buffer => bool = "libvim_vimBufferGetModified";
external vimBufferGetChangedTick: buffer => int =
  "libvim_vimBufferGetChangedTick";
external vimBufferSetCurrent: buffer => unit = "libvim_vimBufferSetCurrent";

external vimCursorGetLine: unit => int = "libvim_vimCursorGetLine";
external vimCursorGetColumn: unit => int = "libvim_vimCursorGetColumn";

type bufferUpdateCallback = (buffer, int, int, int) => unit;
/* external vimSetBufferUpdateCallback: bufferUpdateCallback => unit; */
