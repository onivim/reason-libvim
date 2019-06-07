module Buffer = Buffer;

external vimInit: unit => unit = "libvim_vimInit";
external vimInput: string => unit = "libvim_vimInput";
external vimCommand: string => unit = "libvim_vimCommand";

let init = _ => {
  vimInit();
  print_endline("Hello, world!");
};

let input = v => vimInput(v);
let command = v => vimCommand(v);
