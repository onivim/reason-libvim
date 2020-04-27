type t =
  | Message({
      priority: Types.msgPriority,
      title: string,
      message: string,
    })
  | BufferUpdate;

let matches = (~f, effects) => 
effects
|> List.exists(f);
