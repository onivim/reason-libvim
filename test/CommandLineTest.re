open TestFramework;
open Vim;

let reset = () => Helpers.resetBuffer("test/testfile.txt");

describe("CommandLine", ({describe, _}) => {
  describe("getType", ({test, _}) =>
    test("simple command line", ({expect}) => {
      let _ = reset();
      input(":");
      expect.bool(CommandLine.getType() == Types.Ex).toBe(true);
      input("<esc>");

      input("/");
      expect.bool(CommandLine.getType() == Types.SearchForward).toBe(true);
      input("<esc>");

      input("?");
      expect.bool(CommandLine.getType() == Types.SearchReverse).toBe(true);
      input("<esc>");
    })
  );

  describe("listeners", ({test, _}) => {
    test("enter / leave listeners", ({expect}) => {
      open Vim.Types;
      let _ = reset();
      let enterEvents: ref(list(CommandLine.t)) = ref([]);
      let getEnterCount = () => List.length(enterEvents^);
      let leaveCount = ref(0);

      let expectCommandLineType = m =>
        expect.bool(m == List.hd(enterEvents^).cmdType).toBe(true);
      let expectCommandLineText = v =>
        expect.bool(String.equal(v, List.hd(enterEvents^).text)).toBe(
          true,
        );

      let dispose1 =
        CommandLine.onEnter(c => enterEvents := [c, ...enterEvents^]);
      let dispose2 = CommandLine.onLeave(_ => incr(leaveCount));

      input(":");
      expect.int(getEnterCount()).toBe(1);
      expect.int(leaveCount^).toBe(0);
      expectCommandLineType(Ex);
      expectCommandLineText("");
      input("a");
      expect.int(getEnterCount()).toBe(1);
      expect.int(leaveCount^).toBe(0);
      input("<esc>");
      expect.int(getEnterCount()).toBe(1);
      expect.int(leaveCount^).toBe(1);

      input("/");
      expectCommandLineType(SearchForward);
      expectCommandLineText("");
      expect.int(getEnterCount()).toBe(2);
      expect.int(leaveCount^).toBe(1);
      input("b");
      expect.int(getEnterCount()).toBe(2);
      expect.int(leaveCount^).toBe(1);
      input("<esc>");
      expect.int(getEnterCount()).toBe(2);
      expect.int(leaveCount^).toBe(2);

      dispose1();
      dispose2();
    });

    test("update listener", ({expect}) => {
      open Vim.Types;
      let _ = reset();

      let updateEvents: ref(list(CommandLine.t)) = ref([]);

      let getUpdateCount = () => List.length(updateEvents^);
      let expectCommandLineText = v =>
        expect.bool(String.equal(v, List.hd(updateEvents^).text)).toBe(
          true,
        );
      let expectCommandLinePosition = v =>
        expect.bool(v == List.hd(updateEvents^).position).toBe(true);

      let dispose1 =
        CommandLine.onUpdate(c => updateEvents := [c, ...updateEvents^]);

      input(":");
      input("a");
      expect.int(getUpdateCount()).toBe(1);
      expectCommandLineText("a");
      expectCommandLinePosition(1);
      input("b");
      expectCommandLineText("ab");
      expectCommandLinePosition(2);
      input("<c-h>");
      expectCommandLineText("a");
      expectCommandLinePosition(1);

      input("<esc>");
      dispose1();
    });
  });

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
