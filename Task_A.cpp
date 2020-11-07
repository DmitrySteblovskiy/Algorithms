#include <iostream>
#include <vector>

// Найдите все вхождения шаблона в строку. Длина шаблона – p, длина строки – n. Время O(n + p), доп. память – O(p). p ≤ 30000, n ≤ 300000. 
// Использовать один из методов: вариант 1 - с помощью префикс-функции; вариант 2 - с помощью z-функции.
// https://contest.yandex.ru/contest/19772/run-report/35138688/
//
// Сначала считаем префикс-функцию для шаблона (используя функцию pref_pattern), затем для всех символов строки (цикл ниже). В случае, если pref функция символа равна длине паттерна,
// то это и есть одно из "вхождений". В цикле находим все возможные вхождения, запоминая их номера, которые необходимо вывести.

std::vector<size_t> PrefPattern(const std::string &pattern, const std::string &str) {
    std::vector<size_t> patt(pattern.length());
    patt[0] = 0;

    for (size_t i = 1; i < pattern.length(); ++i) {
        size_t j = patt[i - 1];
        while ((pattern[i] != pattern[j]) && (j > 0)) {
            j = patt[j - 1];
        }
        if (pattern[j] != pattern[i]) {
            patt[i] = 0;
        }
        else {
            patt[i] = j + 1;
        }
    }
    return patt;
}

std::vector<size_t> FindEntries(const std::string &pattern, const std::string &str) {
        std::vector<size_t> patt(pattern.length());
        patt = PrefPattern(pattern, str);
    
    std::vector<size_t> entries;
    size_t prev_patt = 0;

    for (size_t i = 0; i < str.size(); ++i) {
        size_t j = prev_patt;
        while ((j > 0) && (str[i] != pattern[j])) {
            j = patt[j - 1];
        }
        if (str[i] == pattern[j]) {
            ++j;
        }
        else {
            j = 0;
        }
        if (j == pattern.length()) {
            entries.push_back(i + 1 - pattern.length());
        }
        prev_patt = j;
    }
    return entries;
}

//size_t Solve(const std::string &pattern, const std::string &str, std::vector<size_t> &entries, size_t i) {
//        return i;
//}


int main() {
    std::string pattern, str;
    std::cin >> pattern >> str;

    std::vector<size_t> entries = FindEntries(pattern, str);

    for (size_t i : entries) {
        std::cout << i << " ";
    }
    //system("pause");
}
