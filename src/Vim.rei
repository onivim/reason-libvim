/**
[init] must be called prior to [input] or [command], and must only be called once.

[init] initializes and sets up initial global state for Vim.
*/
let init: unit => unit;

/**
[input(s)] sends a single keystroke to Vim.

The value [s] may be of the following form:
- A single ASCII character, ie ["a"] or [":"]
- A Vim key, ie ["<cr>"] or ["<bs>"]
- A Vim key with modifiers, ie ["<C-a>"]

The keystroke is processed synchronously.
*/
let input: string => unit;

/**
[command(cmd)] executes [cmd] as an Ex command.

For example, [command("edit! buf.txt")] would run the [edit!] command and open [buf.txt].

You may use any valid Ex command, although you must omit the leading semicolon.

The command [cmd] is processed synchronously.
*/
let command: string => unit;

/**
[onDirectoryChanged(f)] registers a directory changed listener [f].

[f] is called whenever the active directory is changed, for example,
via a [command("cd some-new-directory")].
*/
let onDirectoryChanged:
  Listeners.directoryChangedListener => Event.unsubscribe;

/**
[onMessage(f)] registers a message listener [f].

[f] is called whenever a message is emitted from Vim.
*/
let onMessage: Listeners.messageListener => Event.unsubscribe;

/**
[onQuit(f)] registers a quit listener [f].

[f] is called whenever a quit is requested, for example,
by [command(":q")] or [ZZ].
*/

let onQuit: Listeners.quitListener => Event.unsubscribe;

/**
[onYank(f)] registers a yank listener [f]

[f] is called whenever a value is 'yanked' to a register -
this could happen as a result of a yank or a delete command.
*/
let onYank: Listeners.yankListener => Event.unsubscribe;

module AutoClosingPairs = AutoClosingPairs;
module AutoCommands = AutoCommands;
module Buffer = Buffer;
module BufferMetadata = BufferMetadata;
module BufferUpdate = BufferUpdate;
module CommandLine = CommandLine;
module Cursor = Cursor;
module Mode = Mode;
module Options = Options;
module Position = Position;
module Range = Range;
module Search = Search;
module Types = Types;
module Visual = Visual;
module VisualRange = VisualRange;
module Window = Window;
module Yank = Yank;
