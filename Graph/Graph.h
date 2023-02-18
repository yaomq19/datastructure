#ifndef Digraph_H
#define Digraph_H
#include<iostream>
#include<vector>
#include<queue>
#include<fstream>
using namespace std;
template<typename WeightType>
struct NodeE
{
    unsigned int index;
    WeightType weight;
    NodeE<WeightType>* next;
    NodeE(){}
    NodeE(int ind,int w):index(ind),weight(w),next(nullptr){}
};
template<typename DataType,typename WeightType>
struct NodeV
{
    DataType data;
    NodeE<WeightType>* firstEdge;
    NodeV(){}
    NodeV(DataType d):data(d),firstEdge(nullptr){}
};

//有向图ADT
template<typename DataType,typename WeightType>
class Digraph
{
    public:
    /* datas */
        int nv;//顶点数
        int ne;//边数
    /* functions */
    //顶点
        virtual int insert(const DataType&)=0;//插入顶点，返回编号
        virtual DataType remove(int)=0;//删除顶点及其关联边，返回该顶点信息
        virtual DataType& vertex(int)=0;//返回顶点数据的引用，使用时需要保证该顶点一定存在
        virtual inDegree(int)=0;//顶点的入度，使用时需要保证该顶点一定存在
        virtual outDegree(int)=0;//顶点的出度，使用时需要保证该顶点一定存在
    //边
        virtual bool exists(int,int)=0;//边<v,u>是否存在
        virtual void insert(int,int,const WeightType&)=0;//插入边
        virtual WeightType remove(int,int)=0;//删除边
        virtual WeightType& weight(int,int)=0;//返回边权值的引用，使用时需要保证该边一定存在
    //算法
        virtual vector<DataType> topoSort()=0;//拓扑排序，返回顶点数据域的一个拓扑序列
};

template<typename DataType,typename WeightType>
class DigraphList:public Digraph<DataType,WeightType>
{
    protected:
        vector<NodeV<DataType,WeightType>> vertices;
        vector<int> indegree;
        vector<int> outdegree;
    public:
    /* constructor and destructor*/
        DigraphList();
        DigraphList(const char* filename);
        virtual ~DigraphList();
        /* functions */
    //顶点
        virtual int insert(const DataType&);//插入顶点，返回编号
        virtual DataType remove(int);//删除顶点及其关联边，返回该顶点信息
        virtual DataType& vertex(int);//返回顶点数据的引用，使用时需要保证该顶点一定存在
        virtual int inDegree(int);//顶点的入度，使用时需要保证该顶点一定存在
        virtual int outDegree(int);//顶点的出度，使用时需要保证该顶点一定存在
    //边
        virtual bool exists(int,int);//边<v,u>是否存在
        virtual void insert(int,int,const WeightType&);//插入边
        virtual WeightType remove(int,int);//删除边
        virtual WeightType& weight(int,int);//返回边权值的引用，使用时需要保证该边一定存在
    //算法
        //拓扑排序
        virtual vector<DataType> topoSort();
};
//默认构造函数
template<typename DataType,typename WeightType>
DigraphList<DataType,WeightType>::DigraphList()
{
    this->nv=0;
    this->ne=0;
}
//含参构造函数
template<typename DataType,typename WeightType>
DigraphList<DataType,WeightType>::DigraphList(const char* filename)
{
    ifstream ifs(filename,ios::in);
    if(!ifs.is_open())
    {
        cout<<"failed to open file"<<filename<<endl;
        return;
    }
    int nvex,nedge;
    ifs>>nvex>>nedge;

    this->nv=nvex;this->ne=nedge;
    vertices.resize(this->nv);
    outdegree.resize(this->nv,0);
    indegree.resize(this->nv,0);
    int temp;
    for(int i=0;i<this->nv;i++)
    {
        ifs>>temp;
        NodeV<DataType,WeightType> nodev((DataType)temp);
        vertices[i]=nodev;
    }
    int indA,indB,w;
    while(nedge--)
    {
        ifs>>indA>>indB>>w;
        outdegree[indA]++;
        indegree[indB]++;
        //特殊情况特殊处理
        if(vertices[indA].firstEdge == nullptr)
        {
            vertices[indA].firstEdge = new NodeE<WeightType>(indB,w);
        }
        else
        {
            //p指向第一个边表结点（由于在else中，该结点一定存在）
            NodeE<WeightType>* p = vertices[indA].firstEdge;
            while(p->next!=nullptr)
            {
                p=p->next;
            }
            //在最后一个结点后面新增一个结点
            p->next = new NodeE<WeightType>(indB,w);
        }
    }
}
//析构函数
template<typename DataType,typename WeightType>
DigraphList<DataType,WeightType>::~DigraphList()
{
    for(auto &it:vertices)
    {
        NodeE<WeightType>* p =it.firstEdge;
        NodeE<WeightType>* q;
        if(p)q=p->next;
        while(p)
        {
            delete p;
            p=q;
            if(q)q=q->next;
        }
    }
}
//顶点
template<typename DataType,typename WeightType>
int DigraphList<DataType,WeightType>::insert(const DataType& d)
{
    NodeV<DataType,WeightType> nodev(d);
    vertices.push_back(nodev);
    indegree.push_back(0);
    outdegree.push_back(0);
    return this->nv++;
}
template<typename DataType,typename WeightType>
DataType DigraphList<DataType,WeightType>::remove(int ind)
{
    NodeE<WeightType>* ep = vertices[ind].firstEdge;
    NodeE<WeightType>* post,*pre;
    //删除所有出边
    while(ep)
    {
        indegree[ep->index]--;
        post = ep->next;
        delete ep;
        ep=post;
    }
    //删除所有入边
    for(int i=0;i<vertices.size();i++)
    {
        if(i==ind)continue;//该点本身跳过
        ep = vertices[i].firstEdge;
        pre = vertices[i].firstEdge;
        while(ep)
        {
            if(ep->index == ind){
                outdegree[i]--;
                post = ep->next;
                delete ep;
                if(ep == vertices[i].firstEdge){
                    //第一条边需要特殊处理
                    vertices[i].firstEdge = post;
                }
                else{
                    pre->next = post;
                }
            } 
            else{
                if(ep!=pre)
                {
                    pre=pre->next;
                }
                ep=ep->next;
            }
        }
    }
    //删除顶点本身
    DataType res = vertices[ind].data;
    vertices.erase(vertices.begin()+ind);
    indegree.erase(indegree.begin()+ind);
    outdegree.erase(outdegree.begin()+ind);
    this->nv--;
    return res;
}
template<typename DataType,typename WeightType>
DataType& DigraphList<DataType,WeightType>::vertex(int i)
{
    return vertices[i].data;
}

template<typename DataType,typename WeightType>
int DigraphList<DataType,WeightType>::inDegree(int i)
{
    return indegree[i];
}

template<typename DataType,typename WeightType>
int DigraphList<DataType,WeightType>::outDegree(int i)
{
    return outdegree[i];
}
//边
template<typename DataType,typename WeightType>
bool DigraphList<DataType,WeightType>::exists(int u,int v)
{
    NodeE<WeightType>* ep = vertices[u].firstEdge;
    while(ep)
    {
        if(ep->index==v)return true;
        ep=ep->next;
    }
    return false;
}
template<typename DataType,typename WeightType>
void DigraphList<DataType,WeightType>::insert(int u,int v,const WeightType& w)
{
    NodeE<WeightType>* ep = vertices[u].firstEdge;
    if(ep==nullptr)
    {
        vertices[u].firstEdge = new NodeE<WeightType>(v,w);
    }
    else
    {
        while(ep->next)
        {
            ep = ep->next;
        }
        ep->next = new NodeE<WeightType>(v,w);
    }
    outdegree[u]++;
    indegree[v]++;
}
template<typename DataType,typename WeightType>
WeightType DigraphList<DataType,WeightType>::remove(int u,int v)
{
    WeightType res;
    NodeE<WeightType>* ep = vertices[u].firstEdge;
    NodeE<WeightType>* pre = vertices[u].firstEdge;
    NodeE<WeightType>* post;
    while(ep)
    {
        if(ep->index == v){
            res = ep->weight;
            post = ep->next;
            delete ep;
            if(ep == vertices[u].firstEdge){
                //第一条边需要特殊处理
                vertices[u].firstEdge = post;
            }
            else{
                pre->next = post;
            }
        } 
        else{
            if(ep!=pre)
            {
                pre=pre->next;
            }
            ep=ep->next;
        }
    }
    outdegree[u]--;
    indegree[v]--;
    this->ne--;
    return res;
}
template<typename DataType,typename WeightType>
WeightType& DigraphList<DataType,WeightType>::weight(int u,int v)
{
    NodeE<WeightType>* ep = vertices[u].firstEdge;
    while(ep)
    {
        if(ep->index == v)
        {
            return ep->weight;
        }
        ep=ep->next;
    }
}
//算法
//拓扑排序
template<typename DataType,typename WeightType>
vector<DataType> DigraphList<DataType,WeightType>::topoSort()
{
    vector<int> indeg = this->indegree;
    //队列
    queue<int> q;
    //记录有多少个顶点入过队
    int count = 0;
    for(int i=0;i<vertices.size();i++)
    {
        if(indeg[i]==0)
        {
            q.push(i);
        }
    }
    vector<DataType> res;
    while(!q.empty())
    {
        //index记录队首结点的下标
        int index = q.front();
        q.pop();count++;
        res.push_back(vertices[index].data);
        NodeE<WeightType>* ep = vertices[index].firstEdge;
        while(ep)
        {
            int temp = --indeg[ep->index];
            if(temp == 0)
            {
                q.push(ep->index);
            }
            ep=ep->next;
        }
    }
    if(count != vertices.size())res.clear();
    return res;
}
#endif