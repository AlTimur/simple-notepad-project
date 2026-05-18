# Notepad

A WordPad-like rich text editor built with C++ and Qt 6.

## Build

```bash
cmake -S . -B build && cmake --build build
```

Set the working directory to the project root so that `data/images/` and `data/words.txt` resolve correctly. In CLion: **Run > Edit Configurations > Working Directory** → `$ProjectFileDir$`.

## Features

### File

| Action | Shortcut |
|---|---|
| New | — |
| Open | `Ctrl+O` |
| Open Readme | `Ctrl+Shift+R` |
| Save | `Ctrl+S` |
| Save As | — |

**Open Readme** loads `Readme.md` from the working directory and automatically enables the Markdown preview panel.

### Edit

| Action | Shortcut |
|---|---|
| Undo / Redo | `Ctrl+Z` / `Ctrl+Y` |
| Cut / Copy / Paste | `Ctrl+X` / `Ctrl+C` / `Ctrl+V` |
| Select All | `Ctrl+A` |
| Insert Symbol | `Ctrl+Shift+S` |

**Insert Symbol** opens a table of 50 special characters grouped by category: typography, math, currency, arrows, and marks. Single click inserts and keeps the dialog open; double click inserts and closes.

### Format

**Bold / Italic / Underline** — toolbar buttons and `Ctrl+B` / `Ctrl+I` / `Ctrl+U`.

**Font** — opens `QFontDialog`; applies to selection or whole document.

**Text Color** — opens `QColorDialog`; applies to selection.

**Highlight** — applies a background color to selected text. Five preset colors (Yellow, Green, Cyan, Pink, Orange), a custom color picker, and a clear option. The `H` toolbar button applies the last used color.

**Text Case** — five transforms applied to selection or whole document:

- To Uppercase
- To Lowercase
- Capitalize Words
- Sentence Case
- Swap Case

### Search

**Find / Replace** (`Ctrl+F`) — find next, replace one, replace all, optional case-sensitive matching.

### Tools

**Word Frequency** — counts every word in the document and shows a sorted table.

**Text Statistics** — shows characters (total and without spaces), words, sentences, paragraphs, average word length, and the most frequent word.

**Check Spelling** — re-runs the spell-check highlight pass over the whole document.

### View

**Zoom In / Out / Reset** — `Ctrl++` / `Ctrl+-` / `Ctrl+0`.

**Markdown Preview** (`Ctrl+Shift+M`) — splits the window: editor on the left, rendered Markdown on the right. Updates live as you type. Uses Qt 6's built-in `setMarkdown()` — no external libraries required.

### Spell Checker

Loaded from `data/words.txt` at startup (370 105 English words). Misspelled words are underlined in red as you type. Right-clicking a misspelled word shows up to five suggestions.

### Status Bar

Displays word count, line count, and current cursor position (line and column) in real time.

### Exception Handling

File errors are caught and shown in a `QMessageBox::critical` dialog:

- `file_not_found_exception` — path does not exist
- `file_read_exception` — file cannot be opened for reading
- `file_write_exception` — file cannot be opened for writing

All three inherit from `notepad_exception`, which inherits from `std::runtime_error`.

## Project Structure

```
.
├── main.cpp
├── main_window.h
├── main_window.cpp
├── text_transform.h
├── notepad_exception.h
├── spell_checker.h
├── spell_checker_highlighter.h
├── text_statistics_dialog.h
├── symbol_insert_dialog.h
├── find_replace_dialog.ui
├── word_frequency_dialog.ui
└── data/
    ├── words.txt
    └── images/
        ├── bold.svg
        ├── italic.svg
        └── underline.svg
```

## Implementation Notes

**`text_transform`** is an abstract base class with a pure virtual `apply()` method. Each of the five case transforms is a concrete subclass. The main window stores them as `std::vector<std::unique_ptr<text_transform>>` and iterates over them to build the Text Case menu dynamically.

**`spell_checker`** stores the word list in `std::set<std::string>` for O(log n) lookup. Suggestions are generated using Norvig-style edit distance 1: deletions, transpositions, replacements, and insertions.

**`spell_checker_highlighter`** extends `QSyntaxHighlighter` and underlines unrecognized words using `QTextCharFormat::SpellCheckUnderline`.

**`text_statistics_dialog`** and **`symbol_insert_dialog`** are self-contained header-only `QDialog` subclasses.

**Markdown preview** uses `QSplitter` to divide the window and `QTextEdit::setMarkdown()` to render. The preview panel updates on every `textChanged` signal when visible.
