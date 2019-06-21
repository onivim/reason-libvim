open TestFramework;
open Vim;

let resetBuffer = () => Helpers.resetBuffer("test/testfile.txt");

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
  });
});
