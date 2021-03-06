open EditorCoreTypes;

let getHighlightsInRange = (startLine, stopLine) => {
  let highlights = Native.vimSearchGetHighlights(startLine, stopLine);
  Array.map(
    ((startLine, startColumn, stopLine, stopColumn)) =>
      Range.create(
        ~start=
          Location.create(
            ~line=Index.fromOneBased(startLine),
            ~column=Index.fromZeroBased(startColumn),
          ),
        ~stop=
          Location.create(
            ~line=Index.fromOneBased(stopLine),
            ~column=Index.fromZeroBased(stopColumn),
          ),
      ),
    highlights,
  );
};

let getHighlights = () => {
  getHighlightsInRange(0, 0);
};

let getMatchingPair = () => {
  switch (Native.vimSearchGetMatchingPair()) {
  | Some((line, column)) =>
    Some(
      Location.create(
        ~line=Index.fromOneBased(line),
        ~column=Index.fromZeroBased(column),
      ),
    )
  | None => None
  };
};

let onStopSearchHighlight = f => Event.add(f, Listeners.stopSearchHighlight);
