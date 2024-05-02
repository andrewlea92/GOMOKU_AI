#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>
#include<set>
#include<queue>
#include<limits.h>
using namespace std;

int player;//玩家是白棋還是黑棋
const int SIZE = 15;//棋盤大小
array<array<int, SIZE>, SIZE> komoku;//棋盤

// 0: empty  ,1: black  ,2: white
enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};

//從player_ramdom微調的
struct Point {
    int x, y;
	Point() : Point(0, 0) {}
	Point(float x, float y) : x(x), y(y) {}
	bool operator==(const Point& rhs) const {
		return x == rhs.x && y == rhs.y;
	}
	bool operator!=(const Point& rhs) const {
		return !operator==(rhs);
	}
	Point operator+(const Point& rhs) const {
		return Point(x + rhs.x, y + rhs.y);
	}
	Point operator-(const Point& rhs) const {
		return Point(x - rhs.x, y - rhs.y);
	}
    bool operator<(const Point &r)const{
        if(x!=r.x) return x < r.x;
        if(y!=r.y) return y < r.y;
        return 0;
    }
};

bool checkSurrounding(std::array<std::array<int, SIZE>, SIZE> bd, int i, int j);

Point Best;

class State{
public:
    array<array<int, SIZE>, SIZE> komoku; 
    set<Point> candi;

    State(array<array<int, SIZE>, SIZE>a){
        for(int i=0;i<SIZE;i++){
            for(int j=0;j<SIZE;j++){
                komoku[i][j] = a[i][j];
            }
        }
    }

    State(State& tmp){
        for(int i=0;i<SIZE;i++){
            for(int j=0;j<SIZE;j++){
                komoku[i][j] = tmp.komoku[i][j];
            }
        }
    }

    //可行點
    void next_candi(){
        for(int i=0;i<SIZE;i++){
            for(int j=0;j<SIZE;j++){
                if(komoku[i][j] == 0){
                    if(checkSurrounding(komoku,i,j))
                    {
                      candi.insert(Point(i,j));  
                    }
                }
            }
        }
        return;
    }

    //下棋
    void add_Point(Point point, int ply){
        komoku[point.x][point.y] = ply;
    }

    //計算heuristic
    int evaluate_score(){
        int h = 0;
        int opponent = 3 - player;
        for(int i = 0; i<SIZE; i++){
            for(int j=0; j<SIZE; j++){
                int flag1=0;
                int flag2=0;
                int flag3=0;
                if(komoku[i][j] == player){    
                    if(j+1<SIZE && komoku[i][j+1] == player){  
                        flag1++;
                        if(j+2<SIZE && komoku[i][j+2] == player){
                            flag1++;
                            if(j+3<SIZE && komoku[i][j+3] == player){
                                flag1++;
                                if(j+4<SIZE && komoku[i][j+4] == player)
                                {
                                    flag1++;
                                    h+=999999;
                                }
                                else
                                {
                                    int pt=0;
                                    if(j+4<SIZE && komoku[i][j+4] == 0)
                                    {
                                        pt++;
                                    }
                                    if(j-1>=0 && komoku[i][j-1] == 0)
                                    {
                                        pt++;
                                    }
                                    if(pt==2)
                                    {
                                        h+=3000;
                                    }
                                    else if(pt==1)
                                    {
                                        h+=3000;
                                    }
                                    else if(pt==0)
                                    {
                                        flag1-=3;
                                    }
                                }
                            }
                            else
                            {
                                int pt=0;
                                if(j+3<SIZE && komoku[i][j+3] == 0)
                                {
                                    pt++;
                                    
                                }
                                if(j-1>=0 &&komoku[i][j-1] == 0)
                                {
                                    pt++;
                                }
                                if(pt==1)
                                {
                                    h+=100;
                                }
                                if(pt==2)
                                {
                                    h+=1000;
                                    flag2++;
                                    if(i-3>=0&&i+1<SIZE&&komoku[i-3][j]==0&&komoku[i-2][j]==player&&komoku[i-1][j]==player&&komoku[i+1][j]==0)
                                    {
                                        h+=2500;
                                        flag2++;
                                    }
                                }
                                else if(pt==0)
                                {
                                    flag1-=2;
                                }                                    
                            }
                        }
                        else
                        {
                            if(j+2<SIZE&&j+3<SIZE&&komoku[i][j+2]==0&&komoku[i][j+3]==player)
                            {
                                    if(j+4<SIZE&&komoku[i][j+4]==player)
                                    {
                                        h+=3000;
                                    }
                                    else
                                    {
                                        h+=500;
                                    }
                            }
                           int pt=0;
                           if(j+2<SIZE && komoku[i][j+2] == 0)
                           {
                            pt++;      
                           }
                           if(j-1>=0 &&komoku[i][j-1] == 0)
                           {
                            pt++;
                           }
                           if(pt==1)
                           {
                            h+=20;
                           }
                           else if(pt==2)
                           {
                            h+=100;
                           }
                           else if(pt==0)
                           {
                            flag1--;
                           }
                        }
                        
                    }
                    if(i+1<SIZE && komoku[i+1][j] == player){  
                        flag1++;
                        if(i+2<SIZE && komoku[i+2][j] == player){
                            flag1++;
                            if(i+3<SIZE && komoku[i+3][j] == player){
                                flag1++;
                                if(i+4<SIZE && komoku[i+4][j] == player){
                                    h+=999999;
                                    flag1++;
                                }
                                    else
                                    {
                                    int pt=0;
                                    if(i+4<SIZE && komoku[i+4][j] == 0)
                                    {
                                        pt++;
                                    }
                                    if(i-1>=0 && komoku[i-1][j] == 0)
                                    {
                                        pt++;
                                    }
                                    if(pt==2)
                                    {
                                        h+=3000;
                                    }
                                    if(pt==1)
                                    {
                                        h+=3000;
                                    }
                                    if(pt==0)
                                    {
                                        flag1-=3;
                                    }
                                }
                            }
                            else
                            {
                                    int pt=0;
                                if(i+3<SIZE && komoku[i+3][j] == 0)
                                {
                                    pt++;
                                    
                                }
                                if(i-1>=0 &&komoku[i-1][j] == 0)
                                {
                                    pt++;
                                }
                                if(pt==1)
                                {
                                    h+=100;
                                }
                                if(pt==2)
                                {
                                    h+=1000;
                                    flag2++;
                                    if(j-3>=0&&i+1<SIZE&&komoku[i][j-3]==0&&komoku[i][j-2]==player&&komoku[i][j-1]==player&&komoku[i][j+1]==0)
                                    {
                                        h+=2500;
                                        flag2++;
                                    }
                                }
                                else if(pt==0)
                                {
                                    flag1-=2;
                                }
                                
                            }
                        }
                        else
                        {
                            if(i+2<SIZE&&i+3<SIZE&&komoku[i+2][j]==0&&komoku[i+3][j]==player)
                            {
                                    if(i+4<SIZE&&komoku[i+4][j]==player)
                                    {
                                        h+=3000;
                                    }
                                    else
                                    {
                                        h+=500;
                                    }
                            }
                           int pt=0;
                           if(i+2<SIZE && komoku[i+2][j] == 0)
                           {
                                    pt++;
                                    
                                }
                                if(i-1>=0 &&komoku[i-1][j] == 0)
                                {
                                    pt++;
                                }
                                if(pt==1)
                                {
                                    h+=20;
                                }
                                if(pt==2)
                                {
                                    h+=100;
                                }
                                else if(pt==0)
                                {
                                    flag1--;
                                } 
                        }
                        
                    }
                    if(i+1<SIZE && j+1<SIZE && komoku[i+1][j+1] == player){
                        flag1++;
                        if(i+2<SIZE && j+2<SIZE && komoku[i+2][j+2] == player){
                            flag1++;
                            if(i+3<SIZE && j+3<SIZE && komoku[i+3][j+3] == player){
                                flag1++;
                                if(i+4<SIZE && j+4<SIZE && komoku[i+4][j+4] == player){
                                    h+=999999;
                                    flag1++;
                                }
                                    else
                                    {
                                    int pt=0;
                                    if(j+4<SIZE &&i+4<SIZE&& komoku[i+4][j+4] == 0)
                                    {
                                        pt++;
                                    }
                                    if(j-1>=0 && i-1>=0&&komoku[i-1][j-1] == 0)
                                    {
                                        pt++;
                                    }
                                    if(pt==2)
                                    {
                                        h+=3000;
                                    }
                                    if(pt==1)
                                    {
                                        h+=3000;
                                    }
                                    if(pt==0)
                                    {
                                        flag1-=3;
                                    }
                                }
                            }
                            else
                            {
                                int pt=0;
                                if(j+3<SIZE &&i+3<SIZE&& komoku[i+3][j+3] == 0)
                                {
                                    pt++;
                                    
                                }
                                if(j-1>=0 &&i-1>=0&&komoku[i-1][j-1] == 0)
                                {
                                    pt++;
                                }
                                if(pt==1)
                                {
                                    h+=100;
                                }
                                if(pt==2)
                                {
                                    h+=1000;
                                    flag2++;
                                }
                                else if(pt==0)
                                {
                                    flag1-=2;
                                }
                            }
                        }
                        else
                        {
                            if(j+2<SIZE&&j+3<SIZE&&i+2<SIZE&&i+3<SIZE&&komoku[i+2][j+2]==0&&komoku[i+3][j+3]==player)
                                {
                                    if(j+4<SIZE&&i+4<SIZE&&komoku[i+4][j+4]==player)
                                    {
                                        h+=3000;
                                    }
                                    else
                                    {
                                        h+=500;
                                    }
                                }
                            int pt=0;
                                if(j+2<SIZE &&i+2<SIZE&& komoku[i+2][j+2] == 0)
                                {
                                    pt++;
                                    
                                }
                                if(j-1>=0 &&i-1>=0&&komoku[i-1][j-1] == 0)
                                {
                                    pt++;
                                }
                                if(pt==1)
                                {
                                    h+=20;
                                }
                                if(pt==2)
                                {
                                    h+=100;
                                }
                                else if(pt==0)
                                {
                                    flag1--;
                                }
                        }
                        
                    }
                    if(i+1<SIZE && j-1>=0 && komoku[i+1][j-1] == player){
                        flag1++;
                        if(i+2<SIZE && j-2>=0 && komoku[i+2][j-2] == player){
                            flag1++;
                            if(i+3<SIZE && j-3>=0 && komoku[i+3][j-3] == player){
                                flag1++;
                                if(i+4<SIZE && j-4>=0 && komoku[i+4][j-4] == player){
                                    h+=999999;
                                    flag1++;
                                }
                                else
                                {
                                    int pt=0;
                                    if(j-4>=0 && i+4<SIZE && komoku[i+4][j-4] == 0)
                                    {
                                        pt++;
                                    }
                                    if(j+1<SIZE && i-1>=0 && komoku[i-1][j+1] == 0)
                                    {
                                        pt++;
                                    }
                                    if(pt==2)
                                    {
                                        h+=3000;
                                    }
                                    if(pt==1)
                                    {
                                        h+=3000;
                                    }
                                    if(pt==0)
                                    {
                                        flag1-=3;
                                    }
                                }
                            }
                            else
                            {
                                int pt=0;
                                if(j-3>=0 &&i+3<SIZE&& komoku[i+3][j-3] == 0)
                                {
                                    pt++;
                                    
                                }
                                if(j+1<SIZE &&i-1>=0&&komoku[i-1][j+1] == 0)
                                {
                                    pt++;
                                }
                                if(pt==1)
                                {
                                    h+=100;
                                }
                                if(pt==2)
                                {
                                    h+=1000;
                                    flag2++;
                                }
                                else if(pt==0)
                                {
                                    flag1-=2;
                                }
                            }
                        }
                        else
                        {
                            if(j-2>=0&&j-3>=0&&i+2<SIZE&&i+3<SIZE&&komoku[i+2][j-2]==0&&komoku[i+3][j-3]==player)
                                {
                                    if(j-4>=0&&i+4<SIZE&&komoku[i+4][j-4]==player)
                                    {
                                        h+=3000;
                                    }
                                    else
                                    {
                                        h+=500;
                                    }
                                }
                            int pt=0;
                                if(j-2>=0 &&i+2<SIZE&& komoku[i+2][j-2] == 0)
                                {
                                    pt++;
                                    
                                }
                                if(j+1<SIZE &&i-1>=0&&komoku[i-1][j+1] == 0)
                                {
                                    pt++;
                                }
                                if(pt==1)
                                {
                                    h+=20;
                                }
                                if(pt==2)
                                {
                                    h+=100;
                                }
                                else if(pt==0)
                                {
                                    flag1--;
                                }
                    }
                }
                }else if(komoku[i][j] == opponent){
                    if(j+1<SIZE && komoku[i][j+1] == opponent){  
                        flag1--;
                        if(j+2<SIZE && komoku[i][j+2] == opponent){
                            flag1--;
                            if(j+3<SIZE && komoku[i][j+3] == opponent){
                                flag1--;
                                if(j+4<SIZE && komoku[i][j+4] == opponent){
                                    flag1--;
                                    h-=2000000;
                                }
                                else
                                {
                                    int pt=0;
                                    if(j+4<SIZE && komoku[i][j+4] == 0)
                                    {
                                        pt++;
                                    }
                                    if(j-1>=0 && komoku[i][j-1] == 0)
                                    {
                                        pt++;
                                    }
                                    if(pt==2)
                                    {
                                        h-=20000;
                                        //flag3++;
                                    }
                                    if(pt==1)
                                    {
                                        h-=12000;
                                        //flag3++;
                                    }
                                    if(pt==0)
                                    {
                                        flag1+=3;
                                    }
                                }
                            }
                            else
                            {
                                int pt=0;
                                if(j+3<SIZE && komoku[i][j+3] == 0)
                                {
                                    pt++;
                                    if(j+4<SIZE&&komoku[i][j+4]==opponent)
                                    {
                                        h-=5000;
                                    }
                                    
                                }
                                if(j-1>=0 &&komoku[i][j-1] == 0)
                                {
                                    pt++;
                                    if(j-2>=0&&komoku[i][j-2]==opponent)
                                    {
                                        h-=5000;
                                    }
                                }
                                if(pt==1)
                                {
                                    h-=80;
                                }
                                if(pt==2)
                                {
                                    h-=2500;
                                    flag3++;
                                    if(i-3>=0&&i+1<SIZE&&komoku[i-3][j]==0&&komoku[i-2][j]==opponent&&komoku[i-1][j]==opponent&&komoku[i+1][j]==0)
                                    {
                                        h-=2500;
                                        flag3++;
                                    }
                                }
                                else if(pt==0)
                                {
                                    flag1+=2;
                                }
                            }
                        }
                        else
                        {
                            if(j+2<SIZE&&j+3<SIZE&&komoku[i][j+2]==0&&komoku[i][j+3]==opponent)
                            {
                                    if(j+4<SIZE&&komoku[i][j+4]==opponent)
                                    {
                                        h-=10000;
                                    }
                                    else
                                    {
                                        h-=1000;
                                    }
                            }
                            int pt=0;
                                if(j+2<SIZE && komoku[i][j+2] == 0)
                                {
                                    pt++;
                                    
                                }
                                if(j-1>=0 &&komoku[i][j-1] == 0)
                                {
                                    pt++;
                                }
                                if(pt==1)
                                {
                                    h-=10;
                                }
                                if(pt==2)
                                {
                                    h-=60;
                                    if(j+4<SIZE&&komoku[i][j+3]==opponent&&komoku[i][j+4]==0)
                                    {
                                        h-=1500;
                                    }
                                    else if(j-3>=0&&komoku[i][j-2]==opponent&&komoku[i][j-3]==0)
                                    {
                                        h-=1500;
                                    }
                                }
                                else if(pt==0)
                                {
                                    flag1++;
                                }
                    }
                    }
                    if(i+1<SIZE && komoku[i+1][j] == opponent){  
                        flag1--;
                        if(i+2<SIZE && komoku[i+2][j] == opponent){
                            h-=60;
                            flag1--;
                            if(i+3<SIZE && komoku[i+3][j] == opponent){
                                h-=200;
                                flag1--;
                                if(i+4<SIZE && komoku[i+4][j] == opponent){
                                    flag1--;
                                    h-=2000000;
                                    //cout<<"NOOO";
                                }
                                else
                                    {
                                    int pt=0;
                                    if(i+4<SIZE && komoku[i+4][j] == 0)
                                    {
                                        pt++;
                                    }
                                    if(i-1>=0 && komoku[i-1][j] == 0)
                                    {
                                        pt++;
                                    }
                                    if(pt==2)
                                    {
                                        h-=20000;
                                        //flag3++;
                                    }
                                    if(pt==1)
                                    {
                                        h-=12000;
                                        //flag3++;
                                    }
                                    if(pt==0)
                                    {
                                        flag1+=3;
                                    }
                                }
                            }
                            else
                            {
                                int pt=0;
                                if(i+3<SIZE && komoku[i+3][j] == 0)
                                {
                                    pt++;
                                    if(i+4<SIZE&&komoku[i+4][j]==opponent)
                                    {
                                        h-=5000;
                                    }
                                    
                                }
                                if(i-1>=0 &&komoku[i-1][j] == 0)
                                {
                                    pt++;
                                    if(i-2>=0&&komoku[i-2][j]==opponent)
                                    {
                                        h-=5000;
                                    }
                                }
                                if(pt==1)
                                {
                                    h-=80;
                                }
                                if(pt==2)
                                {
                                    h-=2500;
                                    flag3++;
                                    if(j-3>=0&&j+1<SIZE&&komoku[i][j-3]==0&&komoku[i][j-2]==opponent&&komoku[i][j-1]==opponent&&komoku[i][j+1]==0)
                                    {
                                        h-=2500;
                                        flag3++;
                                    }
                                }
                                else if(pt==0)
                                {
                                    flag1+=2;
                                }
                            }
                        }
                        else
                        {
                            if(i+2<SIZE&&i+3<SIZE&&komoku[i+2][j]==0&&komoku[i+3][j]==opponent)
                            {
                                    if(i+4<SIZE&&komoku[i+4][j]==opponent)
                                    {
                                        h-=10000;
                                    }
                                    else
                                    {
                                        h-=1000;
                                    }
                            }
                        int pt=0;
                                if(i+2<SIZE && komoku[i+2][j] == 0)
                                {
                                    pt++;
                                }
                                if(i-1>=0 &&komoku[i-1][j] == 0)
                                {
                                    pt++;
                                }
                                if(pt==1)
                                {
                                    h-=10;
                                }
                                if(pt==2)
                                {
                                    h-=60;
                                    if(i+4<SIZE&&komoku[i+3][j]==opponent&&komoku[i+4][j]==0)
                                    {
                                        h-=1500;
                                    }
                                    else if(i-3>=0&&komoku[i-2][j]==opponent&&komoku[i-3][j]==0)
                                    {
                                        h-=1500;
                                    }
                                }
                                else if(pt==0)
                                {
                                    flag1++;
                                }
                        }
                        
                    }
                    if(i+1<SIZE && j+1<SIZE && komoku[i+1][j+1] == opponent){
                        flag1--;
                        if(i+2<SIZE && j+2<SIZE && komoku[i+2][j+2] == opponent){
                            flag1--;
                            if(i+3<SIZE && j+3<SIZE && komoku[i+3][j+3] == opponent){
                                flag1--;
                                if(i+4<SIZE && j+4<SIZE && komoku[i+4][j+4] == opponent){
                                    flag1--;
                                    h-=2000000;
                                }
                                    else
                                    {
                                    int pt=0;
                                    if(j+4<SIZE &&i+4<SIZE&& komoku[i+4][j+4] == 0)
                                    {
                                        pt++;
                                    }
                                    if(j-1>=0 && i-1>=0&&komoku[i-1][j-1] == 0)
                                    {
                                        pt++;
                                    }
                                    if(pt==2)
                                    {
                                        h-=20000;
                                        //flag3++;
                                    }
                                    if(pt==1)
                                    {
                                        h-=12000;
                                        //flag3++;
                                    }
                                    if(pt==0)
                                    {
                                        flag1+=3;
                                    }
                                }
                            }
                            else
                            {
                                int pt=0;
                                if(j+3<SIZE &&i+3<SIZE&& komoku[i+3][j+3] == 0)
                                {
                                    pt++; 
                                    if(j+4<SIZE&&i+4<SIZE&&komoku[i+4][j+4]==opponent)
                                    {
                                        h-=5000;
                                    }
                                }
                                if(j-1>=0 &&i-1>=0&&komoku[i-1][j-1] == 0)
                                {
                                    pt++;
                                    if(i-2>=0&&j-2>=0&&komoku[i-2][j-2]==opponent)
                                    {
                                        h-=5000;
                                    }
                                }
                                if(pt==1)
                                {
                                    h-=80;
                                }
                                if(pt==2)
                                {
                                    h-=2500;
                                    flag3++;
                                }
                                else
                                {
                                    flag1+=2;
                                }
                            }
                        }
                        else{
                             if(j+2<SIZE&&j+3<SIZE&&i+2<SIZE&&i+3<SIZE&&komoku[i+2][j+2]==0&&komoku[i+3][j+3]==opponent)
                                {
                                    if(j+4<SIZE&&i+4<SIZE&&komoku[i+4][j+4]==opponent)
                                    {
                                        h-=10000;
                                    }
                                    else
                                    {
                                        h-=1000;
                                    }
                                }
                            int pt=0;
                                    if(j+2<SIZE &&i+2<SIZE&& komoku[i+2][j+2] == 0)
                                    {
                                        pt++;
                                    }
                                    if(j-1>=0 && i-1>=0&&komoku[i-1][j-1] == 0)
                                    {
                                        pt++;
                                    }
                                    if(pt==1)
                                    {
                                        h-=10;
                                    }
                                    else if(pt==2)
                                    {
                                        h-=60;
                                        if(j+4<SIZE&&i+4<SIZE&&komoku[i+3][j+3]==opponent&&komoku[i+4][j+4]==0)
                                        {
                                        h-=1500;
                                        }
                                        else if(j-3>=0&&i-3>=0&&komoku[i-2][j-2]==opponent&&komoku[i-3][j-3]==0)
                                        {
                                        h-=1500;
                                        }
                                    }
                                    else if(pt==0)
                                    {
                                        flag1++;
                                    }
                        }
                    }
                    if(i+1<SIZE && j-1>=0 && komoku[i+1][j-1] == opponent){
                        flag1--;
                        if(i+2<SIZE && j-2>=0 && komoku[i+2][j-2] == opponent){
                            flag1--;
                            if(i+3<SIZE && j-3>=0 && komoku[i+3][j-3] == opponent){
                                flag1--;
                                h-=200;
                                if(i+4<SIZE && j-4>=0 && komoku[i+4][j-4] == opponent){
                                    flag1--;
                                    h-=2000000;
                                }
                                else
                                {
                                    int pt=0;
                                    if(j-4>=0 && i+4<SIZE && komoku[i+4][j-4] == 0)
                                    {
                                        pt++;
                                        
                                    }
                                    if(j+1<SIZE && i-1>=0 && komoku[i-1][j+1] == 0)
                                    {
                                        pt++;
                                    }
                                    if(pt==2)
                                    {
                                        h-=20000;
                                        //flag3++;
                                    }
                                    if(pt==1)
                                    {
                                        h-=12000;
                                        //flag3++;
                                        //cout<<"hi"<<i<<" "<<j<<'\n';
                                    }
                                    if(pt==0)
                                    {
                                        flag1+=3;
                                    }
                                }
                            }
                            else
                            {
                                int pt=0;
                                if(j-3>=0 && i+3<SIZE&& komoku[i+3][j-3] == 0)
                                {
                                    pt++;
                                    if(j-4>=0&&i+4<SIZE&&komoku[i+4][j-4]==opponent)
                                    {
                                        h-=5000;
                                    }
                                    
                                }
                                if(j+1<SIZE && i-1>=0&&komoku[i-1][j+1] == 0)
                                {
                                    pt++;
                                    if(j+2<SIZE&&i-2>=0&&komoku[i-2][j+2]==opponent)
                                    {
                                        h-=5000;
                                    }
                                }
                                if(pt==1)
                                {
                                    h-=80;
                                }
                                if(pt==2)
                                {
                                    h-=2500;
                                    flag3++;
                                }
                                else if(pt==0)
                                {
                                    flag1+=2;
                                }
                            }
                        }
                        else{
                            if(j-2>=0&&j-3>=0&&i+2<SIZE&&i+3<SIZE&&komoku[i+2][j-2]==0&&komoku[i+3][j-3]==opponent)
                                {
                                    if(j-4>=0&&i+4<SIZE&&komoku[i+4][j-4]==opponent)
                                    {
                                        h-=10000;
                                    }
                                    else
                                    {
                                        h-=1000;
                                    }
                                }
                            int pt=0;
                                if(j-2>=0 &&i+2<SIZE&& komoku[i+2][j-2] == 0)
                                {
                                    pt++;
                                    
                                }
                                if(j+1<SIZE &&i-1>=0&&komoku[i-1][j+1] == 0)
                                {
                                    pt++;
                                }
                                if(pt==1)
                                {
                                    h-=10;
                                }
                                if(pt==2)
                                {
                                    h-=60;
                                    if(i+4<SIZE&&j-4>=0&&komoku[i+3][j-3]==opponent&&komoku[i+4][j-4]==0)
                                    {
                                        h-=1500;
                                    }
                                    else if(i-3>=0&&j+3<SIZE&&komoku[i-2][j+2]==opponent&&komoku[i-3][j+3]==0)
                                    {
                                        h-=1500;
                                    }
                                }
                                else if(pt==0)
                                {
                                    flag1++;
                                }
                        }
                        
                    }
                    //spec
                    if(i+3<SIZE&&i-1>=0&&j+3<SIZE&&j-3>=0)
                    {
                        if(komoku[i-1][j+1]==0&&komoku[i-1][j-1]==0&&komoku[i+1][j-1]==opponent&&komoku[i+1][j+1]==opponent&&komoku[i+2][j+2]==opponent&&komoku[i+2][j-2]==opponent&&komoku[i+3][j+3]==0&&komoku[i+3][j-3]==0)
                        {
                            h-=2000;
                        }
                    }
                }
                else if(komoku[i][j]==0)
                {
                    if(i+3<SIZE&&i-1>=0&&j+3<SIZE&&j-3>=0)
                    {
                        if(komoku[i-1][j+1]==0&&komoku[i-1][j-1]==0&&komoku[i+1][j-1]==opponent&&komoku[i+1][j+1]==opponent&&komoku[i+2][j+2]==opponent&&komoku[i+2][j-2]==opponent&&komoku[i+3][j+3]==0&&komoku[i+3][j-3]==0)
                        {
                            h-=2000;
                        }
                    }
                }
                if(flag1>1)
                {
                   h+=flag1*50; 
                }
                if(flag2>1)
                {
                    h+=1500*flag2;
                }
                if(flag3>1)
                {
                    h-=3000*flag3;
                }
                
            }
        }
        return h;
    }

};

//mini max alpha beta
int Minimax(State state, int depth, int Alpha, int Beta, bool maximizingPlayer);

//確認四面八方有沒有棋子(要不然會超時QQ)
bool checkSurrounding(std::array<std::array<int, SIZE>, SIZE> bd, int i, int j){
        if(i>0 && i<SIZE-1){
            if(j>0 && j<SIZE-1){
                return bd[i-1][j-1]>0||bd[i-1][j]>0||bd[i-1][j+1]>0||bd[i][j-1]>0||bd[i][j+1]>0 ||bd[i+1][j-1]>0|| bd[i+1][j]>0 ||bd[i+1][j+1]>0;
            }
            else if(j==0){
                return bd[i-1][j]>0 ||bd[i-1][j+1]>0 ||bd[i][j+1]>0|| bd[i+1][j]>0 ||bd[i+1][j+1]>0;
            }
            else{
                return bd[i-1][j]>0||bd[i-1][j-1]>0||bd[i][j-1]>0 ||bd[i+1][j]>0 ||bd[i+1][j-1]>0;
            }
        }
        else if(i == 0){
            if(j>0 && j<SIZE-1){
                return bd[i][j-1]>0||bd[i][j+1]>0 ||bd[i+1][j-1]>0 ||bd[i+1][j]>0 ||bd[i][j+1]>0;
            }
            else if(j==0){
                return bd[i+1][j]>0 ||bd[i][j+1]>0 ||bd[i+1][j+1]>0;
            }
            else{
                return bd[i+1][j]>0 ||bd[i][j-1]>0 ||bd[i+1][j-1]>0;
            }
        }
        else{
            if(j>0 && j<SIZE-1){
                return bd[i][j-1]>0 ||bd[i-1][j-1]>0 ||bd[i-1][j]>0 ||bd[i-1][j+1]>0 ||bd[i][j+1]>0;
            }
            else if(j==0){
                return bd[i-1][j]>0|| bd[i-1][j+1]>0|| bd[i][j+1]>0;
            }
            else{
                return bd[i][j-1]>0 ||bd[i-1][j-1]>0 || bd[i-1][j]>0;
            }
        }
    }

//讀棋盤
void read_board(std::ifstream& fin) {
    fin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> komoku[i][j];
        }
    }
}

//找出best以及印出有用資訊
Point Next_Point(State &state){
    //minimax
    int best_val = Minimax(state, 3, -999999, 999999, true);
    //debug
    cout<<" ";
    cout<<"Best ("<<Best.x<<","<<Best.y<<")\n";
    cout<<player<<"\n";
    cout<<"hi 原:"<<state.evaluate_score()<<"\n";
    State tmp=state;
    tmp.add_Point(Best,player);
    cout<<"hello 後:"<<tmp.evaluate_score()<<"\n";
    //cout<<"hooma"<<best_val<<"\n";
    return Best;   
}

//決定策略
void write_valid_spot(std::ofstream& fout, State &state) {
srand(time(NULL));
    int flag = 0;
    //數棋局上自己的子有幾顆
    for(int i=0;i<SIZE;i++){
      for(int j=0;j<SIZE;j++){
             if(state.komoku[i][j] == player){
                 flag++;
                 break;
             }
         }
     }
    if(flag == 0){ 
        //第一個下天元要不然就下天元附近
        if(komoku[SIZE/2][SIZE/2] == 0) fout<<SIZE/2<<" "<<SIZE/2;
        else if(komoku[(SIZE/2)-1][(SIZE/2)-1] == 0) fout<<(SIZE/2)-1<<" "<<(SIZE/2)-1;
        else fout<<(SIZE/2)+1<<" "<<SIZE/2;
        
    }else{
        //其餘用minimax判斷
        Point next = Next_Point(state);
        fout<<next.x<<" "<<next.y<<"\n";
    }
    fout.flush();
}

//遞迴
int Minimax(State state, int depth, int Alpha, int Beta, bool maximizingPlayer){
    //alpha:目前為止player至少能拿幾分
    //beta:繼續走下去player最多能拿幾分
    if(depth == 0){ 
        return state.evaluate_score();   
    }
    state.next_candi();
    if(maximizingPlayer){
        int maxEval = -999999;
        for(auto e : state.candi){
            State next = state;
            next.add_Point(e, player);
            int eval = Minimax(next, depth - 1, Alpha, Beta, !maximizingPlayer);
            //player的角度:player越高分越好
            maxEval = max(eval, maxEval);
            if(eval == maxEval){
                if(depth==3)
                {
                   //在頂層紀錄最佳解
                   Best = e; 
                } 
            }
            
            Alpha = max(Alpha, maxEval);
            //當beta>alpha就不繼續走下去(後面沒更好的選擇)
            if(Beta < Alpha) break; 
        }
        return maxEval;

    }else{
        int minEval = 999999;
        for(auto e : state.candi){
            State next = state;
            next.add_Point(e, 3-player);
            int eval = Minimax(next, depth - 1, Alpha, Beta, !maximizingPlayer);
            //opponent的角度:player越低分越好
            minEval = min(minEval, eval);
            if(eval == minEval){
                //do nothing
            }
            Beta = min(Beta, minEval);
            //當beta>alpha就不繼續走下去(後面沒更好的選擇)
            if(Beta < Alpha) break;
        }
        return minEval;
    }
    //return 0;
}

//從ramdom抓過來用的
int main(int, char** argv) {
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    State start(komoku);
    write_valid_spot(fout, start);
    fin.close();
    fout.close();
    return 0;
}
