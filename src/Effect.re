open EditorCoreTypes;
type t =
  | ShowIntro
  | ShowVersion
  | Goto({
      location: Location.t,
      gotoType: Types.gotoType,
    })
  | Message({
      priority: Types.msgPriority,
      title: string,
      message: string,
    })
  | BufferUpdate;

let matches = (f, effects) => effects |> List.exists(f);
