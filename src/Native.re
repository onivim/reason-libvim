open Types;

external vimInit: unit => unit = "libvim_vimInit";
external vimInput: string => unit = "libvim_vimInput";
external vimCommand: string => unit = "libvim_vimCommand";

external vimGetMode: unit => Types.mode = "libvim_vimGetMode";

external vimBufferOpen: string => Types.buffer = "libvim_vimBufferOpen";
external vimBufferGetId: Types.buffer => int = "libvim_vimBufferGetId";
external vimBufferGetById: int => option(Types.buffer) = "libvim_vimBufferGetById";
external vimBufferSetCurrent: Types.buffer => unit = "libvim_vimBufferSetCurrent";

external vimCursorGetLine: unit => int = "libvim_vimCursorGetLine";
external vimCursorGetColumn: unit => int = "libvim_vimCursorGetColumn";

type bufferUpdateCallback = (Types.buffer, int, int, int) => unit;
/* external vimSetBufferUpdateCallback: bufferUpdateCallback => unit; */
