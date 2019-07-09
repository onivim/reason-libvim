open TestFramework;
open Vim;

let resetBuffer = () => Helpers.resetBuffer("test/testfile.txt");

describe("Stress", ({describe, _}) => {
  describe("Adding lots of lines", ({test, _}) => {
    test("Adding 1 million lines", ({expect}) => {

      let initialBuffer = resetBuffer();
      expect.int(Buffer.getLineCount(initialBuffer)).toBe(3);
      let buffer = Buffer.openFile("test/lines_100.txt");
      expect.int(Buffer.getLineCount(buffer)).toBe(100);

      // Now lets duplicate the buffer a bunch.
      input("yG");
      input("9999p");

      expect.int(Buffer.getLineCount(buffer)).toBe(1000000);
    });

    test("Repeat adding 1 million lines", ({expect}) => {

      let initialBuffer = resetBuffer();
      expect.int(Buffer.getLineCount(initialBuffer)).toBe(3);
      let buffer = Buffer.openFile("test/lines_100.txt");
      expect.int(Buffer.getLineCount(buffer)).toBe(100);

      // Now lets duplicate the buffer a bunch.
      input("yG");
      input("9999p");

      expect.int(Buffer.getLineCount(buffer)).toBe(1000000);
    });
  });
});
