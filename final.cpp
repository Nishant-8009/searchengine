#include <bits/stdc++.h>
#include <memory>
#include <windows.h>
#include <conio.h>
using namespace std;

unordered_set<string> stopWords = {"the", "is", "and", "a", "an", "in", "of", "on", "for", "with", "to"};

string toLower(const string& str) {
    string result;
    result.reserve(str.size());
    for (char ch : str) {
        result += tolower(ch);
    }
    return result;
}

string removePunctuation(const string& str) {
    string result;
    for (char ch : str) {
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || 
            (ch >= '0' && ch <= '9') || ch == ' ') {
            result += ch;
        }
    }
    return result;
}

string toSingular(const string& word) {
    if (word.size() > 2 && word.substr(word.size() - 3) == "ies") {
        return word.substr(0, word.size() - 3) + "y";
    } else if (word.size() > 1 && word.back() == 's') {
        return word.substr(0, word.size() - 1);
    }
    return word;
}

vector<string> tokenize(const string& content) {
    vector<string> tokens;
    string cleanedContent = toLower(removePunctuation(content));
    string token;
    for (char ch : cleanedContent) {
        if (ch == ' ') {
            if (!token.empty()) {
                if (stopWords.find(token) == stopWords.end()) {
                    tokens.push_back(token);
                    string singularToken = toSingular(token);
                    if (singularToken != token) {
                        tokens.push_back(singularToken);
                    }
                }
                token.clear();
            }
        } else {
            token += ch;
        }
    }
    
    if (!token.empty() && stopWords.find(token) == stopWords.end()) {
        tokens.push_back(token);
        string singularToken = toSingular(token);
        if (singularToken != token) {
            tokens.push_back(singularToken);
        }
    }

    return tokens;
}

class WebPage {
public:
    string url;
    string content;
    unordered_map<string, int> wordFrequency;

    WebPage(const string& u, const string& c) : url(u), content(c) {
        auto tokens = tokenize(content);
        for (const string& token : tokens) {
            wordFrequency[token]++;
        }
    }
};

class InvertedIndex {
private:
    unordered_map<string, set<int>> index;

public:
    void addPage(int pageId, const vector<string>& tokens) {
        for (const string& token : tokens) {
            index[token].insert(pageId);
        }
    }

    set<int> search(const string& token) const {
        string singularToken = toSingular(toLower(token));
        set<int> results;

        if (index.find(token) != index.end()) {
            results.insert(index.at(token).begin(), index.at(token).end());
        }

        if (index.find(singularToken) != index.end()) {
            results.insert(index.at(singularToken).begin(), index.at(singularToken).end());
        }

        return results;
    }
};

class SearchEngine {
private:
    vector<WebPage> pages;
    InvertedIndex index;

    bool hasExactMatch(const string& pageContent, const string& query) {
        return toLower(removePunctuation(pageContent)).find(toLower(removePunctuation(query))) != string::npos;
    }

public:
    void addPage(const WebPage& page) {
        pages.push_back(page);
        int pageId = pages.size() - 1;
        index.addPage(pageId, tokenize(page.content));
    }

    void search(const string& query) {
        vector<string> queryTokens = tokenize(query);
        unordered_map<int, int> pageMatchCount;
        unordered_map<int, int> pageFrequencySum;
        vector<int> exactMatches;

        for (const string& token : queryTokens) {
            set<int> results = index.search(token);
            for (set<int>::const_iterator it = results.begin(); it != results.end(); ++it) {
                int pageId = *it;
                pageMatchCount[pageId]++;
                pageFrequencySum[pageId] += pages[pageId].wordFrequency[token];
            }
        }

        for (size_t i = 0; i < pages.size(); i++) {
            if (hasExactMatch(pages[i].content, query)) {
                exactMatches.push_back(i);
            }
        }

        vector<pair<int, pair<int, int>>> sortedResults;
        for (unordered_map<int, int>::const_iterator it = pageMatchCount.begin(); it != pageMatchCount.end(); ++it) {
            int pageId = it->first;
            int matchCount = it->second;
            sortedResults.push_back(make_pair(pageId, make_pair(matchCount, pageFrequencySum[pageId])));
        }

        sort(sortedResults.begin(), sortedResults.end(), 
            [&](const pair<int, pair<int, int>>& a, const pair<int, pair<int, int>>& b) {
                bool aExact = find(exactMatches.begin(), exactMatches.end(), a.first) != exactMatches.end();
                bool bExact = find(exactMatches.begin(), exactMatches.end(), b.first) != exactMatches.end();
                if (aExact != bExact) return aExact > bExact;
                if (a.second.first != b.second.first) return a.second.first > b.second.first;
                return a.second.second > b.second.second;
            });

        if (sortedResults.empty()) {
            cout << "No results found for: " << query << endl;
        } else {
            for (vector<pair<int, pair<int, int>>>::const_iterator it = sortedResults.begin(); it != sortedResults.end(); ++it) {
                int pageId = it->first;
                int matchCount = it->second.first;
                int frequencySum = it->second.second;
                cout << "Found: " << pages[pageId].url
                     << " (Matches: " << matchCount
                     << ", Frequency Sum: " << frequencySum
                     << (find(exactMatches.begin(), exactMatches.end(), pageId) != exactMatches.end() ? ", Exact Match" : "")
                     << ")" << endl;
            }
        }
    }
};

class TrieNode {
public:
    std::unordered_map<char, std::unique_ptr<TrieNode>> children;
    bool is_end_of_word = false;
};

class Trie {
private:
    std::unique_ptr<TrieNode> root;

    void findSuggestions(TrieNode* node, std::string prefix, std::vector<std::string>& suggestions) {
        if (node->is_end_of_word) {
            suggestions.push_back(prefix);
        }

        for (auto& child : node->children) {
            findSuggestions(child.second.get(), prefix + child.first, suggestions);
        }
    }

public:
    Trie() : root(std::make_unique<TrieNode>()) {}

    void insert(const std::string& word) {
        TrieNode* current = root.get();
        for (char c : word) {
            if (!current->children.count(c)) {
                current->children[c] = std::make_unique<TrieNode>();
            }
            current = current->children[c].get();
        }
        current->is_end_of_word = true;
    }

    std::vector<std::string> autocomplete(const std::string& prefix) {
        TrieNode* current = root.get();
        std::vector<std::string> suggestions;

        for (char c : prefix) {
            if (current->children.count(c)) {
                current = current->children[c].get();
            } else {
                return suggestions;
            }
        }

        findSuggestions(current, prefix, suggestions);
        return suggestions;
    }
};

void setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void setTextColor(WORD color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
int main() {
    SearchEngine engine;
    engine.addPage(WebPage("https://example.com", "This is an example page with a search engine."));
    engine.addPage(WebPage("https://example2.com", "Another example page with algorithms and data."));
    engine.addPage(WebPage("https://example3.com", "The news are spreading quickly. Search engines use algorithms."));
    engine.addPage(WebPage("https://example4.com", "Exact match test: example search"));

    Trie trie;
    trie.insert("apple");
    trie.insert("application");
    trie.insert("appreciate");
    trie.insert("appliance");
    trie.insert("banana");
    trie.insert("band");
    trie.insert("cat");
    trie.insert("caterpillar");
    trie.insert("search");
    trie.insert("engine");


    std::string input;
    int suggestion_index = 0;
    std::vector<std::string> suggestions;

    std::cout << "Type a sentence: ";
    std::string last_word="";
    while(true) {
        char ch = _getch();

        if(ch == 13) {
            cout << endl;
            engine.search(input);
            break;
        } 
        else if(ch == '\b') {
            if (!input.empty()) {
                input.pop_back();
            }
        } 
        else if(ch == 0 || ch == -32) {
            ch = _getch();
            if (ch == 72 && !suggestions.empty()) {
                suggestion_index = (suggestion_index + suggestions.size() - 1) % suggestions.size();
            } else if (ch == 80 && !suggestions.empty()) {
                suggestion_index = (suggestion_index + 1) % suggestions.size();
            }
        }
        else if(ch=='\t') {
            input += suggestions[suggestion_index].substr(last_word.size());
        } 
        else if(isprint(ch)) {
            input += ch;
        }

        setCursorPosition(0, 2);
        std::cout << std::string(50, ' ');
        setCursorPosition(0, 2);

        
        size_t pos = input.find_last_of(' ');
        if (pos != std::string::npos) {
            last_word = input.substr(pos + 1);
        } else {
            last_word = input;
        }

        suggestions = trie.autocomplete(last_word);

        std::cout << "Type a sentence: " << input;

        if (!suggestions.empty() && suggestion_index < suggestions.size()) {
            setTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            std::cout << suggestions[suggestion_index].substr(last_word.size());
            setTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        }

    }





    

    return 0;
}
