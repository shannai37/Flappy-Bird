#include <iostream>
#include <graphics.h> //EasyX��ͷ�ļ� (ͼ�ν���ͻ���ͼ�λ���ͷ�ļ�)
#include <windows.h> //GetAsyncKeyStateͷ�ļ� 
#pragma comment(lib,"winmm.lib")
#include <time.h>
#include <stdlib.h>
//�ո���Ծ
//P����ͣ
//С���ˮ����ײ�� С�����Ͻǵ�(x,y)��ˮ�����Ͻǵ�(x,y)����ľ���ֵС��С��İ뾶����Ϊ��ײ 
//��������  ʹ��winmm.lib��ý�忪���� ��Ҫ����#pragma comment(lib,"winmm.lib")  
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
#define PIPE_GAP 250  // �ܵ�֮��ļ�϶
#define MAX_PIPES 5   // ���ܵ�����
#define COIN_COUNT 3 //��������� 
#define COIN_LEFT 200  //��ҳ���������߽�
#define COIN_RIGHT 700  //��ҳ��������ұ߽� 
#define COIN_TOP 100 	//��ҳ��������ϱ߽� 
#define COIN_BOTTOM 800 //��ҳ��������±߽� 
#define COIN_SPEED 2

//ȫ�ֱ��� 
int menuSelect = 0; 
int birdX = 100;
int birdY = 200;
//int pipUpX = 400;
//int pipUpY = 500;
//int pipDownX = 750;
//int pipDownY = -200; 
IMAGE aniImg; //����Ҫ�ں�������ʾ������Ҫд��ȫ�ֱ��� 
bool gameOver = false;    // ��¼��Ϸ�Ƿ���� 

void showMenu();
void showCoins(int x,int y);
bool checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
void generatePipes();
void updatePipes();
void generateCoins();
void drawCoins();
void CoinCollision();
void updateCoins();

// �ܵ��ṹ��
typedef struct {
    int x;
    int upY;
    int downY;
    bool passed; // �Ƿ��ѱ�С��ͨ��
} Pipe;

//��ҽṹ��
typedef struct{
	int x;
	int y;
	bool appear; //�Ƿ����ʾ 
} Coin; 

Pipe pipes[MAX_PIPES]; // ʹ������洢�ܵ�
int pipeCount = 0;     // ��ǰ�ܵ�����

Coin coins[COIN_COUNT]; //������� 
int coinCount = 0; //��ǰ������� 

int main(int argc, char** argv) 
{
	// ��ʼ�����������
    srand((unsigned int)time(NULL));
    //��ҳ�ʼ��
	for (int i = 0; i < COIN_COUNT ;i++)
	{
		coins[i].appear = false;	
	} 
    
	IMAGE menuImg; //����������Ϸ�˵������ı���
	IMAGE backImg; //����������Ϸ�����ı��� 
	IMAGE birdBkImg;
	IMAGE birdImg;
	IMAGE pipUpBkImg;
	IMAGE pipUpImg;
	IMAGE pipDownBkImg;
	IMAGE pipDownImg;
	IMAGE deadImg; //����������ײ���������ı��� 
	IMAGE deadBkImg;//����ǰ�� 
	
	// ��ȡͼ��ߴ�
	int deadX = 200;
	int deadY = 300;

	bool gamePause = false;   // ��¼��Ϸ�Ƿ���ͣ
	bool spaceKeyWasPressed = false; // ��¼�ո���ϴ�״̬
	bool pKeyWasPressed = false;     // ��¼P���ϴ�״̬
	
	char* deadStr = "��Enter�Խ���...";
	RECT rect_dead = {200,400,456,440};

	initgraph(900,1051); //�������ڲ���������̨ ʹ��EasyX��ǰ��Ҫ����Ŀ���ԡ��������������������-leasyx ����ᱨ��	
	loadimage(&menuImg,"./img/back1111.jpg");
    putimage(0, 0, &menuImg);
    
    //���Ų˵����� 
    mciSendString("open sound/menu1.mp3 alias bgm1",NULL,0,NULL);
	mciSendString("play bgm1 from 0 repeat",NULL,0,NULL); //ѭ������
	 
	showMenu();

	//��Ϸ��ʼ����
	loadimage(&backImg,"./img/back121.jpg"); //��ʾ��Ϸ����
	loadimage(&birdBkImg,"./img/bird1_1.jpg");//��ʾС��ǰ��ͼƬ  
	loadimage(&birdImg,"./img/bird1_2.jpg");//��ʾС��ͼƬ(����)
	loadimage(&pipUpBkImg,"./img/stone_up2.gif");
	loadimage(&pipUpImg,"./img/stone_up1.gif");
	loadimage(&pipDownBkImg,"./img/stone_down2.gif");
	loadimage(&pipDownImg,"./img/stone_down1.gif");
	loadimage(&aniImg,"./img/ani.bmp");
	
	//��������ǰҪ����Ŀ���ԡ��������������������-lwinmm 
	
	mciSendString("open sound/menu3.mp3 alias bgm2",NULL,0,NULL);
	mciSendString("play bgm2 from 0 repeat",NULL,0,NULL);
	printf("test.....\n");
	
	// ���ɳ�ʼ�ܵ�
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

			// �������йܵ�
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
		if(birdY > 800) //�����߽�
		{
			birdY = 0; 
		}
//		if(abs(birdX-pipUpX) < 7 && abs(birdY-pipUpY) < 7)
//		{
//			printf("С����ˮ�ܷ�����ײ\n");
//			gameOver = true;
//		} 

		//�����ײ���
		CoinCollision();
		
		 
		//�ܵ���ײ���
        for(int i = 0; i < pipeCount; i++) {
            if (checkCollision(birdX, birdY, birdWidth, birdHeight, pipes[i].x, pipes[i].upY, pipWidth, pipHeight) || 
                checkCollision(birdX, birdY, birdWidth, birdHeight, pipes[i].x, pipes[i].downY, pipWidth, pipHeight)) 
            {
                mciSendString("open sound/dead.mp3 alias dead",NULL,0,NULL);
                mciSendString("play dead from 0",NULL,0,NULL);
                printf("С����ˮ�ܷ�����ײ\n");
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
		
		//�ո����Ծ
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
		
		//P����ͣ
		if(GetAsyncKeyState('P') && !pKeyWasPressed)  
		{
			gamePause = !gamePause;
			pKeyWasPressed = true;
			if (gamePause)
            {
                printf("��Ϸ����ͣ\n"); 
            }
            else
            {
               printf("��Ϸ�Ѽ���\n");
            }
		}
		else if(!GetAsyncKeyState('P'))
		{
			pKeyWasPressed = false;
		}
		
	}
	printf("��Ϸ����\n");
	mciSendString("close bgm2", NULL, 0, NULL); 
	return 0;
}

void showMenu()
{
	char *str = "��ʼ��Ϸ";
	char *game_opt = "��Ϸ����";
	char *game_help = "��Ϸ����";
	char *game_exit = "��Ϸ�˳�";
	RECT rect_value = {250,200,500,250};
	RECT rect_game_opt = {250,250,500,300};
	RECT rect_game_help = {250,300,500,350};
	RECT rect_game_exit = {250,350,500,400};
//	rectangle(200,120,450,170);	//���ƿհ׾��� (left,top,right,bottom) 
	setfillcolor(BLUE);	//��������������Ҫ������ɫ 
	
	fillrectangle(250,200,500,250);//���������� 
	drawtext(str,&rect_value,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	fillrectangle(250,250,500,300);//x�᲻�䣬y������һ����Ļ����ϼ�50
	drawtext(game_opt,&rect_game_opt,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	fillrectangle(250,300,500,350);
	drawtext(game_help,&rect_game_help,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	fillrectangle(250,350,500,400); 
	drawtext(game_exit,&rect_game_exit,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	
	//��ʼ��ϷĬ��Ϊ��ɫ 
	setfillcolor(RED);
	fillrectangle(250,200,500,250);//���������� 
	drawtext(str,&rect_value,DT_CENTER|DT_SINGLELINE|DT_VCENTER);//����ᱻ���ǣ�����������дһ��
	 
	//��̬�˵�����ĵľ��Ǽ��̲�����β���������¼�
	//kbhit keybord hit  ͷ�ļ� conio.h 
	//GetAsyncKeyState ʵʱ���첽��ÿʱÿ�̶��ڲ�ѯ����ͬ��ͬ��ֻ����Ϣ���е��˲Ų�ѯ���ز�ѯ���̻���갴ť��˲ʱ״̬
	while(1)
	{
		Sleep(100); //��ֹ��ת���� 
		if(GetAsyncKeyState(VK_DOWN)) //���¼��������� 
		{
			mciSendString("open sound/select.mp3 alias select",NULL,0,NULL);
			mciSendString("play select from 0 ",NULL,0,NULL);
			switch(menuSelect)
			{
				//��������Ĭ��Ϊ��ʼ��Ϸ�����Ե�һ�ΰ���Ӧ����ת����Ϸ���� 
				case menu_game_start:
					setfillcolor(BLUE); //�ָ�Ϊ��ɫ  
					fillrectangle(250,200,500,250);//���������� 
					drawtext(str,&rect_value,DT_CENTER|DT_SINGLELINE|DT_VCENTER);//����ᱻ���ǣ�����������дһ�� 
						
					setfillcolor(RED);
					fillrectangle(250,250,500,300);
					drawtext(game_opt,&rect_game_opt,DT_CENTER|DT_SINGLELINE|DT_VCENTER);//����ᱻ���ǣ�����������дһ�� 
					menuSelect = menu_game_opt; 
					break;
					
				case menu_game_opt:
					setfillcolor(BLUE); //�ָ�Ϊ��ɫ  
					fillrectangle(250,250,500,300);//���������� 
					drawtext(game_opt,&rect_game_opt,DT_CENTER|DT_SINGLELINE|DT_VCENTER);//����ᱻ���ǣ�����������дһ�� 
							
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
					fillrectangle(250,200,500,250);//���������� 
					drawtext(str,&rect_value,DT_CENTER|DT_SINGLELINE|DT_VCENTER);//����ᱻ���ǣ�����������дһ�� 
					menuSelect = menu_game_start;
					break;	
			}
		}
		if(GetAsyncKeyState(VK_UP)) //�߼���VK_Down��࣬���±��Ͼ����� 
		{	
			mciSendString("open sound/select.mp3 alias select",NULL,0,NULL);
			mciSendString("play select from 0 ",NULL,0,NULL);
			switch(menuSelect)
			{
				//��������Ĭ��Ϊ��ʼ��Ϸ�����Ե�һ�ΰ���Ӧ����ת����Ϸ���� 
				case menu_game_start:
					setfillcolor(BLUE); //�ָ�Ϊ��ɫ  
					fillrectangle(250,200,500,250);//���������� 
					drawtext(str,&rect_value,DT_CENTER|DT_SINGLELINE|DT_VCENTER);//����ᱻ���ǣ�����������дһ�� 
						
					setfillcolor(RED);
					fillrectangle(250,350,500,400); 
					drawtext(game_exit,&rect_game_exit,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
					menuSelect = menu_game_exit;
					break;
					
				case menu_game_opt:
					setfillcolor(BLUE); //�ָ�Ϊ��ɫ  
					fillrectangle(250,250,500,300);//���������� 
					drawtext(game_opt,&rect_game_opt,DT_CENTER|DT_SINGLELINE|DT_VCENTER);//����ᱻ���ǣ�����������дһ�� 
							
					setfillcolor(RED);
					fillrectangle(250,200,500,250);//���������� 
					drawtext(str,&rect_value,DT_CENTER|DT_SINGLELINE|DT_VCENTER);//����ᱻ���ǣ�����������дһ�� 
					menuSelect = menu_game_start; 	
					break;	
					
				case menu_game_help:
					setfillcolor(BLUE);
					fillrectangle(250,300,500,350);
					drawtext(game_help,&rect_game_help,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
					
					setfillcolor(RED);
					fillrectangle(250,250,500,300);//���������� 
					drawtext(game_opt,&rect_game_opt,DT_CENTER|DT_SINGLELINE|DT_VCENTER);//����ᱻ���ǣ�����������дһ�� 
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
				
					return;	 //showMenu()��������
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
	static int index = 0; //��̬����������ÿ��ˢ�¶�����ֵΪ0 
	if(index>=4) index = 0;
	putimage(x,y,COIN_WIDTH,COIN_HEIGHT,&aniImg,(0+index)*COIN_WIDTH,9*COIN_HEIGHT,SRCAND);
	putimage(x,y,COIN_WIDTH,COIN_HEIGHT,&aniImg,(0+index)*COIN_WIDTH,8*COIN_HEIGHT,SRCPAINT);
	index++;
	
}

// ������ײ��⺯��
bool checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
    // ������������Ƿ��ص�
    if (x1 + w1 >= x2 &&     // �ұ�Ե�������Ե
        x1 <= x2 + w2 &&     // ���Ե�����ұ�Ե
        y1 + h1 >= y2 &&     // �±�Ե�����ϱ�Ե
        y1 <= y2 + h2) {     // �ϱ�Ե�����±�Ե
        return true;
    }
    return false;
}

void generatePipes()
{
	pipeCount = 3; //��ʼ�������Թܵ�
	for(int i = 0; i < pipeCount; i++) 
	{
        int gapPosition = (rand() % 500) + 200;  
        
        pipes[i].x = 900 + i * 400; // ÿ�Թܵ����400����
        pipes[i].upY = gapPosition - pipHeight; //�Ϸ��Ĺܵ�������Ϊ�������������ģ���Ϊ�Ϲܵ��߶�Ҫ�Ѹ��������Ż��������  
        pipes[i].downY = gapPosition + PIPE_GAP;//�·��Ĺܵ���PIPE_GAP��Ϊ�˺��Ϲܵ�����һ�οռ䣬��������ܵ�����һ���ˣ�С�����ȥ 
        pipes[i].passed = false; 
	}
}

// ���¹ܵ�λ��
void updatePipes() 
{
    for(int i = 0; i < pipeCount; i++) 
	{
        pipes[i].x -= 3;
        
        // ����ܵ��Ƴ���Ļ��࣬���õ��Ҳ�
        if(pipes[i].x + pipWidth < 0) 
		{
            int gapPosition = (rand() % 500) + 200; //�ܵ����Ϊ600��Ӧ��ά����200-500�����
            pipes[i].x = 900;
            pipes[i].upY = gapPosition - pipHeight;
            pipes[i].downY = gapPosition + PIPE_GAP;
            pipes[i].passed = false;
        }
    }
}

//���Ľ��λ�� 
void updateCoins()
{
	for(int i = 0; i < COIN_COUNT; i++) {
        if(coins[i].appear) {
            coins[i].x -= COIN_SPEED; // ��������ƶ�
    
            // �������Ƴ���Ļ��࣬������
            if(coins[i].x + COIN_WIDTH < 0) {
                coins[i].appear = false;
                coinCount--;
            }
        }
    }
}

//���ɽ�� 
void generateCoins()
{
	if(rand() % 100 < 20 && coinCount < COIN_COUNT)//��������Ƿ������½�ң�20%�ļ��������½��
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

//���ƽ��
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


//�������ײ
void CoinCollision()
{
	for(int i = 0; i< COIN_COUNT ; i++)
	{
		if(coins[i].appear && checkCollision(birdX,birdY,birdWidth,birdHeight,coins[i].x,coins[i].y,COIN_WIDTH,COIN_HEIGHT))
		{
			// ��ײ����ң�������Ч�����ӷ�����
            mciSendString("open sound/coins.mp3 alias coin", NULL, 0, NULL);
            mciSendString("play coin from 0", NULL, 0, NULL);
            coins[i].appear = false;
            coinCount--;
            // ����������ӷ���	
		}	
	}	
} 
