open TestFramework;

describe("Preamble", ({test, describe, _}) => {
  test("fails", ({expect}) => {
    expect.bool(false).toBe(
      true,
    );
  });
});
