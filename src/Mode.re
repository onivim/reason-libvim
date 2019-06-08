type t = Types.mode;

let show = (mode: t) => {
    switch (mode) {
    | Normal => "Normal"
    | Visual => "Visual"
    | CommandLine => "CommandLine"
    | Replace => "Replace"
    | Operator => "Operator"
    | Insert => "Insert"
    }
}

let getCurrent = Native.vimGetMode;

let onChanged = (f: Types.modeChangedListener) => {
    Globals.modeChangedListeners := [f, ...Globals.modeChangedListeners^];

    () => {
        List.filter((g) => g !== f, Globals.modeChangedListeners^);
    };
};
