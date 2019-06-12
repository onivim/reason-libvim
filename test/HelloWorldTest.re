open TestFramework;

describe("HelloWorld", ({test, _}) =>
  test("passes", ({expect}) =>
    expect.bool(true).toBe(true)
  )
);
