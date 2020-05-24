// Требуется найти в связном графе остовное дерево минимального веса. Воспользуйтесь алгоритмом Крускала.
// https://contest.yandex.ru/contest/17986/run-report/32773324/
//
// Строим систему непересекающихся множеств (DSU), на которой очень легко реализуется алгоритм Крускала с помощью priority_queue, чтоб не словить TL. Он выполнит свою задачу: найти MST


#include <iostream>
#include <queue>
#include <limits>
#include <set>
#include <vector>


class Edge {
public:
	size_t v1, v2, weight;
	Edge() {}
	Edge(size_t v_1, size_t v_2, size_t w) {
		v1 = v_1;
		v2 = v_2;
		weight = w;
	}
};

bool operator == (const Edge& e1, const Edge& e2) { return e1.weight == e2.weight; }
bool operator != (const Edge& e1, const Edge& e2) { return e1.weight != e2.weight; }
bool operator < (const Edge& e1, const Edge& e2) { return e1.weight < e2.weight; }
bool operator > (const Edge& e1, const Edge& e2) { return e1.weight > e2.weight; }

class DSU {
private:
	std::vector<size_t> parents;
	std::vector<size_t> size;

public:
	DSU(size_t n) {
		parents.resize(n);
		size.resize(n);

		for (size_t i = 0; i < n; ++i)
			make_set(i);
	}

	void make_set(size_t v) {
		parents[v] = v;
		size[v] = 1;
	}

	size_t find(size_t v) {
		if (v == parents[v]) {
			return v;
		}
		return parents[v] = find(parents[v]);
	}

	void merge(size_t a, size_t b) {
		a = find(a);
		b = find(b);
		if (a != b) {
			if (size[a] < size[b]) {
				std::swap(a, b);
			}
			parents[b] = a;
			size[a] += size[b];
		}
	}
};

size_t Kruskal(std::priority_queue <Edge, std::vector<Edge>, std::greater<Edge>> pq, size_t n) {
	DSU dsu(n + 1);
	Edge current = pq.top();
	pq.pop();
	size_t weight = current.weight;
	dsu.merge(current.v1, current.v2);

	while (!pq.empty()) {
		current = pq.top();
		pq.pop();
		if (dsu.find(current.v1) != dsu.find(current.v2)) {
			dsu.merge(current.v1, current.v2);
			weight += current.weight;
		}
	}
	return weight;
}


int main() {
	size_t n, m, v1, v2, weight;
	std::cin >> n >> m;
	std::priority_queue <Edge, std::vector<Edge>, std::greater <Edge>> pq;

	for (size_t i = 0; i < m; ++i) {
		std::cin >> v1 >> v2 >> weight;
		Edge e(v1, v2, weight);
		pq.push(e);
	}

	std::cout << Kruskal(pq, n);

	system("pause");
}