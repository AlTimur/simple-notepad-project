#ifndef SYMBOL_INSERT_DIALOG_H
#define SYMBOL_INSERT_DIALOG_H

#include <QDialog>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QTableWidget>
#include <QVBoxLayout>

class symbol_insert_dialog : public QDialog {
    Q_OBJECT

public:
    explicit symbol_insert_dialog(QWidget* parent = nullptr)
        : QDialog(parent)
    {
        setWindowTitle("Insert Symbol");

        auto* layout = new QVBoxLayout(this);

        layout->addWidget(new QLabel("Click a symbol to insert it:", this));

        const QStringList symbols = {
            "\xe2\x80\xa6", "\xe2\x80\x94", "\xe2\x80\x93", "\xc2\xab", "\xc2\xbb",
            "\xe2\x80\x9c", "\xe2\x80\x9d", "\xe2\x80\x98", "\xe2\x80\x99", "\xc2\xa0",
            "\xc2\xb0",     "\xc2\xb1",     "\xc3\x97",     "\xc3\xb7",     "\xe2\x89\xa0",
            "\xe2\x89\x88", "\xe2\x89\xa4", "\xe2\x89\xa5", "\xc2\xb2",     "\xc2\xb3",
            "\xe2\x82\xac", "\xc2\xa3",     "\xc2\xa5",     "\xe2\x82\xbd", "\xc2\xa2",
            "\xe2\x82\xb4", "\xe2\x82\xb8", "\xe2\x82\xb9", "\xe2\x82\xa9", "\xe2\x82\xbf",
            "\xe2\x86\x90", "\xe2\x86\x92", "\xe2\x86\x91", "\xe2\x86\x93", "\xe2\x87\x90",
            "\xe2\x87\x92", "\xe2\x86\x94", "\xe2\x86\x95", "\xe2\x86\xb6", "\xe2\x86\xb7",
            "\xc2\xa9",     "\xc2\xae",     "\xe2\x84\xa2", "\xe2\x80\xa0", "\xe2\x80\xa1",
            "\xe2\x80\xa2", "\xe2\x98\x86", "\xe2\x98\x85", "\xe2\x9c\x93", "\xe2\x9c\x97",
        };

        const int cols = 10;
        const int rows = (symbols.size() + cols - 1) / cols;

        auto* table = new QTableWidget(rows, cols, this);
        table->horizontalHeader()->setVisible(false);
        table->verticalHeader()->setVisible(false);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionMode(QAbstractItemView::NoSelection);
        table->setShowGrid(true);

        for (int c = 0; c < cols; ++c)
            table->setColumnWidth(c, 36);
        for (int r = 0; r < rows; ++r)
            table->setRowHeight(r, 36);

        for (int i = 0; i < symbols.size(); ++i) {
            const int r = i / cols;
            const int c = i % cols;
            const QString display = (symbols[i] == "\xc2\xa0") ? QString("[nb]") : symbols[i];
            auto* item = new QTableWidgetItem(display);
            item->setTextAlignment(Qt::AlignCenter);
            item->setToolTip(symbols[i] == "\xc2\xa0" ? "Non-breaking space" : symbols[i]);
            table->setItem(r, c, item);
        }

        connect(table, &QTableWidget::cellDoubleClicked, this, [this, symbols, cols](int r, int c) {
            const int idx = r * cols + c;
            if (idx < symbols.size())
                emit symbol_selected(symbols[idx]);
            accept();
        });

        connect(table, &QTableWidget::cellClicked, this, [this, symbols, cols](int r, int c) {
            const int idx = r * cols + c;
            if (idx < symbols.size())
                emit symbol_selected(symbols[idx]);
        });

        layout->addWidget(table);

        auto* hint = new QLabel("Single click = insert and keep open.  Double click = insert and close.", this);
        hint->setStyleSheet("color: gray; font-size: 11px;");
        layout->addWidget(hint);

        auto* close_btn = new QPushButton("Close", this);
        connect(close_btn, &QPushButton::clicked, this, &QDialog::accept);
        layout->addWidget(close_btn, 0, Qt::AlignRight);

        const int total_w = cols * 36 + 4;
        table->setFixedWidth(total_w);
        setFixedWidth(total_w + layout->contentsMargins().left() * 2 + 16);
    }

signals:
    void symbol_selected(const QString& symbol);
};

#endif