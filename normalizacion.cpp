#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

struct ComandaHistorica{
char fecha[11]; // "DD-MM-AAAA"
char nombreMozo[50]; // el nombre completo, repetido en cada venta
int codigoProducto;
int cantidad;
float comision;
};

struct Mozo {
    int idMozo;
    char nombre[50];
    char password[20];
    float totalComision;
};

struct Producto {
    int codigo;
    char descripcion[50];
    float precio;
    int stockActual;
};

struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

void ordenarPorFecha(ComandaHistorica C[], int lenCH);
string fechaString(const char f[]);
void generarClave(Mozo &m);
void cargarArrayMozos(ComandaHistorica C[], int lenC, Mozo M[], int &lenM);

int main(){

    ComandaHistorica comandas[500]; //Array para almacenar las comandas hitoricas

    FILE* fHistorica = fopen("comandas_historicas.dat","rb"); //Abro el archivo de comandas históricas
    int lenCH = 0;

    if(fHistorica == NULL){
        cout<<"Error al abrir el archivo comandas_historicas.dat"<<endl;
        return 1;   
    }

    while(fread(&comandas[lenCH],sizeof(ComandaHistorica),1,fHistorica) == 1){  //While para cargar el Array de comandas históricas

        lenCH++;         //Aumento la longitud del array de comandas históricas
    }

    fclose(fHistorica); //Cierro el archivo de comandas históricas

    ordenarPorFecha(comandas,lenCH); //Ordeno el Array de comandas históricas por fecha    

    //Creo un nuevo archivo con Las comandas ordenadas

    FILE* fHistoricaNueva = fopen("comandas_ordenadas.dat","wb"); 

    if(fHistoricaNueva == NULL){
        cout<<"Error, No se pudo crear el archivo comandas_ordenadas.dat"<<endl;
        return 1;
    }

    for(int i = 0; i < lenCH; i++){ //Carga en el muevo archivo

        fwrite(&comandas[i], sizeof(ComandaHistorica), 1,fHistoricaNueva);
    }

    fclose(fHistoricaNueva);

    Mozo mozos[30];     //Array para almacenar los mozos
    int lenMozos = 0;

    cargarArrayMozos(comandas, lenCH, mozos, lenMozos);    //Carga el array de mozos con los mozos de las comandas históricas

    FILE* fmozos = fopen("mozos.dat","wb");     //Creo un nuevo archivo con Los mozos

    if(fmozos == NULL){
        cout<<"Error, No se pudo crear el archivo mozos.dat"<<endl;
        return 1;
    }

    for(int i = 0; i < lenMozos; i++){ //Carga en el archivo mozos.dat

        fwrite(&mozos[i], sizeof(Mozo), 1,fmozos);
    }

    fclose(fmozos);

    return 0;
}

//FUNCIONES:

string fechaString(const char fecha[]){
                        //Devuelve el array DD-MM-AAAA en string AAAAMMDD. Para despues poder compararlo
    string s = fecha;
    
    string anio = s.substr(6,4);
    string mes = s.substr(3,2);
    string dia = s.substr(0,2);

return anio + mes + dia;
}

void ordenarPorFecha(ComandaHistorica C[], int lenCH){ //ordenamiento por Selección

    for(int i = 0; i < lenCH; i++){
        
        int minId = i;
        for(int j = i + 1; j < lenCH; j++){

            if(fechaString(C[j].fecha) < fechaString(C[minId].fecha)){ 
                            //USO la funcion fechaString() para comparar las fechas
                minId = j;
            }
        }
        ComandaHistorica temp = C[i]; //intercambio los arrays
        C[i] = C[minId];
        C[minId] = temp;
    }
}

void generarClave(Mozo &m){
    //La clave se crea con dos letras del nombre del mozo y los dos primeros numeros de su ID
    //Ejm: idMozo = 1010 nombre = Juan Perez -----> Clave: ua10
    m.password[0] = m.nombre[1] + 7;
    m.password[1] = m.nombre[2] + 7;
    m.password[2] = '0' + ((m.idMozo % 100) / 10) + 7;
    m.password[3] = '0' + ((m.idMozo % 100) % 10) + 7;
    m.password[4] = '\0';
    //La encriptacion se hace con un desplazamiento de 7 unidades
}

void cargarArrayMozos(ComandaHistorica C[], int lenC, Mozo M[], int &lenM){ //Carga el array de mozos del array de comandas históricas

    for(int i = 0; i < lenC; i++){

        bool  encontrado = false;
        for(int j = 0; j < lenM; j++){

            if(!strcmp(C[i].nombreMozo, M[j].nombre)){  //Compara los mozos y si encuentra uno igual, le suma la comision al total
                M[j].totalComision += C[i].comision;
                encontrado = true;
                break;
            }            
        }

        if (encontrado == false){   //Si no encuentra al mozo, lo agrega al array de mozos
            
            M[lenM].idMozo = 1000 + lenM;           //Le asigna un idMozo unico, a partir de 1000, para cada mozo nuevo
            strcpy(M[lenM].nombre, C[i].nombreMozo);
            generarClave(M[lenM]);                  //Genero laclave del mozo
            M[lenM].totalComision = C[i].comision;  //Asigno comision de la comanda historica
            lenM++;
        }
    }
}