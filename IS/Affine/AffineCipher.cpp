#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

// Function to calculate the greatest common divisor (GCD) of two numbers.
int gcd(int a, int b)
{
    if (b == 0)
    {
        return a;
    }
    return gcd(b, a % b);
}

// Function to calculate the modular multiplicative inverse using the extended Euclidean algorithm.
int modInverse(int a, int m)
{
    a = a % m;
    for (int x = 1; x < m; x++)
    {
        if ((a * x) % m == 1)
        {
            return x;
        }
    }
    return -1; // If the modular inverse does not exist.
}

// Function to encrypt a plaintext message using the Affine cipher.
string affineEncrypt(const string &plaintext, int a, int b)
{
    string ciphertext = "";
    int m = 26; // Alphabet size (English)

    for (char c : plaintext)
    {
        if (isalpha(c))
        {
            char encryptedChar = (a * (toupper(c) - 'A') + b) % m + 'A';
            ciphertext += encryptedChar;
        }
        else
        {
            ciphertext += c; // Preserve non-alphabet characters.
        }
    }

    return ciphertext;
}

// Function to decrypt a ciphertext using the Affine cipher.
string affineDecrypt(const string &ciphertext, int a, int b)
{
    string plaintext = "";
    int m = 26; // Alphabet size (English)

    int aInverse = modInverse(a, m);
    if (aInverse == -1)
    {
        cout << "Modular inverse of 'a' does not exist. Cannot decrypt." << endl;
        return "";
    }

    for (char c : ciphertext)
    {
        if (isalpha(c))
        {
            char decryptedChar = (aInverse * (toupper(c) - 'A' - b + m)) % m + 'A';
            plaintext += decryptedChar;
        }
        else
        {
            plaintext += c; // Preserve non-alphabet characters.
        }
    }

    return plaintext;
}

int main()
{
    string plaintext;
    int a, b;

    cout << "Enter a message to encrypt: ";
    getline(cin, plaintext);

    cout << "Enter 'a' (an integer): ";
    cin >> a;

    cout << "Enter 'b' (an integer): ";
    cin >> b;

    // Check if 'a' is coprime to 26
    if (gcd(a, 26) != 1)
    {
        cout << "'a' must be coprime to 26 for the Affine cipher. Please choose another 'a' value." << endl;
        return 1;
    }

    string ciphertext = affineEncrypt(plaintext, a, b);
    cout << "Encrypted Text: " << ciphertext << endl;

    string decryptedText = affineDecrypt(ciphertext, a, b);
    cout << "Decrypted Text: " << decryptedText << endl;

    return 0;
}
