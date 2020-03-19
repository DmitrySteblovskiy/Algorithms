#include <iostream>
#include <vector>
#include <math.h>
#include <queue>
#include <algorithm>

void bfs(const std::vector<std::vector<int>> &graph, int node, int n, std::vector <int> &dist) {
	dist.assign(n, -1);
	std::vector <bool> used(n, false);
	std::queue <int> q;
	q.push(node);
	dist[node] = 0;
	used[node] = true;
	while (!q.empty()) {
		int tmp = q.front();
		for (int v : graph[tmp]) {
			if (!used[v]) {
				q.push(v);
				dist[v] = dist[tmp] + 1;
				used[v] = true;
			}
		}
		q.pop();
	}
};

int main() {
	int n, m, leo, mat, milk, begin, end;

	std::cin >> n >> m >> leo >> mat >> milk;
	std::vector <std::vector<int>> graph(n, std::vector <int>());
	leo--;
	mat--;
	milk--;



	for (int i = 0; i < m; ++i) {
		std::cin >> begin >> end;
		begin--;
		end--;
		graph[begin].push_back(end);
		graph[end].push_back(begin);
	}


	std::vector <int> resleo(n);
	std::vector <int> resmat(n);
	std::vector <int> resmilk(n);

	bfs(graph, leo, n, resleo);
	bfs(graph, mat, n, resmat);
	bfs(graph, milk, n, resmilk);

	std::vector <int> sum(n, 0);
	for (int i = 0; i < n; ++i) {
		sum[i] = resleo[i] + resmat[i] + resmilk[i];
	}
	int min = sum[0];
	for (int i = 0; i < sum.size(); ++i)
		if (sum[i] < min)
			min = sum[i];
	std::cout << min;

	system("pause");
}
