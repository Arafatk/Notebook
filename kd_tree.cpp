#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <bits/stdc++.h>
#define pq  priority_queue
#define MAX_DIM 3
using namespace std;
struct kd_node_t{
    double x[MAX_DIM];
    double dis,rad; // sort by a closest then farthest
    bool operator<(const struct kd_node_t& other) const{return dis < other.dis;} // logic
    struct kd_node_t *left, *right;
};
 
    inline double
dist(struct kd_node_t *a, struct kd_node_t *b, int dim)
{
    double t, d = 0;
    while (dim--) {
        t = a->x[dim] - b->x[dim];
        d += t * t;
    }
    return d;
}
inline void swap(struct kd_node_t *x, struct kd_node_t *y) {
    double tmp[MAX_DIM];
    memcpy(tmp,  x->x, sizeof(tmp));
    memcpy(x->x, y->x, sizeof(tmp));
    memcpy(y->x, tmp,  sizeof(tmp));
}
 
 
/* see quickselect method */
    struct kd_node_t*
find_median(struct kd_node_t *start, struct kd_node_t *end, int idx)
{
    if (end <= start) return NULL;
    if (end == start + 1)
        return start;
 
    struct kd_node_t *p, *store, *md = start + (end - start) / 2;
    double pivot;
    while (1) {
        pivot = md->x[idx];
 
        swap(md, end - 1);
        for (store = p = start; p < end; p++) {
            if (p->x[idx] < pivot) {
                if (p != store)
                    swap(p, store);
                store++;
            }
        }
        swap(store, end - 1);
 
        /* median has duplicate values */
        if (store->x[idx] == md->x[idx])
            return md;
 
        if (store > md) end = store;
        else        start = store;
    }
}
 
    struct kd_node_t*
make_tree(struct kd_node_t *t, int len, int i, int dim)
{
    struct kd_node_t *n;
 
    if (!len) return 0;
 
    if ((n = find_median(t, t + len, i))) {
        i = (i + 1) % dim;
        n->left  = make_tree(t, n - t, i, dim);
        n->right = make_tree(n + 1, t + len - (n + 1), i, dim);
    }
    return n;
}
 
/* global variable, so sue me */
int visited;
 
set<kd_node_t> bruter;
void nearest(struct kd_node_t *root, struct kd_node_t *nd, int i, int dim,
        struct kd_node_t **best, double *best_dist,int size)
{
    double d, dx, dx2;
 
    if (!root) return;
    d = dist(root, nd, dim);
    dx = root->x[i] - nd->x[i];
    dx2 = dx * dx;
 
    visited ++;

    root->dis = d;    // Putting inside pq with priority distance
    root->rad = dx2;
    bruter.insert(*root);
    while(bruter.size()>size){
    set<kd_node_t>::iterator it = bruter.end();it--;bruter.erase(it);};
 
    if (!*best || d < *best_dist) {
        *best_dist = d;
        *best = root;
    }
  
    if (++i >= dim) i = 0;
    nearest(dx > 0 ? root->left : root->right, nd, i, dim, best, best_dist,size);
    set<kd_node_t>::iterator it = bruter.end();it--;
    if((bruter.size() < size || it->dis >= dx2))
    nearest(dx > 0 ? root->right : root->left, nd, i, dim, best, best_dist,size);
}
 
#define N 1000000
#define rand1() (rand() / (double)RAND_MAX)
#define rand_pt(v) { v.x[0] = rand1(); v.x[1] = rand1(); v.x[2] = rand1(); }
struct kd_node_t wp[] = {
        {{2, 3},123,0}, {{5, 4},123,0}, {{9, 6},123,0}, {{3, 7},123,0},
         {{4, 7},123,0}, {{8, 1},123,0}, {{7, 2},123,0}, {{6, 2},123,0},
         {{14, 17},123,0},{{2, 2},123,0},{{6, 6},123,0},{{11, 2},123,0},
         {{7, 4},123,0},{{9, 1},123,0},{{5, 1},123,0},{{7, 1},123,0}    };
struct kd_node_t testNode = {{9, 2},123,0};
struct kd_node_t *root, *found, *million;



void brute( struct kd_node_t *nd)
{
   for(int i=0;i<sizeof(wp) / sizeof(wp[1]);i++)
   {
    wp[i].dis=dist(&wp[i],nd,2);
    bruter.insert(wp[i]);
   }
   cout << "\nThis is the test node and these are the results of brute force \n " << testNode.x[0] << " " << testNode.x[1] << "\n\n\n";
   set<kd_node_t>::iterator it;
   for(int i=0;i<10;i++)
   {

    it = bruter.begin();
    cout << it->x[0] <<"  " << it->x[1] <<"  " << it->dis << "\n";
    bruter.erase(it);
   }

}

set<kd_node_t> asd;
void brute2( struct kd_node_t *nd,int size)
{
   for(int i=0;i<N;i++)
   {
    million[i].dis=dist(&million[i],nd,3);
    asd.insert(million[i]);
   }
   cout << "\nThis is the test node " << testNode.x[0] << " " << testNode.x[1] << " " << testNode.x[2]<<"\n and these are the results of brute force \n "<< "\n";
   set<kd_node_t>::iterator it;
   for(int i=0;i<size;i++)
   {
    it = asd.begin();
    cout << it->x[0] <<"  " << it->x[1] <<"  " << it->dis << "\n";
    asd.erase(it);
   }

}
int main(void)
{
    int i;
    double best_dist;
 
    root = make_tree(wp, sizeof(wp) / sizeof(wp[1]), 0, 2);
 
    visited = 0;
    found = 0;
    nearest(root, &testNode, 0, 2, &found, &best_dist,8);
 
    printf(">> WP tree\nsearching for (%g, %g)\n"
            "found (%g, %g) dist %g\nseen %d nodes\n\n",
            testNode.x[0], testNode.x[1],
            found->x[0], found->x[1], sqrt(best_dist), visited);
    set<kd_node_t>::iterator it;
    while(bruter.size()){
        it = bruter.begin();
        cout << it->x[0] <<"  " << it->x[1] <<"  " << it->dis << "\n";
        bruter.erase(it);
    };
    
    
    brute(&testNode);
    while(bruter.size()){
        it = bruter.begin();
        bruter.erase(it);
    };
    
million =(struct kd_node_t*) calloc(N, sizeof(struct kd_node_t));
srand(time(0));
    for (i = 0; i < N; i++) rand_pt(million[i]);
 
    root = make_tree(million, N, 0, 3);
    rand_pt(testNode);
 
    visited = 0;
    found = 0;
    int size=14;
       clock_t t1,t2;
    t1=clock();
    //code goes here
    nearest(root, &testNode, 0, 3, &found, &best_dist,size);
 
    t2=clock();
    float diff ((float)t2-(float)t1);

    printf(">> Million tree\nsearching for (%g, %g, %g)\n"
            "found (%g, %g, %g) dist %g\nseen %d nodes\n",
            testNode.x[0], testNode.x[1], testNode.x[2],
            found->x[0], found->x[1], found->x[2],
            (best_dist), visited);
     while(bruter.size()){
        it = bruter.begin();
        cout << it->x[0] <<"  " << it->x[1] <<"  " << it->x[2]  <<"  "<< it->dis << "\n";
        bruter.erase(it);
    };
       t1=clock();
    brute2(&testNode,size);
    nearest(root, &testNode, 0, 3, &found, &best_dist,size);
 
    t2=clock();
    float diff2 ((float)t2-(float)t1);
    cout<< "\nTime taken by brute force \n"<<diff2<<endl; 
    cout<< "\nTime taken by kd tree \n"<<diff<<endl;
    cout << "The ratio of time taken by k-d tree and highly optimized brute force(O(N Log N)) " << diff2/diff << "\n";
        return 0;
}