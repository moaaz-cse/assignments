#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

int main()
{
    int fileDesc;
    const char *myfifo = "fifo";

    // Create the FIFO (named pipe)
    mkfifo(myfifo, 0666);

    string msgBA;
    char receivedMsg[1024];

    const string stop_message = "exit";
    bool exitFlag = false;

    while (!exitFlag)
    {
        // Open the FIFO
        fileDesc = open(myfifo, O_WRONLY);
        if (fileDesc == -1)
        {
            perror("Error opening FIFO for writing");
            return 1;
        }

        cout << "Enter a message (or type 'exit' to quit): ";
        getline(cin, msgBA);

        if (msgBA == stop_message)
        {
            exitFlag = true;
        }
        else
        {
            write(fileDesc, msgBA.c_str(), msgBA.length() + 1);
            close(fileDesc);

            fileDesc = open(myfifo, O_RDONLY);
            if (fileDesc == -1)
            {
                perror("Error opening FIFO for reading");
                return 1;
            }

            ssize_t bytesRead = read(fileDesc, receivedMsg, sizeof(receivedMsg));
            if (bytesRead == -1)
            {
                perror("Error reading from FIFO");
                close(fileDesc); // Close the file descriptor before exiting
                return 1;
            }
            cout << "ProcessA: " << receivedMsg << endl;
            close(fileDesc);
        }
    }

    return 0;
}