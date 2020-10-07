#include <iostream>
#include <vector>
#include <limits.h>

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
     // где parent - индекс родителя, а parentChar (символ на ребре) - данная вершина.
    
        Vertex(int parent, char parentChar): parentChar(parentChar), parent(parent), suffixLink(-1), isPattern(false) {
            for (size_t j = 0; j < ALPHABET_SIZE; ++j) {
                edges[j] = -1;
                fastMove[j] = -1;
            }
        }

        int parent;
        char parentChar;
        bool isPattern;    // флаг конца шаблона 

        int edges[ALPHABET_SIZE];   // исходящие вершины
    
        int fastMove[ALPHABET_SIZE];    // переходы из вершины
    
        int suffixLink;   // ссылка из данной вершины

        std::vector<int> indicesOfPattern;    // шаблоны, с которыми связана вершина
    };


    void addPattern(const std::pair<int, int> &submaskPos, int pattIndex) {     // добавляет шаблон в бор; использует поле submaskPos пара <int, int> (начало и конец строки), // а pattIndex - номер шаблона.
        int current_vertex = 0;
        for (int i = submaskPos.first; i <= submaskPos.second; ++i) {
            char character = mask_[i] - 'a';
            if (bohr_[current_vertex].edges[character] == -1) {
                bohr_.emplace_back(Vertex(current_vertex, character));
                bohr_[current_vertex].edges[character] = static_cast<int>((bohr_.size() - 1));
            }
            current_vertex = bohr_[current_vertex].edges[character];
        }
        bohr_[current_vertex].isPattern = true;
        bohr_[current_vertex].indicesOfPattern.emplace_back(pattIndex);
    }

   
    void findSubmaskPositions(const std::string &mask){     // находит подшаблоны без вопросов в маске, записывая результат в submask_positions_.
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
    std::vector<Vertex> bohr_;  
    std::vector<std::pair<int, int>> submask_positions_;

    int getSuffixLink(int index){
        if (bohr_[index].suffixLink == -1) {
            // если ссылка не определена и вершина - сын корня, то зададим ссылку на корень, если нет - ищем рекурсивно
            if (!bohr_[index].parent) {
                bohr_[index].suffixLink = 0;
            }
            else {
                bohr_[index].suffixLink = getLink(getSuffixLink(bohr_[index].parent), bohr_[index].parentChar);
            }
        }
        return bohr_[index].suffixLink;
    }


    int getLink(int index, char character){
        if (bohr_[index].fastMove[character] == -1) {
            if (bohr_[index].edges[character] != -1) {
                bohr_[index].fastMove[character] = bohr_[index].edges[character];
            }
            else if (!index) {
                bohr_[index].fastMove[character] = 0;
            }
            else {
                bohr_[index].fastMove[character] = getLink(getSuffixLink(index), character);
            }
        }
        return bohr_[index].fastMove[character];
    }


public:

    AhoCorasickMachine(const std::string &mask) : bohr_(1, Vertex(0, -1)), mask_(mask) {    // создает карася по подшаблонам без ? из маски
        bohr_[0].suffixLink = 0;
        findSubmaskPositions(mask);
        for (size_t i = 0; i < submask_positions_.size(); ++i) {
            addPattern(submask_positions_[i], i);
        }
    }

    std::vector<int> findMatches(const std::string &text){    // ищет вхождения маски в тексте, возвращая vector индексов вхождений
        std::vector<int> entries(text.length());
         int v = 0;
    
        for (int i = 0; i < text.length(); ++i) {   // ищем вхождения всех подшаблонов и увеличиваем счетчик вхождений в индексе, соответвующем началу mask
            v = getLink(v, text[i] - 'a');
            int u = v;

            do {
                if (bohr_[u].isPattern) {
                    for (size_t index = 0; index < bohr_[u].indicesOfPattern.size(); ++index) {

                        int startIndex = i - submask_positions_[bohr_[u].indicesOfPattern[index]].second
                            + submask_positions_[bohr_[u].indicesOfPattern[index]].first;

                        if ((startIndex - submask_positions_[bohr_[u].indicesOfPattern[index]].first >= 0) && 
                        (startIndex - submask_positions_[bohr_[u].indicesOfPattern[index]].first + mask_.length() - 1 < text.length())) {
                        entries[startIndex - submask_positions_[bohr_[u].indicesOfPattern[index]].first]++;
                        }
                    }
                }
                u = getSuffixLink(u);
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


int main() {
    std::string pattern, text;
    std::cin >> pattern >> text;
    
    int lenpat = pattern.length();
    int lentex = text.length();
    AhoCorasickMachine ahoCorasickMachine(pattern);

    std::vector<int> entries = ahoCorasickMachine.findMatches(text);
    for (size_t i = 0; i < entries.size(); ++i) {
        if (entries[i] <= (lentex - lenpat)){
            std::cout << entries[i] << " ";
        }
        else {
            break;
        }
    }

    //system("pause");
}
