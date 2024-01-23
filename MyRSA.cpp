#include "MyRSA.h"

RSA::RSA()
{
    p = randprime(512);
    q = randprime(512);
    n = p * q;
    phi = (p - 1) * (q - 1);
    e = 65537;
    mpz_class y;
    exgcd(e, phi, d, y);

}
RSA::~RSA()
{

}

mpz_class RSA::randbits(int bits)
{
    gmp_randclass a(gmp_randinit_default);
    a.seed(rand());
    mpz_class l(bits);
    return a.get_z_bits(l);
}

inline mpz_class RSA::randprime(int bits)
{
    mpz_class a = 0;
    while (true)
    {
        a = randbits(bits);
        if (is_prime(a))
        {
            break;
        }
    }
    return a;
}

void RSA::powmod(const mpz_class& base, const mpz_class& exponent, const mpz_class& modulus, mpz_class& result)
{
    mpz_powm(result.get_mpz_t(), base.get_mpz_t(), exponent.get_mpz_t(), modulus.get_mpz_t());
}

bool RSA::is_prime(const mpz_class& n)
{
    vector<mpz_class>A = { 2,3,5,7,11,17,19,23,117 };
    int k = 0;
    mpz_class m = n;
    m = m - 1;
    while (m % 2 == 0)
    {
        m = m >> 1;
        k++;
    }
    mpz_class q = m;
    for (auto a : A)
    {
        int possible = 0;
        for (int i = 0; i < k; i++)
        {
            mpz_class ret = 0;
            mpz_class base = pow(2, i) * q;
            powmod(a, base, n, ret);
            if (ret == 1 || ret == n - 1)
            {
                possible++;
                break;
            }
        }
        if (possible == 0) { return false; }
    }
    return true;
}

void RSA::exgcd(mpz_class a, mpz_class b, mpz_class& x, mpz_class& y)
{
    if (b == 0) {
        x = 1, y;
        return;
    }
    exgcd(b, a % b, y, x);
    y -= a / b * x;
}

string RSA::Encrypt(string s)
{
    string bstr = StrToBitStr(s);
    string NumStr = BitStrToNumStr(bstr);

    mpz_class m(NumStr);
    mpz_class c;
    powmod(m, e, n, c);
    return c.get_str();
}

string RSA::Decrypt(string s)
{
    mpz_class c(s);
    mpz_class m;
    powmod(c, d, n, m);

    string NumStr = m.get_str();
    string bstr = NumStrToBitStr(NumStr);
    string ss = BitStrToStr(bstr);
    return ss;
}

string RSA::StrToBitStr(string str)
{
    int n = str.size();
    vector<bool>bstr(n * 8);
    for (int i = 0; i < n; i++)//i：n个char
    {
        bitset<8> bits = bitset<8>(str[i]);
        for (int j = 0; j < 8; j++)//j：64bit里面的界限,每8个一组
        {
            bstr[i * 8 + j] = bits[7 - j];
        }
    }
    string s = "";
    for (auto x : bstr) { if (x == 1) { s = s + '1'; } else { s = s + '0'; } }
    //加上前导1避免0为前导
    s = '1' + s;
    return s;
}

string RSA::BitStrToStr(string b2)
{
    string bstr;
    for (int i = 1; i < b2.size(); i++) { bstr += b2[i]; }
    string str = "";
    //每八位转化成十进制，然后将数字结果转化成字符
    int sum;
    for (int i = 0; i < bstr.size(); i += 8)
    {
        sum = 0;
        for (int j = 0; j < 8; j++)
            if (bstr[i + j] == '1')
                sum = sum * 2 + 1;
            else
                sum = sum * 2;
        str = str + char(sum);
    }
    return str;
}

string RSA::BitStrToNumStr(string bstr)
{
    int n = bstr.size();
    mpz_class sum = 0;
    mpz_class x = 1;
    for (int i = 0; i < n; i++)
    {
        if (bstr[i] == '1')
        {
            sum += x;
        }
        x = x * 2;
    }
    return sum.get_str();
}
string RSA::NumStrToBitStr(string nstr)
{
    mpz_class N(nstr);
    string bstr = "";
    while (1)
    {
        if (N%2!=0)
        {
            bstr = bstr+'1';
        }
        else
        {
            bstr = bstr+'0';
        }
        N = N / 2;
        if (N == 0)
        {
            //如果二进制最后一位是0就加上之后再退出
            bstr = bstr + '0';
            break;
        }
    }
    return bstr;
}



