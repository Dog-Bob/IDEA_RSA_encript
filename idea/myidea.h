#pragma once
#include<iostream>
#include<vector>
#include<bitset>
#include<math.h>
using namespace std;
typedef bitset<16> code; //16位
typedef bitset<128> key; //128位秘钥


class IDEA
{
public:
	IDEA();
	~IDEA();

	//Encrypt string
	string Encrypt(string p);
	string Decrypt(string c);
private:
	//二进制操作
	code XOR(code code_1, code code_2);
	code Plus(code code_1, code code_2);
	code invPlus(code code_in);
	code Times(code code_1, code code_2);
	//求乘法逆元
	void Exgcd(int a, int b, int& x, int& y);
	code invTimes(code code_in);
	//生成子密钥
	void subkeys_get(code keys_input[8]);
	void inv_subkeys_get(code sub_key[52]);
	//EncryptBlock
	string encrypt(string p);
	string decrypt(string c);
	//16bits str to binary str "01011100"
	string StrToBitStr(string str);
	string BitStrToStr(string bstr);

private:
	const string key_str="youngzhanjiegood";
	code keys_input[8] = { 0x151a, 0x048b, 0x71a1, 0xf9c7, 0x5266, 0xbfd6, 0x24a2, 0xdff1 };//128位秘钥
	bitset<16> sub_key[52];  //52个子秘钥
	bitset<16> inv_sub_key[52];//52个逆子秘钥

	int r;
};
