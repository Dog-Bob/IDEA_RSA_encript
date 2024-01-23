#include"myidea.h"

IDEA::IDEA()
{
    for (int i = 0; i < 8; i = i + 2)
    {
        int64_t l, r;
        l = key_str[2*i] << 8;
        r = key_str[2*i + 1];
        keys_input[i] = l + r;
    }
    subkeys_get(keys_input); //生成子秘钥
    inv_subkeys_get(sub_key);//生成逆子秘钥
}

IDEA::~IDEA()
{
    
}

//二进制操作
code IDEA::XOR(code code_1, code code_2)
{
	return code_1 ^ code_2;
}
code IDEA::Plus(code code_1, code code_2)
{
    int tmp = 0;
    code result;
    for (int i = 0; i < 16; i++) //二进制转换成十进制
    {
        tmp += code_1[i] * pow(2, i) + code_2[i] * pow(2, i);
    }
    tmp %= 65536;
    bitset<16>binary(tmp); //转换成二进制
    for (int i = 0; i < 16; i++)
        result[i] = binary[i];

    return result;
}
code IDEA::invPlus(code code_in)
{
    int tmp = 0;
    code result;
    for (int i = 0; i < 16; i++) //二进制转换成十进制
        tmp += code_in[i] * pow(2, i);
    tmp = 65536 - tmp;
    bitset<16>binary(tmp); //转换成二进制
    for (int i = 0; i < 16; i++)
        result[i] = binary[i];

    return result;

}
code IDEA::Times(code code_1, code code_2)
{
    code result;
    long long tmp;
    long long tmp_1 = 0, tmp_2 = 0;
    for (int i = 0; i < 16; i++)  //二进制转换成十进制
    {
        tmp_1 += code_1[i] * pow(2, i);
        tmp_2 += code_2[i] * pow(2, i);
    }
    if (code_1 == 0)
        tmp_1 = 65536;
    if (code_2 == 0)
        tmp_2 = 65536;

    tmp = (tmp_1 * tmp_2) % 65537;
    if (tmp == 65536)  //如果得到最大值即等价于0x0000
        result = 0x0000;
    else
    {
        bitset<16>binary(tmp); //转换成二进制
        for (int i = 0; i < 16; i++)
            result[i] = binary[i];
    }
    return result;
}
//求乘法逆元
void IDEA::Exgcd(int a, int b, int& x, int& y)
{
    if (!b)
        x = 1, y = 0;
    else
        Exgcd(b, a % b, y, x), y -= a / b * x;
}
code IDEA::invTimes(code code_in)
{
    code result;
    int tmp = 0;
    for (int i = 0; i < 16; i++)  //首先转换成十进制
        tmp += code_in[i] * pow(2, i);

    int x, y;
    int p = 65537;
    Exgcd(tmp, p, x, y);
    x = (x % p + p) % p;  //x即为tmp在 (mod65537) 的乘法逆
    bitset<16>binary(x); //转换成二进制
    for (int j = 0; j < 16; j++)
        result[j] = binary[j];

    return result;
}
//生成子密钥
void IDEA::subkeys_get(code keys_input[8])
{
    key keys;
    for (int i = 0; i < 8; i++)  //转化成128位
    {
        for (int j = 0; j < 16; j++)
        {
            keys[j + 16 * i] = keys_input[7 - i][j];
        }
    }
    for (int i = 0; i < 8; i++)  //前8个子秘钥（不移动）
    {
        for (int j = 0; j < 16; j++)
            sub_key[i][15 - j] = keys[127 - (j + 16 * i)];
    }
    for (int i = 0; i < 5; i++)  //中间40个子秘钥（）每次循环左移25位
    {
        key tmp_keys = keys >> 103;
        keys = (keys << 25) | (tmp_keys);
        for (int j = (8 + 8 * i); j < (8 * (i + 2)); j++)
        {
            for (int k = 0; k < 16; k++)
                sub_key[j][15 - k] = keys[127 - (k + 16 * (j - 8 - 8 * i))];
        }
    }
    key tmp_keys = keys >> 103;   //最后一次循环左移取前四个
    keys = (keys << 25) | (tmp_keys);
    for (int i = 48; i < 52; i++)
    {
        for (int j = 0; j < 16; j++)
            sub_key[i][15 - j] = keys[127 - (j + 16 * (i - 48))];
    }
}
void IDEA::inv_subkeys_get(code sub_key[52])
{
    //生成逆子秘钥
    for (int i = 6; i < 48; i = i + 6)   //U_1, U_2, U_3, U_4   (2 <= i <= 8)
    {
        inv_sub_key[i] = invTimes(sub_key[48 - i]);
        inv_sub_key[i + 1] = invPlus(sub_key[50 - i]);
        inv_sub_key[i + 2] = invPlus(sub_key[49 - i]);
        inv_sub_key[i + 3] = invTimes(sub_key[51 - i]);
    }
    for (int i = 0; i < 48; i = i + 6)    //U_5, U_6   (1 <= i <= 8)
    {
        inv_sub_key[i + 4] = sub_key[46 - i];
        inv_sub_key[i + 5] = sub_key[47 - i];
    }
    //U_1, U_2, U_3, U_4   (i = 1, 9)
    inv_sub_key[0] = invTimes(sub_key[48]);
    inv_sub_key[1] = invPlus(sub_key[49]);
    inv_sub_key[2] = invPlus(sub_key[50]);
    inv_sub_key[3] = invTimes(sub_key[51]);

    inv_sub_key[48] = invTimes(sub_key[0]);
    inv_sub_key[49] = invPlus(sub_key[1]);
    inv_sub_key[50] = invPlus(sub_key[2]);
    inv_sub_key[51] = invTimes(sub_key[3]);
}

//En/De crypt str having 8 chars
string IDEA::encrypt(string p)
{
    string bp = StrToBitStr(p);
    bitset<64> plaint(bp);
    bitset<16> I_1, I_2, I_3, I_4;
    bitset<64> cipher;
    for (int i = 0; i < 16; i++) //明文分成4个16位(I_1, I_2, I_3, I_4)
    {
        I_1[15 - i] = plaint[63 - i];
        I_2[15 - i] = plaint[47 - i];
        I_3[15 - i] = plaint[31 - i];
        I_4[15 - i] = plaint[15 - i];
    }
    for (int i = 0; i < 48; i = i + 6)  //轮结构运算
    {
        bitset<16> tmp_1 = Times(sub_key[i], I_1);
        bitset<16> tmp_2 = Plus(sub_key[i + 1], I_2);
        bitset<16> tmp_3 = Plus(sub_key[i + 2], I_3);
        bitset<16> tmp_4 = Times(sub_key[i + 3], I_4);
        bitset<16> tmp_5 = XOR(tmp_1, tmp_3);
        bitset<16> tmp_6 = XOR(tmp_2, tmp_4);
        bitset<16> tmp_7 = Times(sub_key[i + 4], tmp_5);
        bitset<16> tmp_8 = Plus(tmp_6, tmp_7);
        bitset<16> tmp_9 = Times(tmp_8, sub_key[i + 5]);
        bitset<16> tmp_10 = Plus(tmp_7, tmp_9);
        I_1 = XOR(tmp_1, tmp_9);
        I_2 = XOR(tmp_3, tmp_9);
        I_3 = XOR(tmp_2, tmp_10);
        I_4 = XOR(tmp_4, tmp_10);
    }
    //输出变换
    bitset<16> Y_1 = Times(I_1, sub_key[48]);
    bitset<16> Y_2 = Plus(I_3, sub_key[49]);
    bitset<16> Y_3 = Plus(I_2, sub_key[50]);
    bitset<16> Y_4 = Times(I_4, sub_key[51]);

    for (int i = 0; i < 16; i++) //整合4个输出成密文
    {
        cipher[i] = Y_4[i];
        cipher[i + 16] = Y_3[i];
        cipher[i + 32] = Y_2[i];
        cipher[i + 48] = Y_1[i];
    }
    string bc = cipher.to_string();
    string c = BitStrToStr(bc);
    return c;
}
string IDEA::decrypt(string c)
{
    string bc = StrToBitStr(c);
    bitset<64> cipher(bc);
    //解密
    bitset<16> I_1, I_2, I_3, I_4;
    bitset<64> plaint;
    for (int i = 0; i < 16; i++)
    {
        I_1[15 - i] = cipher[63 - i];
        I_2[15 - i] = cipher[47 - i];
        I_3[15 - i] = cipher[31 - i];
        I_4[i] = cipher[i];
    }
    for (int i = 0; i < 48; i = i + 6)
    {
        bitset<16> tmp_1 = Times(inv_sub_key[i], I_1);
        bitset<16> tmp_2 = Plus(inv_sub_key[i + 1], I_2);
        bitset<16> tmp_3 = Plus(inv_sub_key[i + 2], I_3);
        bitset<16> tmp_4 = Times(inv_sub_key[i + 3], I_4);
        bitset<16> tmp_5 = XOR(tmp_1, tmp_3);
        bitset<16> tmp_6 = XOR(tmp_2, tmp_4);
        bitset<16> tmp_7 = Times(inv_sub_key[i + 4], tmp_5);
        bitset<16> tmp_8 = Plus(tmp_6, tmp_7);
        bitset<16> tmp_9 = Times(tmp_8, inv_sub_key[i + 5]);
        bitset<16> tmp_10 = Plus(tmp_7, tmp_9);
        I_1 = XOR(tmp_1, tmp_9);
        I_2 = XOR(tmp_3, tmp_9);
        I_3 = XOR(tmp_2, tmp_10);
        I_4 = XOR(tmp_4, tmp_10);
    }
    bitset<16> Y_1 = Times(I_1, inv_sub_key[48]);
    bitset<16> Y_2 = Plus(I_3, inv_sub_key[49]);
    bitset<16> Y_3 = Plus(I_2, inv_sub_key[50]);
    bitset<16> Y_4 = Times(I_4, inv_sub_key[51]);

    for (int i = 0; i < 16; i++)
    {
        plaint[i] = Y_4[i];
        plaint[i + 16] = Y_3[i];
        plaint[i + 32] = Y_2[i];
        plaint[i + 48] = Y_1[i];
    }
    string bp = plaint.to_string();
    string p = BitStrToStr(bp);
    return  p;
}

//任意位str转010串：?=00111111(ascii-63)
string IDEA::StrToBitStr(string str)//ok
{
    //bitset<> *bstr=new bitset<>(n);
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
    //string s = bstr.to_string();
    //添加一个翻转操作
    //reverse(begin(s), end(s));
    return s;
}

string IDEA::BitStrToStr(string bstr)
{
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


//En、Decrypt str of any lenth
string IDEA::Encrypt(string p)
{
    int n = p.size();
    //n=m*8+r
    int m = n / 8;
    r = n % 8;

    string c = "";
    for (int i = 0; i < m; i++)
    {
        string tmp = p.substr(i * 8, 8);
        c = c + encrypt(tmp);
        //string tmp_en = encrypt(tmp);
        //cout << tmp_en << "\n" << decrypt(tmp_en) << "\n";
    }
    string tmp = p.substr(m * 8, r);
    for (int i = 0; i < 8 - r; i++) { tmp += "0"; }
    c = c + encrypt(tmp);
    return c;
}
string IDEA::Decrypt(string c)
{
    int n = c.size();
    int m = n / 8;
    string last = c.substr(n - 8, 8);//last
    string plast = decrypt(last);
    plast.erase(r, 8);
    string p = "";

    for (int i = 0; i < m - 1; i++)
    {
        string tmp = c.substr(i * 8, 8);
        p = p + decrypt(tmp);
    }
    p = p + plast;
    return p;
}
