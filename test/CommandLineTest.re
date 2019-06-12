open TestFramework;
open Vim;

let reset = () => Helpers.resetBuffer("test/testfile.txt");

describe("CommandLine", ({describe, _}) => {
  describe("getCompletions", ({test, _}) =>
    test("simple command line", ({expect}) => {
      let _ = reset();
      input(":");
      input("e");

      let completions = CommandLine.getCompletions();

      expect.int(Array.length(completions)).toBe(20);

      input(" ");
      input("s");

      let completions = CommandLine.getCompletions();
      expect.int(Array.length(completions)).toBe(1);

      if (Sys.win32) {
        expect.string(completions[0]).toEqual("src\\");
      } else {
        expect.string(completions[0]).toEqual("src/");
      };
    })
  );
  describe("getType", ({test, _}) =>
    test("simple command line", ({expect}) => {
      let _ = reset();
      input(":");
      expect.bool(CommandLine.getType() == CommandLine.Ex).toBe(true);
      input("<esc>");

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
  );

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
