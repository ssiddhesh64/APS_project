#include <iostream>
#include <bits/stdc++.h>
#include <time.h>

using namespace std;

#define checkpoint(x) cout<<x<<endl

class vtree{

    private:
        long long u;
        long long minimum;
        long long maximum;

        vtree *summary;
        vtree **cluster;

    public:

        vtree(long long s);

        long long find_min();
        long long find_max();
        long long extractMin();

        void insert(long long x);
        void del(long long x);

        void empty_tree_insert(long long x);
};

vtree :: vtree(long long s){

    u = s;
    this->minimum = -1;
    this->maximum = -1;

    if(u == 2){
        this->summary = NULL;
        this->cluster = NULL;
    }
    else{

        long long sqrt_u = (long long)sqrt(u);
        vtree* summ = new vtree(sqrt_u);
        this->summary = summ;
        vtree** t = new vtree*[sqrt_u];
        this->cluster = t;
        for(int i = 0; i < sqrt_u; i++){
            this->cluster[i] = new vtree(sqrt_u);
        }

    }
}
long long vtree :: find_min(){
    return minimum;
}

long long vtree :: find_max(){
    return maximum;
}

void vtree :: empty_tree_insert(long long x){
    minimum = x;
    maximum = x;
}

void vtree ::insert(long long x){

    if(minimum == -1){
        empty_tree_insert(x);
    }
    else{
        if(x < minimum){
            swap(x,minimum);
        }
        if(u > 2){

            long long sub_size = (long long)sqrt(u);
            long long high = x/sub_size;
            long long low = x%sub_size;
            if(cluster[high]->find_min() == -1){

                if(summary == NULL){
                    vtree *summ = new vtree(sub_size);
                    this->summary = summ;
                }
                summary->insert(high);
                cluster[high]->empty_tree_insert(low);
            }


            else{
                cluster[high]->insert(low);
            }
        }
        if(x > maximum){
            maximum = x;
        }
    }
}

long long vtree :: extractMin()
{
    long long ret = this -> find_min();
    this -> del(ret);
    return ret;
}

void vtree :: del(long long x){

    if(minimum == maximum){
        minimum = -1;
        maximum = -1;
        return;
    }
    if(u == 2){
        //cout << "3" << endl;
        if(x == 0){
            minimum = 1;
        }
        else{
            minimum = 0;
        }
        maximum = minimum;
        return;
    }

    long long sub_size = (long long)sqrt(u);
    if(x == minimum){

        long long high = summary->find_min()*sub_size;
        long long low = summary->find_min();
        minimum = x = high + cluster[low]->find_min();
    }
    long long i = x/sub_size;
    long long lo = x % sub_size;

    cluster[i]->del(lo);

    if(cluster[i]->find_min()==-1){

        summary->del(i);

        if(x == maximum){
            long long summax = summary->find_max();
            //cout << summax << endl;
            if(summax == -1 ){
                maximum = minimum;
            }
            else{
                long long off = cluster[summax]->find_max();
                maximum = summax * sub_size + off;
            }
        }
     }
    else{
        if(x == maximum){
            long long offs = cluster[i]->find_max();
            maximum = i*sub_size + offs;
        }
    }
}

struct disjoint_sets{

    long long *parent, *rnk;
    long long n;

    disjoint_sets(long long n){

        this->n = n;
        parent = new long long[n+1];
        rnk = new long long[n+1];

        for (long long i = 0; i <= n; i++)
        {
            rnk[i] = 0;
            parent[i] = i;
        }
    }

    long long find(long long u){

        if (u != parent[u])
            parent[u] = find(parent[u]);
        return parent[u];
    }

    void merge(long long x, long long y){
        x = find(x), y = find(y);

        if (rnk[x] > rnk[y])
            parent[y] = x;
        else
            parent[x] = y;

        if (rnk[x] == rnk[y])
            rnk[y]++;
    }
};

//This program takes a graph as an input and gives weight of minimum spanning tree as output
//No negative weights accepted

//input format
//n e   (where n is number of nodes and e is number of edges)
//u v w  (There is an edge between u and v of weight w)
/*
Given 2 integers N and M. N represents the number of vertices in the graph. M represents the number of edges between any 2 vertices.
Then M lines follow, each line has 3 space separated integers u, v and w where u and v represents an edge from a vertex u to a vertex v and w represents the weight of that edge.
*/

// input can also be given in file by following syntax
// ./van_Emde_Boas < file_name.txt

int main(){

    //freopen("final_dense_test_99.txt", "r", stdin);

    long long n, e;
    cin >> n >> e;
    long long u, v;
    long long w;
    long long freq[100000];
    vector<vector<pair<long long, long long>>> a(100000);

    vtree *vEB = new class vtree(65536);

    for(long long i = 0; i < e; i++){
        cin >> u;
        cin >> v;
        cin >> w;
        if(freq[w]==0){

            vEB->insert(w);

            freq[w]=1;
        }
        else{
            freq[w]++;
        }
        a[w].push_back(make_pair(u,v));

    }

    clock_t t;
    t = clock();

    disjoint_sets ds(n+1);

    long long count = 1;
    long long mst_weight = 0;
    long long min_weight;
    while(count < n){

        min_weight = vEB->find_min();
        if(freq[min_weight]==1){
            freq[min_weight]--;
            vEB->del(min_weight);
        }
        else{
            freq[min_weight]--;
        }

        auto itr = a[min_weight].rbegin();
        u = itr->first;
        v = itr->second;
        a[min_weight].pop_back();

        long long set_u = ds.find(u);
        long long set_v = ds.find(v);

        if (set_u != set_v)
        {
            mst_weight += min_weight;
            ds.merge(set_u, set_v);
            count++;
        }
    }
    t = clock() - t;

    cout << mst_weight << endl << endl;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("%f\n", time_taken);

    return 0;
}







