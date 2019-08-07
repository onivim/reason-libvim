let getLine = Native.vimCursorGetLine;

let getColumn = Native.vimCursorGetColumn;

let getPosition = () => {
  Position.create(
    ~line=Native.vimCursorGetLine(),
    ~column=Native.vimCursorGetColumn(),
  );
};

let setPosition = (line, column) => {
  let previousTopLine = Native.vimWindowGetTopLine();
  let previousLeft = Native.vimWindowGetLeftColumn();

  let lastPosition = getPosition();
  Native.vimCursorSetPosition(line, column);
  let newPosition = getPosition();
  let newTopLine = Native.vimWindowGetTopLine();
  let newLeft = Native.vimWindowGetLeftColumn();

  if (!Position.equals(lastPosition, newPosition)) {
    Event.dispatch(newPosition, Listeners.cursorMoved);
  };

  if (previousTopLine !== newTopLine) {
    Event.dispatch(newTopLine, Listeners.topLineChanged);
  };

  if (previousLeft !== newLeft) {
    Event.dispatch(newLeft, Listeners.leftColumnChanged);
  };
};

let onMoved = f => {
  Event.add(f, Listeners.cursorMoved);
};
