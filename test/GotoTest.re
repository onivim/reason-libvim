open TestFramework;
open Vim;

let resetBuffer = () => Helpers.resetBuffer("test/testfile.txt");

describe("Goto", ({test, _}) => {
  test("gd", ({expect, _}) => {
    let _ = resetBuffer();

    let (_context, effects) = Vim.input("gd");

    let isExpectedEffect =
      fun
      | Vim.Effect.Goto({gotoType, _}) => {
          gotoType == Types.Definition;
        }
      | _ => false;

    expect.equal(effects |> Effect.matches(isExpectedEffect), true);
  });
  test("gD", ({expect, _}) => {
    let _ = resetBuffer();

    let (_context, effects) = Vim.input("gD");
    let isExpectedEffect =
      fun
      | Vim.Effect.Goto({gotoType, _}) => {
          gotoType == Types.Declaration;
        }
      | _ => false;

    expect.equal(effects |> Effect.matches(isExpectedEffect), true);
  });
});
