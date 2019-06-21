let getLine = Native.vimCursorGetLine;

let getColumn = Native.vimCursorGetColumn;

let getPosition = () => {
  Position.create(
    ~line=Native.vimCursorGetLine(),
    ~column=Native.vimCursorGetColumn(),
  );
};

let setPosition = Native.vimCursorSetPosition;

let onMoved = f => {
  Event.add(f, Listeners.cursorMoved);
};
