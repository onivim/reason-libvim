# reason-libvim

Reason API for libvim - an abstraction of Vim as an API.

## Usage

- Call `vimInit()` before anything else

- Call `vimInput("i")` or `vimInput("<esc>")` to send keystrokes
- Call `vimCommand("%s/hello/world/g")` to execute Ex commands
- Introspect the state with the API, for example:

```
vimInit();

vimCommand("e ./aBigFile.txt");

/* Use 'G' to move to the end of the file */
vimInput("G");

/* Print line position */
print_endline ("Cursor is at line: " ++ string_of_int(Cursor.getLine()));

/* Type a at the beginning of the line */
vimInput("I");
vimInput("a");
```

For more example usage, see the [test cases](https://github.com/onivim/reason-libvim/tree/master/test)

## Documentation

Documentation is available [here](https://onivim.github.io/reason-libvim/libvim/Vim/index.html)

## License

[MIT License](./LICENSE)

Copyright 2019 Outrun Labs, LLC
