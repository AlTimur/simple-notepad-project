#ifndef SPELL_CHECKER_H
#define SPELL_CHECKER_H

#include <algorithm>
#include <fstream>
#include <set>
#include <string>
#include <vector>

class spell_checker {
public:
    void load(const std::string& path)
    {
        std::ifstream file(path);
        std::string word;
        while (std::getline(file, word)) {
            if (!word.empty())
                words_.insert(word);
        }
    }

    [[nodiscard]] bool is_loaded() const { return !words_.empty(); }

    [[nodiscard]] bool check(const std::string& word) const
    {
        if (word.empty())
            return true;
        return words_.count(word) > 0;
    }

    [[nodiscard]] std::vector<std::string> suggestions(const std::string& word, int max = 5) const
    {
        // Norvig-style: generate all edits at distance 1 and check against dictionary
        std::set<std::string> candidates;
        const std::string alphabet = "abcdefghijklmnopqrstuvwxyz";

        for (std::size_t i = 0; i <= word.size(); ++i) {
            // deletion
            if (i < word.size()) {
                const auto del = word.substr(0, i) + word.substr(i + 1);
                if (words_.count(del))
                    candidates.insert(del);
            }
            // transposition
            if (i + 1 < word.size()) {
                auto trans = word;
                std::swap(trans[i], trans[i + 1]);
                if (words_.count(trans))
                    candidates.insert(trans);
            }
            for (const char c : alphabet) {
                // replacement
                if (i < word.size()) {
                    auto rep = word;
                    rep[i] = c;
                    if (words_.count(rep))
                        candidates.insert(rep);
                }
                // insertion
                const auto ins = word.substr(0, i) + c + word.substr(i);
                if (words_.count(ins))
                    candidates.insert(ins);
            }
        }

        std::vector<std::string> result(candidates.begin(), candidates.end());
        if (static_cast<int>(result.size()) > max)
            result.resize(max);
        return result;
    }

private:
    std::set<std::string> words_;
};

#endif // SPELL_CHECKER_H
