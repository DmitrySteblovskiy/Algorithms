#include <iostream>
#include <limits.h>
#include <vector>

// Шаблон поиска задан строкой длины m, в которой кроме обычных символов могут встречаться символы “?”. Найти позиции всех вхождений шаблона в тексте длины n. 
// Каждое вхождение шаблона предполагает, что все обычные символы совпадают с соответствующими из текста, а вместо символа “?” в тексте встречается произвольный символ. 
// Гарантируется, что сам “?” в тексте не встречается.
// Время работы - O(n + m + Z), где Z - общее число вхождений подстрок шаблона “между вопросиками” в исходном тексте. m ≤ 5000, n ≤ 2000000.
//
// https://contest.yandex.ru/contest/19772/run-report/45755010/

class SpecialPare {
public:
	int start, end;
	
	SpecialPare() = default;
};

class AhoCorasickMachine {
private:
    static const int ALPHABET_SIZE = 26;

    void AddPattern(SpecialPare submaskPos, int pattIndex) {     // добавляет шаблон в бор; использует поле submaskPos пара <int, int> (начало и конец строки), // а pattIndex - номер шаблона.
        int current_vertex = 0;
        for (int i = submaskPos.start; i <= submaskPos.end; ++i) {
            char character = mask_[i] - FIRST_TRIE_SYMB;
            if (trie_[current_vertex].edges[character] == -1) {
                trie_.emplace_back(Vertex(current_vertex, character));
                trie_[current_vertex].edges[character] = static_cast<int>((trie_.size() - 1));
            }
            current_vertex = trie_[current_vertex].edges[character];
        }
        trie_[current_vertex].is_pattern = true;
        trie_[current_vertex].indices_of_pattern.emplace_back(pattIndex);
    }

    std::string mask_;
    std::vector<SpecialPare> submask_positions_;

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

    struct Vertex {     // Структура, соответвующая вершине бора и создающая ее без исходящих вершин и суффиксных ссыллок, не являющуюся концом шаблона
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

    std::vector<Vertex> trie_;  

public:
    const char FIRST_TRIE_SYMB = 'a';

    void FindSubmaskPositions(const std::string &mask) {     // находит подшаблоны без вопросов в маске, записывая результат в submask_positions_.
        SpecialPare current_submask_pos;
        if (isalpha(mask[0])) {
            current_submask_pos.start = 0;
        }
        if (mask_[1] == '?' && isalpha(mask_[0])) {
            current_submask_pos.end = 0;
            submask_positions_.emplace_back(current_submask_pos);
        }

        for (int i = 1; i < mask_.length() - 1; ++i) {
            if (mask[i - 1] == '?' && isalpha(mask[i])) {
                current_submask_pos.start = i;
            }
            if (mask[i + 1] == '?' && isalpha(mask[i])) {
                current_submask_pos.end = i;
                submask_positions_.emplace_back(current_submask_pos);
            }
        }

        if (mask_[mask.length() - 2] == '?' && isalpha(mask[mask.length() - 1])) {
            current_submask_pos.start = static_cast<int>(mask.length() - 1);
        }
        if (isalpha(mask_[mask_.length() - 1])) {
            current_submask_pos.end = static_cast<int>(mask.length() - 1);
            submask_positions_.emplace_back(current_submask_pos);
        }
    }

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
            v = GetLink(v, text[i] - FIRST_TRIE_SYMB);
            int u = v;

            do {
                if (trie_[u].is_pattern) {
                    for (size_t index = 0; index < trie_[u].indices_of_pattern.size(); ++index) {

                        int startIndex = i - submask_positions_[trie_[u].indices_of_pattern[index]].end
                            + submask_positions_[trie_[u].indices_of_pattern[index]].start;

                        if ((startIndex - submask_positions_[trie_[u].indices_of_pattern[index]].start >= 0) && 
                        (startIndex - submask_positions_[trie_[u].indices_of_pattern[index]].start + mask_.length() - 1 < text.length())) {
                        entries[startIndex - submask_positions_[trie_[u].indices_of_pattern[index]].start]++;
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

std::vector<int> Solve(const std::string &pattern, const std::string &text) {
    int lenpat = pattern.length();
    int lentex = text.length();
    AhoCorasickMachine ahoCorasickMachine(pattern);

    std::vector<int> entries = ahoCorasickMachine.FindMatches(text);
    
    return entries;
}


int main() {
    std::ios::sync_with_stdio(false);
    
    std::string pattern, text;
    std::cin >> pattern >> text;

    int lenpat = pattern.length();
    int lentex = text.length();

    std::vector<int> entries = Solve(pattern, text);

    for (size_t i = 0; i < entries.size(); ++i) {
        if (entries[i] <= (lentex - lenpat)) {
            std::cout << entries[i] << " ";
        }
    }

    //system("pause");
}
