#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

struct Producto {
    int codigo;
    char descripcion[50];
    float precio;
    int stockActual;
};

struct Mozo {
    int idMozo;
    char nombre[50];
    char password[20];
    float totalComision;
};

struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

const float TASA_COMISION = 0.10f;

void obtenerUltimaFecha(char f[]);
string fechaString(const char fecha[]);
bool validarFecha(char fecha[], char ultimaFecha[]);

int main() {

    char fechaActual[11];
    char ultimaFecha[11];

    cout<<"=== SISTEMAS DE VENTAS - BUFFET ALBERT EINSTEIN ==="<<endl;

    //Carga de la fecha actual y validacion de la misma

    do{
        cout<<"Formato de Fecha: DD MM AAAA"<<endl;
        cout<<"(Para salir ingrese '00')"<<endl;
        cout<<"Fecha actual: "<<endl;
        cin.getline(fechaActual, 11);

        obtenerUltimaFecha(ultimaFecha);    //Obtiene la ultima fecha procesada
        
        if(!strcmp(fechaActual, "00")){
            return 0;       //Sale del programa
        }

    }while(!validarFecha(fechaActual, ultimaFecha));

    return 0;
}

void obtenerUltimaFecha(char f[]){ 

    FILE* fControl = fopen("ultima_fecha_procesada.dat","rb");
    if(fControl == NULL){
       strcpy(f,"07-06-2025");  //Si no encuentra el archivo de ultima_fecha_procesada.dat carga como 1ra fecha la ultima procesada en el historico
       return;
    }

    fread(f,11*sizeof(char),1,fControl);    //Busca la ultima fecha en el archivo ultima_fecha_procesada.dat
    fclose(fControl);
}

string fechaString(const char fecha[]){
                        //Devuelve el array DD-MM-AAAA en string AAAAMMDD. Para despues poder compararlo
    string s = fecha;
    
    string anio = s.substr(6,4);
    string mes = s.substr(3,2);
    string dia = s.substr(0,2);

return anio + mes + dia;
}

bool validarFecha(char fecha[], char ultimaFecha[]){ //Valida el formato de fecha ingresado (dd mm aaaa)

    for(int i = 0; i < 10; i++){

        if(i == 2 || i ==5){

            if(fecha[i] != ' '){
                cout<<"Error, respete los espacios entre los numeros de la fecha (Ej.: 02 06 2025)"<<endl;
                return false;
            }
        }

        else if(fecha[i] < '0' || fecha[i] > '9'){

            cout<<"Error, respete el formato de fecha (Ej.: 02 06 2025)"<<endl;
            return false;
        }
    }

    fecha[2] = '-'; //Si el formato de entrada de fecha esta bien agrega los '-'
    fecha[5] = '-';

    int dia = atoi(&fecha[0]);  //Convierte los char que son numeros en enteros, corta cuando no es un numero
    int mes = atoi(&fecha[3]);
    int anio = atoi(&fecha[6]);

    if(dia < 1 || dia > 31 || mes < 1 || mes > 12 || anio > 2035 ){

        cout<<"Error, La fecha Invalida"<<endl;
        return false;
    }

    //Valida que la fecha sea mayor a la ultima fecha procesada

    if(fechaString(fecha) < fechaString(ultimaFecha)){ //Compara si la fecha ingresada es mayot a la ultima fecha procesada
             
        cout<<"Error, la fecha tiene que ser mayor a "<<ultimaFecha<<endl;
        return false;               
    }

    return true;
}