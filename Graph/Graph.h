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

//����ͼADT
template<typename DataType,typename WeightType>
class Digraph
{
    public:
    /* datas */
        int nv;//������
        int ne;//����
    /* functions */
    //����
        virtual int insert(const DataType&)=0;//���붥�㣬���ر��
        virtual DataType remove(int)=0;//ɾ�����㼰������ߣ����ظö�����Ϣ
        virtual DataType& vertex(int)=0;//���ض������ݵ����ã�ʹ��ʱ��Ҫ��֤�ö���һ������
        virtual inDegree(int)=0;//�������ȣ�ʹ��ʱ��Ҫ��֤�ö���һ������
        virtual outDegree(int)=0;//����ĳ��ȣ�ʹ��ʱ��Ҫ��֤�ö���һ������
    //��
        virtual bool exists(int,int)=0;//��<v,u>�Ƿ����
        virtual void insert(int,int,const WeightType&)=0;//�����
        virtual WeightType remove(int,int)=0;//ɾ����
        virtual WeightType& weight(int,int)=0;//���ر�Ȩֵ�����ã�ʹ��ʱ��Ҫ��֤�ñ�һ������
    //�㷨
        virtual vector<DataType> topoSort()=0;//�������򣬷��ض����������һ����������
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
    //����
        virtual int insert(const DataType&);//���붥�㣬���ر��
        virtual DataType remove(int);//ɾ�����㼰������ߣ����ظö�����Ϣ
        virtual DataType& vertex(int);//���ض������ݵ����ã�ʹ��ʱ��Ҫ��֤�ö���һ������
        virtual int inDegree(int);//�������ȣ�ʹ��ʱ��Ҫ��֤�ö���һ������
        virtual int outDegree(int);//����ĳ��ȣ�ʹ��ʱ��Ҫ��֤�ö���һ������
    //��
        virtual bool exists(int,int);//��<v,u>�Ƿ����
        virtual void insert(int,int,const WeightType&);//�����
        virtual WeightType remove(int,int);//ɾ����
        virtual WeightType& weight(int,int);//���ر�Ȩֵ�����ã�ʹ��ʱ��Ҫ��֤�ñ�һ������
    //�㷨
        //��������
        virtual vector<DataType> topoSort();
};
//Ĭ�Ϲ��캯��
template<typename DataType,typename WeightType>
DigraphList<DataType,WeightType>::DigraphList()
{
    this->nv=0;
    this->ne=0;
}
//���ι��캯��
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
        //����������⴦��
        if(vertices[indA].firstEdge == nullptr)
        {
            vertices[indA].firstEdge = new NodeE<WeightType>(indB,w);
        }
        else
        {
            //pָ���һ���߱��㣨������else�У��ý��һ�����ڣ�
            NodeE<WeightType>* p = vertices[indA].firstEdge;
            while(p->next!=nullptr)
            {
                p=p->next;
            }
            //�����һ������������һ�����
            p->next = new NodeE<WeightType>(indB,w);
        }
    }
}
//��������
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
//����
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
    //ɾ�����г���
    while(ep)
    {
        indegree[ep->index]--;
        post = ep->next;
        delete ep;
        ep=post;
    }
    //ɾ���������
    for(int i=0;i<vertices.size();i++)
    {
        if(i==ind)continue;//�õ㱾������
        ep = vertices[i].firstEdge;
        pre = vertices[i].firstEdge;
        while(ep)
        {
            if(ep->index == ind){
                outdegree[i]--;
                post = ep->next;
                delete ep;
                if(ep == vertices[i].firstEdge){
                    //��һ������Ҫ���⴦��
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
    //ɾ�����㱾��
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
//��
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
                //��һ������Ҫ���⴦��
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
//�㷨
//��������
template<typename DataType,typename WeightType>
vector<DataType> DigraphList<DataType,WeightType>::topoSort()
{
    vector<int> indeg = this->indegree;
    //����
    queue<int> q;
    //��¼�ж��ٸ����������
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
        //index��¼���׽����±�
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