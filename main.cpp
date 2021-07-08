#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream> 
#include <vector>
#include <time.h>
#include <math.h>
#include <chrono>
#define TEMP_INICIAL 1000000000000000
#define TEMP_FINAL 0.90 
#define ALPHA 0.935 // factor de enfriamiento temp*ALPHA 
using namespace std;

/*
@Funcion: Inicializa una solucion aleatoria 
@Parametros: null
@Retorna: nueva matriz solucion 
*/
vector<vector<int>> inicializacion(){
    vector<vector<int>> matriz(10000, vector<int>(2,0));
    ifstream archivo;
    archivo.open("numeros.txt",ios::in);
    string num;
    if (archivo.is_open()){
        for (int i=0; i< matriz.size(); i++){
            getline(archivo,num);
            matriz[i][0]= stoi(num);
            matriz[i][1]= rand()%3+1;
        }
    }else{
        cout<< "Problema al abrir el archivo" << endl;
    }
    return matriz;
}
/*
@Funcion: solucion vecina cambiando una piedra de bolsa.
@Parametros: solucion actual
@Retorna: nueva solucion vecina 
*/
vector<vector<int>> solucion_vecina(vector<vector<int>> matriz){ //asigna una solucion random a al vector
    vector<vector<int>> nuevo_random= matriz; 
    int maximo = matriz.size()-1;
    int num1;
    int random;
    num1 = rand()%maximo; 
    do {
        random = rand()%3+1;
    }while(nuevo_random[num1][1]==random);
    nuevo_random[num1][1]=random;
    return nuevo_random;
}
/*
@Funcion: solucion vecina cambiando dos piedras de bolsa.
@Parametros: solucion actual
@Retorna: nueva solucion vecina 
*/
vector<vector<int>> solucion_vecina2(vector<vector<int>> matriz){ //asigna una solucion random a al vector
    vector<vector<int>> nuevo_random= matriz; 
    int maximo = matriz.size()-1;
    int num1, num2; 
    num1 = rand()%maximo;
    num2 = rand()%maximo;
    nuevo_random[num1][1]=rand()%3+1;
    nuevo_random[num2][1]= rand()%3+1;
    return nuevo_random;
}

/*
@Funcion: Evalua el valor de acertacion de una solucion. 
@Parametros: una matriz solucion.
@Retorna: long int del valor fitness
*/
long int fitness_function(vector<vector<int>> matriz){ 
    long int s1=0;
    long int s2=0;
    long int s3=0; //suma de cada grupo 
    for (int i =0; i< matriz.size();i++){
        if (matriz[i][1]==1){
            s1+= matriz[i][0];
        }
        if (matriz[i][1]==2){
            s2+= matriz[i][0];
        }
        if (matriz[i][1]==3){
            s3+= matriz[i][0];
        }
    }
    //cout << s1 << ", " << s2 << ", " << s3 <<  endl;
    return (s1 - s2)*(s1 - s2) + (s1 - s3)*(s1 - s3) + (s2 - s3)*(s2 - s3);
}

/*
@Funcion: Calcula la probabilidad de pasar a una solucion peor.
@Parametros: long int diferencia de costo, long double temperatura actual
@Retorna: double probabilidad random.  
*/
double prob_cambio(long int diferencia_costo, long double temp){
    double probrabilidad= exp(-diferencia_costo/temp); 
    return probrabilidad;
}
/*
@Funcion: copia una matriz solucion en otro diferente.
@Parametros: nueva matriz solucion
@Retorna: nueva matriz copiada. 
*/
vector<vector<int>> copiar_solucion(vector<vector<int>> nueva_solucion){
    vector<vector<int>> nueva(nueva_solucion.begin(),nueva_solucion.end());
    return nueva;
}

/*
@Funcion: Muestra los resultados de la solucion encontrada.
@Parametros: matriz solucion actual, valor fitness, tiempo. 
@Retorna: nulo. 
*/
void mostrar_resultado(vector<vector<int>>  matriz, long int fitness, int tiempo){
    long int s1=0;
    long int s2=0;
    long int s3=0; //suma de cada grupo 
    for (int i =0; i< matriz.size();i++){
        if (matriz[i][1]==1){
            s1+= matriz[i][0];
        }
        if (matriz[i][1]==2){
            s2+= matriz[i][0];
        }
        if (matriz[i][1]==3){
            s3+= matriz[i][0];
        }
    }
    cout << "Mejor fitness encontrado: " << fitness << endl;
    cout << "Duracion: " << tiempo << " segundos" << endl;
    cout << "S1: " << s1 << ", s2: " << s2 << ", s3: "<< s3 << endl;
}

int main(int argc, char* argv[]){ 
    srand(time(NULL)); //inicializa la semilla 
    long int diferencia_costo;
    long double temp = TEMP_INICIAL;
    double random_number;
    double prob_cambio_peor; 
    long int actual_fitness;
    int cant_ite_enfriamiento;
    if (argc>0){
        cant_ite_enfriamiento = atoi(argv[1]);
    }else{ 
        cant_ite_enfriamiento = 100; 
    }
    
    long int mitad = TEMP_INICIAL/2;
    auto tiempo_Inicio = chrono::high_resolution_clock::now();
    vector<vector<int>> solucion_actual = inicializacion();
    long double valor_ideal =0;
    for (int i=0; i <solucion_actual.size(); i++){
        valor_ideal+=solucion_actual[i][0];
    }
    long int nuevo_fitness; 
    cout << "Trabajando...." << endl; 
    vector<vector<int>> nueva_solucion = inicializacion();
    vector<vector<int>> mejor_solucion = inicializacion();
    long int mejor_fitness = fitness_function(mejor_solucion);
    int iteraciones=0;
    //comienza el algoritmo. 
    while (temp > TEMP_FINAL){
        for (int i =0 ; i< cant_ite_enfriamiento; i ++){ 
            if (temp<mitad){
                nueva_solucion = solucion_vecina(nueva_solucion); //evalua cambiando una piedra
            }else{
                nueva_solucion = solucion_vecina2(nueva_solucion); //evalua cambiando varias piedras
            }

            nuevo_fitness = fitness_function(nueva_solucion);
            actual_fitness = fitness_function(solucion_actual);
            
            if ( nuevo_fitness < actual_fitness){ //es mejor solucion
                solucion_actual = copiar_solucion(nueva_solucion);
                if ( actual_fitness < mejor_fitness){
                    mejor_solucion = copiar_solucion(solucion_actual);
                    mejor_fitness = actual_fitness;
                }

            }else{
                diferencia_costo = nuevo_fitness-actual_fitness;
                prob_cambio_peor = prob_cambio(diferencia_costo, temp);
                random_number = (double)rand()/RAND_MAX;
                if (prob_cambio_peor > random_number){
                    solucion_actual = copiar_solucion(nueva_solucion);
                }
            }
        }
        iteraciones+=1;
        temp = temp*ALPHA;
    }
    auto tiempo_final = chrono::high_resolution_clock::now();
    auto duracion = chrono::duration_cast<chrono::seconds>(tiempo_final-tiempo_Inicio);
    cout << "Iteraciones totales: " << iteraciones*cant_ite_enfriamiento << endl;
    mostrar_resultado(mejor_solucion, actual_fitness,duracion.count());
    return 0;
}