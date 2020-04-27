open TestFramework;
open Vim;

let resetBuffer = () => Helpers.resetBuffer("test/testfile.txt");

describe("Goto", ({test, _}) => {
  test("gd", ({expect}) => {
    let _ = resetBuffer();

    let (_context, effects) = Vim.input("gd");

    let isExpectedEffect = fun
    | Vim.Effect.Goto({location, gotoType}) => {
      gotoType == Types.Definition 
    }
    | _ => false;

    expect.equal(effects |> Effect.matches(isExpectedEffect), true);
  });
  test("gD", ({expect}) => {
    let _ = resetBuffer();

    let (_context, effects) = Vim.input("gD");
    let isExpectedEffect = fun
    | Vim.Effect.Goto({location, gotoType}) => {
      gotoType == Types.Declaration 
    }
    | _ => false;

    expect.equal(effects |> Effect.matches(isExpectedEffect), true);
  });
});
