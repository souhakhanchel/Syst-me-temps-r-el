#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

struct arg_struct
{
    char *metro_name;
    char *ligne;
};

pthread_mutex_t mutex_ab;
pthread_mutex_t mutex_bc;
pthread_mutex_t mutex_cd;
pthread_mutex_t mutex_ec;
pthread_mutex_t mutex_cb;
pthread_mutex_t mutex_fb;
pthread_mutex_t mutex_ba;

void *metro(void *arguments)
{
    struct arg_struct *args = arguments;
    char *ligne = args->ligne;
    char *nom = args->metro_name;
    if (strcmp(ligne, "MR1") == 0)
    {
        pthread_mutex_lock(&mutex_ab);
        printf("Le métro %s traverse AB\n", nom);
        usleep(6000000);
        pthread_mutex_unlock(&mutex_ab);

        pthread_mutex_lock(&mutex_bc);
        printf("Le métro %s traverse BC\n", nom);
        usleep(6000000);
        pthread_mutex_unlock(&mutex_bc);

        pthread_mutex_lock(&mutex_cd);
        printf("Le métro %s traverse CD\n", nom);
        usleep(6000000);
        pthread_mutex_unlock(&mutex_cd);
    }
    else if (strcmp(ligne, "MR3") == 0)
    {
        pthread_mutex_lock(&mutex_ec);
        printf("Le métro %s traverse EC\n", nom);
        usleep(6000000);
        pthread_mutex_unlock(&mutex_ec);

        pthread_mutex_lock(&mutex_cb);
        printf("Le métro %s traverse CB\n", nom);
        usleep(6000000);
        pthread_mutex_unlock(&mutex_cb);
    }
    else if (strcmp(ligne, "MR2") == 0)
    {
        pthread_mutex_lock(&mutex_fb);
        printf("Le métro %s traverse FB\n", nom);
        usleep(6000000);
        pthread_mutex_unlock(&mutex_fb);

        pthread_mutex_lock(&mutex_ba);
        printf("Le métro %s traverse BA\n", nom);
        usleep(6000000);
        pthread_mutex_unlock(&mutex_ba);
    }
}

int main()
{
    pthread_mutex_init(&mutex_ab, NULL);
    pthread_mutex_init(&mutex_bc, NULL);
    pthread_mutex_init(&mutex_cd, NULL);
    pthread_mutex_init(&mutex_ec, NULL);
    pthread_mutex_init(&mutex_cb, NULL);
    pthread_mutex_init(&mutex_fb, NULL);
    pthread_mutex_init(&mutex_ba, NULL);

    // MR1("metroABCD");
    // MR2("metroFA");
    // MR3("metroEB");
    pthread_t th0, th1, th2, th3, th4, th5;
    struct arg_struct args0, args1, args2, args3, args4, args5;
    args0.metro_name = "metroFBA1";
    args0.ligne = "MR2";
    args1.metro_name = "metroECB1";
    args1.ligne = "MR3";
    args4.metro_name = "metroFBA2";
    args4.ligne = "MR2";
    args5.metro_name = "metroECB2";
    args5.ligne = "MR3";
    args2.metro_name = "metroABCD1";
    args2.ligne = "MR1";
    args3.metro_name = "metroABCD2";
    args3.ligne = "MR1";
    pthread_create(&th0, NULL, metro, (void *)&args0);
    pthread_create(&th1, NULL, metro, (void *)&args1);
    pthread_create(&th2, NULL, metro, (void *)&args2);
    pthread_create(&th3, NULL, metro, (void *)&args3);
    pthread_create(&th4, NULL, metro, (void *)&args4);
    pthread_create(&th5, NULL, metro, (void *)&args5);

    pthread_join(th0, NULL);
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(th3, NULL);
    pthread_join(th4, NULL);
    pthread_join(th5, NULL);

    return 0;
}