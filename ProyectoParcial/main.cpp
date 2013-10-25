
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int NumEquipos;
vector<string> equipos;
vector<int> peso;
int *tabla;

void leerArchivo(string ruta){
    string delimiter = " ";
    size_t pos;
    string line;
    ifstream myfile(ruta);
    if (myfile.is_open())
    {
        getline(myfile,line);
        NumEquipos = stoi(line);
        
        
        
        while (getline(myfile,line))
        {
            pos = 0;
            string token;
            while ((pos = line.find(delimiter)) != string::npos) {
                token = line.substr(0, pos);
                equipos.push_back(token);
                //cout << token << endl;
                line.erase(0, pos + delimiter.length());
            }
            //cout << line << endl;
            peso.push_back(stoi(line));
        }
        myfile.close();
    }
    else{
        cout << "No se pudo leer el archivo";
    }
}



int main () {
    
    leerArchivo("/Users/Antonio/Desktop/Algoritmos/ProyectoParcial/Equipos.txt");
    
    cout<<"Número de equipos: "<<NumEquipos<<endl;
    
    for(int i =  0; i < NumEquipos; i++){
        cout<<equipos.at(i)<<" "<<peso.at(i)<<endl;
    }
    
    return 0;
}