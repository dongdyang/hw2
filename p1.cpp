#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<sstream>
#include<map>
#include<unordered_map>    
#include<math.h>
#include<queue>
#include<random>
#include<time.h>
#include<cstdint>
#include<limits.h>
#include<algorithm>
#define MAX_PLAYER 1
#define MIN_PLAYER 0

using namespace std;

struct Node {
	int x;
	int y;
	int fruit;
	int num;
	int rate;
};

int small = 0;

void init();
void dfs(int x, int y, vector<vector<int>>& visit, int &count, int fruit);
int alphaBeta(int alpha, int beta, int depth, int player);
void locate(int x, int y, int fruit);
void fruitFall();
vector<Node*> generator();
vector<vector<int>> board;
vector<Node*> path;
vector<vector<int>> HistoryTable;
Node* bestNextNode;
int n;
int DEEP = 3; 
float gtime = 300;

int main() {
	clock_t start, finish;
	start = clock();
	//Part 1: Init Read
	init();


	vector<Node*> nodeList = generator();
	int fruitleft=0;
	int listlen = nodeList.size();
	//cout << "length:" << listlen << endl;


	if (listlen > 250 || (gtime < 10 && n>20)){ 
	//	cout << " case 1 "<< endl;
		DEEP = 2;
	}
	else if ( (listlen > 120 && listlen <= 250) || (gtime < 30 && n>=18) ){
	//	cout << " case 2" << endl;
		DEEP = 3;

	}else if ( (listlen > 70 && listlen <= 120) || (n >= 18 && listlen <= 100) ) {
	//	cout << " case 3" << endl;
		DEEP = 4;
	}else if ( (listlen > 50 && listlen <= 70) || (n >= 18 && listlen <= 60) ){ 
	//	cout << " case 4" << endl;
		DEEP = 5;
	}else if ( (listlen > 30 && listlen <= 50 ) || (n >= 18 && listlen <= 30) ) { 
	//cout << " case 5" << endl;
		DEEP = 6;
	}else if ( listlen <= 30 || (n > 20 && listlen <= 15) ) { 
	//	cout << " case 6" << endl;
		DEEP = 7;
	}
	
	


	

	//Part 2
	int v = alphaBeta(INT_MIN, INT_MAX, DEEP, MAX_PLAYER);//for player: 1 means max, 0 means min

	//time limit
	ofstream fw;
	fw.open("./output.txt");
	//Part 3
	if (bestNextNode) {
		int x = bestNextNode->x;
		int y = bestNextNode->y;
		locate(x, y, board[x][y]);
		fruitFall();
		
		fw << char('A' + y) << 1 + x << endl;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (board[i][j] == -1) {
					fw << "*";
				}
				else {
					fw << board[i][j];
				}
			}
			if (i != n - 1) {
				fw << "\n";
			}
		}
		
	}
	finish = clock();
	fw << "\n" << bestNextNode->num * bestNextNode->num << endl;
	fw << (double)(finish - start) / CLOCKS_PER_SEC << endl;
//cout << (double)(finish - start) / CLOCKS_PER_SEC << endl;
	fw.close();




	return 0;




}




void locate(int x, int y, int fruit) {
	if (x < 0 || y < 0 || x >= n || y >= n || board[x][y] == -1 || board[x][y] != fruit)return;
	board[x][y] = -1;
	locate(x - 1, y, fruit);
	locate(x, y - 1, fruit);
	locate(x + 1, y, fruit);
	locate(x, y + 1, fruit);
}

void fruitFall() {
	for (int j = 0; j < n; j++) {
		int count = 0, i = n - 1;
		while (i >= 0) {
			if (board[i][j] == -1) {
				count++;
				i--;
			}
			else {
				if (count == 0) {
					i--;
					continue;
				}
				while (i >= 0 && board[i][j] != -1) {
					board[i + count][j] = board[i][j];
					board[i][j] = -1;
					i--;
				}
			}
		}
	}
}

int evaluation(int player) {
	int score1 = 0, score2 = 0;
	for (int i = 0; i < path.size(); i++) {
		if (i & 1) {
			score2 += path[i]->num * path[i]->num;
		}
		else {
			score1 += path[i]->num * path[i]->num;
		}
	}
	//if (player == MAX_PLAYER)
	//	return score2 - score1;
	//else
	return score1 - score2;
}


bool comp(const Node* n1, const Node* n2) {
	return (n1->rate + n1->num) > (n2->rate + n2->num);
}

int alphaBeta(int alpha, int beta, int depth, int player) {
	vector<Node*> nodeList = generator();

	

	if (depth == 0 || nodeList.empty()) {
		return evaluation(player);
	}
	//else if( nodeList.empty()){
	//	return INT_MAX;
	//}
	else {
		for (int i = 0; i < nodeList.size(); i++) {
			nodeList[i]->rate = HistoryTable[nodeList[i]->x][nodeList[i]->y];
		}
		sort(nodeList.begin(), nodeList.end(), comp);

		Node* localBestNode = NULL;
		if (MAX_PLAYER == player) {
			int v = INT_MIN;
			for (int i = 0; i < nodeList.size(); i++) {
				auto tmp = board;
				path.push_back(nodeList[i]);
				locate(nodeList[i]->x, nodeList[i]->y, board[nodeList[i]->x][nodeList[i]->y]);
				fruitFall();
				int v_tmp = alphaBeta(alpha, beta, depth - 1, MIN_PLAYER);
				v = v > v_tmp ? v : v_tmp;
				path.pop_back();
				board = tmp;

				if (v > alpha) {
					if (DEEP == depth)
						bestNextNode = nodeList[i];
					localBestNode = nodeList[i];
					alpha = v;
				}
				if (alpha >= beta) {
					HistoryTable[localBestNode->x][localBestNode->y] += 1 << depth;
					break;
				}
			}
			return v;
		}
		else {
			int v = INT_MAX;
			//for (int i = nodeList.size() - 1; i >= 0; i--) {
			for (int i = 0; i < nodeList.size(); i++) {
				auto tmp = board;
				path.push_back(nodeList[i]);
				locate(nodeList[i]->x, nodeList[i]->y, board[nodeList[i]->x][nodeList[i]->y]);
				fruitFall();
				int v_tmp = alphaBeta(alpha, beta, depth - 1, MAX_PLAYER);
				v = v < v_tmp ? v : v_tmp;
				path.pop_back();
				board = tmp;
				if (v < beta) {
					beta = v;
					localBestNode = nodeList[i];
				}
				if (alpha >= beta) {
					HistoryTable[localBestNode->x][localBestNode->y] += 1 << depth;
					break;
				}
			}
			return v;
		}
	}
}

/*
bool comp(const Node* n1, const Node* n2) {
return n1->num > n2->num;
}
*/
vector<Node*> generator() {
	vector<Node*>res;
	vector<vector<int>> visit(n, vector<int>(n, 0));
	//for (int i = board.size()-1; i > -1; i--) {
	//	for (int j = board[0].size()-1; j > -1; j--) {
	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board[0].size(); j++) {
			if (board[i][j] != -1 && visit[i][j] == 0) {
				int count = 0;
				dfs(i, j, visit, count, board[i][j]);
				Node* mynode = (Node*)malloc(sizeof(Node));
				mynode->x = i;
				mynode->y = j;
				mynode->num = count;
				mynode->rate = 0;
				mynode->fruit = board[i][j];
				res.push_back(mynode);
			}
		}
	}
	//sort(res.begin(), res.end(), comp);



	return res;
}

void dfs(int x, int y, vector<vector<int>>& visit, int &count, int fruit) {

	if (x < 0 || y < 0 || x >= n || y >= n || visit[x][y] == 1 || board[x][y] != fruit)
		return;
	visit[x][y] = 1;
	count++;
	dfs(x + 1, y, visit, count, fruit);
	dfs(x, y + 1, visit, count, fruit);
	dfs(x - 1, y, visit, count, fruit);
	dfs(x, y - 1, visit, count, fruit);
}


string& trim(string &s)
{
	if (s.empty())
	{
		return s;
	}
	s.erase(0, s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
	return s;
}

void init() {
	fstream fr;
	fr.open("./input.txt");
	if (!fr.is_open()) { cout << "error" << endl; return; }
	string stmp;
	getline(fr, stmp);
	stmp = trim(stmp);
	n = atoi(stmp.c_str());
	int row = 0;
	getline(fr, stmp);		getline(fr, stmp);
	gtime = atof(stmp.c_str());
	for (int i = 0; i < n; i++) {
		getline(fr, stmp);
		vector<int> tmp(n, 0);
		stmp = trim(stmp);
		for (int j = 0; j < stmp.size(); j++) {
			if (stmp[j] == '*') {
				tmp[j] = -1;
			}
			else {
				tmp[j] = stmp[j] - '0';
			}
			Node* n = (Node*)malloc(sizeof(Node));
			n->x = j;
			n->y = i;

		}
		board.push_back(tmp);
		row++;
	}
	vector<int> tmp(n, 0);
	for (int i = 0; i < n; i++)
		HistoryTable.push_back(tmp);
	fr.close();
}



