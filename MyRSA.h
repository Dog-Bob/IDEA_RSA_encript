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

	//加密字符串s
	string Encrypt(string s);

	//解密s
	string Decrypt(string s);

private:

	//生成指定位数随机数
	mpz_class randbits(int bits);
	//生成指定位数素数
	inline mpz_class randprime(int bits);

	// 大数次幂取模函数
	void powmod(const mpz_class& base, const mpz_class& exponent, const mpz_class& modulus, mpz_class& result);

	//米勒罗宾素性检测
	bool is_prime(const mpz_class& n);

	//求a模b的乘法逆元，结果存入x
	void exgcd(mpz_class a, mpz_class b, mpz_class& x,mpz_class& y);

	//任意位str转010串
	string StrToBitStr(string str);
	//010串转str
	string BitStrToStr(string bstr);

	//010str转数字字符串
	string BitStrToNumStr(string bstr);
	//数字字符串转010str
	string NumStrToBitStr(string nstr);



public:
	mpz_class p, q,n,e,d,phi;
};