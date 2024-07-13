/*
    Creamos los semaforos
*/
#include <stdlib.h>
#include <stdio.h>
#include <sys/sem.h>
#include <sys/ipc.h>

#define BUFFER_SIZE 5

void main()
{
    /*
        Creamos 3 semaforos
        1_ Para empty slots
        2_ Para full slots
        3_ Mutual exclusion

        semget() -> Crea un set de semaforos
        Se puede visualizar en la consola usando el mando ipcs -s
    */
    int semid = semget(0x5, 3, IPC_CREAT | IPC_EXCL | 0600);

    if (semid == -1)
    {
        printf("No se pudo crear el set de semaforos.\n");
        exit(0);
    }

    // con SETVAL seteamos los valores de los semaforos
    semctl(semid, 0, SETVAL, BUFFER_SIZE); // semaforo 0, slots vacios
    semctl(semid, 1, SETVAL, 0);           // semaforo 1, slots llenos
    semctl(semid, 2, SETVAL, 1);           // semaforo 2 exclusion mutua

    exit(0);
}