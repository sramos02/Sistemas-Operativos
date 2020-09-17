#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_ESTUDIANTES 15 //rand()%100;
#define TIPOS_BOCADILLO 2

pthread_t estudiantes[NUM_ESTUDIANTES];
pthread_t cocinero;

pthread_mutex_t barra;
pthread_mutex_t cocina;

sem_t bocadillos[TIPOS_BOCADILLO];

//----------------------------------------------------------------------------

void conseguirBocadillo(int tipo){
    pthread_mutex_lock(&barra); //

    //Si no quedan bocadillos el estudiante espera a que se cocinen
    sem_wait(bocadillos[tipo]);
    printf("Un estudiante ha cogido bocadillo %d\n", tipo);
    pthread_mutex_unlock(&barra);
}

void comer(){
   sleep(1);
}

void servirBocadillos(){
    pthread_mutex_lock(&cocina); //Se utiliza para cocinar tandas de una en una
    pthread_mutex_lock(&barra);

    for(int i = 0; i < TIPOS_BOCADILLO; i++) {
        if (bocadillos[i] == 0) {
            printf("NO QUEDAN BOCADILLOS %d\n", i);
            for(int j = 0 ; j < rand() % 4; j++) sem_post(bocadillos[i]);   //Cocina un numero aleatorio de bocadillos
            printf("Num bocadillos[%d] es %d\n", i, bocadillos[i]);
        }
    }

    pthread_mutex_lock(&barra);
    pthread_mutex_unlock(&cocina);
}
//-----------------------------------------------------------------------------

void Estudiante(int tipoBocadillo){
    while(1) {
        conseguirBocadillo(tipoBocadillo);
        comer();
    }
}

void Cocinero(){
    while (1) {
        servirBocadillos();
    }
}


int main(){
    pthread_mutex_init(&cocina, NULL);
    pthread_mutex_init(&barra, NULL);
    sem_init(bocadillos[0], cocinero, 4);
    sem_init(bocadillos[1], cocinero, 2);

    int value;
    sem_getvalue(&bocadillos[0], &value);
    printf("%d\n", value);

    pthread_create(&cocinero, NULL, Cocinero, NULL);
    for(int i = 0 ; i < NUM_ESTUDIANTES; i++) pthread_create(&estudiantes[i], NULL, Estudiante, rand() % 2);
    pthread_join(cocinero, NULL);
    for(int i = 0 ; i < NUM_ESTUDIANTES; i++) pthread_join(estudiantes[i], NULL);

    pthread_mutex_destroy(&cocina);
    pthread_mutex_destroy(&barra);
    sem_destroy(bocadillos[0]);
    sem_destroy(bocadillos[1]);

    return 0;
}
