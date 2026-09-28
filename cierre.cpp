#include <iostream> 
#include <cstdio>
#include <cstring>
#include <string>
using namespace std;


struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

struct estadoCierre {
    char ultimaFecha[11];
    int semana;
};

void nombreArchivoFecha (char nombre[], char fecha[]);
void avanzarFecha(char fecha[]);
bool esBisiesto(int anio);

 
int main () {}

void nombreArchivoFecha (char nombre[], char fecha[]){
    strcpy(nombre, "comandas_");
    strcat(nombre, fecha);
    strcat(nombre, ".dat");
}

void avanzarFecha(char fecha[]) {
    int dia, mes, anio;
    sscanf(fecha, "%2d-%2d-%4d", &dia, &mes, &anio);

    int diasMes[] = {31, esBisiesto(anio) ? 29 : 28, 31, 30, 31, 30,
                     31, 31, 30, 31, 30, 31};

    dia++;
    if (dia > diasMes[mes - 1]) {
        dia = 1;
        mes++;
        if (mes > 12) {
            mes = 1;
            anio++;
        }
    }

    sprintf(fecha, "%02d-%02d-%04d", dia, mes, anio);
}

bool esBisiesto(int anio) {
    return (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
}