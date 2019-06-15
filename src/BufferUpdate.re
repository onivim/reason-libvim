type t = {
  id: int,
  startLine: int,
  endLine: int,
  lines: array(string),
  version: int,
};

let createInitial = (buffer: Native.buffer) => {
  let id = Native.vimBufferGetId(buffer);
  let version = Native.vimBufferGetChangedTick(buffer) - 1;
  let startLine = 1;
  let endLine = Native.vimBufferGetLineCount(buffer) + 1;

  let idx = ref(startLine);
  let max = endLine;
  let count = endLine - startLine;
  let lines = Array.make(count, "");

  while (idx^ < max) {
    let i = idx^;
    let line = Native.vimBufferGetLine(buffer, i);
    lines[i - startLine] = line;
    incr(idx);
  };

  {id, startLine, endLine, lines, version };
};

let create = (~buffer: Native.buffer, ~startLine, ~endLine, ~extra: int) => {
  let id = Native.vimBufferGetId(buffer);
  let version = Native.vimBufferGetChangedTick(buffer);
  let idx: ref(int) = ref(startLine);

  let max = endLine + extra;
  let count = max - startLine;

  let lines = Array.make(count, "");

  while (idx^ < max) {
    let i = idx^;
    let line = Native.vimBufferGetLine(buffer, i);
    lines[i - startLine] = line;
    incr(idx);
  };

  {id, startLine, endLine, lines, version };
};
