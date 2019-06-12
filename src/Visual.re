let getRange = () => {
 let (startLine, startColumn, endLine, endColumn) = Native.vimVisualGetRange();

 VisualRange.create(~startLine, ~startColumn, ~endLine, ~endColumn, ());
};

let onRangeChanged = f => {
  Event.add(f, Listeners.visualRangeChanged);
};
