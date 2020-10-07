#include <iostream>
#include <vector>

// Строка называется палиндромом, если она одинаково читается как слева направо, так и справа налево. Например, «abba» – палиндром, а «roman» – нет.
// Для заданной строки s длины n (1 ≤ n ≤ 105) требуется подсчитать число пар (i, j), 1 ≤ i < j ≤ n, таких что подстрока s[i..j] является палиндромом.
// https://contest.yandex.ru/contest/19772/run-report/35309109/
//
// Для каждой позиции i = 0...(n-1) найдём значения nechet[i] и chet[i], обозначающие количество палиндромов соответственно нечётной и чётной длины с центром в позиции i. 
// Тогда чечевидно, что искомым число подстрок-палиндромов будет сумма всех элементов двух векторов за вычетом числа палиндромов длиной в одну букву (их - str.length()).

long palindroms(std::string str, long len) {
    std::vector<long> nechet(len);   // для подпалиндромов нечётной длины
    long left = 0;
    long right = -1;

    for (long i = 0; i < len; ++i) {
        long k = i > right ? 1 : std::min (nechet[left + right - i], right - i + 1);
        while (((i + k) < len) && ((i - k) >= 0) && (str[i + k] == str[i - k])) {
            ++k;
        }
        nechet[i] = k;
        if (i + k - 1 > right) {
            left = i - k + 1;
            right = i + k - 1;
        }
    }

    std::vector<long> chet(len);   // для подпалиндромов чётной длины
    left = 0;
    right = -1;

    for (long  i = 0; i < len; ++i) {
        long k = i > right ? 0 : std::min (chet[left + right - i + 1], right - i + 1);
        while (i + k < len && i - k - 1 >= 0 && str[i + k] == str[i - k - 1]) {
            ++k;
        }
        chet[i] = k;
        if (i + k - 1 > right) {
            left = i - k,  right = i + k - 1;
        }
    }
    
    long answer = 0;
    for(long i = 0; i < len; ++i) {
        answer += nechet[i];
        answer += chet[i];
    }

    return (answer - len);
}


int main() {
    std::string str;
    std::cin >> str;
    long len = str.length();

    std::cout << palindroms(str, len);

    //system("pause");
}