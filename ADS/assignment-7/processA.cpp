#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

#define SHM_SIZE 1024
#define KEY 12345

struct shared_data
{
    char message[SHM_SIZE];
    bool stop;
    bool ready;
};

int shmid;
int semid;
struct shared_data *shared_memory = NULL;

void signal_handler(int signo)
{
    if (signo == SIGINT || signo == SIGTERM)
    {
        printf("Process 1 received stop signal. Cleaning up...\n");
        shared_memory->stop = true;
    }
}

int main()
{
    // Create or access the shared memory segment
    shmid = shmget(KEY, sizeof(struct shared_data), 0666 | IPC_CREAT);
    if (shmid == -1)
    {
        perror("shmget");
        exit(1);
    }

    // Attach to the shared memory
    shared_memory = shmat(shmid, NULL, 0);
    if (shared_memory == (struct shared_data *)-1)
    {
        perror("shmat");
        exit(1);
    }

    // Create or access the semaphore
    semid = semget(KEY, 1, 0666 | IPC_CREAT);
    if (semid == -1)
    {
        perror("semget");
        exit(1);
    }

    // Initialize semaphore
    semctl(semid, 0, SETVAL, 1);

    // Set up a signal handler for stopping the communication
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    // Communication loop
    while (!shared_memory->stop)
    {
        // Wait for the semaphore
        struct sembuf wait_buf = {0, -1, 0};
        semop(semid, &wait_buf, 1);

        if (!shared_memory->stop)
        {
            if (shared_memory->ready)
            {
                if (strcmp(shared_memory->message, "STOP") == 0)
                {
                    printf("Process 1 received stop signal. Cleaning up...\n");
                    shared_memory->stop = true;
                }
                else
                {
                    printf("Message received by Process 1 (or 'STOP' to quit) : %s\n", shared_memory->message);
                }
                printf("Enter a message for Process 2 : ");
                fgets(shared_memory->message, SHM_SIZE, stdin);

                // Remove newline character
                size_t len = strlen(shared_memory->message);
                if (shared_memory->message[len - 1] == '\n')
                {
                    shared_memory->message[len - 1] = '\0';
                }

                if (strcmp(shared_memory->message, "STOP") == 0)
                {
                    shared_memory->stop = true;
                }

                printf("Process 1 sent: %s\n", shared_memory->message);
                shared_memory->ready = false;
            }
        }

        // Release the semaphore
        struct sembuf signal_buf = {0, 1, 0};
        semop(semid, &signal_buf, 1);

        // Wait for Process 2 to signal readiness
        while (!shared_memory->ready && !shared_memory->stop)
        {
            usleep(100000); // Sleep for 0.1 seconds
        }
    }

    // Clean up
    shmdt(shared_memory);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);

    return 0;
}