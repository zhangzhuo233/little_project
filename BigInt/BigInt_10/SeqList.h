#ifndef _SEQLIST_H
#define _SEQLIST_H_

template<class Type>
class SeqList
{
    public:
        typedef Type& reference;
        typedef const Type& const_reference;
    public:
        SeqList(size_t sz = DEFAULT_SIZE);
        ~SeqList();
    public:
        void SetSymbol(Symbol sign);
        Symbol GetSymbol()const;
    public:
        void push_back(const_reference x);
        void push_front(const_reference x);
        void pop_back();
        void pop_front();
        reference front();
        reference back();
        const_reference front()const;
        const_reference back()const;
        size_t size()const;
        void reverse();
        void clear();
        void sort();
        int find(const_reference key);
        void erase(const_reference key);//删除第一个元素
        void swap(reference v1, reference v2);
        void show();
    public:
        reference operator[](int pos);
        const_reference operator[](int pos)const;
        bool Inc();                 //空间配置器

    public:
        SeqList(const SeqList<Type> &bt);//拷贝构造和赋值函数声明成保护
        SeqList& operator=(const SeqList<Type> &st);
    private:
        enum{DEFAULT_SIZE = 10, INC_SIZE = 5};
        Type *base;
        size_t capacity;
        size_t len;
};

template<class Type>
SeqList<Type>::SeqList(const SeqList<Type> &bt)
{
    capacity = bt.capacity;
    base = new Type[sizeof(Type) * (capacity+1)];
    memset(base, 0, sizeof(Type)*(capacity+1));
    memcpy(base, bt.base, sizeof(bt.len+1));
}
    template<class Type>
SeqList<Type>& SeqList<Type>::operator=(const SeqList<Type> &st)
{
    if(this != &st)//防止自赋值
    {
        if(capacity < st.len)
        {
            delete []base;
            base = new Type[st.capacity+1];
            capacity = st.capacity;
        }
        memcpy(base, st.base, sizeof(Type)*(st.len+1));
        len = st.len;
    }
    return *this;
}
    template<class Type>
void SeqList<Type>::SetSymbol(Symbol sign)
{
    base[0] = sign;
}
template<class Type>
Symbol SeqList<Type>::GetSymbol()const
{
    return (Symbol)base[0];
}
    template<class Type>
SeqList<Type>::SeqList(size_t sz)       //实现不能带默认参数
{
    capacity = sz>DEFAULT_SIZE ? sz : DEFAULT_SIZE;
    base = new Type[capacity+1];
    memset(base, 0, sizeof(Type) * (capacity+1));
    len = 0;
}
    template<class Type>
SeqList<Type>::~SeqList()
{
    delete []base;
    base = NULL;
    capacity = len = 0;
}
    template<class Type>
void SeqList<Type>::push_back(const_reference x)
{
    if(len >= capacity && !Inc())
        return;
    base[++len] = x;
}
    template<class Type>
void SeqList<Type>::push_front(const_reference x)
{
    if(len >= capacity && !Inc())
        return;
    int i=len;
    for(; i>0; --i)
    {
        base[i+1] = base[i];
    }
    base[1] = x;
    len++;
}
    template<class Type>
void SeqList<Type>::pop_back()
{
    assert(len > 0);
    --len;
}
    template<class Type>
void SeqList<Type>::pop_front()
{
    assert(len > 0);
    int i=1;
    for(; i<len; ++i)
    {
        base[i] = base[i+1];
    }
    --len;
}
    template<class Type>
typename SeqList<Type>::reference SeqList<Type>::front()
{
    assert(len > 0);
    return base[1];
}
    template<class Type>
typename SeqList<Type>::reference SeqList<Type>::back()
{
    assert(len > 0);
    return base[len];
}
template<class Type>
typename SeqList<Type>::const_reference SeqList<Type>::front()const
{
    assert(len > 0);
    return base[1];
}
template<class Type>
typename SeqList<Type>::const_reference SeqList<Type>::back()const
{
    assert(len > 0);
    return base[len];
}
template<class Type>
size_t SeqList<Type>::size()const
{
    return len;
}
    template<class Type>
void SeqList<Type>::swap(reference v1, reference v2)
{
    Type tmp;
    tmp = v1;
    v1 = v2;
    v2 = tmp;
}
    template<class Type>
void SeqList<Type>::reverse()
{
    //就地逆置1和len,2和len-1
    int low = 1;
    int high = len;
    while(low < high)
    {
        swap(base[low], base[high]);
        low++;
        high--;
    }
}
    template<class Type>
void SeqList<Type>::clear()
{
    if(len<1)
        return;
    memset(base, 0, sizeof(Type)*(capacity+1));
    len = 0;
}
    template<class Type>
void SeqList<Type>::sort()
{
    if(len == 1)
        return;
    //插入排序
    int minIndex = 0;
    for(int i=1; i<len-1; ++i)//只需要比较len-1次
    {
        minIndex = i;
        for(int j=i+1; j<len; ++j)//从i+1比较，因为minIndex已经默认为1
        {
            if(base[j] < base[minIndex])
                minIndex = j;
        }
        if(minIndex != i)
            swap(base[i], base[minIndex]);
    }
}
    template<class Type>
int SeqList<Type>::find(const_reference key)
{
    for(int i=1; i<len; ++i)
    {
        if(base[i] == key)
            return i;
    }
    return -1;
}
    template<class Type>
void SeqList<Type>::erase(const_reference key)
{
    int n = find(key);
    for(int i=n; i<len; ++i)
    {
        base[i] = base[i+1];
    }
    len--;
}
    template<class Type>
void SeqList<Type>::show()
{
    for(int i=1; i<=len; ++i)
    {
        cout<<base[i]<<'\t';
    }
    cout<<endl;
}

    template<class Type>
typename SeqList<Type>::reference SeqList<Type>::operator[](int pos)
{
    assert(pos>0 && pos<=len);
    return base[pos];
}
template<class Type>
typename SeqList<Type>::const_reference SeqList<Type>::operator[](int pos)const
{
    assert(pos>0 && pos<=len);
    return base[pos];
}
    template<class Type>
bool SeqList<Type>::Inc()
{
    //用Inc重新分配空间
    //为什么不用realloc，析构函数用delete,realloc对应的是free
    capacity+=INC_SIZE;
    Type *new_base = new Type[capacity+1];
    if(new_base == NULL)
    {
        cout<<"Out of memory"<<endl;
        return false;
    }
    memset(new_base, 0, sizeof(Type)*(capacity+1));
    memcpy(new_base, base, sizeof(Type)*(capacity-INC_SIZE+1));
    delete []base;
    base = new_base;
    return true;
}

#endif
