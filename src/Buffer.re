type t;

let openFile = (filePath: string) => {
    Native.vimCommand("e! filePath");
}

let test = () => print_endline("Hello, world");
