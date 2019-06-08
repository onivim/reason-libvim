type mode =
| Normal
| Insert
| CommandLine
| Replace
| Visual
| Operator;

/**
 * NOTE: This type must be kept in sync with the `auto_event` enum in `vim.h`!
 *
 * For information on specific events, consult the comments for that enum.
 */
type autocmd =
| BufAdd
| BufDelete
| BufEnter
| BufFilePost
| BufFilePre
| BufHidden
| BufLeave
| BufNew
| BufNewFile
| BufReadCmd
| BufReadPost
| BufUnload
| BufWinEnter
| BufWinLeave
| BufWipeOut
| BufWriteCmd
| BufWritePost
| BufWritePre
| CmdLineChanged
| CmdLineEnter
| CmdlineLeave
| CmdUndefined
| CmdWinEnter
| CmdWinLeave
| Colorscheme
| ColorschemePre
| CompleteChanged
| CompleteDone
| CursorHold
| CursorHoldI
| CursorMoved
| CursorMovedI
| DiffUpdated
| DirChanged
| EncodingChanged
| ExitPre
| FileAppendCmd
| FileAppendPost
| FileAppendPre
| FileChangedRO
| FileChangedShell
| FileChangedShellPost
| FileReadCmd
| FileReadPost
| FileReadPre
| FileType
| FileWriteCmd
| FileWritePost
| FileWritePre
| FilterReadPost
| FilterReadPre
| FilterWritePost
| FilterWritePre
| FocusGained
| FocusLost
| FuncUndefined
| GUIEnter
| GUIFailed
| InsertChange
| InsertCharPre
| InsertEnter
| InsertLeave
| MenuPopup
| OptionSet
| QuickFixCmdPost
| QuickFixCmdPre
| QuitPre
| RemoteReply
| SessionLoadPost
| ShellCmdPost
| ShellFilterPost
| SourceCmd
| SourcePre
| SourcePost
| SpellFileMissing
| StdinReadPost
| StdinReadPre
| SwapExists
| Syntax
| TabClosed
| TabEnter
| TabLeave
| TabNew
| TermChanged
| TerminalOpen
| TermResponse
| TextChanged
| TextChangedI
| TextChangedP
| TextYankPost
| User
| VimEnter
| VimLeave
| VimLeavePre
| VimResized
| WinEnter
| WinLeave
| WinNew;

type modeChangedListener = mode => unit;

type buffer;

type bufferUpdateListener = BufferUpdate.t => unit;
