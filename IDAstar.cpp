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

bool isFinished(State st){
	for(int i = 0; i < N2; i++){
		if(st.value[i] != answer[i]) return false;
	}
	return true;
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

int dfs(State st,int depth,int preD,int limit){
	int sx,sy;//スペースの位置
    int tx,ty;//入れ替える位置
	int ans;
	State tmp;
	
	if(depth + h(st) > limit)	return -1;
	if(isFinished(st)) return depth;
		
	count++;
	
	sx = st.space % N;
    sy = st.space / N;
	
	for(int i = 0; i < 4; i++){
		tx = sx + dx[i];
		ty = sy + dy[i];
		if(tx < 0 ||tx >= N || ty < 0 || ty >= N)continue;
		if(abs(preD - i) == 2)continue;//元の位置に戻ろうとしたら飛ばす
		tmp = st;
		swap(st.value[sx + sy * N],st.value[tx + ty * N]);
        st.space = ty * N + tx;
		ans = dfs(st , depth + 1 , i , limit);	
		if(ans != -1){
			return ans;
		}
		st = tmp;
	}
	return -1;
}

int IDS(State st){
	int ans;
	for(int limit = 5; limit < LIMIT; limit++){
		if((ans = dfs(st , 0 , -10 , limit)) != -1){
			return ans;
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
    
    cout<<IDS(init)<<endl;
    cout<<count<<endl;

    return 0;
}
