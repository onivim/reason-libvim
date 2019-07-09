open TestFramework;
open Vim;

let resetBuffer = () => Helpers.resetBuffer("test/testfile.txt");

describe("Stress", ({describe, _}) =>
  describe("Stress with undo lots of lines", ({test, _}) => {
    test("Add and remove 1 million lines", ({expect}) => {
      let initialBuffer = resetBuffer();
      expect.int(Buffer.getLineCount(initialBuffer)).toBe(3);
      let buffer = Buffer.openFile("test/lines_100.txt");
      expect.int(Buffer.getLineCount(buffer)).toBe(100);

      // Now lets duplicate the buffer a bunch.
      input("yG");
      input("9999p");

      expect.int(Buffer.getLineCount(buffer)).toBe(1000000);

      input("u");

      expect.int(Buffer.getLineCount(buffer)).toBe(100);
    });

    test("Add and remove 3 million lines", ({expect}) => {
      let initialBuffer = resetBuffer();
      expect.int(Buffer.getLineCount(initialBuffer)).toBe(3);
      let buffer = Buffer.openFile("test/lines_100.txt");
      expect.int(Buffer.getLineCount(buffer)).toBe(100);

      // Now lets duplicate the buffer a bunch.
      input("yG");
      input("29999p");

      expect.int(Buffer.getLineCount(buffer)).toBe(3000000);

      input("u");

      expect.int(Buffer.getLineCount(buffer)).toBe(100);
    });

    test("Add and remove 6 million lines", ({expect}) => {
      let initialBuffer = resetBuffer();
      expect.int(Buffer.getLineCount(initialBuffer)).toBe(3);
      let buffer = Buffer.openFile("test/lines_100.txt");
      expect.int(Buffer.getLineCount(buffer)).toBe(100);

      // Now lets duplicate the buffer a bunch.
      input("yG");
      input("59999p");

      expect.int(Buffer.getLineCount(buffer)).toBe(6000000);

      input("u");

      expect.int(Buffer.getLineCount(buffer)).toBe(100);
    });

    // test("Add and remove 10 million lines", ({expect}) => {
    //   let initialBuffer = resetBuffer();
    //   expect.int(Buffer.getLineCount(initialBuffer)).toBe(3);
    //   let buffer = Buffer.openFile("test/lines_100.txt");
    //   expect.int(Buffer.getLineCount(buffer)).toBe(100);

    //   // Now lets duplicate the buffer a bunch.
    //   input("yG");
    //   input("99999p");

    //   expect.int(Buffer.getLineCount(buffer)).toBe(10000000);

    //   input("u");

    //   expect.int(Buffer.getLineCount(buffer)).toBe(100);
    // });
  })
);