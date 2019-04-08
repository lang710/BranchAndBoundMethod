#include <iostream>
#include <queue>
using namespace std;

struct point{
  int x,y;
}

//对归约矩阵进行迭代增广，获取最小耗费
int augment(int c_size,int **graph,int sum,int *visitX,int* visitY){
    queue<point> points;
    for(int i=0;i<c_size;i++)
        for(int j=0;j<c_size;j++)
            if(graph[i][j]==0)
                points.push({i,j});
    if(points.empty())
        return sum;
    
    int cx,cy,fx=points.front().x,fy=points.front.y;
    int key;
    int minX,minY;
    int cost=0,cost1=0;
    while(!points.empty()){
        minX=minY=0x7fffffff;
        cx=points.front.x;
        cy=points.front.y;
        key=graph[cx][cy];
        graph[cx][cy]=-1;
        for(int i=0;i<c_size;i++){
            if(graph[cx][i]>=0&&graph[cx][i]<minX)
                minX=graph[cx][i];
            if(graph[i][cy]>=0&&graph[i][cy]<minY)
                minY=graph[i][cy];
        }
        if(cost<=minX+minY&&minX!=0x7fffffff&&minY!=0x7fffffff){
            cost=minX+minY;
            fx=cx,fy=cy;
        }
        points.pop();
        graph[cx][cy]=key;
    }
    graph[fx][fy]=-1;
    
    int **graph_t=new int*[c_size];
    for(int i=0;i<c_size;i++)
        graph_t[i]=new int[c_size];
    for(int i=0;i<c_size;i++)
        for(int j=0;j<c_size;j++)
            if(i==fx||j==fy)
                graph_t[i][j]=-1;
            else
                graph_t[i][j]=graph[i][j];
    visitX[fx]=1;
    visitY[fy]=1;
    graph_t[fy][fx]=-1;
    for(int i=0;i<c_size;i++){
        minX=0x7fffffff;
        for(int j=0;j<c_size;j++)
            if(visitX[i]==0&&graph_t[i][j]>=0&&graph_t[i][j]<minX)
                minX=graph_t[i][j];
        if(minX>0&&minX<0x7fffffff) {
            for(int j=0;j<c_size;j++)
                if(graph_t[i][j]>0)
                    graph_t[i][j]-=minX;
            cost1 += minX;
        }
        minY=0x7fffffff;
        for(int j=0;j<c_size;j++)
            if(visitY[i]==0&&graph_t[j][i]>=0&&graph_t[j][i]<minY)
                minY=graph_t[j][i];
        if(minY>0&&minY<0x7fffffff) {
            for(int j=0;j<c_size;j++)
                if(graph_t[j][i]>0)
                    graph_t[j][i]-=minY;
            cost1 += minY;
        }
    }

    if(cost1<=cost)
        return augment(c_size,graph_t,sum+cost1,visitX,visitY);
    else{
        visitX[fx]=0,visitY[fy]=0;
        return augment(c_size,graph,sum+cost,visitX,visitY);
    }

}


//获取原始矩阵的归约矩阵，其中-1表示正无穷
int statute(int c_size,int **graph){
    int sum=0;
    int min_row,min_col;
    for(int i=0;i<c_size;i++) {
        min_row = 0x7fffffff;
        for (int j = 0; j < c_size; j++) {
            if (graph[i][j] >= 0 && graph[i][j] < min_row)
                min_row = graph[i][j];
        }
        if(min_row>0) {
            for (int j = 0; j < c_size; j++)
                if (graph[i][j] > 0)
                    graph[i][j] -= min_row;
            sum+=min_row;
        }
    }
    for(int i=0;i<c_size;i++){
        min_col = 0x7fffffff;
        for (int j = 0; j < c_size; j++) {
            if (graph[j][i] >= 0 && graph[j][i] < min_col)
                min_col = graph[j][i];
        }
        if(min_col>0){
            for(int j=0;j<c_size;j++)
                if(graph[j][i]>0)
                    graph[j][i]-=min_col;
            sum+=min_col;
        }
    }

    int *visitX=new int[c_size];
    int *visitY=new int[c_size];
    memset(visitX,0, sizeof(visitX));
    memset(visitY,0, sizeof(visitY));
    return augment(c_size,graph,sum,visitX,visitY);
}


int main(){
    int c_size;         //city size
    cin>>c_size;
    int **graph=new int*[c_size];
    for(int i=0;i<c_size;i++)
        graph[i]=new int[c_size];
    for(int i=0;i<c_size;i++)
        for(int j=0;j<c_size;j++)
            cin>>graph[i][j];
    cout<<statute(c_size,graph);

    return 0;
}

/*
 *
input:
5
-1 17 7 35 18
9 -1 5 14 19
29 24 -1 30 12
27 21 25 -1 48
15 16 28 18 -1

output:
67

 *
 */
