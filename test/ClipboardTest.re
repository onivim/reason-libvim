open TestFramework;
open Vim;

let resetBuffer = () => Helpers.resetBuffer("test/testfile.txt");

let makeFun = (s) => (_) => Some([|s|]);

describe("Clipboard", ({describe, _}) => {
  describe("garbage collection", ({test, _}) => {
    test("clipboard function should not get collected", ({expect, _}) => {
      
      let f = makeFun("a");

      let finalizeCount = ref(0);

      print_endline("setting provider...");
      Clipboard.setProvider(f);
      print_endline("provider set.");

      Gc.finalise_last(() => incr(finalizeCount), f);

      Gc.full_major();

      print_endline ("finalise count: " ++ string_of_int(finalizeCount^));

      expect.int(finalizeCount^).toBe(0);
    });
    
    test("clipboard function should be collected if it is overridden", ({expect, _}) => {
      
      let f0 = makeFun("a");
      let f1 = makeFun("b");

      let finalizeCount = ref(0);

      Clipboard.setProvider(f0);

      Gc.finalise_last(() => incr(finalizeCount), f0);
      
      Clipboard.setProvider(f1);

      Gc.full_major();

      expect.int(finalizeCount^).toBe(1);
    });
  });
});
