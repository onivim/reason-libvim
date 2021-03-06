type t = {
  autoClosingPairs: AutoClosingPairs.t,
  bufferId: int,
  width: int,
  height: int,
  leftColumn: int,
  topLine: int,
  cursors: list(Cursor.t),
  lineComment: option(string),
  tabSize: int,
  insertSpaces: bool,
};

let current = () => {
  autoClosingPairs: AutoClosingPairs.empty,
  bufferId: Buffer.getCurrent() |> Buffer.getId,
  width: Window.getWidth(),
  height: Window.getHeight(),
  leftColumn: Window.getLeftColumn(),
  topLine: Window.getTopLine(),
  cursors: [Cursor.get()],
  tabSize: Options.getTabSize(),
  insertSpaces: Options.getInsertSpaces(),
  lineComment: None,
};
