Este producer-consumer se realizo utilizando SYSTEM V shared memory y SYSTEM V semaphores

shm - > 
System V Shared Memory (shmget, shmat, shmdt, and shmctl functions).

sempahores -> 
System V Semaphores (semget, semop, and semctl functions).

--- FLUJO ---

Creamos la memoria compartida con shm.c 
Creamos los semaforos con crearsem.c 
Ejecutamos el productor
Ejecutamos el consumidor 
