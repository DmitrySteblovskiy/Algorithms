#include <iostream>
#include <map>
#include <string>
#include <vector>

// Даны строки s и t. Постройте сжатое суффиксное дерево, которое содержит все суффиксы строки s и строки t. Найдите такое дерево, которое содержит минимальное количество вершин
// https://contest.yandex.ru/contest/20019/run-report/36980825/

class SuffixTree {
public:

    struct Vertex {
        int left;
        int right;
        int link;
        int parent;
        std::map<char, int> next;
 
        Vertex (int l = -1, int r = -1, int par = -1) : left(l), right(r), parent(par), link(0) {
            
        }
    };

    struct State {    // здесь node - вершина текущего суффикса, а pos - положение в строке соответствующее месту на ребре от tree[v].left до tree[v].right
        int node;
        int pos;
        State (int node, int pos) : node(node), pos(pos)  {
            
        }
    };

    void buildTree() {  // построение
        for (size_t i = 2; i < tree.size(); ++i) {
            tree[i].right = sourceLen - 1;
        }
 
        for (char ch = 'a'; ch <= 'z'; ++ch) {
            tree[1].next[ch] = 0;
        }
        tree[1].next['#'] = 0;
        tree[1].next['$'] = 0;
        tree[0].link = 1;
 
        for (int i = 0; i < sourceLen; ++i) {
            addLetter(sourceStr[i]);
            ++currentPos;
        }
    }

    SuffixTree (const std::string& source, int fLen) : firstLen(fLen), St(0, 0), sourceStr(source), size(2), currentPos(0), sourceLen(static_cast<int>(source.length())) {
        tree.resize(sourceLen * 2);
        Vertex root;
        tree[0] = root;

        buildTree();
    }


    void addLetter (char ch) {  // добавление новой буквы с соблюдением правил сжатого суфф. дерева
        if (St.pos > tree[St.node].right) {
            while ((tree[St.node].next.find(ch) == tree[St.node].next.end())) {
                tree[St.node].next[ch] = size;
                tree[size].left = currentPos;
                tree[size++].parent = St.node;
                St.node = tree[St.node].link;
                St.pos = tree[St.node].right + 1;
                if (St.pos <= tree[St.node].right) {
                    break;
                }
            }

        if (St.pos > tree[St.node].right) {
                St.node = tree[St.node].next[ch];
                St.pos = tree[St.node].left;
            }
        }

        addLetter2 (ch);
    }

    void addLetter2 (char ch) {
        if ((St.pos == -1) || (sourceStr[St.pos] == ch)) {
            ++St.pos;
        } else {
            tree[size] = Vertex(tree[St.node].left, St.pos - 1, tree[St.node].parent);
            tree[size].next[sourceStr[St.pos]] = St.node;
            tree[size].next[ch] = size + 1;
            tree[size+1].left = currentPos;
            tree[size+1].parent = size;

            tree[St.node].left = St.pos;
            tree[St.node].parent = size;
            tree[tree[size].parent].next[sourceStr[ tree[size].left] ] = size;
            ++ ++size;

            St.node = tree[tree[size-2].parent].link;
            St.pos = tree[size-2].left;

            while (tree[size-2].right >= St.pos) {
                St.node = tree[St.node].next[sourceStr[St.pos]];
                St.pos += 1 + tree[St.node].right - tree[St.node].left;
            }

            if (St.pos == tree[size-2].right + 1) {
                tree[size-2].link = St.node;
            }
            else {
                tree[size-2].link = size;
            }

            St.pos = tree[St.node].right - (St.pos - tree[size-2].right) + 2;
            addLetter (ch);
        }
    }


    void DFS (int vertexNum) {      // поиск в глубину - здесь заодно выводим описания вершин дерева
        used[vertexNum] = true;
        if (vertexNum) {
            reNum[vertexNum] = iter;
            ++iter;
            
            std::cout << reNum[tree[vertexNum].parent] << " ";
 
            if (tree[vertexNum].left < firstLen) {
                std::cout << 0 << " ";
                std::cout << tree[vertexNum].left << " " << (tree[vertexNum].right < firstLen ? tree[vertexNum].right + 1 : firstLen) << "\n";
        } else {
                std::cout << 1 << " ";
                std::cout << tree[vertexNum].left - firstLen << " " <<  tree[vertexNum].right - firstLen + 1 << "\n";
            }
        }
 
        for (auto elem : tree[vertexNum].next) {
            if (!used[elem.second])
                DFS(elem.second);
            }
        }
    
    void Answer() {     // выводим количество вершин дерева
        iter = 1;
        used.resize(size, false);
        reNum.resize(size, 0);
        reNum[0] = 0;

        std::cout << size - 1 << "\n";

        DFS(0);
    }

private:

    size_t size;
    int iter;
    int currentPos;
    int sourceLen;
    int firstLen;
    
    std::vector<int> reNum;    
    std::string sourceStr;
    std::vector<bool> used;    
    std::vector<Vertex> tree;

    State St;
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