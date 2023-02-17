#ifndef GRAPHL_H
#define GRAPHL_H
#include<vector>
#include<queue>
#include<fstream>
using namespace std;
struct NodeE
{
    int index;
    int weight;
    NodeE* next;
    NodeE(int ind,int w):index(ind),weight(w),next(nullptr){}
};
struct NodeV
{
    int data;
    int indegree,outdegree;
    NodeE* firstEdge;
    NodeV(int d):data(d),firstEdge(nullptr),
    indegree(0),outdegree(0){}
};
class Graph
{
    public:
        /* destructor*/
        virtual ~Graph()=0;
        /* functions */
        virtual vector<int> topoSort()=0;
};
Graph::~Graph(){}
class GraphL:public Graph
{
    protected:
        vector<NodeV> vertices; 
        int nv,ne;
    public:
        /* constructor and destructor*/
        GraphL(const char* filename);
        ~GraphL();
        /* functions */
        vector<int> topoSort()
        {
            vector<int> indegree;
            for(auto &it:vertices)
            {
                indegree.push_back(it.indegree);
            }
            //队列
            queue<int> q;
            //记录有多少个顶点入过队
            int count = 0;
            for(int i=0;i<vertices.size();i++)
            {
                if(indegree[i]==0)
                {
                    q.push(i);
                }
            }
            vector<int> res;
            while(!q.empty())
            {
                //index记录队首结点的下标
                int index = q.front();
                q.pop();count++;
                res.push_back(vertices[index].data);
                NodeE* ep = vertices[index].firstEdge;
                while(ep)
                {
                    int temp = --indegree[ep->index];
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
};
GraphL:: GraphL(const char* filename)
{
    ifstream ifs(filename,ios::in);
    if(!ifs.is_open())
    {
        cout<<"failed to open file"<<filename<<endl;
        return;
    }
    int nvex,nedge;
    ifs>>nvex>>nedge;
    while(nvex--)
    {
        int temp;
        ifs>>temp;
        NodeV nodev(temp);
        vertices.push_back(nodev);
    }
    int indA,indB,w;
    while(nedge--)
    {
        ifs>>indA>>indB>>w;
        vertices[indA].outdegree++;
        vertices[indB].indegree++;
        //特殊情况特殊处理
        if(vertices[indA].firstEdge == nullptr)
        {
            vertices[indA].firstEdge = new NodeE(indB,w);
        }
        else
        {
            //p指向第一个边表结点（由于在else中，该结点一定存在）
            NodeE* p = vertices[indA].firstEdge;
            while(p->next!=nullptr)
            {
                p=p->next;
            }
            //在最后一个结点后面新增一个结点
            p->next = new NodeE(indB,w);
        }
    }
}
GraphL:: ~GraphL()
{
    for(auto &it:vertices)
    {
        NodeE* p =it.firstEdge;
        NodeE* q;
        if(p)q=p->next;
        while(p)
        {
            delete p;
            p=q;
            if(q)q=q->next;
        }
    }
}

#endif