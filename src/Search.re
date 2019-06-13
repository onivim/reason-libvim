
let getHighlightsInRange = (startLine, endLine) => {
    let highlights = Native.vimSearchGetHighlights(startLine, endLine);
    Array.map((v) => {
        let (startLine, startColumn, endLine, endColumn) = v;
        Range.create(~startLine, ~startColumn, ~endLine, ~endColumn, ());
}, highlights);
}

let getHighlights = () => {
    getHighlightsInRange(0, 0);
};

let getMatchingPair = () => {
  let result: option((int, int)) = Native.vimSearchGetMatchingPair();

  switch (result) {
  | None => None
  | Some((line, column)) => Some(Position.create(~line, ~column))
  };
};
