#ifndef _BIGINT_H_
#define _BIGINT_H_

#include"utili.h"
#include"SeqList.h"

class BigInt;
ostream& operator<<(ostream &out, const BigInt &bt);
class BigInt
{
    friend ostream& operator<<(ostream &out, const BigInt &bt);
    public:
        BigInt(long value = 0);
        void LoadData(u_long sz);//加载大数或者生成大数
        void PrintData();//打印大数
        size_t size()const;//大数的位数
    public:
        void SetSymbol(Symbol sign);
        Symbol GetSymbol()const;
        BigInt _Abs()const;
    public:
        //有符号运算
        static void S_Add(BigInt &res, const BigInt &bt1, const BigInt &bt2);//res = bt1+bt2
        static void S_Sub(BigInt &res, const BigInt &bt1, const BigInt &bt2);//res = bt1-bt2
        static void S_Mul(BigInt &res, const BigInt &bt1, const BigInt &bt2);//res = bt1*bt2
        static void S_Div(BigInt &res, const BigInt &bt1, const BigInt &bt2);//res = bt1/bt2
        static void S_Mod(BigInt &res, const BigInt &bt1, const BigInt &bt2);//res = bt1%bt2
        static void S_Power(BigInt &res, BigInt &bt1, BigInt &bt2);       //res = bt1^bt2
        static void S_PowerMod(BigInt &res,BigInt a, BigInt b, BigInt n); //res = a^b%n,为RSA加密算法作准备
    public:
        //无符号运算
        static void Add(BigInt &res, const BigInt &bt1, const BigInt &bt2);//res = bt1+bt2
        static void Sub(BigInt &res, const BigInt &bt1, const BigInt &bt2);//res = bt1-bt2
        static void Mul(BigInt &res, const BigInt &bt1, const BigInt &bt2);//res = bt1*bt2
        static void Div(BigInt &res, const BigInt &bt1, const BigInt &bt2);//res = bt1/bt2
        static void Mod(BigInt &res, const BigInt &bt1, const BigInt &bt2);//res = bt1%bt2
        static void Square(BigInt &res, const BigInt &bt1);                //res = bt1^2
        static BigInt Power(BigInt &bt1, BigInt bt2);
        static void PowerMod(BigInt &res,BigInt a, BigInt b, BigInt n); //res = a^b%n,为RSA加密算法作准备
        static u_char AddItem(u_char a, u_char b, u_char &carry);
        static u_char SubItem(u_char a, u_char b, u_char &borrow);
        static void MulItem(BigInt &res, const BigInt &bt1, const u_char bt2);
        static void AddMove(BigInt &res, const BigInt &bt, const u_long pos);
    public:
        void push_back(u_char x);
        void push_front(u_char x);
        u_char back();
        void clear();
        void clear_head_zero();
        void pop_back();
        u_char& operator[](int pos);
        const u_char& operator[](int pos)const;
        BigInt(const BigInt &bt);
    public:
        BigInt& operator=(const BigInt &bt);//赋值函数
        BigInt& operator=(u_long value);
        BigInt& operator++();           //++i
        BigInt& operator--();           //--i
        BigInt operator++(int);           //i++
        BigInt operator--(int);           //i--
    public:
        BigInt& operator+=(const BigInt &bt);
        BigInt& operator-=(const BigInt &bt);
        BigInt& operator*=(const BigInt &bt);
        BigInt& operator/=(const BigInt &bt);
        BigInt& operator%=(const BigInt &bt);
    public:
        //重载加减乘除模
        BigInt operator+(const BigInt &bt);
        BigInt operator-(const BigInt &bt);
        BigInt operator*(const BigInt &bt);
        BigInt operator/(const BigInt &bt);
        BigInt operator%(const BigInt &bt);
    public:
        bool operator>(const BigInt &bt)const;
        bool operator<(const BigInt &bt)const;
        bool operator>=(const BigInt &bt)const;
        bool operator<=(const BigInt &bt)const;
        bool operator==(const BigInt &bt)const;
        bool operator!=(const BigInt &bt)const;
    public:
        bool operator&(u_long x);       //a&1
    private:
        SeqList<u_char> big;
};

#endif
