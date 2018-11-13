
#ifndef LIEBMAN_HPP
#define LIEBMAN_HPP

#include <curses.h>
#include <math.h>
#include <omp.h>
#include <vector>


using namespace std;

struct message{
		vector<double> pUp;
		vector<double> pDown;
		vector<double> pLeft;
		vector<double> pRight;
		int horizontal;
		int vertical;
		int sizeSquare;
	};

namespace liebman{
	const double acceptable = 0.001;
	const int iterations = 9;

	double cal_error(double &_a, double &_b);
	void Liebmann(message _s,double *matrix);
} // namespace LIEBMAN

#include <liebman.cpp>

#endif
