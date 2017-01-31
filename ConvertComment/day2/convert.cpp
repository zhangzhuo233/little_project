#include"ConvertComment.h"
#include"BrowseDir.h"

int main(int argc, char *argv[])
{
    //操作选择
    int opt = -1;
    while((opt = getopt(argc, argv, "s::S::r::R::h::H::")) != -1)       //::代表后面可跟字母或者也可不跟
    {
        switch(opt)
        {
            case 'S':
            case 's':
                SeeArgc(argc);
                OneFileCovert(argv[3], argv[2]);
                break;
            case 'R':
            case 'r':
                SeeArgc(argc);
                DirConvert(argv[3], argv[2]);
                break;
            case 'H':
            case 'h':
                help();
                break;
            default:
                cout<<"Usage: ./convert [-Option] src dest"<<endl;
                cout<<"[Option]: S/R/H"<<endl;
                break;
        }
    }
    SeeArgc(argc);
    return 0;
}
