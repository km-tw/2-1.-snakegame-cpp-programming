#include <ncurses.h>
#include <utility>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "stage.h"
using namespace std;

class board : public stage{
    public:
    
    int bn,bm;
    char barr[30][30];
    WINDOW* bwin;

    WINDOW* swin;

    WINDOW* mwin;

    pair<int,int> growthxy,poisonxy;
    struct timeval growthtime,poisontime;
    pair<int,int> gate[2];
    struct timeval gatetime;

    board(int a=0,int n=21,int m=21):stage(a){
        bn=n;
        bm=m;
        for(int i=0;i<bn;i++){
            for(int j=0;j<bm;j++){
                barr[i][j]=arr[stagen][i][j];
            }
        }
        bwin=newwin(bn+10,bm+10,0,0);

        swin=newwin(11,20,0,bm+10);
        
        mwin=newwin(11,20,11,bm+10);

        get_growth();
        get_poison();
        get_gate();
    }

    void print(){
        box(bwin,0,0);
        mvwprintw(bwin,0,0,"board");
        for(int i=0;i<bn;i++){
            for(int j=0;j<bm;j++){
                mvwprintw(bwin,i+5,j+5,"%c",barr[i][j]);
            }
        }
        wrefresh(bwin);

        box(swin,0,0);
        mvwprintw(swin,0,0,"score");
        wrefresh(swin);

        box(mwin,0,0);
        mvwprintw(mwin,0,0,"mission");
        wrefresh(mwin);

        return;
    }

    void get_growth(){
        if(barr[growthxy.first][growthxy.second]=='+') barr[growthxy.first][growthxy.second]=' ';
        int x,y;
        do{
            x=rand()%bn;
            y=rand()%bm;
        }while(barr[x][y]!=' '||make_pair(x,y)==growthxy);
        barr[x][y]='+';
        growthxy={x,y};
        gettimeofday(&growthtime,NULL);
        return;
    }
    void get_poison(){
        if(barr[poisonxy.first][poisonxy.second]=='-') barr[poisonxy.first][poisonxy.second]=' ';
        int x,y;
        do{
            x=rand()%bn;
            y=rand()%bm;
        }while(barr[x][y]!=' '||make_pair(x,y)==poisonxy);
        barr[x][y]='-';
        poisonxy={x,y};
        gettimeofday(&poisontime,NULL);
        return;
    }
    void get_gate(){
        if(barr[gate[0].first][gate[0].second]=='%') barr[gate[0].first][gate[0].second]='#';
        if(barr[gate[1].first][gate[1].second]=='%') barr[gate[1].first][gate[1].second]='#';
        for(int k=0;k<2;k++){
            int gn=0;
            for(int i=0;i<bn;i++) for(int j=0;j<bm;j++) gn+=(barr[i][j]=='#');
            int tmp=rand()%gn;
            for(int i=0;i<bn;i++){
                for(int j=0;j<bm;j++){
                    if(barr[i][j]!='#') continue;
                    if(tmp==0){
                        barr[i][j]='%';
                        gate[k]={i,j};
                    }
                    tmp--;
                }
            }
        }
        gettimeofday(&gatetime,NULL);
        return;
    }
};
