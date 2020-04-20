#include <iostream>
#include <queue>
#include <vector>
#include <limits>


class Graph
{
public:
	explicit Graph(long long n)
	{
		g.assign(n, std::vector<std::pair<long long, long long>>());
	}
	void addEdge(long long a, long long b, long long w)
	{
		g[a].push_back(std::pair<long long, long long>(b, w));
	}
	long long size()
	{
		return g.size();
	}
	std::vector<std::pair<long long, long long>>& getNext(long long a)
	{
		return g[a];
	}
private:
	std::vector<std::vector<std::pair<long long, long long>>> g;
};


long long deikstra(Graph &g, long long x, long long y, long long limit)
{
	std::priority_queue<std::pair<long long, std::pair<long long, long long>>> pq;
	std::vector<long long> way_weight(g.size(), std::numeric_limits<long long>::max());
	way_weight[x] = 0;
	pq.push(std::pair<long long, std::pair<long long, long long>>(0, std::pair<long long, long long>(x, 0)));
	while (pq.size() != 0)
	{
		long long v = pq.top().second.first;
		long long way = -pq.top().first;
		long long k = pq.top().second.second;
		pq.pop();
		if (way != way_weight[v])
			continue;
		if (k >= limit)
			continue;
		std::vector<std::pair<long long, long long>> vec = g.getNext(v);
		for (long long i = 0; i < vec.size(); ++i)
		{
			std::pair<long long, long long> to = vec[i];
			if (way_weight[to.first] > way_weight[v] + to.second)
			{
				way_weight[to.first] = way_weight[v] + to.second;
				pq.push(std::pair<long long, std::pair<long long, long long>>(-way_weight[to.first], std::pair<long long, long long>(to.first, k + 1)));
			}
		}
	}
	return way_weight[y] == std::numeric_limits<long long>::max() ? -1 : way_weight[y];
}


int main()
{
	long long n, m, k, s, f;

	std::cin >> n >> m >> k >> s >> f;
	s--;
	f--;
	Graph g(n);

	for (long long i = 0; i < m; ++i)
	{
		long long si, fi, pi;
		std::cin >> si >> fi >> pi;
		g.addEdge(si - 1, fi - 1, pi);
	}
	std::cout << deikstra(g, s, f, k);

	system("pause");
}