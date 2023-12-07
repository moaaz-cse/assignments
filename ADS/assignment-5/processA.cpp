#include <iostream>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <csignal>
using namespace std;

const key_t msgqueueKey = 64; // Define a key for the message queue.

int mqueue = -1; // Initialize the message queue descriptor.

struct Message
{
    long mtype;       // Message type (used for filtering messages).
    char mtext[1024]; // Message content.
};

int main()
{
    // Attempt to create or access an existing message queue with the specified key.
    mqueue = msgget(msgqueueKey, 0666 | IPC_CREAT);
    if (mqueue == -1)
    {
        cerr << "Message queue creation failed." << endl;
        exit(1); // Exit the program with an error code.
    }

    while (true)
    {
        try
        {
            Message msg;

            // Receive a message from the message queue with type 1 (any type).
            msgrcv(mqueue, &msg, sizeof(msg.mtext), 1, 0);
            string receivedMessage = msg.mtext;

            if (receivedMessage == "STOP")
            {
                cout << "Aborting connection." << endl;
                msgctl(mqueue, IPC_RMID, nullptr); // Remove the message queue.
                exit(0);                           // Exit the program successfully.
            }

            cout << "Message received from processB: " << receivedMessage << endl;

            string response;
            cout << "Enter a message: ";
            getline(cin, response);

            msg.mtype = 1;
            strncpy(msg.mtext, response.c_str(), sizeof(msg.mtext));
            msgsnd(mqueue, &msg, sizeof(msg.mtext), 0); // Send a response message to processB.
        }
        catch (int err)
        {
            cerr << "Some error occurred: " << err << endl;
            msgctl(mqueue, IPC_RMID, nullptr); // Remove the message queue on error.
            exit(1);                           // Exit the program with an error code.
        }
    }

    return 0;
}
