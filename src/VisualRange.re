type t = {
  startPos: Position.t,
  endPos: Position.t,
};

let create = (
~startLine,
~startColumn,
~endLine,
~endColumn,
()
) => {
    let startPos = Position.create(~line=startLine, ~column=startColumn);
    let endPos = Position.create(~line=endLine, ~column=endColumn);

    { startPos, endPos };
}

let equals = (a: t, b: t) => {
    Position.equals(a.startPos, b.startPos) && Position.equals(a.endPos, b.endPos);
}
