
type os =
    | Windows
    | Mac
    | Linux
    | Unknown

let uname () =
    let ic = Unix.open_process_in "uname" in
    let uname = input_line ic in
    let () = close_in ic in
    uname;;

let get_os =
    match Sys.os_type with
    | "Win32" -> Windows
    | _ -> match uname () with
        | "Darwin" -> Mac
        | "Linux" -> Linux
        | _ -> Unknown

let root = Sys.getenv "cur__root"
let libvimIncludePath = Sys.getenv "LIBVIM_INCLUDE_PATH"
let libvimLibPath = Sys.getenv "LIBVIM_LIB_PATH"
let c_flags = ["-I"; libvimIncludePath ; "-I"; Filename.concat libvimIncludePath "proto"] (* "-I"; Filename.concat root "src"] *)

let c_flags =
    match get_os with
    | Windows -> c_flags
    | Linux -> c_flags @ ["-DHAVE_CONFIG_H"]
    | _ -> c_flags @ ["-DHAVE_CONFIG_H"; "-DMACOS_X"; "-DMACOS_X_DARWIN"]

let libPath = "-L" ^ (Sys.getenv "LIBVIM_LIB_PATH")

let _ = print_endline (libPath)

let ccopt s = ["-ccopt"; s]
let cclib s = ["-cclib"; s]

let flags =
    match get_os with
    | Windows ->  []
        @ ccopt(libPath)
        @ cclib("-lvim")
        @ cclib("-luuid")
        @ cclib("-lnetapi32")
        @ cclib("-lole32")
        @ cclib("-lgdi32")
    | Linux -> []
        @ ccopt(libPath)
        @ cclib("-lvim")
        @ cclib("-lX11")
        @ cclib("-lSM")
        @ cclib("-lncurses")
        @ cclib("-lXt")
    | _ -> []
        @ ccopt(libPath)
        @ cclib("-lvim")
        @ cclib("-lm")
        @ cclib("-lncurses")
        @ cclib("-liconv")
        @ cclib("-framework AppKit")
;;

let cxx_flags =
    match get_os with
    | Windows -> c_flags @ ["-fno-exceptions"; "-fno-rtti"; "-lstdc++"]
    | _ -> c_flags
;;

Configurator.V1.Flags.write_sexp "c_flags.sexp" c_flags;
Configurator.V1.Flags.write_sexp "cxx_flags.sexp" cxx_flags;
Configurator.V1.Flags.write_sexp "flags.sexp" flags;
