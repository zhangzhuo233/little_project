#include"utili.h"
//#include"config.h"
#include"BigInt.h"

#ifndef RAND
int main(int argc, char const* argv[])
{

}

#else
int main(int argc, char const* argv[])
{
    BigInt bt1, bt2, bt3, res;
    bt1.LoadData(2);
    sleep(1);
    bt2.LoadData(2);
    bt3.LoadData(5);
    cout<<bt1<<endl;
    cout<<bt2<<endl;
    //cout<<bt3<<endl;
    res = BigInt::Power(bt1, bt2);
    //BigInt::Power(res, bt1, bt2);
    //BigInt::Mod(res, bt1, bt2);
    //BigInt::PowerMod(res, bt1, bt2, bt3);
    cout<<res<<endl;
    return 0;
}

#endif
