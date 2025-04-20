#include<bits/stdc++.h>

using namespace std;

#pragma GCC optimize("Ofast,unroll-loops")

#define int long long
#define all(v) v.begin(), v.end()
#define rall(v) v.rbegin(), v.rend()

int overallPos;
int achieved;
map<pair<int, int>, int> pts;

struct node{
    int x, y, val;
};

int n, m; // n = crstal cnt, m = mine count
vector<node> crystal, mine;
int totalPositive;
int totalNegative; //negative
int best165points;
int worst165points;

void _input(){

    pts.clear();
    cin >> n;
    crystal.clear();
    crystal.resize(n);

    totalPositive = 0;
    totalNegative = 0;
    vector<int> t;

    for(int i = 0; i < n; i++){
        cin >> crystal[i].x >> crystal[i].y >> crystal[i].val;
        overallPos += crystal[i].val;
        totalPositive += crystal[i].val;
        t.push_back(crystal[i].val);

        pts[{crystal[i].x, crystal[i].y}] += crystal[i].val;
    }

    best165points = 0;

    sort(rall(t));
    for(int i = 0; i < min(166ll, (int)t.size()); i++) best165points += t[i];

    t.clear();

    cin >> m; 
    mine.clear();
    mine.resize(m);

    for(int i = 0; i < m; i++){
        cin >> mine[i].x >> mine[i].y >> mine[i].val;
        mine[i].val *= -1;
        t.push_back(mine[i].val);
        totalNegative += mine[i].val;

        pts[{mine[i].x, mine[i].y}] += mine[i].val;
    }

    worst165points = totalPositive + totalNegative;

    sort(all(t));
    for(int i = 0; i < min(166ll, (int)t.size()); i++) worst165points -= t[i];

    achieved = max(worst165points, best165points);
}

//debug :
int stripperX(int threshold){
    map<int, int> m;

    for(auto i : crystal){
        m[i.x] += i.val;
    }
    for(auto i : mine){
        m[i.x] += i.val;
    }

    vector<int> v;
    int curr = 0;
    for(auto i : m){
        if(i.second >= threshold){
            curr += i.second;
        }
        else{
            v.push_back(curr);
            curr = 0;
        }
    }

    v.push_back(curr);
    sort(rall(v));

    curr = 0;
    for(int i = 0; i < min((int) v.size(), 250ll); i++){
        curr += abs(v[i]);
    }

    achieved = max(achieved, curr);
    return curr;
}

void generate_stripperX(int threshold){
    map<int, int> m;
    
    map<double, double> e1, e2;

    for(auto i : crystal){
        m[i.x] += i.val;
    }
    for(auto i : mine){
        m[i.x] += i.val;
    }

    vector<pair<int, pair<int, int>>> v;
    int curr = 0;

    int st = 0;
    for(int i = 0; i <= 1e4; i++){
        if(m[i] >= threshold){
            curr += m[i];
        }
        else{
            if(st == i){
                curr = 0;
                st++;
                continue;
            }
            
            v.push_back({curr, {st, i - 1}});
            curr = 0;
            st = i + 1;
        }
    }

    if(curr > 0) v.push_back({curr, {st, 10000}});

    sort(rall(v));

    int cost = 0;

    vector<pair<pair<double, double>, pair<double, double>>> edgeH, edgeV; 
    set<pair<pair<double, double>, pair<double, double>>> fin;

    for(int i = 0; i < min((int) v.size(), 250ll); i++){
        if(v[i].first <= 0) break;

        double st = v[i].second.first;
        double en = v[i].second.second;

        if(st == en) en += 0.5;

        edgeV.push_back({{st, 0}, {st, 1e4}});
        edgeV.push_back({{en, 0}, {en, 1e4}});

        edgeH.push_back({{st, 0}, {en, 0}});
        edgeH.push_back({{st, 1e4}, {en, 1e4}});
        e1[st] = en;
        e1[en] = st;

        e2[st] = en;
        e2[en] = st;

        cost += v[i].first;
    }

    for(auto i : edgeV) fin.insert(i);
    for(auto i : edgeH) fin.insert(i);

    sort(all(edgeH));
    sort(all(edgeV));

    int ex = 0;

    for(int i = 0; i + 2 < edgeH.size(); i += 2){
        int up = 0;
        int down = 0;

        for(int j = (int)edgeH[i].second.first + 1; j < (int)edgeH[i + 2].first.first; j++){
            up += pts[{j, 0}];
        }
        for(int j = (int)edgeH[i].second.first + 1; j < (int)edgeH[i + 2].first.first; j++){
            down += pts[{j, 1e4}];
        }

        double pehle = edgeH[i].second.first;
        double baad = edgeH[i + 2].first.first;

        if(up >= down){

            fin.erase({{e1[pehle] , 0}, {pehle, 0}});
            fin.erase({{baad, 0}, {e1[baad], 0}});
            fin.insert({{e1[pehle], 0}, {e1[baad], 0}});

            fin.insert({{pehle, 0.5}, {baad, 0.5}});

            fin.erase({{pehle, 0}, {pehle, 1e4}});
            fin.erase({{baad, 0}, {baad, 1e4}});

            fin.insert({{pehle, 0.5}, {pehle, 1e4}});
            fin.insert({{baad, 0.5}, {baad, 1e4}});

            e1[e1[baad]] = e1[pehle];
            
            ex += up;
        }
        else{

            fin.erase({{e2[pehle] , 1e4}, {pehle, 1e4}});
            fin.erase({{baad, 1e4}, {e2[baad], 1e4}});
            fin.insert({{e2[pehle], 1e4}, {e2[baad], 1e4}});

            fin.insert({{pehle, 9999.5}, {baad, 9999.5}});

            fin.erase({{pehle, 0}, {pehle, 1e4}});
            fin.erase({{baad, 0}, {baad, 1e4}});

            fin.insert({{pehle, 0}, {pehle, 9999.5}});
            fin.insert({{baad, 0}, {baad, 9999.5}});

            e2[e2[baad]] = e2[pehle];

            ex += down;
        }
    }

    achieved = max(achieved, cost + ex);

    // cout << "percent_top250_strips = " << (cost + ex) * 100 / (totalPositive + 0.0) << "%" << endl;
    int f = fin.size();
    cout << cost + ex << endl;
    cout << f << ", " << f << endl;
    for(auto i : fin){
        cout << "(" << i.first.first << ", " << i.first.second << "), (" << i.second.first << ", " << i.second.second << ")" << endl;
    }
}

int stripperY(int threshold){
    map<int, int> m;

    for(auto i : crystal){
        m[i.y] += i.val;
    }
    for(auto i : mine){
        m[i.y] += i.val;
    }
    
    vector<int> v;
    int curr = 0;
    for(auto i : m){
        if(i.second >= threshold){
            curr += i.second;
        }
        else{
            v.push_back(curr);
            curr = 0;
        }
    }

    sort(rall(v));

    curr = 0;
    for(int i = 0; i < min((int) v.size(), 250ll); i++){
        curr += abs(v[i]);
    }

    achieved = max(achieved, curr);
    return curr;
}

void generate_stripperY(int threshold){
    map<int, int> m;
    
    map<double, double> e1, e2;

    for(auto i : crystal){
        m[i.y] += i.val;
    }
    for(auto i : mine){
        m[i.y] += i.val;
    }

    vector<pair<int, pair<int, int>>> v;
    int curr = 0;

    int st = 0;
    for(int i = 0; i <= 1e4; i++){
        if(m[i] >= threshold){
            curr += m[i];
        }
        else{
            if(st == i){
                curr = 0;
                st++;
                continue;
            }
            
            v.push_back({curr, {st, i - 1}});
            curr = 0;
            st = i + 1;
        }
    }

    if(curr > 0) v.push_back({curr, {st, 10000}});

    sort(rall(v));

    int cost = 0;

    vector<pair<pair<double, double>, pair<double, double>>> edgeH, edgeV; 
    set<pair<pair<double, double>, pair<double, double>>> fin;

    for(int i = 0; i < min((int) v.size(), 250ll); i++){
        if(v[i].first <= 0) break;

        double st = v[i].second.first;
        double en = v[i].second.second;

        if(st == en) en += 0.5;

        set<int> s;
        auto it = s.begin();

        edgeV.push_back({{st, 0}, {st, 1e4}});
        edgeV.push_back({{en, 0}, {en, 1e4}});

        edgeH.push_back({{st, 0}, {en, 0}});
        edgeH.push_back({{st, 1e4}, {en, 1e4}});
        e1[st] = en;
        e1[en] = st;

        e2[st] = en;
        e2[en] = st;

        cost += v[i].first;
    }

    for(auto i : edgeV) fin.insert(i);
    for(auto i : edgeH) fin.insert(i);

    sort(all(edgeH));
    sort(all(edgeV));

    int ex = 0;

    for(int i = 0; i + 2 < edgeH.size(); i += 2){
        int up = 0;
        int down = 0;

        for(int j = (int)edgeH[i].second.first + 1; j < (int)edgeH[i + 2].first.first; j++){
            up += pts[{0, j}];
        }
        for(int j = (int)edgeH[i].second.first + 1; j < (int)edgeH[i + 2].first.first; j++){
            down += pts[{1e4, j}];
        }

        double pehle = edgeH[i].second.first;
        double baad = edgeH[i + 2].first.first;

        if(up >= down){

            fin.erase({{e1[pehle] , 0}, {pehle, 0}});
            fin.erase({{baad, 0}, {e1[baad], 0}});
            fin.insert({{e1[pehle], 0}, {e1[baad], 0}});

            fin.insert({{pehle, 0.5}, {baad, 0.5}});

            fin.erase({{pehle, 0}, {pehle, 1e4}});
            fin.erase({{baad, 0}, {baad, 1e4}});

            fin.insert({{pehle, 0.5}, {pehle, 1e4}});
            fin.insert({{baad, 0.5}, {baad, 1e4}});

            e1[e1[baad]] = e1[pehle];
            
            ex += up;
        }
        else{

            fin.erase({{e2[pehle] , 1e4}, {pehle, 1e4}});
            fin.erase({{baad, 1e4}, {e2[baad], 1e4}});
            fin.insert({{e2[pehle], 1e4}, {e2[baad], 1e4}});

            fin.insert({{pehle, 9999.5}, {baad, 9999.5}});

            fin.erase({{pehle, 0}, {pehle, 1e4}});
            fin.erase({{baad, 0}, {baad, 1e4}});

            fin.insert({{pehle, 0}, {pehle, 9999.5}});
            fin.insert({{baad, 0}, {baad, 9999.5}});

            e2[e2[baad]] = e2[pehle];

            ex += down;
        }
    }

    achieved = max(achieved, cost + ex);

    int f = fin.size();
    cout << cost + ex << endl;
    // cout << " " << ex << endl;
    cout << f << ", " << f << endl;
    for(auto i : fin){
        cout << "(" << i.first.second << ", " << i.first.first << "), (" << i.second.second << ", " << i.second.first << ")" << endl;
    }
}
//debug^

//done
void generateBest165(){
    vector<pair<int, pair<int, int>>> t;

    for(auto i : crystal){
        t.push_back({pts[{i.x, i.y}], {i.x, i.y}});
    }

    sort(rall(t));

    int curr = 0;
    map<int, set<pair<int, int>>> s;

    int took = 0;
    for(int i = 0; i < t.size() && took < 166; i++){
        
        if(t[i].second.second == 0) continue;
        if(t[i].second.second == 1e4) continue;
        if(t[i].second.first == 0) continue;
        if(t[i].second.first == 1e4) continue;

        curr += t[i].first;

        if(s[t[i].second.first].find(t[i].second) != s[t[i].second.first].end()){
            continue;
        }

        s[t[i].second.first].insert(t[i].second);
        took++;
    }

    double minx = 1e4, maxx = 0;
    for(auto i : s){
        minx = min(minx, (double) i.first);
        maxx = max(maxx, (double) i.first);
    }

    double top = 1e4;
    int tempsc = 0;

    for(int i = minx; i <= maxx; i++){
        tempsc += pts[{i, 1e4}];
    }
    
    set<pair<pair<double, double>, pair<double, double>>> fin;

    if(tempsc < 0){
        top = 1e4 - 0.1;
    }

    if(minx != 0) minx -= 0.2;
    if(maxx != 1e4) maxx -= 0.1;

    fin.insert({{minx, top}, {maxx, top}});

    double lst = minx;

    for(auto i : s){
        if(i.first - 0.2 == minx){

            auto it = i.second.begin();
            double mxy = it -> second;

            mxy -= 0.1;
            fin.insert({{minx, top}, {minx, mxy}});

            double currx = minx;

            fin.insert({{currx, mxy}, {currx + 0.3, mxy}});
            fin.insert({{currx + 0.3, mxy}, {currx + 0.3, mxy + 0.2}});
            fin.insert({{currx + 0.3, mxy + 0.2}, {currx + 0.1, mxy + 0.2}});

            currx += 0.1;
            double curry = mxy + 0.2;

            int n1 = i.second.size();

            it = i.second.begin();
            it++;

            for(int j = 1; j < n1 && it != i.second.end(); j++){    
                int x1 = it -> first;;
                int y1 = it -> second;

                fin.insert({{currx, curry}, {currx, y1 - 0.1}});
                fin.insert({{currx, y1 - 0.1}, {x1 + 0.1, y1 - 0.1}});
                fin.insert({{x1 + 0.1, y1 - 0.1}, {x1 + 0.1, y1 + 0.1}});
                fin.insert({{x1 + 0.1, y1 + 0.1}, {currx, y1 + 0.1}});

                curry = y1 + 0.1;
                it++;
            }

            fin.insert({{currx, curry}, {currx, top - 0.1}});
            lst = currx;
        }
        else if(i.first - 0.1 == maxx){

            auto it = i.second.begin();
            double mxy = it -> second;
            mxy -= 0.1;

            double currx = i.first - 0.2;
            fin.insert({{lst, top - 0.1}, {currx, top - 0.1}});

            fin.insert({{currx, top - 0.1}, {currx, mxy}});
            fin.insert({{currx, mxy}, {currx + 0.3, mxy}});
            fin.insert({{currx + 0.3, mxy}, {currx + 0.3, mxy + 0.2}});
            fin.insert({{currx + 0.3, mxy + 0.2}, {currx + 0.1, mxy + 0.2}});

            currx += 0.1;

            double curry = mxy + 0.2;

            int n1 = i.second.size();

            it = i.second.begin();
            it++;

            for(int j = 1; j < n1 && it != i.second.end(); j++){
                int x1 = it -> first;
                int y1 = it -> second;

                fin.insert({{currx, curry}, {currx, y1 - 0.1}});
                fin.insert({{currx, y1 - 0.1}, {x1 + 0.1, y1 - 0.1}});
                fin.insert({{x1 + 0.1, y1 - 0.1}, {x1 + 0.1, y1 + 0.1}});
                fin.insert({{x1 + 0.1, y1 + 0.1}, {currx, y1 + 0.1}});

                curry = y1 + 0.1;
                it++;
            }

            fin.insert({{currx, curry}, {currx, top}});
            lst = currx;
        }
        else{
            auto it = i.second.begin();

            double mxy = it -> second;
            mxy -= 0.1;

            double currx = i.first - 0.2;
            fin.insert({{lst, top - 0.1}, {currx, top - 0.1}});

            fin.insert({{currx, top - 0.1}, {currx, mxy}});
            fin.insert({{currx, mxy}, {currx + 0.3, mxy}});
            fin.insert({{currx + 0.3, mxy}, {currx + 0.3, mxy + 0.2}});
            fin.insert({{currx + 0.3, mxy + 0.2}, {currx + 0.1, mxy + 0.2}});

            currx += 0.1;
            double curry = mxy + 0.2;

            int n1 = i.second.size();
            it = i.second.begin();
            it++;

            for(int j = 1; j < n1 && it != i.second.end(); j++){
                int x1 = it -> first;
                int y1 = it -> second;

                fin.insert({{currx, curry}, {currx, y1 - 0.1}});
                fin.insert({{currx, y1 - 0.1}, {x1 + 0.1, y1 - 0.1}});
                fin.insert({{x1 + 0.1, y1 - 0.1}, {x1 + 0.1, y1 + 0.1}});
                fin.insert({{x1 + 0.1, y1 + 0.1}, {currx, y1 + 0.1}});

                curry = y1 + 0.1;
                it++;
            }

            fin.insert({{currx, curry}, {currx, top - 0.1}});
            lst = currx;
        }
    }

    // cout << "percent = " << curr * 100 / (totalPositive + 0.0) << "%" << endl;
    cout << curr + max(0ll, tempsc) << endl;
    cout << fin.size() << ", " << fin.size() << endl;
    for(auto i : fin){
        cout << "(" << i.first.first << ", " << i.first.second << "), (" << i.second.first << ", " << i.second.second << ")" << endl;
    }

    achieved = max(achieved, curr + max(0ll, tempsc));
}

void generateWorst165(){
    vector<pair<int, pair<int, int>>> t;

    for(auto i : mine){
        t.push_back({pts[{i.x, i.y}], {i.x, i.y}});
    }

    sort(all(t));

    int curr = totalPositive + totalNegative;
    map<int, set<pair<int, int>>> s;

    int took = 0;
    for(int i = 0; i < t.size() && took < 166; i++){
        
        if(t[i].second.second == 0) continue;
        if(t[i].second.second == 1e4) continue;
        if(t[i].second.first == 0) continue;
        if(t[i].second.first == 1e4) continue;

        if(s[t[i].second.first].find(t[i].second) != s[t[i].second.first].end()){
            continue;
        }

        curr -= t[i].first;

        s[t[i].second.first].insert(t[i].second);

        took++;
    }

    double top = 1e4 - 0.1;
    
    set<pair<pair<double, double>, pair<double, double>>> fin;

    fin.insert({{1e4 - 0.1, top}, {1e4 - 0.1, 0.1}});
    fin.insert({{1e4 - 0.1, 0.1}, {0.1, 0.1}});
    fin.insert({{0.1, 0.1}, {0.1, top}});

    double lst = 0.1;

    for(auto i : s){
        auto it = i.second.begin();

        double mxy = it->second;
        mxy -= 0.1;

        double currx = i.first - 0.2;
        fin.insert({{lst, top}, {currx, top}});

        fin.insert({{currx, top}, {currx, mxy}});
        fin.insert({{currx, mxy}, {currx + 0.3, mxy}});
        fin.insert({{currx + 0.3, mxy}, {currx + 0.3, mxy + 0.2}});
        fin.insert({{currx + 0.3, mxy + 0.2}, {currx + 0.1, mxy + 0.2}});

        currx += 0.1;
        double curry = mxy + 0.2;

        int n1 = i.second.size();

        it++;

        it = i.second.begin();
        it++;

        for(int j = 1; j < n1 && it != i.second.end(); j++){    
            int x1 = it -> first;
            int y1 = it -> second;

            fin.insert({{currx, curry}, {currx, y1 - 0.1}});
            fin.insert({{currx, y1 - 0.1}, {x1 + 0.1, y1 - 0.1}});
            fin.insert({{x1 + 0.1, y1 - 0.1}, {x1 + 0.1, y1 + 0.1}});
            fin.insert({{x1 + 0.1, y1 + 0.1}, {currx, y1 + 0.1}});

            curry = y1 + 0.1;
            it++;
        }

        fin.insert({{currx, curry}, {currx, top}});
        lst = currx;
    }

    fin.insert({{lst, top}, {1e4 - 0.1, top}});

    for(int i = 0; i <= 1e4; i++){
        curr -= pts[{i, 1e4}];
        curr -= pts[{i, 0}];
    }

    for(int i = 1; i < 1e4; i++){
        curr -= pts[{0, i}];
        curr -= pts[{1e4, i}];
    }

    cout << curr << endl;
    cout << fin.size() << ", " << fin.size() << endl;
    for(auto i : fin){
        cout << "(" << i.first.first << ", " << i.first.second << "), (" << i.second.first << ", " << i.second.second << ")" << endl;
    }

    achieved = max(achieved, curr);
}
//done^

void inp_normal(){
    int ans = 0;
    int st = 0, en = 0;
    cin >> st >> en;

    for(int i = st; i <= en; i++){
        string in = "input";
        string op = "output";

        string tx = ".txt";x

        string num = to_string(i);
        if(num.size() == 1) num = "0" + num;

        in = "input/" + in + num + tx;
        op = "output/" + op + num + tx;

        const char *inp = in.c_str();
        const char *opp = op.c_str();

        freopen(inp, "r", stdin);
        freopen(opp, "w", stdout);

        achieved = 0;

        _input();   
        if(i == 10) continue;
        if(i == 11) continue;
        if(i == 12) continue;
        if(i == 14) continue;
        if(i == 16) continue;
        if(i == 18) continue;

        int stripX = 0;
        int threshX = 0;
        for(int j = 0; j >= -45000; j -= 1000){
            int temp = stripperX(j);
            if(stripX < temp){
                stripX = temp;
                threshX = j;
            }
        }

        int stripY = 0;
        int threshY = 0;
        for(int j = 0; j >= -45000; j -= 1000){
            int temp = stripperY(j);
            if(stripY < temp){
                stripY = temp;
                threshY = j;
            }
        }

        // generate_stripperX(threshX);

        if(achieved == stripX){
            cout << "X" << endl;
            generate_stripperX(threshX);
        }
        else if(achieved == stripY){
            cout << "Y" << endl;
            generate_stripperY(threshY);
        }
        else if(achieved == best165points){
            cout << "best" << endl;
            generateBest165();
        }
        else if(achieved == worst165points){
            cout << "worst" << endl;
            generateWorst165();
        }

        ans += achieved;
    }   

    freopen("output/ZZZZ.txt", "w", stdout);
    cout << ans << endl << overallPos << endl;
    cout << ans * 100 / (overallPos + 0.0) << "%" << endl;
}

signed main(){

    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    inp_normal();

    return 0;
}