int dato = 5;   /*recurso*/
int n_lect= 0;  /*numero de lectores*/

pthread_mutex_t escritores;  /*controlar el acceso a dato*/
pthread_mutex_t lectores; /*controla la variable n_lect*/

void enter_lector() {
  pthread_mutex_lock(&lectores);
  n_lect ++;
  if(n_lect == 1) pthread_mutex_lock(&escritores);  
  pthread_mutex_unlock(&lectores);
}

void exit_lector(){
  pthread_mutex_lock(&lectores);
  n_lect--;
  if(n_lect == 0) pthread_mutex_unlock(&escritores);  
  pthread_mutex_unlock(&lectores);
}

void Lector(void){
  while(1){
    ReaderEnter();
    printf("%d\n", dato);
    ReaderExit();
    sleep(1);
  }
}

void Escritor(void) {
  while(1){
    pthread_mutex_lock(&escritores);  
    dato += 2;
    pthread_mutex_unlock(&escritores);  
  }
}


main(int argc, char *argv[])  {
  pthread_t th1, th2, th3, th4;
  
  pthread_mutex_init(&mutex,  NULL);
  pthread_mutex_init(&m_lect,  NULL);
  
  pthread_create(&th1, NULL, Lector, NULL);
  pthread_create(&th2, NULL, Escritor, NULL);
  pthread_create(&th3, NULL, Lector, NULL);
  pthread_create(&th4, NULL, Escritor, NULL);
  pthread_join(th1, NULL);    
  pthread_join(th2, NULL);
  pthread_join(th3, NULL);    
  pthread_join(th4, NULL);
  
  pthread_mutex_destroy(&mutex);
  pthread_mutex_destroy(&m_lect);exit(0);
}

