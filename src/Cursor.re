let getLine = () => {
    Native.vimCursorGetLine();
};

let getColumn = () => {
    Native.vimCursorGetLine();
};

let getPosition = () => {
    Position.create(
        ~line=Native.vimCursorGetLine(),
        ~column=Native.vimCursorGetColumn(),
    );
};
