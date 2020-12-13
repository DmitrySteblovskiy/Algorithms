#include <iostream>
#include <map>
#include <string>
#include <vector>

// Даны строки s и t. Постройте сжатое суффиксное дерево, которое содержит все суффиксы строки s и строки t. Найдите такое дерево, которое содержит минимальное количество вершин
// https://contest.yandex.ru/contest/20019/run-report/45159825/

class SuffixTree {
public:

    struct Vertex {
        int left;
        int right;
        int link;
        int parent;
        std::map<char, int> next;
 
        Vertex(int l = -1, int r = -1, int par = -1) : left(l), right(r), parent(par), link(0) {}
    };

    struct State {    // здесь node - вершина текущего суффикса, а pos - положение в строке соответствующее месту на ребре от tree_[v].left до tree_[v].right
        int node;
        int pos;
        State(int node, int pos) : node(node), pos(pos)  {}
    };

    SuffixTree(const std::string& source, int fLen) : first_len_(fLen), St_(0, 0), source_str_(source), size_(2), current_pos_(0), source_len_(static_cast<int>(source.length())) {
        tree_.resize(source_len_ * 2);
        Vertex root;
        tree_[0] = root;

        BuildTree();
    }


    void AddLetter(char ch) {  // добавление новой буквы с соблюдением правил сжатого суфф. дерева
        if (St_.pos > tree_[St_.node].right) {
            while ((tree_[St_.node].next.find(ch) == tree_[St_.node].next.end())) {
                tree_[St_.node].next[ch] = size_;
                tree_[size_].left = current_pos_;
                tree_[size_++].parent = St_.node;
                St_.node = tree_[St_.node].link;
                St_.pos = tree_[St_.node].right + 1;
                if (St_.pos <= tree_[St_.node].right) {
                    break;
                }
            }

        if (St_.pos > tree_[St_.node].right) {
                St_.node = tree_[St_.node].next[ch];
                St_.pos = tree_[St_.node].left;
            }
        }

        FixingSuffTree(ch);
    }

    void FixingSuffTree(char ch) {
        if ((St_.pos == -1) || (source_str_[St_.pos] == ch)) {
            ++St_.pos;
        } else {
            tree_[size_] = Vertex(tree_[St_.node].left, St_.pos - 1, tree_[St_.node].parent);
            tree_[size_].next[source_str_[St_.pos]] = St_.node;
            tree_[size_].next[ch] = size_ + 1;
            tree_[size_+1].left = current_pos_;
            tree_[size_+1].parent = size_;

            tree_[St_.node].left = St_.pos;
            tree_[St_.node].parent = size_;
            tree_[tree_[size_].parent].next[source_str_[ tree_[size_].left] ] = size_;
            ++ ++size_;

            St_.node = tree_[tree_[size_-2].parent].link;
            St_.pos = tree_[size_-2].left;

            while (tree_[size_-2].right >= St_.pos) {
                St_.node = tree_[St_.node].next[source_str_[St_.pos]];
                St_.pos += 1 + tree_[St_.node].right - tree_[St_.node].left;
            }

            if (St_.pos == tree_[size_-2].right + 1) {
                tree_[size_-2].link = St_.node;
            }
            else {
                tree_[size_-2].link = size_;
            }

            St_.pos = tree_[St_.node].right - (St_.pos - tree_[size_-2].right) + 2;
            AddLetter(ch);
        }
    }


    void PrintVertices(int vertexNum) {      // поиск в глубину - здесь заодно выводим описания вершин дерева
        used_[vertexNum] = true;
        if (vertexNum) {
            re_num_[vertexNum] = iter_;
            ++iter_;
            
            std::cout << re_num_[tree_[vertexNum].parent] << " ";
 
            if (tree_[vertexNum].left < first_len_) {
                std::cout << 0 << " ";
                std::cout << tree_[vertexNum].left << " " << (tree_[vertexNum].right < first_len_ ? tree_[vertexNum].right + 1 : first_len_) << "\n";
        } else {
                std::cout << 1 << " ";
                std::cout << tree_[vertexNum].left - first_len_ << " " <<  tree_[vertexNum].right - first_len_ + 1 << "\n";
            }
        }
 
        for (auto elem : tree_[vertexNum].next) {
            if (!used_[elem.second])
                PrintVertices(elem.second);
            }
        }
    
    void Answer() {     // выводим количество вершин дерева
        iter_ = 1;
        used_.resize(size_, false);
        re_num_.resize(size_, 0);
        re_num_[0] = 0;

        std::cout << size_ - 1 << "\n";

        PrintVertices(0);
        iter_ = 1;
    }

private:

    size_t size_;
    int iter_;
    int current_pos_;
    int source_len_;
    int first_len_;

    std::vector<int> re_num_;    
    std::string source_str_;
    std::vector<bool> used_;    
    std::vector<Vertex> tree_;

    State St_;

    void BuildTree() {  // построение
        for (size_t i = 2; i < tree_.size(); ++i) {
            tree_[i].right = source_len_ - 1;
        }
 
        for (char ch = 'a'; ch <= 'z'; ++ch) {
            tree_[1].next[ch] = 0;
        }
        tree_[1].next['#'] = 0;
        tree_[1].next['$'] = 0;
        tree_[0].link = 1;
 
        for (int i = 0; i < source_len_; ++i) {
            AddLetter(source_str_[i]);
            ++current_pos_;
        }
    }
};


int main() {
    std::ios::sync_with_stdio(false);

    std::string str1, str2, answ;
    std::cin >> str1 >> str2;
 
    answ = str1 + str2;
 
    SuffixTree sufTree(answ, static_cast<int>(str1.length()));
    sufTree.Answer();
 
    //system("pause");
}
