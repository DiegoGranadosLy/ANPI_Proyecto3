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


int main(int ac, char* av[])
{
    try {

    	message msj;

        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "Help message")
            ("TempBS,t",po::value<double>()->default_value(50), "Temperatura borde superior")
            ("TempBI,b",po::value<double>()->default_value(50), "Temperatura borde inferior")
            ("TempBL,l",po::value<double>()->default_value(50), "Temperatura borde izquierdo")
            ("TempBD,d",po::value<double>()->default_value(50), "temperatura borde derecho")
            ("Aislar,i",po::value<string>() , "Aislar los bordes indicados (t=arriba , b=abajo, i=izquierda, r=derecha)")
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
            cout << desc << "\n";
        }

        if (vm.count("Horizontal")) {
            msj.horizontal = vm["Horizontal"].as<int>();
            msj.pUp.resize(msj.horizontal);
            msj.pDown.resize(msj.horizontal);
        }

        if (vm.count("Vertical")) {
            msj.vertical = vm["Vertical"].as<int>();
            msj.pLeft.resize(msj.horizontal);
            msj.pRight.resize(msj.horizontal);
        }

        if (vm.count("Tamano")) {
            msj.sizeSquare = vm["Tamano"].as<int>();
        }

        if (vm.count("TempBS")) {
        	fillPlate(msj.pUp,vm["TempBS"].as<double>());
        }

        if (vm.count("TempBI")) {
            fillPlate(msj.pDown,vm["TempBI"].as<double>());
        }

        if (vm.count("TempBL")) {
            fillPlate(msj.pLeft,vm["TempBL"].as<double>());
        }

        if (vm.count("TempBD")) {
            fillPlate(msj.pRight,vm["TempBD"].as<double>());
        }

        /**/if (vm.count("Aislar")) {
            cout << "Opcion i" << "\n";
            cout << "I: " << vm["Aislar"].as<std::string>() << ".\n";
        }

        /**/if (vm.count("Archivo")) {
            cout << "Opcion p" << "\n";
            cout << "P: " << vm["Archivo"].as<std::string>() << ".\n";
        }

        /**/if (vm.count("Calcular")) {
            cout << "Opcion para calcular" << "\n";
        }
        
        /**/if (vm.count("Visualizar")) {
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