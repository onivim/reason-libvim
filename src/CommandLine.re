open Types;

type t = Types.cmdline;

let getCompletions = () => Native.vimCommandLineGetCompletions();

let getText = () => Native.vimCommandLineGetText();

let getPosition = () => Native.vimCommandLineGetPosition();

let getType = () =>
  switch (Native.vimCommandLineGetType()) {
  | 0 => Ex
  | 1 => SearchForward
  | 2 => SearchReverse
  | _ => Unknown
  };

let onEnter = (f) => Event.add(f, Listeners.commandLineEnter);
let onLeave = (f) => Event.add(f, Listeners.commandLineLeave);
let onUpdate = (f) => Event.add(f, Listeners.commandLineUpdate);
