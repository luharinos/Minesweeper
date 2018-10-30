//minesweeper game
#include<graphics.h>
#include<iostream.h>
#include<stdlib.h>
#include<conio.h>
#include<stdio.h>
#include<timer.h>
#include<math.h>
#include<dos.h>

int gameOver = 1;
union REGS i, o;

void welcome(int maxx, int maxy) {
	int midx=maxx/2;
	int midy=maxy/2;
	int p1=midx-220,p2=midy-120,p3=midx+220,p4=midy+120;

	int pts[]={p1,p2,p3,p2,p3,p4,p1,p4,p1,p2};
	setfillstyle(1,RED);
	fillpoly(4,pts);
	rectangle(p1-2,p2-2,p3+2,p4+2);

	settextjustify(1,1);
	settextstyle(4,0,0);
	outtextxy(midx,p2+40,"Welcome");
	outtextxy(midx,p2+80,"to");
	settextstyle(4,0,5);
	outtextxy(midx,p2+120,"The Minesweeper Game");
	settextstyle(2,0,0);
	outtextxy(midx,p4-10,"(Press any key to continue..)");

	getch();
	cleardevice();
}

void bgConsole(int maxx, int maxy) {
	setbkcolor(9);

	//border
	rectangle(2,2,maxx-2,maxy-2);
	rectangle(4,4,maxx-4,maxy-4);

	//title background
	setfillstyle(1,15);
	int p[10]={maxx/2-100,0,maxx/2+100,0,maxx/2+100,15,maxx/2-100,15,maxx/2-100,15};
	fillpoly(4,p);

	//title
	settextjustify(1,1);
	settextstyle(2,0,7);
	setcolor(9);
	outtextxy(maxx/2,4,"MINESWEEPER GAME");
}

void smallBlock(int x, int y, int n) {
	int pts[]={x,y,x+20,y,x+20,y+20,x,y+20,x,y};
	if(n==1)
		setfillstyle(1,8);
	else
		setfillstyle(1,7);
	fillpoly(4,pts);

	if(n==3) {
		setfillstyle(1,8);
		setcolor(8);
		fillellipse(x+10,y+10,4,4);
		setcolor(15);
	}
	if(n==4) {
		settextjustify(1,1);
		setcolor(8);
		settextstyle(2,0,5);
		outtextxy(x+10,y+7,"n");
		setcolor(15);
	}
}

void sidePanel(int maxx, int maxy) {
	//STATISTICS

	//Container
	rectangle(450,45,maxx-25,maxy/2-12);
	int pts1[]={510,40,555,40,555,60,510,60,555,40};
	setcolor(9);
	setfillstyle(1,9);
	fillpoly(4,pts1);
	setcolor(15);
	settextstyle(2,0,5);
	outtextxy(533,45,"STATS");

	//Content
	settextjustify(0,1);
	outtextxy(460,70,"Score:");
	outtextxy(460,120,"Moves:");
	outtextxy(460,170,"Time:");

	//HELP
	int y=maxy/2+12;
	//Container
	rectangle(450,y,maxx-25,maxy-34);
	int pts2[]={515,y-5,550,y-5,550,y+15,515,y+15,550,y-5};
	setcolor(9);
	setfillstyle(1,9);
	fillpoly(4,pts2);
	setcolor(15);
	settextstyle(2,0,5);
	settextjustify(1,1);
	outtextxy(533,y,"HELP");

	//Content
	settextjustify(0,1);
	outtextxy(460,y+25,"W - Move Up");
	outtextxy(460,y+45,"A - Move Right");
	outtextxy(460,y+65,"S - Move Down");
	outtextxy(460,y+85,"D - Move Left");
	outtextxy(460,y+105,"Space - Open Area");

	settextstyle(2,0,4);
	smallBlock(460,y+125,1);
	outtextxy(485,y+128,"Closed");
	outtextxy(485,y+138,"Area");
	smallBlock(533,y+125,2);
	outtextxy(558,y+128,"Mine-free");
	outtextxy(558,y+138,"Area");
	smallBlock(460,y+160,3);
	outtextxy(485,y+163,"Mine in");
	outtextxy(485,y+173,"the Area");
	smallBlock(533,y+160,4);
	settextstyle(2,0,4);
	settextjustify(0,1);
	outtextxy(558,y+163,"'n' Mines");
	outtextxy(558,y+173,"Around");
}


void uncoveredBlock(int x, int y) {
	int pts[]={x,y,x+50,y,x+50,y+50,x,y+50,x,y};
	setfillstyle(1,7);
	fillpoly(4,pts);
}

void bomb(int x, int y) {
	uncoveredBlock(x,y);
	setcolor(8);
	setfillstyle(1,8);
	fillellipse(x+25,y+25,10,10);
	setcolor(15);
}

void bombAround(int x, int y, char ch[]) {
	uncoveredBlock(x,y);
	settextjustify(1,1);
	settextstyle(1,0,5);
	setcolor(8);
	outtextxy(x+30,y+20,ch);
	setcolor(15);
}

void coveredBlock(int x, int y) {
	int pts[]={x,y,x+50,y,x+50,y+50,x,y+50,x,y};
	setfillstyle(1,8);
	fillpoly(4,pts);

}

void cursorBlock(int x, int y) {
	setcolor(13);
	rectangle(x,y,x+50,y+50);
	setcolor(15);
  }

char* convertChar(int a){
		switch(a){
			case 1:
				return "1";
			case 2:
				return "2";
			case 3:
				return "3";
			case 4:
				return "4";
			case 5:
				return "5";
			case 6:
				return "6";
			case 7:
				return "7";
			case 8:
				return "8";

			}
			return "0";
}

class cell {
  public:
  int revealed,isBomb,neighbor;
  int x,y,i,j,w;

  void input(int i,int j,int w){
   this->i = i;
   this->j = j;
   this->x = (i * w) + 25;
   this->y = (j * w) + 45;
   this->revealed = 0;
   this->isBomb = 0;
   this->neighbor = 0;
   }

  int countBomb(){
	if(this->isBomb){
		this->neighbor = -1;
		return 0;
	}
	int total = 0;
	for (int xoff = -1; xoff <= 1; xoff++) {
	    int i = this->i + xoff;
	    if (i < 0 || i > 7)
		continue;

	    for (int yoff = -1; yoff <= 1; yoff++) {
	      int j = this->j + yoff;
	      if (j < 0 || j > 7)
		continue;

	      cell n = grid[i][j];
	      if (n.isBomb) {
		total++;
	      }
	    }
	  }
	  this->neighbor = total;
	  return 0;
  }

  void show(){
   if(this->revealed){
    if(this->isBomb){
     //if cell is bomb
     bomb(this->x,this->y);
    } else if(this->neighbor > 0){
	  //if bomb present in neighbor
	  char* c = convertChar(this->neighbor);
	  bombAround(this->x,this->y,c);
	 } else {
	  //if blank cell
	  uncoveredBlock(this->x,this->y);
	 }
   } else {
   //if cell is not revealed
    coveredBlock(this->x,this->y);
   }
  }

  void gridCursor(){
   cursorBlock(this->x,this->y);
  }

  void reveal(){
	this->revealed = 1;
	this->show();
	  if(this->neighbor == 0) {
	    // fill neighbors time
	    this->fillNeighbor();
	    //printf("Blank");
	  }
	}

  void fillNeighbor(){
    for (int xoff = -1; xoff <= 1; xoff++) {
	    int i = this->i + xoff;
	    if (i < 0 || i > 7)
		continue;

	    for (int yoff = -1; yoff <= 1; yoff++) {
	      int j = this->j + yoff;
	      if (j < 0 || j > 7)
		continue;

	      if (!grid[i][j].isBomb && !grid[i][j].revealed) {
		grid[i][j].reveal();
		//printf("i : %d\tj: %d",i,j);
	      }
	    }
	  }
	}

}grid[8][8];


int chCursor(int a, int s){
  if(s==1 && a<7){
	a++;
  } else if(s==-1 && a>0){
	a--;
  }
  return a;
}

void GameOver(int maxx, int maxy){
 for(int i = 0 ; i < 8 ; i++){
  for(int j = 0 ; j < 8 ; j++){
   grid[i][j].revealed = 1;
   grid[i][j].show();
  }
 }
 delay(2500);
 cleardevice();
 setbkcolor(RED);
 delay(1000);
 setbkcolor(15);
 setcolor(8);
 settextstyle(1,0,3);
 outtextxy(maxx/2,maxy/2-50,"You Failed, Mine Bursted!");
 settextstyle(4,0,6);
 setcolor(RED);
 outtextxy(maxx/2,maxy/2,"Game Over");
 delay(5000);


 exit(0);

}

int initmouse() {
	i.x.ax=0;
	int86(0X33,&i,&o);
	return o.x.ax;
}

void show_mouse() {
	i.x.ax=1;
	i.x.cx=getmaxx()/2;
	i.x.dx=getmaxy()/2;
	int86(0X33,&i,&o);
}

void get_mouse_pos(int *x,int *y,int *button) {
	i.x.ax=3;
	int86(0x33,&i,&o);
	*x=o.x.cx;
	*y=o.x.dx;
	*button=o.x.bx;
}

void hide_mouse() {
	i.x.ax=2;
	int86(0x33,&i,&o);
}

int main() {
	clrscr();
	int gd=DETECT, gm, err, status;
	initgraph(&gd,&gm,"..\\BGI");

	err=graphresult();
	if (err != grOk) {  /* an error occurred */
	      printf("Graphics error: %s\n", grapherrormsg(err));
	      printf("Press any key to halt:");
	      getch();
	      exit(1); /* terminate with an error code */
	}
	int maxx=getmaxx();
	int maxy=getmaxy();

	status=initmouse();

	if(status==0)
		printf("\nMouse support not available.");
	else
		show_mouse();

	welcome(maxx,maxy);

	//background
	bgConsole(maxx,maxy);

	//board
	setcolor(15);
	rectangle(25,45,425,445);

	//side panel
	sidePanel(maxx,maxy);
	int w = 50;
	int i,j;
	for(i = 0 ; i < 8 ; i++){
	 for(j = 0; j < 8 ; j++){
	  grid[i][j].input(i,j,w);
	 }
	}

	//calculating random position for bomb spot
	int option[3][64];
	int counter = 0;
	for(i = 0 ; i < 8 ; i++){
	 for(j = 0; j < 8 ; j++){
	  option[0][counter] = i;
	  option[1][counter] = j;
	  option[2][counter] = 1;
	  counter++;
	 }
	}

	//planting bomb at random position
	srand(time(NULL));
	int plantBomb,plantI,plantJ,numBomb = 10;
	for(i = 0; i < numBomb ; i++){
	 plantBomb = rand()%64;
	 if(option[2][plantBomb]){
	   plantI = option[0][plantBomb];
	   plantJ = option[1][plantBomb];
	   option[2][plantBomb] = 0;
	   grid[plantI][plantJ].isBomb = 1;
	 } else {
	    numBomb++;
	 }
	}

	//counting bombs
	for(i = 0 ; i < 8 ; i++){
	 for(j = 0 ; j < 8 ; j++){
	  grid[i][j].countBomb();
	 }
	}

	int cursor_i=3, cursor_j = 4;
	for(i = 0 ; i < 8 ; i++){
	 for(j = 0; j < 8 ; j++){
	  grid[i][j].show();
	 }
	}
	//main logic(game function)
	int x,y,button;
	do {
	//display all grids


	//grid[cursor_i][cursor_j].gridCursor();


	/*switch(getch()){
	 case 119: //w
	 case 87:
		cursor_j=chCursor(cursor_j,-1);
		break;
	 case 97:  //a
	 case 65:
		cursor_i=chCursor(cursor_i,-1);
		break;
	 case 115: //s
	 case 83:
		cursor_j=chCursor(cursor_j,1);
		break;
	 case 100: //d
	 case 68:
		cursor_i=chCursor(cursor_i,1);
		break;
	 case 27:  //esc
	       gameOver = 0;
	       break;
	 case 32:
		if(grid[cursor_i][cursor_j].isBomb){
		 GameOver(maxx,maxy);
		 //cout << "Opps!\t";
		} else {
		 grid[cursor_i][cursor_j].reveal();
		}
	}*/

	show_mouse();
	get_mouse_pos(&x,&y,&button);

	//x2=x1;
	//y2=y1;
	if(button==1)
	{
		get_mouse_pos(&x,&y,&button);
		x-=25;
		y-=45;
		cursor_i=x/50;
		cursor_j=y/50;
		if(cursor_i>=0 && cursor_i<=7 && cursor_j>=0 && cursor_j<=7) {
		if(grid[cursor_i][cursor_j].isBomb){
		 GameOver(maxx,maxy);
		 //cout << "Opps!\t";
		} else {
		 grid[cursor_i][cursor_j].reveal();
//		 hide_mouse();
		}     }
		//get_mouse_pos(&x2,&y2,&button);
	}
	if(kbhit()) {
	gameOver=0;
	break;}
//	displayTime();
//	delay(1000);
	}while(gameOver);

	closegraph();
	return 0;
}
