open TestFramework;
open Vim;
open Range;

let resetBrackets = () => Helpers.resetBuffer("test/brackets.txt");
let reset = () => Helpers.resetBuffer("test/testfile.txt");
let input = (s) => ignore(Vim.input(s));

describe("Search", ({describe, _}) => {
  describe("getSearchHighlights", ({test, _}) => {
    test("gets highlights", ({expect}) => {
      let _ = reset();

      input("/");
      input("e");

      let highlights = Search.getHighlights();
      expect.int(Array.length(highlights)).toBe(13);

      input("s");
      let highlights = Search.getHighlights();
      expect.int(Array.length(highlights)).toBe(3);

      let secondHighlight = highlights[1];
      expect.int(secondHighlight.startPos.line).toBe(2);
      expect.int(secondHighlight.startPos.column).toBe(30);
      expect.int(secondHighlight.endPos.line).toBe(2);
      expect.int(secondHighlight.endPos.column).toBe(32);
    });

    test("gets highlights", ({expect}) => {
      let _ = reset();

      input("/");
      input("e");

      let highlights = Search.getHighlightsInRange(1, 1);
      expect.int(Array.length(highlights)).toBe(4);

      input("s");
      let highlights = Search.getHighlightsInRange(1, 1);
      expect.int(Array.length(highlights)).toBe(1);
    });
  });

  describe("onStopSearchHighlight", ({test, _}) =>
    test(
      "onStopSearchHighlight dispatches when nohlsearch is called",
      ({expect}) => {
      let _ = reset();

      let callCount = ref(0);
      let unsubscribe =
        Vim.Search.onStopSearchHighlight(() => incr(callCount));

      Vim.command("nohlsearch");

      expect.int(callCount^).toBe(1);

      unsubscribe();
    })
  );

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
  });
});
