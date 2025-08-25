#include <iostream>
#include <graphics.h> //EasyX的头文件 (图形界面和基本图形绘制头文件)
#include <windows.h> //GetAsyncKeyState头文件 
#pragma comment(lib,"winmm.lib")
#include <time.h>
#include <stdlib.h>
//空格跳跃
//P键暂停
//小鸟和水管相撞？ 小鸟左上角的(x,y)与水管左上角的(x,y)相减的绝对值小于小鸟的半径就认为碰撞 
//背景音乐  使用winmm.lib多媒体开发库 需要加入#pragma comment(lib,"winmm.lib")  
#define menu_game_start 0
#define menu_game_opt 1
#define menu_game_help 2
#define menu_game_exit 3
#define birdWidth 64
#define birdHeight  55 
#define COIN_WIDTH 32
#define COIN_HEIGHT 32 
#define pipWidth 140
#define pipHeight  600
#define PIPE_GAP 250  // 管道之间的间隙
#define MAX_PIPES 5   // 最大管道数量
#define COIN_COUNT 3 //最大金币数量 
#define COIN_LEFT 200  //金币出现区域左边界
#define COIN_RIGHT 700  //金币出现区域右边界 
#define COIN_TOP 100 	//金币出现区域上边界 
#define COIN_BOTTOM 800 //金币出现区域下边界 
#define COIN_SPEED 2

//全局变量 
int menuSelect = 0; 
int birdX = 100;
int birdY = 200;
//int pipUpX = 400;
//int pipUpY = 500;
//int pipDownX = 750;
//int pipDownY = -200; 
IMAGE aniImg; //由于要在函数内显示，所以要写成全局变量 
bool gameOver = false;    // 纪录游戏是否结束 

void showMenu();
void showCoins(int x,int y);
bool checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
void generatePipes();
void updatePipes();
void generateCoins();
void drawCoins();
void CoinCollision();
void updateCoins();

// 管道结构体
typedef struct {
    int x;
    int upY;
    int downY;
    bool passed; // 是否已被小鸟通过
} Pipe;

//金币结构体
typedef struct{
	int x;
	int y;
	bool appear; //是否该显示 
} Coin; 

Pipe pipes[MAX_PIPES]; // 使用数组存储管道
int pipeCount = 0;     // 当前管道数量

Coin coins[COIN_COUNT]; //金币数组 
int coinCount = 0; //当前金币数量 

int main(int argc, char** argv) 
{
	// 初始化随机数种子
    srand((unsigned int)time(NULL));
    //金币初始化
	for (int i = 0; i < COIN_COUNT ;i++)
	{
		coins[i].appear = false;	
	} 
    
	IMAGE menuImg; //用来保存游戏菜单背景的变量
	IMAGE backImg; //用来保存游戏背景的变量 
	IMAGE birdBkImg;
	IMAGE birdImg;
	IMAGE pipUpBkImg;
	IMAGE pipUpImg;
	IMAGE pipDownBkImg;
	IMAGE pipDownImg;
	IMAGE deadImg; //用来保存碰撞死亡背景的变量 
	IMAGE deadBkImg;//死亡前景 
	
	// 获取图像尺寸
	int deadX = 200;
	int deadY = 300;

	bool gamePause = false;   // 记录游戏是否暂停
	bool spaceKeyWasPressed = false; // 记录空格键上次状态
	bool pKeyWasPressed = false;     // 记录P键上次状态
	
	char* deadStr = "按Enter以结束...";
	RECT rect_dead = {200,400,456,440};

	initgraph(900,1051); //创建窗口并调出控制台 使用EasyX库前需要在项目属性——参数——链接中添加-leasyx 否则会报错	
	loadimage(&menuImg,"./img/back1111.jpg");
    putimage(0, 0, &menuImg);
    
    //播放菜单音乐 
    mciSendString("open sound/menu1.mp3 alias bgm1",NULL,0,NULL);
	mciSendString("play bgm1 from 0 repeat",NULL,0,NULL); //循环播放
	 
	showMenu();

	//游戏开始代码
	loadimage(&backImg,"./img/back121.jpg"); //显示游戏背景
	loadimage(&birdBkImg,"./img/bird1_1.jpg");//显示小鸟前景图片  
	loadimage(&birdImg,"./img/bird1_2.jpg");//显示小鸟图片(背景)
	loadimage(&pipUpBkImg,"./img/stone_up2.gif");
	loadimage(&pipUpImg,"./img/stone_up1.gif");
	loadimage(&pipDownBkImg,"./img/stone_down2.gif");
	loadimage(&pipDownImg,"./img/stone_down1.gif");
	loadimage(&aniImg,"./img/ani.bmp");
	
	//加入音乐前要在项目属性——参数——链接中添加-lwinmm 
	
	mciSendString("open sound/menu3.mp3 alias bgm2",NULL,0,NULL);
	mciSendString("play bgm2 from 0 repeat",NULL,0,NULL);
	printf("test.....\n");
	
	// 生成初始管道
    generatePipes();
    
	while(1)
	{	
		
		if(gamePause == false)
		{
			BeginBatchDraw();
			mciSendString("close bgm1", NULL, 0, NULL);
			putimage(0,0,&backImg);
//			putimage(pipUpX,pipUpY,&pipUpBkImg,NOTSRCERASE);
//			putimage(pipUpX,pipUpY,&pipUpImg,SRCINVERT);
//			putimage(pipDownX,pipDownY,&pipDownBkImg,NOTSRCERASE);
//			putimage(pipDownX,pipDownY,&pipDownImg,SRCINVERT);

			// 绘制所有管道
            for(int i = 0; i < pipeCount; i++) {
                putimage(pipes[i].x, pipes[i].upY, &pipUpBkImg, NOTSRCERASE);
                putimage(pipes[i].x, pipes[i].upY, &pipUpImg, SRCINVERT);
                putimage(pipes[i].x, pipes[i].downY, &pipDownBkImg, NOTSRCERASE);
                putimage(pipes[i].x, pipes[i].downY, &pipDownImg, SRCINVERT);
            }
            
            putimage(birdX,birdY,&birdBkImg,NOTSRCERASE);
			putimage(birdX,birdY,&birdImg,SRCINVERT);
			
			generateCoins();
			updateCoins();
			drawCoins();
//			showCoins(430, 500);
			EndBatchDraw();
			birdY+=3;
			updatePipes();
		}
		if(gameOver)
		{
			break;
		}
		if(birdY > 800) //超出边界
		{
			birdY = 0; 
		}
//		if(abs(birdX-pipUpX) < 7 && abs(birdY-pipUpY) < 7)
//		{
//			printf("小鸟与水管发生相撞\n");
//			gameOver = true;
//		} 

		//金币碰撞检测
		CoinCollision();
		
		 
		//管道碰撞检测
        for(int i = 0; i < pipeCount; i++) {
            if (checkCollision(birdX, birdY, birdWidth, birdHeight, pipes[i].x, pipes[i].upY, pipWidth, pipHeight) || 
                checkCollision(birdX, birdY, birdWidth, birdHeight, pipes[i].x, pipes[i].downY, pipWidth, pipHeight)) 
            {
                mciSendString("open sound/dead.mp3 alias dead",NULL,0,NULL);
                mciSendString("play dead from 0",NULL,0,NULL);
                printf("小鸟与水管发生碰撞\n");
                gamePause = true;
                loadimage(&deadImg,"./img/gameover.jpg");
                loadimage(&deadBkImg,"./img/gameoverback.jpg");
                putimage(deadX,deadY,&deadBkImg,NOTSRCERASE);
                putimage(deadX,deadY,&deadImg,SRCINVERT); 
                
                if(GetAsyncKeyState(VK_RETURN))         
                {
                    gameOver = true;
                }
                drawtext(deadStr,&rect_dead,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
                break;
            }
        }
        
		Sleep(50);
		
		//空格键跳跃
		if(GetAsyncKeyState(VK_SPACE) && !spaceKeyWasPressed && !gamePause)  
		{
			birdY -= 50;
			mciSendString("open sound/jump.mp3 alias jump",NULL,0,NULL);
			mciSendString("play jump from 0 ",NULL,0,NULL);
			spaceKeyWasPressed= true;
		}
		else if(!GetAsyncKeyState(VK_SPACE))
		{
			spaceKeyWasPressed= false;
		}
		
		//P键暂停
		if(GetAsyncKeyState('P') && !pKeyWasPressed)  
		{
			gamePause = !gamePause;
			pKeyWasPressed = true;
			if (gamePause)
            {
                printf("游戏已暂停\n"); 
            }
            else
            {
               printf("游戏已继续\n");
            }
		}
		else if(!GetAsyncKeyState('P'))
		{
			pKeyWasPressed = false;
		}
		
	}
	printf("游戏结束\n");
	mciSendString("close bgm2", NULL, 0, NULL); 
	return 0;
}

void showMenu()
{
	char *str = "开始游戏";
	char *game_opt = "游戏设置";
	char *game_help = "游戏帮助";
	char *game_exit = "游戏退出";
	RECT rect_value = {250,200,500,250};
	RECT rect_game_opt = {250,250,500,300};
	RECT rect_game_help = {250,300,500,350};
	RECT rect_game_exit = {250,350,500,400};
//	rectangle(200,120,450,170);	//绘制空白矩形 (left,top,right,bottom) 
	setfillcolor(BLUE);	//函数用于设置你要填充的颜色 
	
	fillrectangle(250,200,500,250);//绘制填充矩形 
	drawtext(str,&rect_value,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	fillrectangle(250,250,500,300);//x轴不变，y轴在上一个框的基础上加50
	drawtext(game_opt,&rect_game_opt,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	fillrectangle(250,300,500,350);
	drawtext(game_help,&rect_game_help,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	fillrectangle(250,350,500,400); 
	drawtext(game_exit,&rect_game_exit,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	
	//开始游戏默认为红色 
	setfillcolor(RED);
	fillrectangle(250,200,500,250);//绘制填充矩形 
	drawtext(str,&rect_value,DT_CENTER|DT_SINGLELINE|DT_VCENTER);//字体会被覆盖，还得重新再写一遍
	 
	//动态菜单最核心的就是键盘捕获，如何捕获键盘上下键
	//kbhit keybord hit  头文件 conio.h 
	//GetAsyncKeyState 实时、异步（每时每刻都在查询，不同于同步只有消息队列到了才查询）地查询键盘或鼠标按钮的瞬时状态
	while(1)
	{
		Sleep(100); //防止跳转过快 
		if(GetAsyncKeyState(VK_DOWN)) //向下键被按下了 
		{
			mciSendString("open sound/select.mp3 alias select",NULL,0,NULL);
			mciSendString("play select from 0 ",NULL,0,NULL);
			switch(menuSelect)
			{
				//由于我们默认为开始游戏，所以第一次按下应该跳转到游戏设置 
				case menu_game_start:
					setfillcolor(BLUE); //恢复为蓝色  
					fillrectangle(250,200,500,250);//绘制填充矩形 
					drawtext(str,&rect_value,DT_CENTER|DT_SINGLELINE|DT_VCENTER);//字体会被覆盖，还得重新再写一遍 
						
					setfillcolor(RED);
					fillrectangle(250,250,500,300);
					drawtext(game_opt,&rect_game_opt,DT_CENTER|DT_SINGLELINE|DT_VCENTER);//字体会被覆盖，还得重新再写一遍 
					menuSelect = menu_game_opt; 
					break;
					
				case menu_game_opt:
					setfillcolor(BLUE); //恢复为蓝色  
					fillrectangle(250,250,500,300);//绘制填充矩形 
					drawtext(game_opt,&rect_game_opt,DT_CENTER|DT_SINGLELINE|DT_VCENTER);//字体会被覆盖，还得重新再写一遍 
							
					setfillcolor(RED);
					fillrectangle(250,300,500,350);
					drawtext(game_help,&rect_game_help,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
					menuSelect = menu_game_help; 	
					break;	
					
				case menu_game_help:
					setfillcolor(BLUE);
					fillrectangle(250,300,500,350);
					drawtext(game_help,&rect_game_help,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
					
					setfillcolor(RED);
					fillrectangle(250,350,500,400); 
					drawtext(game_exit,&rect_game_exit,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
					menuSelect = menu_game_exit;
					break;
						
				case menu_game_exit:
					setfillcolor(BLUE);
					fillrectangle(250,350,500,400); 
					drawtext(game_exit,&rect_game_exit,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
					
					setfillcolor(RED);
					fillrectangle(250,200,500,250);//绘制填充矩形 
					drawtext(str,&rect_value,DT_CENTER|DT_SINGLELINE|DT_VCENTER);//字体会被覆盖，还得重新再写一遍 
					menuSelect = menu_game_start;
					break;	
			}
		}
		if(GetAsyncKeyState(VK_UP)) //逻辑跟VK_Down差不多，由下变上就行了 
		{	
			mciSendString("open sound/select.mp3 alias select",NULL,0,NULL);
			mciSendString("play select from 0 ",NULL,0,NULL);
			switch(menuSelect)
			{
				//由于我们默认为开始游戏，所以第一次按下应该跳转到游戏设置 
				case menu_game_start:
					setfillcolor(BLUE); //恢复为蓝色  
					fillrectangle(250,200,500,250);//绘制填充矩形 
					drawtext(str,&rect_value,DT_CENTER|DT_SINGLELINE|DT_VCENTER);//字体会被覆盖，还得重新再写一遍 
						
					setfillcolor(RED);
					fillrectangle(250,350,500,400); 
					drawtext(game_exit,&rect_game_exit,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
					menuSelect = menu_game_exit;
					break;
					
				case menu_game_opt:
					setfillcolor(BLUE); //恢复为蓝色  
					fillrectangle(250,250,500,300);//绘制填充矩形 
					drawtext(game_opt,&rect_game_opt,DT_CENTER|DT_SINGLELINE|DT_VCENTER);//字体会被覆盖，还得重新再写一遍 
							
					setfillcolor(RED);
					fillrectangle(250,200,500,250);//绘制填充矩形 
					drawtext(str,&rect_value,DT_CENTER|DT_SINGLELINE|DT_VCENTER);//字体会被覆盖，还得重新再写一遍 
					menuSelect = menu_game_start; 	
					break;	
					
				case menu_game_help:
					setfillcolor(BLUE);
					fillrectangle(250,300,500,350);
					drawtext(game_help,&rect_game_help,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
					
					setfillcolor(RED);
					fillrectangle(250,250,500,300);//绘制填充矩形 
					drawtext(game_opt,&rect_game_opt,DT_CENTER|DT_SINGLELINE|DT_VCENTER);//字体会被覆盖，还得重新再写一遍 
					menuSelect = menu_game_opt;
					break;
						
				case menu_game_exit:
					setfillcolor(BLUE);
					fillrectangle(250,350,500,400); 
					drawtext(game_exit,&rect_game_exit,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
					
					setfillcolor(RED);
					fillrectangle(250,300,500,350);
					drawtext(game_help,&rect_game_help,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
					menuSelect = menu_game_help;
					break;	
			}	
		}
		if(GetAsyncKeyState(VK_RETURN))
		{
			switch(menuSelect)
			{
				case menu_game_start:
				
					return;	 //showMenu()函数结束
					break; 
				case menu_game_opt:
					break;
				case menu_game_help:
					break;
				case menu_game_exit:
					gameOver = true;
					return;
					break;
			}
		}
	}
}

void showCoins(int x,int y)
{
	static int index = 0; //静态变量，避免每次刷新都被赋值为0 
	if(index>=4) index = 0;
	putimage(x,y,COIN_WIDTH,COIN_HEIGHT,&aniImg,(0+index)*COIN_WIDTH,9*COIN_HEIGHT,SRCAND);
	putimage(x,y,COIN_WIDTH,COIN_HEIGHT,&aniImg,(0+index)*COIN_WIDTH,8*COIN_HEIGHT,SRCPAINT);
	index++;
	
}

// 矩形碰撞检测函数
bool checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
    // 检查两个矩形是否重叠
    if (x1 + w1 >= x2 &&     // 右边缘超过左边缘
        x1 <= x2 + w2 &&     // 左边缘超过右边缘
        y1 + h1 >= y2 &&     // 下边缘超过上边缘
        y1 <= y2 + h2) {     // 上边缘超过下边缘
        return true;
    }
    return false;
}

void generatePipes()
{
	pipeCount = 3; //初始生成三对管道
	for(int i = 0; i < pipeCount; i++) 
	{
        int gapPosition = (rand() % 500) + 200;  
        
        pipes[i].x = 900 + i * 400; // 每对管道间隔400像素
        pipes[i].upY = gapPosition - pipHeight; //上方的管道，可能为负数，是正常的，因为上管道高度要把负数抵消才会延伸出来  
        pipes[i].downY = gapPosition + PIPE_GAP;//下方的管道加PIPE_GAP是为了和上管道隔开一段空间，免得两个管道合在一起了，小鸟过不去 
        pipes[i].passed = false; 
	}
}

// 更新管道位置
void updatePipes() 
{
    for(int i = 0; i < pipeCount; i++) 
	{
        pipes[i].x -= 3;
        
        // 如果管道移出屏幕左侧，重置到右侧
        if(pipes[i].x + pipWidth < 0) 
		{
            int gapPosition = (rand() % 500) + 200; //管道最高为600，应该维持在200-500这个区
            pipes[i].x = 900;
            pipes[i].upY = gapPosition - pipHeight;
            pipes[i].downY = gapPosition + PIPE_GAP;
            pipes[i].passed = false;
        }
    }
}

//更改金币位置 
void updateCoins()
{
	for(int i = 0; i < COIN_COUNT; i++) {
        if(coins[i].appear) {
            coins[i].x -= COIN_SPEED; // 金币向左移动
    
            // 如果金币移出屏幕左侧，重置它
            if(coins[i].x + COIN_WIDTH < 0) {
                coins[i].appear = false;
                coinCount--;
            }
        }
    }
}

//生成金币 
void generateCoins()
{
	if(rand() % 100 < 20 && coinCount < COIN_COUNT)//随机决定是否生成新金币，20%的几率生成新金币
	{
		for(int i = 0; i < COIN_COUNT ;i++)
		{
			if(!coins[i].appear)
			{
				coins[i].x = COIN_LEFT + rand() % (COIN_RIGHT - COIN_LEFT);
				coins[i].y = COIN_TOP + rand() % (COIN_BOTTOM - COIN_TOP);
				coins[i].appear = true;
				coinCount++;
				break;	
			}	
		}	
	} 
}

//绘制金币
void drawCoins()
{
	for(int i = 0;i < COIN_COUNT ;i++)
	{
		if(coins[i].appear)
		{
			showCoins(coins[i].x,coins[i].y);	
		}	
	}	
} 


//检测金币碰撞
void CoinCollision()
{
	for(int i = 0; i< COIN_COUNT ; i++)
	{
		if(coins[i].appear && checkCollision(birdX,birdY,birdWidth,birdHeight,coins[i].x,coins[i].y,COIN_WIDTH,COIN_HEIGHT))
		{
			// 碰撞到金币，播放音效，增加分数等
            mciSendString("open sound/coins.mp3 alias coin", NULL, 0, NULL);
            mciSendString("play coin from 0", NULL, 0, NULL);
            coins[i].appear = false;
            coinCount--;
            // 这里可以增加分数	
		}	
	}	
} 
