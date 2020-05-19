// Требуется найти в связном графе остовное дерево минимального веса. Воспользуйтесь алгоритмом Прима.
// https://contest.yandex.ru/contest/17986/run-report/33076537/
//
// Используя priority_queue алгоритм Прима постепенно находит MST графа из веса top элемента очереди: answer += top.weight

#include <iostream>
#include <limits>
#include <queue>
#include <set>
#include <vector>


int Sum(std::vector<int> min_) {
	int sum = 0;
	for (size_t i = 0; i < min_.size(); ++i) {
		sum += min_[i];
	}
	return sum;
}

class edge {
public:
	int to, weight;

	edge(int to_, int weight_)
	{
		to = to_;
		weight = weight_;
	}
};


class compare {
public:
	bool operator() (const edge& a, const edge& b) const {
		return a.weight > b.weight;
	}
};


int Prim(int n, int m)
{
	std::vector<bool> used;
	std::vector<std::vector<edge>> elements;
	std::priority_queue<edge, std::vector<edge>, compare> nodes;
	used.resize(n);
	elements.resize(n);
	int from, to, cost, answer, curr, nearest;
	answer = 0; curr = 1;

	for (int i = 0; i < m; ++i)
	{
		std::cin >> from >> to >> cost;
		if (to == from) {
			continue;
		}
		elements[from - 1].emplace_back(edge(to - 1, cost));
		elements[to - 1].emplace_back(edge(from - 1, cost));
	}
	used[0] = true;

	for (edge near : elements[0])
	{
		nodes.push(near);
	}


	while ((0 < nodes.size()) && (curr < n)) {
		edge top = nodes.top();
		nodes.pop();
		if (used[top.to]) {
			continue;
		}
		used[top.to] = true;
		answer += top.weight;
		++curr;


		for (edge near_ : elements[top.to])
		{
			if (!used[near_.to])
			{
				nodes.push(near_);
			}
		}
	}

	return answer;
}


int main()
{
	int n, m;
	std::cin >> n >> m;
	std::cout << Prim(n, m);

	system("pause");
}