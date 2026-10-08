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

int main()
{
    FILE* f = fopen("comandas_semanas_s1-06.dat", "rb");// falta actualizar 
    if (f == NULL){
        cout<<"No se puede abrir el archivo"<<endl;
    }

    Comanda c;

    int totalProductos = 0;
    int leido = fread(&c, sizeof(Comanda), 1, f);

    while (leido == 1){
        int mozoActual = c.idMozo;
        int productoMozo = 0;
        float comisionMozo = 0;

        while(leido == 1 && c.idMozo == mozoActual){
            productoMozo = productoMozo + c.cantidad;
            comisionMozo = comisionMozo + c.comision;
            leido = fread(&c, sizeof(Comanda), 1, f);
        }

        totalProductos = totalProductos+ productoMozo;
    }
    // Escribir los datos que se mostrara por pantalla
    fclose(f);
    return 0;
} 
