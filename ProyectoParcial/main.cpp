
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int N;
int **matriz, **tablaScores, **tablaEstad, sigFecha=1;
bool run = true;
vector<string> equipos;
vector<int> peso;

int generaPartidos(int x1, int y1, int sX, int sY){
    if(sX == 0) return 0;
    else{
        for(int i = 0; i < sY/2; i++){
            matriz[x1+sX/2][y1+sY/2+i] = matriz[x1][y1+i];
        }
        int contY = 0;
        for(int i = sY/2; i <sY; i++){
            matriz[x1+sX/2][y1+contY] = matriz[x1][y1+sY/2+ contY];
            contY++;
        }
        generaPartidos(x1, y1, sX/2, sY/2);
        generaPartidos(x1 + sX/2, y1, sX/2, sY/2);
        generaPartidos(x1, y1 + sY/2, sX/2, sY/2);
        generaPartidos(x1 + sX/2, y1 + sY/2, sX/2, sY/2);
    }
    return 0;
}

void generaPartidos(){
    if(N % 2 == 0){
        generaPartidos(0, 0, N, N);
    }
}

void calculaGanador(){
	srand(time(NULL));
	for(int i = 0 ; i < N; i++){
		for(int j = 1 ; j < N; j++){
			if(peso[i]>5){
				tablaScores[i][j] = rand() % (6);
			}
			else{
				tablaScores[i][j] = rand() % (peso.at(i)+1);
			}
		}
	}

}

void calculaEstadisticas(int eq1, int eq2, int goles1, int goles2){
	//partidos jugados
	tablaEstad[eq1][0]++;
	tablaEstad[eq2][0]++;
	//partidos ganados
	if(goles1>goles2){
		tablaEstad[eq1][1]++;
		tablaEstad[eq2][3]++;
		//pts
		tablaEstad[eq1][7] += 3;
	}
	//partidos empatados
	if(goles1==goles2){
		tablaEstad[eq1][2]++;
		tablaEstad[eq2][2]++;
		//pts
		tablaEstad[eq1][7] += 1;
		tablaEstad[eq2][7] += 1;
	}
	//partidos perdidos
	if(goles1<goles2){
		tablaEstad[eq1][3]++;
		tablaEstad[eq2][1]++;
		//pts
		tablaEstad[eq2][7] += 3;
	}
	//goles a favor
	tablaEstad[eq1][4] +=goles1;
	tablaEstad[eq2][4] +=goles2;
	//goles en contra
	tablaEstad[eq1][5] +=goles2;
	tablaEstad[eq2][5] +=goles1;
	//diferencia de goles
	tablaEstad[eq1][6] = tablaEstad[eq1][4] - tablaEstad[eq1][5];
	
	tablaEstad[eq2][6] = tablaEstad[eq2][4] - tablaEstad[eq2][5];
}

void imprimeEstadisticas(){
	cout << "\t\t\t Tabla Estadisticas" << endl;
	cout << "PJ  PG  PE  PP  GF  GC  DG  PTS" << endl;
	for(int i = 0; i < N; i++){
		for(int j = 0; j < 8; j++){
			cout<<tablaEstad[i][j] << "   ";
		}
		cout << " " << equipos.at(matriz[i][0]-1) << endl;
	}
	cout << endl;
}

void imprimeCalendario(){
	for(int j = 1; j < N; j++){
		cout << "Dia numero " << j << " del torneo" << endl;
		for(int i = 0; i < N; i++){
			if(matriz[i][j] > matriz[i][0]){
				cout << equipos.at(matriz[i][0]-1) << " VS " << equipos.at(matriz[i][j]-1) << endl;
			}
		}
		cout << endl;
	}
	cout << endl;
}

void simulacionFechas(){
	if(sigFecha < N-1){
		cout << "Dia numero " << sigFecha << " del torneo" << endl;
		for(int i = 0; i < N; i++){
			if(matriz[i][sigFecha] > matriz[i][0]){
				cout << equipos.at(matriz[i][0]-1) << " VS " << equipos.at(matriz[i][sigFecha]-1) << endl;
				cout << tablaScores[i][sigFecha] << "-" << tablaScores[matriz[i][sigFecha]-1][sigFecha] <<endl;
				calculaEstadisticas(matriz[i][0]-1, matriz[i][sigFecha]-1, tablaScores[i][sigFecha], tablaScores[matriz[i][sigFecha]-1][sigFecha]);
			}
		}
		cout << endl;
		sigFecha++;
		imprimeEstadisticas();
	}
	else{
		cout << "No hay mas fechas disponibles" << endl;
	}
}

void simulacionCompleta(){
	for(int j = 1; j < N; j++){
		cout << "Dia numero " << j << " del torneo" << endl;
		for(int i = 0; i < N; i++){
			if(matriz[i][j] > matriz[i][0]){
				cout << equipos.at(matriz[i][0]-1) << " VS " << equipos.at(matriz[i][j]-1) << endl;
				cout << tablaScores[i][j] << "-" << tablaScores[matriz[i][j]-1][j] <<endl;
				calculaEstadisticas(matriz[i][0]-1, matriz[i][j]-1, tablaScores[i][j], tablaScores[matriz[i][j]-1][j]);
			}
		}
		cout << endl;
	}
	cout << endl;
	imprimeEstadisticas();
	sigFecha = N-1;
}

void crearMatriz(){
    if(N % 2 == 0){
        matriz = new int *[N];
		tablaScores = new int *[N];
		tablaEstad = new int *[N];
        for(int i = 0 ; i < N; i++){
            matriz[i] = new int[N];
			tablaScores[i] = new int[N];
			tablaEstad[i] = new int [8];
        }
        for(int i = 0 ; i < N; i++){
            for(int j = 0; j < N; j++){
                if(i == 0){
                    matriz[i][j] = j+1;
					tablaScores[i][j] = -1;
                }else{
                    matriz[i][j] = 0;
					tablaScores[i][j] = -1;
                }
            }
			for(int j = 0; j < 8; j++){
				tablaEstad[i][j] = 0;
			}
        }
    }
	else{
		cout << "El numero de equipos tiene que ser par " << endl;
	}
}

void imprimeMatriz(){
    cout << "Matriz equipos" << endl;
	for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            cout<<matriz[i][j]<<" ";
        }
        cout<<endl;
    }
	cout << endl;
	cout << "Tabla Scores" << endl;
	for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            cout<<tablaScores[i][j]<<" ";
        }
        cout<<endl;
    }
	cout << endl;
}

void leerArchivo(string ruta){
    string delimiter = " ";
    size_t pos;
    string line;
    ifstream myfile(ruta);
    if (myfile.is_open())
    {
        getline(myfile,line);
        N = stoi(line);
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
        cout << "No se pudo leer el archivo " <<endl;
    }
}

int main(){
	string exit = "";
    leerArchivo("C:\\Users\\SuGaR\\Documents\\Dropbox\\ITESM\\5to Semestre\\Analisis y D. de Algoritmos\\RoundRobin\\Equipos.txt");
    crearMatriz();
    generaPartidos();
    calculaGanador();
	while(run){
		int menu;

		cout << "\t\t\t Liga Mexicana " << endl;
		cout << "1- Imprime Calendario de Juegos" << endl;
		cout << "2- Simular liga completa" << endl;
		cout << "3- Simular siguiente por fechas" << endl;
		cout << "4- Imprime Estadisticas" << endl;
		cout << "5- Salir de la Liga" << endl;
		cin >> menu;

		if(menu==1){
			system("cls");
			cout << "\t\t\t Liga Mexicana " << endl;
			imprimeCalendario();
			//cin >> exit;
		}
		else if(menu==2){
			system("cls");
			cout << "\t\t\t Liga Mexicana " << endl;
			simulacionCompleta();
			//cin >> exit;
		}
		else if(menu==3){
			system("cls");
			cout << "\t\t\t Liga Mexicana " << endl;
			simulacionFechas();
			//cin >> exit;
		}
		else if(menu==4){
			system("cls");
			cout << "\t\t\t Liga Mexicana " << endl;
			imprimeEstadisticas();
			//cin >> exit;
		}
		else if(menu==5){
			system("cls");
			run = false;
		}
		else{
			system("cls");
			cout << "\t\t\t Liga Mexicana \n" << endl;
			cout << "entrada invalida, por favor intente de nuevo" << endl;
			//cin >> exit;
		}

	}

    return 0;
}