open TestFramework;
open Vim;

let resetBuffer = () => Helpers.resetBuffer("test/lines_100.txt");

describe("Window", ({describe, _}) => {
  describe("get / set metrics", ({test, _}) =>
    test("simple test", ({expect}) => {
      let _ = resetBuffer();

      Window.setWidth(50);
      Window.setHeight(51);

      expect.int(Window.getWidth()).toBe(50);
      expect.int(Window.getHeight()).toBe(51);
    })
  );

  describe("setTopLeft", ({test, _}) =>
    test("simple test", ({expect}) => {
      let _ = resetBuffer();

      Window.setWidth(3);
      Window.setHeight(3);
      Cursor.setPosition(2, 4);
      Window.setTopLeft(2, 3);

      expect.int(Window.getTopLine()).toBe(2);
      expect.int(Window.getLeftColumn()).toBe(3);
    })
  );

  describe("onLeftColumnChanged", ({test, _}) =>
    test("dispatches on change", ({expect}) => {
      let _ = resetBuffer();

      Window.setWidth(3);
      Window.setHeight(3);

      let leftColumnChanges: ref(list(int)) = ref([]);
      let dispose =
        Window.onLeftColumnChanged(tl =>
          leftColumnChanges := [tl, ...leftColumnChanges^]
        );

      input("$");

      expect.int(List.length(leftColumnChanges^)).toBe(1);
      expect.int(List.hd(leftColumnChanges^)).toBe(4);

      input("0");
      expect.int(List.length(leftColumnChanges^)).toBe(2);
      expect.int(List.hd(leftColumnChanges^)).toBe(0);

      dispose();
    })
  );

  describe("onTopLineChanged", ({test, _}) =>
    test("dispatches on change", ({expect}) => {
      let _ = resetBuffer();

      Window.setWidth(50);
      Window.setHeight(20);

      let topLineChanges: ref(list(int)) = ref([]);
      let dispose =
        Window.onTopLineChanged(tl =>
          topLineChanges := [tl, ...topLineChanges^]
        );

      input(":");
      input("5");
      input("0");
      input("<cr>");

      input("z");
      input("z");

      expect.int(List.length(topLineChanges^)).toBe(1);
      expect.int(List.hd(topLineChanges^)).toBe(41);

      input("z");
      input("b");

      expect.int(List.length(topLineChanges^)).toBe(2);
      expect.int(List.hd(topLineChanges^)).toBe(31);

      input("z");
      input("t");

      expect.int(List.length(topLineChanges^)).toBe(3);
      expect.int(List.hd(topLineChanges^)).toBe(50);

      dispose();
    })
  );

  describe("onMovement", ({test, _}) => {
    test("simple movement", ({expect}) => {
      let _ = resetBuffer();

      let movements = ref([]);
      let dispose =
        Window.onMovement((movementType, count) =>
          movements := [(movementType, count), ...movements^]
        );

      input("<c-w>");
      input("j");

      expect.int(List.length(movements^)).toBe(1);

      let (movementType, count) = List.hd(movements^);

      expect.bool(movementType == Types.OneDown).toBe(true);
      expect.int(count).toBe(1);

      dispose();
    });

    test("movement with count", ({expect}) => {
      let _ = resetBuffer();

      let movements = ref([]);
      let dispose =
        Window.onMovement((movementType, count) =>
          movements := [(movementType, count), ...movements^]
        );

      input("5");
      input("<c-w>");
      input("h");

      expect.int(List.length(movements^)).toBe(1);

      let (movementType, count) = List.hd(movements^);

      expect.bool(movementType == Types.OneLeft).toBe(true);
      expect.int(count).toBe(5);

      dispose();
    });

    test("rotate downwards", ({expect}) => {
      let _ = resetBuffer();

      let movements = ref([]);
      let dispose =
        Window.onMovement((movementType, count) =>
          movements := [(movementType, count), ...movements^]
        );

      input("<c-w>");
      input("r");

      expect.int(List.length(movements^)).toBe(1);

      let (movementType, count) = List.hd(movements^);

      expect.bool(movementType == Types.RotateDownwards).toBe(true);
      expect.int(count).toBe(1);

      dispose();
    });

    test("rotate upwards", ({expect}) => {
      let _ = resetBuffer();

      let movements = ref([]);
      let dispose =
        Window.onMovement((movementType, count) =>
          movements := [(movementType, count), ...movements^]
        );

      input("<c-w>");
      input("R");

      expect.int(List.length(movements^)).toBe(1);

      let (movementType, count) = List.hd(movements^);

      expect.bool(movementType == Types.RotateUpwards).toBe(true);
      expect.int(count).toBe(1);

      dispose();
    });
  });
  describe("onSplit", ({test, _}) => {
    test("vsp creates split", ({expect}) => {
      let _ = resetBuffer();

      let splits = ref([]);
      let dispose =
        Window.onSplit((splitType, name) =>
          splits := [(splitType, name), ...splits^]
        );

      command("vsp test.txt");

      expect.int(List.length(splits^)).toBe(1);

      let (splitType, name) = List.hd(splits^);

      expect.bool(splitType == Types.Vertical).toBe(true);
      expect.string(name).toEqual("test.txt");

      dispose();
    });

    test("sp creates split", ({expect}) => {
      let _ = resetBuffer();

      let splits = ref([]);
      let dispose =
        Window.onSplit((splitType, name) =>
          splits := [(splitType, name), ...splits^]
        );

      command("sp test2.txt");

      expect.int(List.length(splits^)).toBe(1);

      let (splitType, name) = List.hd(splits^);

      expect.bool(splitType == Types.Horizontal).toBe(true);
      expect.string(name).toEqual("test2.txt");

      dispose();
    });
  });
});
