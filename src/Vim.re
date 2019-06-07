module Buffer = Buffer;

let init = _ => {
  Native.vimInit();
  print_endline("Hello, world!");
};

let input = v => Native.vimInput(v);
let command = v => Native.vimCommand(v);
