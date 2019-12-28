open EditorCoreTypes;

module AutoClosingPair = {
  type t = {
    opening: string,
    closing: string,
  };

  let create = (~closeBefore=[], ~opening, ~closing, ()) => {
    let hash = Hashtbl.create(16);
    List.iter(item => Hashtbl.add(hash, item, true), closeBefore);

    {opening, closing};
  };
};

type t = {
  pairs: list(AutoClosingPair.t),
  before: Hashtbl.t(string, bool),
};

let empty: t = {pairs: [], before: Hashtbl.create(0)};

let create = (~allowBefore=[], p: list(AutoClosingPair.t)) => {
  let hash = Hashtbl.create(16);
  List.iter(item => Hashtbl.add(hash, item, true), allowBefore);
  {pairs: p, before: hash};
};

let closingPairs = ref(empty);
let enabled = ref(false);

let setEnabled = e => enabled := e;
let getEnabled = () => enabled^;

let setPairs = (acp: t) => {
  closingPairs := acp;
};

let isMatchingPair = c => {
  List.exists((p: AutoClosingPair.t) => p.opening == c, closingPairs^.pairs);
};

let isOpeningPair = c => {
  let currentPairs = closingPairs^;
  List.exists((p: AutoClosingPair.t) => p.opening == c, currentPairs.pairs);
};

let isClosingPair = c => {
  let currentPairs = closingPairs^;
  List.exists((p: AutoClosingPair.t) => p.closing == c, currentPairs.pairs);
};

let getByOpeningPair = c => {
  let currentPairs = closingPairs^;
  let matches =
    List.filter(
      (p: AutoClosingPair.t) => p.opening == c,
      currentPairs.pairs,
    );

  switch (matches) {
  | [hd, ..._] => hd
  | [] => failwith("No matching pair: " ++ c)
  };
};

let isBetweenPairs = (line, index) => {
  let index = Index.toZeroBased(index);
  let len = String.length(line);
  if (index > 0 && index < len) {
    List.exists(
      (p: AutoClosingPair.t) =>
        p.opening == String.sub(line, index - 1, 1)
        && p.closing == String.sub(line, index, 1),
      closingPairs^.pairs,
    );
  } else {
    false;
  };
};

let _exists = (key, hashtbl) =>
  switch (Hashtbl.find_opt(hashtbl, key)) {
  | Some(_) => true
  | None => false
  };

let canCloseBefore = (line, index) => {
  let index = Index.toZeroBased(index);
  let len = String.length(line);
  if (index > 0 && index < len) {
    let nextChar = String.sub(line, index, 1);
    _exists(nextChar, closingPairs^.before);
  } else {
    true;
        // No character past cursor, always allow
  };
};

let doesNextCharacterMatch = (line, index, s) => {
  let index = Index.toZeroBased(index);
  let len = String.length(line);
  if (index > 0 && index < len) {
    s == String.sub(line, index, 1);
  } else {
    false;
  };
};
