#include"BraceMatch.h"
int main()
{
    char buffer[20] = {'\0'};
    cout<<"input str：";
    cin>>buffer;
    if(BraceMatch(buffer))
        cout<<"Match"<<endl;
    else
        cout<<"Not Match"<<endl;
    return 0;
}
