open Types;

external vimInit: unit => unit = "libvim_vimInit";
external vimInput: string => unit = "libvim_vimInput";
external vimCommand: string => unit = "libvim_vimCommand";

external vimGetMode: unit => Types.mode = "libvim_vimGetMode";

external vimBufferOpen: string => Types.buffer = "libvim_vimBufferOpen";
external vimBufferGetById: int => option(Types.buffer) = "libvim_vimBufferGetById";
external vimBufferSetCurrent: Types.buffer => unit = "libvim_vimBufferSetCurrent";

type bufferUpdateCallback = (Types.buffer, int, int, int) => unit;
external vimSetBufferUpdateCallback: bufferUpdateCallback => unit;
