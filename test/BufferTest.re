open TestFramework;
open Vim;

describe("Buffer", ({describe, _}) => {
  describe("getLine", ({test, _}) =>
    test("single file", ({expect}) => {
      let buffer = Buffer.openFile("test/testfile.txt");
      let line = Buffer.getLine(buffer, 1);
      expect.string(line).toEqual("This is the first line of a test file");
    })
  );
  describe("getLineCount", ({test, _}) =>
    test("single file", ({expect}) => {
      let buffer = Buffer.openFile("test/testfile.txt");
      expect.int(Buffer.getLineCount(buffer)).toBe(3);
    })
  );
});
