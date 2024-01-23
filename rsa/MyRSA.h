#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<gmpxx.h>
#include<bitset>

using namespace std;

class RSA
{
public:
	RSA();
	~RSA();

	//�����ַ���s
	string Encrypt(string s);

	//����s
	string Decrypt(string s);

private:

	//����ָ��λ�������
	mpz_class randbits(int bits);
	//����ָ��λ������
	inline mpz_class randprime(int bits);

	// ��������ȡģ����
	void powmod(const mpz_class& base, const mpz_class& exponent, const mpz_class& modulus, mpz_class& result);

	//�����ޱ����Լ��
	bool is_prime(const mpz_class& n);

	//��aģb�ĳ˷���Ԫ���������x
	void exgcd(mpz_class a, mpz_class b, mpz_class& x,mpz_class& y);

	//����λstrת010��
	string StrToBitStr(string str);
	//010��תstr
	string BitStrToStr(string bstr);

	//010strת�����ַ���
	string BitStrToNumStr(string bstr);
	//�����ַ���ת010str
	string NumStrToBitStr(string nstr);



public:
	mpz_class p, q,n,e,d,phi;
};