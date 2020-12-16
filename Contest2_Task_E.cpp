#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <set>

// Арсений открыл эзотерический салон в свои снах на планете Ка-Пэкс. У Арсения всё будет хорошо. А вот у его клиентов — не факт.
// Если хотя бы какие-нибудь их палочки в гадании "Мокусо Дзэй" пересеклись, то день точно сложится удачно. А если нет — то стоит ждать беды.
// Чтобы точнее сказать, что конкретно пойдет хорошо в этот день, нужно знать, какие именно палочки пересекаются. Помогите Арсению узнать, как пройдет день у его клиентов.
// https://contest.yandex.ru/contest/20642/run-report/42969108/

const double inaccuracy = 0.000000001;
bool good_day_result = false;	// для ответа на первый вопрос задачи

class Point {
public:
	double x, y;

	int Vec(const Point &a, const Point &b, const Point &c) {
		double res = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);

		return (std::abs(res) < inaccuracy ? 0 : (res > 0) ? 1 : -1);
	}
	
	friend std::istream& operator >> (std::istream &, Point&);
};

std::istream& operator >> (std::istream &in, Point &point) {
    in >> point.x;
    in >> point.y;

    return in;
}


class Segm {		// класс отрезка
public:
	Point point1, point2;
	size_t index;

	double GetY(double x) const {
		if (std::abs(point1.x - point2.x) < inaccuracy) {
			return point1.y;
		}

		double required_y = (point2.y - point1.y) * (x - point1.x) / (point2.x - point1.x);
		required_y += point1.y;

		return required_y;
	}

	bool IntersectLines(double l1, double r1, double l2, double r2) {	// проверка пересечения
		if (l1 > r1) {
			std::swap (l1, r1);
		}
		if (l2 > r2) {
			std::swap (l2, r2);
		}
		double max_l = std::max(l1, l2);
		double min_r = std::min(r1, r2);

		return (max_l <= (min_r + inaccuracy));
	}

	bool Intersect(const Segm &a, const Segm &b) {	// пересечения
		bool line_intersection = IntersectLines(a.point1.x, a.point2.x, b.point1.x, b.point2.x) && IntersectLines(a.point1.y, a.point2.y, b.point1.y, b.point2.y);
		bool vec_intersection = point1.Vec(a.point1, a.point2, b.point1) * point1.Vec(a.point1, a.point2, b.point2) <= 0
		    && point1.Vec(b.point1, b.point2, a.point1) * point1.Vec(b.point1, b.point2, a.point2) <= 0;

		return (line_intersection && vec_intersection);
	}
	
	friend std::istream& operator>>(std::istream &, Segm&);
};

std::istream& operator >> (std::istream &in, Segm &segm) {
    in >> segm.point1;
    in >> segm.point2;

    return in;
}



bool operator < (const Segm &a, const Segm &b) {
	double x = std::max (std::min (a.point1.x, a.point2.x), std::min (b.point1.x, b.point2.x));

	return a.GetY(x) < (b.GetY(x) - inaccuracy);
}

std::set<Segm> set_;	 // сет отрезков
std::vector <std::set<Segm>::iterator> seg_locations; 	// здесь хранятся итераторы, указывающие положение каждого отрезка в сете

class Event {
public:

	int tp, index;
	double x;

	Event() {}
	Event (double x, int tp, size_t index) : x(x), tp(tp), index(index) {}

	bool operator < (const Event &evnt) const {
		if (std::abs (x - evnt.x) > inaccuracy) {
			return (x < evnt.x);
		}

		return (tp > evnt.tp);
	}
};

std::vector<Event> FillSortEvents(std::vector<Segm> &a) {
	int n = static_cast<int>(a.size());
	std::vector<Event> evnt;

	for (size_t i = 0; i < n; ++i) {
		evnt.push_back(Event (std::min (a[i].point1.x, a[i].point2.x), 1, i));
		evnt.push_back(Event (std::max (a[i].point1.x, a[i].point2.x), -1, i));
	}
	std::sort(evnt.begin(), evnt.end());

	return evnt;
}

inline std::set<Segm>::iterator prev(std::set<Segm>::iterator it) {
	return it == set_.begin() ? set_.end() : --it;
}

inline std::set<Segm>::iterator next(std::set<Segm>::iterator it) {
	return ++it;
}

std::pair<int,int> ScanLine(std::vector<Segm> &a) {	 // находим искомые индексы для удачного дня или std::pair(-1, -1) в ином случае
	int n = static_cast<int>(a.size());
	std::vector<Event> evnt = FillSortEvents(a);

	set_.clear();
	seg_locations.resize (a.size());
	size_t evnt_size = evnt.size();

	for (size_t i = 0; i < evnt_size; ++i) {
		size_t index = evnt[i].index;

		if (evnt[i].tp != 1) {
			std::set<Segm>::iterator
				nxt = next (seg_locations[index]),
				prv = prev (seg_locations[index]);

			if (nxt != set_.end() && prv != set_.end() && a[i].Intersect (*nxt, *prv)) {
				std::pair ind_pointers = std::make_pair (prv->index, nxt->index);

				return ind_pointers;
			}

			set_.erase (seg_locations[index]);
		}
		else {
			std::set<Segm>::iterator
				nxt = set_.lower_bound (a[index]),
				prv = prev (nxt);

			if (nxt != set_.end() && a[i].Intersect (*nxt, a[index])) {
				return std::make_pair (nxt->index, index);
			}
			if (prv != set_.end() && a[i].Intersect (*prv, a[index])) {
				return std::make_pair (prv->index, index);
			}

			seg_locations[index] = set_.insert (nxt, a[index]);
		}
	}

	return std::make_pair (-1, -1);
}

std::pair<int, int> Solve(std::vector<Segm> &a) {
    std::pair check = std::make_pair(-1, -1);
    std::pair<int, int> answ = ScanLine(a);

    if (answ != check) {	// проверяем, каким выдался день
		++answ.first;
		++answ.second;
		good_day_result = true;

		return answ;
    }
	// здесь вылезает warning, т.к. я не делаю return в некоторых случаях. Это стоит фиксить?
}


int main() {
    std::ios::sync_with_stdio(false);

	std::vector<Segm> segm;
    size_t N;
    std::cin >> N;

    for (size_t i = 0; i < N; ++i) {
		Segm current;
        std::cin >> current;
		current.index = i;
        segm.emplace_back(current);
    }
	std::pair<int, int> answ = Solve(segm);

	if (good_day_result) {	// проверяем, каким выдался день
		std::cout << "YES\n" << answ.first << " " << answ.second;
	} else {
		std::cout << "NO";
	}

    //system("pause");
}