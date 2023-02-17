#include<bits/stdc++.h>
#include<windows.h>
using namespace std;
class SegmentTree
{
    protected:
    int *tree;
    int *lazy;
    int datasize;
    bool isLazyEnabled;
    public:
    SegmentTree(int* data,int n)
    {
        //Ϊ�߶�������4n�ռ�
        this->datasize=n;
        this->tree = new int[datasize*4];
        //��ʱ������lazy���ܣ�����ָ���ÿգ����λ��false
        this->lazy = nullptr;
        this->isLazyEnabled = false;
        //��data�е����ݵݹ��ʼ���߶���
        build(1,0,n-1,data);
    }
    ~SegmentTree()
    {
        if(tree)
            delete[] tree;
        if(lazy)
            delete[] lazy;
    }
    //���������޸Ĺ���
    void enableLazy(int initValue=0)
    {
        if(lazy)delete[] lazy;
        this->lazy = new int[datasize*4];
        for(int i=0;i<datasize*4;i++)
        {
            lazy[i]=initValue;
        }
        this->isLazyEnabled = true;
    }
    void disenableLazy()
    {
        if(!lazy)delete[] lazy;
        this->isLazyEnabled = false;
    }
    //�����޸ģ����±�Ϊind_data��Ԫ��ֵ��value
    void update(int ind_data,int value)
    {
        update(1,0,datasize-1,ind_data,value);
    }
    //�����޸ģ����±���[s,t]֮���Ԫ��ֵ��value
    void update(int s,int t,int value)
    {
        if(!isLazyEnabled)enableLazy(0);
        update(1,0,datasize-1,s,t,value);
    }
    //�����ѯ����ѯ[s,t]�е�Ԫ��֮��
    int query(int s,int t)
    {
        if(isLazyEnabled)return queryLazy(1,0,datasize-1,s,t);
        else return query(1,0,datasize-1,s,t);
    }
    void print()
    {
        cout<<"---------------"<<endl;
        print(1,0,datasize-1);
        cout<<"---------------"<<endl;
    }
    void print(int ind_tree,int l,int r)
    {
        int mid = (l+r)>>1;
        if(l!=r)print(ind_tree*2,l,mid);
        if(l==r)
        {
            cout<<"data["<<l<<"] = "<<tree[ind_tree]<<endl;
            return;
        }
        if(l!=r)print(ind_tree*2+1,mid+1,r);
    }
    protected:
    //�ݹ麯��������data�е�����
    //���߶����±�Ϊind_tree��ά������Ϊ[left,right]�Ľ�㿪ʼ
    //�����䱾���ȫ���ӽڵ�
    int build(int ind_tree,int left,int right,int* data)
    {
        if(left == right)
        {
            tree[ind_tree] = data[left];
            return tree[ind_tree];
        }
        int mid = (left+right) >>1;
        tree[ind_tree] = build(ind_tree * 2 ,left,mid,data)
        + build(ind_tree * 2 + 1,mid+1,right,data);
        return tree[ind_tree];
    }
    //�ݹ麯�������߶����±�Ϊind_tree��ά������Ϊ[l,r]��
    //�޸Ķ�Ӧԭ�����±�Ϊind_data��Ҷ�ڵ㣬����������Ƚڵ��ֵ
    void update(int ind_tree,int l,int r,int ind_data,int value)
    {
        tree[ind_tree]+=value;
        if(l==r){
            return;
        }
        int mid = (l+r)>>1;
        if(ind_data<=mid){
            update(ind_tree*2,l,mid,ind_data,value);
        }else{
            update(ind_tree*2+1,mid+1,r,ind_data,value);
        }
    }
    //�ݹ麯�������߶����±�Ϊind_tree��ά������Ϊ[l,r]��
    //�޸Ķ�Ӧԭ�����±���[l,r]�е�Ҷ�ڵ㣬����������Ƚڵ��ֵ
    void update(int ind_tree,int l,int r,int s,int t,int value)
    {
        if(l==s&&r==t)
        {
            lazy[ind_tree]+=value;
            return;
        }
        int mid = (l+r)>>1;
        if(t<=mid){
            update(ind_tree*2,l,mid,s,t,value);
        }else if(s>mid){
            update(ind_tree*2+1,mid+1,r,s,t,value);
        }else {
            update(ind_tree*2,l,mid,s,mid,value);
            update(ind_tree*2+1,mid+1,r,mid+1,t,value);
        }
    }

    int query(int ind_tree,int l,int r,int s,int t)
    {
        if(l==s && r ==t){
            return tree[ind_tree];
        }
        int mid = (l+r)>>1;
        if(t<=mid){
            return query(ind_tree*2,l,mid,s,t);
        }else if(s>mid){
            return query(ind_tree*2+1,mid+1,r,s,t);
        }else{
            return query(ind_tree*2,l,mid,s,mid)
            + query(ind_tree*2+1,mid+1,r,mid+1,t);
        }
    }
    //���±�Ϊind_tree�Ľ��ά����[l,r]�����У���������[s,t]�ĺ�
    int queryLazy(int ind_tree,int l,int r,int s,int t)
    {
        //����ҵ�����
        if(l==s && r ==t){
            //�ݹ鴦����ոý�㼰ȫ���ӽڵ��lazyֵ
            calculate(ind_tree,l,r);
            //���ظý��ά���ĺ�ֵ
            return tree[ind_tree];
        }
        //����[s,t]ֻ��[l,r]�������䣬˵��[l,r]��û�б�������ֱ�Ӳ鵽
        //�������ڲ�ѯ�����У������Ҫ��������lazy�·ţ�����ձ�����lazyֵ
        lazy[ind_tree*2]+=lazy[ind_tree];
        lazy[ind_tree*2+1]+=lazy[ind_tree];
        lazy[ind_tree]=0;
        //�ݹ��ѯ�����ӽڵ�
        int mid = (l+r)>>1;
        if(t<=mid){
            return queryLazy(ind_tree*2,l,mid,s,t);
        }else if(s>mid){
            return queryLazy(ind_tree*2+1,mid+1,r,s,t);
        }else{
            return queryLazy(ind_tree*2,l,mid,s,mid)+
                queryLazy(ind_tree*2+1,mid+1,r,mid+1,t);
        }
    }
    int calculate(int ind_tree,int l,int r)
    {
        if(l==r)
        {
            tree[ind_tree]+=lazy[ind_tree];
            lazy[ind_tree]=0;
            return tree[ind_tree];
        }else{
            lazy[ind_tree*2]+=lazy[ind_tree];
            lazy[ind_tree*2+1]+=lazy[ind_tree];
            lazy[ind_tree]=0;
            int mid=(l+r)>>1;
            tree[ind_tree] =calculate(ind_tree*2,l,mid)+
                calculate(ind_tree*2+1,mid+1,r);
            return tree[ind_tree];
        }
    }
};
void judge(vector<int>answer,vector<int>output)
{
    if(answer.size()!=output.size())
    {
        cout<<"error:answer.size = "<<answer.size()<<" but output.size = "<<output.size()<<endl;
        return;
    }
    else{
        for(int i=0;i<answer.size();i++)
        {
            if(answer[i]!=output[i])
            {
                cout<<"error:answer["<<i<<"] = "<<answer[i]<<" but output["<<i<<"] = "<<output[i]<<endl;
                return;
            }
        }
    }
    cout<<"AC"<<endl;
    return;
}
void execute(const char* filename,vector<int>&answer,vector<int>&output)
{
    ifstream ifs;
    ifs.open(filename,ios::in);
    int n,m;
    ifs>>n>>m;
    int *data = new int[n];
    for(int i=0;i<n;i++)
    {
        ifs>>data[i];
    }
    SegmentTree tree(data,n);
    bool enablelazyflag = 0;
    for(int i=0;i<m;i++)
    {
        int command,ind,s,t,v;
        ifs>>command;
        if(command==1){
            ifs>>ind>>v;
            tree.update(ind,v);
        }else if(command ==2){
            if(!enablelazyflag)
            {
                tree.enableLazy(0);
                enablelazyflag=true;
            }
            ifs>>s>>t>>v;
            tree.update(s,t,v);
        }else if(command==3){
            ifs>>s>>t;
            output.push_back(tree.query(s,t));
        }
    }
    int temp = -1;
    while(!ifs.eof())
    {
        ifs>>temp;
        answer.push_back(temp);
    }
    delete[] data;
}
int main()
{
    vector<int> answer,output;
    auto start = GetTickCount();
    execute("testcase.txt",answer,output);
    auto end = GetTickCount();
    judge(answer,output);
    cout<<"this testcase took "<<end-start<<"ms."<<endl;
    return 0;
}