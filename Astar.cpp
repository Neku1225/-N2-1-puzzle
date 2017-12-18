#include <stdio.h>
#include <iostream>
#include <cmath>
#include <string>
#include <cassert>
#include <random>
#include <map>
#include <queue>

using namespace std;
static const int N = 3;
static const int N2 = N*N; // N2-1パズル
static const int LIMIT = 30;

// 盤面の移動(上、右、下、左)
static const int dx[4] = {0, -1, 0, 1};
static const int dy[4] = {1, 0, -1, 0};

// 盤面(使用する値は1~(N^2-1)とN^2(スペース))
static const int answer[N2] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

struct State{
    int value[N2];//盤面
    int space;
    int h;
    int g;
    int cost;
    bool operator < (const State &p) const {
    for(int i = 0; i < N2; i++){
      if(value[i] == p.value[i]) continue;
      return value[i] < p.value[i];
    }
    return false;
  }
};

int count;
int pattern;

bool operator >(const State &st1 ,const State &st2){
    return st1.cost > st2.cost;
}

//正しい位置にない数
int h1(State st){
    int num = 0;
    for(int i = 0; i< N2;i++){
        if(st.value[i] != answer[i])num++;
    }
    return num;
}

//マンハッタン距離
int h2(State st){
    int md = 0;
    for(int i = 0; i < N2 ; i++){
        if(st.value[i] != answer[i]){
            md += abs(((answer[st.value[i] - 1] - 1) % N) - i % N) + abs(((answer[st.value[i] - 1] - 1) / N) - i / N);
        }
    }
    return md;
}

int h(State st){
	switch(pattern){
		case 0:
		return 0;
		break;
		case 1:
		return h1(st);
		break;
		case 2:
		return h2(st);
		break;
	}
	return -1;
}

int astar(State st){
    priority_queue<State, vector<State>, greater<State> > PQ;
    State st_base,st_after;
    int sx,sy;//スペースの位置
    int tx,ty;//入れ替える位置
    st.h = h(st);
    st.g = 0;
    st.cost = st.h + st.g;
    map<State, bool> V;
    PQ.push(st);
    count = 0;
    
    while(!PQ.empty()){
        st_base = PQ.top();
        PQ.pop();
        count++;
        if(h1(st_base) == 0) return st_base.g;//何手で解けるか返す
        V[st_base] = true;//今の状態を探索済みにする
        
        sx = st_base.space % N;
        sy = st_base.space / N;
        for(int i = 0; i < 4; i++){
            tx = sx + dx[i];
            ty = sy + dy[i];
            if(tx < 0 ||tx >= N || ty < 0 || ty >= N)continue;
            st_after = st_base;
            swap(st_after.value[sx + sy * N],st_after.value[tx + ty * N]);
            st_after.space = ty * N + tx;
            
            if(!V[st_after]){
                st_after.h = h(st_after);
                st_after.g++;
                st_after.cost = st_after.h + st_after.g;
                PQ.push(st_after);
            }
        }
    }
    
    return -1;
}

int main(int argc, char *argv[]){
    random_device rd;
    mt19937 mt(rd());
    State init;
    bool flag = true;
    int temp;
	
	pattern = atoi(argv[1]);
    
    /*パズルの数字をかぶらないようにランダムに決める*/
    for(int i = 0; i < N2 ;){
        flag = true;
        temp = 1 + mt() % N2 ;
        for(int j = 0; j < i; j++){
            if(init.value[j] == temp){
                flag = false;
                break;
            }
        }
        if(flag){
            init.value[i] = temp;
            if(init.value[i] == N2)init.space = i;
            i++;
        }
    }
        
    for(int i = 0; i < N2; i++){
        cout << init.value[i] << " ";
        if(i % N == N - 1)cout << endl;
    }
    
    cout<<astar(init)<<endl;
    cout<<count<<endl;

    return 0;
}
