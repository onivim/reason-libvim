type t = 
| Ex
| SearchForward
| SearchReverse
| Unknown;

let getText = () => Native.vimCommandLineGetText();

let getPosition = () => Native.vimCommandLineGetPosition();

let getType = () => switch(Native.vimCommandLineGetType()) {
| 0 => Ex
| 1 => SearchForward
| 2 => SearchReverse
| _ => Unknown
};
