//  У художника-авангардиста есть полоска разноцветного холста. За один раз он перекрашивает некоторый отрезок полоски в некоторый цвет. 
//После каждого перекрашивания специально обученный фотограф делает снимок части получившегося творения для музея современного искусства. 
//Для правильного экспонирования требуется знать яркость самого темного цвета на выбранном фотографом отрезке.
// Напишите программу для определения яркости самого темного цвета на отрезке.
// https://contest.yandex.ru/contest/18281/run-report/33103746/
//
// Построили SegmentTree с поддержкой минимума, который считается в countAnswer (искомая brightness это SegmentTree::min на фотографируемом отрезке).

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>


class SegmentTree {
public:
	int n;
	const int number = std::numeric_limits<int>::max();
	std::vector<int> tree;
	std::vector<int> new_;
	std::vector<int> sequence;


	void push(int numb, int left, int right) {
		if (new_[numb] != number) {
			tree[numb] = new_[numb];
			if (left != right) {
				int step = numb * 2 + 1;
				new_[step] = new_[numb];
				new_[step + 1] = new_[numb];
			}

			new_[numb] = number;
		}
	}

	void makeTree(int numb, int left, int right) {
		if (left == right) {
			tree[numb] = sequence[left];
		}
		else {
			int mid = (left + right) / 2;
			int step = numb * 2 + 1;
			makeTree(step, left, mid);
			makeTree(step + 1, mid + 1, right);

			if (tree[step] <= tree[step + 1]) {
				tree[numb] = tree[step];
			}
			else {
				tree[numb] = tree[step + 1];
			}
		}
	}

	void update(int numb, int left, int right, int l, int t, int num) {
		push(numb, left, right);
		if ((t < left) || (right < l)) {
			return;
		}
		if ((l <= left) && (right <= t)) {
			new_[numb] = num;
			push(numb, left, right);
			return;
		}
		int mid = left + (right - left) / 2;
		int step = numb * 2 + 1;

		update(step, left, mid, l, t, num);
		update(step + 1, mid + 1, right, l, t, num);

		if (tree[step] <= tree[step + 1]) {
			tree[numb] = tree[step];
		}
		else {
			tree[numb] = tree[step + 1];
		}
	}

	int min(int numb, int left, int right, int l, int t) {
		push(numb, left, right);

		if ((left > t) || (right < l)) {
			return number;
		}

		if ((l <= left) && (right <= t)) {
			return tree[numb];
		}

		int mid = left + (right - left) / 2;
		int step = numb * 2 + 1;
		int child1 = SegmentTree::min(step, left, mid, l, t);
		int child2 = SegmentTree::min(step + 1, mid + 1, right, l, t);

		if (child1 <= child2) {
			return child1;
		}
		else {
			return child2;
		}
	}

	void countAnswer() {
		std::cin >> n;
		tree = std::vector<int>(n * 4, number);
		new_ = std::vector<int>(n * 4, number);
		sequence = std::vector<int>(n);

		for (int i = 0; i < n; ++i) {
			unsigned int r, g, b;
			std::cin >> r >> g >> b;
			sequence[i] = r + g + b;
		}

		makeTree(0, 0, n - 1);
		int m;
		std::cin >> m;

		for (int i = 0; i < m; ++i) {
			unsigned int c, d, r, g, b, e, f;
			std::cin >> c >> d >> r >> g >> b >> e >> f;
			unsigned int color = r + g + b;

			update(0, 0, n - 1, c, d, color);

			int brightness = SegmentTree::min(0, 0, n - 1, e, f);

			std::cout << brightness << ' ';
		}
	}

};


int main() {
	SegmentTree tree;

	tree.countAnswer();

	system("pause");
}
