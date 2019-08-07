open TestFramework;
open Vim;

let resetBuffer = () => Helpers.resetBuffer("test/testfile.txt");

describe("Yank", ({test, _}) => {
  test("onYank works for delete", ({expect}) => {
    let _ = resetBuffer();

    let yanks: ref(list(Yank.t)) = ref([]);
    let dispose = Vim.onYank((yank) => {
      yanks := [yank, ...yanks^];
    });

    Vim.input("d");
    Vim.input("d");

    expect.int(List.length(yanks^)).toBe(1);

    dispose();
  });
});
