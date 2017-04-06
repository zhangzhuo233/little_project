#include"BigInt.h"

BigInt::BigInt(long value)
{
    SetSymbol(POSITIVE);
    (*this).clear();
    if(value == 0)
        push_back(0);
    else
    {
        if(value < 0)
        {
            SetSymbol(NEGATIVE);
            value = abs(value);
        }
        while(value > 0)
        {
            push_back(value%10);
            value/=10;
        }
    }
}
BigInt::BigInt(const BigInt &bt)
{
    *this = bt;
}
void BigInt::SetSymbol(Symbol sign)
{
    big.SetSymbol(sign);
}
Symbol BigInt::GetSymbol()const
{
    big.GetSymbol();
}
BigInt BigInt::operator+(const BigInt &bt)
{
    BigInt res;
    S_Add(res, *this, bt);
    return res;
}
BigInt BigInt::operator-(const BigInt &bt)
{
    BigInt res;
    S_Sub(res, *this, bt);
    return res;
}
BigInt BigInt::operator*(const BigInt &bt)
{
    BigInt res;
    S_Mul(res, *this, bt);
    return res;
}
BigInt BigInt::operator/(const BigInt &bt)
{
    BigInt res;
    S_Div(res, *this, bt);
    return res;
}
BigInt BigInt::operator%(const BigInt &bt)
{
    BigInt res;
    S_Mod(res, *this, bt);
    return res;
}
bool BigInt::operator&(u_long x)
{
    BigInt tmp(2);
    BigInt ans = *this % tmp;
    return ans==1;
}
ostream& operator<<(ostream &out, const BigInt &bt)
{
    if(bt.GetSymbol() == NEGATIVE && bt != 0)
        cout<<"-";
    for(size_t i=bt.size(); i>0; --i)
        out<<(int)bt[i];
    return out;
}
void BigInt::push_back(u_char x)
{
    big.push_back(x);
}

void BigInt::push_front(u_char x)
{
    big.push_front(x);
}

u_char BigInt::back()
{
    return big.back();
}
void BigInt::clear()
{
    big.clear();
}
void BigInt::pop_back()
{
    big.pop_back();
}
void BigInt::clear_head_zero()
{
    while(size()>0 && *this!=0 && back() == 0)
        pop_back();
}
BigInt& BigInt::operator=(const BigInt &bt)
{
    (*this).big = bt.big;
    return *this;
}
BigInt& BigInt::operator=(u_long value)
{
    (*this).clear();
    if(value == 0)
        push_back(0);
    else
    {
        while(value > 0)
        {
            push_back(value % 10);
            value/=10;
        }
    }
    return *this;
}
BigInt& BigInt::operator++()//++i
{
    u_char sign = 1;
    size_t i = 1;
    size_t j = 1;
    if(this->GetSymbol() == POSITIVE)
    {
        while(sign>0 && i<=size())
        {
            (*this)[i] = AddItem((*this)[i], 0, sign);
            ++i;
        }
        if(sign > 0)
            push_back(sign);
    }
    else if(this->GetSymbol() == NEGATIVE)
    {
        while(sign>0 && i<=size())
        {
            (*this)[j] = SubItem((*this)[j], 0, sign);
            ++j;
        }
    }
    clear_head_zero();
    return *this;
}
BigInt BigInt::operator++(int)//i++
{
    BigInt tmp = *this;
    ++*this;
    return tmp;
}
BigInt& BigInt::operator--()
{
    u_char sign = 1;
    size_t i = 1;
    size_t j = 1;
    if(this->GetSymbol() == POSITIVE)
    {
        while(sign>0 && i<=size())
        {
            (*this)[i] = SubItem((*this)[i], 0, sign);
            ++i;
        }
    }
    else if(this->GetSymbol() == NEGATIVE)
    {
        while(sign>0 && i<=size())
        {
            (*this)[j] = AddItem((*this)[j], 0, sign);
            ++j;
        }
        if(sign > 0)
            push_back(sign);
    }
    clear_head_zero();
    return *this;
}
BigInt BigInt::operator--(int)//i--
{
    BigInt tmp = *this;
    --*this;
    return tmp;
}
u_char& BigInt::operator[](int pos)
{
    return big[pos];
}
const u_char& BigInt::operator[](int pos)const
{
    return big[pos];
}
BigInt& BigInt::operator+=(const BigInt &bt2)
{
    BigInt res;
    res.clear();
    BigInt::S_Add(res, *this, bt2);
    *this=res;
    return *this;
}
BigInt& BigInt::operator-=(const BigInt &bt)
{
    assert(*this >= bt);
    if(*this == bt)
    {
        *this = 0;
    }
    else
    {
        BigInt res;
        S_Sub(res, *this, bt);
        *this = res;
    }
    return *this;
}
BigInt& BigInt::operator*=(const BigInt &bt)
{
    if(bt == 0 && (*this) == 0)
    {
        *this = 0;
    }
    else
    {
        BigInt res;
        S_Mul(res, *this, bt);
        *this = res;
    }
    return *this;
}
BigInt& BigInt::operator/=(const BigInt &bt)
{
    if(bt == 0)
    {
        cout<<"diver is 0";
        exit(1);
    }
    if(*this == bt)
    {
        *this = 1;
        return *this;
    }
    BigInt res;
    S_Div(res, *this, bt);
    *this = res;
}
BigInt& BigInt::operator%=(const BigInt &bt)
{
    if(bt == 1)
        return *this;
    BigInt res;
    S_Mod(res, *this, bt);
    *this = res;
}
bool BigInt::operator>(const BigInt &bt)const
{
    Symbol symbol1 = GetSymbol();
    Symbol symbol2 = bt.GetSymbol();
    if(symbol1 != symbol2)//正数大于负数
    {
        if(symbol1 == POSITIVE)
            return true;
        return false;
    }
    if(symbol1 == POSITIVE)
    {
        if(size() > bt.size())
            return true;
        else if(size() < bt.size())
            return false;
        //=
        int i = size();
        while(i > 0)
        {
            if((*this)[i] > bt[i])
                return true;
            else if((*this)[i] < bt[i])
                return false;
            --i;
        }
        return false;
    }
    else if(symbol1 == NEGATIVE)
    {
        if(size() > bt.size())
            return false;
        else if(size() < bt.size())
            return true;
        //=
        int i = size();
        while(i > 0)
        {
            if((*this)[i] > bt[i])
                return false;
            else if((*this)[i] < bt[i])
                return true;
            --i;
        }
        return false;
    }
}
bool BigInt::operator<(const BigInt &bt)const
{
    Symbol symbol1 = GetSymbol();
    Symbol symbol2 = bt.GetSymbol();
    if(symbol1 != symbol2)//正数大于负数
    {
        if(symbol1 == NEGATIVE)
            return true;
        return false;
    }
    if(symbol1 == POSITIVE)
    {
        if(size() < bt.size())
            return true;
        else if(size() > bt.size())
            return false;
        //=
        int i = size();
        while(i > 0)
        {
            if((*this)[i] < bt[i])
                return true;
            else if((*this)[i] > bt[i])
                return false;
            --i;
        }
        return false;
    }
    else if(symbol1 == NEGATIVE)
    {
        if(size() > bt.size())
            return true;
        else if(size() < bt.size())
            return false;
        //=
        int i = size();
        while(i > 0)
        {
            if((*this)[i] > bt[i])
                return true;
            else if((*this)[i] < bt[i])
                return false;
            --i;
        }
        return false;
    }
}
bool BigInt::operator>=(const BigInt &bt)const
{
    return !((*this)<bt);
}
bool BigInt::operator<=(const BigInt &bt)const
{
    return ((*this)>bt);
}
bool BigInt::operator==(const BigInt &bt)const
{
    Symbol symbol1 = GetSymbol();
    Symbol symbol2 = bt.GetSymbol();
    if(symbol1 != symbol2)
    {
        return false;
    }
    if(size() != bt.size())
        return false;
    int i = size();
    while(i > 0)
    {
        if(((*this)[i]) != bt[i])
            return false;
        --i;
    }
    return true;
}
bool BigInt::operator!=(const BigInt &bt)const
{
    return !((*this)==bt);
}

void BigInt::LoadData(u_long sz)
{
    //用时间生成随机种子，再生成随机大数
    clear();
    srand(time(NULL));
    for(u_long i=0; i<sz; ++i)
    {
        push_back(rand()%10);//随机生成0到9
    }
    clear_head_zero();
}
void BigInt::PrintData()
{
    if(GetSymbol() == NEGATIVE && *this != 0)
    {
        cout<<"-";
    }
    for(int i=size(); i>0; --i)
    {
        cout<<(int)(big[i]);
    }
    cout<<endl;
}
size_t BigInt::size()const
{
    return big.size();
}
BigInt BigInt::_Abs()const
{
    BigInt tmp = *this;
    if(this->GetSymbol() == NEGATIVE)
        tmp.SetSymbol(POSITIVE);
    return tmp;
}
void BigInt::S_Add(BigInt &res, const BigInt &bt1, const BigInt &bt2)
{
    if(bt1.GetSymbol() == bt2.GetSymbol())
    {
        Add(res, bt1, bt2);
        if(bt1.GetSymbol() == NEGATIVE)
            res.SetSymbol(NEGATIVE);
        else
            res.SetSymbol(POSITIVE);
    }
    else
    {
        if(bt1._Abs() == bt2._Abs())
        {
            res = 0;
            return;
        }
        if(bt1.GetSymbol() == NEGATIVE && bt2.GetSymbol() != NEGATIVE)
        {
            if(bt1._Abs() > bt2)
            {
                Sub(res, bt1, bt2);
                res.SetSymbol(NEGATIVE);
            }
            else
            {
                Sub(res, bt2, bt1);
            }
        }
        else if(bt2.GetSymbol() == NEGATIVE && bt1.GetSymbol() != NEGATIVE)
        {
            if(bt2._Abs() > bt1)
            {
                Sub(res, bt2, bt1);
                res.SetSymbol(NEGATIVE);
            }
            else
            {
                Sub(res, bt1, bt2);
            }
        }
    }
}
void BigInt::S_Sub(BigInt &res, const BigInt &bt1, const BigInt &bt2)
{
    BigInt tmp = bt2;
    if(bt2.GetSymbol() == POSITIVE)
        tmp.SetSymbol(NEGATIVE);
    else if(bt2.GetSymbol() == NEGATIVE)
        tmp.SetSymbol(POSITIVE);
    S_Add(res, bt1, tmp);
}
void BigInt::S_Mul(BigInt &res, const BigInt &bt1, const BigInt &bt2)
{
    Mul(res, bt1, bt2);
    if(bt1.GetSymbol() == bt2.GetSymbol())
        res.SetSymbol(POSITIVE);
    else
        res.SetSymbol(NEGATIVE);
}
void BigInt::S_Div(BigInt &res, const BigInt &bt1, const BigInt &bt2)
{
    Div(res, bt1, bt2);
    if(bt1.GetSymbol() == bt2.GetSymbol())
        res.SetSymbol(POSITIVE);
    else
        res.SetSymbol(NEGATIVE);
}
void BigInt::S_Mod(BigInt &res, const BigInt &bt1, const BigInt &bt2)
{
    Mod(res, bt1, bt2);
    res.SetSymbol(bt1.GetSymbol());
}
u_char BigInt::AddItem(u_char a, u_char b, u_char &carry)
{
    u_char res = a + b + carry;
    if(res >= 10)
    {
        res -= 10;
        carry = 1;
    }
    else
    {
        carry = 0;
    }
    return res;
}
void BigInt::Add(BigInt &res, const BigInt &bt1, const BigInt &bt2)
{
    //思路大致是：
    //1.从最低位开始，对应位置相加，和大于10的向高位进位，取个位数为该位置相加的和
    //2.对应位置相加时，要加上来自低位的进位值
    //3.n位数+m位数，其和最大为max(n,m)+1位数
    res.clear();
    u_char carry = 0;//进位
    u_char sum = 0;
    u_long i = 1;//bt1位
    u_long j = 1;//bt2位
    while(i <= bt1.size() && j <= bt2.size())
    {
        sum = AddItem(bt1[i++], bt2[j++], carry);
        res.push_back(sum);
    }
    while(i <= bt1.size())
    {
        sum = AddItem(bt1[i++], 0, carry);
        res.push_back(sum);
    }
    while(j <= bt2.size())
    {
        sum = AddItem(0, bt2[j++], carry);
        res.push_back(sum);
    }
    if(carry >= 1)
        res.push_back(carry);
    res.clear_head_zero();
}
u_char BigInt::SubItem(u_char a, u_char b, u_char &borrow)
{
    u_char res = 0;
    if(a < b + borrow)//a-borrow<b错误，如果a是0,borrow=1,会牵扯到负数的存储
    {
        res = a + 10 -b - borrow;
        borrow = 1;
    }
    else
    {
        res = a-borrow-b;
        borrow = 0;
    }
    return res;
}
void BigInt::Sub(BigInt &res, const BigInt &bt1, const BigInt &bt2)
{
    //解决思路:
    //1.从最低位开始，对应位置相减，不够减的向高位借位，得到的差作为该位置的差
    //2.对应位置相减时，要减去来自低位的借位值
    //3.n位数-m位数，其和最大为n位数
    res.clear();
    if(bt1<bt2)
    {
        cout<<"结果为负数，程序员正在开发中..."<<endl;
        exit(EXIT_FAILURE);
    }
    if(bt1 == bt2)
    {
        res.push_back(0);
        return;
    }
    u_char differ = 0;
    u_char borrow = 0;
    u_long i = 1;
    u_long j = 1;

    while(i <= bt1.size() && j <= bt2.size())
    {
        differ = SubItem(bt1[i++], bt2[j++], borrow);
        res.push_back(differ);
    }
    while(i <= bt1.size())
    {
        differ = SubItem(bt1[i++], 0, borrow);
        res.push_back(differ);
    }
    while(j <= bt2.size())
    {
        differ = SubItem(0, bt2[j++], borrow);
        res.push_back(differ);
    }
    res.clear_head_zero();
}
void BigInt::MulItem(BigInt &res, const BigInt &bt1, const u_char bt2)
{
    u_char sign = 0;//进位
    u_char pro = 0;
    for(u_long i=1; i<=bt1.size(); ++i)
    {
        pro = bt1[i] * bt2 + sign;
        sign = pro/10;
        pro %= 10;
        res.push_back(pro);
    }
    if(sign > 0)
        res.push_back(sign);
}
void BigInt::AddMove(BigInt &res, const BigInt &bt, u_long pos)
{
    u_long i = pos;
    u_long j = 1;
    u_char sign = 0;
    while(i<=res.size() && j<=bt.size())
    {
        res[i] = BigInt::AddItem(res[i], bt[j], sign);
        i++;
        j++;
    }
    while(i<=res.size())
    {
        res[i] = BigInt::AddItem(res[i], 0, sign);
        i++;
    }
    while(j<=bt.size())
    {
        u_char add = BigInt::AddItem(0, bt[j++],sign);
        res.push_back(add);
    }
    if(sign > 0)
        res.push_back(sign);
}
void BigInt::Mul(BigInt &res, const BigInt &bt1, const BigInt &bt2)
{
    //思路：计算bt1与bt2中的每一位相乘的结果，切记要处理有进位
    //将所得结果 与上一位的所得结果进行相加，处理竖式的位置
    //  123
    //*  12
    //  ————
    //  246
    // 123
    // ——————
    // 1476
    res.clear();
    BigInt tmp;
    for(u_long i=1; i<=bt2.size(); ++i)
    {
        tmp.clear();
        MulItem(tmp, bt1, bt2[i]);//大数bt1与bt2的每一位相乘
        AddMove(res, tmp, i);//res和tmp按照位置相加
    }
    res.clear_head_zero();
}

void BigInt::Div(BigInt &res, const BigInt &bt1, const BigInt &bt2)
{
    //判断两个数的大小，如果bt1<bt2,直接尾插0；如果bt1==bt2,直接尾插1
    res.clear();
    if(bt1 < bt2)
    {
        res.push_back(0);
        return;
    }
    else if(bt1 == bt2)
    {
        res.push_back(1);
        return;
    }
    else
    {
        size_t bt1_len = bt1.size();
        size_t bt2_len = bt2.size();
        long k = bt1_len - bt2_len;      //得到bt1比bt2大多少位,不能定义成size_t 在64位底下，它是unsigned long类型
        BigInt btd;                        //被除数
        //取出bt1比bt2大的部分
        btd.clear();
        for(size_t i=1; i<=bt2.size(); ++i)
        {
            btd.push_back(bt1[i+k]);
        }
        //用对应的btd求每一位上的商
        u_char div = 0;
        while(k >= 0)                      //k< 0表示已经除尽，就可以取出res
        {
            btd.clear_head_zero();     //030 < 50,但是，如果不清除头部的0，-=调用减法运算时，030由于位数多，造成仍大于50的假象
            while(btd >= bt2)                 //如果被除数>=除数是一种情况，而当被除数<除数，则要执行下面一种情况
            {
                btd -= bt2;
                div++;
                btd.clear_head_zero();     //030 < 50,但是，如果不清除头部的0，-=调用减法运算时，030由于位数多，造成仍大于50的假象
            }
            res.push_front(div);           //获得每一位上的商
            div = 0;
            if(k > 0)
                btd.push_front(bt1[k]);         //将余数与被除数的下一位结合，继续求商
            --k;
        }
        res.clear_head_zero();
    }
}
void BigInt::Mod(BigInt &res, const BigInt &bt1, const BigInt &bt2)
{
    //得到除法运算最终的余数就OK
    res.clear();
    BigInt btd;         //实时的被除数
    btd.clear();
    if(bt1 < bt2)
    {
        res = bt1;
        return;
    }
    else if(bt1 == bt2)
    {
        res = 0;
        return;
    }
    else
    {
        size_t bt1_len = bt1.size();
        size_t bt2_len = bt2.size();
        long k = bt1_len - bt2_len;      //得到bt1比bt2大多少位,不能定义成size_t 在64位底下，它是unsigned long类型
        //取出bt1比bt2大的部分
        btd.clear();
        for(size_t i=1; i<=bt2.size(); ++i)
        {
            res.push_back(bt1[i+k]);
        }
        //用对应的btd求每一位上的商
        while(k >= 0)                      //k< 0表示已经除尽，就可以取出res
        {
            while(res >= bt2)                 //如果被除数>=除数是一种情况，而当被除数<除数，则要执行下面一种情况
            {
                res -= bt2;
                res.clear_head_zero();     //030 < 50,但是，如果不清除头部的0，-=调用减法运算时，030由于位数多，造成仍大于50的假象
            }
            if(k > 0)
                res.push_front(bt1[k]);         //将余数与被除数的下一位结合，继续求商
            --k;
        }
        res.clear_head_zero();
    }
}
void BigInt::Square(BigInt &res, const BigInt &bt1)                //res = bt1^2
{
    S_Mul(res, bt1, bt1);
}
void BigInt::S_PowerMod(BigInt &res,BigInt a, BigInt b, BigInt n)
{
    PowerMod(res, a, b, n);
    if(b & 1)
        res.SetSymbol(a.GetSymbol());
    else
        res.SetSymbol(POSITIVE);
}
//剑指offer P93
void BigInt::S_Power(BigInt &res, BigInt &bt1, BigInt &bt2)
{
    res = Power(bt1, bt2);
    if(bt2 & 1)
        res.SetSymbol(bt1.GetSymbol());
    else
        res.SetSymbol(POSITIVE);
}
BigInt BigInt::Power(BigInt &bt1, BigInt bt2)
{
    if(bt2 == 0)
    {
        return 1;
    }
    if(bt2 == 1)
        return bt1;
    BigInt res = Power(bt1, bt2/2);
    res *= res;
    if(bt2 & 0x1)
        res *= bt1;
    return res;
}
//#if 0//用快速幂求次方，效果慢???
//BigInt BigInt::Power(BigInt &bt1, BigInt &bt2)
//{
//    BigInt res = 1;
//    BigInt res1;
//    while(bt2 > 0)
//    {
//         if(bt2&1)
//         {
//             res *= bt1;
//         }
//         bt1 = bt1*bt1;
//         bt2 = bt2 / 2;
//    }
//    res.clear_head_zero();
//    return res;
//}
//#endif
void BigInt::PowerMod(BigInt &res,BigInt a, BigInt b, BigInt n) //res = a^b%n,为RSA加密算法作准备
{
    //蒙哥马利算法
    //快速幂取模就是在O(logn)内求出a^n mod b的值。算法的原理是ab mod c=(a mod c)(b mod c)mod c
#if 0
    BigInt tmp;
    Power(tmp, a, b);
    Mod(res, tmp, n);
#endif
#if 1
    res = 1;
    a %= n;
    while(b > 0)
    {
        if(b & 1)
            res = (res * a) % n;
        a = (a*a) % n;
        b = b / 2;
    }
    res.clear_head_zero();
#endif
}
