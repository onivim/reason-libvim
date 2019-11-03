open TestFramework;
open Vim;

let resetBuffer = () => Helpers.resetBuffer("test/testfile.txt");

describe("Multi-cursor", ({describe, _}) => {
  describe("insert mode", ({test, _}) =>
    test("insert / backspace", ({expect}) => {
      let buf = resetBuffer();

      let updates: ref(list(BufferUpdate.t)) = ref([]);
      let dispose = Buffer.onUpdate(upd => updates := [upd, ...updates^]);

      let _ = Vim.input("i");
      expect.int(List.length(updates^)).toBe(0);

      let cursors =
        Vim.input(
          ~cursors=[
            Cursor.create(~line=1, ~column=0, ()),
            Cursor.create(~line=2, ~column=0, ()),
            Cursor.create(~line=3, ~column=0, ()),
          ],
          "a",
        );

      let line1 = Buffer.getLine(buf, 1);
      let line2 = Buffer.getLine(buf, 2);
      let line3 = Buffer.getLine(buf, 3);

      expect.string(line1).toEqual("aThis is the first line of a test file");
      expect.string(line2).toEqual(
        "aThis is the second line of a test file",
      );
      expect.string(line3).toEqual("aThis is the third line of a test file");

      let cursors = Vim.input(~cursors, "b");

      let line1 = Buffer.getLine(buf, 1);
      let line2 = Buffer.getLine(buf, 2);
      let line3 = Buffer.getLine(buf, 3);

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

      let line1 = Buffer.getLine(buf, 1);
      let line2 = Buffer.getLine(buf, 2);
      let line3 = Buffer.getLine(buf, 3);

      expect.string(line1).toEqual("aThis is the first line of a test file");
      expect.string(line2).toEqual(
        "aThis is the second line of a test file",
      );
      expect.string(line3).toEqual("aThis is the third line of a test file");

      dispose();
    })
  )
});
