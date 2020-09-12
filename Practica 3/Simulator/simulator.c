#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define N_PARADAS  5        // número de paradas de la ruta

#define EN_RUTA    0        // autobús en ruta
#define EN_PARADA  1        // autobús en la parada

#define MAX_USUARIOS 40     // capacidad del autobús
#define USUARIOS   4        // numero de usuarios

// estado inicial
int estado = EN_RUTA;
int parada_actual = 0;     // parada en la que se encuentra el autobus
int n_ocupantes   = 0;     // ocupantes que tiene el autobús

int esperando_parada[N_PARADAS]; // personas que desean subir en cada parada
int esperando_bajar[N_PARADAS];  // personas que desean bajar en cada parada

pthread_mutex_t mutex;              //Controla el movimiento del autobus (entre estaciones, para que no se vaya sin alguien)
pthread_cond_t subir[N_PARADAS];    //Controla el acceso de pasajeros
pthread_cond_t bajar[N_PARADAS];    //Controla el acceso de pasajeros
pthread_cond_t bus;                 //Controla el paso de personas dentro del bus (espera a que todos salgan o entren)

//--------------------------------------------------------------------------------------------


void Subir_Autobus(int id_usuario, int origen){
    pthread_mutex_lock(&mutex);
    esperando_parada[origen]++;  
    pthread_cond_wait(&subir[origen], &mutex); //Espera en la parada
    while(n_ocupantes == MAX_USUARIOS) pthread_cond_wait(&subir[origen], &mutex); //Para entrar tiene que caber en el autobus
    
    esperando_parada[origen]--;
    n_ocupantes++;
    printf("El usuario %d se ha subido al autobus", id_usuario);
    pthread_mutex_unlock(&mutex);
}

void Bajar_Autobus(int id_usuario, int destino){
    pthread_mutex_lock(&mutex);
    esperando_bajar[destino]++;
    
    pthread_cond_wait(&bajar[destino], &mutex);
    esperando_bajar[destino]--;
    n_ocupantes--;
    
    printf("El usuario %d se ha bajado del autobus", id_usuario);
    pthread_mutex_unlock(&mutex);
}


void Autobus_En_Parada(){
    pthread_mutex_lock(&mutex);
    estado = EN_PARADA;

    //Bajan los que quieran
    pthread_cond_broadcast(&bajar[parada_actual]);
    while(esperando_bajar[parada_actual] != 0) pthread_cond_wait(&bus, &mutex);
    
    //Suben los que quieran
    pthread_cond_broadcast(&subir[parada_actual]);
    while(esperando_parada[parada_actual] != 0) pthread_cond_wait(&bus, &mutex);

    pthread_mutex_unlock(&mutex);
}

void Conducir_Hasta_Siguiente_Parada(){
    pthread_mutex_lock(&mutex);
    
    sleep(1); //En ruta
    estado = EN_RUTA;
    parada_actual = (parada_actual + 1) % N_PARADAS; //Actualiza la parada
    
    pthread_mutex_unlock(&mutex);
}

// Definiciones globales (comunicación y sincronización)
void* thread_autobus(void* args) {
    while (parada_actual < N_PARADAS){
        Autobus_En_Parada();                // esperar a que los viajeros suban y bajen
        Conducir_Hasta_Siguiente_Parada();  // conducir hasta siguiente parada
    }
}


//--------------------------------------------------------------------------------------------

void Usuario(int id_usuario, int origen, int destino) {
    Subir_Autobus(id_usuario, origen);  // Esperar a que el autobus esté en parada origen para subir
    Bajar_Autobus(id_usuario, destino); // Bajarme en estación destino
}



void* thread_usuario(void* arg) {
    int id_usuario = arg;
    int b = 0;
    
    while(1){
        int a = rand()%N_PARADAS;
        
        do{ b = rand() % N_PARADAS; //Elige donde se baja
        }while(a==b);
        
        Usuario(id_usuario,a,b);
    }
}

//--------------------------------------------------------------------------------------------

int main(int argc, char*argv[]) {

    if(argc != 3) prinf("Usage: ./simulator <bus_capacity> <user_count> <number_of_stops>\n");
    else{
        thread_autobus(); // Crear el thread Autobus
        for (int i = 0; i < USUARIOS; i++) thread_usuario(i);
        pthread_join(thread_autobus, NULL); // Esperar terminación de los hilos
    }
    return 0;
}
