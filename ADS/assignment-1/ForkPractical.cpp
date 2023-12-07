#include <iostream>
#include <unistd.h>
using namespace std;

int main()
{
    pid_t r_value = fork();
    if (r_value < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (r_value > 0)
    {
        //  for Positive value
        cout << "parent process ID: " << getpid()
             << endl;
    }
    else
    {
        // For Zero value
        cout << "child process  ID: " << getpid()
             << endl;
    }
    return 0;
}