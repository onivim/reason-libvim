open TestFramework;

let resetBuffer = () => Helpers.resetBuffer("test/testfile.txt");

describe("onUnhandledEscape", ({test, _}) => {
  test("unhandled escape called with no pending operator", ({expect}) => {
    let _ = resetBuffer();

    let callCount = ref(0);
    let dispose = Vim.onUnhandledEscape(() => incr(callCount));

    Vim.input("<esc>");
    expect.int(callCount^).toBe(1);

    dispose();
  });
  test("unhandled escape not called when in insert", ({expect}) => {
    let _ = resetBuffer();

    let callCount = ref(0);
    let dispose = Vim.onUnhandledEscape(() => incr(callCount));

    Vim.input("i");
    Vim.input("<esc>");
    expect.int(callCount^).toBe(0);

    dispose();
  });
});
