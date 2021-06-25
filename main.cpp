#include <stdio.h>
#include <iostream>
#include <fstream> 
#include <omp.h>
#include <vector>
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
            matriz[i][1]= int(rand()%3+1);
        }
    }else{
        cout<< "Problema al abrir el archivo" << endl;
    }
    return matriz;
}

int fitness_function(vector<vector<int>> matriz){
    int s1,s2,s3 = 0; //suma de cada grupo 
    for (int i =0; i< matriz.size();i++){
        if (matriz[i][1]==1){
            s1++;
        }
        if (matriz[i][1]==2){
            s2++;
        }
        if (matriz[i][1]==3){
            s3++;
        }
    }
    double evaluacion = (s1 - s2)^2 + (s1 - s3)^2 + (s2 - s3)^2;
    return evaluacion; 
}

int main(){
#pragma omp parallel num_threads(4)
    {
#pragma omp critical 
    cout << "This is proccess: " << omp_get_thread_num() << endl;
    }
    inicializacion();
   return 0;
}