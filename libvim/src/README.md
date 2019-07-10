![Vim Logo](https://github.com/vim/vim/blob/master/runtime/vimlogo.gif)

# Vim source code #

Here are a few hints for finding your way around the source code.  This
doesn't make it less complex than it is, but it gets you started.

You might also want to read
[`:help development`](http://vimdoc.sourceforge.net/htmldoc/develop.html#development).


## Jumping around ##

First of all, use `:make tags` to generate a tags file, so that you can jump
around in the source code.

To jump to a function or variable definition, move the cursor on the name and
use the `CTRL-]` command.  Use `CTRL-T` or `CTRL-O` to jump back.

To jump to a file, move the cursor on its name and use the `gf` command.

Most code can be found in a file with an obvious name (incomplete list):

File name | Description
--------- | -----------
autocmd.c	| autocommands
buffer.c	| manipulating buffers (loaded files)
change.c	| handling changes to text
debugger.c	| vim script debugger
diff.c		| diff mode (vimdiff)
eval.c		| expression evaluation
evalfunc.c	| built-in functions
fileio.c	| reading and writing files
findfile.c	| search for files in 'path'
fold.c		| folding
getchar.c	| getting characters and key mapping
indent.c	| C and Lisp indentation
insexpand.c	| Insert mode completion
mark.c		| marks
mbyte.c		| multi-byte character handling
memfile.c	| storing lines for buffers in a swapfile
memline.c	| storing lines for buffers in memory
menu.c		| menus
message.c	| (error) messages
ops.c		| handling operators ("d", "y", "p")
option.c	| options
quickfix.c	| quickfix commands (":make", ":cn")
regexp.c	| pattern matching
screen.c	| updating the windows
search.c	| pattern searching
sign.c		| signs
spell.c		| spell checking
syntax.c	|  syntax and other highlighting
tag.c		| tags
term.c		| terminal handling, termcap codes
undo.c		| undo and redo
usercmd.c	| user defined commands
userfunc.c	| user defined functions
window.c	| handling split windows


## Debugging ##

If you have a reasonable recent version of gdb, you can use the `:Termdebug`
command to debug Vim.  See  `:help :Termdebug`.

When something is time critical or stepping through code is a hassle, use the
channel logging to create a time-stamped log file.  Add lines to the code like
this:

	ch_log(NULL, "Value is now %02x", value);

After compiling and starting Vim, do:

	:call ch_logfile('debuglog', 'w')

And edit `debuglog` to see what happens.  The channel functions already have
`ch_log()` calls, thus you always see that in the log.


## Important Variables ##

The current mode is stored in `State`.  The values it can have are `NORMAL`,
`INSERT`, `CMDLINE`, and a few others.

The current window is `curwin`.  The current buffer is `curbuf`.  These point
to structures with the cursor position in the window, option values, the file
name, etc.  These are defined in
[`structs.h`](https://github.com/vim/vim/blob/master/src/structs.h).

All the global variables are declared in
[`globals.h`](https://github.com/vim/vim/blob/master/src/globals.h).


## The main loop ##

This is conveniently called `main_loop()`.  It updates a few things and then
calls `normal_cmd()` to process a command.  This returns when the command is
finished.

The basic idea is that Vim waits for the user to type a character and
processes it until another character is needed.  Thus there are several places
where Vim waits for a character to be typed.  The `vgetc()` function is used
for this.  It also handles mapping.

Updating the screen is mostly postponed until a command or a sequence of
commands has finished.  The work is done by `update_screen()`, which calls
`win_update()` for every window, which calls `win_line()` for every line.
See the start of
[`screen.c`](https://github.com/vim/vim/blob/master/src/screen.c)
for more explanations.


## Command-line mode ##

When typing a `:`, `normal_cmd()` will call `getcmdline()` to obtain a line
with an Ex command.  `getcmdline()` contains a loop that will handle each typed
character.  It returns when hitting `CR` or `Esc` or some other character that
ends the command line mode.


## Ex commands ##

Ex commands are handled by the function `do_cmdline()`.  It does the generic
parsing of the `:` command line and calls `do_one_cmd()` for each separate
command.  It also takes care of while loops.

`do_one_cmd()` parses the range and generic arguments and puts them in the
`exarg_t` and passes it to the function that handles the command.

The `:` commands are listed in `ex_cmds.h`.  The third entry of each item is
the name of the function that handles the command.  The last entry are the
flags that are used for the command.


## Normal mode commands ##

The Normal mode commands are handled by the `normal_cmd()` function.  It also
handles the optional count and an extra character for some commands.  These
are passed in a `cmdarg_t` to the function that handles the command.

There is a table `nv_cmds` in
[`normal.c`](https://github.com/vim/vim/blob/master/src/normal.c)
which lists the first character of every command.  The second entry of each
item is the name of the function that handles the command.


## Insert mode commands ##

When doing an `i` or `a` command, `normal_cmd()` will call the `edit()`
function. It contains a loop that waits for the next character and handles it.
It returns when leaving Insert mode.


## Options ##

There is a list with all option names in
[`option.c`](https://github.com/vim/vim/blob/master/src/option.c),
called `options[]`.


## The GUI ##

Most of the GUI code is implemented like it was a clever terminal.  Typing a
character, moving a scrollbar, clicking the mouse, etc. are all translated
into events which are written in the input buffer.  These are read by the
main code, just like reading from a terminal.  The code for this is scattered
through [`gui.c`](https://github.com/vim/vim/blob/master/src/gui.c).
For example, `gui_send_mouse_event()` for a mouse click and `gui_menu_cb()` for
a menu action.  Key hits are handled by the system-specific GUI code, which
calls `add_to_input_buf()` to send the key code.

Updating the GUI window is done by writing codes in the output buffer, just
like writing to a terminal.  When the buffer gets full or is flushed,
`gui_write()` will parse the codes and draw the appropriate items.  Finally the
system-specific GUI code will be called to do the work.


## Debugging the GUI ##

Remember to prevent that gvim forks and the debugger thinks Vim has exited,
add the `-f` argument.  In gdb: `run -f -g`.

When stepping through display updating code, the focus event is triggered
when going from the debugger to Vim and back.  To avoid this, recompile with
some code in `gui_focus_change()` disabled.


## Contributing ##

If you would like to help making Vim better, see the
[`CONTRIBUTING.md`](https://github.com/vim/vim/blob/master/CONTRIBUTING.md)
file.


This is `README.md` for version 8.1 of the Vim source code.
