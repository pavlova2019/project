#include <iostream>
#include <set>

#define n 4

using namespace std;

int p[n][n] = { // we are studying this particular profile
{1, 2, 3, 4},
{1, 2, 3, 4}, 
{1, 3, 2, 4},
{1, 4, 3, 2}
};

int now[n]; // item i belings to agent now[i-1]
int util[n]; // agent i has util[i-1] points
int ban[n][n]; // ban[i][j] = 1, if agent i can't steal item j no more
int order[n], used[n];

int get_next(int num){
	int st = 0;
	for (int i=0;i<n;i++) if (!ban[num-1][p[num-1][i]-1]) {
		int item = p[num-1][i]-1; // first most valuable item available
		if (now[item]) { // means that some other agent has this item, so we steal
			st = now[item];
			util[st-1] = 0;
			ban[st-1][item] = 1;
		}
		now[item] = num;
		util[num-1] = n-1 - i;
		break;
	}
	return st;
}

int get_next_2(int num){
	int st = 0, f = 0, it = 0;
	for (int i=0;i<n;i++) if (!ban[num-1][p[num-1][i]-1]) {
		if (f){
			int item = p[num-1][i]-1; // second most valuable item available
			if (now[item]) {
				st = now[item];
				util[st-1] = 0;
				ban[st-1][item] = 1;
			}
			now[item] = num;
			util[num-1] = n-1 - i;
			f = 0;
			break;
		}
		f = p[num-1][i];
		it = i;
	}
	if (f){ // if we didn't find the second most valuable item, we take the first
		int item = f - 1;
		if (now[item]) {
			st = now[item];
			util[st-1] = 0;
			ban[st-1][item] = 1;
		}
		now[item] = num;
		util[num-1] = n-1 - it;
	}
	return st;
}

void YS(int from, int it){ //from = 0 (if we didn't steal during the previous round), id of agent we stole from -- otherwise
	if (it == n) return;
	int num = order[it]; // next agent
	if (from) num = from; // agent we stoli from (if we did)
	int nxt = get_next(num); // next agent or 0
	if (nxt) YS(nxt, it);
	else YS(0, it+1);
}

void S2(int from, int it){ // the same as YS
	if (it == n) return;
	int num = order[it];
	if (from) num = from;
	int nxt = get_next_2(num);
	if (nxt) S2(nxt, it);
	else S2(0, it+1);
}

void init(bool f){
	for (int i=0;i<n;i++){
		now[i] = util[i] = 0;
		if (f) order[i] = used[i] = 0;
		for (int j=0;j<n;j++) ban[i][j] = 0;
	}
}

int main() {
	set <int> s; // set of numbers like "1234"
	double ys = 0, s2 = 0;
	while (s.size() < 24){
		init(1);
		int cnt = n, number = 0;
		while (cnt){
			int x = rand() % n;
			if (!used[x]){
				used[x] = 1;
				cnt--;
				order[cnt] = x+1;
				number *= 10;
				number += x;
			}
		}
		if (s.find(number) == s.end()){
			s.insert(number);
		}
		else continue;
		YS(0, 0);
		int sum = 0; // total utility for the given order, YS
		for (int i=0;i<n;i++) sum += util[i];
		ys += (sum + 0.0);
		init(0);
		S2(0, 0); // total utility for the given order, YS + our strategy
		int sum_2 = 0;
		for (int i=0;i<n;i++) sum_2 += util[i];
		s2 += (sum_2 + 0.0);
	}
	cout << "The arithmetic mean of total utilities for YS: " << ys/24 << '\n';
	cout << "The arithmetic mean of total utilities for S2: " << s2/24 << '\n';
}
