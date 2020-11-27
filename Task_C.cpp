#include <iostream>
#include <limits.h>
#include <vector>

// Шаблон поиска задан строкой длины m, в которой кроме обычных символов могут встречаться символы “?”. Найти позиции всех вхождений шаблона в тексте длины n. 
// Каждое вхождение шаблона предполагает, что все обычные символы совпадают с соответствующими из текста, а вместо символа “?” в тексте встречается произвольный символ. 
// Гарантируется, что сам “?” в тексте не встречается.
// Время работы - O(n + m + Z), где Z - общее число вхождений подстрок шаблона “между вопросиками” в исходном тексте. m ≤ 5000, n ≤ 2000000.
//
// https://contest.yandex.ru/contest/19772/run-report/35195873/

int const ALPHABET_SIZE = 26; // онглийский алфавит

class AhoCorasickMachine {
private:

    struct Vertex {     // Структура, соответвующая вершине бора и создающая ее без исходящих вершин и суффиксных сыллок, не являющуюся концом шаблона
     // где parent - индекс родителя, а parent_char (символ на ребре) - данная вершина.
    
        Vertex(int parent, char parent_char): parent_char(parent_char), parent(parent), suffix_link(-1), is_pattern(false) {
            for (size_t j = 0; j < ALPHABET_SIZE; ++j) {
                edges[j] = -1;
                fast_move[j] = -1;
            }
        }

        int parent;
        char parent_char;
        bool is_pattern;    // флаг конца шаблона 

        int edges[ALPHABET_SIZE];   // исходящие вершины
    
        int fast_move[ALPHABET_SIZE];    // переходы из вершины
    
        int suffix_link;   // ссылка из данной вершины

        std::vector<int> indices_of_pattern;    // шаблоны, с которыми связана вершина
    };


    void AddPattern(const std::pair<int, int> &submaskPos, int pattIndex) {     // добавляет шаблон в бор; использует поле submaskPos пара <int, int> (начало и конец строки), // а pattIndex - номер шаблона.
        int current_vertex = 0;
        for (int i = submaskPos.first; i <= submaskPos.second; ++i) {
            char character = mask_[i] - 'a';
            if (trie_[current_vertex].edges[character] == -1) {
                trie_.emplace_back(Vertex(current_vertex, character));
                trie_[current_vertex].edges[character] = static_cast<int>((trie_.size() - 1));
            }
            current_vertex = trie_[current_vertex].edges[character];
        }
        trie_[current_vertex].is_pattern = true;
        trie_[current_vertex].indices_of_pattern.emplace_back(pattIndex);
    }

   
    void FindSubmaskPositions(const std::string &mask){     // находит подшаблоны без вопросов в маске, записывая результат в submask_positions_.
        std::pair<int, int> current_submask_pos;
        if (isalpha(mask[0])) {
            current_submask_pos.first = 0;
        }
        if (mask_[1] == '?' && isalpha(mask_[0])) {
            current_submask_pos.second = 0;
            submask_positions_.emplace_back(current_submask_pos);
        }

        for (int i = 1; i < mask_.length() - 1; ++i) {
            if (mask[i - 1] == '?' && isalpha(mask[i])) {
                current_submask_pos.first = i;
            }
            if (mask[i + 1] == '?' && isalpha(mask[i])) {
                current_submask_pos.second = i;
                submask_positions_.emplace_back(current_submask_pos);
            }
        }

        if (mask_[mask.length() - 2] == '?' && isalpha(mask[mask.length() - 1])) {
            current_submask_pos.first = static_cast<int>(mask.length() - 1);
        }
        if (isalpha(mask_[mask_.length() - 1])) {
            current_submask_pos.second = static_cast<int>(mask.length() - 1);
            submask_positions_.emplace_back(current_submask_pos);
        }
    }

    std::string mask_;
    std::vector<Vertex> trie_;  
    std::vector<std::pair<int, int>> submask_positions_;

    int GetSuffixLink(int index) {
        if (trie_[index].suffix_link == -1) {
            // если ссылка не определена и вершина - сын корня, то зададим ссылку на корень, если нет - ищем рекурсивно
            if (!trie_[index].parent) {
                trie_[index].suffix_link = 0;
            }
            else {
                trie_[index].suffix_link = GetLink(GetSuffixLink(trie_[index].parent), trie_[index].parent_char);
            }
        }
        return trie_[index].suffix_link;
    }


    int GetLink(int index, char character) {
        if (trie_[index].fast_move[character] == -1) {
            if (trie_[index].edges[character] != -1) {
                trie_[index].fast_move[character] = trie_[index].edges[character];
            }
            else if (!index) {
                trie_[index].fast_move[character] = 0;
            }
            else {
                trie_[index].fast_move[character] = GetLink(GetSuffixLink(index), character);
            }
        }
        return trie_[index].fast_move[character];
    }


public:

    AhoCorasickMachine(const std::string &mask) : trie_(1, Vertex(0, -1)), mask_(mask) {    // создает карася по подшаблонам без ? из маски
        trie_[0].suffix_link = 0;
        FindSubmaskPositions(mask);
        for (size_t i = 0; i < submask_positions_.size(); ++i) {
            AddPattern(submask_positions_[i], i);
        }
    }

    std::vector<int> FindMatches(const std::string &text) {    // ищет вхождения маски в тексте, возвращая vector индексов вхождений
        std::vector<int> entries(text.length());
         int v = 0;
    
        for (int i = 0; i < text.length(); ++i) {   // ищем вхождения всех подшаблонов и увеличиваем счетчик вхождений в индексе, соответвующем началу mask
            v = GetLink(v, text[i] - 'a');
            int u = v;

            do {
                if (trie_[u].is_pattern) {
                    for (size_t index = 0; index < trie_[u].indices_of_pattern.size(); ++index) {

                        int startIndex = i - submask_positions_[trie_[u].indices_of_pattern[index]].second
                            + submask_positions_[trie_[u].indices_of_pattern[index]].first;

                        if ((startIndex - submask_positions_[trie_[u].indices_of_pattern[index]].first >= 0) && 
                        (startIndex - submask_positions_[trie_[u].indices_of_pattern[index]].first + mask_.length() - 1 < text.length())) {
                        entries[startIndex - submask_positions_[trie_[u].indices_of_pattern[index]].first]++;
                        }
                    }
                }
                u = GetSuffixLink(u);
            } while (u != 0);
        }

        std::vector<int> result;    // если индекс равен количеству подшаблонов, он является вхождением подстроки
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i] == submask_positions_.size()) {
                result.emplace_back(i);
            }
        }

        return result;
    }
};

int Solve (std::string pattern, std::string text, size_t i) {
    int lenpat = pattern.length();
    int lentex = text.length();
    AhoCorasickMachine ahoCorasickMachine(pattern);

    std::vector<int> entries = ahoCorasickMachine.FindMatches(text);
    if (entries[i] <= (lentex - lenpat)) {
        return entries[i];
    }
    else {
        return;
    }
}


int main() {
    std::string pattern, text;
    std::cin >> pattern >> text;
    
    int lenpat = pattern.length();
    int lentex = text.length();
    AhoCorasickMachine ahoCorasickMachine(pattern);

    std::vector<int> entries = ahoCorasickMachine.FindMatches(text);
    for (size_t i = 0; i < entries.size(); ++i) {
        std::cout << Solve(pattern, text, i) << " ";
    }

    //system("pause");
}
