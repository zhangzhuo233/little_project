/* stack.h , Version 1.0. (See source code in here
 * Author: zhangzhuo
 * $Date: 2017-01-24 17:07:24 -0500 (Tue, 24 Jan 2017) $
 */
#ifndef _SEQSTACK_H_
#define _SEQSTACK_H_

#include"utili.h"
template<typename Type>
class SeqStack
{
public:
    SeqStack(int top=0, size_t capacity=10) : top(top), capacity(capacity)      //构造函数
    {
        base = new Type[capacity];
        if(NULL == base)
        {
            cout<<"Application Error!"<<endl;
            exit(EXIT_FAILURE);
        }
    }
    ~SeqStack()                                 //析构函数
    {
         delete[] base;
    }
public:
    void Push(const Type item);                 //入栈
    Type Pop();                                 //出栈
    Type GetTop()const;                         //获取栈顶元素,函数体内不能对成员数据做任何改动
    void Show();                                //打印
    void clear()                                //清空栈
    {
         top = 0;
    }
    BOOL IsEmpty()const                         //判空
    {
        return top==0;
    }
    BOOL IsFull()const                          //叛满
    {
         return top==capacity-1;
    }
private:
    int top;        //栈顶
    Type *base;     //指向栈
    int capacity;    //栈大小
};

template<typename Type>
void SeqStack<Type>::Push(const Type item)
{
    if(IsFull())
    {
        cout<<"The stack is full!"<<item<<"can not push!"<<endl;
        return;
    }
    base[top++] = item;
}
template<typename Type>
Type SeqStack<Type>::Pop()
{
    if(IsEmpty())
    {
         cout<<"There is no element!"<<endl;
         exit(EXIT_FAILURE);
    }
    return base[top--];
}
template<typename Type>
Type SeqStack<Type>::GetTop()const
{
    if(IsEmpty())
    {
         cout<<"There is no element!"<<endl;
         exit(EXIT_FAILURE);
    }
    return base[top-1];
}
template<typename Type>
void SeqStack<Type>::Show()
{
    for(int i=0;i<top;++i)
    {
         cout<<base[i]<<"-->";
    }
    cout<<"top"<<endl;
}
#endif
