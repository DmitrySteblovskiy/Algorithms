#include <cmath>
#include <iomanip>
#include <iostream>

// Арсений уснул. И приснилась ему планета Ка-Пэкс, на которой протекают две реки. Эти реки достаточно необычны для землян: 
// они текут строго от одной точки до другой точки напрямую в пространстве (так как в сне Арсения планета вовсе не круглая). 
// Арсений решил, что он хочет прорыть тоннель между реками. Так как он ленивый, то и копать много он не хочет. Помогите Арсению найти, сколько-таки ему придется прорыть.



const double min_e = 0.00000001;

class Vector3D {    // любой отрезок в пространстве предвставим векторами, с помощью которых можно найти минимальное расстояние между ними
public:
    double x, y, z;

    Vector3D (double x, double y, double z) : x(x), y(y), z(z) {
    }


    Vector3D operator - () {
        return Vector3D(-x, -y, -z);
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

    double Norm() {
        return sqrt(Dot(*this, *this));
    }

    friend double Dot (const Vector3D &v1, const Vector3D &v2) {
        return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
    }
};


class Segment3D {   // отрезок
public:
    Vector3D from, to;

    Segment3D (const Vector3D &from, const Vector3D &to) : from(from), to(to) {
    }
};



double SegmentToSegment (Segment3D S1, Segment3D S2) {      // поиск расст-я от отрезка к другому отрезку
    const double zero = 0.0;
    Vector3D v1 = S1.to - S1.from;
    Vector3D v2 = S2.to - S2.from;
    Vector3D v_delta = S1.from - S2.from;
    double V1 = Dot(v1, v1);
    double V1_2 = Dot(v1, v2);
    double V2 = Dot(v2, v2);
    double V1d = Dot(v1, v_delta);
    double V2d = Dot(v2, v_delta);
    double diff = V1 * V2 - V1_2 * V1_2;
    double s, s_n, s_diff = diff;
    double t, t_n, t_diff = diff;

    if (diff <= min_e) {
        s_n = zero;
        s_diff = static_cast<double>(static_cast<int>(zero) + 1);
        t_n = V2d;
        t_diff = V2;
    } else {
        s_n = (V1_2 * V2d - V2 * V1d);
        t_n = (V1 * V2d - V1_2 * V1d);
        if (s_n <= zero) {
            s_n = zero;
            t_n = V2d;
            t_diff = V2;
        } else {
            if (s_n >= s_diff) {
                s_n = s_diff;
                t_n = V2d + V1_2;
                t_diff = V2;
            }
        }
    }

    if (t_n <= zero) {
        t_n = zero;
        if (-V1d <= zero) {
            s_n = zero;
        } else {
            if (-V1d >= V1) {
                s_n = s_diff;
            } else {
                s_n = -V1d;
                s_diff = V1;
            }
        }
    } else {
        if (t_n >= t_diff) {
            t_n = t_diff;
            if ((-V1d + V1_2) <= zero) {
                s_n = 0;
            } else {
                if ((-V1d + V1_2) >= V1) {
                    s_n = s_diff;
                } else {
                    s_n = (-V1d +  V1_2);
                    s_diff = V1;
                }
            }
        }
    }
    s = (s_n <= min_e ? zero : s_n / s_diff);
    t = (t_n <= min_e ? zero : t_n / t_diff);

    Vector3D final_vector = v_delta + (s * v1) - (t * v2);

    return final_vector.Norm();
}


int main() {
    std::ios::sync_with_stdio(false);

    double x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4;
    std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
    std::cin >> x3 >> y3 >> z3 >> x4 >> y4 >> z4;

    printf("%.6f", SegmentToSegment (Segment3D(Vector3D(x1, y1, z1), Vector3D(x2, y2, z2)),
                                        Segment3D(Vector3D(x3, y3, z3), Vector3D(x4, y4, z4))));

    //system("pause");
}