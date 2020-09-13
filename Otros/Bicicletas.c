#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_CARRILES 2
#define NUM_CICLISTAS 15
#define MAX_CICLISTAS_CARRIL 2

pthread_t ciclistas[NUM_CICLISTAS];

pthread_mutex_t acceso;
pthread_cond_t carril_cerrado[NUM_CARRILES];

int num_ciclistas[NUM_CARRILES];
int id = 1;

void acceder_al_puente(int sentido){
	pthread_mutex_lock(&acceso);
	
	int contrario = (sentido + 1) % NUM_CARRILES; //0 o 1
	
	(sentido == 0)?printf("Nuevo ciclista (%d) carril derecho\n", id++):printf("Nuevo ciclista (%d) carril izquierdo\n", id++);
	
	//Si en el carril contrario hay mas del maximo no deja entrar al actual
	//Si en el carril contrario hay alguien, el actual no puede superar el maximo
	if((num_ciclistas[contrario] > MAX_CICLISTAS_CARRIL) || ((num_ciclistas[contrario] > 0) && (num_ciclistas[sentido] == MAX_CICLISTAS_CARRIL))){
		printf("Esperando para entrar...\n");
		while((num_ciclistas[contrario] > MAX_CICLISTAS_CARRIL) || ((num_ciclistas[contrario] > 0) && (num_ciclistas[sentido] == MAX_CICLISTAS_CARRIL))){
			pthread_cond_wait(&carril_cerrado[sentido], &acceso);	
		}	
	}

	(sentido == 0)?printf("Entra derecha. "):printf("Entra izquierda. "); 	
	num_ciclistas[sentido]++;
	printf("Transito izquierda: %d / Transito derecha: %d\n", num_ciclistas[1], num_ciclistas[0]);
	pthread_mutex_unlock(&acceso);
}

void salir_del_puente(int sentido){
	pthread_mutex_lock(&acceso);
	num_ciclistas[sentido]--;
	
	int contrario = (sentido + 1) % NUM_CARRILES; //0 o 1
	(sentido == 0)?printf("Sale derecha\n"):printf("Sale izquierda\n");

	//Sale del carril y abre las condiciones si es que toca
	if(num_ciclistas[sentido] == MAX_CICLISTAS_CARRIL)
		pthread_cond_signal(&carril_cerrado[contrario]);
	else if ((num_ciclistas[sentido] == 0) && (num_ciclistas[contrario] == MAX_CICLISTAS_CARRIL))
		pthread_cond_signal(&carril_cerrado[contrario]);		

	pthread_mutex_unlock(&acceso);
}

void ciclista(int sentido){
	acceder_al_puente(sentido);
	printf("-Circulando por el puente-\n");
	salir_del_puente(sentido);
}


int main(int argc, char*argv[]) {
	
	//Inicializa el programa
	pthread_mutex_init(&acceso, NULL);
	for(int i = 0; i < NUM_CARRILES; i++) pthread_cond_init(&carril_cerrado[i], &acceso);
	for(int i = 0; i < NUM_CARRILES; i++) num_ciclistas[i] = 0;
	
	//Crea los hilos
	for(int i = 0; i < NUM_CICLISTAS; i++){
		int r = rand() % NUM_CARRILES;
		pthread_create(&ciclistas[i], NULL, ciclista, r);
	}
	printf("!!!Todos los ciclistas han entrado con exito!!!\n");	
		
	//Esepera la ejecucion de todos los hilos
	for(int i = 0; i < NUM_CICLISTAS; i++) {
		pthread_join(ciclistas[i], NULL);
		printf("El ciclista %d ha salido con exito\n", i);	
	}
	printf("!!!Fin!!!\n");
	
	//Destruye las condiciones y mutex
	for(int i = 0; i < NUM_CARRILES; i++) pthread_cond_destroy(&carril_cerrado[i]);
	pthread_mutex_destroy(&acceso);

    return 0;
}
