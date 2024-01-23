
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
        cout << "0-加密" << "\n";
        cout << "1-解密" << "\n";
        cout << "2-查看参数" << "\n";
        cout << "其他任意键-退出" << "\n";
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
    cout << "输入要加密的字符串：" << "\n";
    string m;
    cin >> m;
    string c = A->Encrypt(m);
    cout << "加密结果：" << c << "\n";
    cout << "\n";
}
void De(RSA* A)
{
    cout << "输入要解密的字符串：" << "\n";
    string c;
    cin >> c;
    string m = A->Decrypt(c);
    cout << "解密结果：" << m << "\n";
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
