#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int P(int semid, int semnum);
int V(int semid, int semnum);

int main()
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

    // variable para indicar en que posicion del buffer vamos escribir
    int indice = 0;
    int nro_a_escribir = 5;

    while (1)
    {
        /*

        P(semid, 0) -> decrementamos el semaforo sem_empty que es el que tiene la cantidad de slots disponibles

        Inicialmente arranca con el tamano del buffer_size(5) y a cada iteracion se decrementa en 1, hasta llegar a 0
        Una vez que llega a 0, el semaforo intenta decrementar, pero al no poder hacerlo dada la naturaleza de los semaforos, queda bloqueado hasta que otro proceso incremente el valor
        */

        P(semid, 0);
        P(semid, 2); // semaforo exclusion mutua

        /*
        seccion critica

        cada posicion a escribir en el buffer va a ser indicada por la variable indice
        y va a escribir el nro_a_escribir

        Por ejemplo:
            indice = 0;
            nro_a_escribir = 5;
            buffer[0] = 5;

        counter se va a incrementar en 1 en cada iteracion
        */

        sleep(1);
        buffer[indice] = nro_a_escribir;

        printf("Buffer posicion %d, escribe %d\n", indice, nro_a_escribir);

        /*
            indice = (indice + 1) % BUFFER_SIZE hace circular al indice

            indice = (0 + 1) % 5 = 1
            indice = (1 + 1) % 5 = 2
            indice = (2 + 1) % 5 = 3
            indice = (3 + 1) % 5 = 4
            indice = (4 + 1) % 5 = 0
        */
        indice = (indice + 1) % BUFFER_SIZE;

        // Decrementamos el numero a escribir haciendolo circular
        if (nro_a_escribir == 0)
            nro_a_escribir = 5;

        nro_a_escribir--;

        // fin seccion critica

        V(semid, 2); // desbloqueamos la exclusion mutua
        /*
        V(semid, 1) -> incrementamos el sem_full que tiene la cantidad de slots que se van completando
        Inicialmente arranca en 0
        */
        V(semid, 1);
    }

    return 0;
}