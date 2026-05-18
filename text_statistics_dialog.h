#ifndef TEXT_STATISTICS_DIALOG_H
#define TEXT_STATISTICS_DIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QMap>
#include <QPushButton>
#include <QRegularExpression>
#include <QString>
#include <QStringList>

class text_statistics_dialog : public QDialog {
public:
    explicit text_statistics_dialog(const QString& text, QWidget* parent = nullptr)
        : QDialog(parent)
    {
        setWindowTitle("Text Statistics");
        setMinimumWidth(320);

        auto* layout = new QGridLayout(this);
        layout->setColumnStretch(1, 1);

        const int chars_total = text.length();

        int chars_no_spaces = 0;
        for (const QChar c : text) {
            if (!c.isSpace())
                ++chars_no_spaces;
        }

        const QStringList words = text.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        const int word_count = words.size();

        int sentence_count = 0;
        for (const QChar c : text) {
            if (c == '.' || c == '!' || c == '?')
                ++sentence_count;
        }
        if (sentence_count == 0 && !text.trimmed().isEmpty())
            sentence_count = 1;

        int paragraph_count = 0;
        for (const QString& line : text.split('\n')) {
            if (!line.trimmed().isEmpty())
                ++paragraph_count;
        }

        double avg_word_len = 0.0;
        if (word_count > 0) {
            int total_len = 0;
            for (const QString& w : words)
                total_len += w.length();
            avg_word_len = static_cast<double>(total_len) / word_count;
        }

        QMap<QString, int> freq;
        for (const QString& w : words) {
            const QString clean = w.toLower().remove(QRegularExpression("[^a-zA-Z]"));
            if (!clean.isEmpty())
                ++freq[clean];
        }
        QString most_frequent;
        int max_freq = 0;
        for (auto it = freq.begin(); it != freq.end(); ++it) {
            if (it.value() > max_freq) {
                max_freq = it.value();
                most_frequent = it.key();
            }
        }

        int row = 0;
        auto add_row = [&](const QString& label, const QString& value) {
            auto* lbl = new QLabel(label, this);
            auto* val = new QLabel(value, this);
            val->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
            val->setStyleSheet("font-weight: bold;");
            layout->addWidget(lbl, row, 0);
            layout->addWidget(val, row, 1);
            ++row;
        };

        add_row("Characters (total):", QString::number(chars_total));
        add_row("Characters (no spaces):", QString::number(chars_no_spaces));
        add_row("Words:", QString::number(word_count));
        add_row("Sentences:", QString::number(sentence_count));
        add_row("Paragraphs:", QString::number(paragraph_count));
        add_row("Average word length:", QString::number(avg_word_len, 'f', 2));

        if (!most_frequent.isEmpty())
            add_row("Most frequent word:", most_frequent + "  (" + QString::number(max_freq) + "\xc3\x97)");

        auto* close_btn = new QPushButton("Close", this);
        connect(close_btn, &QPushButton::clicked, this, &QDialog::accept);
        layout->addWidget(close_btn, row, 0, 1, 2, Qt::AlignRight);
    }
};

#endif
