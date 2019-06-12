open TestFramework;
open Vim;

let resetBrackets = () => Helpers.resetBuffer("test/brackets.txt");

describe("Search", ({describe, _}) =>
  describe("getMatchingPair", ({test, _}) => {
    test("get matching bracket for initial character", ({expect}) => {
      let _ = resetBrackets();

      let bracket = Search.getMatchingPair();
      switch (bracket) {
      | None => expect.int(0).toBe(1)
      | Some(v) =>
        expect.int(v.line).toBe(6);
        expect.int(v.column).toBe(0);
      };
    });

    test("get matching bracket after moving", ({expect}) => {
      let _ = resetBrackets();

      input("l");

      let bracket = Search.getMatchingPair();
      switch (bracket) {
      | None => expect.int(0).toBe(1)
      | Some(v) =>
        expect.int(v.line).toBe(4);
        expect.int(v.column).toBe(0);
      };
    });

    test("matching bracket is none when there is no match", ({expect}) => {
      let _ = resetBrackets();

      input("j");

      let bracket = Search.getMatchingPair();
      switch (bracket) {
      | None => expect.int(1).toBe(1)
      | Some(_) => expect.int(0).toBe(1)
      };
    });
  })
);
