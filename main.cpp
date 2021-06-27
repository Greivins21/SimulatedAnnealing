#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream> 
#include <omp.h>
#include <vector>
#include <time.h> 
#include <math.h>
#define TEMP_INICIAL 400000
#define TEMP_FINAL 300000
#define ALPHA 0.895 // velocidad de enfriamiento o cantidad de iteraciones que se mantiene una temperatura 
using namespace std;

vector<vector<int>> inicializacion(){
    vector<vector<int>> matriz(10, vector<int>(2,0));
    ifstream archivo;
    archivo.open("numeros.txt",ios::in);
    string num;
    if (archivo.is_open()){
        for (int i=0; i< 10; i++){
            getline(archivo,num);
            matriz[i][0]= stoi(num);
            matriz[i][1]= int(rand()%3+1);
        }
    }else{
        cout<< "Problema al abrir el archivo" << endl;
    }
    return matriz;
}

vector<vector<int>> agrupacion_random(vector<vector<int>> matriz){ //asigna una solucion random a al vector
    vector<vector<int>> nuevo_random= matriz; 
    for (int i=0; i< matriz.size(); i++){
            nuevo_random[i][1]= int(rand()%3+1);
        }
    return nuevo_random;
}

long int fitness_function(vector<vector<int>> matriz){ //evalua la solucion
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
   return (s1 - s2)*(s1 - s2) + (s1 - s3)*(s1 - s3) + (s2 - s3)*(s2 - s3);
}

long double prob_cambio(long int diferencia_costo, int time){
    cout << "diferencia de costo " <<diferencia_costo << endl;
    cout << "time " << time << endl;
    cout << "dif/time " << diferencia_costo/time << endl;
    long double prob= exp(-diferencia_costo/time); 
    cout << "prob " << prob << endl;
    return prob;
}
vector<vector<int>> copiar_solucion(vector<vector<int>> nueva_solucion){
    vector<vector<int>> nueva(nueva_solucion.begin(),nueva_solucion.end());
    return nueva;
}
void mostrar_resultado(vector<vector<int>>  matriz){
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
    for (int i=0; i< matriz.size(); i++){
            cout << matriz[i][0] << " / " << matriz[i][1]<< endl;
    }
    long int evaluacion = (s1 - s2)*(s1 - s2) + (s1 - s3)*(s1 - s3) + (s2 - s3)*(s2 - s3);
    cout << "Valor fitness: " << evaluacion << endl;

}
int main(int argc, char* argv[]){
    srand(time(NULL)); //inicializa la semilla 
    vector<vector<int>> solucion_actual = inicializacion();
    vector<vector<int>> nueva_solucion = inicializacion();
    //mostrar_resultado(solucion_actual);
    //mostrar_resultado(nueva_solucion);
    long int diferencia_costo;
    double temp = TEMP_INICIAL;
    double random_number;
    long double prob_cambio_peor; 
    int temp_iter=1;
    int max_temp_iter;
    int cant_ite;
    if (argc>1){
        cant_ite = atoi(argv[1]);
        max_temp_iter= atoi(argv[2]);
    }else{ 
        cant_ite= 1000;
        max_temp_iter=5;    
    }
    
    cout << "empieza " << endl;
    cout << 
    while (temp > TEMP_FINAL){
        for (int i =0 ; i< cant_ite; i ++){
            if (temp_iter < max_temp_iter){
                nueva_solucion =agrupacion_random(nueva_solucion);
                cout << "fitness nuevo: " << fitness_function(nueva_solucion) << endl;
                    cout << "fitness actual: " << fitness_function(solucion_actual) << endl;
                if ( fitness_function(nueva_solucion) < fitness_function(solucion_actual)){ //es mejor solucion
                    solucion_actual= copiar_solucion(nueva_solucion);
                    cout << "fitness solucion actual debe ser fitness nueva solucion: " << fitness_function(solucion_actual)<< endl;
                    cout << "llegue aqui 1" << endl;
                }else{
                    cout << "fitness nuevo: " << fitness_function(nueva_solucion) << endl;
                    cout << "fitness actual: " << fitness_function(solucion_actual) << endl;
                    diferencia_costo = fitness_function(nueva_solucion)-fitness_function(solucion_actual);
                    cout << "diferencia de costo" << diferencia_costo << endl;
                    prob_cambio_peor = prob_cambio(diferencia_costo, temp_iter);
                    cout << "prob cambio peor: " << prob_cambio_peor <<endl;
                    random_number= (long double)rand()/RAND_MAX;
                    cout << "random number: " << random_number << endl;

                    if (prob_cambio_peor > random_number){
                        solucion_actual= copiar_solucion(nueva_solucion);
                        cout << "llegue aqui 5" << endl;
                    }
                }
                temp_iter++;
            }else{
                temp = temp*ALPHA;
                temp_iter = 1;
            }
        }
    }
    //mostrar_resultado(solucion_actual);
    cout << "This is temperature: " <<temp << endl;
    return 0;
}