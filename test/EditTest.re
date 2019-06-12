open TestFramework;
open Vim;

describe("Edit", ({describe, _}) => {
  describe("insert mode", ({test, _}) =>
    test("getLine reflects inserted text", ({expect}) => {
      let buffer = Buffer.openFile("test/testfile.txt");
      command("e!");
      
      input("I");
      let line = Buffer.getLine(buffer, 1);
      expect.string(line).toEqual("This is the first line of a test file");

      input("a");
      let line = Buffer.getLine(buffer, 1);
      expect.string(line).toEqual("aThis is the first line of a test file");

      input("b");
      let line = Buffer.getLine(buffer, 1);
      expect.string(line).toEqual("abThis is the first line of a test file");

      input("c");
      let line = Buffer.getLine(buffer, 1);
      expect.string(line).toEqual("abcThis is the first line of a test file");

      input("<cr>");
      let line = Buffer.getLine(buffer, 1);
      expect.string(line).toEqual("abc");
    })
  );
});
