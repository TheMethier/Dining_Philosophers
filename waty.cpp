#include <iostream>
#include<mutex>
#include <cstring>
#include<ncurses.h>
#include<thread>
#include<cstdlib>        
#include <chrono>
using namespace std;

struct Forks
{
    Forks()
    {
        Id=0;
    }
    Forks(int val)
    {
        Id=val;
    }

    int Id;
    string State;
    mutex p;
};

struct phil
{
    phil(int id,string state)
    {
        Id=id;
        State=state;
        LeftForkIndex=-1;
        RightForkIndex=-1;
    }

    phil()
    {
        State="Exist in the world of ideas.";
    }

    int Id;
    string State;
    int LeftForkIndex;
    int RightForkIndex;
    chrono::milliseconds Duration;
    bool IsReady;
};

void philo(phil *p ,Forks *l,Forks *r)
{
    while(1)
    {
    p->IsReady=true;    
    p->State="Thinking...";
    this_thread::sleep_for(chrono::milliseconds(rand()%600+1000));
    unique_lock<mutex> left(l->p);//Blokowanie lewego widecla
    unique_lock<mutex> right (r->p);//Blokowanie prawego widelca
    //Wejscie do sekcji krytycznej:
        p->LeftForkIndex=l->Id;
        p->RightForkIndex=r->Id;
        p->IsReady=false;
        p->IsReady=true;
        p->State="Eating...";
        this_thread::sleep_for(chrono::milliseconds(rand()%600+1000));
    right.unlock();
    left.unlock();
    p->LeftForkIndex=-1;
    p->RightForkIndex=-1;
    p->IsReady=false;
    }

}

void o(phil *p,Forks *f,const int n)
{
    initscr();
    while(1)
    {
        int height=15;
        int width=80;
        int start_x=1;
        int start_y=1;
        WINDOW *win=newwin(height,width,start_x,start_y);
        box(win,0,0);
        for(int i=0;i<n;i++)
        {
            mvwprintw(win,i+2,3,"%i",p[i].Id);
            const char* statep = p[i].State.c_str();
            mvwprintw(win,i+2,15,"%s",statep);
            if(p[i].Duration<chrono::milliseconds(1000)){
            mvwprintw(win,i+2,30,"%i",((p[i].Duration)));
            mvwprintw(win,i+2,38,"%s","ms");
            }
            else{
            mvwprintw(win,i+2,30,"%i",((p[i].Duration/1000)));
            mvwprintw(win,i+2,38,"%s","s");
            }
            if(p[i].LeftForkIndex!=-1){
            mvwprintw(win,i+2,45,"%i",p[i].LeftForkIndex);
            }
            if(p[i].RightForkIndex!=-1){
            mvwprintw(win,i+2,65,"%i",p[i].RightForkIndex);}
        }
        mvwprintw(win,1,1,"PhilNumber");
        mvwprintw(win,1,15,"PhilState");
        mvwprintw(win,1,30,"Duration");
        mvwprintw(win,1,45,"LeftFork");
        mvwprintw(win,1,65,"RightFork");

        redrawwin(win);
        wrefresh(win);
    }
}

void timer(phil *t,const int n)
{
    while(1){

            if(t->IsReady)
            {
                this_thread::sleep_for(chrono::milliseconds(1));
                t->Duration+=chrono::milliseconds(1);
            }
            else{
            t->Duration=chrono::milliseconds(0);
            }
    }    
    
}
int main(int argc,char** argv)
{
static const int  n=7;
phil philos[n];
thread t[n];
Forks f[n];
for(int i=0;i<n;i++)
{
    f[i].Id=i;

}
thread timr[n];
for(int i=0; i<n;i++)
{
    philos[i]=phil(i,"Comming...");
    timr[i]=thread(timer,&philos[i],n);
   if(i!=n-1)
    {
    t[i]=thread(philo,&philos[i],&f[i],&f[i+1]);
    }
    else
    {
    t[i]=thread(philo,&philos[i],&f[i],&f[0]);
    }
}
thread print=thread(o,philos,f,n);
for(int i=0;i<n;i++)
{ 
    print.join();  
    timr[i].join();
    t[i].join();
}

return 0;

}

