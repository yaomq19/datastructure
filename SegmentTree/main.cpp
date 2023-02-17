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
        //为线段树开辟4n空间
        this->datasize=n;
        this->tree = new int[datasize*4];
        //暂时不启用lazy功能，将其指针置空，标记位置false
        this->lazy = nullptr;
        this->isLazyEnabled = false;
        //用data中的数据递归初始化线段树
        build(1,0,n-1,data);
    }
    ~SegmentTree()
    {
        if(tree)
            delete[] tree;
        if(lazy)
            delete[] lazy;
    }
    //启用区间修改功能
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
    //单点修改，将下标为ind_data的元素值加value
    void update(int ind_data,int value)
    {
        update(1,0,datasize-1,ind_data,value);
    }
    //区间修改，将下标在[s,t]之间的元素值加value
    void update(int s,int t,int value)
    {
        if(!isLazyEnabled)enableLazy(0);
        update(1,0,datasize-1,s,t,value);
    }
    //区间查询，查询[s,t]中的元素之和
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
    //递归函数，根据data中的数据
    //从线段树下标为ind_tree，维护区间为[left,right]的结点开始
    //构造其本身和全部子节点
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
    //递归函数，在线段树下标为ind_tree，维护区间为[l,r]中
    //修改对应原数组下标为ind_data的叶节点，及其相关祖先节点的值
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
    //递归函数，在线段树下标为ind_tree，维护区间为[l,r]中
    //修改对应原数组下标在[l,r]中的叶节点，及其相关祖先节点的值
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
    //在下标为ind_tree的结点维护的[l,r]区间中，搜索区间[s,t]的和
    int queryLazy(int ind_tree,int l,int r,int s,int t)
    {
        //如果找到区间
        if(l==s && r ==t){
            //递归处理并清空该结点及全部子节点的lazy值
            calculate(ind_tree,l,r);
            //返回该结点维护的和值
            return tree[ind_tree];
        }
        //否则[s,t]只是[l,r]的子区间，说明[l,r]还没有被完整地直接查到
        //由于是在查询函数中，因此需要将本结点的lazy下放，并清空本结点的lazy值
        lazy[ind_tree*2]+=lazy[ind_tree];
        lazy[ind_tree*2+1]+=lazy[ind_tree];
        lazy[ind_tree]=0;
        //递归查询左右子节点
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