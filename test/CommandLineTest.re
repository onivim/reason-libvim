open TestFramework;
open Vim;

let reset = () => Helpers.resetBuffer("test/testfile.txt");

describe("CommandLine", ({describe, _}) => {

  describe("ex", ({test, _}) => {
   test("substitution command", ({expect}) => {
      let buffer = reset();
      input(":");
      input("%");
      input("s");
      input("!");
      input("T");
      input("!");
      input("D");
      input("!");
      input("g");
      input("<cr>");

      expect.bool(Mode.getCurrent() == Normal).toBe(true);

      let line = Buffer.getLine(buffer, 3);
      expect.string(line).toEqual("Dhis is the third line of a test file");
   });

   test("move to line", ({expect}) => {
      let _ = reset();
      input(":");
      input("3");
      input("<cr>");

      expect.int(Cursor.getLine()).toBe(3);
   });
  });

  describe("getType", ({test, _}) => {
    test("simple command line", ({expect}) => {
      let _ = reset();
      input(":");
      expect.bool(CommandLine.getType() == CommandLine.Ex).toBe(true);
      input("<c-c>");

      input("/");
      expect.bool(CommandLine.getType() == CommandLine.SearchForward).toBe(
        true,
      );
      input("<esc>");

      input("?");
      expect.bool(CommandLine.getType() == CommandLine.SearchReverse).toBe(
        true,
      );
      input("<esc>");
    })
  });

  describe("getText", ({test, _}) =>
    test("simple command line", ({expect}) => {
      let _ = reset();

      input(":");
      input("a");

      switch (CommandLine.getText()) {
      | Some(v) => expect.string(v).toEqual("a")
      | None => expect.int(0).toBe(1)
      };

      input("b");

      switch (CommandLine.getText()) {
      | Some(v) => expect.string(v).toEqual("ab")
      | None => expect.int(0).toBe(1)
      };

      input("c");

      switch (CommandLine.getText()) {
      | Some(v) => expect.string(v).toEqual("abc")
      | None => expect.int(0).toBe(1)
      };
    })
  );

  describe("getPosition", ({test, _}) =>
    test("simple command line", ({expect}) => {
      let _ = reset();
      input(":");
      expect.int(CommandLine.getPosition()).toBe(0);

      input("a");
      expect.int(CommandLine.getPosition()).toBe(1);

      input("b");
      expect.int(CommandLine.getPosition()).toBe(2);

      input("c");
      expect.int(CommandLine.getPosition()).toBe(3);
    })
  );
});
