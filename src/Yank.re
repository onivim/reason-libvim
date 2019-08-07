type yankType =
| Block
| Line
| Char;

type yankOperator =
| Delete
| Yank;

type t = {
  lines: array(string),
  yankType: yankType,
  register: char,
  operator: yankOperator,
  startLine: int,
  startColumn: int,
  endLine: int,
  endColumn: int,
};

let create = (~lines,()) => {
  lines,
  yankType: Line,
  operator: Yank,
  register: 'a',
  startLine: 1,
  endLine: 2,
  startColumn: 1,
  endColumn: 1,
};

