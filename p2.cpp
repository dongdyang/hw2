#include <iostream>
#include<vector>
#include<string>
#include<queue>
#include<fstream>
#include<map>
#include <time.h>
#include <stdio.h>
using namespace std;

int totalnode=0;
int n;
int p;
float lefttime;
vector<string> board;

int maxdepth=2;
int player2node;


struct node{
int maxadj;
int x;
int y;
vector<vector<int> > pieces;
vector<string> board;
int nowscore;
};

struct cmp{
  bool operator () (node a, node b){
   return a.maxadj<b.maxadj;
  }
};


vector<string> changeit(vector<string> b, vector<vector<int> > path){
  int i,j,tmpx,tmpy;
  map<int, int> hashs;
  for (i=0; i<path.size(); i++){
    tmpx=path[i][0];
    tmpy=path[i][1];
    b[tmpx][tmpy]='*';
    if (hashs.find(tmpy)==hashs.end()){
        hashs[tmpy]=tmpx;
    } else if (hashs[tmpy]<tmpx) {
       hashs[tmpy]=tmpx;
    }
  }

  map<int, int>::iterator iter;
  for (iter=hashs.begin(); iter!=hashs.end(); iter++){
     tmpy=iter->first;
     tmpx=iter->second;
     i=tmpx;
     for (j=tmpx-1; j>=0; j--){
        if (b[j][tmpy]!='*'){
            b[i][tmpy]=b[j][tmpy];
            i--;
            b[j][tmpy]='*';
        }
     }
  }
  return b;

}

int nowget;
vector<vector<int> > path;
void findnode(vector<string> &b, int x, int y){
  vector<int> apair(2);
  apair[0]=x; apair[1]=y;
  path.push_back(apair);
  char nowch=b[x][y];
  b[x][y]='*';
  nowget++;
  if (x>0 && b[x-1][y]==nowch) findnode(b, x-1, y);
  if (x<n-1 && b[x+1][y]==nowch) findnode(b, x+1, y);
  if (y>0 && b[x][y-1]==nowch) findnode(b, x, y-1);
  if (y<n-1 && b[x][y+1]==nowch) findnode(b, x, y+1);

}

void minnode(int depth, vector<string> b, int alpha, int beta, node &retnode, int &bestget, int tmpscore, int piececnt, vector<node > &paths);
void maxnode(int depth, vector<string> b, int alpha, int beta, node &retnode, int &bestget, int tmpscore, int piececnt, vector<node > &paths){
       // totalnode++;
       // if (totalnode%50==0) cout<<"max node:"<<totalnode<<endl;

  int i,j;
  vector<string> tmpb;

  node tmpnode, retnode2;
  priority_queue<node, vector<node>, cmp> pq;

  vector<vector<int> > nullpath;
  if (piececnt<=0){
    retnode=tmpnode;
    retnode.x=-1;
    retnode.y=-1;
    retnode.board=b;
    retnode.nowscore=tmpscore;
     if (tmpscore>0) bestget=1000000;
    else if (tmpscore<0) bestget=-1000000;
    else bestget=0;
    return;
  }
  if (tmpscore+(piececnt)*(piececnt)<=alpha){

    retnode=tmpnode;
    retnode.x=-1;
    retnode.y=-1;
    retnode.board=b;
    retnode.nowscore=tmpscore;
    bestget=tmpscore+(piececnt)*(piececnt);
    return;
  }

  vector<string> hashb=b;
  for (i=0; i<n; i++){
    for (j=0; j<n; j++)
        if (hashb[i][j]!='*' ){
       path=nullpath;
       nowget=0;
       findnode(hashb, i,j);
       tmpnode.pieces=path;
       tmpnode.maxadj=nowget;
       tmpnode.x=i;
       tmpnode.y=j;
       pq.push(tmpnode);
    }
  }

   if (depth==0){
   if (lefttime<=1) maxdepth=0;
    else if (lefttime>3 && pq.size()<20) maxdepth=4;
  else if (lefttime>3 && pq.size()<60) maxdepth=3;
  else if (lefttime>3 && pq.size()<300) maxdepth=2;
  else if (pq.size()>=300 || lefttime<=3) maxdepth=1;

    // cout<<"pq size:"<<pq.size()<<" maxdepth:"<<maxdepth<<endl;
   }

  int  bestget2=0, nowget;
  bestget=-5000000;
  vector<node> respath;
  while (!pq.empty()){
    tmpnode=pq.top();
    pq.pop();
    tmpb=changeit(b, tmpnode.pieces);
    tmpnode.board=tmpb;
    nowget=tmpscore+tmpnode.maxadj*tmpnode.maxadj;
    tmpnode.nowscore=nowget;
    bestget2=nowget;
    vector<node> newpath;
    newpath=paths;
    newpath.push_back(tmpnode);
    if (depth<maxdepth) minnode(depth+1, tmpb, alpha, beta, retnode2, bestget2, nowget, piececnt-tmpnode.maxadj, newpath);
    /*
    if (depth==0 ){
        tmpout<<"max choice:";
        tmpout<<tmpnode.x<<","<<tmpnode.y<<" score:"<<bestget2<<" nowscore:"<<tmpscore<<endl;
        tmpout<<"adverse process:"<<endl;
        for (i=0; i<newpath.size(); i++){
                tmpout<<newpath[i].x<<","<<newpath[i].y<<" nowscore:"<<newpath[i].nowscore<<" adj:"<<newpath[i].maxadj<<endl;
                for (j=0; j<b.size(); j++){
                   tmpout<<newpath[i].board[j]<<endl;
                }
        }
    }
      */
    if (bestget2>=beta) {
        bestget=bestget2;
       retnode=tmpnode;
       paths=respath;
       return;
    } else if (bestget2>bestget) {

       bestget=bestget2;
       alpha=bestget;
       retnode=tmpnode;
       respath=newpath;
    }

  }
paths=respath;
}


void minnode(int depth, vector<string> b, int alpha, int beta, node &retnode, int &bestget, int tmpscore, int piececnt, vector<node > &paths){

     //   totalnode++;
     //   if (totalnode%50==0) cout<<"min node:"<<totalnode<<endl;


  int i,j;
  vector<string> tmpb;

  node tmpnode, retnode2;

  if (piececnt<=0){
    retnode=tmpnode;
    retnode.x=-1;
    retnode.y=-1;
    retnode.board=b;
    retnode.nowscore=tmpscore;
    if (tmpscore>0) bestget=1000000;
    else if (tmpscore<0) bestget=-1000000;
    else bestget=0;
    return;
  }
  if (tmpscore-piececnt*piececnt>=beta){
    retnode=tmpnode;
    retnode.x=-1;
    retnode.y=-1;
    retnode.board=b;
    retnode.nowscore=tmpscore;
    bestget=tmpscore-piececnt*piececnt;
    return;
  }

  priority_queue<node, vector<node>, cmp> pq;

  vector<vector<int> > nullpath;

  vector<string> hashb=b;
  for (i=0; i<n; i++){
    for (j=0; j<n; j++)
        if (hashb[i][j]!='*' ){
       path=nullpath;
       nowget=0;
       findnode(hashb, i,j);
       tmpnode.pieces=path;
       tmpnode.maxadj=nowget;
       tmpnode.x=i;
       tmpnode.y=j;
       retnode=tmpnode;
       pq.push(tmpnode);
    }
  }


  int  bestget2=tmpscore, nowget;
  bestget=50000000;
  vector<node> respath;
  while (!pq.empty()){
    tmpnode=pq.top();
    pq.pop();

    tmpb=changeit(b, tmpnode.pieces);
    tmpnode.board=tmpb;
    nowget=tmpscore-tmpnode.maxadj*tmpnode.maxadj;
    tmpnode.nowscore=nowget;
    bestget2=nowget;
    vector<node> newpath;
    newpath=paths;
    newpath.push_back(tmpnode);
    if (depth<maxdepth) maxnode(depth+1, tmpb, alpha, beta, retnode2, bestget2, nowget, piececnt-tmpnode.maxadj, newpath);

    if (bestget2<=alpha) {
        bestget=bestget2;
        retnode=tmpnode;
        paths=respath;
       return;
    } else if (bestget2<bestget) {
       bestget=bestget2;
       beta=bestget2;
       retnode=tmpnode;
       respath=newpath;
    }
  }
  paths=respath;
}




int main()
{
   ifstream fin("input.txt");
   ofstream fout("output.txt");
   fin>>n;
   fin>>p;
   fin>>lefttime;
   int i,j;
   string tmp;
   // double starttime,endtime;
   //  starttime=clock();
double starttime,endtime;
 starttime=clock();
   for (i=0; i<n; i++){
      fin>>tmp;
      board.push_back(tmp);
   }

   int piececnt=0;
   for (i=0; i<n; i++){
    for (j=0; j<n; j++){
        if (board[i][j]!='*') piececnt++;
    }
   }

   int bestget=0;
   int alpha=-50000000, beta=50000000;
   char cols;
   vector<string> resboard;
   node tmpnode;
   vector<node> respath;
   maxnode(0, board, alpha, beta, tmpnode, bestget,0, piececnt, respath);
      piececnt=piececnt-tmpnode.maxadj;
   cols=tmpnode.y+'A';
   fout<<cols<<tmpnode.x+1<<endl;
   resboard=changeit(board, tmpnode.pieces);
   for (i=0; i<n; i++){
    for (j=0; j<n; j++){
        fout<<resboard[i][j];
    }
    fout<<endl;
   }
 endtime=clock();
 fout<<tmpnode.maxadj*tmpnode.maxadj<<endl;
 fout<<(endtime-starttime)*1.0/CLOCKS_PER_SEC<<endl;
   fin.close();


      // endtime=clock();
     //   lefttime=lefttime-(endtime-starttime)*1.0/CLOCKS_PER_SEC;
     //   int p1score=tmpnode.maxadj*tmpnode.maxadj;

    //    fout<<p1score<<endl;
    //    fout<<(endtime-starttime)*1.0/CLOCKS_PER_SEC<<endl;
fout.close();
    return 0;
}
