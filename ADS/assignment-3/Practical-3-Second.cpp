#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;

int main()
{
    int pipe1[2], pipe2[2];
    char buffer1[100], buffer2[100];

    if (pipe(pipe1) == -1)
    {
        cout << "Pipe 1 failed" << endl;
        perror("Pipe 1 creation failed");
        return 1;
    }

    if (pipe(pipe2) == -1)
    {
        cout << "Pipe 2 failed" << endl;
        perror("Pipe 2 creation failed");
        return 1;
    }

    pid_t child1, child2;

    child1 = fork();
    if (child1 < 0)
    {
        perror("Fork failed for first child");
        return 1;
    }
    if (child1 == 0)
    {
        // First child process
        close(pipe2[0]); // Close read end for pipe2
        close(pipe1[1]); // Close write end for pipe1
        while (true)
        {
            cout << "Child1 enter a message for child2 ('STOP' to quit): " << endl;
            cin >> buffer2;
            if (strcmp(buffer2, "STOP") == 0)
            {
                break;
            }
            write(pipe2[1], buffer2, strlen(buffer2) + 1);
            read(pipe1[0], buffer1, sizeof(buffer1));
            cout << "Message received by child1: " << buffer1 << endl << endl;
        }
        close(pipe1[0]);
        close(pipe2[1]);
    }

    child2 = fork();
    if (child2 < 0)
    {
        perror("Fork failed for second child");
        return 1;
    }
    if (child2 == 0)
    {
        // Second child process
        close(pipe2[1]); // Close write end for pipe2
        close(pipe1[0]); // Close read end for pipe1
        while (true)
        {
            cout << "Child2 enter a message for child1 ('STOP' to quit): " << endl;
            cin >> buffer1;
            if (strcmp(buffer1, "STOP") == 0)
            {
                break;
            }
            write(pipe1[1], buffer1, strlen(buffer1) + 1);
            read(pipe2[0], buffer2, sizeof(buffer2));
            cout << "Message received by child2: " << buffer2 << endl << endl;
        }
        close(pipe1[1]);
        close(pipe2[0]);
    }
    else
    {
        // Parent process
        // Close pipes not needed in the parent
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);
        // Wait for both child processes to finish
        wait(NULL);
        wait(NULL);
    }

    return 0;
}
