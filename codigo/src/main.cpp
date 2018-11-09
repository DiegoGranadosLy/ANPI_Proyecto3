#include <boost/program_options.hpp>
#include <iostream>
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

void fillPlate(vector<double> &myVector,double value){
	for(unsigned int i=0;i<myVector.size();++i){
		myVector[i] = value;
	}
}


void fillPlateWithDelta(vector<double> &myVector,double startvalue,double endvalue,int width){
	double delta = (startvalue - endvalue)/((double) width);
	for(unsigned int i=0;i<myVector.size();++i){
		myVector[i] = startvalue + (delta*i);
	}
}

int main(int ac, char* av[])
{
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
            ("Aislar,i",po::value<std::vector<string> >(&listI)->multitoken()->default_value(" t b l r "),
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
            cout << "Opcion p" << "\n";
            cout << "P: " << vm["Archivo"].as<std::string>() << ".\n";
        }

        if (vm.count("Aislar")) {							//Bandera para aislar la placa
    		cout << "listI is length " << listI.size() << endl;
    		for(unsigned int i = 0; i < listI.size(); i++){
      			if(listI[i].compare("t")){					//Aislar placa de Arriba
      				fillPlate(msj.pUp,(double)0);
      			}else 
      				if (listI[i].compare("b")){				//Aislar placa de Abajo
      					fillPlate(msj.pDown,(double)0);
      				}else 
      					if(listI[i].compare("l")){			//Aislar placa izquierda
      						fillPlate(msj.pLeft,(double)0);
      					}else
      						if(listI[i].compare("r")){		//Aislar placa derecha
      							fillPlate(msj.pRight,(double)0);
      						}else {
      							cout << "\n Unknown option \n" << endl;
      						}
  			}
        }

        if (vm.count("Calcular")) {/************************/  //Sin valor por default.
            //Llamada al maetodo de Liebman para calcular el perfil termico.
            //liebman(msj);
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