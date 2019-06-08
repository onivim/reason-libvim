module AutoCommands = AutoCommands;
module Buffer = Buffer;
module BufferMetadata = BufferMetadata;
module BufferUpdate = BufferUpdate;
module Cursor = Cursor;
module Mode = Mode;

let _onAutocommand = (autoCommand: Types.autocmd, buffer: Buffer.t) => {
    print_endline ("start autocmd handler...");
    let n = switch(Buffer.getFilename(buffer)) {
    | Some(v) => v
    | None => "[none]"
    };

    print_endline ("Name: " ++ n);
    print_endline ("AUTOCOMMAND: " ++ string_of_int(Obj.magic(autoCommand)));
    print_endline ("BUFFER: " ++ string_of_int(Obj.magic(buffer)));
    
    /* switch (autoCommand) { */
    /* | BufNew => { */
    /*     print_endline ("buf new start..."); */
    /*     Printf.printf("[AutoCommand] Buffer new: %d name: %s\n", Buffer.getId(buffer), "derp"); */
    /*     print_endline ("buf new end..."); */
    /* } */
    /* | BufEnter => Printf.printf("[AutoCommand] Buffer enter: %d name: %s\n", Buffer.getId(buffer), n); */
    /* | _ => (); */
    /* } */

    Event.dispatch2(autoCommand, buffer, Listeners.autocmd);

    Gc.full_major();
    print_endline ("end autocmd handler...");
};

let _onBufferChanged = (buffer: Buffer.t, startLine: int, endLine: int, xtra: int) => {

    let idx: ref(int) = ref(startLine);

    let max = endLine + xtra;
    let count = max - startLine;

    let lines = Array.make(count, "");


    while (idx^ < max) {
        let i = idx^;
        print_endline ("Getting line: " ++ string_of_int(i));
        let line = Native.vimBufferGetLine(buffer, i)
        print_endline ("Got line: " ++ string_of_int(i));
        print_endline ("Line: " ++ line);
        Array.set(lines, i - startLine, line);
        incr(idx);
    }

    let bufferId = Buffer.getId(buffer);
    Printf.printf("Buffer changed - id: %d startLine: %d endLine: %d xtra: %d\n", bufferId, startLine, endLine, xtra);
    print_endline ("---- lines changed: ");
    Array.iter(print_endline, lines);
    print_endline ("----");
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

    Gc.full_major();
    print_endline("--- Done with input ---");
};

let command = v => Native.vimCommand(v);
