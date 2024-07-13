#include <sys/sem.h>
#include <semaphore.h>

/*
    semop() performs operations on selected semaphores in the set indicated by semid.
*/

// wait
int P(int semid, int semnum)
{
    struct sembuf buf;
    buf.sem_num = semnum;  // semaphore number
    buf.sem_op = -1;       // semaphore operation
    buf.sem_flg = 0;       // operation flags
    semop(semid, &buf, 1); // id del set de semaforos, bufer asociado, nro de operaciones
}

// signal
int V(int semid, int semnum)
{
    struct sembuf buf;
    buf.sem_num = semnum;  // semaphore number
    buf.sem_op = 1;        // semaphore operation
    buf.sem_flg = 0;       // operation flags
    semop(semid, &buf, 1); // id del set de semaforos, bufer asociado, nro de operaciones
}