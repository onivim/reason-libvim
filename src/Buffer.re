type t= Types.buffer;

let openFile = (filePath: string) => {
    Native.vimBufferOpen(filePath);
}

let getById = (id: int) => {
    Native.vimBufferGetById(id);
};

let setCurrent = (buffer: t) => {
    Native.vimBufferSetCurrent(buffer);
};

let onUpdate = (f: bufferUpdateListener) => {
    Global.bufferUpdateListeners := [f, ...Global.bufferUpdateListeners^];

    () => {
        List.filter((g) => g !== f, Global.bufferUpdateListeners^);
    };
};

let test = () => print_endline("Hello, world");
