module Buffer = Buffer;
module BufferUpdate = BufferUpdate;
module Mode = Mode;

let _onBufferChanged = (buffer: Buffer.t, startLine: int, endLine: int, xtra: int) => {

    let bufferId = Buffer.getId(buffer);
    Printf.printf("Buffer changed - id: %d startLine: %d endLine: %d xtra: %d\n", bufferId, startLine, endLine, xtra);
}

let init = () => {
  Callback.register("lv_onBufferChanged", _onBufferChanged);

  Native.vimInit();

  Event.dispatch(Mode.getCurrent(), Globals.modeChangedListeners);
};

let input = v => {
    let prevMode = Mode.getCurrent();
    print_endline ("Previous mode: " ++ Mode.show(prevMode));

    Native.vimInput(v);

    let newMode = Mode.getCurrent();
    print_endline ("New mode: " ++ Mode.show(newMode));

    if (newMode != prevMode) {
        Event.dispatch(newMode, Globals.modeChangedListeners);
    }
};

let command = v => Native.vimCommand(v);
