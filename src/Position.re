type t = {
  line: int,
  column: int,
};

let create = (~line, ~column) => {line, column};

let show = (v: t) => {
  Printf.sprintf("[Position.t] Line: %d Column: %d\n", v.line, v.column);
};
