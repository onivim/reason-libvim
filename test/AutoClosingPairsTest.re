open EditorCoreTypes;
open Vim;
open TestFramework;

let resetBuffer = () => Helpers.resetBuffer("test/testfile.txt");
let input = s => ignore(Vim.input(s));

describe("AutoClosingPairs", ({test, _}) => {
  test("get / set acp options", ({expect}) => {
    let _ = resetBuffer();

    Options.setAutoClosingPairs(true);
    expect.bool(Options.getAutoClosingPairs()).toBe(true);

    Options.setAutoClosingPairs(false);
    expect.bool(Options.getAutoClosingPairs()).toBe(false);

    Options.setAutoClosingPairs(true);
    expect.bool(Options.getAutoClosingPairs()).toBe(true);
  });

  test("acp disabled", ({expect}) => {
    let b = resetBuffer();

    Options.setAutoClosingPairs(false);

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
    Options.setAutoClosingPairs(true);
    AutoClosingPairs.create(
      AutoClosingPairs.[
        AutoClosingPair.create(~opening="[", ~closing="]", ()),
      ],
    )
    |> AutoClosingPairs.setPairs;

    input("O");
    input("[");
    input("(");
    input("\"");
    input("{");

    let line = Buffer.getLine(b, Index.zero);
    expect.string(line).toEqual("[(\"{]");
  });

  test("isBetweenPairs", ({expect}) => {
    let b = resetBuffer();
    Options.setAutoClosingPairs(true);
    AutoClosingPairs.create(
      AutoClosingPairs.[
        AutoClosingPair.create(~opening="[", ~closing="]", ()),
      ],
    )
    |> AutoClosingPairs.setPairs;

    input("O");
    input("[");

    let line = Buffer.getLine(b, Index.zero);
    let location = Cursor.getLocation();
    expect.bool(AutoClosingPairs.isBetweenPairs(line, location.column)).toBe(
      true,
    );
  });

  test("backspace between pairs", ({expect}) => {
    let b = resetBuffer();
    Options.setAutoClosingPairs(true);
    AutoClosingPairs.create(
      AutoClosingPairs.[
        AutoClosingPair.create(~opening="[", ~closing="]", ()),
      ],
    )
    |> AutoClosingPairs.setPairs;

    input("O");
    input("[");
    input("<BS>");

    let line = Buffer.getLine(b, Index.zero);
    expect.string(line).toEqual("");
  });

  test("pass-through between pairs", ({expect}) => {
    let b = resetBuffer();
    Options.setAutoClosingPairs(true);
    AutoClosingPairs.create(
      AutoClosingPairs.[
        AutoClosingPair.create(~opening="[", ~closing="]", ()),
      ],
    )
    |> AutoClosingPairs.setPairs;

    input("O");
    input("[");
    input("]");

    let line = Buffer.getLine(b, Index.zero);
    let location = Cursor.getLocation();
    expect.string(line).toEqual("[]");
    expect.int((location.column :> int)).toBe(2);
  });

  test("pass-through not between pairs", ({expect}) => {
    let b = resetBuffer();
    Options.setAutoClosingPairs(true);
    AutoClosingPairs.create(
      AutoClosingPairs.[
        AutoClosingPair.create(~opening="(", ~closing=")", ()),
      ],
    )
    |> AutoClosingPairs.setPairs;

    input("O");
    input("(");
    input("x");
    input(")");

    let line = Buffer.getLine(b, Index.zero);
    let location = Cursor.getLocation();
    expect.string(line).toEqual("(x)");
    expect.int((location.column :> int)).toBe(3);
  });

  test(
    "pass-through not between pairs, with same begin/end pair", ({expect}) => {
    let b = resetBuffer();
    Options.setAutoClosingPairs(true);
    let quote = {|"|};
    AutoClosingPairs.create(
      AutoClosingPairs.[
        AutoClosingPair.create(~opening=quote, ~closing=quote, ()),
      ],
    )
    |> AutoClosingPairs.setPairs;

    input("O");
    input(quote);
    input("x");
    input(quote);

    let line = Buffer.getLine(b, Index.zero);
    let location = Cursor.getLocation();
    expect.string(line).toEqual({|"x"|});
    expect.int((location.column :> int)).toBe(3);
  });

  test("can insert closing pair", ({expect}) => {
    let b = resetBuffer();
    Options.setAutoClosingPairs(true);
    AutoClosingPairs.create(
      AutoClosingPairs.[
        AutoClosingPair.create(~opening="[", ~closing="]", ()),
      ],
    )
    |> AutoClosingPairs.setPairs;

    input("O");
    input("]");

    let line = Buffer.getLine(b, Index.zero);
    expect.string(line).toEqual("]");
  });

  test("multiple acp", ({expect}) => {
    let b = resetBuffer();
    Options.setAutoClosingPairs(true);
    AutoClosingPairs.create(
      AutoClosingPairs.[
        AutoClosingPair.create(~opening="[", ~closing="]", ()),
        AutoClosingPair.create(~opening="{", ~closing="}", ()),
        AutoClosingPair.create(~opening="(", ~closing=")", ()),
        AutoClosingPair.create(~opening="\"", ~closing="\"", ()),
      ],
    )
    |> AutoClosingPairs.setPairs;

    input("O");
    input("[");
    input("(");
    input("\"");
    input("{");

    let line = Buffer.getLine(b, Index.zero);
    expect.string(line).toEqual("[(\"{}\")]");
  });
});
