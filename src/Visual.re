let getRange = () => {
  let (startLine, startColumn, endLine, endColumn) =
    Native.vimVisualGetRange();

  Range.create(~startLine, ~startColumn, ~endLine, ~endColumn, ());
};

let getType = Native.vimVisualGetType;

let onRangeChanged = f => {
  Event.add(f, Listeners.visualRangeChanged);
};
