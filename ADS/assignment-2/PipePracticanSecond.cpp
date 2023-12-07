#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;

int main()
{
    int pipe1[2], pipe2[2];
    char buffer[100];
    pid_t child1, child2;

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1)
    {
        cout << "Pipe creation failed." << endl;
        return 1;
    }

    child1 = fork();

    if (child1 == -1)
    {
        cerr << "Fork 1 failed." << endl;
        return 1;
    }
    else if (child1 == 0) // Child 1
    {
        close(pipe1[0]); // Close unused read end of pipe1
        close(pipe2[1]); // Close unused write end of pipe2

        cout << "Child 1 sending data to Child 2" << endl;
        write(pipe1[1], "Hello from Child 1\n", 20);

        // Read response from Child 2
        int n = read(pipe2[0], buffer, sizeof(buffer));
        cout << "Child 1 received: " << string(buffer, n) << endl;

        close(pipe1[1]);
        close(pipe2[0]);
    }
    else
    {
        child2 = fork();

        if (child2 == -1)
        {
            cerr << "Fork 2 failed." << endl;
            return 1;
        }
        else if (child2 == 0) // Child 2
        {
            close(pipe1[1]); // Close unused write end of pipe1
            close(pipe2[0]); // Close unused read end of pipe2

            // Read data from Child 1
            int n = read(pipe1[0], buffer, sizeof(buffer));
            cout << "Child 2 received: " << string(buffer, n) << endl;

            // Respond to Child 1
            write(pipe2[1], "Hello from Child 2\n", 20);

            close(pipe1[0]);
            close(pipe2[1]);
        }
        else // Parent
        {
            close(pipe1[0]); // Close unused read ends
            close(pipe1[1]); // Close unused write ends
            close(pipe2[0]);
            close(pipe2[1]);

            // Wait for both child processes to finish
            wait(NULL);
            wait(NULL);
        }
    }

    return 0;
}
