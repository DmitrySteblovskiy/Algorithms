#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <functional>
#include <vector>

// Арсений решил поиграть в настольную игру со своим другом Ильей. Так как играть обычными кубиками им уже стало неинтересно,
// Илья попросил по его чертежам сконструировать новую игральную кость. Так как он ленивый, то просто накидал точек в пространстве и сказал,
// что все они должны лежать в кубике его мечты. У Арсения есть 3D-принтер, вот только материалы для печати достаточно дорогие,
// поэтому он хочет выполнить требования своего друга, потратив как можно меньше пластика.
//

long double const inf = std::numeric_limits<long double>::max();

class Vector3D {
public:
    long long x, y, z;

    Vector3D (long long x, long long y, long long z) : x(x), y(y), z(z) {}

    Vector3D operator - () {
        return Vector3D(-x, -y, -z);
    }

    friend long double length (const Vector3D &v1) {
        return sqrt(v1.x * v1.x + v1.y * v1.y + v1.z * v1.z);
    }

    friend Vector3D operator - (const Vector3D &v1, const Vector3D &v2) {
        return Vector3D(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
    }

    friend Vector3D operator + (const Vector3D &v1, const Vector3D &v2) {
        return Vector3D(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
    }

    friend Vector3D operator * (double c, const Vector3D &vec2) {
        return Vector3D(c * vec2.x, c * vec2.y, c * vec2.z);
    }

    friend Vector3D operator / (double c, const Vector3D &vec2) {
        return Vector3D(c * vec2.x, c * vec2.y, c * vec2.z);
    }

    long double Norm() {
        return sqrt(Dot(*this, *this));
    }

    friend long double Dot(const Vector3D &v1, const Vector3D &v2) {
        return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
    }
};


class Point {
public:
    long long id;
    long double x, y, z;
 
    Point *next, *prev;
 
    Point(long double x = 0, long double y = 0, long double z = 0, long long id = -1) : x(x), y(y), z(z), id(id), next(nullptr), prev(nullptr) {}
 
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

	friend std::istream& operator >> (std::istream &, Point&);
};

std::istream& operator >> (std::istream &in, Point &point) {
    in >> point.x >> point.y >> point.z;
    return in;
}

 
class Face {
private:
    Point p0_, p1_, p2_;

public:
    int points[3];

    Face(int first, int second, int third) : points({first, second, third}) {}

    void FixOrder() {
        if (points[1] < points[0] && points[1] < points[2]) {
            std::swap(points[0], points[1]);
            std::swap(points[1], points[2]);
        } else {
            if (points[2] < points[0] && points[2] < points[1]) {
                std::swap(points[1], points[2]);
                std::swap(points[0], points[1]);
            }
        }
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


long double PointsSign(const Point *a, const Point *b, const Point *c) {
    long double sgn;
    if (a == nullptr || b == nullptr || c == nullptr) {
        return inf;
    }
    sgn = (b->x - a->x) * (c->y - b->y);
    sgn -= (b->y - a->y) * (c->x - b->x);
 
    return sgn;
}

long double CurTime(const Point *a, const Point *b, const Point *c) {
    long double time_;
    if (a == nullptr || b == nullptr || c == nullptr) {
        return inf;
    }
    time_ = ((b->x - a->x) * (c->z - b->z) - (b->z - a->z) * (c->x - b->x)) / PointsSign(a, b, c);
 
    return time_;
}

std::vector<Point*> BuildDHull(std::vector<Point> &points, size_t left, size_t right) {
    if (right - left == 1) {
        return std::vector<Point*> ();
    }
    size_t mid = (left + right) / 2;
    size_t p1 = 0, p2 = 0;

    std::vector<Point*> evnts[2] = {
        BuildDHull(points, left, mid),
        BuildDHull(points, mid, right)
    };
    std::vector<Point*> result;

    Point *v = &points[mid];
    Point *u = &points[mid - 1];

    while (true) {
        if (PointsSign(u, v, v->next) < 0) {
            v = v->next;
        } else {
            if (PointsSign(u->prev, u, v) < 0) {
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
            next_t[0] = CurTime(left->prev, left, left->next);
        }
        if (p2 < evnts[1].size()) {
            right = evnts[1][p2];
            next_t[1] = CurTime(right->prev, right, right->next);
        }
 
        next_t[2] = CurTime(u, v, v->next);
        next_t[3] = CurTime(u, v->prev, v);
        next_t[4] = CurTime(u->prev, u, v);
        next_t[5] = CurTime(u, u->next, v);
 
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
 
 
        if (!min_i) {
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

std::vector<Face> ConvFaceBuilder(std::vector<Face>& result, std::vector<Point*>& events, bool beg_) {
    for (Point* event : events) {
        Face current(event->prev->id, event->id, event->next->id);

        if (beg_) {
            if (!event->PerformEvent()) {
                std::swap(current.points[0], current.points[1]);
            }
            result.emplace_back(current);
        } else {
            if (event->PerformEvent()) {
                std::swap(current.points[0], current.points[1]);
            }
            result.emplace_back(current);
        }
    }
    return result;
}

std::vector<Face> BuildConvex(std::vector<Point>& points) {
    size_t n = points.size();
    std::vector<Face> result;

    std::sort(points.begin(), points.end());
    std::vector<Point*> events = BuildDHull(points, 0, n);

    result = ConvFaceBuilder(result, events, true);

    for (Point& p : points) {
        p.next = nullptr;
        p.prev = nullptr;
        p.z = -p.z;
    }
    events = BuildDHull(points, 0, n);

    result = ConvFaceBuilder(result, events, false);   

    return result;
}

void Rotate(Point &p, long double angle) {
    long double new_z = -p.x * sin(angle) + p.z * cos(angle);
    long double new_x = p.x * cos(angle) + p.z * sin(angle);
 
    p.z = new_z;
    p.x = new_x;
 
    new_z = p.z * cos(angle) + p.y * sin(angle);
    long double new_y = -p.z * sin(angle) + p.y * cos(angle);
    p.y = new_y;
    p.z = new_z;
 
    new_x = p.x * cos(angle) + p.y * sin(angle);
    new_y = -p.x * sin(angle) + p.y * cos(angle);
    p.x = new_x;
    p.y = new_y;
}

void InputTests(std::istream& in, long long& m, std::vector<Point>& points) {
    in >> m;
    for (long long j = 0; j < m; j++) {
        Point p;
        in >> p;
        p.id = j;
        Rotate(p, 0.01);
        points.push_back(p);
    }
}

void OutputHull(std::ostream& out, std::vector<Face>& hull) {
    out << hull.size() << "\n";
    for (Face& f : hull) {
        out << 3 << " " << f.points[0] << " " << f.points[1] << " " << f.points[2] << "\n";
    }
}


int main() {
    int n;
    long long q;

    std::cin >> n;
    for (size_t i = 0; i < n; i++) {
        long long m;
        std::vector<Point> points;

        InputTests(std::cin, m, points);

        std::vector<Face> hull = BuildConvex(points);
        for (Face& f : hull) {
            f.FixOrder();
        }

        std::sort(hull.begin(), hull.end());

        OutputHull(std::cout, hull);
    }

    //system("pause");
}