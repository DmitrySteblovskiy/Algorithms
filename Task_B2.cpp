#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <vector>

// Арсений продолжает спать. Теперь ему снится кроличья ферма на планете Ка-Пэкс. Правда, ферма у него так себе — одни кроличьи норы в пустом поле. 
// Чтобы её хоть как-то облагородить, Арсений решил поставить забор так, чтобы все норки оказались внутри огражденной территории, а досок он потратил как можно меньше. 
// Помогите Арсению найти длину забора, чтобы он мог уже заказывать стройматериалы!
// https://contest.yandex.ru/contest/20642/run-report/45733202/
// Находим длину забора алгоритмом Эндрю.

class Point {
public:
    double x, y;
};

bool Comp(const Point &a, const Point &b) {     // для сортировки
    return ((a.x < b.x) || (a.x == b.x && a.y < b.y));
}

double Cross(const Point &origin, const Point &A, const Point &B) {     // определяет пересечение
    double result = (A.x - origin.x) * (B.y - origin.y) - (A.y - origin.y) * (B.x - origin.x);

    return result;
}

double Dist(Point x, Point y) {     // дистанция между точками
    double result = sqrt((y.x - x.x) * (y.x - x.x) + (y.y - x.y) * (y.y - x.y));

    return result;
}

std::vector<Point> SortForAndryu(std::vector<Point> &points_vec) {
    size_t k = 0;
    size_t vec_size = points_vec.size();
    std::vector<Point> hull(vec_size * 2);

    std::sort(points_vec.begin(), points_vec.end(), Comp);

    for (size_t i = 0; i < vec_size; ++i) {
        while ((k >= 2) && (Cross(hull[k - 2], hull[k - 1], points_vec[i]) <= 0)) {
            --k;
        }
        hull[k] = points_vec[i];
        ++k;
    }

    for (size_t i = vec_size - 1, j = k + 1; i > 0; --i) {
        while ((k >= j) && (Cross(hull[k - 2], hull[k - 1], points_vec[i - 1]) <= 0)) {
            --k;
        }

        hull[k] = points_vec[i - 1];
        ++k;
    }
    hull.resize(k - 1);

    return hull;
}

std::vector<Point> Andryu(std::vector<Point> &points_vec) {     // алгоритм Эндрю для построения выпуклой оболочки в двумерном пространстве
    size_t k = 0;
    size_t vec_size = points_vec.size();

    if (vec_size <= 3) {
        return points_vec;
    }
    std::vector<Point> hull(vec_size * 2);

    hull = SortForAndryu(points_vec);

    return hull;
}

double Solve(std::vector<Point> &point_vec) {
    double dst = 0;
    std::vector<Point> point_convex = Andryu(point_vec);
    size_t convex_size = point_convex.size();

    for (int i = 0; i < convex_size - 1; ++i) {
        dst += Dist(point_convex[i], point_convex[i + 1]);
    }

    dst += Dist(point_convex[0], point_convex[convex_size - 1]);

    return dst;
}


int main() {
    std::ios::sync_with_stdio(false);

    size_t quantity;
    double x, y, dst;
    std::vector<Point> point_vec;

    std::cin >> quantity;
    for (size_t i = 0; i < quantity; ++i) {
        std::cin >> x >> y;
        point_vec.push_back({x, y});
    }

    dst = Solve(point_vec);

    std::cout << std::fixed << std::setprecision(6) << dst;

	//system("pause");
}
