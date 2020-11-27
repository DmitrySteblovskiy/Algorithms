#include <iostream>
#include <vector>

// Строка называется палиндромом, если она одинаково читается как слева направо, так и справа налево. Например, «abba» – палиндром, а «roman» – нет.
// Для заданной строки s длины n (1 ≤ n ≤ 105) требуется подсчитать число пар (i, j), 1 ≤ i < j ≤ n, таких что подстрока s[i..j] является палиндромом.
// https://contest.yandex.ru/contest/19772/run-report/35309109/
//
// Для каждой позиции i = 0...(n-1) найдём значения nechet[i] и chet[i], обозначающие количество палиндромов соответственно нечётной и чётной длины с центром в позиции i. 
// Тогда чечевидно, что искомым число подстрок-палиндромов будет сумма всех элементов двух векторов за вычетом числа палиндромов длиной в одну букву (их - str.length()).

long Palindroms(std::string str) {
    long len = str.length();
    std::vector<long> res(len, 0);   // для подпалиндромов нечётной длины
    long left = 0;
    long right = -1;
    bool evenness;

    for (long i = 0; i < len; ++i) {
        evenness = 0;

        do {
            long k = i > right ? (!evenness) : std::min (res[left + right - i + static_cast<long>(evenness)], right - i + 1);
            while (((i + k) < len) && ((i - k - static_cast<long>(evenness)) >= 0) && (str[i + k] == str[i - k - static_cast<long>(evenness)])) {
                ++k;
            }
            res[i] += k;

            if (i + k - 1 > right) {
                left = i - k + static_cast<long>(!evenness);
                right = i + k - 1;
            }
            evenness = !evenness;

        } while (evenness != false);
    }

    long answer = 0;
    for (long i = 0; i < len; ++i) {
        answer += res[i];
    }

    return (answer - len - 1);
}


int main() {
    std::string str;
    std::cin >> str;

    std::cout << Palindroms(str);

    //system("pause");
}
