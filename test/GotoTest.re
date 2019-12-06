open TestFramework;
open Vim;

let resetBuffer = () => Helpers.resetBuffer("test/testfile.txt");
let input = s => ignore(Vim.input(s));

describe("Goto", ({test, _}) => {
  test("gd", ({expect}) => {
    let _ = resetBuffer();

    let updates = ref([]);

    let dispose =
      onGoto((_position, gotoType) => updates := [gotoType, ...updates^]);

    input("gd");
    let gotoType = List.hd(updates^);

    expect.int(List.length(updates^)).toBe(1);
    expect.equal(gotoType, Types.Definition);

    dispose();
  });
  test("gD", ({expect}) => {
    let _ = resetBuffer();

    let updates = ref([]);

    let dispose =
      onGoto((_position, gotoType) => updates := [gotoType, ...updates^]);

    input("gD");
    let gotoType = List.hd(updates^);

    expect.int(List.length(updates^)).toBe(1);
    expect.equal(gotoType, Types.Declaration);

    dispose();
  });
});
