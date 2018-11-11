//Instituto Tecnologico de Costa Rica
//Area Academica de Ingenieria en Computadores
//Analisis Numerico para Ingenieria
//Proyecto 3: Calculo del perfil termico de una 
//			  placa por el metodo de Liebmman
//Profesor: Dr.Pablo Alvarado Moya
//Estudiantes:
//			Nestor Baltodano
//			Diego Granados
//Segundo Semestre, 2018



/**
* @file main.cpp
* @Author Nestor Baltodano, Diego Granados
* @date 10/11/2018
* @brief Menu Implementation with boost/program_options and cubic spline interpolation
*/
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <opencv2/core.hpp>    // For cv::Mat
#include <opencv2/highgui.hpp> // For cv::imread/imshow
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <vector>

namespace po = boost::program_options;
using namespace std;


struct message{
	vector<double> pUp;
	vector<double> pDown;
	vector<double> pLeft;
	vector<double> pRight;
	string pathFile;
	int horizontal;
	int vertical;
	int sizeSquare;
};

struct SplineSet{
    double a;
    double b;
    double c;
    double d;
    double x;
};

//Definiciones de los metodos
void fillPlate(vector<double> &myVector,double value);
void fillPlateWithDelta(vector<double> &myVector,double startvalue,double endvalue,int width);
void ReadFile(string path, message &msj);
void spline(vector<double> &myVector,vector<string> &strs);
vector<SplineSet> getSpline(vector<double> &x, vector<double> &y);

/**
* @brief Menu of the program
* @return 0 if all run ok, -1 if there is a problem
*/
int main(int ac, char* av[]){
    try {

    	message msj;
    	std::vector<std::string> listI;

        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "Help message")
            ("TempBS,t",po::value<double>(), "Temperatura borde superior")
            ("TempBI,b",po::value<double>(), "Temperatura borde inferior")
            ("TempBL,l",po::value<double>(), "Temperatura borde izquierdo")
            ("TempBD,d",po::value<double>(), "temperatura borde derecho")
            ("Aislar,i",po::value<std::vector<string> >(&listI)->multitoken(),
            			"Aislar los bordes indicados (t=arriba , b=abajo, i=izquierda, r=derecha)")
            ("Archivo,p",po::value<string>(), "Nombre del archivo con el perfil termico")
            ("Horizontal,h",po::value<int>()->default_value(500), "Numero de pixeles horizontales")
            ("Vertical,v",po::value<int>()->default_value(500), "Numero de pixeles verticales")
            ("Visualizar,q", "Desactivar la visualizacion")
            ("Calcular,f", "Activar el calculo de flujo de calor")
            ("Tamano,g",po::value<int>()->default_value(10), "Tamano de la rejilla para el calculo de calor")
        ;

        po::variables_map vm;        
        po::store(po::parse_command_line(ac, av, desc), vm);
        po::notify(vm);    

        if (vm.count("help")) {
            cout << desc << "\n";							//Desplegar ayuda
        }

        if (vm.count("Horizontal")) {						//Bandera para numero de pixeles horizontales.500 por default
            msj.horizontal = vm["Horizontal"].as<int>();	//Almacenando en struct el valor.
            msj.pUp.resize(msj.horizontal);					//Redimensionando el vector que representa la placa de arriba.
            msj.pDown.resize(msj.horizontal);				//Redimensionando el vector que representa la placa de abajo.
        }

        if (vm.count("Vertical")) {							//Bandera para el numero de pixeles verticales. 500 por default
            msj.vertical = vm["Vertical"].as<int>();		//Almacenando el valor en un struct
            msj.pLeft.resize(msj.horizontal);				//Redimensionando el vector que representa la placa izquierda
            msj.pRight.resize(msj.horizontal);				//Redimensionando el vector que representa la placa derecha
        }

        if (vm.count("Tamano")) {//Sin valor por default.	//Bandera para el tamano de la celda de visualizacion.
            msj.sizeSquare = vm["Tamano"].as<int>();		//Almacenando el valor en un struct.
        }

        if (vm.count("TempBS")) {//Sin valor por default.	//Temperatura en placa de arriba
        	fillPlate(msj.pUp,vm["TempBS"].as<double>());
        }

        if (vm.count("TempBI")) {//Sin valor por default.	//Temperatura en placa de abajo
            fillPlate(msj.pDown,vm["TempBI"].as<double>());
        }

        if (vm.count("TempBL")) {//Sin valor por default.	//Temperatura en placa Izquierda
            fillPlate(msj.pLeft,vm["TempBL"].as<double>());
        }

        if (vm.count("TempBD")) {//Sin valor por default.	//Temperatura en placa derecha
            fillPlate(msj.pRight,vm["TempBD"].as<double>());
        }

        if (vm.count("Archivo")) {/*************************/ //Sin valor por default. Ruta absoluta
            ReadFile(vm["Archivo"].as<std::string>(),msj);
        }

        if (vm.count("Aislar")) {							//Bandera para aislar la placa
    		cout << "listI is length " << listI.size() << endl;
    		for(unsigned int i = 0; i < listI.size(); i++){
      			if(listI[i].compare("t")==0){					//Aislar placa de Arriba
      				fillPlate(msj.pUp,(double)0);
      			}else 
      				if (listI[i].compare("b")==0){				//Aislar placa de Abajo
      					fillPlate(msj.pDown,(double)0);
      				}else 
      					if(listI[i].compare("l")==0){			//Aislar placa izquierda
      						fillPlate(msj.pLeft,(double)0);
      					}else
      						if(listI[i].compare("r")==0){		//Aislar placa derecha
      							fillPlate(msj.pRight,(double)0);
      						}else {
      							cout << "\n Unknown option \n" << endl;
      						}
  			}
        }

        //En caso de que no se defina ni un archivo, ni una placa aislada, ni temperatura en la placa
        if(!(vm.count("Aislar"))&& !(vm.count("Archivo"))){
            if(!(vm.count("TempBS")) || !(vm.count("TempBI")) || !(vm.count("TempBL")) || !(vm.count("TempBD"))){
                fillPlate(msj.pUp,(double)0);
                fillPlate(msj.pDown,(double)0);
                fillPlate(msj.pLeft,(double)0);
                fillPlate(msj.pRight,(double)0);
            }
        }

        if (vm.count("Calcular")) {/************************/  //Sin valor por default.
            //Llamada al maetodo de Liebman para calcular el perfil termico.
            //liebman(msj);
            cout << "Opcion para calcular" << "\n";
        }
        
        if (vm.count("Visualizar")) {/**********************/  //Sin valor por default.
        	//Conversion a una imagen de opencv de la matriz.
            // double map[20][20] =   {{1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10},
            //                         {1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10},
            //                         {1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10},
            //                         {1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10},
            //                         {1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10},
            //                         {1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10},
            //                         {1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10},
            //                         {1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10},
            //                         {1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10},
            //                         {1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10},
            //                         {1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10},
            //                         {1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10},
            //                         {1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10},
            //                         {1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10},
            //                         {1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10},
            //                         {1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10},
            //                         {1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10},
            //                         {1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10},
            //                         {1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10},
            //                         {1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10},}
            // // map /= 255.0f; // normalize image range to 0 .. 255
            // cv::imshow(mapPath,map);
            cout << "Opcion de visualizacion" << "\n";
        }

    }
    catch(exception& e) {
        cerr << "error: " << e.what() << "\n";
        return 1;
    }
    catch(...) {
        cerr << "Exception of unknown type!\n";
    }
    return 0;
}

/**
* @brief Fill the vector with a given value
* @param myVector vector representing a plate
* @param value constant value on the vector
* @return none
*/
void fillPlate(vector<double> &myVector,double value){
    for(unsigned int i=0;i<myVector.size();++i){
        myVector[i] = value;
    }
}

/**
* @brief Fill the vector with linear formula
* @param myVector vector representing a plate
* @param startvalue Start value
* @param endvalue Final value
* @param widht Size of the vector
* @return none
*/
void fillPlateWithDelta(vector<double> &myVector,double startvalue,double endvalue,int width){
    double delta = (startvalue - endvalue)/((double) width);
    for(unsigned int i=0;i<myVector.size();++i){
        myVector[i] = startvalue + (delta*i);
    }
}

/**
* @brief Create a Queue
* @param path path to the file
* @param msj struct with many params
* @return none
*/
void ReadFile(string path, message &msj){
    std::ifstream infile(path);
    std::string line;
    vector<string> strs;
    vector<double> *v(0);
    int width =0;
    while (std::getline(infile, line)) {                //Se lee el archivo linea por linea
        boost::split(strs,line,boost::is_any_of(" "));  //Se parte la linea por el separador de espacio.
        if(strs[0].compare("top")==0){                  //Placa superior
            v = &msj.pUp;
            width = msj.horizontal;
        }else
            if(strs[0].compare("bottom")==0){           //Placa inferior
                v = &msj.pDown;
                width = msj.horizontal;
            }else
                if(strs[0].compare("left")==0){         //Placa izquierda
                    v = &msj.pLeft;
                    width = msj.vertical;
                }else
                    if(strs[0].compare("right")==0){    //Placa derecha
                        v = &msj.pRight;
                        width = msj.vertical;
                    }    

        if(strs.size()==3){         //Valor constante en la placa
            fillPlate(*v,atof(strs[2].c_str()));
        }else{
            if(strs.size()==4){     //Incremento lineal en la temperatura de la placa
                fillPlateWithDelta(*v,atof(strs[2].c_str()),atof(strs[3].c_str()),width);
            }else{
                if (strs.size()>4){ //Spline cubico para el calculo de la temperatura de la placa.
                    spline(*v,strs);
                }else{
                }
            }
        }
    }
}

/**
* @brief Interpolate a function in a series of points
* @param myVector Vector representing the plate
* @param strs Line readed from the file
* @return none
*/
void spline(vector<double> &myVector,vector<string> &strs){
    vector<double> x(strs.size()-2);
    vector<double> y(x.size());
    double delta = myVector.size()/(strs.size()-2);
    for(unsigned int i=0;i<x.size();++i){
        y[i] = atof(strs[i+2].c_str());
        x[i] = (i+1)*delta;
    }
    x[x.size()-1] = myVector.size();
    vector<SplineSet> cs = getSpline(x, y);
    for(unsigned int i = 0; i < cs.size(); ++i){
        cout << cs[i].a << " *("<< strs[i+2] << "-t)^3 +";
        cout << cs[i].b << " *("<< strs[i+2] << "-t)^2 +";
        cout << cs[i].c << " *("<< strs[i+2] << "-t)^1 +";
        cout << cs[i].d;
        cout << "\n\n\n";
    }
    unsigned int index=0;
    unsigned int seccion=myVector.size()/(strs.size()-2);
    for(unsigned int i=0;i<myVector.size();++i){
        if (i>seccion*index)
            ++index;
        myVector[i] = cs[index].a*(cs[index].x-i)*(cs[index].x-i)*(cs[index].x-i) + 
                      cs[index].b*(cs[index].x-i)*(cs[index].x-i) + 
                      cs[index].c*(cs[index].x-i) + 
                      cs[index].d;
    }

    cout << "resultado del vector..!!\n\n" << endl;
    for(unsigned int i=0;i<myVector.size();++i){
        cout << myVector[i] << endl;
    }


}

/**
* @brief Calculate the constants of the formulas.
* @param x Points to evaluate
* @param y Values evaluates in x
* @return Vector with the coefficients
*/
vector<SplineSet> getSpline(vector<double> &x, vector<double> &y){
    int n = x.size()-1;
    vector<double> a;
    a.insert(a.begin(), y.begin(), y.end());
    vector<double> b(n);
    vector<double> d(n);
    vector<double> h;

    for(int i = 0; i < n; ++i)
        h.push_back(x[i+1]-x[i]);

    vector<double> alpha;
    for(int i = 0; i < n; ++i)
        alpha.push_back( 3*(a[i+1]-a[i])/h[i] - 3*(a[i]-a[i-1])/h[i-1]  );

    vector<double> c(n+1);
    vector<double> l(n+1);
    vector<double> mu(n+1);
    vector<double> z(n+1);
    l[0]  = 1;
    mu[0] = 0;
    z[0]  = 0;

    for(int i = 1; i < n; ++i){
        l[i] = 2 *(x[i+1]-x[i-1])-h[i-1]*mu[i-1];
        mu[i] = h[i]/l[i];
        z[i] = (alpha[i]-h[i-1]*z[i-1])/l[i];
    }

    l[n] = 1;
    z[n] = 0;
    c[n] = 0;

    for(int j = n-1; j >= 0; --j){
        c[j] = z [j] - mu[j] * c[j+1];
        b[j] = (a[j+1]-a[j])/h[j]-h[j]*(c[j+1]+2*c[j])/3;
        d[j] = (c[j+1]-c[j])/3/h[j];
    }

    vector<SplineSet> output_set(n);
    for(int i = 0; i < n; ++i){
        output_set[i].a = a[i];
        output_set[i].b = b[i];
        output_set[i].c = c[i];
        output_set[i].d = d[i];
        output_set[i].x = x[i];
    }
    return output_set;
}