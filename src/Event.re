type handler('a) = 'a => unit;
type handler2('a, 'b) = ('a, 'b) => unit;

let dispatch = (v: 'a, handlers: ref(list(handler('a)))) => {
  List.iter(f => f(v), handlers^);
};

let dispatch2 = (a: 'a, b: 'b, handlers: ref(list(handler2('a, 'b)))) => {
  List.iter(f => f(a, b), handlers^);
};

let add = (f: handler('a), handlers: ref(list(handler('a)))) => {
  handlers := [f, ...handlers^];

  () => {
    handlers := List.filter(g => g !== f, handlers^);
  };
};

let add2 = (f: handler2('a, 'b), handlers: ref(list(handler2('a, 'b)))) => {
  handlers := [f, ...handlers^];

  () => {
    handlers := List.filter(g => g !== f, handlers^);
  };
};
