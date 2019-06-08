module AutoCommands = AutoCommands;
module Buffer = Buffer;
module BufferMetadata = BufferMetadata;
module BufferUpdate = BufferUpdate;
module Cursor = Cursor;
module Mode = Mode;

type fn =  unit => unit;

let queuedFunctions: ref(list(fn)) = ref([]);

let queue = (f) => queuedFunctions := [f, ...queuedFunctions^];

let flushQueue = () => {
    List.iter((f) => f(), queuedFunctions^);
    queuedFunctions := [];
};

let checkAndUpdateState = (f) => {
    let prevMode = Mode.getCurrent();
    let prevPosition = Cursor.getPosition();
    print_endline ("Previous position: " ++ Position.show(prevPosition));
    print_endline ("Previous mode: " ++ Mode.show(prevMode));

    f();

    let newPosition = Cursor.getPosition();
    let newMode = Mode.getCurrent();
    print_endline ("New mode: " ++ Mode.show(newMode));
    print_endline ("New position: " ++ Position.show(newPosition));

    Buffer.checkCurrentBufferForUpdate();

    if (newMode != prevMode) {
        Event.dispatch(newMode, Listeners.modeChanged);
    }

    if (newPosition != prevPosition) {
        Event.dispatch(newPosition, Listeners.cursorMoved);
    }

    Gc.full_major();
    print_endline("--- Done with input ---");

    flushQueue();
}

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

let _onBufferChanged = (buffer: Buffer.t, startLine: int, endLine: int, extra: int) => {
    let update = BufferUpdate.create(buffer, startLine, endLine, extra);

    queue(() => Event.dispatch(update, Listeners.bufferUpdate));
}

let init = () => {
  Callback.register("lv_onBufferChanged", _onBufferChanged);
  Callback.register("lv_onAutocommand", _onAutocommand);

  Native.vimInit();

  Event.dispatch(Mode.getCurrent(), Listeners.modeChanged);
};

let input = v => {
    checkAndUpdateState(() => {
        Native.vimInput(v);
    });
};

let command = v => {
    checkAndUpdateState(() => {
        Native.vimCommand(v);
    });
};
