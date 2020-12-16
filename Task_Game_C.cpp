#include <algorithm>
#include <cmath>
#include <cstdio>
#include <functional>
#include <iostream>
#include <iomanip>
#include <vector>
#include <set>
#include <utility>

// Штирлиц и Мюллер стреляют по очереди. В очереди n человек, стоящих друг за другом. Каждым выстрелом убивается один из стоящих.
// Кроме того, если у кого-то из стоящих в очереди убиты все его соседи, то этот человек в ужасе убегает. Проигрывает тот, кто не может ходить.
// Первым стреляет Штирлиц. Требуется определить, кто выиграет при оптимальной игре обеих сторон, и если победителем будет Штирлиц, то найти все возможные первые ходы, ведущие к его победе.
// https://contest.yandex.ru/contest/23297/run-report/45571009/

int Mex(std::vector<size_t> &base_cases) {
    std::set<int> set_;
    for(size_t i = 0; i < base_cases.size(); ++i) {
        set_.insert(base_cases[i]);
    }
    for (size_t i = 0; ; ++i) {
        if (!set_.count(i)) {
            return i;
        }
    }
}

int Grandi(int n, std::vector<int> &temp) {
    std::vector<size_t> sequence_grandi;

    if (temp[n] >= 0) {
        return temp[n];
    }
    if (n >= 3) {
        sequence_grandi.emplace_back(Grandi(n - 1, temp));
    }
    if (n >= 4) {
        sequence_grandi.emplace_back(Grandi(n - 2, temp));
    }
    if (n >= 5) {
        sequence_grandi.emplace_back(Grandi(n - 1, temp));

        std::vector<int> first_(n - 4);
        std::vector<int> sec_(n - 4);
        std::vector<int> res(n - 4);

        for (size_t i = 2; i < n - 2; ++i) {
            first_[i - 2] = Grandi(i, temp);
            sec_[i - 2] = Grandi(n - i - 1, temp);     
        }

        std::transform(first_.begin(), first_.end(), sec_.begin(), res.begin(), std::bit_xor<int>());

        for (size_t i = 2; i < n - 2; ++i) {
            sequence_grandi.emplace_back(res[i - 2]);
        }
    }
    temp[n] = Mex(sequence_grandi);

    return temp[n];
}

std::vector<int> OuterSchtzShots(int n, std::vector<int> &schtirlitz_answ, std::vector<int> &temp, bool begin_) {
    if (temp[n - 1] == 0) {
        schtirlitz_answ.emplace_back(static_cast<int>(begin_) + n * static_cast<int>(!begin_));
    }
    if (temp[n - 2] == 0) {
        schtirlitz_answ.emplace_back(2 * static_cast<int>(begin_) + (n - 1) * static_cast<int>(!begin_));
    }
    return schtirlitz_answ;
}


std::vector<int> Solve(int n, std::vector<int> &base_cases, std::vector<int> &temp) {
    if (n < 4) {
        base_cases[2] = true;

        if (n < 3) {
            base_cases[0] = 1;
            base_cases[1] = 2;
            return base_cases;
        }
        base_cases[0] = 2;
        return base_cases;
    }
    base_cases[2] = false;

    std::vector<int> schtirlitz_answ;

    OuterSchtzShots(n, schtirlitz_answ, temp, true);

    std::vector<int> first(n - 4);
    std::vector<int> second(n - 4);
    std::vector<int> result(n - 4);

    for (size_t i = 2; i < n - 2; ++i) {
        first[i - 2] = temp[i];
        second[i - 2] = temp[n - i - 1];     
    }

    std::transform(first.begin(), first.end(), second.begin(), result.begin(), std::bit_xor<int>());

    for (size_t i = 2; i < n - 2; ++i) {
        if (!result[i - 2]) {
            schtirlitz_answ.emplace_back(i + 1);
        }
    }

    OuterSchtzShots(n, schtirlitz_answ, temp, false);

    return schtirlitz_answ;
}


int main() {
    std::ios::sync_with_stdio(false);

    int n;
    std::cin >> n;

    std::vector<int> base_cases(3, 0);
    std::vector<int> temp(n + 1, -1);
    std::vector<int> schtirlitz_answ;

    if (n < 4) {
        base_cases = Solve(n, base_cases, temp);

        std::cout << "Schtirlitz" << "\n";

        for (size_t i = 0; i < 2; ++i) {
            if (base_cases[i]) {
                std::cout << base_cases[i] << "\n";
            }
        }
    } else {
        temp[0] = 0;
        temp[1] = 0;
        temp[2] = 1;
        temp[3] = 2;

        if (Grandi(n, temp)) {
            std::cout << "Schtirlitz" << "\n";

            schtirlitz_answ = Solve(n, base_cases, temp);

            for (size_t i = 0; i < schtirlitz_answ.size(); ++i) {     // номера людей в очереди
                std::cout << schtirlitz_answ[i] << "\n";
            }
        } else {
            std::cout << "Mueller";
        }
    }

    //system("pause");
}