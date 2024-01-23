#include<iostream>
#include<string>
#include"myidea.h"


using namespace std;

void Encrypt(IDEA*A,vector<string>& Cipher);
void Decrypt(IDEA*A,vector<string>& Cipher);


int main()
{
    IDEA *A=new IDEA;
    vector<string>Cipher;
    while (true)
    {
        cout << endl;
        cout << "************IDEA ENCRYPT***************\n";
        cout << "1.Encrypt text\n";
        cout << "2.Decrypt text\n";
        cout << "0.Quit\n";
        cout << "************Version  1.0***************\n";
        int choice;
        cin >> choice;
        switch (choice)
        {
        case 1:
            Encrypt(A,Cipher);
            break;
        case 2:
            Decrypt(A,Cipher);
            break;
        case 0:
            return 0;
        default:
            exit(0);
        }
    }
    
    return 0;
}


void Encrypt(IDEA*A,vector<string>&Cipher)
{
    cout << "******Enctypt Procedure:\n";
    cout << "input text being encrypted:\n";
    string p="";
    cin >> p;
    cout << "the cipher text is:\n";
    auto tmp = A->Encrypt(p);
    Cipher.push_back(tmp);
    cout <<tmp<<"\n";
}
void Decrypt(IDEA*A,vector<string>& Cipher)
{
    cout << "******Dectypt Procedure:\n";
    cout << "input number of text being decrypted:\n";
    for (int i=0;i<Cipher.size();i++)
    {
        cout << i << "->" << Cipher[i] << "\n";
    }
    cout << endl;
    int choice = 0;
    cin >> choice;
    string c = Cipher[choice];
    cout << "the plaint text is:\n";
    cout << A->Decrypt(c) << "\n";
}