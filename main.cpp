#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream> 
#include <omp.h>
#include <vector>
#include <time.h> 
#include <math.h>
#define TEMP_INICIAL 100000000000 //400000 cien mil millones. 
#define TEMP_FINAL 10
#define ALPHA 0.90 // velocidad de enfriamiento o cantidad de iteraciones que se mantiene una temperatura 
using namespace std;

vector<vector<int>> inicializacion(){
    vector<vector<int>> matriz(10000, vector<int>(2,0));
    ifstream archivo;
    archivo.open("numeros.txt",ios::in);
    string num;
    if (archivo.is_open()){
        for (int i=0; i< 10000; i++){
            getline(archivo,num);
            matriz[i][0]= stoi(num);
            matriz[i][1]= rand()%3+1;
        }
    }else{
        cout<< "Problema al abrir el archivo" << endl;
    }
    return matriz;
}

vector<vector<int>> solucion_vecina(vector<vector<int>> matriz){ //asigna una solucion random a al vector
    vector<vector<int>> nuevo_random= matriz; 
    int maximo = matriz.size()-1;
    int num1, num2, num3;
    

    //intercambio tres digitos
    /*
    num1 = rand()%maximo;
    num2 = rand()%maximo;
    num3 = rand()%maximo;
    nuevo_random[num1][1]= rand()%3+1;
    nuevo_random[num2][1]= rand()%3+1;
    nuevo_random[num3][1]= rand()%3+1;*/
    

    //intercambiando tres digitos entre ellos
   /* num1 = rand()%maximo;
    num2 = rand()%maximo;
    num3 = rand()%maximo;
    int grupo_temp = nuevo_random[num1][1];
    nuevo_random[num1][1]= nuevo_random[num2][1];
    nuevo_random[num2][1]= nuevo_random[num3][1];
    nuevo_random[num3][1]= grupo_temp;*/
    
    //intercambiando dos digitos al azar
    /*num1 = rand()%maximo;
    num2 = rand()%maximo;
    nuevo_random[num1][1]=rand()%3+1;
    nuevo_random[num2][1]= rand()%3+1;
    */

    //intercambiando dos digitos entre ellos
    num1= rand()%maximo;
    do{ 
        num2=rand()%maximo;
    }while(nuevo_random[num1][1]==nuevo_random[num2][1]);

    int grupo_temp = nuevo_random[num1][1];
    nuevo_random[num1][1]= nuevo_random[num2][1];
    nuevo_random[num2][1]= grupo_temp;
    
    //cambiando un digito
    /*int random ;
    do {
        random = rand()%3+1;
    }while(nuevo_random[num1][1]==random);
    nuevo_random[num1][1]=random;*/
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
    //cout << s1 << ", " << s2 << ", " << s3 <<  endl;
    return (s1 - s2)*(s1 - s2) + (s1 - s3)*(s1 - s3) + (s2 - s3)*(s2 - s3);
}

long double prob_cambio(long int diferencia_costo, long double temp){
    //cout << "diferencia de costo " <<diferencia_costo << endl;
    //cout << "time " << time << endl;
    //cout << "dif/time " << diferencia_costo/time << endl;
    long double prob= exp(-diferencia_costo/temp); 
    //cout << "prob " << prob << endl;
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
   cout << "S1: " << s1 << ", s2: " << s2 << ", s3: "<< s3 << endl;
}

int main(int argc, char* argv[]){
    srand(time(NULL)); //inicializa la semilla 
    long int diferencia_costo;
    long double temp = TEMP_INICIAL;
    double random_number;
    long int prob_cambio_peor; 
    long int actual_fitness;
    int temp_iter=1;
    int cant_ite_enfriamiento;
    if (argc>0){
        cant_ite_enfriamiento = atoi(argv[1]);
    }else{ 
        cant_ite_enfriamiento = 1000; 
    }
    
    
    cout << "empieza " << endl;
    vector<vector<int>> solucion_actual = inicializacion();
    long double valor_ideal =0;
    for (int i=0; i <solucion_actual.size(); i++){
        valor_ideal+=solucion_actual[i][0];
    }
    long int nuevo_fitness; 
    cout << "Valor ideal: "<< valor_ideal << "/3 = "<<valor_ideal/3 << endl;
    vector<vector<int>> nueva_solucion = inicializacion();
    vector<vector<int>> mejor_solucion = inicializacion();
    long int mejor_fitness = fitness_function(mejor_solucion);

    while (temp > TEMP_FINAL){
        for (int i =0 ; i< cant_ite_enfriamiento; i ++){ 
            //cout << "nueva solucion " << endl;
            nueva_solucion = solucion_vecina(nueva_solucion);
            //cout << "fitness nuevo: " << fitness_function(nueva_solucion) << endl;
            //cout << "fitness actual: " << fitness_function(solucion_actual) << endl;
            nuevo_fitness = fitness_function(nueva_solucion);
            actual_fitness = fitness_function(solucion_actual);
            
            if ( nuevo_fitness < actual_fitness){ //es mejor solucion
                solucion_actual = copiar_solucion(nueva_solucion);

                if ( actual_fitness < mejor_fitness){
                    mejor_solucion = copiar_solucion(solucion_actual);
                    mejor_fitness = actual_fitness;
                    cout << "Mejor fitness: " << mejor_fitness <<endl;
                    mostrar_resultado(mejor_solucion);
                }
                /*if (temp< 100){
                    cout << "This is temperature: " << temp << endl;
                    cout << "Mejor valor fitness encontrado: " << mejor_fitness << endl;
                    mostrar_resultado(mejor_solucion);
                }*/
                //cout << "fitness solucion actual debe ser fitness nueva solucion: " << fitness_function(solucion_actual)<< endl;
                //cout << "llegue aqui 1" << endl;
            }else{
                //cout << "fitness nuevo: " << fitness_function(nueva_solucion) << endl;
                //cout << "fitness actual: " << fitness_function(solucion_actual) << endl;
                diferencia_costo = nuevo_fitness-actual_fitness;
                //cout << "diferencia de costo" << diferencia_costo << endl;
                prob_cambio_peor = prob_cambio(diferencia_costo, temp);
                //cout << "prob cambio peor: " << prob_cambio_peor <<endl;
                random_number = (double)rand()/RAND_MAX;
                //cout << "random number: " << random_number << endl;

                if (prob_cambio_peor > random_number){
                    solucion_actual = copiar_solucion(nueva_solucion);
                    cout << "probabilidad: " << prob_cambio_peor << endl;
                }
            }
        }
        temp = temp*ALPHA;
    }
    
    cout << "This is temperature: " << temp << endl;
    cout << "Mejor valor fitness encontrado: " << mejor_fitness << endl;
    mostrar_resultado(mejor_solucion);
    return 0;
}