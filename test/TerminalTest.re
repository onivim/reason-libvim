open EditorCoreTypes;
open Vim;
open TestFramework;

let resetBuffer = () => Helpers.resetBuffer("test/testfile.txt");

describe("Terminal", ({test, _}) => {
  test("Empty command works", ({expect}) => {
    let _ = resetBuffer();

    let hitCount = ref(0);
    let unsubscribe =
      Vim.onTerminal(terminalRequest => {
        expect.equal(terminalRequest.curwin, false);
        incr(hitCount);
      });

    // Enter term command
    let _ = Vim.input(":term");
    let _ = Vim.input("<CR>");

    expect.equal(hitCount^, 1);

    unsubscribe();
  });
  test("Command is reported correctly", ({expect}) => {
    let _ = resetBuffer();

    let hitCount = ref(0);
    let unsubscribe =
      Vim.onTerminal(terminalRequest => {
        expect.equal(terminalRequest.cmd, "bash");
        expect.equal(terminalRequest.curwin, false);
        incr(hitCount);
      });

    // Enter term command
    let _ = Vim.input(":term bash");
    let _ = Vim.input("<CR>");

    expect.equal(hitCount^, 1);

    unsubscribe();
  });

  test("Curwin flag is set", ({expect}) => {
    let _ = resetBuffer();

    let hitCount = ref(0);
    let unsubscribe =
      Vim.onTerminal(terminalRequest => {
        expect.equal(terminalRequest.cmd, "bash");
        expect.equal(terminalRequest.curwin, true);
        incr(hitCount);
      });

    // Enter term command
    let _ = Vim.input(":term ++curwin bash");
    let _ = Vim.input("<CR>");

    expect.equal(hitCount^, 1);

    unsubscribe();
  });
  test("GC stress test", ({expect}) => {
    let _ = resetBuffer();

    let hitCount = ref(0);
    let unsubscribe =
      Vim.onTerminal(terminalRequest => {
        Gc.full_major();
        expect.equal(terminalRequest.cmd, "bash");
        incr(hitCount);
      });

    // Enter term command
    let iter = () => {
      let _ = Vim.input(":term ++curwin bash");
      let _ = Vim.input("<CR>");
      ();
    };

    for (_ in 1 to 1000) {
      iter();
    };

    expect.equal(hitCount^, 1000);

    unsubscribe();
  });
});
