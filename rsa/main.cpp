
#include"MyRSA.h"


using namespace std;

void En(RSA*A);
void De(RSA* A);
void show(RSA* A);

int main()
{
    RSA *A=new RSA;
    while (1)
    {
        cout << "0-����" << "\n";
        cout << "1-����" << "\n";
        cout << "2-�鿴����" << "\n";
        cout << "���������-�˳�" << "\n";
        int i;
        cin >> i;
        switch (i)
        {
        case 0:
            En(A);
            break;
        case 1:
            De(A);
            break;
        case 2:
            show(A);
            break;
        default:
            exit(0);
        }
    }
    return 0;
}

void En(RSA* A)
{
    cout << "����Ҫ���ܵ��ַ�����" << "\n";
    string m;
    cin >> m;
    string c = A->Encrypt(m);
    cout << "���ܽ����" << c << "\n";
    cout << "\n";
}
void De(RSA* A)
{
    cout << "����Ҫ���ܵ��ַ�����" << "\n";
    string c;
    cin >> c;
    string m = A->Decrypt(c);
    cout << "���ܽ����" << m << "\n";
    cout << "\n";
}
void show(RSA* A)
{
    cout << "[p]=" << A->p << "\n";
    cout << "[q]=" << A->q << "\n";
    cout << "[e]=" << A->e << "\n";
    cout << "[d]=" << A->d << "\n";
    cout << "[phi]=" << A->phi << "\n";
    cout << "\n";
}
