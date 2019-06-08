module Buffer = Buffer;
module BufferUpdate = BufferUpdate;
module Mode = Mode;

let init = _ => {
  Native.vimInit();
  print_endline("Hello, world!");
};

let input = v => {
    let prevMode = Mode.getCurrent();
    print_endline ("Previous mode: " ++ Mode.show(prevMode));

    Native.vimInput(v);

    let newMode = Mode.getCurrent();
    print_endline ("New mode: " ++ Mode.show(newMode));

    if (newMode != prevMode) {
        List.iter((f) => f(newMode), Globals.modeChangedListeners^);
    }
};

let command = v => Native.vimCommand(v);
