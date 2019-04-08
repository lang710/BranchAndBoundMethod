#include <iostream>
#include <queue>
using namespace std;

struct point{
    int x,y;
};


//对归约矩阵进行迭代增广，获取最小耗费
float augment(int c_size,float **graph,float sum,int *visitX,int* visitY){
    int i,j;
    queue<point> points;
    for(i=0;i<c_size;i++)
        for(j=0;j<c_size;j++)
            if(graph[i][j]==0)
                points.push({i,j});
    if(points.empty())
        return sum;

    int cx,cy,fx=points.front().x,fy=points.front().y;
    float key;
    float minX,minY;
    float cost=0,cost1=0;

    while(!points.empty()){
        minX=minY=0x7fffffff;
        cx=points.front().x;
        cy=points.front().y;
        key=graph[cx][cy];
        graph[cx][cy]=-1;
        for(i=0;i<c_size;i++) {
            if (graph[cx][i]>=0&&graph[cx][i] < minX)
                minX = graph[cx][i];
            if (graph[i][cy]>=0&&graph[i][cy]<minY)
                minY=graph[i][cy];
        }
        if(cost<=minX+minY&&minX!=0x7fffffff&&minY!=0x7fffffff) {
            cost = minX + minY;
            fx=cx,fy=cy;
        }
        points.pop();
        graph[cx][cy]=key;
    }
    graph[fx][fy]=-1;

    float **graph_t=new float*[c_size];
    for(i=0;i<c_size;i++)
        graph_t[i]=new float[c_size];
    for(i=0;i<c_size;i++)
        for(j=0;j<c_size;j++)
            if(i==fx||j==fy)
                graph_t[i][j]=-1;
            else
                graph_t[i][j]=graph[i][j];
    visitX[fx]=1;
    visitY[fy]=1;
    graph_t[fy][fx]=-1;
    for(i=0;i<c_size;i++) {
        minX = 0x7fffffff;
        for (j = 0; j < c_size; j++)
            if (visitX[i]==0&&graph_t[i][j]>=0&&graph_t[i][j]<minX)
                minX=graph_t[i][j];
        if(minX>0&&minX<0x7fffffff) {
            for(j=0;j<c_size;j++)
                if(graph_t[i][j]>0)
                    graph_t[i][j]-=minX;
            cost1 += minX;
        }
        minY=0x7fffffff;
        for(j=0;j<c_size;j++)
            if(visitY[i]==0&&graph_t[j][i]>=0&&graph_t[j][i]<minY)
                minY=graph_t[j][i];
        if(minY>0&&minY<0x7fffffff) {
            for(j=0;j<c_size;j++)
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
float statute(int c_size,float **graph){
    float sum=0;
    float min_row,min_col;
    int i,j;
    for(i=0;i<c_size;i++) {
        min_row = 0x7fffffff;
        for (j = 0; j < c_size; j++) {
            if (graph[i][j] >= 0 && graph[i][j] < min_row)
                min_row = graph[i][j];
        }
        if(min_row>0) {
            for (j = 0; j < c_size; j++)
                if (graph[i][j] > 0)
                    graph[i][j] -= min_row;
            sum+=min_row;
        }
    }
    for(i=0;i<c_size;i++){
        min_col = 0x7fffffff;
        for (j = 0; j < c_size; j++) {
            if (graph[j][i] >= 0 && graph[j][i] < min_col)
                min_col = graph[j][i];
        }
        if(min_col>0){
            for(j=0;j<c_size;j++)
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
    int i,j;
    int c_size;         //city size
    cin>>c_size;
    float **graph=new float*[c_size];
    for(i=0;i<c_size;i++)
        graph[i]=new float[c_size];
    for(i=0;i<c_size;i++)
        for(j=0;j<c_size;j++)
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

input:
4
-1 10 15 -1
10 -1 35 25
15 35 -1 30
-1 25 30 -1
 
output:
80

input:
10
-1 2538.94 2873.8 2575.27 2318.1 2158.71 2216.58 3174.04 3371.13 3540.24
2538.94 -1 1073.54 111.288 266.835 395.032 410.118 637.942 853.554 1055
2873.8 1073.54 -1 964.495 988.636 1094.32 1382.73 1240.15 1460.25 1687
2575.27 111.288 964.495 -1 262.053 416.707 503.563 624.725 854.916 1068.42
2318.1 266.835 988.636 262.053 -1 163.355 395.14 885 1110.86 1318.19
2158.71 395.032 1094.32 416.707 163.355 -1 338.634 1030.34 1248.58 1447.69
2216.58 410.118 1382.73 503.563 395.14 338.634 -1 984.068 1160.26 1323.7
3174.04 637.942 1240.15 624.725 885 1030.34 984.068 -1 243.417 473.768
3371.13 853.554 1460.25 854.916 1110.86 1248.58 1160.26 243.417 -1 232.112
3540.24 1055 1687 1068.42 1318.19 1447.69 1323.7 473.768 232.112 -1

output:
7884.28

 *
 */
