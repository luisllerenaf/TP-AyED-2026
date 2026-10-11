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
int entradaDeFecha(char fechaActual[], char ultimaFecha[]);
int mozosArray(const char* nombre, Mozo arrayMozos[], int &lenM);

int main() {

    char fechaActual[11];
    char ultimaFecha[11];

    cout<<"=== SISTEMAS DE VENTAS - BUFFET ALBERT EINSTEIN ===\n"<<endl;

    //Carga de la fecha actual y validacion de la misma

    if(entradaDeFecha(fechaActual, ultimaFecha) == 1){  //Pido la fecha a procesar, la valido o salgo de programa

        return 1;   //Salgo del programa  
    }

    //Carga de los mozos del archivo "mozos.dat" en un array para poder comparar

    Mozo arrayMozos[100];

    int lenM = 0;

    if(mozosArray("mozos.dat", arrayMozos, lenM) == 1){
        
        return 1;
    }

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

bool validarFecha(char fecha[], char ultimaFecha[]){ //Valida que el formato de fecha ingresado y que sea mayor o igual a la ultima fecha procesada

    for(int i = 0; i < 10; i++){

        if(i == 2 || i ==5){

            if(fecha[i] != ' '){
                cout<<"##Error: Respete los espacios entre los numeros de la fecha (Ej.: 07 06 2025)\n"<<endl;
                return false;
            }
        }

        else if(fecha[i] < '0' || fecha[i] > '9'){

            cout<<"##Error: Respete el formato de fecha (Ej.: 07 06 2025)\n"<<endl;
            return false;
        }
    }

    fecha[2] = '-'; //Si el formato de entrada de fecha esta bien agrega los '-'
    fecha[5] = '-';

    int dia = atoi(&fecha[0]);  //Convierte los char que son numeros en enteros, corta cuando no es un numero
    int mes = atoi(&fecha[3]);
    int anio = atoi(&fecha[6]);

    if(dia < 1 || dia > 31 || mes < 1 || mes > 12 || anio > 2035 ){

        cout<<"###Error: La fecha Invalida\n"<<endl;
        return false;
    }

    //Valida que la fecha sea mayor a la ultima fecha procesada

    if(fechaString(fecha) < fechaString(ultimaFecha)){ //Compara si la fecha ingresada es mayot a la ultima fecha procesada
             
        cout<<"###Error: La fecha tiene que ser mayor o igual a "<<ultimaFecha<<"\n"<<endl;
        return false;               
    }

    return true;
}

int entradaDeFecha(char fechaActual[], char ultimaFecha[]){ //Gestiona la carga de la fecha actual y valida que sea mayor a la ultima fecha procesada

    do{
        cout<<"Ingrese la fecha en Formato: DD MM AAAA"<<endl;
        cout<<"Para salir ingrese: 00"<<endl;
        cout<<"Fecha actual: ";
        cin.getline(fechaActual, 11);

        obtenerUltimaFecha(ultimaFecha);    //Obtiene la ultima fecha procesada
        
        if(!strcmp(fechaActual, "00")){
            return 1;               //Retorna 1 para salir del programa
        }

    }while(!validarFecha(fechaActual, ultimaFecha));    //valida que la fecha sea mayor o igual a la ultima fecha procesada

    return 0;    //Retorna 0 si la fecha es mayor o igual a la ultima fecha procesada
}

int mozosArray(const char* nombre, Mozo arrayMozos[], int &lenM){

    FILE* fmozos = fopen(nombre,"rb");
    if (fmozos == NULL){
        cout<<"#Error, no se pudo abrir el archivo mozos.dat\n"<<endl;
        return 1;
    }

    while(fread(&arrayMozos[lenM], sizeof(Mozo), 1, fmozos) == 1){

        lenM++;
    }
    fclose(fmozos);
    return 0;
}