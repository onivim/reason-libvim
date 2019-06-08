type t = Native.buffer;

let openFile = (filePath: string) => {
    print_endline("Start open....\n");
    Native.vimBufferOpen(filePath);
    print_endline("End open....\n");
}

let getFilename = (buffer: t) => {
    Native.vimBufferGetFilename(buffer);
}

let getId = (buffer: t) => {
    Native.vimBufferGetId(buffer);
}

let getById = (id: int) => {
    Native.vimBufferGetById(id);
};

let setCurrent = (buffer: t) => {
    Native.vimBufferSetCurrent(buffer);
};

let onUpdate = (f: Listeners.bufferUpdateListener) => {
    Event.add(f, Listeners.bufferUpdate);
};
