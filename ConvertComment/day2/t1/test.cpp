#include<iostream>
#include<string.h>
using namespace std;
void IsCFile(char *str)
{
    int len = strlen(str);
    if(str[len-1] == 'c' && str[len-2] == '.')
    {
        cout<<"yes!"<<endl;
    }
    else
        cout<<"no!"<<endl;
}
int main(int argc, char const* argv[])
{
    char *str = "5.cpp";
    IsCFile(str);
    return 0;
}
/**/
