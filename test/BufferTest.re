open TestFramework;
open Vim;

let resetBuffer = () => Helpers.resetBuffer("test/testfile.txt");
let input = (s) => ignore(Vim.input(s));

describe("Buffer", ({describe, _}) => {
  describe("getLine", ({test, _}) =>
    test("single file", ({expect}) => {
      let _ = resetBuffer();
      let buffer = Buffer.openFile("test/testfile.txt");
      let line = Buffer.getLine(buffer, 1);
      expect.string(line).toEqual("This is the first line of a test file");
    })
  );
  describe("getLineCount", ({test, _}) =>
    test("single file", ({expect}) => {
      let _ = resetBuffer();
      let buffer = Buffer.openFile("test/testfile.txt");
      expect.int(Buffer.getLineCount(buffer)).toBe(3);
    })
  );
  describe("onModifiedChanged", ({test, _}) =>
    test(
      "switching to a new file should not trigger an onFilenameChanged event",
      ({expect}) => {
      let _ = resetBuffer();

      let modifyVals = ref([]);
      let dispose =
        Buffer.onModifiedChanged((_id, modified) =>
          modifyVals := [modified, ...modifyVals^]
        );

      input("i");

      // Switching modes shouldn't trigger a modified change..
      expect.int(List.length(modifyVals^)).toBe(0);

      // Typing a character should trigger a modified change...
      input("a");

      expect.int(List.length(modifyVals^)).toBe(1);
      expect.bool(List.hd(modifyVals^)).toBe(true);

      // Switching mode shouldn't trigger a modified change
      input("<esc>");
      expect.int(List.length(modifyVals^)).toBe(1);

      // ..undo should, though
      input("u");
      expect.int(List.length(modifyVals^)).toBe(2);
      expect.bool(List.hd(modifyVals^)).toBe(false);

      // ..and redo
      input("<c-r>");
      expect.int(List.length(modifyVals^)).toBe(3);
      expect.bool(List.hd(modifyVals^)).toBe(true);

      dispose();
    })
  );
  describe("onFilenameChanged", ({test, _}) => {
    test(
      "switching to a new file should not trigger an onFilenameChanged event",
      ({expect}) => {
      let _ = resetBuffer();

      let updates = ref([]);
      let onEnter = ref([]);
      let dispose =
        Buffer.onFilenameChanged(meta => updates := [meta, ...updates^]);
      let dispose2 = Buffer.onEnter(v => onEnter := [v, ...onEnter^]);

      command("e! some-new-file.txt");

      /* A filename changed event should not have been triggered */
      expect.int(List.length(updates^)).toBe(0);
      /* An enter event should've been triggered */
      expect.int(List.length(onEnter^)).toBe(1);

      dispose();
      dispose2();
    });
    test(
      "saving to a new file should trigger an onFilenameChanged event",
      ({expect}) => {
      let _ = resetBuffer();

      let updates = ref([]);
      let onEnter = ref([]);
      let dispose =
        Buffer.onFilenameChanged(meta => updates := [meta, ...updates^]);
      let dispose2 = Buffer.onEnter(v => onEnter := [v, ...onEnter^]);

      let string_opt = s =>
        switch (s) {
        | None => ""
        | Some(v) => v
        };

      let previousFilename =
        Buffer.getCurrent() |> Buffer.getFilename |> string_opt;
      command("sav! some-new-file-2.txt");
      let newFilename =
        Buffer.getCurrent() |> Buffer.getFilename |> string_opt;

      expect.bool(String.equal(previousFilename, newFilename)).toBe(false);

      expect.int(List.length(updates^)).toBe(1);

      /* A buffer enter event should not have been triggered */
      expect.int(List.length(onEnter^)).toBe(0);

      dispose();
      dispose2();
    });
  });
  describe("onFiletypeChanged", ({test, _}) => {
    test("switching filetype should trigger event", ({expect}) => {
      let _ = resetBuffer();

      let updates = ref([]);
      let dispose =
        Buffer.onFiletypeChanged(meta =>
          updates := [meta.fileType, ...updates^]
        );

      command("set filetype=derp");

      /* A filename changed event should not have been triggered */
      expect.int(List.length(updates^)).toBe(1);
      /* An enter event should've been triggered */
      expect.bool(List.hd(updates^) == Some("derp")).toBe(true);

      dispose();
    })
  });
  describe("onEnter", ({test, _}) => {
    test(
      "editing a new file should trigger a buffer enter event", ({expect}) => {
      let _ = resetBuffer();

      let updates: ref(list(Buffer.t)) = ref([]);
      let dispose = Buffer.onEnter(upd => updates := [upd, ...updates^]);

      let _ = Buffer.openFile("test/lines_100.txt");

      expect.int(List.length(updates^)).toBe(1);

      dispose();
    });

    test(
      "editing a new file via ':e' should trigger a buffer enter event",
      ({expect}) => {
      let _ = resetBuffer();

      let updates: ref(list(Buffer.t)) = ref([]);
      let dispose = Buffer.onEnter(upd => updates := [upd, ...updates^]);

      let _ = Buffer.openFile("test/some_random_file.txt");

      expect.int(List.length(updates^)).toBe(1);
      dispose();
    });

    test(
      "jumping to a previous file via '<c-o>' should trigger buffer enter",
      ({expect}) => {
      let _ = resetBuffer();

      let buf1 = Buffer.openFile("test/lines_100.txt");

      let buf2 = Buffer.openFile("test/some_random_file.txt");

      let updates: ref(list(Buffer.t)) = ref([]);
      let dispose = Buffer.onEnter(upd => updates := [upd, ...updates^]);

      expect.bool(Buffer.getCurrent() == buf2).toBe(true);

      input("<c-o>");
      expect.bool(Buffer.getCurrent() == buf1).toBe(true);
      expect.int(List.length(updates^)).toBe(1);
      expect.bool(List.hd(updates^) == buf1).toBe(true);

      input("<c-i>");
      expect.bool(Buffer.getCurrent() == buf2).toBe(true);
      expect.int(List.length(updates^)).toBe(2);
      expect.bool(List.hd(updates^) == buf2).toBe(true);

      dispose();
    });
  });
});
