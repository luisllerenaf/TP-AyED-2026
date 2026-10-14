# TP Integrador — Buffet Albert Einstein

## Grupo Nro. 20
## Integrantes:
Luis Angel LLerena Flores - luisllerenaf

## Compilación y ejecución de los programas:

Para compilar cada uno de los módulos fuente (.cpp), se debe utilizar el compilador de C++ (g++) aplicando el nivel de optimización –02 y especificando el archivo de salida mediante la bandera -o. Para ello, se deben ejecutar los siguientes comandos en la terminal:

```bash
g++ -O2 -o normalizacion normalizacion.cpp
g++ -O2 -o ventas        ventas.cpp
g++ -O2 -o cierre        cierre.cpp
g++ -O2 -o resumen       resumen.cpp
```
Los programas se deben ejecutar respetando un orden especifico, donde cada uno procesa la información y deja los archivos necesarios para que el siguiente pueda operar.

El programa normalizacion se ejecuta una sola y única vez para la puesta en marcha del sistema. Luego, los programas de ventas, cierre y resumen se ejecutan periódicamente en función del flujo de trabajo y de los archivos que se van generando día a día y semana a semana. 

Se ejecutan desde la terminal de comandos anteponiendo ./ al nombre del archivo ejecutable:

```bash
./normalizacion     # Se ejecuta solo la primera vez
./ventas            
./cierre          
./resumen           
```

## Explicación de los programadas:
### normalizacion.cpp
Este programa se encarga de gestionar y ordenar lo que dejó el sistema anterior. Lee el archivo de comandas_historicas.dat y carga la información en un array, se ordena por fecha este array para poder utilizarlo para crear el archivo de mozos.dat, el corte de control que genera las comandas diarias y actualizar el inventario.

En la creación del archivo de mozos.dat se estableció un ID de Mozo, iniciando los ID en 1000, con lo cual queda ordenado por ID de mozo. También en este archivo se le asigna una clave a cada mozo que consta de 4 dígitos: dos letras y dos números. Para la encriptación de claves se utiliza un desplazamiento de 7 unicidades, o sea se suma 7 a cada digito.

Por último, crea un archivo que servirá para controlar la ultima fecha procesada que luego utilizara el programa ventas en su primera ejecución. Como se menciona anteriormente este programa se ejecuta una única vez al iniciar el sistema.

### ventas.cpp
Se encarga de generar las comandas diarias en archivos con formato comandas_dd-mm-aaaa. Para la obtención de información se usaron funciones que piden ingresar las entradas al usuario, las validan y procesan en caso de estar correctas. En cada entrada se puede salir escribiendo ‘00’.

La validación de cada mozo se hizo sumando un desplazamiento de 7 unidades a la clave ingresada por el usuario y se compara esta clave con el array de mozos. Este array se cargó previamente al leer el archivo de mozos.dat. Se uso una búsqueda binaria en el array de mozos porque está ordenados por ID.

El producto ingresado se busca mediante una búsqueda binaria directamente en el archivo inventario.dat, ya que este también esta ordenado por código de producto. 

En la etapa final se actualiza el stock en el inventario.dat, la comisión del mozo y se agrega la comanda en el archivo diario creado. El programa finaliza ordenando el archivo de comandas diarias y actualiza la última fecha procesada (para tener un control al procesar una nueva entrada).

### cierre.cpp
Se utiliza una modificación del patrón algorítmico apareo.
Se utiliza el archivo "estadoCierre.dat", que guarda la fecha desde la que se inicia el próximo cierre y el número de semana correspondiente (la numeración semanal va de 1 a 4).

Se crea un segundo archivo auxiliar "Nombre.dat", el cual guarda el nombre del archivo semanal creado para facilitar su lectura desde resumen.cpp. 

Para manejar las fechas, se implementaron funciones auxiliares que permiten generar el nombre de cada archivo, avanzar un día teniendo en cuenta la cantidad de días de cada mes y los años bisiestos, e identificar los domingos.

### resumen.cpp
Para el programa resumen.cpp se implementó un sistema que primero abre de forma dinámica el archivo semanal y luego realiza una validación para confirmar que exista y que no este vacío ese archivo. A continuación, mediante el algoritmo de corte de control, se recorre el archivo para agrupar y procesar la cantidad de ventas de cada mozo, su comisión total y al final el total general de productos vendidos del buffet. Todo el resumen de cierre se muestra impreso por pantalla para el cliente.
