open BufferUpdate;

type t = list(BufferUpdate.t);

let add = (update: BufferUpdate.t, acc: t) => {
  prerr_endline ("ACCUMULATE3");
  switch (acc) {
  // No update yet - add it
  | [] => [update] 
  // There is an update, but it's taken care of by a current update - either there is a full update
  // (endLine == -1) or there is already a matching update. No need to add again.
  | [v, ..._tail] when v.endLine == -1 || (v.startLine == update.startLine && v.endLine == update.endLine) => 
    prerr_endline ("REJECTING AN UPDATE WOOHOO");
    acc
  // Not covered by last update - add
  | _ => [update, ...acc];
  } 
};

let empty = [];

let iter = (f, acc: t) => {
  acc
  |> List.rev
  |> List.iter(f);
}

