#ifndef SEGMENTTREE_H
#define SEGMENTTREE_H
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
        }else if{
            return query(ind_tree*2+1,mid+1,r,s,t);
        }else{
            return query(ind_tree*2,l,mid,s,mid)
            + query(ind_tree*2+1,mid+1,r,mid+1,t);
        }
    }
};
#endif