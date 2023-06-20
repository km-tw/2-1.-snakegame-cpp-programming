#include <ncurses.h>
#include <deque>
#include <sys/time.h>
#include <stdlib.h>
#include <time.h>
#include <utility>
#include "board.h"
using namespace std;

class snake : public board{
    public:

    deque<pair<int,int> > dq;
    int direction;
    int dx[4]={-1,0,1,0},dy[4]={0,1,0,-1};

    int msize,growthn,poisonn,gaten;

    int ingate;

    snake(int a=0,int n=21,int m=21):board(a,n,m){
        int startx=startxy[stagen].first,starty=startxy[stagen].second;
        dq.push_back({startx,starty});
        barr[startx][starty]='@';
        dq.push_back({startx+1,starty});
        barr[startx+1][starty]='O';
        dq.push_back({startx+2,starty});
        barr[startx+2][starty]='O';
        direction=0;

        msize=3;
        growthn=poisonn=gaten=0;

        ingate=0;
    }

    void print(){
        board::print();

        wrefresh(bwin);

        msize=max(msize,(int)dq.size());
        mvwprintw(swin,2,1,"length : %d / %d",(int)dq.size(),msize);
        mvwprintw(swin,4,1,"growth : %d",growthn);
        mvwprintw(swin,6,1,"poison : %d",poisonn);
        mvwprintw(swin,8,1,"gate : %d",gaten);
        wrefresh(swin);

        mvwprintw(mwin,2,1,"length : %d (%c)",missionlength[stagen],(int)dq.size()<missionlength[stagen]?'x':'v');
        mvwprintw(mwin,4,1,"growth : %d (%c)",missiongrowth[stagen],growthn<missiongrowth[stagen]?'x':'v');
        mvwprintw(mwin,6,1,"poison : %d (%c)",missionpoison[stagen],poisonn<missionpoison[stagen]?'x':'v');
        mvwprintw(mwin,8,1,"gate : %d (%c)",missiongate[stagen],gaten<missiongate[stagen]?'x':'v');
        wrefresh(mwin);

        return;
    }

    int move(){
        int fx=dq.front().first,fy=dq.front().second;
        int bx=dq.back().first,by=dq.back().second;
        
        barr[bx][by]=' ';
        dq.pop_back();

        int nx=fx+dx[direction],ny=fy+dy[direction];

        if(barr[nx][ny]=='#') return 0;

        if(barr[nx][ny]=='%'){
            if(ingate>1) return 0;

            if(make_pair(nx,ny)==gate[0]){
                nx=gate[1].first;
                ny=gate[1].second;
            }
            else{
                nx=gate[0].first;
                ny=gate[0].second;
            }
            
            for(int i=0;i<4;i++){
                int nnx=nx+dx[(direction+i)%4],nny=ny+dy[(direction+i)%4];
                if(!((0<=nnx&&nnx<bn)&&(0<=nny&&nny<bm))) continue;
                if(barr[nnx][nny]=='#'||barr[nnx][nny]=='O'||barr[nnx][nny]=='%'||barr[nnx][nny]=='X') continue;
                nx=nnx;
                ny=nny;
                direction=(direction+i)%4;
                break;
            }
            if(barr[nx][ny]=='%') return 0;
            ingate=(int)dq.size()+1;
            gaten++;
        }

        if(barr[nx][ny]=='+'){
            barr[bx][by]='O';
            dq.push_back({bx,by});

            get_growth();
            growthn++;
        }
        else if(barr[nx][ny]=='-'){
            barr[dq.back().first][dq.back().second]=' ';
            dq.pop_back();
            
            ingate=max(0,ingate-2);

            get_poison();
            poisonn++;
        }
        else{
            ingate=max(0,ingate-1);
        }

        if(barr[nx][ny]=='O') return 0;
        
        barr[nx][ny]='@';
        dq.push_front({nx,ny});
        barr[fx][fy]='O';

        print();

        return (int)dq.size()>=3;
    }

    void set_direction(int d){
        if(d==(direction+2)%4) return;
        direction=d;
        return;
    }

    void get_growth(){
        board::get_growth();
        return;
    }
    void get_poison(){
        board::get_poison();
        return;
    }
    void get_gate(){
        board::get_gate();
        return;
    }

    int get_satisfy(){
        if((int)dq.size()<missionlength[stagen]) return 0;
        else if(growthn<missiongrowth[stagen]) return 0;
        else if(poisonn<missionpoison[stagen]) return 0;
        else if(gaten<missiongate[stagen]) return 0;
        else return 1;
    }
};
