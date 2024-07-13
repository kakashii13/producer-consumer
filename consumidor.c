#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int P(int semid, int semnum);
int V(int semid, int semnum);

void main()
{
    // obtenemos los id de las secciones de memoria compartida
    int shmidbufer = shmget(0x3, 0, 0);
    int shmidcontador = shmget(0x4, 0, 0);

    printf("El id del buffer es: %d\n", shmidbufer);
    printf("El id del contador es: %d\n", shmidcontador);

    // obtenemos el id del set de semaforos
    int semid = semget(0x5, 0, 0);

    // validamos que haya un semid con esa key
    if (semid == -1)
    {
        printf("No se pudo obtener el set con esa key.\n");
    }

    printf("SemID %d\n", semid);

    printf("**********************************\n");
    printf("Semaforo sem_empty: %d\n", semctl(semid, 0, GETVAL)); // sem_empty
    printf("Semaforo sem_full: %d\n", semctl(semid, 1, GETVAL));  // sem_full
    printf("Semaforo exclusion: %d\n", semctl(semid, 2, GETVAL)); // exclusion
    printf("**********************************\n");

    /*
        Esta direccion de memoria es las misma que la declarada en la creacion del shared memory (shm.c) porque apuntan a la misma direccion de memoria
    */
    char *buffer = (char *)shmat(shmidbufer, 0, 0);

    // variable para indicar en que posicion del buffer vamos leer
    int indice = 0;

    while (1)
    {
        /*
            P(semid, 1) -> decrementamos el semafaro de los slots llenos
            Inicialmente arranca en 0

            Inteta decrementar los slots que se van completando por el productor, hasta que llegue a 0
            Una vez llegue a 0, queda bloqueado hasta que el productor produzca otro valor
        */

        P(semid, 1);
        P(semid, 2); // semaforo exclusion mutua

        /*
            Seccion critica
            Lee las posiciones del buffer
        */
        sleep(1);

        printf("Buffer posicion: %d, lee: %d\n", indice, buffer[indice]);

        /*
            indice = (indice + 1) % BUFFER_SIZE hace circular al indice

            indice = (0 + 1) % 5 = 1
            indice = (1 + 1) % 5 = 2
            indice = (2 + 1) % 5 = 3
            indice = (3 + 1) % 5 = 4
            indice = (4 + 1) % 5 = 0
        */
        indice = (indice + 1) % BUFFER_SIZE;

        // fin seccion critica

        V(semid, 2); // semaforo exclusion mutua
        /*
            V(semid, 0) -> incrementamos los slots disponibles del sem_empty
        */
        V(semid, 0);
    }
}