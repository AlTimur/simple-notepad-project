#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "spell_checker.h"
#include "text_transform.h"

#include <QColor>
#include <QDialog>
#include <QMainWindow>
#include <QSplitter>
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
    void setup_file_menu();
    void setup_edit_menu();
    void setup_format_menu();
    void setup_format_toolbar();
    void setup_search_menu();
    void setup_tools_menu();
    void setup_view_menu();

    void open_file();
    void open_readme();
    void save_file();
    void save_file_as();
    void update_title();

    void update_status_bar();

    void apply_transform(const text_transform& transform) const;

    void show_find_replace_dialog();
    void find_next(const QString& term,
        QTextDocument::FindFlags flags = QTextDocument::FindFlags()) const;
    void replace_current(const QString& term, const QString& replacement,
        QTextDocument::FindFlags flags = QTextDocument::FindFlags()) const;
    void replace_all(const QString& term, const QString& replacement,
        QTextDocument::FindFlags flags = QTextDocument::FindFlags()) const;

    void show_word_frequency();
    void show_text_statistics();
    void show_symbol_insert();

    void apply_highlight(const QColor& color);
    void clear_highlight();

    void toggle_markdown_preview();
    void update_markdown_preview();

    void show_context_menu(const QPoint& pos);

    void zoom_in();
    void zoom_out();
    void zoom_reset();

    QSplitter* splitter_ { nullptr };
    QTextEdit* editor { nullptr };
    QTextEdit* preview_ { nullptr };

    QString current_file;
    std::vector<std::unique_ptr<text_transform>> transforms;

    QDialog* find_replace_dlg { nullptr };
    std::unique_ptr<Ui::find_replace_dialog> find_replace_ui;

    spell_checker checker_;
    spell_checker_highlighter* highlighter_ { nullptr };

    int zoom_level_ { 0 };
    QColor highlight_color_ { Qt::yellow };
    bool preview_visible_ { false };
};

#endif
