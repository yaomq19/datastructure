#ifndef SEGMENTTREE_H
#define SEGMENTTREE_H
#include <bits/stdc++.h>
using namespace std;
//支持单一修改操作线段树
class SegmentTree
{
private:
    int *lazy;
protected:
    int *tree;
    int datasize;
    bool isLazyEnabled;
public:
    SegmentTree(int* data,int n);
    virtual ~SegmentTree();
    /*启用区间修改功能*/
    virtual void enableLazy(int initValue=0);

    /*关闭区间修改功能*/
    virtual void disenableLazy();

    /*单点修改，将下标为ind_data的元素值加value*/
    virtual void update(int ind_data,int value);

    /*区间修改，将下标在[s,t]之间的元素值加value*/
    virtual void update(int s,int t,int value);

    /*区间查询，查询[s,t]中的元素之和*/
    virtual int query(int s,int t);

    /*调试用打印函数*/
    virtual void print();

    /*调试用打印函数*/
    virtual void print(int ind_tree,int l,int r);
protected:
    /*递归函数，根据data中的数据*/
    /*从线段树下标为ind_tree，维护区间为[left,right]的结点开始*/
    /*构造其本身和全部子节点*/
    virtual int build(int ind_tree,int left,int right,int* data);

    /*递归函数，在线段树下标为ind_tree，维护区间为[l,r]中*/
    /*修改对应原数组下标为ind_data的叶节点，及其相关祖先节点的值*/
    virtual void update(int ind_tree,int l,int r,int ind_data,int value);

    /*递归函数，在线段树下标为ind_tree，维护区间为[l,r]中*/
    /*修改对应原数组下标在[l,r]中的叶节点，及其相关祖先节点的值*/
    virtual void update(int ind_tree,int l,int r,int s,int t,int value);

    /*无lazy查询：在下标为ind_tree的结点维护的[l,r]区间中，搜索区间[s,t]的和*/
    virtual int query(int ind_tree,int l,int r,int s,int t);
    
    /*有lazy查询：在下标为ind_tree的结点维护的[l,r]区间中，搜索区间[s,t]的和*/
    virtual int queryLazy(int ind_tree,int l,int r,int s,int t);
    
    /*递归处理并清空下标为ind_tree的结点维护的区间[l,r]的所有lazy标记*/
    /*然后返回该区间的和值*/
    virtual int calculate(int ind_tree,int l,int r);

    
};
//支持复合修改操作的线段树
class SegmentTreeMultioperation:public SegmentTree
{
    public:
    struct Operation
    {
        int order;
        char op;
        int v;
        Operation(int a,char b,int c):order(a),op(b),v(c){}
    };
    bool mycmp(const Operation& a,const Operation&b)
    {
        return a.order<b.order;
    }
    private:
    vector<Operation> *lazy;

    public:
    SegmentTreeMultioperation(int* a,int n):SegmentTree(a,n)
    {
        
    }
    virtual ~SegmentTreeMultioperation()
    {
        if(lazy)
        {
            delete[] lazy;
        }
    }
    //线段树中下标为ind_tree的结点维护[l,r]的区间的和值
    virtual void update(int ord,int ind_tree,int l,int r,int s,int t,int value,char op)
    {
        if(l==s&&r==t)
        {
            lazy[ind_tree].push_back(Operation(ord,op,value));
            return;
        }
        int mid = (l+r)>>1;
        if(t<=mid){
            update(ord,ind_tree*2,l,mid,s,t,value,op);
        }else if(s>mid){
            update(ord,ind_tree*2+1,mid+1,r,s,t,value,op);
        }else {
            update(ord,ind_tree*2,l,mid,s,mid,value,op);
            update(ord,ind_tree*2+1,mid+1,r,mid+1,t,value,op);
        }
    }
    int handle(int ind_tree,int l,int r)
    {
        if(l==r)
        {
            sort(lazy[ind_tree].begin(),lazy[ind_tree].end(),mycmp);
            for(auto &it:lazy[ind_tree])
            {
                if(it.op=='+'){
                    tree[ind_tree]+=it.v;
                }else if(it.op=='*'){
                    tree[ind_tree]*=it.v;
                }
            }
            lazy[ind_tree].clear();
            return tree[ind_tree];
        }else{
            lazy[ind_tree*2].insert(
                lazy[ind_tree*2].end(),
                lazy[ind_tree].begin(),
                lazy[ind_tree].end()
            );
            lazy[ind_tree*2+1].insert(
                lazy[ind_tree*2+1].end(),
                lazy[ind_tree].begin(),
                lazy[ind_tree].end()
            );
            lazy[ind_tree].clear();
            int mid=(l+r)>>1;
            tree[ind_tree] =  handle(ind_tree*2,l,mid)
            +handle(ind_tree*2+1,mid+1,r);
            return tree[ind_tree];
        }
    }
    int query(int ind_tree,int l,int r,int s,int t)
    {
        if(l==s && r ==t){
            handle(ind_tree,l,r);
            return tree[ind_tree];
        }
        int mid = (l+r)>>1;
        lazy[ind_tree*2].insert(
                lazy[ind_tree*2].end(),
                lazy[ind_tree].begin(),
                lazy[ind_tree].end()
            );
            lazy[ind_tree*2+1].insert(
                lazy[ind_tree*2+1].end(),
                lazy[ind_tree].begin(),
                lazy[ind_tree].end()
            );
            lazy[ind_tree].clear();
        if(t<=mid){
            int temp=query(ind_tree*2,l,mid,s,t);
            return temp;
        }else if(s>mid){
            int temp=query(ind_tree*2+1,mid+1,r,s,t);
            return temp;
        }else{
            int temp=query(ind_tree*2,l,mid,s,mid)
            + query(ind_tree*2+1,mid+1,r,mid+1,t);
            return temp;
        }
    }
    virtual void print()
    {
        print(1,0,datasize-1);
        cout<<endl;
    }
    void print(int ind,int l,int r)
    {
        cout<<"tree["<<ind<<"] = "<<tree[ind]<<endl;
        if(!lazy[ind].empty()){
            cout<<"lazy["<<ind<<"] = ";
        }
        for(auto &it:lazy[ind])
        {
            cout<<it.order<<"th:"<<it.op<<it.v<<endl;
        }
        if(l==r)return;
        else{
            int mid = (l+r)>>1;
            print(ind*2,l,mid);
            print(ind*2+1,mid+1,r);
        }
    }
};

SegmentTree::SegmentTree(int* data,int n)
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
SegmentTree::~SegmentTree()
{
    if(tree)
        delete[] tree;
    if(lazy)
        delete[] lazy;
}
void SegmentTree::enableLazy(int initValue)
{
    if(lazy)delete[] lazy;
    this->lazy = new int[datasize*4];
    for(int i=0;i<datasize*4;i++)
    {
        lazy[i]=initValue;
    }
    this->isLazyEnabled = true;
}
void SegmentTree::disenableLazy()
{
    if(!lazy)delete[] lazy;
    this->isLazyEnabled = false;
}
void SegmentTree::update(int ind_data,int value)
{
    update(1,0,datasize-1,ind_data,value);
}
void SegmentTree::update(int s,int t,int value)
{
    if(!isLazyEnabled)enableLazy(0);
    update(1,0,datasize-1,s,t,value);
}
int SegmentTree::query(int s,int t)
{
    if(isLazyEnabled)return queryLazy(1,0,datasize-1,s,t);
    else return query(1,0,datasize-1,s,t);
}
void SegmentTree::print()
{
    cout<<"---------------"<<endl;
    print(1,0,datasize-1);
    cout<<"---------------"<<endl;
}
void SegmentTree::print(int ind_tree,int l,int r)
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
int SegmentTree::build(int ind_tree,int left,int right,int* data)
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
void SegmentTree::update(int ind_tree,int l,int r,int ind_data,int value)
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
void SegmentTree::update(int ind_tree,int l,int r,int s,int t,int value)
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
int SegmentTree::query(int ind_tree,int l,int r,int s,int t)
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
int SegmentTree::queryLazy(int ind_tree,int l,int r,int s,int t)
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
int SegmentTree::calculate(int ind_tree,int l,int r)
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
#endif