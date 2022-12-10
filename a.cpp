/*

zoom out 4 times on terminals before playing 

to play enter these lines:
$ g++ -std=c++11 -pthread a.cpp -oa
$ ./a

and hit enter

play on num-pad with num-lock on 
w + enter:  up
s + enter: down
d + enter: right
a + enter: left
Ctrl + c: terminate
*/

#include <bits/stdc++.h>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#define num 62

using namespace std;

condition_variable cv;

char direction='s';

void read_value(){
	cin>> direction;
	cv.notify_one();
}  

bool display[num][num];
int snake[300][2], snake_head, snake_tail, frogs[][2]={{13,4},{12,3},{20,40},{21,4},{0,0}}, frog_num=0, tot_frog_num = 5; // add more frogs

void pause_execution(double seconds){
	time_t start = time(0);
	while(difftime(time(0), start) < seconds) ;
}

void line(){
	int n=45, x=2,  y=1, s=6, s1=6 , x1=2, y1=1, ss=15,s2=15,x2=2, y2=1, sss=20, s3=20;
	while(n--)
	{
	 display[x][y]=1;
	y++;
	if(s==0) s=s1;
	else 
	{
	x++;
	s--;
	}
	 display[x1][y1]=1;
	y1++;
	x1++;
	}
}

void circle(int xcenter, int ycenter, int r, bool v){
	int x = 0, y = r, p = 1 - r;
	
	display[xcenter + x][ycenter + y] = v;	
	display[xcenter + x][ycenter - y] = v;
	display[xcenter - x][ycenter + y] = v;
	display[xcenter - x][ycenter - y] = v;
	display[xcenter + y][ycenter + x] = v;
	display[xcenter + y][ycenter - x] = v;
	display[xcenter - y][ycenter + x] = v;
	display[xcenter - y][ycenter - x] = v;
	
	while(x<y){
		x++;
		if( p < 0 ) p = p + 2 * x + 1;
		else{
			y--;
			p = p + 2 * (x - y) + 1;
		}
		display[xcenter + x][ycenter + y] = v;	
		display[xcenter + x][ycenter - y] = v;
		display[xcenter - x][ycenter + y] = v;
		display[xcenter - x][ycenter - y] = v;
		display[xcenter + y][ycenter + x] = v;
		display[xcenter + y][ycenter - x] = v;
		display[xcenter - y][ycenter + x] = v;
		display[xcenter - y][ycenter - x] = v;
	}
}

void Display(){
	for(int i=0; i<num; i++)
	{
		for(int j=0; j<num; j++) cout<<((display[i][j])?' ':'@')<<' ';
		cout<<"\n";
	}
}

void play_waves(){
	int t=2;
	while(t--){
		pause_execution(1);
		circle(30,30,5,1);
		Display();
		pause_execution(1);
		circle(30,30,8,1);
		Display();
		pause_execution(1);
		circle(30,30,12,1);
		Display();
		pause_execution(1);
		circle(30,30,16,1);
		Display();
		pause_execution(1);
		circle(30,30,20,1);
		Display();
		pause_execution(1);
		circle(30,30,20,0);
		Display();
		pause_execution(1);
		circle(30,30,16,0);
		Display();
		pause_execution(1);
		circle(30,30,12,0);
		Display();
		pause_execution(1);
		circle(30,30,8,0);
		Display();
		pause_execution(1);
		circle(30,30,5,0);
		Display();
	}
}
void basic_snake(){
	int snake_head[][2] = {{2,4},{3,4},{4,4},{4,5},{5,5},{6,5},{6,6},{6,7},{6,8},{6,9},{6,10},{7,10}}, len = 6;
	
	int len1 = len;
	while(len1--){
		display[snake_head[len1][0]][snake_head[len1][1]] = 1;
	}
	Display();
	pause_execution(1);
	for(int i = 5; i<12; i++){
		display[snake_head[i-5][0]][snake_head[i-5][1]] = 0;
		display[snake_head[i][0]][snake_head[i][1]] = 1;
		Display();
		pause_execution(1);
	}
}

void create_snake(int len){

	snake_tail = 0; 
	snake_head = len-1;
	
	for(int i = snake_tail; i<= snake_head; i++){
		snake[i][0] = i;
		snake[i][1] = 1;
		display[snake[i][0]][snake[i][1]] = 1;		
	}
	Display();
	pause_execution(1);
}

void run_snake(){
	display[frogs[frog_num][0]][frogs[frog_num++][1]] = 1;
	while(true){
		thread th(read_value);
		mutex mtx;
		unique_lock<mutex> lck(mtx);
		while(cv.wait_for(lck, chrono::milliseconds(200)) == cv_status::timeout){
			snake_head++;
			switch (direction){
				case 'w':
					snake[snake_head][0] = snake[snake_head-1][0]-1;
					snake[snake_head][1] = snake[snake_head-1][1]; 
					break;
				case 'd':
					snake[snake_head][0] = snake[snake_head-1][0];
					snake[snake_head][1] = snake[snake_head-1][1]+1; 
					break;
				case 's':
					snake[snake_head][0] = snake[snake_head-1][0]+1;
					snake[snake_head][1] = snake[snake_head-1][1]; 
					break;
				case 'a':
					snake[snake_head][0] = snake[snake_head-1][0];
					snake[snake_head][1] = snake[snake_head-1][1]-1; 
					break;
				default:
					snake[snake_head][0] = snake[snake_head-1][0]+1;
					snake[snake_head][1] = snake[snake_head-1][1]; 
			}
			
			if(!display[snake[snake_head][0]][snake[snake_head][1]]){ 
			 	display[snake[snake_tail][0]][snake[snake_tail][1]]=0;
			 	display[snake[snake_head][0]][snake[snake_head][1]] = 1;
			 	snake_tail++; 
			}
			else{										//frog found or struck to itself
			 	int x1 = snake[snake_head][0], y1= snake[snake_head][1], i;
			 	
			 	if(x1 == frogs[frog_num-1][0] && y1 == frogs[frog_num-1][1] && frog_num!=tot_frog_num){ 
			 		while(display[frogs[frog_num][0]][frogs[frog_num][1]]) frog_num++;
			 		display[frogs[frog_num][0]][frogs[frog_num][1]] = 1; //frog eaten and next frog created
			 		frog_num++;
			 	}
			 	else{
			 		play_waves();
			 		break;
			 	}
			}
			Display();
		}
		cout<<"press ctrl+c to exit\n";
		th.join();
	}
}

int main()
{
	//ios_base::sync_with_studio(false);
	cin.tie(0);
	
	for(int i = 0; i < 30; i++){ display[30][i] = 1; display[i][30] = 1; display[0][i] = 1; display[i][0] = 1;} //creates frame
	display[30][15]= 0;
	create_snake(12);
	run_snake();
	return 0;
}
