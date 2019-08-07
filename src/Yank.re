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

