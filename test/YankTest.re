open TestFramework;
open Vim;

let resetBuffer = () => Helpers.resetBuffer("test/testfile.txt");

describe("Yank", ({test, _}) => {
  test("onYank works for deleting single line", ({expect}) => {
    let _ = resetBuffer();

    let yanks: ref(list(Yank.t)) = ref([]);
    let dispose = Vim.onYank(yank => yanks := [yank, ...yanks^]);

    Vim.input("d");
    Vim.input("d");

    expect.int(List.length(yanks^)).toBe(1);
    let del: Yank.t = List.hd(yanks^);

    expect.bool(del.operator == Yank.Delete).toBe(true);
    expect.int(Array.length(del.lines)).toBe(1);
    expect.string(del.lines[0]).toEqual(
      "This is the first line of a test file",
    );

    dispose();
  });
  test("onYank works for deleting multiple lines", ({expect}) => {
    let _ = resetBuffer();

    let yanks: ref(list(Yank.t)) = ref([]);
    let dispose = Vim.onYank(yank => yanks := [yank, ...yanks^]);

    Vim.input("d");
    Vim.input("j");

    expect.int(List.length(yanks^)).toBe(1);
    let del: Yank.t = List.hd(yanks^);

    expect.int(Char.code(del.register)).toBe(0);
    expect.bool(del.yankType == Yank.Line).toBe(true);
    expect.bool(del.operator == Yank.Delete).toBe(true);
    expect.int(Array.length(del.lines)).toBe(2);
    expect.string(del.lines[0]).toEqual(
      "This is the first line of a test file",
    );
    expect.string(del.lines[1]).toEqual(
      "This is the second line of a test file",
    );

    dispose();
  });

  test("onYank works for single character", ({expect}) => {
    let _ = resetBuffer();

    let yanks: ref(list(Yank.t)) = ref([]);
    let dispose = Vim.onYank(yank => yanks := [yank, ...yanks^]);

    Vim.input("x");

    expect.int(List.length(yanks^)).toBe(1);
    let del: Yank.t = List.hd(yanks^);

    expect.int(Char.code(del.register)).toBe(0);
    expect.bool(del.yankType == Yank.Char).toBe(true);
    expect.bool(del.operator == Yank.Delete).toBe(true);
    expect.int(Array.length(del.lines)).toBe(1);
    expect.string(del.lines[0]).toEqual("T");

    dispose();
  });

  test("onYank works for yanking a line", ({expect}) => {
    let _ = resetBuffer();

    let yanks: ref(list(Yank.t)) = ref([]);
    let dispose = Vim.onYank(yank => yanks := [yank, ...yanks^]);

    Vim.input("y");
    Vim.input("y");

    expect.int(List.length(yanks^)).toBe(1);
    let del: Yank.t = List.hd(yanks^);

    expect.int(Char.code(del.register)).toBe(0);
    expect.bool(del.yankType == Yank.Line).toBe(true);
    expect.bool(del.operator == Yank.Yank).toBe(true);
    expect.int(Array.length(del.lines)).toBe(1);
    expect.string(del.lines[0]).toEqual(
      "This is the first line of a test file",
    );

    dispose();
  });

  test("onYank sets register correctly", ({expect}) => {
    let _ = resetBuffer();

    let yanks: ref(list(Yank.t)) = ref([]);
    let dispose = Vim.onYank(yank => yanks := [yank, ...yanks^]);

    Vim.input("\"");
    Vim.input("a");
    Vim.input("y");
    Vim.input("y");

    expect.int(List.length(yanks^)).toBe(1);
    let del: Yank.t = List.hd(yanks^);

    expect.int(Char.code(del.register)).toBe(Char.code('a'));
    expect.bool(del.yankType == Yank.Line).toBe(true);
    expect.bool(del.operator == Yank.Yank).toBe(true);
    expect.int(Array.length(del.lines)).toBe(1);
    expect.string(del.lines[0]).toEqual(
      "This is the first line of a test file",
    );

    dispose();
  });
});
