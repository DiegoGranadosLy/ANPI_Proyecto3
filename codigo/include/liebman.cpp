

namespace liebman{
	/**
	* @brief calcula el error entre un valor anterior y el nuevo.
	* @param double &_a Es el valor anterior 
	* @param double &_b Es el valor nuevo
	* @return Porcentaje de error entre ambos datos.
	*/
	double cal_error(double &_a, double &_b){
		return ((_a-_b)/_a) * 100;
	}

	/**
	* @brief Calcula la temperatura de una placa. Placa representada por una matriz
	* @param message _s Es un struct con los parametros de configuracion.
	* @param double **tempMatrix Matriz de representacion para la placa.
	* @return none
	*/
	void Liebmann(message _s,double *tempMatrix){
		vector<double> _pUp, _pDown, _pLeft, _pRight;
		double _tmp, _error, _testAcept;
		double valueUp,valueDown,valueLeft,valueRight;
		double *tmp2;
		int   _detectUp, _detectDown, _detectLeft, _detectRigth;
		int   _hor, _ver;

		_pUp    = _s.pUp;
		_pDown  = _s.pDown;
		_pLeft  = _s.pLeft;
		_pRight = _s.pRight;

		_hor = _s.horizontal;
		_ver = _s.vertical;
		for (int k=iterations-8;k>0;--k){
			for (int i=0;i<_ver;++i){
				for (int j=0;j<_hor;++j){
					 _detectUp = j-1, _detectDown = j+1, _detectLeft = i-1, _detectRigth = i+1;
					if (_detectUp == -1)				
						valueUp = _pUp[j];				//Borde superior
					else
						valueUp = *(tempMatrix+(i)*j);	//Borde interno

					if (_detectDown == _ver)			
						valueDown = _pDown[j];			//Borde inferior
					else
						valueDown = *(tempMatrix+(i)*j);	//Borde interno
					
					if (_detectLeft == -1)				
						valueLeft = _pLeft[i];			//Borde izquierdo
					else
						valueLeft = *(tempMatrix+i*(j));	//Borde interno
					
					if (_detectRigth == _hor)			
						valueRight = _pRight[i];		//Borde derecho
					else
						valueRight = *(tempMatrix+i*(j));//Borde interno

					_tmp  = *(tempMatrix+i*j);
					tmp2  =   tempMatrix+i*j;

					*tmp2 = (valueUp + valueDown + valueRight + valueLeft)/((double)4);
					_error = cal_error(*tmp2, _tmp);
					cout << *tmp2 << endl;
					if (_error >  _testAcept)
						_testAcept = _error; 
				}
			}
		}
	}
}//End of namespace LIEBMAN