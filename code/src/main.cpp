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




#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <../src/liebmann.cpp>

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

/*
*
*/
void fillPlate(vector<double> &myVector,double value){
	for(unsigned int i=0;i<myVector.size();++i){
		myVector[i] = value;
	}
}

/*
*
*/
void fillPlateWithDelta(vector<double> &myVector,double startvalue,double endvalue,int width){
	double delta = (startvalue - endvalue)/((double) width);
	for(unsigned int i=0;i<myVector.size();++i){
		myVector[i] = startvalue + (delta*i);
	}
}

/*
*
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
                   // spline();
                }else{
                }
            }
        }
    }
}

/*
*
*/
int main(int ac, char* av[])
{
    try {

    	message msj;
    	std::vector<std::string> listI;

        msj.pUp = 25 30 100;
        msj.pDown = 100;
        msj.pLeft = 10 40 50 100;
        msj.pRight = 25 1000 10;
        msj.horizontal = 500;
        msj.vertical = 500;
        msj.sizeSquare = 5;

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

        //En caso de que nose defina ni un archivo, ni una placa aislada, ni temperatura en la placa
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
            liebman(msj);
            cout << "Opcion para calcular" << "\n";
        }
        
        if (vm.count("Visualizar")) {/**********************/  //Sin valor por default.
        	//Conversion a una imagen de opencv de la matriz.
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