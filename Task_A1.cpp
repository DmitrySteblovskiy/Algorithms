#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

// Арсений уснул. И приснилась ему планета Ка-Пэкс, на которой протекают две реки. Эти реки достаточно необычны для землян: 
// они текут строго от одной точки до другой точки напрямую в пространстве (так как в сне Арсения планета вовсе не круглая). 
// Арсений решил, что он хочет прорыть тоннель между реками. Так как он ленивый, то и копать много он не хочет. Помогите Арсению найти, сколько-таки ему придется прорыть.
// https://contest.yandex.ru/contest/20642/run-report/45731901/

const double MIN_E = 0.00000001;

class Vector3D {    // любой отрезок в пространстве предвставим векторами, с помощью которых можно найти минимальное расстояние между ними
public:
    double x, y, z;

    Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}

    Vector3D() = default;

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

    friend double Dot(const Vector3D &v1, const Vector3D &v2) {
        return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
    }

	friend std::istream& operator >> (std::istream &, Vector3D&);
};

std::istream& operator >> (std::istream &in, Vector3D &vec) {
    in >> vec.x >> vec.y >> vec.z;
    return in;
}



class Segment3D {   // отрезок
public:
    Vector3D from, to;

    Segment3D() = default;

    Segment3D(const Vector3D &from, const Vector3D &to) : from(from), to(to) {}

    friend std::istream& operator >> (std::istream &, Segment3D&);
};

std::istream& operator >> (std::istream &in, Segment3D &segm) {
    in >> segm.from >> segm.to;
    return in;
}

std::vector<double> AuxiliaryVarsForFindingDist(Segment3D S1, Segment3D S2) {      // поиск расст-я от отрезка к другому отрезку
    const double zero = 0.0;
    Vector3D v1 = S1.to - S1.from;
    Vector3D v2 = S2.to - S2.from;
    Vector3D v_delta = S1.from - S2.from;

    double dist_v1 = Dot(v1, v_delta);
    double dist_v2 = Dot(v2, v_delta);

    std::vector<double> auxiliary_dots(3);
    auxiliary_dots[0] = Dot(v1, v1);
    auxiliary_dots[1] = Dot(v1, v2);
    auxiliary_dots[2] = Dot(v2, v2);

    double aux_diff = auxiliary_dots[0] * auxiliary_dots[2] - auxiliary_dots[1] * auxiliary_dots[1];

    std::vector<double> auxiliary_vars(6);
    auxiliary_vars[4] = aux_diff;
    auxiliary_vars[5] = aux_diff;

    if (aux_diff <= MIN_E) {
        auxiliary_vars[1] = zero;
        auxiliary_vars[4] = static_cast<double>(static_cast<int>(zero) + 1);
        auxiliary_vars[3] = dist_v2;
        auxiliary_vars[5] = auxiliary_dots[2];
    } else {
        auxiliary_vars[1] = (auxiliary_dots[1] * dist_v2 - auxiliary_dots[2] * dist_v1);
        auxiliary_vars[3] = (auxiliary_dots[0] * dist_v2 - auxiliary_dots[1] * dist_v1);
        if (auxiliary_vars[1] <= zero) {
            auxiliary_vars[1] = zero;
            auxiliary_vars[3] = dist_v2;
            auxiliary_vars[5] = auxiliary_dots[2];
        } else {
            if (auxiliary_vars[1] >= auxiliary_vars[4]) {
                auxiliary_vars[1] = auxiliary_vars[4];
                auxiliary_vars[3] = dist_v2 + auxiliary_dots[1];
                auxiliary_vars[5] = auxiliary_dots[2];
            }
        }
    }

    if (auxiliary_vars[3] <= zero) {
        auxiliary_vars[3] = zero;
        if (-dist_v1 <= zero) {
            auxiliary_vars[1] = zero;
        } else {
            if (-dist_v1 >= auxiliary_dots[0]) {
                auxiliary_vars[1] = auxiliary_vars[4];
            } else {
                auxiliary_vars[1] = -dist_v1;
                auxiliary_vars[4] = auxiliary_dots[0];
            }
        }
    } else {
        if (auxiliary_vars[3] >= auxiliary_vars[5]) {
            auxiliary_vars[3] = auxiliary_vars[5];
            if ((-dist_v1 + auxiliary_dots[1]) <= zero) {
                auxiliary_vars[1] = 0;
            } else {
                if ((-dist_v1 + auxiliary_dots[1]) >= auxiliary_dots[0]) {
                    auxiliary_vars[1] = auxiliary_vars[4];
                } else {
                    auxiliary_vars[1] = (-dist_v1 +  auxiliary_dots[1]);
                    auxiliary_vars[4] = auxiliary_dots[0];
                }
            }
        }
    }
    return auxiliary_vars;
}

double FindingRequiredDist(Segment3D S1, Segment3D S2) {
    std::vector<double> auxiliary_vars = AuxiliaryVarsForFindingDist(S1, S2);

    const double zero = 0.0;
    Vector3D v1 = S1.to - S1.from;
    Vector3D v2 = S2.to - S2.from;
    Vector3D v_delta = S1.from - S2.from;

    auxiliary_vars[0] = (auxiliary_vars[1] <= MIN_E ? zero : auxiliary_vars[1] / auxiliary_vars[4]);
    auxiliary_vars[2] = (auxiliary_vars[3] <= MIN_E ? zero : auxiliary_vars[3] / auxiliary_vars[5]);

    Vector3D final_vector = v_delta + (auxiliary_vars[0] * v1) - (auxiliary_vars[2] * v2);

    return final_vector.Norm();
}


int main() {
    std::ios::sync_with_stdio(false);

    Segment3D Segm1, Segm2;
    std::cin >> Segm1 >> Segm2;

    std::cout << std::fixed << std::setprecision(6) << FindingRequiredDist(Segm1, Segm2);

    //system("pause");
} 
