#include <iostream>
#include <cstdio>
using namespace std;

struct Comanda
{
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

void recorrerArchivo(const char* semana,const char* mes);

int main(){
    char semana[10];
    char mes[10];

    cout<<"RESUMEN GENERAL DE LA SEMANA"<<endl;
    cout<<"Ingrese la semana que quiera revisar (ej: s1,s2...)"<<endl;
    cin>> semana;
    cout<<"Ingrese el mes (ej: 01, 11,...)"<<endl;
    cin>>mes;

    recorrerArchivo(semana, mes);
    return 0;
}

void recorrerArchivo (const char* semana, const char* mes){
    char nombreArchivo[50];
    sprintf(nombreArchivo, "comandas_semana_%s-%s.dat", semana, mes);

    FILE* f = fopen(nombreArchivo, "rb");
    if (f = NULL){
        cout << "No se encontro el archivo "<<nombreArchivo<<endl;
        return;
    }

    Comanda c;
    int totalProdVendidos = 0; // falta revisar si es cantidad o monto total de ventas
    int leido = fread(&c, sizeof(Comanda), 1, f);

    while(leido == 1){
        int mozoActual = c.idMozo;
        int cantidadTotal = 0;
        int comisionTotal =  0;

        while(leido == 1 && c.idMozo == mozoActual){
            cantidadTotal = cantidadTotal + c.cantidad;
            comisionTotal = comisionTotal + c.comision;
            leido = fread (&c,sizeof(Comanda), 1, f);
        }
        totalProdVendidos = totalProdVendidos+ cantidadTotal;
        cout<<"ID MOZO: "<<mozoActual
            <<"Cantidad total de ventas: "<< cantidadTotal
            <<"Comision total de Mozo: "<< comisionTotal<<endl;
    }

    cout<<"Total de productos vendidos: "<< totalProdVendidos;
}
