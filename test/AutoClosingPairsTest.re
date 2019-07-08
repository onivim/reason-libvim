open TestFramework;
open Vim;

let resetBuffer = () => Helpers.resetBuffer("test/testfile.txt");

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

      let line = Buffer.getLine(b, 1);
      expect.string(line).toEqual("[(\"{");
    });
});
