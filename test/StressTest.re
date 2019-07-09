open TestFramework;
open Vim;

let resetBuffer = () => Helpers.resetBuffer("test/testfile.txt");

describe("Stress", ({describe, _}) => {
  describe("Adding lots of lines", ({test, _}) => {
    test("Adding 1 million lines", ({expect}) => {

      let _ = resetBuffer();
      let buffer = Buffer.openFile("test/lines_100.txt");

      // Now lets duplicate the buffer a bunch.
      input("yG");
      input("9999p");

      expect.int(Buffer.getLineCount(buffer)).toBe(1000000);
    });

    test("Repeat adding 1 million lines", ({expect}) => {

      let _ = resetBuffer();
      let buffer = Buffer.openFile("test/lines_100.txt");

      // Now lets duplicate the buffer a bunch.
      input("yG");
      input("9999p");

      expect.int(Buffer.getLineCount(buffer)).toBe(1000000);
    });

    // test("Undoing 1 million lines", ({expect}) => {

    //   let _ = resetBuffer();
    //   let buffer = Buffer.openFile("test/lines_100.txt")

    //   // Now lets duplicate the buffer a bunch.
    //   input("yG");
    //   input("9999p");

    //   expect.int(Buffer.getLineCount(buffer)).toBe(1000000);

    //   // Okay, now lets issue an undo and make sure we don't crash.
    //   input("u");

    //   expect.int(Buffer.getLineCount(buffer)).toBe(100);
    // });

    // test("Adding 10 million lines", ({expect}) => {

    //   let _ = resetBuffer();
    //   let buffer = Buffer.openFile("test/lines_100.txt");

    //   // Now lets duplicate the buffer a bunch.
    //   input("yG");
    //   input("99999p");

    //   expect.int(Buffer.getLineCount(buffer)).toBe(10000000);
    // });

    // test("Undoing 10 million lines", ({expect}) => {

    //   let _ = resetBuffer();
    //   let buffer = Buffer.openFile("test/lines_100.txt")

    //   // Now lets duplicate the buffer a bunch.
    //   input("yG");
    //   input("99999p");

    //   expect.int(Buffer.getLineCount(buffer)).toBe(10000000);

    //   // Okay, now lets issue an undo and make sure we don't crash.
    //   input("u");

    //   expect.int(Buffer.getLineCount(buffer)).toBe(100);
    // });
  });
});
