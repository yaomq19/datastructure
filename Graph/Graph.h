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
            //����
            queue<int> q;
            //��¼�ж��ٸ����������
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
                //index��¼���׽����±�
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
        //����������⴦��
        if(vertices[indA].firstEdge == nullptr)
        {
            vertices[indA].firstEdge = new NodeE(indB,w);
        }
        else
        {
            //pָ���һ���߱��㣨������else�У��ý��һ�����ڣ�
            NodeE* p = vertices[indA].firstEdge;
            while(p->next!=nullptr)
            {
                p=p->next;
            }
            //�����һ������������һ�����
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