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

    cout<<"----------SISTEMA DE RESUMEN----------"<<endl;
    cout<<"Ingrese la semana que quiera revisar (ej: s1,s2...)"<<endl;
    cin>> semana;
    cout<<"Ingrese el mes (ej: 01, 05, 11,...)"<<endl;
    cin>>mes;

    recorrerArchivo(semana, mes);
    return 0;
}
 
void recorrerArchivo (const char* semana, const char* mes){
    char nombreArchivo[50];
    sprintf(nombreArchivo, "comandas_semana_%s-%s.dat", semana, mes);

    FILE* f = fopen(nombreArchivo, "rb");
    if (f == NULL){
        cout << "No se encontro el archivo "<<nombreArchivo<<endl;
        cout << "Revise si se hizo el cierre de esa semana"<<endl;
        return;
    }

    Comanda c;
    int totalProdVendidos = 0; 

    int leido = fread(&c, sizeof(Comanda), 1, f);
    
    if(leido != 1){
        cout<<"El archivo se encuentra vacio, no hay comandas registradas";
        fclose(f);
        return;
    }

    cout<<"\nRESUMEN DE CIERRE DE SEMANA "<<semana<<", MES "<<mes<<endl;
    cout<<"-------------------------------------------"<<endl;

    while(leido == 1){
        int mozoActual = c.idMozo;
        int cantidadTotal = 0;
        float comisionTotal =  0;

        while(leido == 1 && c.idMozo == mozoActual){
            cantidadTotal = cantidadTotal + c.cantidad;
            comisionTotal = comisionTotal + c.comision;
            leido = fread (&c,sizeof(Comanda), 1, f);
        }
        totalProdVendidos = totalProdVendidos+ cantidadTotal;
        cout<<"ID Mozo: "<<mozoActual<<endl;
        cout<<"Cantidad total de ventas: "<< cantidadTotal<<endl;
        cout<<"Comision total a pagar: $"<< comisionTotal<<endl;
        cout<<"-------------------------------------------"<< endl;
    }
    
    cout<<"-------------------------------------------"<<endl;
    cout<<"Total de productos vendidos del Buffet: "<< totalProdVendidos<<endl;

    fclose(f);
}

