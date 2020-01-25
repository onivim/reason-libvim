open EditorCoreTypes;
open Vim;
open TestFramework;

let resetBuffer = () => Helpers.resetBuffer("test/testfile.txt");
let input = (~autoClosingPairs=AutoClosingPairs.empty, s) =>
  ignore(Vim.input(~autoClosingPairs, s));

describe("AutoClosingPairs", ({test, _}) => {
  test("no auto-closing pairs", ({expect}) => {
    let b = resetBuffer();

    input("O");
    input("[");
    input("(");
    input("\"");
    input("{");

    let line = Buffer.getLine(b, Index.zero);
    expect.string(line).toEqual("[(\"{");
  });

  test("single acp", ({expect}) => {
    let b = resetBuffer();
    let autoClosingPairs =
      AutoClosingPairs.create(
        AutoClosingPairs.[
          AutoClosingPair.create(~opening="[", ~closing="]", ()),
        ],
      );

    input(~autoClosingPairs, "O");
    input(~autoClosingPairs, "[");
    input(~autoClosingPairs, "(");
    input(~autoClosingPairs, "\"");
    input(~autoClosingPairs, "{");

    let line = Buffer.getLine(b, Index.zero);
    expect.string(line).toEqual("[(\"{]");
  });

  test("isBetweenPairs", ({expect}) => {
    let b = resetBuffer();
    let autoClosingPairs =
      AutoClosingPairs.create(
        AutoClosingPairs.[
          AutoClosingPair.create(~opening="[", ~closing="]", ()),
        ],
      );

    input(~autoClosingPairs, "O");
    input(~autoClosingPairs, "[");

    let line = Buffer.getLine(b, Index.zero);
    let location = Cursor.getLocation();
    expect.bool(
      AutoClosingPairs.isBetweenPairs(
        line,
        location.column,
        autoClosingPairs,
      ),
    ).
      toBe(
      true,
    );
  });

  test("backspace between pairs", ({expect}) => {
    let b = resetBuffer();

    let autoClosingPairs =
      AutoClosingPairs.create(
        AutoClosingPairs.[
          AutoClosingPair.create(~opening="[", ~closing="]", ()),
        ],
      );

    input(~autoClosingPairs, "O");
    input(~autoClosingPairs, "[");
    input(~autoClosingPairs, "<BS>");

    let line = Buffer.getLine(b, Index.zero);
    expect.string(line).toEqual("");
  });

  test("pass-through between pairs", ({expect}) => {
    let b = resetBuffer();

    let autoClosingPairs =
      AutoClosingPairs.create(
        AutoClosingPairs.[
          AutoClosingPair.create(~opening="[", ~closing="]", ()),
        ],
      );

    input(~autoClosingPairs, "O");
    input(~autoClosingPairs, "[");
    input(~autoClosingPairs, "]");

    let line = Buffer.getLine(b, Index.zero);
    let location = Cursor.getLocation();
    expect.string(line).toEqual("[]");
    expect.int((location.column :> int)).toBe(2);
  });

  test("pass-through not between pairs", ({expect}) => {
    let b = resetBuffer();
    let autoClosingPairs =
      AutoClosingPairs.create(
        AutoClosingPairs.[
          AutoClosingPair.create(~opening="(", ~closing=")", ()),
        ],
      );

    input(~autoClosingPairs, "O");
    input(~autoClosingPairs, "(");
    input(~autoClosingPairs, "x");
    input(~autoClosingPairs, ")");

    let line = Buffer.getLine(b, Index.zero);
    let location = Cursor.getLocation();
    expect.string(line).toEqual("(x)");
    expect.int((location.column :> int)).toBe(3);
  });

  test(
    "pass-through not between pairs, with same begin/end pair", ({expect}) => {
    let b = resetBuffer();
    let quote = {|"|};
    let autoClosingPairs =
      AutoClosingPairs.create(
        AutoClosingPairs.[
          AutoClosingPair.create(~opening=quote, ~closing=quote, ()),
        ],
      );

    input(~autoClosingPairs, "O");
    input(~autoClosingPairs, quote);
    input(~autoClosingPairs, "x");
    input(~autoClosingPairs, quote);

    let line = Buffer.getLine(b, Index.zero);
    let location = Cursor.getLocation();
    expect.string(line).toEqual({|"x"|});
    expect.int((location.column :> int)).toBe(3);
  });

  test("can insert closing pair", ({expect}) => {
    let b = resetBuffer();
    let autoClosingPairs =
      AutoClosingPairs.create(
        AutoClosingPairs.[
          AutoClosingPair.create(~opening="[", ~closing="]", ()),
        ],
      );

    input(~autoClosingPairs, "O");
    input(~autoClosingPairs, "]");

    let line = Buffer.getLine(b, Index.zero);
    expect.string(line).toEqual("]");
  });

  test("multiple acp", ({expect}) => {
    let b = resetBuffer();
    let autoClosingPairs =
      AutoClosingPairs.create(
        ~allowBefore=["]", "}", ")", "\""],
        AutoClosingPairs.[
          AutoClosingPair.create(~opening="[", ~closing="]", ()),
          AutoClosingPair.create(~opening="{", ~closing="}", ()),
          AutoClosingPair.create(~opening="(", ~closing=")", ()),
          AutoClosingPair.create(~opening="\"", ~closing="\"", ()),
        ],
      );

    input(~autoClosingPairs, "O");
    input(~autoClosingPairs, "[");
    input(~autoClosingPairs, "(");
    input(~autoClosingPairs, "\"");
    input(~autoClosingPairs, "{");

    let line = Buffer.getLine(b, Index.zero);
    expect.string(line).toEqual("[(\"{}\")]");
  });
});
