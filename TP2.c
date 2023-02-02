
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

#define NB_CAMIONS 5
#define NB_VOITURES 5
#define NB_VEHICULES (NB_CAMIONS + NB_VOITURES)

sem_t sem;

pthread_mutex_t mutex_camions;

void entrer(int poids_veh)
{

    if (poids_veh == 15)
    {
        pthread_mutex_lock(&mutex_camions);
        sem_wait(&sem);
        sem_wait(&sem);
        sem_wait(&sem);
        pthread_mutex_unlock(&mutex_camions);
    }
    else
        sem_wait(&sem);
}

void quitter(int poids_veh)
{
    while (poids_veh > 0)
    {
        sem_post(&sem);
        poids_veh = poids_veh - 5;
    };
}
void *voiture(void *arg)
{
    int pid = *((int *)arg);

    entrer(5);
    printf("Voiture %d : je traverse le pont\n", pid);
    usleep(1000000);

    printf("Voiture %d : je quitte le pont\n", pid);
    quitter(5);

    pthread_exit(NULL);
}

void *camion(void *arg)
{
    int pid = *((int *)arg);

    entrer(15);
    printf("Camion %d : je traverse le pont\n", pid);
    usleep(1000000);
    printf("Camion %d : je quitte le pont\n", pid);
    quitter(15);

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int i;
    pthread_t id;

    sem_init(&sem, 0, 3);

    for (i = 0; i < NB_VEHICULES; i++)
    {
        int *j = (int *)malloc(sizeof(int));
        *j = i;
        if (i < NB_CAMIONS)
            pthread_create(&id, NULL, camion, j);
        else
            pthread_create(&id, NULL, voiture, j);
    }

    pthread_exit(NULL);
};
