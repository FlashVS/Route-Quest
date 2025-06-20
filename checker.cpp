#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>

#pragma GCC optimize("Ofast,unroll-loops")

#define int long long
#define all(v) v.begin(), v.end()
#define rall(v) v.rbegin(), v.rend()

using namespace std;
using namespace __gnu_pbds;

template<class T> using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>; // find_by_order, order_of_key

int n, t, m, k, f;
vector<vector<int>> v;
vector<int> h, d, fs;

vector<int> hash_h, hash_d, hash_fs;
vector<vector<int>> floyd_minm;
vector<vector<int>> floyd_minm_tree;
vector<vector<int>> closest_fuel_station;
map<int, int> mp;

vector<int> temp_path;
vector<int> path;
vector<int> final_path;
int temp_cost;
int final_cost;

void reset(){
    v.clear();
    h.clear();
    d.clear();
    fs.clear();
    v.resize(t + 1, vector<int>(t + 1, 1e18));
    h.resize(n + 1);
    d.resize(n + 1);
    fs.resize(k + 1);

    hash_h.clear();
    hash_h.resize(t + 1, 0);
    hash_d.clear();
    hash_d.resize(t + 1, 0);
    hash_fs.clear();
    hash_fs.resize(t + 1, 0);

    floyd_minm.clear();
    floyd_minm.resize(t + 1, vector<int>(t + 1, 1e18));

    floyd_minm_tree.clear();
    floyd_minm_tree.resize(t + 1, vector<int>(t + 1, -1));

    closest_fuel_station.clear();
    closest_fuel_station.resize(t + 1, vector<int>(k, 1e18));

    mp.clear();

    final_path.clear();
    final_cost = 1e18;
}

void input(){
    cin >> n >> t >> m >> k >> f;

    reset();

    for(int i = 0; i < n; i++){
        cin >> h[i];
        hash_h[h[i]] = 1;
    }
    for(int i = 0; i < n; i++){
        cin >> d[i];
        hash_d[d[i]] = 1;
        mp[d[i]] = h[i];
    }

    for(int i = 0; i < k; i++){
        cin >> fs[i];
        hash_fs[fs[i]] = 1;
    }

    for(int i = 0; i < m; i++){
        int a, b, c;
        cin >> a >> b >> c;
        v[a][b] = c;
        v[b][a] = c;
    }
    for(int i = 0; i <= t; i++){
        v[i][i] = 0;
    }
}

void solve(int tc){
    string op = "output/output_" + to_string(tc) + ".txt";

    freopen(op.c_str(), "r", stdin);

    int tempp;
    cin >> tempp;

    int l;
    cin >> l;

    vector<int> a(l);
    for(int i = 0; i < l; i++){
        cin >> a[i];
    }

    int curr = a[0];
    int left = f;

    set<int> hb, dt;
    for(int i = 0; i < n; i++){
        dt.insert(d[i]);
    }

    int ans = 0;
    for(int i = 1; i < l; i++){
        int next = a[i];

        if(hash_h[curr]){
            hb.insert(curr);
        }

        if(hash_d[curr]){
            if(hb.find(mp[curr]) != hb.end()){
                dt.erase(curr);
            }
        }

        if(v[curr][next] == 1e18){
            cout << "non edge" << endl;
            return;
        }
        if(v[curr][next] > left){
            cout << "not enough fuel" << endl;
            return;
        }
        ans += v[curr][next];
        left -= v[curr][next];
        curr = next;

        if(hash_fs[next]){
            left = f;
        }
    }

    if(hash_d[curr]){
        if(hb.find(mp[curr]) != hb.end()){
            dt.erase(curr);
        }
    }

    cout << endl;
    if(dt.size()){
        cout << "all houses not traversed" << endl;
    }
    else{
        cout << "OK" << endl;
        cout << ans << endl;
    }
}

signed main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    for(int i = 0; i < 3; i++){
        string s = "tc/input_" + to_string(i) + ".txt";
        freopen(s.c_str(), "r", stdin);
        
        input();
        solve(i);
    }

    return 0;
}
