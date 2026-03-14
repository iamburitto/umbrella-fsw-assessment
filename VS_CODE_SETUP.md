# VS Code setup for this repo

## Summary

- Formatting is manual (not on save, not on paste).
- Paste adjusts indentation only.
- C/C++ formatting uses `.clang-format` from the repo.
- Copilot is minimized globally, but can be manually triggered in C/C++.

### User settings.json

```json
{
  "editor.autoIndentOnPaste": true,
  "editor.autoIndent": "full",
  "editor.formatOnPaste": false,
  "editor.formatOnSave": false,
  "[c]": {
    "editor.defaultFormatter": "xaver.clang-format"
  },
  "[cpp]": {
    "editor.defaultFormatter": "ms-vscode.cpptools"
  },
  "C_Cpp.formatting": "clangFormat",
  "C_Cpp.clang_format_style": "file",
  "editor.inlineSuggest.enabled": true,
  "github.copilot.enable": {
    "*": false,
    "plaintext": false,
    "markdown": false,
    "scminput": false,
    "c": true,
    "cpp": true
  },
  "github.copilot.nextEditSuggestions.enabled": false,
  "github.copilot.chat.backgroundAgent.enabled": false,
  "github.copilot.chat.claudeAgent.enabled": false,
  "github.copilot.chat.cloudAgent.enabled": false
}
```

### User keybindings.json

```json
[
  {
    "key": "alt+enter",
    "command": "github.copilot.chat.generate",
    "when": "editorTextFocus && !editorReadonly"
  },
  {
    "key": "ctrl+enter",
    "command": "github.copilot.chat.generate",
    "when": "editorTextFocus && !editorReadonly"
  },
  {
    "key": "tab",
    "command": "editor.action.inlineSuggest.commit",
    "when": "inlineSuggestionVisible && !editorTabMovesFocus"
  }
]
```

### .clang-format

```yaml
BasedOnStyle: Google
BreakBeforeBraces: Allman
```

## Shortcuts

- Manual Copilot trigger: Alt+Enter
- Manual Copilot trigger (alternate): Ctrl+Enter
- Accept inline suggestion: Tab
- Dismiss inline suggestion: Esc
- Format whole file: Shift+Alt+F
- Format selection: Ctrl+K, then Ctrl+F
