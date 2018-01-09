#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <utility>
#include <ctime>
#include <climits>
#include <set>
#include <chrono>

#define prune 1
using namespace std;
using namespace std::chrono;

ofstream op1("output.txt");

struct cmp {
    bool operator()(pair<int, int> p1, pair<int, int> p2) {
        return p1.second < p2.second;
    }
};

class Board {

public:

    vector<vector<char>> arr;
    int n, hue;

    queue<pair<int, int>> points;
    priority_queue<pair<int, int>, vector<pair<int, int>>, cmp> moves;

    explicit Board(int x) {
        n = x;
        hue = 0;
    }

    void gravity() {

        for (int col = 0; col < n; ++col) {
            int x = n - 1;
            for (int row = n - 1; row >= 0; --row) {
                //cout<<a[i][j];

                if (arr[row][col] != '*') {
                    arr[x][col] = arr[row][col];
                    --x;
                }
            }

            while (x >= 0) {
                arr[x][col] = '*';
                --x;
            }
        }
    }

    bool isTerminal() {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j)
                if (arr[i][j] != '*')
                    return false;
        }
        return true;
    }

    void printBoard() {

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                cout << arr[i][j];
            }
            cout << endl;
        }
    }

    void outputBoard() {

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                op1 << arr[i][j];
            }
            op1 << endl;
        }
    }

    void printPoints() {
        queue<pair<int, int>> t = points;
        pair<int, int> temp;

        while (!t.empty()) {
            temp = t.front();
            t.pop();

            cout << (char) (temp.first % n + 'A') << temp.first / n + 1 << " " << temp.second << endl;
        }
    }

    void printNextMoves() {
        priority_queue<pair<int, int>, vector<pair<int, int>>, cmp> t = moves;
        pair<int, int> pt;

        while (!t.empty()) {
            pt = t.top();

            t.pop();

            cout << (char) (pt.first % n + 'A') << pt.first / n + 1 << "\t" << pt.second << endl;
        }
    }


    void allNextMoves() {

        vector<vector<bool>> done(n, vector<bool>(n, false));
        int count, r, c;
        //set<pair<int,int>> moves;

        pair<int, int> pos;

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {

                if (done[i][j] || arr[i][j] == '*') continue;

                count = 0;

                queue<pair<int, int>> q1;

                char p = arr[i][j];

                q1.push(make_pair(i, j));

                while (!q1.empty()) {
                    pos = q1.front();
                    q1.pop();

                    r = pos.first;
                    c = pos.second;

                    done[r][c] = true;
                    count++;

                    if (r + 1 < n && arr[r + 1][c] == p && done[r + 1][c] == 0) {
                        q1.push(make_pair(r + 1, c));
                        done[r + 1][c] = true;
                    }
                    if (r - 1 >= 0 && arr[r - 1][c] == p && done[r - 1][c] == 0) {
                        q1.push(make_pair(r - 1, c));
                        done[r - 1][c] = true;
                    }
                    if (c - 1 >= 0 && arr[r][c - 1] == p && done[r][c - 1] == 0) {
                        q1.push(make_pair(r, c - 1));
                        done[r][c - 1] = true;
                    }
                    if (c + 1 < n && arr[r][c + 1] == p && done[r][c + 1] == 0) {
                        q1.push(make_pair(r, c + 1));
                        done[r][c + 1] = true;
                    }
                }

                moves.push(make_pair(i * n + j, count * count));
            }
        }
    }
};

Board nextState(Board b, int x, int y, bool max) {

    //b.gravity();

    if (b.arr[x][y] == '*') {
        return b;
    }

    queue<pair<int, int>> q1;
    pair<int, int> pos;
    int r, c, count = 0;
    char p = b.arr[x][y];

    q1.push(make_pair(x, y));

    while (!q1.empty()) {
        pos = q1.front();
        q1.pop();

        r = pos.first;
        c = pos.second;

        b.arr[r][c] = '*';
        count++;

        if (r + 1 < b.n && b.arr[r + 1][c] == p) {
            q1.push(make_pair(r + 1, c));
            b.arr[r + 1][c] = '*';
        }
        if (r - 1 >= 0 && b.arr[r - 1][c] == p) {
            q1.push(make_pair(r - 1, c));
            b.arr[r - 1][c] = '*';
        }
        if (c - 1 >= 0 && b.arr[r][c - 1] == p) {
            q1.push(make_pair(r, c - 1));
            b.arr[r][c - 1] = '*';
        }
        if (c + 1 < b.n && b.arr[r][c + 1] == p) {
            q1.push(make_pair(r, c + 1));
            b.arr[r][c + 1] = '*';
        }
    }

    if (max)
        b.hue += (count * count);
    else
        b.hue = b.hue - (count * count);

    b.gravity();

    //TODO
    if (b.points.size() != 1)
        b.points.push(make_pair(x * b.n + y, b.hue));

    b.moves = priority_queue<pair<int, int>, vector<pair<int, int>>, cmp>();

    return b;
}

Board minimaxAll(Board b, bool ismax, int depth, int h, int alpha, int beta) {

    int n = b.n;

    if (depth == h || b.isTerminal()) {
        return b;
    }

    if (ismax) {
        b.allNextMoves();

        Board max(n);
        max.hue = INT_MIN;

        priority_queue<pair<int, int>, vector<pair<int, int>>, cmp> pq = b.moves;
        pair<int, int> ptemp;
        Board btemp(n);


        //cout<<fc<<":"<<pq.size()<<":"<<b.hue<<endl;

        /*if(fc==226){
            
            b.printBoard();
            
            b.printPoints();
            
            while(!pq.empty()){
                cout<<pq.top().first<<" "<<pq.top().second<<endl;
                pq.pop();
            }
        
        }*/

        while (!pq.empty()) {
            //for(int i=0;i<5;++i){
            ptemp = pq.top();
            pq.pop();

            //b.printBoard();
            //b.printPoints();
            //b.printNextMoves();

            //cout<<"i="<<i<<endl;
            //cout<<ptemp.first<<" "<<ptemp.second<<" at depth "<<depth<<endl;

            btemp = nextState(b, ptemp.first / n, ptemp.first % n, true);

            btemp = minimaxAll(btemp, false, depth + 1, h, alpha, beta);

            if (btemp.hue > max.hue) {
                //cout<<"max: "<<max.hue<<" "<<"btemp: "<<btemp.hue<<endl;
                max = btemp;
            }

#if prune
            if (alpha < max.hue) {
                alpha = max.hue;
            }

            if (beta <= alpha) {
                break;
            }
#endif
        }

        return max;
    } else {
        b.allNextMoves();

        Board min(n);
        min.hue = INT_MAX;

        priority_queue<pair<int, int>, vector<pair<int, int>>, cmp> pq = b.moves;
        pair<int, int> ptemp;
        Board btemp(n);

        while (!pq.empty()) {
            //for(int i=0;i<5;++i){
            ptemp = pq.top();
            pq.pop();

            btemp = nextState(b, ptemp.first / n, ptemp.first % n, false);

            btemp = minimaxAll(btemp, true, depth + 1, h, alpha, beta);

            if (btemp.hue < min.hue) {
                min = btemp;
            }

#if prune
            if (beta > min.hue) {
                beta = min.hue;
            }

            if (beta <= alpha) {
                break;
            }
#endif
        }

        return min;
    }
}

int main() {
 	steady_clock::time_point t1 = steady_clock::now();
    ifstream ip1("input.txt");
    int n, p, depth = 3;
    float t;

    ip1 >> n;
    ip1 >> p;
    ip1 >> t;

    ifstream cal("calibrate.txt");

    int calline = 0;
    string line;
    while (getline(cal, line)) {
        ++calline;
    }

    //cout << "calline:" << calline << endl;

    cal.clear();
    cal.seekg(0, ios::beg);

    if (cal && calline >= n) {
        //cout << "true\n";
        for (int i = 0; i < n; ++i) {
            cal >> depth;
        }
    } else {
        //cout << "false\n";
    }

    //cout << depth << endl;

    Board initboard(n);

    for (int i = 0; i < n; ++i) {
        vector<char> temp;
        char t1;
        for (int j = 0; j < n; ++j) {
            ip1 >> t1;
            temp.push_back(t1);
        }
        initboard.arr.push_back(temp);
    }

    //recursion

 	if(t<3 && n>=18)
    	depth=2;
	 
 
    //for (int xx = 1; xx <= 6; ++xx) {

    
#if 1
    Board ans = minimaxAll(initboard, true, 0, depth, INT_MIN, INT_MAX);

    //ans.printBoard();
    //ans.printPoints();
    //ans.allNextMoves();
    //ans.printNextMoves();

    //cout<<"Output\n";

    //cout<<ans.hue<<endl;

    int xy = ans.points.front().first;
    int x = xy / n;
    int y = xy % n;

    //cout<<xx<<": ";
    //cout<<x<<y<<endl;
 	cout<<endl;
    cout << (char) (y + 'A') << x + 1 << endl;
    op1 << (char) (y + 'A') << x + 1 << endl;

    //nextState(initboard, x, y, true).printBoard();

    nextState(initboard, x, y, true).outputBoard();
#endif

    steady_clock::time_point t2 = steady_clock::now();
    auto duration = duration_cast<microseconds>(t2 - t1).count();

    //}

    op1.close();
	cout << "Time: " << (double) duration / 1000000 << endl;
    return 0;
}
