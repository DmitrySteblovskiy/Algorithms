#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>
 
// Арсений настолько потерялся в своих снах, что попал на передел земельных участков на планете Ка-Пэкс. Тут Арсений предложил делить по-научному. 
// Во-первых, устроить аукцион, на котором будут распределяться существующие участки, по два на каждого делегата. А после чего определить новые участки так.
// Пусть A и B — два участка. Для любых положительных u и v (цен, заявленных за каждый из участков на аукционе) рассмотрим фигуру u*A + v*B, 
// образованную точками uP + vQ, где P пробегает все точки из A, а Q — из B. Несложно видеть, что построенная так фигура будет снова выпуклой и ограниченной, 
// зависит от выбора начала системы координат, и более того, при переносе начала координат в другую точку этот участок также подвергается параллельному переносу на некоторый вектор,
// и потому его площадь S(u; v) корректно определена. Так как никто из жителей планеты Ка-Пэкс ничего толком не понял, то решение возражений не вызвало.
// Аукцион прошёл быстро. Только с подсчётом итоговой площади вышли проблемы. Арсений помнил, что функция S(u; v) всегда является однородным квадратным многочленом:
// S(u; v) = Xu2+2Yuv + Zv2 для всех u; v > 0, причем коэффициент X всегда равен площади S(A), а Z — площади B.
// А вот коэффициент Y — так называемую смешанную площадь S(A; B) выпуклых фигур A и B — Арсений подсчитать не мог.
// А вы сможете? Да.
// https://contest.yandex.ru/contest/20642/run-report/42341745/


const long double min_e = 0.00000000001;

class Vector2D {    // класс по сути взят из задачи А1
public:
    Vector2D(long double x, long double y) : x(x), y(y) {}
    long double x;
    long double y;
 
    Vector2D(const Vector2D &other)             = default;
    Vector2D &operator = (const Vector2D &other) = default;
    Vector2D(Vector2D && other)                  = default;
    Vector2D &operator = (Vector2D && other)      = default;
    ~Vector2D()                                 = default;
 
    Vector2D operator - () {
        return Vector2D(-x, -y);
    }
 
    friend Vector2D operator + (const Vector2D &v1, const Vector2D &v2) {
        return Vector2D(v1.x + v2.x, v1.y + v2.y);
    }
 
    friend Vector2D operator - (const Vector2D &v1, const Vector2D &v2) {
        return Vector2D(v1.x - v2.x, v1.y - v2.y);
    }
 
    friend Vector2D operator * (long double c, const Vector2D &v) {
        return Vector2D(c * v.x, c * v.y);
    }
 
    long double Norm() {
        return sqrt(Dot(*this, *this));
    }
 
    friend long double Dot(const Vector2D &v1, const Vector2D &v2) {
        return v1.x * v2.x + v1.y * v2.y;
    }
};
 
 
bool LessPolarAngle(Vector2D v1, Vector2D v2) {     // сортировка
    bool less_angle = (v1.x * v2.y - v1.y * v2.x > 0);  // я правильно понимаю, что по google codestyle лучше объединить это с ретурном независимо от размера строки?
    return less_angle;
}
 
 
std::vector<Vector2D> MinkowskiSum(std::vector<Vector2D> &polygon1, std::vector<Vector2D> &polygon2) {      // поиск суммы Минковского с использованием сортировки
    polygon1.push_back(polygon1[0]);
    polygon2.push_back(polygon2[0]);
    polygon1.push_back(polygon1[1]);
    polygon2.push_back(polygon2[1]);

    std::vector<Vector2D> result;
    size_t i = 0, j = 0;

    while ((i < polygon1.size() - 1) && (j < polygon2.size() - 1)) {
        result.push_back(polygon1[i] + polygon2[j]);
        if (LessPolarAngle(polygon1[i + 1] - polygon1[i], polygon2[j + 1] - polygon2[j])) {
            ++i;
        } else  {
            if (LessPolarAngle(polygon2[j + 1] - polygon2[j], polygon1[i + 1] - polygon1[i])) {
                ++j;
            } else {
                ++i;
                ++j;
            }
        }
    }

    return result;
}


std::vector<Vector2D> Normalize(const std::vector<Vector2D> &v) {
    std::vector<Vector2D> r;
    size_t min = 0;

    for (size_t i = 1; i < v.size(); ++i) {
        if (v[i].x < v[min].x || (std::abs(v[i].x - v[min].x) < min_e && v[i].y < v[min].y)) {
            min = i;
        }
    }
 
    for (size_t i = 0; i < v.size(); ++i) {
        r.push_back(v[(i + min) % v.size()]);
    }

    return r;
}


long double Gauss(std::vector<Vector2D> polygon) {      // с помощью формулы Гаусса считаем площади выпуклых многоугольников, используя лишь координаты их вершин
    long double answer = 0;
    size_t vec_size = polygon.size();

    for (size_t i = 0; i < vec_size - 1; ++i) {
        answer += (polygon[i].x * polygon[i + 1].y - polygon[i].y * polygon[i + 1].x);
    }
    answer += (polygon[vec_size - 1].x * polygon[0].y - polygon[vec_size - 1].y * polygon[0].x);
    answer *= 0.5;

    return std::abs(answer);
}
 
 
int main() {
    std::ios::sync_with_stdio(false);

    size_t n, m;
    long double answer = 0;
    std::vector<Vector2D> polygon1;
    std::vector<Vector2D> polygon2;

    std::cin >> n;
    for (size_t i = 0; i < n; ++i) {
        long double x, y;
        std::cin >> x >> y;
        polygon1.emplace_back(x, y);
    }

    std::cin >> m;
    for (size_t i = 0; i < m; ++i) {
        long double x, y;
        std::cin >> x >> y;
        polygon2.emplace_back(x, y);
    }

    polygon1 = Normalize(polygon1);
    polygon2 = Normalize(polygon2);

    std::vector<Vector2D> msum = MinkowskiSum(polygon1, polygon2);
    answer = 0.5 * (Gauss(msum) - Gauss(polygon1) - Gauss(polygon2));   // считаем смешанную (искомую) площадь

    std::cout << std::fixed << std::setprecision(6) << answer;
    // system("pause");
}