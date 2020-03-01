open EditorCoreTypes;
open Vim;
open TestFramework;

let resetBuffer = () => Helpers.resetBuffer("test/testfile.txt");

describe("Mode", ({describe, _}) => {
  describe("replace mode", ({test, _}) => {
    test("replace mode is reported correctly", ({expect}) => {
      let _ = resetBuffer();

      // Enter replace mode
      let _ = Vim.input("R");

      expect.equal(Vim.Mode.getCurrent(), Vim.Types.Replace);
    })
  })
});
