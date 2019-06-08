open TestFramework;

describe("Preamble", ({test, _}) => {
  test("fails", ({expect}) => {
    expect.bool(false).toBe(
      true,
    );
  });
});
