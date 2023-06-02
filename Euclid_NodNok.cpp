#include "algorithm"
#include "iostream"


size_t NOD(const size_t a, const size_t b) {
    if (a % b == 0)
        return b;
    if (b % a == 0)
        return a;

    if (a > b) {
        return NOD(a % b, b);
    }

    return NOD(a, b % a);
}

size_t NOK(size_t a, size_t b) {
    return (a * b) / NOD(a, b);
}

size_t comp(std::size_t nod, size_t newone) {
    return std::max(nod, newone);
}

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

    size_t n, a = 1, b = 1;
    std::cin >> n;
    size_t nod = 1;

    for (size_t i = 1; i <= n / 2; ++i) {
        if (nod < NOD(i, n - i)) {
            b = n - i;
        }
        nod = comp(nod, NOD(i, n - i));
    }

	std::cout << NOD(a, b) << "\n" << NOK(a, b);
}
