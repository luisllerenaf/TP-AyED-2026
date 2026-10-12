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
const int MAX_COMANDAS = 1000;

void obtenerUltimaFecha(char f[]);
string fechaString(const char fecha[]);
bool validarFecha(char fecha[], char ultimaFecha[]);
int entradaDeFecha(char fechaActual[], char ultimaFecha[]);
int mozosArray(const char* nombre, Mozo arrayMozos[], int &lenM);
int busquedaBinariaMozo(Mozo vec[], int len, int valor);
int validarMozo(Mozo M[], int lenM, Mozo mozo);
int entradaMozo(Mozo arrayMozos[], int lenM, Mozo &mozoActual);
long busquedaBinariaProducto(const char* nombre, int clave, Producto& r);
int entradaProducto(const char* nombre, Producto &productoActual);
int entradaCantidadProducto(Producto productoActual);
bool actualizarStock(const char* nombre, long pos, const Producto &p);
bool actualizarComisionMozo(const char* nombre, int pos, const Mozo &m);
void nombreArchivoFecha(char nombre[], char fecha[]);
int agregarComanda(const char* nombreArchivo, Comanda comandaActual);
void ordenamientoBurbujaComandas(Comanda d[],int lenD);
int ordenarArchivoComandas(const char* nombreArchivo);
int actualizarFechaProcesada(const char* fechaActual);

int main() {

    char fechaActual[11];
    char ultimaFecha[11];
    char nombreArchivo[30];

    cout<<"=== SISTEMAS DE VENTAS - BUFFET ALBERT EINSTEIN ===\n"<<endl;

    //Carga de la fecha actual y validacion de la misma

    if(entradaDeFecha(fechaActual, ultimaFecha) == 1){  //Pido la fecha a procesar, la valido o salgo de programa

        return 1;   //Salgo del programa  
    }

    //Carga de los mozos del archivo "mozos.dat" en un array para poder comparar

    Mozo arrayMozos[100];
    int lenM = 0;

    if(mozosArray("mozos.dat", arrayMozos, lenM) == 1){
        
        return 1;   //Salgo del programa
    }

    //===== Carga de la Comanda diaria ===== 

    char seguir[10];

    do{
        cout<<"\n=== CARGA DE COMANDA ==="<<endl;

        //Ingreso del mozo por idMozo y clave

        Mozo mozoActual;
        int posMozoActual = entradaMozo(arrayMozos, lenM, mozoActual);  //Pido el mozo a procesar
        
        if(posMozoActual == -1){  //Valido la posicion del mozo en el archivo o salgo de programa

            return 1;   //Salgo del programa  
        }

        //pido el Id del producto de la comanda
        
        Producto productoActual;
        long posProducto = entradaProducto("inventario.dat", productoActual);
                
        if( posProducto == -1){

            return 1;   //Salgo del programa
        }

        //pido la cantidad del producto de la comanda

        int cantidad = entradaCantidadProducto(productoActual);

        if(cantidad == -1){

            return 1;   //Sale del programa
        }

        //actualizo el stock del productor actual

        productoActual.stockActual = productoActual.stockActual - cantidad; 

        //Actualizo el stock en el archivo inventario.dat

        if(!actualizarStock("inventario.dat", posProducto, productoActual)){ 
        
            cout<<"##Error, No se pudo actualizar el inventario"<<endl;

            return 1;
        }

        //Actualizar comision Total del mozo actual

        mozoActual.totalComision = mozoActual.totalComision + (cantidad * productoActual.precio) * TASA_COMISION; //comision total del mozo

        if(!actualizarComisionMozo("mozos.dat", posMozoActual, mozoActual)){ //Actualizo en el archivo mozos.dat

            cout<<"Error, No se pudo actualizar la comision del mozo"<<endl;
            return 1;
        }

        //Genero la comanda diaria

        Comanda comandaActual;

        comandaActual.idMozo = mozoActual.idMozo;
        comandaActual.codigoProducto = productoActual.codigo;
        comandaActual.cantidad = cantidad;
        comandaActual.comision = cantidad * productoActual.precio * TASA_COMISION;  //comision en la comanda actual

        //Creo o abro el archivo de comanda del dia actual

        nombreArchivoFecha(nombreArchivo,fechaActual); //array de char en formato del archivo comanda diaria (comandas_DD-MM-AAAA.dat)
    
        if(agregarComanda(nombreArchivo, comandaActual) == 1){
            
            return 1; //sale del programa si no puede agregar la comanda
        }   

        cout<<"\nPrecione Enter o cualquier tecla para continuar cargando comandas"<<endl;
        cout<<"Para salir ingrese '00'"<<endl;
        cin.getline(seguir, 10);

    }while(strcmp(seguir, "00"));   //Si escribe 00 termina la carga de las comandas recien cargadas

    //Ordeno las Comandas del archivo comandas_dd-mm-aaaa.dat recien creado

    if(ordenarArchivoComandas(nombreArchivo) == 1){

        return 1;   //Sale del programa
    }

    //Se actualiza la ultima fecha procesada recientemente

    if(actualizarFechaProcesada(fechaActual) == 1){

        return 1; //Sale si no pudo actualizar la ultima fecha procesada
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

int busquedaBinariaMozo(Mozo vec[], int len, int valor) { //Busca al mozo en el array de Mozos y devuelve la posicion del mozo buscado en el array Mozos
    
    int izquierda = 0; // base 0: primer índice válido
    int derecha = len - 1; // base 0: último índice válido

    while (izquierda <= derecha) {
        int medio = (izquierda + derecha) / 2;
 
        if (vec[medio].idMozo == valor) {
 
            return medio; // encontrado
 
        } else if (vec[medio].idMozo < valor) {
 
            izquierda = medio + 1; // buscar en la mitad derecha
 
        } else {
 
            derecha = medio - 1; // buscar en la mitad izquierda 
        } 
    } 
    return -1; // no encontrado
}

int validarMozo(Mozo M[], int lenM, Mozo mozo){ //Devuelve la posicion del mozo buscado en el arrayMozos o -1 si no lo encuentra

    int pos;

    pos = busquedaBinariaMozo(M, lenM, mozo.idMozo);

    if(pos != -1){
        char aux[20];
        int i = 0;

        while(mozo.password[i] != '\0' && i < 20 ){    //Sirve para cualquier tamaño de contraseña, hasta 19 caracteres

            aux[i] = mozo.password[i] + 7;
            i++;
        }        
        aux[i] = '\0';

        if(!strcmp(aux, M[pos].password)){

            return pos;
        }else{

            return -1;
        }
    }
    return -1;
}

int entradaMozo(Mozo arrayMozos[], int lenM, Mozo &mozoActual){
    
    int posMozoActual;
    char valido[10];

    do{ 
        cout<<"=== Ingrese datos del Mozo ==="<<endl;
        cout<<"Para salir ingrese el Id Mozo 00\n"<<endl;

        cout<<"Ingrese Id de Mozo: "<<endl;
        cin.getline(valido, 10);

        if(!strcmp(valido, "00")){

            return -1;       //Para salir del programa
        }

        cout<<"Ingrese contraseña: "<<endl;
        cin.getline(mozoActual.password, 20);           
            
        if(strlen(valido) == 4){               
                
            mozoActual.idMozo = atoi(valido);
            posMozoActual = validarMozo(arrayMozos, lenM, mozoActual);  //Valida por idMozo y contraseña            
            
            if(posMozoActual == -1){

                cout<<"##Error: Datos incorrectos, vuelva a escribir\n"<<endl;
            }
            else{

                mozoActual = arrayMozos[posMozoActual]; //Si encuentra al mozo copia su informacion en mozo actual
            }
        }
        else{
            cout<<"###Error: Datos incorrectos, vuelva a escribir\n"<<endl;
            posMozoActual = -1; //Para que vuelva a pedir los datos del mozo
        }

    }while(posMozoActual == -1);

    return posMozoActual;
}

long busquedaBinariaProducto(const char* nombre, int clave, Producto& r){ //Busca el producto en el archivo inventario.dat y lo carga en r
 
    FILE* f = fopen(nombre, "rb"); 
    if (f == NULL) return -1;
 
    fseek(f, 0, SEEK_END);  //al final del archivpara saber cuantos registros hay
 
    long n = ftell(f) / sizeof(Producto); // cantidad de registros

    long pri = 0, ult = n - 1, pos = -1;
 
    while (pri <= ult && pos == -1) {
 
        long med = (pri + ult) / 2;
 
        fseek(f, med * sizeof(Producto), SEEK_SET);
 
        fread(&r, sizeof(Producto), 1, f);
 
        if (r.codigo == clave) pos = med;
 
        else if (clave > r.codigo) pri = med + 1;
 
        else ult = med - 1; 
    } 
    fclose(f);
 
    return pos; //Devuelve la posicion del Producto buscado en el archivo (segun laa estructura Proudcto), sino devuelve -1
}

int entradaProducto(const char* nombre, Producto &productoActual){

    char valido[10];
    int id_productoActual;
    long posProducto;   //variable para guardar la posicion del producto en el archivo inventario.dat

    do{
        cout<<"\n=== Ingrese datos del Producto ==="<<endl;
        cout<<"(Para salir ingrese '00')\n"<<endl;
        cout<<"Ingrese codigo del Producto: ";        
        cin.getline(valido, 10);

        if(!strcmp(valido, "00")){

            return -1;       //retorna -1 Para salir del programa
        }

        else if(strlen(valido) == 3){
                
            id_productoActual = atoi(valido);
            posProducto = busquedaBinariaProducto(nombre, id_productoActual,productoActual); //busco el producto  y lo guardo en productoActual            
                
            if(posProducto == -1){

                cout<<"##Error:Codigo de producto invalido, vuelva a escribir"<<endl;
            }
        }
        else{
            cout<<"###Error: Codigo de producto invalido, vuelva a escribir"<<endl;
            posProducto = -1;
        }       

    }while(posProducto == -1);

    return posProducto; //Retorna la posicion del producto en el Inventario.dat
}

int entradaCantidadProducto(Producto productoActual){   //Gestiona que la cantidad ingresada sea mayor a 0 0 -1 si sale del programa

    int cantidad;
    char valido[10];    

    do{
        cout<< "\nProducto selecionado: "<<productoActual.descripcion<<endl;
        cout<<"Para salir ingrese 00"<<endl;
        cout<<"Ingrese la cantidad: ";

        cin.getline(valido, 10);

        if(!strcmp(valido, "00")){
            return -1;       //Para salir del programa
        }

        cantidad = atoi(valido);

        if(cantidad <= 0){

            cout<<"##Error: Cantidad invalida, vuelva a escribir"<<endl;
        }

    }while(cantidad <= 0);
    
    return cantidad;    //Retorna la cantidad ingresada mayor a cero
}

bool actualizarStock(const char* nombre, long pos, const Producto &p){ //Actualiza el stock escribiendo en la posicion pos del archivo

    FILE* f = fopen(nombre, "rb+");
    if(f == NULL){

        cout<<"Error al abrir archivo: "<<nombre<<endl;
        return false;
    }

    fseek(f, pos * sizeof(Producto), SEEK_SET);
    fwrite(&p, sizeof(Producto), 1, f);

    fclose(f);

    return true;
}

bool actualizarComisionMozo(const char* nombre, int pos, const Mozo &m){

    FILE* f = fopen(nombre, "rb+");
    if(f == NULL){
        
        cout<<"Error al abrir archivo: "<<nombre<<endl;
        return false;
    }

    fseek(f, pos * sizeof(Mozo), SEEK_SET);
    fwrite(&m, sizeof(Mozo), 1, f);

    fclose(f);
    return true;
}

void nombreArchivoFecha(char nombre[], char fecha[]){//genera el nombre del archivo en formato comandas_dd-mm-aaaa.dat
    
    strcpy(nombre, "comandas_");
    strcat(nombre, fecha);
    strcat(nombre, ".dat");
}

int agregarComanda(const char* nombreArchivo, Comanda comandaActual){

    FILE* fComanda = fopen(nombreArchivo, "ab"); //ab para que escriba al final si existe, sino lo crea
    if(fComanda == NULL){

        cout<<"#Error: no se pudo abrir el archivo: "<<nombreArchivo<<endl;
        return 1;
    }
    fwrite(&comandaActual, sizeof(Comanda), 1, fComanda); // escribo al final si existe
    fclose(fComanda);

    return 0;   //Retorna 0 si se pudo crear o abrir el archivo y modificarlo
}

void ordenamientoBurbujaComandas(Comanda d[],int lenD){ //Ordena el array de comanas[] por el idMozo

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

int ordenarArchivoComandas(const char* nombreArchivo){

    Comanda d[MAX_COMANDAS];
    int lenD = 0;

    FILE* fComanda = fopen(nombreArchivo, "rb+");
    if(fComanda == NULL){

        cout<<"#Error: no se pudo abrir el archivo: "<<nombreArchivo<<endl;
        return 1;
    }
    
    while(fread(&d[lenD], sizeof(Comanda), 1, fComanda) == 1){

        lenD++;
    }

    ordenamientoBurbujaComandas(d, lenD); //Ordeno el array de Comanda Totales
    fseek(fComanda, 0, SEEK_SET);   //Vuelvo al inicio del archivo para reescribirlo

    for(int i = 0; i < lenD; i++){

        fwrite(&d[i], sizeof(Comanda), 1, fComanda);
    }
    fclose(fComanda);

    return 0;   //Retorna 0 si pudo ordenar
}

int actualizarFechaProcesada(const char* fechaActual){

    FILE* fControl = fopen("ultima_fecha_procesada.dat","wb");
    if(fControl == NULL){

        cout<<"#Error: no se pudo crear el archivo ultima_fecha_procesada.dat"<<endl;
        return 1;
    }

    fwrite(fechaActual, 11*sizeof(char), 1, fControl);
    fclose(fControl);

    return 0;
}