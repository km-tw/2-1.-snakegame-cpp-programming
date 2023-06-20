#include <ncurses.h>

#include <deque>

#include <sys/time.h>

#include <stdlib.h>

#include <time.h>

#include <utility>

#include <unistd.h>

#include "snake.h"

using namespace std;



int get_time(struct timeval ltime,struct timeval ctime){

    return (ctime.tv_sec - ltime.tv_sec) * 1000000 + ctime.tv_usec - ltime.tv_usec;

}



int main(){

    srand(time(NULL));



    initscr();                                                                                                        

    noecho();                                                                                                       

    cbreak(); 

    keypad(stdscr,TRUE);

    //nodelay(stdscr,TRUE);

    refresh();



    mvprintw(5,5,"game play");

    mvprintw(6,5,"press any key");

    refresh();

    getch();

    

    

    clear();

    mvprintw(5,5,"start");

    refresh();

    sleep(2);

    

    int flag=1;



    for(int s=0;s<4;s++){

        

        clear();

        mvprintw(5,5,"stage %d",s+1);

        refresh();

        sleep(2);



        snake tmp(s,21,21);

        tmp.print();



        timeout(10);

        struct timeval ltime,ctime;

        gettimeofday(&ltime,NULL);

        while(1){

            int c = getch();

            switch(c){

                case KEY_UP:

                    //printw("up");

                    tmp.set_direction(0);

                    break;

                case KEY_RIGHT:

                    //printw("right");

                    tmp.set_direction(1);

                    break;

                case KEY_DOWN:

                    //printw("down");

                    tmp.set_direction(2);

                    break;

                case KEY_LEFT:

                    //printw("left");

                    tmp.set_direction(3);

                    break;

            }

            gettimeofday(&ctime,NULL);

            if(get_time(ltime,ctime)>200000){

                if(!tmp.move()){

                    flag=0;

                    break;

                }

                ltime=ctime;

            }

            

            if(get_time(tmp.growthtime,ctime)>5000000){

                tmp.get_growth();

                tmp.print();

            }

            if(get_time(tmp.poisontime,ctime)>5000000){

                tmp.get_poison();

                tmp.print();

            }

            if(tmp.ingate==0&&get_time(tmp.gatetime,ctime)>5000000){

                tmp.get_gate();

                tmp.print();

            }

            if(tmp.get_satisfy()){

                clear();

                mvprintw(5,5,"stage %d clear!",s+1);

                refresh();

                sleep(2);

                break;

            }

        }

        if(flag==0) break;

    }



    clear();

    if(flag) mvprintw(5,5,"game clear!");

    else mvprintw(5,5,"game over..");

    refresh();

    sleep(2);



    endwin();

    return 0;

}
