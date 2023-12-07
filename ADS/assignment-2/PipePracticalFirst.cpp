#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;

int main()
{
    int myPipe[2], m1, m2;
    char buffer[100];
    pid_t forkReturn;
    pipe(myPipe);
    forkReturn = fork();
    if (forkReturn > 0)
    {
        cout << "passing value to child" << endl;
        write(myPipe[1], "Hello my child\n", 15);
        cout << "Parent received message: ";
        m2 = read(myPipe[0], buffer, 100);
        write(1, buffer, m2);
    }
    else
    {
        cout << "Child received data: ";
        m1 = read(myPipe[0], buffer, 100);
        write(1, buffer, m1);
        cout << "Replying back to parent\n" << endl;
        write(myPipe[1], "Hello, sir\n", 10);
    }

    return 0;
}
