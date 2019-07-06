open TestFramework;
open Vim;

let resetBuffer = () => Helpers.resetBuffer("test/testfile.txt");

describe("DirectoryChanged", ({test, _}) => {
    test("get changed directory event", ({expect}) => {
      let _ = resetBuffer();
      
      let updates: ref(list(string)) = ref([]);
      let dispose = onDirectoryChanged(upd => updates := [upd, ...updates^]);

      command("cd /");
      expect.int(List.length(updates^)).toBe(1);

      dispose();
    });
});
