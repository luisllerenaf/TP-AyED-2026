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

string fechaString(const char f[]);
void ordenarPorFecha(ComandaHistorica C[], int lenCH);
void generarClave(Mozo &m);
void cargarArrayMozos(ComandaHistorica C[], int lenC, Mozo M[], int &lenM);
void nombreArchivoFecha(char nombre[], char fecha[]);
void ordenamientoBurbuja(Comanda d[],int lenD);
void corteControlComandas(ComandaHistorica C[], int lenCH, Mozo M[], int lenM);
int actualizarInventario(char fecha[]);
void actualizarTodoInventario(ComandaHistorica C[], int lenC);

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

    //Separo las comandas por dia y las guardo en un archivo

    corteControlComandas(comandas, lenCH, mozos, lenMozos);

    //Actualizamos el inventario

    actualizarTodoInventario(comandas, lenCH);  //Se aprovecha el array de comandas historias ordenadas para actualizar el inventario

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

void nombreArchivoFecha(char nombre[], char fecha[]){//genera el nombre del archivo en formato comandas_dd-mm-aaaa.dat
    
    strcpy(nombre, "comandas_");
    strcat(nombre, fecha);
    strcat(nombre, ".dat");
}

void ordenamientoBurbuja(Comanda d[],int lenD){ //Ordena el array de Comanda[] por el idMozo

    for (int i = 0; i < lenD; i++){

        for (int j = 0; j < lenD -i -1; j++){

            if (d[j].idMozo > d[j + 1].idMozo){

                Comanda temp = d[j];
                d[j] = d[j + 1];
                d[j + 1] = temp;
            }
        }
    }
}
void corteControlComandas(ComandaHistorica C[], int lenCH, Mozo M[], int lenM){

    //Separa las comandas por dia, las carga en un archivo con formato comandas_dd-mm-aaaa.dat ordenado por idMozo

    int i = 0;

    while (i < lenCH){

        char fechActual[11]; 
        char nombreArchivo[30];

        strcpy(fechActual, C[i].fecha);
        nombreArchivoFecha(nombreArchivo, C[i].fecha);

        FILE* fcomanda = fopen(nombreArchivo,"wb");
        if(fcomanda == NULL){
            cout << "Error, no se pudo crear archivo "<< nombreArchivo<<endl;
            return;
        }

        Comanda d[100]; //array de Comandas del dia actual
        int lenD = 0; 

        while (i < lenCH && !strcmp(fechActual, C[i].fecha)){                    
            
            for (int j = 0; j < lenM; j++){

                if (!strcmp(C[i].nombreMozo, M[j].nombre)){

                    d[lenD].idMozo = M[j].idMozo;
                    d[lenD].codigoProducto = C[i].codigoProducto;
                    d[lenD].cantidad = C[i].cantidad;
                    d[lenD].comision = C[i].comision;
                    lenD++;
                    break;
                }
            }                     
            i++;                        
        }

        ordenamientoBurbuja(d,lenD); // Ordeno por idMozo el arreglo de Comandas (de la fecha actual)

        for(int h = 0; h < lenD; h++){ //cargo el arreglo de Comandas ordenado en el archivo comandas_dd-mm-aaaa.dat

            fwrite (&d[h], sizeof(Comanda), 1, fcomanda);
        }

        fclose(fcomanda);
        
    }

    //Creo un archivo de control con la fecha del ultimo dia procesado. Este archivo se actualizara en las ventas diarias.

    FILE* fUltimafecha = fopen("ultima_fecha_procesada.dat", "wb");

    if(fUltimafecha == NULL){
        cout << "Error, no se pudo crear el archivo ultima_fecha_procesada.dat"<<endl;
        return;
    }
    
    fwrite(C[lenCH - 1].fecha, 11*sizeof(char), 1, fUltimafecha);
        
    fclose(fUltimafecha);
}

int actualizarInventario(char fecha[]){ //Actualiza el inventario

    FILE* fInventario = fopen("inventario.dat","rb+");  //abrimos el archivo inventario.dat en modo lectura y escritura
    if(fInventario == NULL){
        cout << "Error, no se pudo abrir el archivo inventario.dat"<<endl;
        return 1;
    }

    char nombreArchivo[30];
    nombreArchivoFecha(nombreArchivo, fecha);

    FILE* fComandaDia = fopen(nombreArchivo,"rb"); //abrimos el archivo comandas_dd-mm-aaaa.dat en modo lectura
    if(fComandaDia == NULL){
        cout << "Error, no se pudo abrir el archivo "<< nombreArchivo<<endl;
        fclose(fInventario);
        return 1;
    }

    Comanda dia;
    Producto prod;

    while (fread(&dia, sizeof(Comanda), 1, fComandaDia) == 1){

        fseek(fInventario, 0, SEEK_SET);           
        while(fread(&prod, sizeof(Producto), 1, fInventario) == 1){

            if(dia.codigoProducto == prod.codigo){

                prod.stockActual -= dia.cantidad;

                fseek(fInventario, -1*(long)sizeof(Producto), SEEK_CUR);
                fwrite(&prod, sizeof(Producto), 1, fInventario);
                break;
            }
        }       
    }

    fclose(fInventario);
    fclose(fComandaDia);
    return 0;               //Retorna 0 si se actualizo correctamente el inventario, retorna 1 si hubo algun error
}

void actualizarTodoInventario(ComandaHistorica C[], int lenC){

    int i = 0;
    char fActual[11];

    while(i < lenC){
        
        strcpy(fActual, C[i].fecha);
        
        actualizarInventario(fActual);  //actualiza la fecha actual en el inventario

        while(i < lenC && !strcmp(fActual, C[i].fecha)){    //avanza hasta que cambie la fecha respecto la fecha actual

            i++;
        }        
    }
}
