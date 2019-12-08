open EditorCoreTypes;
open Vim;
open TestFramework;

let resetBuffer = () => Helpers.resetBuffer("test/testfile.txt");

describe("Multi-cursor", ({describe, _}) => {
  describe("normal mode", ({describe, _}) => {
    describe("single cursor", ({test, _}) => {
      test("set cursor works as expected", ({expect}) => {
        let _ = resetBuffer();
        let cursors1 = Vim.input("j");

        cursors1
        |> List.hd
        |> (cursor => expect.int((cursor.line :> int)).toBe(2));

        expect.int((Cursor.getLine() :> int)).toBe(2);

        // set cursor, and move up
        let cursors2 =
          Vim.input(
            ~cursors=[
              Cursor.create(~line=Index.fromOneBased(3), ~column=Index.zero),
            ],
            "k",
          );

        cursors2
        |> List.hd
        |> (cursor => expect.int((cursor.line :> int)).toBe(2));

        expect.int((Cursor.getLine() :> int)).toBe(2);
      })
    })
  });
  describe("insert mode", ({test, _}) => {
    test("multi-cursor auto-closing paris", ({expect}) => {
      let buf = resetBuffer();

      Options.setAutoClosingPairs(true);
      AutoClosingPairs.create(
        AutoClosingPairs.[
          AutoClosingPair.create(~opening="{", ~closing="}", ()),
        ],
      )
      |> AutoClosingPairs.setPairs;

      let updates: ref(list(BufferUpdate.t)) = ref([]);
      let dispose = Buffer.onUpdate(upd => updates := [upd, ...updates^]);

      let _ = Vim.input("i");
      expect.int(List.length(updates^)).toBe(0);

      let cursors =
        Vim.input(
          ~cursors=[
            Cursor.create(~line=Index.zero, ~column=Index.zero),
            Cursor.create(~line=Index.(zero + 1), ~column=Index.zero),
            Cursor.create(~line=Index.(zero + 2), ~column=Index.zero),
          ],
          "{",
        );

      let line1 = Buffer.getLine(buf, Index.zero);
      let line2 = Buffer.getLine(buf, Index.(zero + 1));
      let line3 = Buffer.getLine(buf, Index.(zero + 2));

      expect.string(line1).toEqual(
        "{}This is the first line of a test file",
      );
      expect.string(line2).toEqual(
        "{}This is the second line of a test file",
      );
      expect.string(line3).toEqual(
        "{}This is the third line of a test file",
      );

      let _ = Vim.input(~cursors, "a");

      let line1 = Buffer.getLine(buf, Index.zero);
      let line2 = Buffer.getLine(buf, Index.(zero + 1));
      let line3 = Buffer.getLine(buf, Index.(zero + 2));

      expect.string(line1).toEqual(
        "{a}This is the first line of a test file",
      );
      expect.string(line2).toEqual(
        "{a}This is the second line of a test file",
      );
      expect.string(line3).toEqual(
        "{a}This is the third line of a test file",
      );

      dispose();
    });
    test("insert / backspace", ({expect}) => {
      let buf = resetBuffer();

      let updates: ref(list(BufferUpdate.t)) = ref([]);
      let dispose = Buffer.onUpdate(upd => updates := [upd, ...updates^]);

      let _ = Vim.input("i");
      expect.int(List.length(updates^)).toBe(0);

      let cursors =
        Vim.input(
          ~cursors=[
            Cursor.create(~line=Index.zero, ~column=Index.zero),
            Cursor.create(~line=Index.(zero + 1), ~column=Index.zero),
            Cursor.create(~line=Index.(zero + 2), ~column=Index.zero),
          ],
          "a",
        );

      let line1 = Buffer.getLine(buf, Index.zero);
      let line2 = Buffer.getLine(buf, Index.(zero + 1));
      let line3 = Buffer.getLine(buf, Index.(zero + 2));

      expect.string(line1).toEqual("aThis is the first line of a test file");
      expect.string(line2).toEqual(
        "aThis is the second line of a test file",
      );
      expect.string(line3).toEqual("aThis is the third line of a test file");

      let cursors = Vim.input(~cursors, "b");

      let line1 = Buffer.getLine(buf, Index.zero);
      let line2 = Buffer.getLine(buf, Index.(zero + 1));
      let line3 = Buffer.getLine(buf, Index.(zero + 2));

      expect.string(line1).toEqual(
        "abThis is the first line of a test file",
      );
      expect.string(line2).toEqual(
        "abThis is the second line of a test file",
      );
      expect.string(line3).toEqual(
        "abThis is the third line of a test file",
      );

      let _ = Vim.input(~cursors, "<bs>");

      let line1 = Buffer.getLine(buf, Index.zero);
      let line2 = Buffer.getLine(buf, Index.(zero + 1));
      let line3 = Buffer.getLine(buf, Index.(zero + 2));

      expect.string(line1).toEqual("aThis is the first line of a test file");
      expect.string(line2).toEqual(
        "aThis is the second line of a test file",
      );
      expect.string(line3).toEqual("aThis is the third line of a test file");

      dispose();
    });
  });
});
