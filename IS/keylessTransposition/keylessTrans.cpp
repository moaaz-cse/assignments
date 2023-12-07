#include <iostream>
#include <string>
using namespace std;
int main()
{
    string message;
    cout << "Enter plain text: " << endl;
    getline(cin, message); // Use getline to read the entire line
    string row1;
    string row2;
    for (int i = 0; i < message.length(); i++)
    {
        if (i % 2 != 0)
        {
            row1 += message[i];
        }
        else
        {
            row2 += message[i];
        }
    }
    cout << "Decrypted Message is : " << endl;
    cout << row2 << row1 << endl;
    return 0;
}

/*
Enter plain text: 
keyless
Decrypted Message is : 
kyesels
*/