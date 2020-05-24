// ООО "Новые Железные Дороги" поручило вам разработать систему бронирования билетов на новый маршрут поезда дальнего следования. 
// Маршрут проходит через N станций, занумерованных от 0 до N-1. Вместимость поезда ограничена.
// В систему бронирования последовательно приходят запросы от пассажиров с указанием номера начальной и конечной станции, а также количество билетов, которые пассажир хочет приобрести.
// https://contest.yandex.ru/contest/18281/run-report/33088304/
//
// Построили SegmentTree с поддержкой максимума, который проверяется в мейне (если максимум свободных мест превысится после обработки текущего вопроса, то выводим его номер, иначе update-м дерево)

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>


class SegmentTree {
private:
	std::vector<std::pair<int, int>> tree;

public:
	int size = 0;

	SegmentTree(const std::vector<int> &vec, const int n) {
		size = (1 << static_cast<int>(std::ceil(std::log2(n))));
		tree.assign(size * 2 - 1, std::make_pair(0, 0));
	}

	void make_tree(const std::vector<int> &vec) {

		for (size_t i = 0; i < vec.size(); ++i) {
			tree[(size * 2 - 1) / 2 + i].first = vec[i];
		}

		for (int i = ((size * 2 - 1) / 2 - 1); i >= 0; --i) {
			int step = 2 * i + 1;
			tree[i].first = std::max(tree[step].first, tree[step + 1].first);
		}
	}

	void update(int n, int left, int right, int f, int t, int num) {
		if (f > t) {
			return;
		}
		else if ((f == left) && (t == right)) {
			tree[n].second += num;
			return;
		}
		else {
			int step = n * 2 + 1;
			int mid = left + (right - left) / 2;

			update(step, left, mid, f, std::min(t, mid), num);
			update(step + 1, mid + 1, right, std::max(f, mid + 1), t, num);
			step = 2 * n + 1;
			tree[n].first = std::max(tree[step].first + tree[step].second, tree[step + 1].first + tree[step + 1].second);
		}
	}

	int max(int n, int left, int right, int f, int t) {
		if (f > t) {
			return 0;
		}
		else if ((f == left) && (t == right)) {
			int sum = tree[n].first + tree[n].second;
			return sum;
		}

		else {
			int mid = left + (right - left) / 2;
			int left_max = max(2 * n + 1, left, mid, f, std::min(mid, t));
			int right_max = max(2 * n + 2, mid + 1, right, std::max(mid + 1, f), t);
			int res = std::max(left_max, right_max) + tree[n].second;

			return res;
		}
	}
};


int main() {
	int n, number, m, from, to, quantity;
	std::cin >> n;

	std::vector<int> tickets(n);
	for (int i = 0; i < n - 1; ++i) {
		int tmp;
		std::cin >> tmp;
		tickets[i] = tmp;
	}

	std::cin >> number;
	SegmentTree tree(tickets, n);
	tree.make_tree(tickets);
	std::cin >> m;

	for (int i = 0; i < m; ++i) {
		std::cin >> from >> to >> quantity;

		if ((tree.max(0, 0, tree.size - 1, from, to - 1)) <= (number - quantity)) {
			tree.update(0, 0, tree.size - 1, from, to - 1, quantity);
		}
		else {
			std::cout << i << " ";
		}
	}

	system("pause");
}