#include <iostream>
#include <string.h>
#include <cxxabi.h>
#include <experimental/source_location>

using namespace std;
using std::experimental::source_location;


typedef struct
{
    float x, y;
}csPOINT2D;

template<class TYPE> struct csLIST_DATA
{
    int position;
    TYPE data;
};

template<class TYPE> struct csLFT
{
    int position;
    TYPE* data;
    int begin;
    int size;
};

struct csTABCOORDS
{
    int position;
    int size;
};


template<class TYPE> class csLIST
{
    private:
        TYPE*DataTab;
        int memSize, N, inc;
    public:
    void (*printAll_v_func)(TYPE*datas, int size, const char*title);
    void (*print_v_func)(TYPE*datas, int id, const char*title);
    csLIST()
    {
        DataTab=0;
        memSize=0;
        N=0;
        inc=2;
    }
    /*~csLIST()
    {
        free(DataTab);
        DataTab = 0;
    }*/
    int size()
    {
        return N;
    }
    void setPrintVFunc(void (*f)(void))
    {
        print_v_func=f;
    }
    TYPE & operator[](int i)
    {
        return DataTab[i];
    }
    TYPE* getTable()
    {
        return DataTab;
    }
    void init(int size, const source_location loc = source_location::current())
    {
        if(size>0)
        {   N=0;
            DataTab = (TYPE*)malloc(size*sizeof(TYPE));
            if(!DataTab)
                std::cout<<"(Line "<<loc.line()<<") Operation failed ! Memory allocation error.\n";
            else
                memSize=size;
        }
        else
            std::cout<<"(Line "<<loc.line()<<") Operation failed ! The size most be upper than 0!\n";
    }
    void addBottom(TYPE data, const source_location loc = source_location::current())
    {
        if(N>=memSize)
        {   memSize+=inc;
            TYPE* tmp=(TYPE*)realloc(DataTab, memSize*sizeof(TYPE));
            if(!tmp)
                std::cout<<"(Line "<<loc.line()<<") Operation failed ! Memory allocation error.\n";
            else
            {
                DataTab=tmp;
                goto g;
            }
        }
        else goto g;

        g :
            {DataTab[N++]=data;}
    }
    void addBottom2(TYPE data, size_t memsz, const source_location loc = source_location::current())
    {
        if(N>=memSize)
        {   memSize+=memsz;
            TYPE* tmp=(TYPE*)realloc(DataTab, memSize*sizeof(TYPE));
            if(!tmp)
                std::cout<<"(Line "<<loc.line()<<") Operation failed ! Memory allocation error.\n";
            else
            {
                DataTab=tmp;
                goto g;
            }
        }
        else goto g;

        g :
            {DataTab[N++]=data;}
    }
    void addTop(TYPE data, const source_location loc = source_location::current())
    {
        if(N>=memSize)
        {   memSize+=inc;
            TYPE* tmp=(TYPE*)realloc(DataTab, memSize*sizeof(TYPE));
            if(!tmp)
            {
                std::cout<<"(Line "<<loc.line()<<") Operation failed ! Memory allocation error.\n";
            }
            else
            {
                DataTab=tmp;
                goto g;
            }
        }
        else goto g;

        g:
            {
                for(int i=N; i>=0; i--)
                    DataTab[i+1]=DataTab[i];

                DataTab[0]=data;
                N++;
            }
    }
    void add(TYPE data, int position, const source_location loc = source_location::current())
    {
        if(position<=N)
        {   if(N>=memSize)
            {   memSize+=inc;
                TYPE* tmp=(TYPE*)realloc(DataTab, memSize*sizeof(TYPE));
                if(!tmp)
                    std::cout<<"(Line "<<loc.line()<<") Operation failed ! Memory allocation error.\n";
                else
                {
                    DataTab=tmp;
                    goto g;
                }
            }
            else goto g;
            g:
                {
                    for(int i=N; i>=position; i--)
                    DataTab[i+1]=DataTab[i];

                    DataTab[position]=data;
                    N++;
                }
        }
        else
        {
            memSize+=position-N+1;
            TYPE* tmp=(TYPE*)realloc(DataTab, memSize*sizeof(TYPE));
            if(!tmp)
                std::cout<<"(Line "<<loc.line()<<") Operation failed ! Memory allocation error.\n";
            else
            {
                DataTab=tmp;
                DataTab[position]=data;
                N = position +1;
                std::cout<<"(Line "<<loc.line()<<") Warning : Position larger than table size.\n";
                std::cout<<"This could cause problems!\n";
            }

        }
    }
    void addFromTable(int position, TYPE* data, int data_pos, int size, const source_location loc = source_location::current())
    {
        if(position<=N)
        {   if(N+size>=memSize)
            {   memSize=N+size+1;
                TYPE* tmp=(TYPE*)realloc(DataTab, memSize*sizeof(TYPE));
                if(!tmp)
                    std::cout<<"(Line "<<loc.line()<<") Operation failed ! Memory allocation error.\n";
                else
                {
                    DataTab=tmp;
                    goto g;
                }
            }
            else goto g;
            g:
            {
                for(int i=N; i>=position; i--)
                DataTab[i+size]=DataTab[i];

                int sz=position+size;
                for(int i=position, j=data_pos; i<sz; i++, j++)
                    DataTab[i]=data[j];
                N+=size;
            }
        }
        else
        {
            memSize+=position-N+size+1;
            TYPE* tmp=(TYPE*)realloc(DataTab, memSize*sizeof(TYPE));
            if(!tmp)
                std::cout<<"(Line "<<loc.line()<<") Operation failed ! Memory allocation error.\n";
            else
            {
                DataTab=tmp;
                int sz=position+size, pos = data_pos-position;
                for(int i=position, j=data_pos; i<sz; i++, j++)
                    DataTab[i]=data[j];
                N = position + size;
                std::cout<<"Warning : Position larger than table size.\n";
                std::cout<<"This could cause problems!\n\n";
            }
        }
    }
    void remBottom(const source_location loc = source_location::current())
    {
        TYPE* tmp=(TYPE*)realloc(DataTab, (N-1)*sizeof(TYPE));
        if(!tmp)
            std::cout<<"(Line "<<loc.line()<<") Operation failed ! Memory allocation error.\n";
        else
        {
            DataTab=tmp;
            memSize=--N; //memSize=N--;
        }

    }
    void remTop(const source_location loc = source_location::current())
    {
        for(int i=0; i<N-1; i++)
        {   DataTab[i]=DataTab[i+1];
        }
        TYPE* tmp=(TYPE*)realloc(DataTab, (N-1)*sizeof(TYPE));
        if(!tmp)
            std::cout<<"(Line "<<loc.line()<<") Operation failed ! Memory allocation error.\n";
        else
        {
            DataTab=tmp;
            memSize=--N;
        }
    }
    void rem(int position, const source_location loc = source_location::current())
    {
        if(position<N)
        {   for(int i=position; i<N-1; i++)
                DataTab[i]=DataTab[i+1];

            TYPE* tmp=(TYPE*)realloc(DataTab, (N-1)*sizeof(TYPE));
            if(!tmp)
                std::cout<<"(Line "<<loc.line()<<") Operation failed ! Memory allocation error.\n";
            else
            {
                DataTab=tmp;
                memSize=--N;
            }
        }
        else
            std::cout<<"(Line "<<loc.line()<<") Operation failed ! Position larger than table size.\n";
    }
    void remTopSection(int size, const source_location loc = source_location::current())
    {
        if(size<N)
        {
            for(int i=size; i<N; i++)
                DataTab[i-size]=DataTab[i];

            TYPE* tmp=(TYPE*)realloc(DataTab, (N-size)*sizeof(TYPE));
            if(!tmp)
                std::cout<<"(Line "<<loc.line()<<") Operation failed ! Memory allocation error.\n";
            else
            {
                DataTab=tmp;
                memSize=(N-=size);
            }
        }
        else
            std::cout<<"(Line "<<loc.line()<<") Operation failed ! Position larger than table size.\n";
    }
    void remFrom(int position, int size, const source_location loc = source_location::current())
    {
        int sz=position+size;
        if(sz<N)
        {
            for(int i=sz; i<N; i++)
                DataTab[i-size]=DataTab[i];

            TYPE* tmp=(TYPE*)realloc(DataTab, (N-size)*sizeof(TYPE));
            if(!tmp)
                std::cout<<"(Line "<<loc.line()<<") Operation failed ! Memory allocation error.\n";
            else
            {
                DataTab=tmp;
                memSize=(N-=size);
            }
        }
        else
        {
            if(position==0&&size>=N)
                clear();
            else
                std::cout<<"(Line "<<loc.line()<<") Operation failed ! Position larger than table size.\n";
        }
    }
    void minMaxSort(const source_location loc = source_location::current())
    {
        if(N>0)
        {
            TYPE t;
            for(int j=0; j<N; j++)
            for(int i=1; i<N; i++)
            {
                if(DataTab[i]<DataTab[i-1])
                {
                    t = DataTab[i-1];
                    DataTab[i-1]=DataTab[i];
                    DataTab[i]=t;
                }
            }
        }
        else
            std::cout<<"(Line "<<loc.line()<<") Empty list connot be sorted !\n";
    }
    void maxMinSort(const source_location loc = source_location::current())
    {
        if(N>0)
        {
            TYPE t;
            for(int j=0; j<N; j++)
            for(int i=1; i<N; i++)
            {
                if(DataTab[i]>DataTab[i-1])
                {
                    t = DataTab[i-1];
                    DataTab[i-1]=DataTab[i];
                    DataTab[i]=t;
                }
            }
        }
        else
            std::cout<<"(Line "<<loc.line()<<") Empty list connot be sorted !\n";
    }
    void print(int i, const source_location loc = source_location::current())
    {
        if(i<0 || i>= N)
            std::cout<<"(Line "<<loc.line()<<") Invalid identifier !\n";
        else
            std::cout<<DataTab[i]<<" ";
    }
    void printAll(const char*title="")
    {
        std::cout<<title<<"\n{ ";
        for(int i=0; i<N; i++)
            std::cout<<DataTab[i]<<", ";
        std::cout<<"}\n";
    }
    void print_v(int id, const char*title="")
    {
        print_v_func(DataTab,id,title);
    }
    void printAll_v(const char*title="")
    {
        printAll_v_func(DataTab,N,title);
    }
    void operator+=(TYPE data)
    {
        addBottom(data);
    }
    void operator+=(csLIST_DATA<TYPE> data)
    {
        add(data.data,data.position);
    }
    void operator+=(csLFT<TYPE> data)
    {
        addFromTable(data.position,data.data,data.begin,data.size);
    }
    void operator--()
    {
        remBottom();
    }
    void operator-=(int i)
    {
        rem(i);
    }
    void operator-=(csTABCOORDS coords)
    {
        remFrom(coords.position, coords.size);
    }
    void clear()
    {
        free(DataTab);
        DataTab=NULL;
        N=0;
        memSize=0;
    }
};

void csPrintAll_POINT2D(csPOINT2D*datas, int size, const char*title)
{
    std::cout<<title<<"\n{ ";
    for(int i=0; i<size; i++)
        std::cout<<"{"<<datas[i].x<<","<<datas[i].y<<"} ";
    std::cout<<"}\n";
}
void csPrint_POINT2D(csPOINT2D*datas, int id, const char*title)
{
    std::cout<<title;
    std::cout<<"{"<<datas[id].x<<","<<datas[id].y<<"} \n";
}

int main()
{
    csLIST<const char*> clist;
    clist+="alpha";
    clist+="beta";
    clist+="gamma";
    clist+="delta";
    clist.printAll("List : ");
    clist-=2;
    clist.printAll("List rem id=2 : ");
    const char* str[]={"a", "b", "c", "d"};

    clist.addFromTable(0,str,0,4);
    clist.printAll("Adding from table : ");

    clist.addFromTable(6,str,0,4);
    clist.printAll("Adding from table : ");

    clist.addFromTable(0,str,1,2);
    clist.printAll("Adding from table section : ");

    clist.remFrom(6,4);
    clist.printAll("Remove section : ");

    clist.clear();

    csLIST<int> ilist;
    ilist+=1;
    ilist+=7;
    ilist+=-2;
    ilist+=0;
    ilist.printAll("List : ");
    ilist-=2;
    ilist.printAll("List rem id=2 : ");
    int tab[]={12,5,17,31};

    ilist.addFromTable(0,tab,0,4);
    ilist.printAll("Adding from table : ");

    ilist.addFromTable(6,tab,0,4);
    ilist.printAll("Adding from table : ");

    ilist.addFromTable(0,tab,1,2);
    ilist.printAll("Adding from table section : ");

    ilist.remFrom(6,4);
    ilist.printAll("Remove section : ");

    ilist.minMaxSort();
    ilist.printAll("MinMax sort : ");

    ilist.maxMinSort();
    ilist.printAll("MaxMin sort : ");

    ilist.clear();

    cout<<"\n\n";
    csLIST<csPOINT2D> lst;
    lst.printAll_v_func=csPrintAll_POINT2D;
    lst.print_v_func=csPrint_POINT2D;
    lst.addBottom({0,10});
    lst.addBottom({10,10});
    lst.addBottom({17,5});
    lst.addBottom({5,5});
    lst.add({5,5}, 7);
    lst.printAll_v("List of Points :");
    lst.print_v(0,"First point : ");
    lst.print_v(1,"Second point : ");
    lst.addTop({35,5});
    lst.print_v(0,"First point : ");
    lst.remTop();
    lst.print_v(0,"First point : ");

    lst.clear();
    return 0;
}
