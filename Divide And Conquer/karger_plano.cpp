// Clase: Topics on Advanced Algoritmos
// Karger
// Juliana Nieto
// Octubre 3
#include <bits/stdc++.h>
using namespace std;
typedef pair<int, int> pii;

struct DSU {
    vector<int> p, r; // padre , rango
    DSU(int n = 0){
        init(n);
    }
    void init(int n){
        p.resize(n); // n padres
        r.assign(n,0); 
        iota(p.begin(), p.end(), 0); // n padres enumerado 0, 1, ..., n-1. cada uno es su propio padre
    }
    int find(int x){
        if (p[x] != x){
            p[x] = find(p[x]); // compresion de caminos
        }
        return p[x];
    }
    
    bool unite(int a, int b){
        // encontrar raices
        a = find(a);
        b = find(b);
        if(a == b) return false; // ya estan en el mismo conjunto
        // decidir quien es padre
        if (r[a] < r[b]) swap(a, b);
    
        p[b] = a;                    // agregar b bajo a
        if (r[a] == r[b]) r[a]++;   
        return true;
        return true;
    }
};

int karger_trial(const vector<pii>& edges, int n, mt19937 &rng){
    DSU dsu(n);
    int comps = n; // cada nodo es un componente
    // obtener numeros aleatorios
    uniform_int_distribution<int> pick(0, (int)edges.size()-1);
    
    // contraer hasta solo tener 2 componentes
    while(comps > 2){
        auto [u0,v0] = edges[pick(rng)]; // arista aleatoria
        int u = dsu.find(u0);
        int v = dsu.find(v0);
        if(u == v) continue; // bucle de u a u (self loop)
        dsu.unite(u,v);
        comps--;
        
    }
    
    int cut = 0;
    for(auto [u,v] : edges){
        if (dsu.find(u) != dsu.find(v)) cut++;
    }
    return cut;
}

// correr karger con generadores de numeros aleatorios 
int karger_min_cut(const vector<pair<int,int>>& edges, int n, int trials){
    random_device rd;
    
    seed_seq seq{rd(), rd(), rd(), rd(), rd(), (unsigned)chrono::high_resolution_clock::now().time_since_epoch().count()};
    mt19937 rng(seq);

    int best = INT_MAX;
    for(int t=0; t<trials; ++t){
        best = min(best, karger_trial(edges, n, rng));
    }
    return best;
}

int main()
{
    int n, m;
    cin >> n >> m;
    vector<pii> edges(m);

    for (int i = 0; i < m; ++i) {
        int u, v; 
        cin >> u >> v;
        // Convertir de 1..n a 0..n-1
        u--; 
        v--;
        edges[i] = {u,v};
    }
    int trials = max(1, n*n);
    int ans = karger_min_cut(edges, n, trials);
    cout << ans << "\n";
}