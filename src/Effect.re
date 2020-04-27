type t =
  | Message({
      priority: Types.msgPriority,
      title: string,
      message: string,
    })
  | BufferUpdate;
