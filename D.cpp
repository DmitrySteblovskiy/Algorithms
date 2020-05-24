// Дано дерево с корнем, содержащее (1 ≤ n ≤ 100 000) вершин, пронумерованных от 0 до n-1.
// Требуется ответить на m(1 ≤ m ≤ 10 000 000) запросов о наименьшем общем предке для пары вершин.
// Запросы генерируются следующим образом.Заданы числа a1, a2 и числа x, y и z.
// Числа a3, ..., a2m генерируются следующим образом : ai = (x ⋅ ai - 2 + y ⋅ ai - 1 + z) mod n.
// Первый запрос имеет вид(a1, a2).Если ответ на i - 1 - й запрос равен v, то i - й запрос имеет вид((a2i - 1 + v) mod n, a2i).
// https://contest.yandex.ru/contest/18281/run-report/33201255/
// 
//Рассмотрим участок от pos[a] до pos[b] включительно - в нем хранится хронология обхода от первого посещения a до первого посещения b, а значит в нем есть:
//все дети(и дети детей и т.д.) a и b, все предки a и b вплоть до первого общего предка. Следовательно, достаточно пройтись по участку от way[pos[a]] до way[pos[b]]
//и найти вершину с min depth, что будет LCA для a и b. Искомая величина(сумме номеров вершин) - sum в LCA.

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>


class Cell {
public:
	int depth, num;
	Cell(int num_ = 0, int depth_ = -1) : num(num_), depth(depth_) {}
	Cell(const Cell& elem) : num(elem.num), depth(elem.depth) {}

	Cell& operator = (const Cell& right) {
		num = right.num; depth = right.depth;
		return *this;
	}

	bool operator < (const Cell& right) const {
		return (depth < right.depth);
	}
};

class Graph {
private:
	int v_num;

	std::vector<std::vector<int>> buffer;


	friend void operator >> (std::istream& inp, Graph& graph) {
		for (size_t i = 1; i < graph.size(); i++) {
			int parent;
			inp >> parent;
			graph.addEdge(parent, i);
		}
	}

public:
	void addEdge(const int from, const size_t to) {
		buffer[from].emplace_back(to);
	}

	int size() const {
		return v_num;
	}

	Graph(int _v_num) : v_num(_v_num) {
		buffer.resize(v_num);
		std::cin >> *this;
	}

	void dfs(std::vector<int>& pos, std::vector<Cell>& way, const int cur_num, const int cur_depth) const {
		way.emplace_back(Cell(cur_num, cur_depth));
		pos[cur_num] = way.size() - 1;

		for (auto _next : buffer[cur_num]) {
			dfs(pos, way, _next, cur_depth + 1);
			way.emplace_back(Cell(cur_num, cur_depth));
		}

	}
};

class SparceTable {
private:
	int n, logn;
	std::vector<std::vector<Cell>> buffer;

	int log(const int value) const {
		if (value == 1) {
			return 0;
		}
		int answer = static_cast<int>(std::floor(std::log2(value)));

		return answer;
	}

public:
	const std::vector<std::vector<Cell>>& get_buffer() const {
		return buffer;
	}

	SparceTable(std::vector<Cell>& data) : n(data.size()) {

		buffer.assign(n, std::vector<Cell>());

		for (int i = 0; i < n; i++) {
			buffer[i].emplace_back(data[i]);
		}
		logn = log(n);

		for (int j = 1; j < logn + 1; ++j) {
			for (int i = 0; i < n; i++) {

				int right_border = i + (1 << j) - 1;
				if (right_border + 1 > n) {
					break;
				}
				int middle = i + (1 << (j - 1));

				buffer[i].emplace_back(std::min(buffer[i][j - 1], buffer[middle][j - 1]));
			}
		}
	}
};

class RMQ {
private:
	const std::vector<std::vector<Cell>>& ST;

	int log(const int value) const {
		if (value == 1) {
			return 0;
		}
		int answer = static_cast<int>(std::floor(std::log2(value)));

		return answer;
	}

public:
	RMQ(int m, const SparceTable& _Table) : ST(_Table.get_buffer()) {}

	Cell findMin(const int l, const int r) const {
		if (r >= l) {

			int j = log(r - l + 1);
			return std::min(ST[l][j], ST[r - (1 << j) + 1][j]);
		}
	}

	size_t size() const {
		return ST.size();
	}
};

class LCA {
private:
	int m;
	long long x, y, z, a, b, n;

	friend void operator >> (std::istream& inp, LCA& lca) {
		inp >> lca.a >> lca.b;
		inp >> lca.x >> lca.y >> lca.z;
	}

	int next_v() const {
		int answer = ((z + a * x + b * y) % n);
		return answer;
	}

public:
	void nextRequest() {
		a = next_v();
		std::swap(a, b);
		a = next_v();
		std::swap(a, b);
	}

	LCA(int m_, const Graph& graph) : m(m_), n(graph.size()) {
		int sum = 0;
		std::cin >> *this;
		std::vector<int> pos(n, -1);
		std::vector<Cell> way;
		graph.dfs(pos, way, 0, 0);

		SparceTable table(way);
		RMQ rmq(m, table);
		Cell ans(0, -1);

		for (int i = 0; i < m; i++) {
			int first_pos = pos[(a + ans.num) % n];
			int second_pos = pos[b];
			if (first_pos > second_pos) {
				std::swap(first_pos, second_pos);
			}

			ans = rmq.findMin(first_pos, second_pos);
			sum += ans.num;
			nextRequest();
		}

		std::cout << sum;
	}
};


int main() {
	int n, m;
	std::cin >> n >> m;
	Graph graph(n);
	LCA(m, graph);

	system("pause");
}
