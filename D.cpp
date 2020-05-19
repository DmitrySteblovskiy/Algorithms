// Шреку необходимо пробраться в башню, чтобы спасти прекрасную принцессу Фиону. И сейчас его путь лежит через старый полуразвалившийся мост. 
//Осёл очень боится упасть с этого моста в лавовую реку под ним и отказывается идти дальше, пока мост не будет полностью починен. Шрек не может идти без друга и решил отремонтировать его.
//Мост представляет собой поле из n × m клеток, причем некоторые клетки ещё остались целыми.У Шрека есть только дощечки размера 1 × 2, установка которых занимает у него a секунд, и 1 × 1, установка которых занимает b секунд.
//Ему необходимо закрыть все пустые клетки, причем только их, не накладывая дощечки друг на друга.
//Определите, через какое минимальное количество секунд Шрек и Осёл смогут продолжить свой путь дальше.
// https://contest.yandex.ru/contest/17986/run-report/33076362/
//
// Проверяя соседей каждой пустой вершины, ищем максимальное число досок 2x1 - с помощью алгоритма Куна найдем максимальное парасочетание.

#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>


std::vector <std::vector <std::vector <std::pair<int, int>>>> ToGraph(const std::vector<std::string> &bridge, int n, int m) {
	std::vector <std::vector <std::vector <std::pair<int, int>>>> graph(n, std::vector <std::vector <std::pair<int, int>>>(m));
	char a = '*';
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			if (bridge[i][j] == '*') {
				if (i + 1 < n && bridge[i + 1][j] == a) {
					graph[i][j].push_back({ i + 1, j });
					graph[i + 1][j].push_back({ i, j });
				}
				if (j + 1 < m && bridge[i][j + 1] == a) {
					graph[i][j].push_back({ i, j + 1 });
					graph[i][j + 1].push_back({ i, j });
				}
			}
		}
	}

	return graph;
}


int check(int i, int j, int n, int m, std::vector <std::vector <std::vector <std::pair<int, int>>>> &graph,
	std::vector <std::vector <std::pair<int, int>>> &par, std::vector <std::vector <char>> &used) {
	if (used[i][j]) {

		return 0;
	}
	used[i][j] = 1;

	for (std::pair<int, int> to : graph[i][j]) {
		std::pair<int, int> next_vert = par[to.first][to.second];
		if ((next_vert == std::make_pair(-1, -1)) || (check(next_vert.first, next_vert.second, n, m, graph, par, used))) {
			par[to.first][to.second] = std::make_pair(i, j);

			return 1;
		}
	}
	return 0;
}


int Kuhn(std::vector <std::vector <std::vector <std::pair<int, int>>>> &graph, int n, int m) {
	std::vector <std::vector <std::pair<int, int>>> par(n, std::vector <std::pair<int, int>>(m, { -1, -1 }));
	std::vector <std::vector <char>> used(n, std::vector<char>(m, 0));
	int counter;
	counter = 0;

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			if (check(i, j, n, m, graph, par, used)) {
				used.assign(n, std::vector<char>(m, 0));
				++counter;
			}
			++j;
		}
		++i;
	}

	for (int i = 1; i < n; ++i) {
		for (int j = 1; j < m; ++j) {
			if (check(i, j, n, m, graph, par, used)) {
				++counter;
				used.assign(n, std::vector<char>(m, 0));
			}
			++j;
		}
		++i;
	}

	return counter;
}


int inp(int n, int m, std::vector<std::string> &bridge) {
	std::string s;
	int counter;
	counter = 0;
	for (int i = 0; i < n; ++i) {
		char a = '*';
		std::cin >> s;
		bridge.emplace_back(s);
		for (int j = 0; j < m; ++j) {
			if (s[j] == a) {
				++counter;
			}
		}
	}

	return counter;
}


int main() {
	int n, m, a, b;
	std::cin >> n >> m >> a >> b;
	std::vector<std::string> bridge;
	int empties = inp(n, m, bridge);

	if (b * 2 >= a) {
		std::vector <std::vector <std::vector <std::pair<int, int>>>> graph = ToGraph(bridge, n, m);
		int twoes = Kuhn(graph, n, m);
		std::cout << a * twoes + b * (empties - 2 * twoes);
	}
	else {
		std::cout << b * empties;
	}

	system("pause");
}