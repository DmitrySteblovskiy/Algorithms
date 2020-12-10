#include <iostream>
#include <vector>

// Строка называется палиндромом, если она одинаково читается как слева направо, так и справа налево. Например, «abba» – палиндром, а «roman» – нет.
// Для заданной строки s длины n (1 ≤ n ≤ 105) требуется подсчитать число пар (i, j), 1 ≤ i < j ≤ n, таких что подстрока s[i..j] является палиндромом.
// https://contest.yandex.ru/contest/19772/run-report/44045136/
//
// Для каждой позиции i = 0...(n-1) найдём значения res, обозначающие количество палиндромов соответственно нечётной и чётной длины (зависит от evenness) с центром в позиции i. 
// Тогда чечевидно, что искомым число подстрок-палиндромов будет сумма всех элементов двух векторов за вычетом числа палиндромов длиной в одну букву (их - str.length()).

long Palindroms (std::string str, bool evenness) {
    long len = str.length();
    std::vector<long> res(len);   // для подпалиндромов

    long answer = 0;
    long left = 0;
    long right = -1;

    for (long i = 0; i < len; ++i) {
        long k = i > right ? (!evenness) : std::min (res[left + right - i + static_cast<long>(evenness)], right - i + 1);
        while (((i + k) < len) && ((i - k - static_cast<long>(evenness)) >= 0) && (str[i + k] == str[i - k - static_cast<long>(evenness)])) {
            ++k;
        }
        res[i] += k;

        if (i + k - 1 > right) {
            left = i - k + static_cast<long>(!evenness);
            right = i + k - 1;
        }
        answer += res[i];
    }

    return answer;
}

long Solve (std::string str) {
    long answer = 0;
    long len = str.length();

    answer += Palindroms(str, 0);   // для нечётных (evenness == 0)
    answer += Palindroms(str, 1);   // для чётных (evenness == 1)

    return (answer - len);
}


int main() {
    std::string str;
    std::cin >> str;

    std::cout << Solve(str);

    //system("pause");
}
