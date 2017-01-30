#ifndef _BRACEMATCH_
#define _BRACEMATCH_

#include"SeqStack.h"
BOOL BraceMatch(char *str)
{
    SeqStack<char> st(0, 20);
    while(*str != '\0')
    {
        switch(*str)
        {
        case '[':
        case '(':
        case '{':
            st.Push(*str);
            break;
        case ']':
            if((st.GetTop()) != '[')
                return FALSE;
            else
                st.Pop();
            break;
        case ')':
            if((st.GetTop()) != '(')
                return FALSE;
            else
                st.Pop();
            break;
        case '}':
            if((st.GetTop()) != '{')
                return FALSE;
            else
                st.Pop();
            break;
        default:
            break;
        }
        str++;
    }
    if((st.IsEmpty()) && (*str=='\0'))
        return TRUE;
}
#endif
