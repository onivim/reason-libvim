type t = Types.buffer;

let openFile = (filePath: string) => {
    Native.vimBufferOpen(filePath);
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

let onUpdate = (f: Types.bufferUpdateListener) => {
    Event.add(f, Globals.bufferUpdateListeners);
};
