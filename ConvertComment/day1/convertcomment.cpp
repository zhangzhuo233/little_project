#include"utili.h"

extern "C" int ConvertComment(FILE *inputfile, FILE *outputfile);
//将此函数以C进行编译，main.cpp和convertcomment.cpp强制编译器不要修改你的函数名，调用的是同一个函数
typedef enum
{
    NO_COMMENT_STATE,
    C_COMMENT_STATE,
    CPP_COMMENT_STATE,
    STR_COMMENT_STATE,
    END_COMMENT_STATE
}STATE_ENUM;

typedef struct
{
    FILE *inputfile;
    FILE *outputfile;
    STATE_ENUM state;
    STATE_ENUM prestate;        //记录先前的状态,进入字符串前进行记录
}State_Machine;

///////////////////////////////////////////////////////////
State_Machine  g_state;

void EventPro(char ch);         //事件处理
void EventProAtNo(char ch);     //无注释状态
void EventProAtC(char ch);      //C语言状态
void EventProAtCpp(char ch);    //Cpp状态
void EventProAtCpp(char ch);    //Cpp状态
void EventProAtStr(char ch);    //字符串状态
char readchr();                 //读取一个字符串
void Write_One_Data(char ch);        //用来写入一个字符
void Write_Data(char ch1, char ch2);//写入两个字符函数

///////////////////////////////////////////////////////////
int ConvertComment(FILE *inputfile, FILE *outputfile)
{
    if(inputfile==NULL || outputfile==NULL)
    {
        cout<<"Argument is invalid."<<endl;
        return -1;
    }

    ///////////////////////////////////////////////////
    g_state.inputfile = inputfile;
    g_state.outputfile = outputfile;
    g_state.state = NO_COMMENT_STATE;
    g_state.prestate = NO_COMMENT_STATE;

    char ch;
    while(g_state.state != END_COMMENT_STATE)
    {
        ch = readchr();
        EventPro(ch);
    }

    return 0;
}

void EventPro(char ch)
{
    switch(g_state.state)
    {
    case NO_COMMENT_STATE:
        EventProAtNo(ch);
        break;
    case C_COMMENT_STATE:
        EventProAtC(ch);
        break;
    case CPP_COMMENT_STATE:
        EventProAtCpp(ch);
        break;
    case STR_COMMENT_STATE:
        EventProAtStr(ch);
        break;
    default:
        break;
    }
}

void EventProAtNo(char ch)
{
    char nextch;
    switch(ch)
    {
    case '\n':
            Write_One_Data('\n');
            break;
    case '/':
        nextch = readchr();
        if(nextch == '/')   //  //
        {
            Write_Data(ch, '*');
            g_state.state = CPP_COMMENT_STATE;
        }
        else if(nextch == '*') // /*
        {
            Write_Data(ch, nextch);
            g_state.state = C_COMMENT_STATE;
        }
        else
            Write_Data(ch, nextch);
        break;
    case '*':
        nextch = readchr();
        if(nextch == '/')
            Write_Data(' ', ' ');
        else
            Write_Data(ch, nextch);
        break;
    case '\"':
        Write_One_Data(ch);
        g_state.state = STR_COMMENT_STATE;
        g_state.prestate = NO_COMMENT_STATE;
        break;
    case EOF:
        g_state.state = END_COMMENT_STATE;
        break;
    default:
        Write_One_Data(ch);
        break;
    }
}
void EventProAtC(char ch)
{
    char nextch;
    switch(ch)
    {
    case '*':
        nextch = readchr();
        if(nextch == '/')
        {
            Write_Data('*', '/');
            g_state.state = NO_COMMENT_STATE;
            g_state.prestate = NO_COMMENT_STATE;
        }
        else if(nextch == '*')
        {
            Write_One_Data(ch);
            fseek(g_state.inputfile, -1L, SEEK_CUR);//奇数个*，最后一个设置成与/匹配的情况,eg./***/
        }
        else
            Write_Data(ch, nextch);
            break;
    case '\n':
        if(g_state.prestate == CPP_COMMENT_STATE)
        {
            Write_Data('*', '/');
            g_state.state = NO_COMMENT_STATE;
        }
        else
            Write_One_Data('\n');
        break;
    case '\"':
        Write_One_Data(ch);
        g_state.state = STR_COMMENT_STATE;
        g_state.prestate = C_COMMENT_STATE;
        break;
    case '/':
        nextch = readchr();
        if(nextch == '/')
        {
            Write_Data(' ', ' ');
        }
        else
        {
                Write_Data(ch, nextch);
        }
        break;
    case EOF:
        g_state.state = END_COMMENT_STATE;
        break;
    default:
        Write_One_Data(ch);
        break;
    }
}
void EventProAtCpp(char ch)
{
    char nextch;
    switch(ch)
    {
    case '\n':
        Write_Data('*', '/');
            Write_One_Data('\n');
            g_state.state = NO_COMMENT_STATE;
            g_state.prestate = NO_COMMENT_STATE;
        break;
    case '\"':
        Write_One_Data(ch);
        g_state.state = STR_COMMENT_STATE;
        g_state.prestate = CPP_COMMENT_STATE;
        break;
    case '/':
        nextch = readchr();
        if(nextch == '/')
        {
            Write_Data(' ', ' ');
        }
        else if(nextch == '*')
        {
            Write_Data(' ', ' ');
        }
        else
        {
            fseek(g_state.inputfile, -3L, SEEK_CUR);
            char bak_3 = readchr();
            if(bak_3 == '/')
                Write_One_Data(' ');
            else
                Write_One_Data(ch);
            fseek(g_state.inputfile, +1L, SEEK_CUR);
        }
        g_state.prestate = CPP_COMMENT_STATE;
        break;
    case '*':
        nextch = readchr();
        if(nextch == '/')
        {
            if(g_state.prestate == CPP_COMMENT_STATE)
            {
                Write_Data(' ', ' ');
            }
            else
            {
                Write_Data(ch, nextch);
                g_state.state = NO_COMMENT_STATE;
                g_state.prestate = CPP_COMMENT_STATE;
            }
        }
        else
            Write_Data(ch, nextch);
        break;
    case EOF:
        Write_Data('*', '/');
        g_state.state = END_COMMENT_STATE;
        break;
    default:
        Write_One_Data(ch);
        break;
    }
}
void EventProAtStr(char ch)
{
    char nextch;
    switch(ch)
    {
        case '\"':
            Write_One_Data(ch);
            g_state.state = g_state.prestate;
            break;
        case EOF:
            g_state.state = END_COMMENT_STATE;
            break;
        default:
            Write_One_Data(ch);
            break;
    }
}
char readchr()
{
     fgetc(g_state.inputfile);
}
void Write_One_Data(char ch)
{
    fputc(ch, g_state.outputfile);
}
void Write_Data(char ch1, char ch2)
{
    fputc(ch1, g_state.outputfile);
    fputc(ch2, g_state.outputfile);
}
