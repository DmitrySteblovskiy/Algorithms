#include <algorithm>
#include <iostream>
#include <math.h>
#include <queue>
#include <vector>
# Для каждой вершины (которая является полицейский) проверяю, есть ли у нее ребра и далее произойдет ли ситуация, 
#когда полицейский позвонит тому, от кого получил информацию (т.е. вершина уже использована в текущем поиске)

void dfs(const std::vector <std::vector <int>> &graph, std::vector<int> &answer, std::vector<int> &used, int node, bool &cycle_checker, std::vector<bool> &blocked) {

	used[node] = 1;
	
	for (size_t i = 0; i < graph[node].size(); ++i) {

		if ((used[graph[node][i]]) && (!blocked[graph[node][i]])) {
			cycle_checker = true;
			return;
		}

		if (!blocked[graph[node][i]]) {
			dfs(graph, answer, used, graph[node][i], cycle_checker, blocked);
		}
	}

	blocked[node] = true;
	answer.push_back(node);
}

void inversion(std::vector<int> &answer) {

	for (size_t i = 0; i < (answer.size() / 2); ++i) {
		std::swap(answer[i], answer[answer.size() - 1 - i]);
	}
}

void sort_graph(std::vector <std::vector <int>> &graph, int n):
		for (int i = 0; i < n; ++i) {
		std::sort(graph[i].begin(), graph[i].end());
	}

void make_dfs(const std::vector <std::vector <int>> &graph, std::vector<int> &answer, std::vector<int> &used, bool &cycle_checker, std::vector<bool> &blocked, std::vector<bool> &traversed, int n):
		for (int i = 0; i < n && not cycle_checker; ++i) {
		if (traversed[i] == 0) {
			dfs(graph, answer, used, i, cycle_checker, blocked);
		}
	}


int main() {

	int n, m, a, b;
	bool cycle_checker = false;
	std::cin >> n >> m;

	std::vector<std::vector<int>> graph(n);
	std::vector<bool> traversed(n);
	std::vector<bool> blocked(n, false);
	std::vector<int> answer;
	std::vector<int> used(n, 0);

	for (int i = 0; i < m; ++i) {
		std::cin >> a >> b;
		graph[a].push_back(b);
		traversed[b] = true;
	}

	sort_graph(graph, n);
	make_dfs(graph, answer, used, cycle_checker, blocked, traversed, n);
	inversion(answer);

	if ((!cycle_checker) && (!answer.empty())) {
		std::cout << "YES" << std::endl;
		for (size_t i = 0; i < answer.size(); ++i) {
			std::cout << answer[i] << " ";
		}
	}

	else {
		std::cout << "NO";
	}

}
