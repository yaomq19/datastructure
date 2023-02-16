#ifndef SEGMENTTREE_H
#define SEGMENTTREE_H
#include <bits/stdc++.h>
using namespace std;
class SegmentTree
{
    public:
    int *tree;
    int *data;
    SegmentTree(int* a,int n)
    {
        data = a;
        tree = new int[4*n+1];
        int left = 0;
        int right = n-1;
        build(1,left,right);
    }
    ~SegmentTree()
    {
        delete[] tree;
    }
    int build(int ind_seg,int left,int right)
    {
        if(left == right)
        {
            tree[ind_seg] = data[left];
            return tree[ind_seg];
        }
        int mid = (left+right) >>1;
        tree[ind_seg] = build(ind_seg * 2 ,left,mid)
        + build(ind_seg * 2 + 1,mid+1,right);
        return tree[ind_seg];
    }
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
};
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
class SegmentTreeWithLazy
{
    public:
    int m_n;
    int *tree;
    vector<Operation> *lazy;
    int *data;
    SegmentTreeWithLazy(int* a,int n):m_n(n)
    {
        data = a;
        tree = new int[4*n+1];
        lazy = new vector<Operation>[4*n+1];
        int left = 0;
        int right = n-1;
        build(1,left,right);
    }
    ~SegmentTreeWithLazy()
    {
        delete[] tree;
    }
    int build(int ind_seg,int left,int right)
    {
        if(left == right)
        {
            tree[ind_seg] = data[left];
            return tree[ind_seg];
        }
        int mid = (left+right) >>1;
        tree[ind_seg] = build(ind_seg * 2 ,left,mid)
        + build(ind_seg * 2 + 1,mid+1,right);
        return tree[ind_seg];
    }
    //线段树中下标为ind_tree的结点维护[l,r]的区间的和值
    void update(int ord,int ind_tree,int l,int r,int s,int t,int value,char op)
    {
        //print();
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
        //print();
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
    void print()
    {
        print(1,0,m_n-1);
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
#endif