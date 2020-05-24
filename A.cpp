// Дано число N и последовательность из N целых чисел. Найти вторую порядковую статистику на заданных диапазонах.
// Для решения задачи используйте структуру данных Sparse Table. Требуемое время обработки каждого диапазона O(1). Время подготовки структуры данных.
// https://contest.yandex.ru/contest/18281/run-report/33084351/
// 
// С помощью Sparce Table можно хранить минимумы на всех отрезках, длины которых равны степеням двойки. Для RMQ считаем для длины отрезка l величину std::floor(log2(l)).
// Пусть теперь дан запрос (l,r). Заметим, что min(A[l],A[l+1],…,A[r])=min(ST[l][j],ST[r−2j+1][j]), где j=max{k∣2k⩽r−l+1}, то есть логарифм длины запрашиваемого отрезка, округленный вниз. 
// Но эту величину мы уже предпосчитали, поэтому запрос выполняется за O(1).

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>


class SparseTable {
private:

	class Cell {
	public:
		int min1 = 0;
		int min2 = 0;
	};

public:
	std::pair<int, int> minimums(Cell & cell1, Cell & cell2) {
		int min1 = ((vec[cell1.min1] < vec[cell2.min1]) ? cell1.min1 : cell2.min1);
		int min2;
		int min1_ex = cell1.min1 + cell2.min1 - min1;
		int min2_min1 = ((vec[cell1.min1] < vec[cell2.min1]) ? cell1.min2 : cell2.min2);


		if (min2_min1 != -1) {
			min2 = vec[min2_min1] < vec[min1_ex] ? min2_min1 : min1_ex;
			if (min1 == min2) {
				min2 = ((vec[cell1.min2] < vec[cell2.min2]) ? cell1.min2 : cell2.min2);
			}
		}
		else {
			min2 = min1_ex;
		}
		return std::make_pair(min1, min2);
	}

	void makeTable() {
		int deg2 = static_cast<int>(floor(std::log2(vec.size())) + 1);
		table.emplace_back(vec.size());
		for (size_t i = 0; i < vec.size(); ++i) {
			table[0][i].min1 = i;
			table[0][i].min2 = -1;
		}
		for (int i = 1; i < deg2; ++i) {
			int pow2 = static_cast<int>(pow(2, i));
			table.emplace_back(std::vector<Cell>(vec.size() - pow2 + 1));

			for (size_t j = 0; j < (vec.size() - pow2 + 1); ++j) {
				std::pair<int, int> mins = minimums(table[i - 1][j], table[i - 1][j + pow2 / 2]);
				table[i][j].min1 = mins.first;
				table[i][j].min2 = mins.second;
			}
		}
	}

	void outp() {
		for (auto & array : table) {
			for (auto & elem : array) {
				std::cout << '(' << elem.min1 << ' ' << elem.min2 << ") ";
			}
			std::cout << std::endl;
		}
	}
	std::vector<std::vector<Cell>> table;
	std::vector<int> vec;

	explicit SparseTable(std::vector<int> && array) : vec(array) {
		makeTable();
	};

	int second_min(int left, int right) {
		int curr = static_cast<int>(std::floor(std::log2(right - left + 1)));
		int degree = static_cast<int>(pow(2, curr));

		Cell & cell1 = table[curr][left];
		Cell & cell2 = table[curr][right - degree + 1];
		std::pair<int, int> id_min1_min2 = minimums(cell1, cell2);
		int answer = vec[minimums(cell1, cell2).second];

		return answer;
	}
};


int main() {
	int n, m, left, right;
	std::cin >> n >> m;
	std::vector<int> vec(n);

	for (int & elem : vec) {
		std::cin >> elem;
	}
	SparseTable table(std::move(vec));

	for (int i = 0; i < m; ++i) {
		std::cin >> left >> right;
		--left; --right;

		std::cout << table.second_min(left, right) << std::endl;
	}
}