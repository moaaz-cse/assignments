#include <iostream>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <csignal>
using namespace std;

const key_t msgqueueKey = 64; // Define the same key for the message queue as processA.

int mqueue = -1; // Initialize the message queue descriptor.

struct Message
{
    long mtype;       // Message type (used for filtering messages).
    char mtext[1024]; // Message content.
};

int main()
{
    while (mqueue == -1)
    {
        try
        {
            // Try to access an existing message queue with the specified key.
            mqueue = msgget(msgqueueKey, 0666);
            if (mqueue == -1)
            {
                cout << "Queue does not exist, wait..." << endl;
                sleep(3); // Sleep for 3 seconds and then retry.
            }
        }
        catch (int err)
        {
            cerr << "An error occurred: " << err << endl;
            exit(1); // Exit the program with an error code.
        }
    }

    while (true)
    {
        try
        {
            string message;
            cout << "Enter a message: ";
            getline(cin, message);

            Message msg;
            msg.mtype = 1;
            strncpy(msg.mtext, message.c_str(), sizeof(msg.mtext) - 1);

            msgsnd(mqueue, &msg, sizeof(msg.mtext), 0); // Send a message to processA.

            sleep(2); // Sleep for 2 seconds.

            // Receive a response message from processA with type 1 (any type).
            msgrcv(mqueue, &msg, sizeof(msg.mtext), 1, 0);
            string receivedMessage = msg.mtext;

            if (receivedMessage == "STOP")
            {
                cout << "Aborting the connection" << endl;
                msgctl(mqueue, IPC_RMID, nullptr); // Remove the message queue.
                exit(0);                           // Exit the program successfully.
            }

            cout << "Message received by processA: " << receivedMessage << endl;
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
