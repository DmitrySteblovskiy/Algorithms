#include <iostream>
#include <vector>

// Найдите все вхождения шаблона в строку. Длина шаблона – p, длина строки – n. Время O(n + p), доп. память – O(p). p ≤ 30000, n ≤ 300000. 
// Использовать один из методов: вариант 1 - с помощью префикс-функции; вариант 2 - с помощью z-функции.
// https://contest.yandex.ru/contest/19772/run-report/45748187/
//
// Сначала считаем префикс-функцию для шаблона (используя функцию pref_pattern), затем для всех символов строки (цикл ниже). В случае, если pref функция символа равна длине паттерна,
// то это и есть одно из "вхождений". В цикле находим все возможные вхождения, запоминая их номера, которые необходимо вывести.


size_t GetNextPi(const std::string &pattern, std::vector<size_t> &pi_func, size_t prev, size_t i, char cur_symbol, bool check_pref) {
    size_t j = prev;

    while ((pattern[j] != cur_symbol) && (j > 0)) {
        j = pi_func[j - 1];
    }

    if (cur_symbol == pattern[j]) {
        if (check_pref) {
            pi_func[i] = j + 1;
            return pi_func[i];
        } else {
            ++j;
            return j;
        }
    } else {
        return 0;
    }
}


std::vector<size_t> PrefPattern(const std::string &pattern, const std::string &str) {
    std::vector<size_t> pi_func(pattern.length());
    pi_func[0] = 0;

    for (size_t i = 1; i < pattern.length(); ++i) {
        pi_func[i] = GetNextPi(pattern, pi_func, pi_func[i - 1], i, pattern[i], 1);
    }

    return pi_func;
}

std::vector<size_t> FindEntries(const std::string &pattern, const std::string &str) {
    std::vector<size_t> pi_func(pattern.length());
    pi_func = PrefPattern(pattern, str);
    
    std::vector<size_t> entries;
    size_t prev_pi = 0;

    for (size_t i = 0; i < str.size(); ++i) {
        size_t j = prev_pi;

        j = GetNextPi(pattern, pi_func, prev_pi, i, str[i], 0);

        if (j == pattern.length()) {
            entries.emplace_back(i + 1 - pattern.length());
        }
        prev_pi = j;
    }

    return entries;
}


int main() {
	std::ios::sync_with_stdio(false);

    std::string pattern, str;
    std::cin >> pattern >> str;

    std::vector<size_t> entries = FindEntries(pattern, str);

    for (size_t i = 0; i < entries.size(); ++i) {
        std::cout << entries[i] << " ";
    }
    //system("pause");
}
