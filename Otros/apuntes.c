
------------------------------------ SISTEMA DE ARCHIVOS -------------------------------------

/*
Vamos a estudiar las tablas FAT (File allocation table). Se trata de una tabla de bloques que apuntan a otros bloques 
o a un fín de fichero. Si un archivo no cabe en la FAT, la instrucción dará error y no se permitirá.

Para calcular el numero de bloques que requiere un archivo basta con lo siguiente: TamTotal/TamBloque.
Las tablas FAT tienen una memoria máxima y no se pueden redimensionar. El resto del sistema oeprativo
no se verá modificado por las operaciones de esta. 

Si se quiere acceder a un dato que está a mitad de camino de un archivo será necesario acceder al disco
N veces para consultar la tabla hasta llegar al bloque requerido. 

En ella NO se admiten enlaces fuerte (hard link) ya que estas tablas solo pueden contener un descriptor 
de fichero a la vez. Con un hard link tendríamos dos descriptores diferentes y eso no es posible. 
que 

En cuanto a su contrapunto en UNIX, las tablas de iNodos, lo único que tenemos en cuanto a teoría es la 
diferencia entre enlaces rigidos y enlaces simbólicos. 

Un enlace rígido (hard link) se trata de varios caminos para llegar a un mismo fichero.
Esto quiere decir que si uno se borra, el archivo puede acogerse a otros y no se elimina hasta que todos sus
enlaces hayan sido eliminados. En la practica no se generan iNodos, simplemente se aumentan las referencias.

Por otro lado, el softLink es una copia que apunta a la original, de esta forma, si la original es eliminada,
este archivo queda corrupto ya que "no apunta a nada". En la practica se generan iNodos diferentes, funciona
muy parecido a los accesos directos.
*/ 

//Los comandos que veremos en los ejercicios serán los siguientes:

creat(): Crea un archivo y devuelve un descriptor
unlink(): Borra un archivo 
open(): Abre un archivo y devuelve un descriptor
close(): Cierra un archivo
read(): Lee los datos de un archvio aboerto usando su descriptor
write(): Escribe los datos a un fichero usando su descriptor
stat(): Devuelve los atributos de un fichero

//Y un ultimo comando que miraremos con un poco más de detalle.

lseek(FICHERO, OFFSET, DESDE)
	//lseek: mueve el puntero de un archivo
	//DESDE SEEK_SET(principio), SEEK_CUR(actual), SEEK_END(final)
	
	//El puntero NO puede salirse nunca del espacio que tiene definido, con SEEK_END el offset
	//debe ser 0. Si que está permitido llevar el puntero al final y posteriormente escribir

	//Cuando hacemos un lseek y despues escribimos algo en un fichero, este fichero en UNIX ocupará la cantidad 
	//total de bloques con 


/*
En tablas FAT cuando se reserva un nuevo bloque para un fichero tienen que estar asignados todos los bloques anteriores 
de ese fichero ya que en la FAT la forma de seguir la pista a los bloques que forman parte de un fichero es una lista 
enlazada, por lo que un lseek + write a un byte disperso en FAT implica reservar todos los bloques necesarios, mientras 
que en los tipo unix (iNodos) no es necesario prereservar esos bloques intermedios.
*/

/*
El tamaño del mapa de bits de unix para bloques libres es solo función del número de bloques que contenga ese sistema de ficheros. 
*/

------------------------------------ GESTION DE PROCESOS ------------------------------------

/*
Una CPU siempre está ejecutando procesos o hilos (threads), puede ser a la vez o uno tras otro, depende de los 
procesadores de los que esté capacitado. Si un procesador no tiene ninguna instrucción que ejecutar enconces 
ejecutará el proceso nulo. Esto quiere decir que nunca se para.

Aquí lo más importante es entender los ejercicios y la diferencia entre los tipos de sistemas
multitarea.
*/

//Los comandos que veremos en los ejercicios son los siguientes:

fork(): Crea un proceso nuevo en estado listo que tiene un solo hilo, con distinto PID. Se crea una imagen identica
de memoria. Esto implica que se copian las tablas TDDA, se comparten la entrada TFA y con ello los punteros L/E.

exec(): Reemplaza el mapa de memoria de un proceso por otro. Esto quiere decir que al ejecutar un exec() todo el
programa que contiene el proceso es borrado y se sustituye por otro PERO todos los identificadores de archivos abiertos
se mantiene intacta despues del exec(), lo que se modifica es la imagen de memoria (el codigo).

execlp(): Se utiliza para que el hilo en cuestión cargue otro programa en ejecución y que por lo tanto no termine
ejecutando las siguientes lineas del primer programa. Si no se ejecuta la llamada (fallo) el programa anterior sigue. 

wait(): Bloquea un proceso y se queda a la espera. Si se especifica un PID espera a que el proceso termine. Si se escribe 
wait(NULL) el proceso espera a que terminen TODOS sus hijos para seguir ejecutandose.

exit(): finaliza el proceso, informa al padre del estatus  y se elimina su imagen de memoria. Se deja activo el BCP hasta
que el padre ejecute un wait() que lo recolecte. 


//Un hijo SIEMPRE debe terminar (return | exit) si no lo hace el padre nunca continúa. Se queda zombie
//

//Tablas de archivos
Existen 3 tablas diferentes que mantiene UNIX para controlar los accesos, permisos y otros asuntos.

- TDDA (una por cada proceso)
/*
	Tabla de descriptores de archivos abiertos. Cada tabla pertenece a un proceso. Contiene información acerca del FD que es 
	el numero por el que el proceso se comunica con un archivo, es un identificador local de archivo que debe estar abierto. 
*/
TFA (global)
TIN (global)

//Para gestionar los hilos dentro de un proceso disponemos de un conjunto de sentencias definidas en la librería pthread.

/*
Es importante saber si los hijos acaban su proceso y si el padre espera a los hijos. La memoria no es compartida, al hacer fork()
se hace un duplicado de la memoria pero cada hilo es independiente.
*/

--------------------------------------- PLANIFICACION ---------------------------------------
/*
Un algoritmo expropiativo puede expulsar a un proceso de la CPU una vez se le ha cedido acceso. Los tipos de algoritmos
expropiativos son:


RR (Turno rotativo)
SRTF (Shortest remaining time first)
Colas multinivel

Y los no expropiativos:

FCFS (First Come First Served)
SJF (Shortest job first)

*/

--------------------------------------- SINCRONIZACION --------------------------------------

- CERROJOS Y CONDICIONES (MUTEX)

/*
Las variables condición suelen compartir en un mismo cerrojo que protege una determinada sección crítica. Cuando varias variables 
condición se configuran con el mismo cerrojo se suele llamar MONITOR.
*/

wait(): bloquea
signal(): desbloquea

//Utilizan la siguiente nomenclatura
lock(mutex);
while(condicion != FALSE) wait(condicion, mutex);
<seccion critica>
unlock(mutex);


- SEMAFOROS


/*
Los semáforos son estructuras que permiten el paso un número determinado de veces y a partir de ahí bloquean la entrada hasta que 
los que hayan entrado terminen y salgan. Si el semaforo se inicializa a 1 funcinará como un mutex y se llama binario. 
*/

/*
Existe una diferencia muy importante entre un cond_signal() y un sem_post(), el primero
despierta un hilo de la cola de la variable condición que esté en la cola, pero si no hay
nadie bloqueado en la cola el cond_signal() se pierde. Sin embargo el sem_post() aunque
no despierte a nadie deja el contador del semáforo aumentado y así podrá pasar otro hilo
en el futuro.
*/


--------------------------------------- ENTRADA SALIDA ---------------------------------------
/*
Los dispositivos de E/S se dividen en dos clases en funcion de la capacidad de acceso, si se pueden acceder a nivel de byte
o a nivel de bloque:

- Dispositivos de caracter (teclaro, ratón...)
- Dispositivos de bloque (discos, red...)

Toda la información referente a dispositivos se almacena en /dev. Estos arvhicos se gestionan con open(), close(), write(), read()...
Tambien existen funciones especiales de control. 

*/

Device drivers de caracteres.

/*
Un driver de dispositivos es un tipo concreto de moddo de kernel, su estructura está compuesta por las funcione sde carga y descarga
así como una estructura de operaciones. Dentro de init_module y cleanup_module se llama a otras funciones del proceso. 
*/

/*
Las funciones descritas a continuación se utilizan para a reserva y liberación de pares (major, minor). cuando un modulo se carga
se le tiene que asignar pares con los que pueda trabajar. De las funciones de asignación mostradas a continuación, solo se puede usuar 
una dependiendo de si se sabe el major o no.
*/

alloc_chrdev_region(FIRSTPAR(ret), FIRSTMINOR, COUNT, NAME);
//Reserva un rango (major, minor) consecutivo con major desconocido. Retorna 0, -1 si error
register_chrdev_region(FIRSTPAR(ret), COUNT, NAME);
//Reserva un rango (major, minor) consecurivo con major desconocido. Retorna 0, -1 si error


unregister_chrdev_region(FIRSTPAR, COUNT)
//Libera un rango de pares (major, minor) del driver. Devuelve 0, -1 si error.

cdev_alloc(): crea una estructura cdev y retorna un puntero no nulo a la misma en caso de exito. 
cdev_init(ESTRUCTURA, DRIVER): Asocia la interfaz del driver a la estructura cdev
cdev_add(ESTRUCTURA, FIRSTPAR, COUNT): Añade pares a la estructura desde first hasta count;
cdev_del(ESTRUCTURA): Elimina la estructura cdev y libera memoria 

//Vamos a ver ahora la implementación de algunos modulos

//Esta funcion puede ser llamada desde consola por el comando insmod (carga un modulo)
int init_module(){
	int major, minor;

	//Reserva memoria
	if(alloc_chrdev_region(&start, 0, 1, DEVICE_NAME)){
		prink(KERN_INFO "Can't allocate chrdev region");
		return -ENOMEM;
	}

	//Asocia la memoria
	chardev = cdev_alloc();
	if(chardev == NULL){
		prink(KERN_INFO "cdev_alloc() failed");
		return -ENOMEM;
	}

	//Inicializa
	major = MAJOR(start);
	minor = MINOR(start);

	return SUCSESS;
}

//Esta funcion puede ser llamada desde consola con el comando rmmod
void cleanup_module(){
	//Elimina el modulo
	if(chardev) cdev_del(chardev);
	//Libera la memoria
	unregister_chrdev_region(start, 1);
}


//BUFFER ES DONDE SE METEN LOS DATOS

static ssize_t device_write(struct file *file , const char buff , size_t len , loff_t off){
	printk (KERN_ALERT "Loquesea");
	copy_from_user(buff, msg_ptr, len);
	return len;
}

static int device_open(struct inode *inode, struct file *file){
	static int counter = 0;
	
	if(device_open) return -EBUSY;
	device_open++;
	sprintf(msg, "I already run this module %d times", counter++);
	try_module_get(THIS_MODULE);
	return SUCSESS;
}

static int device_release(struct inode *inode, struct file *file){
	device_open--;
	module_put(THIS_MODULE);
	return 0;
}

static ssize_t device_read(struct file *file, char *buffer, size_t len, loff_t *offset){
	copy_to_user(buff, msg_ptr, len);
	msg_ptr += length;
	return len;
}

/*
Las funciones de dispositivo open y release tienen dos llamadas especiales  funciones que sirven para
controlar la cantidad de veces que el modulo está siendo utilizado por diferentes usuarios.

try_module_get(THIS_MODULE): Incrementa el contador interno.
module_put(THIS_MODULE): Decrementa el contador interno. 

El kernel lleva un contador interno con el numero de archivos abiertos con el driver. Si un driver
tiene este contador distinto de cero no puede ser eliminado. El comando rmmod daría fallo. Si 
queremos saber por quein está usado, se puede ver listando lsmod en una de sus columnas. 

La variable Device-open se usa para saber si el dispositivo de E/S está en uso (no el driver)
*/

//-----------------Cosos de examen y de teoría que miré anteriormente---------------------

/*
1) El SO terminará procesos que accedan ilegalmente a la memoria para prevenir cualquier tipo de error
2) El Major Number define el tipo de dispositivo y el Mino Number diferencia dispositivos
3) El modulo kernel engloba cualquier tipo de comando posible
*/

/*
1) alloc_chrdev_region(OUTPUT, FIRST MINOR, COUNT, DEVICE,NAME): Reserva un rango de pares (major, minor) para el driver
2) cdev_alloc(): crea una estructura cdev y si la operación es completada devuelve un puntero a dicha estructura
3) cdev_init(ESTRUCTURE, OPERATION POINTER): asocia la interfaz del driver con la estructura creada cdev
4) cdev_add(ESTRUCTURE, (MAJOR MINOR), COUNT): asocia un numero de pares (major, minor) al cdev

5) try_module_get(MODULE): se encarga de proteger el archivo ante la eliminación. Si falla el sistema obviará el modulo y si no falla entonces actuará de forma segura.
6) module_put(MODULE): se encarga de cargar el modulo una vez introducido de forma segura (no lo prueba, lo mete directamente)

7) printk(MESSAGE): Muestra en el log
*/

/*
Los dispositivos de E/S se dividen en dos clases en funcion de la capacidad de acceso, si se pueden acceder a nivel de byte
o a nivel de bloque:

- Dispositivos de caracter (teclaro, ratón...)
- Dispositivos de bloque (discos, red...)

Toda la información referente a dispositivos se almacena en /dev. Estos arvhicos se gestionan con open(), close(), write(), read()...
Tambien existen funciones especiales de control. 
*/

/*
Los dispositivos se agrupan en clases. Cada clase tiene un número de dispositivo principal MAJOR que
la identifica. Dentro de cada clase, los dispositivos se distinguen mediante el MINOR (ID local). En 
linux el par (major, minor) queda representado mediante el tipo dev_t
*/

------------------------------------- GESTION DE MEMORIA ------------------------------------

/*
en esta parte del temario vamos a explicar comandos basicos de Linix
*/

ls FILE 
//Lista el contenido dentro de file

cat ORIGEN > DESTINO 
//Escribe en destino el contenido origen (Pueden ser otros ficheros)

cp ORIGEN DESTINO 
//Reemplaza el fichero destino con el de origen. Si no existe el destino entonces se crea

mv ORIGEN DESTINO 
//Mueve de directorio el origen

rm ORIGEN 
//Borra el archivo origen

ln (-s) ORIGEN DESTINO
//ln: añade un nuevo enlace al origen, si uno de ellos es eliminado, el fichero sigue existiendo (SE SUMA UN NUEVO PUNTERO)
//ln -s: hace un duplicado de la memoria de un fichero (no puede reemplazar)




Creación de arvhivo de dispositivo.
//Podemos crear archivos especiales de dispositivo de la siguente manera
mknod /dev/<nombre> <tipo c/b> <major> <minor>
Ejemplo: mknod /dev/disp1 c 10 0

//Para cambiar los permisos
chmod <permisos> /dev/<nombre> 

0 -> nada
2 -> Escritura
4 -> Lectura
6 -> Lectura/escritura

//Para eliminar un nodo
rmmod disp

//Cargar un modulo
insmod disp 

//ver la informacion de un modulo
stat disp 

//El comando stat nos da toda la información necesaria sobre un dispositivo
stat /dev/disp1 //file, size, device, access, modify, change















//Free(COSO) libera memoria 
//Malloc(COSO) reserva memoria
//Fork() Duplica memoria
///Memcopy() Copia N bytes 

