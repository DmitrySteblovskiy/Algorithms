//Условие задачи:
//Фрэнку опять прилетел новый заказ. Однако в этот раз город играет по очень странным правилам: 
//все дороги в городе односторонние и связывают только офисы нанимателей перевозчика. Множество офисов, 
//между любыми двумя из которых существует путь, образуют квартал, если нельзя добавить никакие другие, чтобы условие выполнялось. 
//Фрэнку интересно, каково минимальное количество односторонних дорог нужно ещё построить, чтобы весь город стал квраталом.
//https://contest.yandex.ru/contest/17228/run-report/31544153/
#include <iostream>
#include <vector>
void reverse(std::vector<int> &vec);
void dfs2(std::vector<std::vector<int>> &g, int u, std::vector<int> &path, std::vector<bool> &used)
{
    used[u] = true;
    path.push_back(u);
    for (int i = 0; i < g[u].size(); i++)
    {
        if(!used[g[u][i]])
        {
            dfs2(g, g[u][i], path, used);
        }
    }

}
void dfs1(std::vector<std::vector<int>> &g, int u, std::vector<int> &path, std::vector<bool> &used)
{
    used[u] = true;
    for (int i = 0; i < g[u].size(); i++)
    {
        if(!used[g[u][i]])
        {
            dfs1(g, g[u][i], path, used);
        }
    }
    path.push_back(u);
}
void times(std::vector<std::vector<int>> &g, std::vector<int> &times)
{
    std::vector<bool> used(g.size(), false);
    for(int i = 0; i < g.size(); i++)
    {
        if(!used[i])
        {
            dfs1(g,i,times,used);
        }
    }
    reverse(times);
}
void reverse(std::vector<int> &vec)
{
    for (int i = 0; i < vec.size() / 2; i++)
    {
        int t = vec[i];
        vec[i]=vec[vec.size() - 1 - i];
        vec[vec.size() - 1 - i] = t;
    }
}
std::pair<int,int> check(std::vector<int>&com, int xi, int yj)
{
    return std::pair<int,int> (com[xi],com[yj]);
}
void Curacao(std::vector<std::vector<int>> &reversed, std::vector<std::vector<int>> &graph)
{
    std::vector<int> timesout;
    times(graph, timesout);
    std::vector<std::vector <int>> result;
    std::vector<bool> used(reversed.size(), false);
    for (int i =0; i < timesout.size(); i++)
    {
        std::vector<int>c;
        if (!used[timesout[i]])
            dfs2(reversed, timesout[i],c, used);
        if(c.size()>0)
            result.push_back(c);
    }
    if (result.size()==1)
    {
        std::cout << 0;
        return;
    }

    std::vector<int>harka(graph.size(), 0);

    for (int i = 0; i < result.size();++i)
    {
        for (int j = 0; j < result[i].size(); ++j)
        {
            harka[result[i][j]] = i;
        }
    }
    std::vector<std::vector<int>> condencionered(result.size(), std::vector<int>());

    for (int i=0; i < reversed.size(); i++)
    {
        for (int j =0; j < graph[i].size(); j++)
        {
            std::pair<int, int> tm= check(harka, i, graph[i][j]);
        
            if (tm.first!=tm.second)
                condencionered[tm.first].push_back(tm.second);
        }
    }
    std::vector<std::vector<int>> degs(result.size(), std::vector<int>(2, 0));


    for (int i=0; i < condencionered.size(); i++)
    {
        for (int j =0; j < condencionered[i].size(); j++)
        {
           degs[condencionered[i][j]][1]+=1;
        }
        degs[i][0]+=condencionered[i].size();
    }
    int zero1 = 0, zero2 = 0;
    for (int i=0; i <degs.size(); i++)
    {
        if(degs[i][0] == 0)
            zero1++;
        if(degs[i][1] == 0)
            zero2++;
    }
    if (zero1>zero2)
        std::cout<<zero1;
    else std::cout<<zero2;
    return;
}
int main()
{
    int n,m;
    std::cin>>n>>m;
    std::vector<std::vector<int>> graph(n, std::vector<int>());
    std::vector<std::vector<int>> reversed(n, std::vector<int>());
    int a, b;
    for(int i = 0; i < m; i++)
    {
        std::cin>>a>>b;
        graph[a - 1].push_back(b - 1);
        reversed[b - 1].push_back(a - 1);
    }

    Curacao(reversed,graph);
    return 0;
}
