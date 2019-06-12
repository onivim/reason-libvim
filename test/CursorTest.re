open TestFramework;
open Vim;

let resetBuffer = () => Helpers.resetBuffer("test/testfile.txt");

describe("Cursor", ({describe, _}) =>
  describe("normal mode", ({test, _}) => {
    test("j / k", ({expect}) => {
      let _ = resetBuffer();

      let cursorMoves: ref(list(Position.t)) = ref([]);
      let dispose = Cursor.onMoved(p => cursorMoves := [p, ...cursorMoves^]);

      expect.int(Cursor.getLine()).toBe(1);
      expect.int(Cursor.getColumn()).toBe(0);
      expect.int(List.length(cursorMoves^)).toBe(0);

      input("j");

      expect.int(Cursor.getLine()).toBe(2);
      expect.int(Cursor.getColumn()).toBe(0);
      expect.int(List.length(cursorMoves^)).toBe(1);

      input("j");

      expect.int(Cursor.getLine()).toBe(3);
      expect.int(Cursor.getColumn()).toBe(0);
      expect.int(List.length(cursorMoves^)).toBe(2);

      input("2");
      input("k");

      expect.int(Cursor.getLine()).toBe(1);
      expect.int(Cursor.getColumn()).toBe(0);
      expect.int(List.length(cursorMoves^)).toBe(3);

      dispose();
    });

    test("gg / G", ({expect}) => {
      let _ = resetBuffer();

      let cursorMoves: ref(list(Position.t)) = ref([]);
      let dispose = Cursor.onMoved(p => cursorMoves := [p, ...cursorMoves^]);

      expect.int(Cursor.getLine()).toBe(1);
      expect.int(Cursor.getColumn()).toBe(0);
      expect.int(List.length(cursorMoves^)).toBe(0);

      input("G");

      expect.int(Cursor.getLine()).toBe(3);
      expect.int(Cursor.getColumn()).toBe(0);
      expect.int(List.length(cursorMoves^)).toBe(1);

      input("g");
      input("g");

      expect.int(Cursor.getLine()).toBe(1);
      expect.int(Cursor.getColumn()).toBe(0);
      expect.int(List.length(cursorMoves^)).toBe(2);

      dispose();
    });
  })
);
