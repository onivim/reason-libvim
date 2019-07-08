let getLine = Native.vimCursorGetLine;

let getColumn = Native.vimCursorGetColumn;

let getPosition = () => {
  Position.create(
    ~line=Native.vimCursorGetLine(),
    ~column=Native.vimCursorGetColumn(),
  );
};

let setPosition = (line, column) => {
  let lastPosition = getPosition();
  Native.vimCursorSetPosition(line, column);
  let newPosition = getPosition();

  if (!Position.equals(lastPosition, newPosition)) {
    Event.dispatch(newPosition, Listeners.cursorMoved);
  }
};

let onMoved = f => {
  Event.add(f, Listeners.cursorMoved);
};
