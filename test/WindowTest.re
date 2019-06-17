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
});
