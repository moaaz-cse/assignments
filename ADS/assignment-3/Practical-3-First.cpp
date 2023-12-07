#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;

int main()
{
    int pipe1[2];
    char buffer[100];

    if (pipe(pipe1) == -1)
    {
        perror("Pipe creation failed");
        return true;
    }

    pid_t childId = fork();

    if (childId < 0)
    {
        perror("Fork failed");
        return true;
    }

    // child process
    if (childId == 0)
    {
        close(pipe1[1]); // close write end in the child
        while (true)
        {
            read(pipe1[0], buffer, sizeof(buffer));
            cout << "Message received by child: " << buffer << endl;
            if (strcmp(buffer, "STOP") == 0)
            {
                break;
            }
        }
        close(pipe1[0]);
    }
    else
    {
        // Parent process
        close(pipe1[0]); // close read end in the perent
        while (1)
        {
            cout << "Enter a message for child('STOP' to quit): " << endl;
            cin >> buffer;
            write(pipe1[1], buffer, sizeof(buffer));
            if (strcmp(buffer, "STOP") == 0)
            {
                break;
            }
        }
        close(pipe1[1]);
    }
    return false;
}