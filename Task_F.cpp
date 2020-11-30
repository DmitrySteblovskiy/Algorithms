#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>
#include <set>

// Даны точки, никакие 3 из которых не лежат на одной прямой. Никакие 4 точки не лежат на одной окружности.
// Кроме того, все точки имеют различные x-координаты. Определите среднее число сторон в многоугольниках диаграммы Вороного этого множества точек.
// Считаются только конечные многоугольники. Если все многоугольники неограниченны, ответ полагается равным 0.
// Число точек n ≤ 100000. Алгоритм должен иметь асимптотику O(n log n).
// https://contest.yandex.ru/contest/20642/problems/F/

long double const inf = std::numeric_limits<long double>::max();
const long double min_e = 0.0000000001;

class Point {
public:
    int id;
    long double x, y, z;

    Point *next, *prev;

    Point(long double x = 0, long double y = 0, long double z = 0, int id = -1) : x(x), y(y), z(z), id(id), next(nullptr), prev(nullptr) {
    }

    friend Point operator - (const Point &p1, const Point &p2) {
        return Point(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
    }

    friend Point operator + (const Point &p1, const Point &p2) {
        return Point(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
    }

    friend bool operator < (const Point &p1, const Point &p2) {
        return (p1.x < p2.x);
    }

    friend bool operator > (const Point &p1, const Point &p2) {
        return (p1.x > p2.x);
    }

    friend long double Vec(const Point &p1, const Point &p2) {
        return (p1.x * p2.y - p1.y * p2.x);
    }

    bool PerformEvent() {
        if (prev->next != this) {
            prev->next = this;
            next->prev = this;
            return true;
        } else {
            prev->next = next;
            next->prev = prev;
            return false;
        }
    }
};


class Face {
public:
    int points[3];

    Face(int first, int second, int third) : points({first, second, third}) {
    }

    friend bool operator < (const Face &f1, const Face &f2) {
        if (f1.points[0] < f2.points[0]) {
            return true;
        } else {
            if (f1.points[0] > f2.points[0]) {
                return false;
            } else {
                if (f1.points[1] < f2.points[1]) {
                    return true;
                } else {
                    if (f1.points[1] > f2.points[1]) {
                        return false;
                    } else {
                        return (f1.points[2] < f2.points[2]);
                    }
                }
            }
        }
    }

    friend bool operator > (const Face &f1, const Face &f2) {
        if (f1.points[0] > f2.points[0]) {
            return true;
        } else {
            if (f1.points[0] < f2.points[0]) {
                return false;
            } else {
                if (f1.points[1] > f2.points[1]) {
                    return true;
                } else {
                    if (f1.points[1] < f2.points[1]) {
                        return false;
                    } else {
                        return (f1.points[2] > f2.points[2]);
                    }
                }
            }
        }
    }
};


bool IsAcceptable(const Point &a, const Point &b, const Point &c) {
    bool accept = (Vec(b - a, c - b) > 0);

    return accept;
}


long double Sign(const Point *a, const Point *b, const Point *c) {
    long double sgn;
    if (a == nullptr || b == nullptr || c == nullptr) {
        return inf;
    }
    sgn = (b->x - a->x) * (c->y - b->y);
    sgn -= (b->y - a->y) * (c->x - b->x);

    return sgn;
}


long double Time(const Point *a, const Point *b, const Point *c) {
    long double time_;
    if (a == nullptr || b == nullptr || c == nullptr) {
        return inf;
    }
    time_ = ((b->x - a->x) * (c->z - b->z) - (b->z - a->z) * (c->x - b->x)) / Sign(a, b, c);

    return time_;
}


typedef Point Event;

std::vector<Event*> BuildDHull(std::vector<Point> &points, size_t left, size_t right) {
    if (right - left == 1) {
        return std::vector<Event*> ();
    }
    size_t mid = right + (left - right) / 2;
    size_t p1 = 0, p2 = 0;

    std::vector<Event*> evnts[2] = {
            BuildDHull(points, left, mid),
            BuildDHull(points, mid, right)
    };
    std::vector<Event*> result;

    Point *v = &points[mid];
    Point *u = &points[mid - 1];

    while (true) {
        if (Sign(u, v, v->next) < 0) {
            v = v->next;
        } else {
            if (Sign(u->prev, u, v) < 0) {
                u = u->prev;
            } else {
                break;
            }
        }
    }


    for (long double current_t = -inf;;) {
        Point *left = nullptr;
        Point *right = nullptr;
        std::vector<long double> next_t(6, inf);

        if (p1 < evnts[0].size()) {
            left = evnts[0][p1];
            next_t[0] = Time(left->prev, left, left->next);
        }
        if (p2 < evnts[1].size()) {
            right = evnts[1][p2];
            next_t[1] = Time(right->prev, right, right->next);
        }

        next_t[2] = Time(u, v, v->next);
        next_t[3] = Time(u, v->prev, v);
        next_t[4] = Time(u->prev, u, v);
        next_t[5] = Time(u, u->next, v);

        int min_i = -1;
        long double min_t = inf;

        for (int i = 0; i < 6; i++) {
            if ((next_t[i] > current_t) && (next_t[i] < min_t)) {
                min_t = next_t[i];
                min_i = i;
            }
        }

        if (min_i == -1 || min_t >= inf) {
            break;
        }


        if (min_i == 0) {
            if (left->x < u->x) {
                    result.emplace_back(left);
            }
            left->PerformEvent();
            ++p1;

        } else {
            if (min_i == 1) {
                if (right->x > v->x) {
                    result.emplace_back(right);
                }
                right->PerformEvent();
                ++p2;
            }

            if (min_i == 2) {
                result.emplace_back(v);
                v = v->next;
            }

            if (min_i == 3) {
                v = v->prev;
                result.emplace_back(v);
            }

            if (min_i == 4) {
                result.emplace_back(u);
                u = u->prev;
            }

            if (min_i == 5) {
                u = u->next;
                result.emplace_back(u);
            }
        }

        current_t = min_t;
    }

    u->next = v;
    v->prev = u;

    for (int i = static_cast<int>(result.size() - 1); i >= 0; i--) {
        Point* current = result[i];
        if (current->x > u->x && current->x < v->x) {
            u->next = v->prev = current;
            current->prev = u;
            current->next = v;
            if (current->x <= points[mid - 1].x) {
                u = current;
            } else {
                v = current;
            }

        } else {
            current->PerformEvent();
            if (current == u) {
                u = u->prev;
            }
            if (current == v) {
                v = v->next;
            }
        }
    }

    return result;
}


std::vector<Face> BuildConvex(std::vector<Point> points) {
    size_t n = points.size();
    std::vector<Face> result;

    std::sort(points.begin(), points.end());
    std::vector<Event*> events = BuildDHull(points, 0, n);

    for (Event* event : events) {
        Face current(event->prev->id, event->id, event->next->id);

        if (!event->PerformEvent()) {
            std::swap(current.points[0], current.points[1]);
        }
        result.emplace_back(current);
    }

    for (Point& p : points) {
        p.next = nullptr;
        p.prev = nullptr;
        p.z = -p.z;
    }
    events = BuildDHull(points, 0, n);

    for (Event* event : events) {
        Face current(event->prev->id, event->id, event->next->id);

        if (event->PerformEvent()) {
            std::swap(current.points[0], current.points[1]);
        }
        result.emplace_back(current);
    }

    return result;
}


typedef std::pair<int, int> Edge;

long double VoronoiCount(std::vector<Point> &sites) {
    size_t sts_size = sites.size();
    std::vector<Face> dimentional_hull = BuildConvex(sites);
    std::set<Edge> edges;
    std::vector<int> edges_number(sts_size);
    std::vector<bool> is_in_planar_hull(sts_size, false);

    for (Face& face : dimentional_hull) {
        for (size_t j = 0; j < 3; j++) {
            Edge edge(face.points[j], face.points[(j + 1) % 3]);
            if (edge.first > edge.second) {
                std::swap(edge.first, edge.second);
            }
            edges.insert(edge);
        }
    }

    for (const Edge& edge : edges) {
        edges_number[edge.first]++;
        edges_number[edge.second]++;
    }

    std::sort(sites.begin(), sites.end());
    std::vector<Point> planar_hull;

    for (const Point& site : sites) {
        while (static_cast<int>(planar_hull.size()) >= 2) {
            if (IsAcceptable(planar_hull[planar_hull.size() - 2], planar_hull.back(), site)) {
                break;
            }
            planar_hull.pop_back();
        }
        planar_hull.emplace_back(site);
    }

    for (int i = static_cast<int>(sts_size) - 2, bottom = static_cast<int>(planar_hull.size()); i >= 0; --i) {
        while (static_cast<int>(planar_hull.size()) > bottom) {
            if (IsAcceptable(planar_hull[planar_hull.size() - 2], planar_hull.back(), sites[i])) {
                break;
            }
            planar_hull.pop_back();
        }
        planar_hull.emplace_back(sites[i]);
    }

    for (Point& i : planar_hull) {
        is_in_planar_hull[i.id] = true;
    }

    size_t delaunay_inner_pts = 0, total_degree = 0;
    for (size_t i = 0; i < sts_size; ++i) {
        if (!is_in_planar_hull[i]) {
            total_degree += edges_number[i];
            ++delaunay_inner_pts;
        }
    }

    if (!delaunay_inner_pts) {
        return 0.0;
    } else {
        return (static_cast<long double>(total_degree) / static_cast<long double>(delaunay_inner_pts));
    }
}



int main() {
    std::ios::sync_with_stdio(false);

    long double x, y;
    std::vector<Point> points;

    for (size_t i = 0; std::cin >> y >> x; ++i) {
        Point pnt(x, y, x * x + y * y, i);
        points.emplace_back(pnt);
    }

    std::cout << std::fixed << std::setprecision(7) << VoronoiCount(points);

    //system("pause");
}
