#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

// Дана строка длины n. Найти количество ее различных подстрок. Используйте суффиксный массив.
// https://contest.yandex.ru/contest/20019/run-report/36863119/

struct suffix {     // Структура для хранения информации суффикса
    int index;      // Для сохранения исходного индекса
    int rank[2];    // Для сохранения рангов и рядомранг пары
};

int cmp(struct suffix a, struct suffix b) {     // Для std::sort
    return (a.rank[0] == b.rank[0]) ? (a.rank[1] < b.rank[1] ?1: 0) : (a.rank[0] < b.rank[0] ?1: 0);
}

std::vector<int> buildSuffixArray(std::string str, int n) {
    struct suffix suffixes[n];      // Структура для хранения суффиксов и их индексов
    // Сохраняем суффиксы и их индексы в массиве структур (она нужна для сортировки суффиксов в алф. порядке и сохранения индексов)
    for (int i = 0; i < n; ++i) {
        suffixes[i].index = i;
        suffixes[i].rank[0] = str[i] - 'a';
        suffixes[i].rank[1] = (((i+1) < n) ? (str[i + 1] - 'a') : -1);
    }

    std::sort(suffixes, n + suffixes, cmp);    // Сортировка суффиксов
    // На этом этапе все суффиксы отсортированы до первых 2 символов. Отсорт. суффиксы в соответствии с первыми 4 символами, затем первыми 8 и далее
    int ind[n];  // массив индексов в суффиксах

    for (int k = 4; k < n * 2; k = k * 2) {
        int rank = 0;
        int prev_rank = suffixes[0].rank[0];
        suffixes[0].rank[0] = rank;
        ind[suffixes[0].index] = 0;

        for (int i = 1; i < n; i++) {   
            if ((suffixes[i].rank[0] == prev_rank) && (suffixes[i].rank[1] == suffixes[i-1].rank[1])) {     // Если первый и следующий ранги совпадают
                prev_rank = suffixes[i].rank[0];
                suffixes[i].rank[0] = rank;
            } else {     // Иначе увеличиваем ранг
                prev_rank = suffixes[i].rank[0];
                suffixes[i].rank[0] = ++rank;
            }
            ind[suffixes[i].index] = i;
        }
   
        for (int i = 0; i < n; ++i) {       // Назначаем следующий ранг суффиксам
            int nextIndex = suffixes[i].index + k / 2;
            suffixes[i].rank[1] = ((nextIndex < n) ? suffixes[ind[nextIndex]].rank[0]: -1);
        }
        std::sort(suffixes, suffixes + n, cmp);   // Сортируем суффиксы по первым k символам
    }

    std::vector<int> suffixArr;       // Сохраняем индексы всех отсортированных суффиксов в суфф. массиве
    for (int i = 0; i < n; ++i) {
        suffixArr.emplace_back(suffixes[i].index);
    }
    return  suffixArr;
}

 

std::vector<int> Kasai(std::string str, std::vector<int> suffixArr) {   // Для построения и возврата LCP (алгос Касая)
    size_t n = suffixArr.size();
    std::vector<int> lcp(n, 0);
    std::vector<int> antiSuff(n, 0);      // Вспомогательный массив для хранения обратного суффиксного массива
    
    for (size_t i = 0; i < n; ++i) {
        antiSuff[suffixArr[i]] = static_cast<int>(i);
    }
    int k = 0;

    for (size_t i = 0; i < n; ++i) {    // Обрабатываем суффиксы, начиная с первого в str
        if (antiSuff[i] == static_cast<int>(n) - 1) {      // Если текущий суффикс = n-1, то lcp не определен для этой подстроки
            k = 0;
            continue;
        }

        int j = suffixArr[antiSuff[i] + 1];    // j - индекс след подстроки, нужный для сравнения с текущей подстрокой
        while ((i + k < n) && (j + k < n) && (str[i + k] == str[j + k])) {
            ++k;
        }
        lcp[antiSuff[i]] = k;    // lcp для текущего суффикса.

        if (k > 0) {    // Удаление начального символа из строки при необходимости
            --k;
        }
    }
    return lcp;
}

int countDistinctSubstr(std::string str) {
    size_t n = str.length();   
    std::vector<int> suffixArr = buildSuffixArray(str, n);       // заполняем массивы суффиксов и lcp
    std::vector<int> lcp = Kasai(str, suffixArr);
    int res = static_cast<int>(n) - suffixArr[0];

    for (size_t i = 1; i < lcp.size(); ++i) { 
        res += (n - suffixArr[i]) - lcp[i - 1];      // вычитаем lcp из длины суффикса
    }
    ++res;      // Для пустой строки
    return res;
}


int main() {
	std::ios::sync_with_stdio(false);
    std::string str;
    std::cin >> str;

    int answ = countDistinctSubstr(str);
    --answ;

    std::cout << answ;
    //system("pause");
}