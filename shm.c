/*
    ESTE ARCHIVO CREA E INICIALIZA LOS SHARED MEMORY
*/
#include <stdio.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h> // para trabajar con shm

// cantidad de valores que almacena el buffer
#define BUFFER 5

void main()
{
    /*
        Dos valores de shared memory
        2_ buffer
    */

    // CREACION DE LA MEMORIA COMPARTIDA PARA AMBAS VARIABLES

    // Buffer memory
    int shmBuffer = shmget(0x3, 5, IPC_CREAT | IPC_EXCL | 0600);

    printf("Shared memory buffer: %d\n", shmBuffer);

    /*
        To show in console if the shared memory are created use ipcs wich means (INTERPROCESS COMUNICATION)
    */

    /*
        INICIALIZADOR DE LA MEMORIA COMPARTIDA PARA AMBAS VARIABLES
        Es decir, para cada espacio de memoria creado, le asignamos una variable dada
    */

    // Buffer memory attach
    char *buffer = (char *)shmat(shmBuffer, 0, 0);

    exit(0);
}