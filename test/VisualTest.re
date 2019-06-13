open TestFramework;
open Vim;

let resetBuffer = () => Helpers.resetBuffer("test/testfile.txt");

describe("Visual", ({describe, _}) => {
  describe("getRange", ({test, _}) =>
    test("simple range", ({expect}) => {
      let _ = resetBuffer();

      input("v");
      input("l");

      let range = Visual.getRange();
      expect.int(range.startPos.line).toBe(1);
      expect.int(range.startPos.column).toBe(0);
      expect.int(range.endPos.line).toBe(1);
      expect.int(range.endPos.column).toBe(1);

      input("3");
      input("l");

      let range = Visual.getRange();
      expect.int(range.startPos.line).toBe(1);
      expect.int(range.startPos.column).toBe(0);
      expect.int(range.endPos.line).toBe(1);
      expect.int(range.endPos.column).toBe(4);

      input("j");
      let range = Visual.getRange();
      expect.int(range.startPos.line).toBe(1);
      expect.int(range.startPos.column).toBe(0);
      expect.int(range.endPos.line).toBe(2);
      expect.int(range.endPos.column).toBe(4);
    })
  );

  describe("onRangeChanged", ({test, _}) =>
    test("dispatches on change", ({expect}) => {
      let _ = resetBuffer();

      let rangeChanges: ref(list(Range.t)) = ref([]);
      let dispose =
        Visual.onRangeChanged(range =>
          rangeChanges := [range, ...rangeChanges^]
        );

      input("V");

      expect.int(List.length(rangeChanges^)).toBe(1);
      let r = List.hd(rangeChanges^);
      expect.int(r.startPos.line).toBe(1);
      expect.int(r.startPos.column).toBe(0);
      expect.int(r.endPos.line).toBe(1);
      expect.int(r.endPos.column).toBe(0);

      input("j");

      expect.int(List.length(rangeChanges^)).toBe(2);
      let r = List.hd(rangeChanges^);
      expect.int(r.startPos.line).toBe(1);
      expect.int(r.startPos.column).toBe(0);
      expect.int(r.endPos.line).toBe(2);
      expect.int(r.endPos.column).toBe(0);

      dispose();
    })
  );
});
