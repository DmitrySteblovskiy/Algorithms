// Вам дан неориентированный граф, состоящий из n вершин. На каждой вершине записано число; число, записанное на вершине i, равно ai. Изначально в графе нет ни одного ребра.
// Вы можете добавлять ребра в граф за определенную стоимость. За добавление ребра между вершинами x и y надо заплатить ax + ay монет. Также существует m специальных 
// предложений, каждое из которых характеризуется тремя числами x, y и w, и означает, что можно добавить ребро между вершинами x и y за w монет. Эти специальные предложения не 
// обязательно использовать: если существует такая пара вершин x и y, такая, что для нее существует специальное предложение, можно все равно добавить ребро между ними за ax + ay монет.
// Сколько монет минимально вам потребуется, чтобы сделать граф связным? Граф является связным, если от каждой вершины можно добраться до любой другой вершины, используя только ребра этого графа.
// https://contest.yandex.ru/contest/17986/run-report/33144513/
//
// Нам достаточно рассмотреть только "специальные" рёбра, и те рёбра, которые соединены с min вершиной. 
// Пока граф становится связным, функция min_weight ищет минимальное количество монет, которое пришлось потратить на этот процесс.

#include <iostream>
#include <queue>
#include <limits>
#include <vector>


const size_t longest = std::numeric_limits<size_t>::max();

size_t Min_(size_t e1, size_t e2) {
	if (e1 <= e2) {
		return e1;
	}
	else {
		return e2;
	}
}

size_t Max_(size_t e1, size_t e2) {
	if (e1 >= e2) {
		return e1;
	}
	else {
		return e2;
	}
}

class Edge {
public:
	size_t dist = longest;
	size_t size_;
	size_t number;

	Edge(size_t numb, size_t distance) : number(numb), dist(distance) {

	}
};

class compare {
public:
	int operator()(const Edge &e1, const Edge &e2) {
		return e1.dist > e2.dist;
	}
};

class Graph {
public:
	std::vector<std::vector<std::pair<size_t, size_t>>> graph;

	Graph(size_t n) : graph(n) {}

	void new_(size_t from, size_t to, size_t weight) {
		if (to != -1)
			graph[from].emplace_back(std::make_pair(to, weight));
	}

	size_t size() const {
		return graph.size();
	}

};

size_t min_weight(const Graph &graph, std::priority_queue<Edge, std::vector<Edge>, compare> &queue, std::vector<size_t> dist, size_t n) {
	size_t res = 0;
	std::vector<size_t> used(n, 0);
	used[0] = 1;
	while (!queue.empty()) {
		Edge curr = queue.top();
		queue.pop();
		if (curr.dist == dist[curr.number]) {
			res += curr.dist;
			for (auto neighbor : graph.graph[curr.number]) {
				size_t sec = neighbor.second;
				if (!used[neighbor.first] && sec < dist[neighbor.first]) {
					dist[neighbor.first] = sec;
					queue.push(Edge(neighbor.first, sec));
				}
			}
			used[curr.number] = 1;
		}
	}

	return res;
}

void answer(size_t n, size_t m) {
	Graph graph_ = Graph(n);
	std::priority_queue<Edge, std::vector<Edge>, compare> queue;
	std::vector<size_t> min_dists(n, longest);
	min_dists[0] = 0;
	std::vector<size_t> a = std::vector<size_t>(n);
	for (size_t i = 0; i < n; ++i) {
		std::cin >> a[i];
	}
	size_t min = 0;
	for (size_t i = 0; i < n; ++i) {
		if (a[i] < a[min]) {
			min = i;
		}
	}

	for (size_t i = 0; i < n; ++i) {
		if (i != min) {
			graph_.new_(min, i, a[min] + a[i]);
			graph_.new_(i, min, a[min] + a[i]);
			if (!min) {
				min_dists[i] = Min_(min_dists[i], a[min] + a[i]);
			}
		}
		else {
			graph_.new_(min, min, longest);
		}
	}

	min_dists[min] = Min_(min_dists[min], a[0] + a[min]);
	for (size_t i = 0; i < m; ++i) {
		size_t a1, a2, weight;
		std::cin >> a1 >> a2 >> weight;
		size_t from = Min_(--a1, --a2);
		size_t to = Max_(a1, a2);

		if ((from == min) || (to == min)) {
			graph_.graph[min][(from == min ? to : from)].second =
				Min_(graph_.graph[min][(from == min ? to : from)].second, weight);
			graph_.graph[(from == min ? to : from)][0].second =
				Min_(graph_.graph[(from == min ? to : from)][0].second, weight);
		}
		else {
			graph_.new_(from, to, Min_(weight, a[from] + a[to]));
			graph_.new_(to, from, Min_(weight, a[from] + a[to]));
		}
		if (!from) {
			min_dists[to] = Min_(min_dists[to], Min_(weight, a[from] + a[to]));
		}
	}

	for (size_t i = 1; i < n; ++i) {
		queue.emplace(Edge(i, min_dists[i]));
	}

	std::cout << min_weight(graph_, queue, min_dists, n);
}


int main() {
	size_t n, m;
	std::cin >> n >> m;

	answer(n, m);

	system("pause");
}