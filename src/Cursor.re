let getLine = () => {
    Native.vimCursorGetLine();
};

let getColumn = () => {
    Native.vimCursorGetColumn();
};

let getPosition = () => {
    Position.create(
        ~line=Native.vimCursorGetLine(),
        ~column=Native.vimCursorGetColumn(),
    );
};

let onCursorMoved = (f) => {
    Event.add(f, Listeners.cursorMoved);
};
