open TestFramework;
open Vim;

let resetBrackets = () => Helpers.resetBuffer("test/brackets.txt");

describe("Search", ({describe, _}) =>
  describe("getMatchingPair", ({test, _}) =>
    test("get matching bracket for initial character", ({expect}) => {
      let _ = resetBrackets();

      let bracket = Search.getMatchingPair();
      switch (bracket) {
      | None => expect.int(0).toBe(1)
      | Some(v) =>
        expect.int(v.line).toBe(5);
        expect.int(v.column).toBe(1);
      };
    })
  )
);
