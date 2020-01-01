open EditorCoreTypes;
open Vim;
open TestFramework;

let resetBuffer = () => Helpers.resetBuffer("test/testfile_swedish.txt");
let input = s => ignore(Vim.input(s));

describe("Cursor with Swedish characters", ({describe, _}) => {
  describe("normal mode", ({test, _}) => {
    test("l over a swedish character", ({expect}) => {
      let _ = resetBuffer();

      let cursorMoves: ref(list(Location.t)) = ref([]);
      let dispose = Cursor.onMoved(p => cursorMoves := [p, ...cursorMoves^]);

      expect.int((Cursor.getLine() :> int)).toBe(0);
      expect.int((Cursor.getColumn() :> int)).toBe(0);
      expect.int(List.length(cursorMoves^)).toBe(0);

      input("l");

      expect.int((Cursor.getLine() :> int)).toBe(0);
      expect.int((Cursor.getColumn() :> int)).toBe(1);

      dispose();
    });

    test("l without swedish characters", ({expect}) => {
      let _ = resetBuffer();

      let cursorMoves: ref(list(Location.t)) = ref([]);
      let dispose = Cursor.onMoved(p => cursorMoves := [p, ...cursorMoves^]);

      expect.int((Cursor.getLine() :> int)).toBe(0);
      expect.int((Cursor.getColumn() :> int)).toBe(0);
      expect.int(List.length(cursorMoves^)).toBe(0);

      input("j");
      input("l");

      expect.int((Cursor.getLine() :> int)).toBe(1);
      expect.int((Cursor.getColumn() :> int)).toBe(1);

      dispose();
    });
  });
});
