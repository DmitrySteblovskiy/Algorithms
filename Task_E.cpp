#include <algorithm>
#include <iostream>
#include <vector>
#include <set>

// Арсений открыл эзотерический салон в свои снах на планете Ка-Пэкс. У Арсения всё будет хорошо. А вот у его клиентов — не факт.
// Если хотя бы какие-нибудь их палочки в гадании "Мокусо Дзэй" пересеклись, то день точно сложится удачно. А если нет — то стоит ждать беды.
// Чтобы точнее сказать, что конкретно пойдет хорошо в этот день, нужно знать, какие именно палочки пересекаются. Помогите Арсению узнать, как пройдет день у его клиентов.
// https://contest.yandex.ru/contest/20642/run-report/42969108/

const double inaccuracy = 1E-9;


class Point {	 // точка
public:
	double x, y;
};

class Segm {		// класс отрезка
public:
	Point point1, point2;
	size_t index;
 
	double GetY (double x) const {
		if (std::abs(point1.x - point2.x) < inaccuracy) {
			return point1.y;
		}

		double reqY = (point2.y - point1.y) * (x - point1.x) / (point2.x - point1.x);
		reqY += point1.y;

		return reqY;
	}
};


inline bool IntersectLines (double l1, double r1, double l2, double r2) {	// пересечения
	if (l1 > r1) {
		std::swap (l1, r1);
	}
	if (l2 > r2) {
		std::swap (l2, r2);
	}
	double maxL = std::max(l1, l2);
	double minR = std::min(r1, r2);

	return (maxL <= (minR + inaccuracy));
}
 
inline int Vec (const Point &a, const Point &b, const Point &c) {
	double res = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);

	return (std::abs(res) < inaccuracy ? 0 : (res > 0) ? 1 : -1);
}
 
bool Intersect (const Segm &a, const Segm &b) {	// пересечения
	bool line_intersection = IntersectLines (a.point1.x, a.point2.x, b.point1.x, b.point2.x) && IntersectLines (a.point1.y, a.point2.y, b.point1.y, b.point2.y);
	bool vec_intersection = Vec (a.point1, a.point2, b.point1) * Vec (a.point1, a.point2, b.point2) <= 0 && Vec (b.point1, b.point2, a.point1) * Vec (b.point1, b.point2, a.point2) <= 0;

	return (line_intersection && vec_intersection);
}
 
 
bool operator < (const Segm &a, const Segm &b) {
	double x = std::max (std::min (a.point1.x, a.point2.x), std::min (b.point1.x, b.point2.x));

	return a.GetY(x) < (b.GetY(x) - inaccuracy);
}

std::set<Segm> Set;	 // сет отрезков
std::vector <std::set<Segm>::iterator> seg_locations; 	// здесь хранятся итераторы, указывающие положение каждого отрезка в сете

class Event {
public:

	int tp, index;
	double x;

	Event() {}
	Event (double x, int tp, size_t index) : x(x), tp(tp), index(index) {
	}

	bool operator < (const Event &evnt) const {
		if (std::abs (x - evnt.x) > inaccuracy) {
			return (x < evnt.x);
		}

		return (tp > evnt.tp);
	}
};


inline std::set<Segm>::iterator prev (std::set<Segm>::iterator it) {
	return it == Set.begin() ? Set.end() : --it;
}

inline std::set<Segm>::iterator next (std::set<Segm>::iterator it) {
	return ++it;
}

std::pair<int,int> Solve (const std::vector<Segm> &a) {	 // выводит искомые индексы для удачного дня или std::pair(-1, -1) в ином случае
	int n = static_cast<int>(a.size());
	std::vector<Event> evnt;

	for (size_t i = 0; i < n; ++i) {
		evnt.push_back (Event (std::min (a[i].point1.x, a[i].point2.x), 1, i));
		evnt.push_back (Event (std::max (a[i].point1.x, a[i].point2.x), -1, i));
	}

	sort (evnt.begin(), evnt.end());
 
	Set.clear();
	seg_locations.resize (a.size());
	size_t evnt_size = evnt.size();

	for (size_t i = 0; i < evnt_size; ++i) {
		size_t index = evnt[i].index;

		if (evnt[i].tp != 1) {
			std::set<Segm>::iterator
				nxt = next (seg_locations[index]),
				prv = prev (seg_locations[index]);

			if (nxt != Set.end() && prv != Set.end() && Intersect (*nxt, *prv)) {
				std::pair ind_pointers = std::make_pair (prv->index, nxt->index);

				return ind_pointers;
			}

			Set.erase (seg_locations[index]);
		}
		else {
			std::set<Segm>::iterator
				nxt = Set.lower_bound (a[index]),
				prv = prev (nxt);

			if (nxt != Set.end() && Intersect (*nxt, a[index])) {
				return std::make_pair (nxt->index, index);
			}
			if (prv != Set.end() && Intersect (*prv, a[index])) {
				return std::make_pair (prv->index, index);
			}

			seg_locations[index] = Set.insert (nxt, a[index]);
		}
	}
 
	return std::make_pair (-1, -1);
}

int main() {
    std::ios::sync_with_stdio(false);

	std::vector<Segm> segm;
    size_t N;
    std::cin >> N;

    for (size_t i = 0; i < N; ++i) {
		Segm current;
        std::cin >> current.point1.x >> current.point1.y >> current.point2.x >> current.point2.y;
		current.index = i;

        segm.emplace_back(current);
    }

	std::pair <int, int> answ = Solve(segm);
	std::pair check = std::make_pair(-1, -1);

	if (answ != check) {	// проверяем, каким выдался день
		++answ.first;
		++answ.second;
		std::cout << "YES\n" << answ.first << " " << answ.second;
	} else {
		std::cout << "NO";
	}

    //system("pause");
}