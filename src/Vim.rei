module AutoCommands = AutoCommands;
module Buffer = Buffer;
module BufferMetadata = BufferMetadata;
module BufferUpdate = BufferUpdate;
module CommandLine = CommandLine;
module Cursor = Cursor;
module Mode = Mode;
module Position = Position;
module Search = Search;
module Visual = Visual;
module VisualRange = VisualRange;

let init: unit => unit;

let input: string => unit;

let command: string => unit;
