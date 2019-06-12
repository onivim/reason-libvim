type t = Types.mode;

let getMatchingPair = () => {
    let result: option((int, int)) = Native.vimSearchGetMatchingPair();

    switch (result) {
    | None => None
    | Some((line, column)) => Some(Position.create(~line, ~column));
    }
};
