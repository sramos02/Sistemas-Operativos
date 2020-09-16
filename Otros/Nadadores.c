//LA IMPLEMENTACIÓN ES INCORRECTA, SE TRATA DE UNA APROXIMACIÓN

#include <pthread.h>
#include <semaphore.h>

#include <stdio.h>
#include <unistd.h>

#define NUM_CARRILES 8
#define TAM_EQUIPOS 4
#define NUM_NADADORES (NUM_CARRILES * TAM_EQUIPOS)


pthread_t nadadores[NUM_NADADORES];     //Cada hilo es un nadador

pthread_mutex_t inicio;                             //Controla el inicio de la carrera
pthread_cond_t listos[NUM_NADADORES];               //Participantes listos para nadar
int participantes_listos = 0;                       //Controla que todos los participantes estén listos

int ronda_actual[NUM_CARRILES];                     //Controla el siguiente que debe salir en cada carril (0-8-16-24)
pthread_mutex_t carrera[NUM_NADADORES];             //Mutex de cada participante
pthread_cond_t esperando_carril[NUM_CARRILES];      //Detiene un carril hasta que un nadador notifica el final


void begin_race(int id_nadador) {
    pthread_mutex_lock(&carrera[id_nadador]);

    int carril = (id_nadador / TAM_EQUIPOS); //Redondea el carril (0-3)

    //Si al nadador no le toca salir entonces espera
    if (id_nadador != ronda_actual[carril]){
        printf("%d esperando su turno en carril %d\n", id_nadador, carril);
        while (id_nadador != ronda_actual[carril]) pthread_cond_wait(&esperando_carril[carril], &carrera[id_nadador]);
    }
    pthread_mutex_unlock(&carrera[id_nadador]);
}

void swim(int id_nadador){
    int carril = (id_nadador / TAM_EQUIPOS); //Redondea el carril (0-3)

    printf("%d Nadando...\n", id_nadador);
    sleep(1);
}

void notify_arrival(int id_nadador){
    pthread_mutex_lock(&carrera[id_nadador]);
    int carril = (id_nadador / TAM_EQUIPOS);        //Redondea el carril (0-3)

    pthread_cond_signal(&esperando_carril[carril]); //Libera al siguiente de la lista
    ronda_actual[carril] += NUM_CARRILES;           //Se mueve hasta el siguiente del mismo carril

    pthread_mutex_unlock(&carrera[id_nadador]);
}

void Nadador(int id_nadador){
    begin_race(id_nadador);
    swim(id_nadador);
    notify_arrival(id_nadador);
}

void thread_nadador(int id_nadador){
    pthread_mutex_lock(&inicio);
    participantes_listos++;

    //Cuando llega el ultimo despierta a todos
    if(participantes_listos == NUM_NADADORES){
        printf("LA CARRERA HA EMPEZADO\n");
        for(int i = 0; i < NUM_NADADORES; i++) pthread_cond_signal(&listos[i]);
    }

    while(participantes_listos != NUM_NADADORES) {
        pthread_cond_wait(&listos[id_nadador], &inicio);
    }

    Nadador(id_nadador);
    pthread_mutex_unlock(&inicio);
}

int main(){
    pthread_mutex_init(&inicio, NULL);
    for(int i = 0; i < NUM_NADADORES; i++) pthread_cond_init(&listos[i], NULL);
    for(int i = 0; i < NUM_CARRILES; i++) ronda_actual[i]= TAM_EQUIPOS * i;
    for(int i = 0; i < NUM_NADADORES; i++) pthread_mutex_init(&carrera[i], NULL);
    for(int i = 0; i < NUM_CARRILES; i++)  pthread_cond_init(&esperando_carril, &carrera[i]);


    for(int i = 0;i < NUM_NADADORES; i++) pthread_create(&nadadores[i], NULL, thread_nadador, i);
    for(int i = 0; i < NUM_NADADORES; i++) pthread_join(nadadores[i], NULL);

    pthread_mutex_destroy(&inicio);
    for(int i = 0; i < NUM_NADADORES; i++) pthread_cond_destroy(&listos[i]);
    for(int i = 0; i < NUM_NADADORES; i++) pthread_mutex_destroy(&carrera[i]);
    for(int i = 0; i < NUM_CARRILES; i++)  pthread_cond_destroy(&esperando_carril);

    return 0;
}
