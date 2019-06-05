module Buffer = Buffer;

external vimInit: unit => unit = "libvim_vimInit";

let init = (_) => {
    vimInit();
    print_endline("Hello, world!");
}

let input = (_) => ();
let command = (_) => ();
