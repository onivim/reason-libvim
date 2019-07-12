open TestFramework;
open Vim;

let resetBuffer = () => Helpers.resetBuffer("test/testfile.txt");

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
  describe("onEnter", ({test, _}) => {
    test(
      "saving to a new file should trigger a buffer enter event", ({expect}) => {
      let _ = resetBuffer();

      let updates: ref(list(Buffer.t)) = ref([]);
      let dispose = Buffer.onEnter(upd => updates := [upd, ...updates^]);

      let derp = s => switch(s) {
      | None => ""
      | Some(v) => v
      };

      let _ = onMessage((_, s, t) => print_endline ("MESSAGE: " ++ s ++ "|" ++ t));

      let previousFilename = Buffer.getCurrent() |> Buffer.getFilename |> derp;
      print_endline ("old id: " ++ previousFilename);
      command("sav some-new-file.txt");
      let newFileName = Buffer.getCurrent() |> Buffer.getFilename |> derp;
      print_endline ("new filename: " ++ newFileName);

      /*expect.bool(newMetadata.id == previousMetadata.id).toBe(false);*/

      expect.int(List.length(updates^)).toBe(1);

      dispose();
    });
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
