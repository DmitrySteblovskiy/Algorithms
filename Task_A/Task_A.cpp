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


long long deikstra(Graph &g, long long x, long long y)
{
	std::priority_queue<std::pair<long long, long long>> pq;
	std::vector<long long> way_weight(g.size(), std::numeric_limits<long long>::max());
	way_weight[x] = 0;
	pq.push(std::pair<long long, long long>(0, x));
	while (pq.size() != 0)
	{
		long long v = pq.top().second;
		long long way = -pq.top().first;
		pq.pop();
		if (way != way_weight[v])
			continue;
		std::vector<std::pair<long long, long long>> vec = g.getNext(v);
		for (int i = 0; i < vec.size(); ++i)
		{
			std::pair<long long, long long> to = vec[i];
			if (way_weight[to.first] > way_weight[v] + to.second)
			{
				way_weight[to.first] = way_weight[v] + to.second;
				pq.push(std::pair<long long, long long>(-way_weight[to.first], to.first));
			}
		}
	}
	return way_weight[y];
}


int main()
{
	long long a, b, m, x, y;

	std::cin >> a >> b >> m >> x >> y;

	Graph g(m);

	for (long long i = 0; i < m; ++i)
	{
		g.addEdge(i, (i + 1) % m, a);
		g.addEdge(i, (i*i + 1) % m, b);
	}
	std::cout << deikstra(g, x, y);

	system("pause");
}