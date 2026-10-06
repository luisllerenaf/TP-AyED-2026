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
bool esDomingo(char fecha[]);
 
int main () {
    estadoCierre estado;

    FILE* fEstado = fopen("estadoCierre.dat", "rb");
    if (fEstado == NULL) {
        strcpy(estado.ultimaFecha, "02-06-2025");
        estado.semana = 1;
    } else {
        fread(&estado, sizeof(estadoCierre), 1, fEstado);
        fclose(fEstado);
    }

    char fecha[11];
    strcpy(fecha, estado.ultimaFecha);

    int mesNum, anioNum, diaNum;
    sscanf(fecha, "%2d-%2d-%4d", &diaNum, &mesNum, &anioNum);

    int diasNulos = 0;
    int n = 0; 

    Aux aux[7]; 

    while (diasNulos < 3 && n < 7) {
        char nombre[30];
        nombreArchivoFecha(nombre, fecha);
    
        FILE* f = fopen(nombre, "rb");
        if (f == NULL) {
            diasNulos++;
        } else {
        aux[n].fIN = f;
        aux[n].fin = fread(&aux[n].actual, sizeof(Comanda), 1, f) != 1;
        n++;
        diasNulos = 0; 
        }

        if (esDomingo(fecha)) break;

        avanzarFecha(fecha);
    }

    if (n == 0) {
        cout << "No se encontraron archivos diarios para esta semana." << endl;
        return 0;
    }

    char mesStr[3];
    sprintf(mesStr, "%02d", mesNum);

    string nombreSemanal = "comandas_semana_s" + to_string(estado.semana) + "-" + string(mesStr) + ".dat";


    FILE* fOUT = fopen(nombreSemanal.c_str(), "wb");
    if (fOUT == NULL) {
    cout << "Error al crear el archivo semanal." << endl;
    return 0;
    }

    while (true) {
        int minIdx = -1;
        for (int i = 0; i < n; i++) {
            if (!aux[i].fin) {
                if (minIdx == -1 || aux[i].actual.idMozo < aux[minIdx].actual.idMozo) {
                    minIdx = i;
                }
            }
        }

        if (minIdx == -1) {
            break; 
        }

        fwrite(&aux[minIdx].actual, sizeof(Comanda), 1, fOUT);

        aux[minIdx].fin = fread(&aux[minIdx].actual, sizeof(Comanda), 1, aux[minIdx].fIN) != 1;
    }

    for (int i = 0; i < n; i++) {
    fclose(aux[i].fIN);
    }
    fclose(fOUT);
    
    char nombreResumen[26];
    strcpy(nombreResumen, nombreSemanal.c_str());

    fwrite(&nombreResumen, sizeof(char), 26, fNombre);

    FILE* fNombre = fopen("Nombre.dat", "wb");
    if (fNombre == NULL) {
    cout << "Error al crear el archivo con el nombre del semanal" << endl;
    return 0;
    }

    fclose(fNombre);
    
    avanzarFecha(fecha);
    strcpy(estado.ultimaFecha, fecha);

    estado.semana++;
    if (estado.semana > 4) {
        estado.semana = 1;
    }

    FILE* fEstadoOut = fopen("estadoCierre.dat", "wb");
    fwrite(&estado, sizeof(estadoCierre), 1, fEstadoOut);
    fclose(fEstadoOut);

    return 0;
}

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

bool esDomingo(char fecha[]) {
    int dia, mes, anio;
    sscanf(fecha, "%2d-%2d-%4d", &dia, &mes, &anio);

    int a = (14 - mes) / 12;
    int y = anio - a;
    int m = mes + 12 * a - 2;

    int diaSemana = (dia + y + y / 4 - y / 100 + y / 400
                     + (31 * m) / 12) % 7;

    return diaSemana == 0;
}