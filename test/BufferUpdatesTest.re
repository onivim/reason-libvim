open TestFramework;
open Vim;

let resetBuffer = () => Helpers.resetBuffer("test/testfile.txt");

describe("Buffer.onUpdate", ({describe, _}) => {
  describe("reloading", ({test, _}) =>
    test("make changes, and reload", ({expect}) => {
      let buffer = resetBuffer();

      input("I");
      input("a");
      input("b");
      input("c");

      let lastChangedTick = Buffer.getVersion(buffer);

      let updates: ref(list(BufferUpdate.t)) = ref([]);
      let dispose = Buffer.onUpdate(upd => updates := [upd, ...updates^]);

      input("<esc>");
      input(":");
      input("e");
      input("!");
      input("<cr>");

      expect.bool(Buffer.getVersion(buffer) > lastChangedTick).toBe(true);
      expect.int(List.length(updates^)).toBe(1);

      dispose();
    })
  );

  describe("normal mode", ({test, _}) => {
    test("add line", ({expect}) => {
      let _ = resetBuffer();

      let updates: ref(list(BufferUpdate.t)) = ref([]);
      let dispose = Buffer.onUpdate(upd => updates := [upd, ...updates^]);

      input("j");
      input("y");
      input("y");
      input("p");

      expect.int(List.length(updates^)).toBe(1);
      let bu: BufferUpdate.t = List.hd(updates^);

      expect.int(bu.startLine).toBe(3);
      expect.int(bu.endLine).toBe(3);
      expect.int(Array.length(bu.lines)).toBe(1);
      expect.string(bu.lines[0]).toEqual(
        "This is the second line of a test file",
      );

      dispose();
    });
    test("delete 2nd line", ({expect}) => {
      let _ = resetBuffer();

      let updates: ref(list(BufferUpdate.t)) = ref([]);
      let dispose = Buffer.onUpdate(upd => updates := [upd, ...updates^]);

      input("j");
      input("d");
      input("d");

      expect.int(List.length(updates^)).toBe(1);
      let bu: BufferUpdate.t = List.hd(updates^);

      expect.int(bu.startLine).toBe(2);
      expect.int(bu.endLine).toBe(3);
      expect.int(Array.length(bu.lines)).toBe(0);

      dispose();
    });
    test("delete all lines", ({expect}) => {
      let _ = resetBuffer();

      let updates: ref(list(BufferUpdate.t)) = ref([]);
      let dispose = Buffer.onUpdate(upd => updates := [upd, ...updates^]);

      input("dG");
      expect.int(List.length(updates^)).toBe(1);
      let bu: BufferUpdate.t = List.hd(updates^);

      expect.int(bu.startLine).toBe(1);
      expect.int(bu.endLine).toBe(4);
      expect.int(Array.length(bu.lines)).toBe(0);

      dispose();
    });
  });
  describe("insert mode", ({test, _}) =>
    test("single file", ({expect}) => {
      let _ = resetBuffer();

      let updates: ref(list(BufferUpdate.t)) = ref([]);
      let dispose = Buffer.onUpdate(upd => updates := [upd, ...updates^]);

      input("i");
      expect.int(List.length(updates^)).toBe(0);

      input("a");
      expect.int(List.length(updates^)).toBe(1);
      let bu: BufferUpdate.t = List.hd(updates^);

      expect.int(bu.startLine).toBe(1);
      expect.int(bu.endLine).toBe(2);
      expect.int(Array.length(bu.lines)).toBe(1);
      expect.string(bu.lines[0]).toEqual(
        "aThis is the first line of a test file",
      );

      input("b");
      expect.int(List.length(updates^)).toBe(2);
      let bu: BufferUpdate.t = List.hd(updates^);

      expect.int(bu.startLine).toBe(1);
      expect.int(bu.endLine).toBe(2);
      expect.int(Array.length(bu.lines)).toBe(1);
      expect.string(bu.lines[0]).toEqual(
        "abThis is the first line of a test file",
      );

      dispose();
    })
  );
});
