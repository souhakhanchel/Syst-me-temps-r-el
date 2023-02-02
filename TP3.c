
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

#define NB_CAMIONS 10
#define NB_VOITURES 10
#define NB_VEHICULES (NB_CAMIONS + NB_VOITURES)

#define DEFAULT 0
#define WAIT 1
#define GO 2

pthread_mutex_t sc;

sem_t sem_array[NB_VEHICULES];

int state[NB_VEHICULES];

int nb_camions_bloques = 0;

int poids = 0;

void entrer(int poids_veh, int veh_id)
{
    pthread_mutex_lock(&sc);

    if (poids + poids_veh <= 15)
    {
        poids = poids + poids_veh;
        state[veh_id] = GO;
        sem_post(&sem_array[veh_id]); // <------------------
    }
    else
    {
        state[veh_id] = WAIT;
        if (poids_veh == 15)
            nb_camions_bloques++;
    }

    pthread_mutex_unlock(&sc);
    sem_wait(&sem_array[veh_id]); // <------------------
}

void quitter(int poids_veh, int veh_id)
{
    int i;

    pthread_mutex_lock(&sc);

    state[veh_id] = DEFAULT;
    poids = poids - poids_veh;

    for (i = 0; i < NB_CAMIONS; i++)
        if ((state[i] == WAIT) && (poids == 0))
        {
            sem_post(&sem_array[i]); // <------------------
            poids = 15;
            nb_camions_bloques--;
        }

    for (i = NB_CAMIONS; i < NB_VEHICULES; i++)
        if ((poids < 15) && (nb_camions_bloques == 0) && (state[i] == WAIT))
        {
            poids = poids + 5;
            sem_post(&sem_array[i]); // <------------------
        }

    pthread_mutex_unlock(&sc);
}

void *voiture(void *arg)
{
    int pid = *((int *)arg);

    // usleep(1000000);
    entrer(5, pid);
    printf("La voiture %d traverse le pont\n", pid);
    usleep(1000000);
    printf("La voiture %d quitte le pont\n", pid);
    quitter(5, pid);

    pthread_exit(NULL);
}

void *camion(void *arg)
{
    int pid = *((int *)arg);

    // usleep(1000000);
    entrer(15, pid);
    printf("Le camion %d traverse le pont\n", pid);
    usleep(1000000);
    printf("Le camion %d quitte le pont\n", pid);
    quitter(15, pid);

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int i;
    pthread_t id;

    for (i = 0; i < NB_VEHICULES; i++)
    {
        state[i] = DEFAULT;
        sem_init(&sem_array[i], 0, 0);
    }

    pthread_mutex_init(&sc, 0);

    for (i = 0; i < NB_VEHICULES; i++)
    {
        int *veh_id_adr = (int *)malloc(sizeof(int));
        *veh_id_adr = i;
        if (i < NB_CAMIONS)
            pthread_create(&id, NULL, camion, veh_id_adr);
        else
            pthread_create(&id, NULL, voiture, veh_id_adr);
    }

    pthread_exit(NULL);
};
