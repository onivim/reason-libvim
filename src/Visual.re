open EditorCoreTypes;

let getRange = () => {
  let (startLine, startColumn, stopLine, stopColumn) =
    Native.vimVisualGetRange();

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
  );
};

let getType = Native.vimVisualGetType;

let onRangeChanged = f => {
  Event.add(f, Listeners.visualRangeChanged);
};
