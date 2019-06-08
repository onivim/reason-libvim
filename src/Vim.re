module Buffer = Buffer;
module BufferUpdate = BufferUpdate;
module Cursor = Cursor;
module Mode = Mode;

let _onAutocommand = (autoCommand: Types.autocmd, buffer: Buffer.t) => {
    print_endline ("start autocmd handler...");
    let n = switch(Buffer.getFilename(buffer)) {
    | Some(v) => v
    | None => "[none]"
    };

    print_endline ("AUTOCOMMAND: " ++ string_of_int(Obj.magic(autoCommand)));
    
    switch (autoCommand) {
    | BufNew => {
        print_endline ("buf new start...");
        Printf.printf("[AutoCommand] Buffer new: %d name: %s\n", Buffer.getId(buffer), n);
        print_endline ("buf new end...");
    }
    | BufEnter => Printf.printf("[AutoCommand] Buffer enter: %d name: %s\n", Buffer.getId(buffer), n);
    | _ => ();
    }

    print_endline ("end autocmd handler...");
};

let _onBufferChanged = (buffer: Buffer.t, startLine: int, endLine: int, xtra: int) => {

    let bufferId = Buffer.getId(buffer);
    Printf.printf("Buffer changed - id: %d startLine: %d endLine: %d xtra: %d\n", bufferId, startLine, endLine, xtra);
}

let init = () => {
  Callback.register("lv_onBufferChanged", _onBufferChanged);
  Callback.register("lv_onAutocommand", _onAutocommand);

  Native.vimInit();

  Event.dispatch(Mode.getCurrent(), Listeners.modeChanged);
};

let input = v => {
    let prevMode = Mode.getCurrent();
    let prevPosition = Cursor.getPosition();
    print_endline ("Previous position: " ++ Position.show(prevPosition));
    print_endline ("Previous mode: " ++ Mode.show(prevMode));

    Native.vimInput(v);

    let newPosition = Cursor.getPosition();
    let newMode = Mode.getCurrent();
    print_endline ("New mode: " ++ Mode.show(newMode));
    print_endline ("New position: " ++ Position.show(newPosition));

    if (newMode != prevMode) {
        Event.dispatch(newMode, Listeners.modeChanged);
    }

    if (newPosition != prevPosition) {
        Event.dispatch(newPosition, Listeners.cursorMoved);
    }

    print_endline("--- Done with input ---");
};

let command = v => Native.vimCommand(v);
