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

let create = (~lines, ~yankTypeInt, ()) =>  {

  /* NOTE: This must be kept in sync with the definitions of MBLOCK/MLINE/MCHAR
     in vim.h
   */
  let yankType = switch(yankTypeInt) {
  | 0 => MCHAR
  | 1 => MLINE
  | 2 => MBLOCK
  | _ => MLINE
  };

{
  lines,
  yankType,
  operator: Yank,
  register: 'a',
  startLine: 1,
  endLine: 2,
  startColumn: 1,
  endColumn: 1,
};

};
