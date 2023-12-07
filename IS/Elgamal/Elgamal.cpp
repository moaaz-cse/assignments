#include <iostream>
#include <cmath>
using namespace std;
// Find gcd
double gcd(double a, double b)
{
    double t;
    while (1)
    {
        t = fmod(a, b);
        if (t == 0)
            return b;
        a = b;
        b = t;
    }
}
int main()
{
    // 2 random prime numbers
    double p, q;
    cout << "Enter first prime number: ";
    cin >> p;
    cout << "Enter next prime number: ";
    cin >> q;
    double n = p * q; // calculate n
    double track;
    double phi = (p - 1) * (q - 1); // calculate phi
    // Public key (e stands for encrypt)
    double e;
    cout << "Enter value for e: ";
    cin >> e;
    // Check that 1 < e < phi(n) and gcd(e, phi(n)) = 1; i.e., e and phi(n) are coprime.
    while (e < phi)
    {
        track = gcd(e, phi);
        if (track == 1)
            break;
        else
            e++;
    }
    // Private key (d stands for decrypt)
    // Choosing d such that it satisfies d * e = 1 mod phi
    double d1 = 1 / e;
    double d = fmod(d1, phi);
    double message;
    cout << "Enter Original Message: ";
    cin >> message;
    cout << "\n"
         << "p = " << p;
    cout << "\n"
         << "q = " << q;
    cout << "\n"
         << "n = pq = " << n;
    cout << "\n"
         << "phi = " << phi;
    cout << "\n"
         << "e = " << e;
    cout << "\n"
         << "d = " << d;
    cout << "\n"
         << "Original Message = " << message;
    double c = pow(message, e); // encrypt the message
    double m = pow(c, d);
    c = fmod(c, n);
    m = fmod(m, n);
    cout << "\n"
         << "Encrypted message = " << c;
    cout << "\n"
         << "Decrypted message = " << m;
    return 0;
}

/*

Enter first prime number: 19
Enter next prime number: 23
Enter value for e: 5
Enter Original Message: 90

p = 19
q = 23
n = pq = 437
phi = 396
e = 5
d = 0.2
Original Message = 90
Encrypted message = 428
Decrypted message = 90[1]
*/