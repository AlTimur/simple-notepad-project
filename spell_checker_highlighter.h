#ifndef SPELL_CHECKER_HIGHLIGHTER_H
#define SPELL_CHECKER_HIGHLIGHTER_H

#include "spell_checker.h"

#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>

class spell_checker_highlighter : public QSyntaxHighlighter {
    Q_OBJECT

public:
    explicit spell_checker_highlighter(QTextDocument* parent, const spell_checker& checker)
        : QSyntaxHighlighter(parent)
        , checker_(checker)
    {
        format_.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
        format_.setUnderlineColor(Qt::red);
    }

    void rehighlight_all() { rehighlight(); }

protected:
    void highlightBlock(const QString& text) override
    {
        static const QRegularExpression word_re("[A-Za-z]+");
        auto it = word_re.globalMatch(text);
        while (it.hasNext()) {
            const auto match = it.next();
            const auto word = match.captured().toLower().toStdString();
            if (!checker_.check(word))
                setFormat(match.capturedStart(), match.capturedLength(), format_);
        }
    }

private:
    const spell_checker& checker_;
    QTextCharFormat format_;
};

#endif // SPELL_CHECKER_HIGHLIGHTER_H
