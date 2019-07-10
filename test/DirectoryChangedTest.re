open TestFramework;
open Vim;

let resetBuffer = () => Helpers.resetBuffer("test/testfile.txt");

describe("DirectoryChanged", ({test, _}) => {
  test("get changed directory event", ({expect}) => {
    let _ = resetBuffer();

    let updates: ref(list(string)) = ref([]);
    let dispose = onDirectoryChanged(upd => updates := [upd, ...updates^]);

    command("cd test");
    expect.int(List.length(updates^)).toBe(1);

    command("cd ..");
    expect.int(List.length(updates^)).toBe(2);

    dispose();
  })
  test("change directory via input", ({expect}) => {
    let _ = resetBuffer();

    let updates: ref(list(string)) = ref([]);
    let dispose = onDirectoryChanged(upd => updates := [upd, ...updates^]);

    input(":");
    input("c");
    input("d");
    input(" ");
    input("t");
    input("e");
    input("s");
    input("t");
    input("<cr>");
    expect.int(List.length(updates^)).toBe(1);

    input("c");
    input("c");
    input("d");
    input(" ");
    input(".");
    input(".");
    input("<cr>");
    expect.int(List.length(updates^)).toBe(2);

    dispose();
  })
});
