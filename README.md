# CherryReadLine

CherryReadLine is a tiny designed readline and libedit replacement specifically for deeply embedded applications.

## Feature

- No dynamic memory used
- Support all single keycodes, including ``F1 - F12``, ``HOME`` and ``END``.
- Support key combination with ``Ctrl + A~Z`` or ``Alt + A~Z``
- Support history with ``↑`` or ``↓``
- Support cursor movement with ``delete``, ``←``, ``→``, ``HOME`` or ``END``
- Support completion, default with ``TAB``, you can use map api to change keycode for completion
- Support format list of completion
- Support multiline prompt
- Support color prompt
- Support ``Ctrl + \<key\>`` mapping
- Support ``Alt + \<key\>`` mapping
- Support auto select completion or space
- Support Xterm alt screen buffer mode
- Support prompt edit API
- Support line edit API
- Support python style using prompt edit API
- Support map some keycodes for user event
- Support debug keycodes
- Support mask mode
