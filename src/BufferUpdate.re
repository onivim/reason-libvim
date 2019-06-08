type t = {
    id: int,
    startLine: int,
    endLine: int,
    lines: array(string),
    version: int,
};

let create = (
    ~buffer: Native.buffer,
    ~startLine,
    ~endLine,
    ~extra: int,
) => {
    id: 0,
    startLine,
    endLine,
    lines: [||],
    version: 0,
};
