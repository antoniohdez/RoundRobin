
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct fecha {
    int dia;
    int mes;
    int anno;
};

int N;
int **matriz, **tablaScores, **tablaEstad, sigFecha=1;
bool run = true, simulado = false;
vector<string> equipos;
vector<int> peso;
fecha fechaJuego;
bool impar = false;

//MANEJO DE FECHAS
bool bisiesto(int a) {
    return !(a%4) && ((a%100) || !(a%400));
}
 
fecha operator +(fecha f1, int d) {
    int dm[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    fecha temp = f1;

    temp.dia += d;
    if(bisiesto(temp.anno)) dm[1] = 29;
    else dm[1] = 28;
    while(temp.dia > dm[temp.mes-1]) {
        temp.dia -= dm[temp.mes-1];
        temp.mes++;
        if(temp.mes > 12) {
            temp.mes = 1;
            temp.anno++;
            if(bisiesto(temp.anno)) dm[1] = 29;
            else dm[1] = 28;
        }
    }
    return temp;
}

void generaPartidos(int x1, int y1, int sX, int sY){
    if(sX != 0){
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
}

void generaPartidosIterativo(){
    unsigned int n,i,j,tmp, ronda, M;
    n = N;
    M = n;
    if (n & 1) M=n+1;
	else       M=n;
    for (ronda=1; ronda<M; ronda++) {
		for (i=1; i<M; i++) {
			for (j=1; j<M; j++) {
				if ( (i+j)%(M-1)== ronda%(M-1)) break;
			}
            if (j==i && M!=n) matriz[i-1][ronda] = 0;
            else if (j==i && M==n)	{matriz[i-1][ronda] = n; tmp=i;}
            else matriz[i-1][ronda] = j;
		}
        if (M==n) matriz[i-1][ronda] = tmp;
	}
    for(int i = 0; i < N; i++){
        matriz[i][0] = i+1;
    }
}

void generaPartidos(){
    long x = N;
    if((x & (x - 1)) == 0){
        generaPartidos(0, 0, N, N);
    }
    else{
        generaPartidosIterativo();
    }
}

void calculaScores(){
	srand(time(NULL));
	for(int i = 0 ; i < N; i++){
		for(int j = 1 ; j < N; j++){
			tablaScores[i][j] = rand() % (peso.at(i)+1);
			if(tablaScores[i][j]>5){
				tablaScores[i][j] = tablaScores[i][j] /2;
			}
		}
	}
}

void calculaEstadisticas(int eq1, int eq2, int goles1, int goles2){
    if(impar && (equipos.at(eq1) == "NO" || equipos.at(eq2) == "NO")){
        return ;
    }
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
	cout << "PJ\tPG\tPE\tPP\tGF\tGC\tDG\tPTS" << endl;
	for(int i = 0; i < N; i++){
		if(equipos.at(matriz[i][0]-1) == "NO") continue;
        for(int j = 0; j < 8; j++){
			cout<<tablaEstad[i][j] << "\t";
		}
        cout << equipos.at(matriz[i][0]-1) << endl;
	}
	cout << endl;
}

void imprimeCalendario(){
	for(int j = 1; j < N; j++){
		cout << "Dia numero " << j << " del torneo" << endl;
		for(int i = 0; i < N; i++){
			if(matriz[i][j] > matriz[i][0]){
                if(!(impar && equipos.at(matriz[i][j]-1) == "NO")){
                    cout << equipos.at(matriz[i][0]-1) << " VS " << equipos.at(matriz[i][j]-1) << endl;
                }
			}
		}
		cout << endl;
	}
	cout << endl;
}

void simulacionFechas(){
	if(sigFecha < N-1){
		cout << "Dia numero " << sigFecha << " del torneo" << endl;
        cout << "Fecha de los partidos: " << fechaJuego.dia<<"/"<<fechaJuego.mes<<"/"<<fechaJuego.anno;
		for(int i = 0; i < N; i++){
			if(matriz[i][sigFecha] > matriz[i][0] && equipos.at(matriz[i][sigFecha]-1) != "NO"){
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
		cout << "Ya se simulo el torneo, no hay mas fechas disponibles" << endl;
	}
}

void simulacionCompleta(){
	if(!simulado){
		for(int j = sigFecha; j < N; j++){
			cout << "Jornada " << j << " del torneo" << endl;
			cout << "Fecha: " << fechaJuego.dia << "/" << fechaJuego.mes << "/" << fechaJuego.anno << endl;
			fechaJuego = fechaJuego + 7;
			for(int i = 0; i < N; i++){
				if(matriz[i][j] > matriz[i][0] && equipos.at(matriz[i][j]-1) != "NO"){
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
		simulado= true;
	}
	else{
		cout << "Ya se simulo el torneo, no hay mas fechas disponibles" << endl;
	}
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
		cout << "El numero de equipos tiene que ser potecia de 2 " << endl;
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
        if(N%2 == 1){
            impar = true;
            N++;
        }
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
        if(impar) peso.push_back(5);
        if(impar) equipos.push_back("NO");
        myfile.close();
		fechaJuego = { 29, 10, 2013 };
    }
    else{
        cout << "No se pudo leer el archivo " <<endl;
		run = false;
    }
}

int main(){
    //leerArchivo("C:\\Users\\SuGaR\\Documents\\Dropbox\\ITESM\\5to Semestre\\Analisis y D. de Algoritmos\\RoundRobin\\Equipos.txt");
    
    leerArchivo("/Users/Antonio/Desktop/Algoritmos/ProyectoParcial/Equipos.txt");
    crearMatriz();
    generaPartidos();
    calculaScores();
	while(run){
		int menu;
        
		cout << "\t\t\t Torneo Round Robin\n " << endl;
		cout << "1- Imprime Calendario de Juegos" << endl;
		cout << "2- Simular liga completa" << endl;
		cout << "3- Simular siguiente por fechas" << endl;
		cout << "4- Imprime Estadisticas" << endl;
		cout << "5- Salir de la Liga" << endl;
		cin >> menu;
        
		if(menu==1){
			cout << "\t\t\t Round Robin\n " << endl;
			imprimeCalendario();
			cout << endl;
		}
		else if(menu==2){
			cout << "\t\t\t Round Robin\n " << endl;
			simulacionCompleta();
			cout << endl;
		}
		else if(menu==3){
			cout << "\t\t\t Round Robin\n " << endl;
			simulacionFechas();
			cout << endl;
		}
		else if(menu==4){
			cout << "\t\t\t Round Robin\n " << endl;
			imprimeEstadisticas();
			cout << endl;
		}
		else if(menu==5){
			run = false;
			cout << endl;
		}
		else{
			cout << "\t\t\t Round Robin\n" << endl;
			cout << "entrada invalida, por favor intente de nuevo" << endl;
			cout << endl;
		}
        
	}
    
    return 0;
    
}