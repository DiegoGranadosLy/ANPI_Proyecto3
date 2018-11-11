#include <curses.h>
#include <Matrix.hpp>
#include <math.h>
#include <omp.h>
#include <vector>

using namespace std;

const double acceptable = 0.001;
const int n = 9;

struct message{
  vector<double> pUp;
  vector<double> pDown;
  vector<double> pLeft;
  vector<double> pRight;
  int horizontal;
  int vertical;
  int sizeSquare;
};

double cal_error(double &_a, double &_b)
{
	return ((a-b)/a) * 100;
}

void Liebmann(message _s)
{
	vector<double> _pUp, _pDown, _pLeft, _pRight;
	double _tmp, _error, _testAcept;
	int _hor, _ver, _sizeSquare, _up, _left, _rigth, _down;

	_pup = _s.pUp;
	_pDown = _s.pDown;
	_pLeft = _s.pLeft;
	_pRight = _s.pRight;

	_hor = _s.horizontal;
	_ver = _s.vertical;

	double _matrix*;
	_matrix = (double*) calloc (_hor * _ver, sizeof(double));

	for (int k = n; k > 0; k--)
	{
		for (int i = 0; i < _ver; i++)
		{
			for (int j = 0; j < _hor; j++)
			{
				_up = j-1, _down = j+1, _left = i-1, _rigth = i+1;
				if (_up == -1)
					_up = _pLeft;
				if (_down == _ver)
					_down = _pDown;
				if (_left == -1)
					_left = _pLeft;
				if (_rigth == _hor)
					_rigth = _pRight;

				_tmp = _matrix + i*j;
				_matrix + i*j = (_matrix + i*_up) + (_matrix + i*_down) + (_matrix + j*_left) + (_matrix + j*_rigth);
				_error = cal_error(_matrix + i*j, _tmp);

				if (_error >  _testAcept)
					_testAcept = _error; 
			}
		}
	}
}