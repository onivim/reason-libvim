
type handler('a) = ('a) => unit;

let dispatch = (v: 'a, handlers: ref(list(handler('a)))) => {
    List.iter((f) => f(v), handlers^);
};

let add = (f: handler('a), handlers: ref(list(handler('a)))) => {
    handlers := [f, ...handlers^];

    () => {
        List.filter((g) => g !== f, handlers^);
    }
};
