external vimInit: unit => unit = "libvim_vimInit";

let init = (_) => {
    print_endline("Initializing...");
    vimInit();
    print_endline("Initialized successfully.");
}
