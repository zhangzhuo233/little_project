#include"utili.h"

extern "C" int ConvertComment(FILE *inputfile, FILE *outputfile);
//���˺�����C���б��룬main.cpp��convertcomment.cppǿ�Ʊ�������Ҫ�޸���ĺ����������õ���ͬһ������
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
    STATE_ENUM prestate;        //��¼��ǰ��״̬,�����ַ���ǰ���м�¼
}State_Machine;

///////////////////////////////////////////////////////////
State_Machine  g_state;

void EventPro(char ch);         //�¼�����
void EventProAtNo(char ch);     //��ע��״̬
void EventProAtC(char ch);      //C����״̬
void EventProAtCpp(char ch);    //Cpp״̬
void EventProAtCpp(char ch);    //Cpp״̬
void EventProAtStr(char ch);    //�ַ���״̬
char readchr();                 //��ȡһ���ַ���
void Write_One_Data(char ch);        //����д��һ���ַ�
void Write_Data(char ch1, char ch2);//д�������ַ�����

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
            fseek(g_state.inputfile, -1L, SEEK_CUR);//������*�����һ�����ó���/ƥ������,eg./***/
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
