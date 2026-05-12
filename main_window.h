#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "spell_checker.h"
#include "text_transform.h"

#include <QDialog>
#include <QMainWindow>
#include <QString>
#include <QTextDocument>
#include <QTextEdit>
#include <memory>
#include <vector>

class spell_checker_highlighter;

namespace Ui {
class find_replace_dialog;
class word_frequency_dialog;
}

class main_window : public QMainWindow {
public:
    main_window();
    ~main_window() override;

private:
    // Menu setup
    void setup_file_menu();
    void setup_edit_menu();
    void setup_format_menu();
    void setup_format_toolbar();
    void setup_search_menu();
    void setup_tools_menu();
    void setup_view_menu();

    // File operations
    void open_file();
    void save_file();
    void save_file_as();
    void update_title();

    // Status bar
    void update_status_bar();

    // Text transforms
    void apply_transform(const text_transform& transform) const;

    // Find / Replace
    void show_find_replace_dialog();
    void find_next(const QString& term,
        QTextDocument::FindFlags flags = QTextDocument::FindFlags()) const;
    void replace_current(const QString& term, const QString& replacement,
        QTextDocument::FindFlags flags = QTextDocument::FindFlags()) const;
    void replace_all(const QString& term, const QString& replacement,
        QTextDocument::FindFlags flags = QTextDocument::FindFlags()) const;

    // Word frequency
    void show_word_frequency();

    // Spell checker context menu
    void show_context_menu(const QPoint& pos);

    // Zoom
    void zoom_in();
    void zoom_out();
    void zoom_reset();

    // Widgets
    QTextEdit* editor { nullptr };
    QString current_file;
    std::vector<std::unique_ptr<text_transform>> transforms;

    // Find/Replace dialog
    QDialog* find_replace_dlg { nullptr };
    std::unique_ptr<Ui::find_replace_dialog> find_replace_ui;

    // Spell checker
    spell_checker checker_;
    spell_checker_highlighter* highlighter_ { nullptr };

    // Zoom level tracker
    int zoom_level_ { 0 };
};

#endif // MAIN_WINDOW_H
