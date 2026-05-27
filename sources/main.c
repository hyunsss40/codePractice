#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include "sprite.h"

#define WIDTH 120
#define HEIGHT 30

#define BLACK 0
#define DARK_BLUE (FOREGROUND_BLUE)
#define DARK_GREEN (FOREGROUND_GREEN)
#define DARK_CYAN (FOREGROUND_GREEN | FOREGROUND_BLUE)
#define DARK_RED (FOREGROUND_RED)
#define DARK_MAGENTA (FOREGROUND_RED | FOREGROUND_BLUE)
#define DARK_YELLOW (FOREGROUND_RED | FOREGROUND_GREEN)
#define GREY (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define DARK_GREY (FOREGROUND_INTENSITY)
#define BLUE (FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define GREEN (FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define CYAN (FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define RED (FOREGROUND_RED | FOREGROUND_INTENSITY)
#define MAGENTA (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define YELLOW (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define WHITE (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)

#define B_BLACK 0
#define B_DARK_BLUE (BACKGROUND_BLUE)
#define B_DARK_GREEN (BACKGROUND_GREEN)
#define B_DARK_CYAN (BACKGROUND_GREEN | BACKGROUND_BLUE)
#define B_DARK_RED (BACKGROUND_RED)
#define B_DARK_MAGENTA (BACKGROUND_RED | BACKGROUND_BLUE)
#define B_DARK_YELLOW (BACKGROUND_RED | BACKGROUND_GREEN)
#define B_GREY (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE)
#define B_DARK_GREY (BACKGROUND_INTENSITY)
#define B_BLUE (BACKGROUND_BLUE | BACKGROUND_INTENSITY)
#define B_GREEN (BACKGROUND_GREEN | BACKGROUND_INTENSITY)
#define B_CYAN (BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY)
#define B_RED (BACKGROUND_RED | BACKGROUND_INTENSITY)
#define B_MAGENTA (BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY)
#define B_YELLOW (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY)
#define B_WHITE (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY)

typedef struct {
	int x;
	int y;
	int index;
	int sprite;
	int leg, step;
	int direction;
}object;

DWORD NOTHING = 0;//임의 저장소
HANDLE buffer[2];//임시 화면
int bufferIndex = 0;//화면 번호(번갈아가면서 작성)

void initBuffer();//버퍼 초기화
void printBuffer(int x, int y, const char* string, WORD color, int width);
void flip();//버퍼 교체
void drawCharacter(object obj, int x, int y, int height);
void drawBackground(WORD color,int x,int y, int height);
void drawSubway();
void drawMap();
void drawChair(int x, int y, WORD color, int height);
void ObjMove(object* Obj);
void drawCloud(int x, int y, int index);
void drawMountain(int x, int y);
void drawBuilding(int x, int y);
void drawDoor(int x, int y);
void titleBackground();

int main() {
	initBuffer();
	object MAP;
	MAP.index = 0;
	MAP.y = 0;
	object CHARA = { .x = 10, .y = 20, .leg = 0, .step = 1 };

	while (1) {
		ObjMove(&MAP);
		if (MAP.index == 0) {
			if (GetAsyncKeyState(VK_RETURN)) MAP.index = MAP.y + 1;
			if (MAP.y < 0) MAP.y = 0;
			if (MAP.y > 2) MAP.y = 2;
			titleBackground();

			drawSubway();
			for (int i = 0; i < 5; i++) {
				printBuffer(100, 9 + i, TitleSelect[i], B_WHITE, 15);
			}
			printBuffer(103, 10 + MAP.y, "▶", B_WHITE, 1);
			Sleep(60);
		}

		else if (MAP.index == 1) {
			drawBackground(GREY, 0, 0, HEIGHT);
			drawMap();
			drawDoor(52, 18);
			drawChair(15, 21, MAGENTA, 6);
			ObjMove(&CHARA);
			drawCharacter(CHARA, CHARA.x, 20, 6);

			Sleep(60);
		}
		else if (MAP.index == 3) { return 0; }
		flip();
	}
	return 0;
}


void initBuffer() {
	buffer[0] = CreateConsoleScreenBuffer(GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	buffer[1] = CreateConsoleScreenBuffer(GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	//CONSOLE_CURSOR_INFO cursorInfo = { 1, FALSE };//커서 안보이게 하기
	//SetConsoleCursorInfo(buffer[0], &cursorInfo);
	//SetConsoleCursorInfo(buffer[1], &cursorInfo);
}
void printBuffer(int x, int y, const char* string, WORD color, int width) {
	COORD position = { x, y };
	WriteConsoleOutputCharacterA(buffer[bufferIndex], string, (DWORD)strlen(string), position, &NOTHING);
	FillConsoleOutputAttribute(buffer[bufferIndex], color, (DWORD)width, position, &NOTHING);
}
void flip() {
	SetConsoleActiveScreenBuffer(buffer[bufferIndex]);
	bufferIndex = !bufferIndex;
	COORD coordRec = { 0, 0 };
	FillConsoleOutputCharacterA(buffer[bufferIndex], ' ', WIDTH * HEIGHT, coordRec, &NOTHING);
	FillConsoleOutputAttribute(buffer[bufferIndex], WHITE, WIDTH * HEIGHT, coordRec, &NOTHING);
}
void drawCharacter(object obj, int x, int y, int height) {
	if (obj.direction == 0) {
		printBuffer(x, y, SpriteCharacter[obj.direction][0], BLACK | B_GREY, 8);
		printBuffer(x + 1, y + 1, SpriteCharacter[obj.direction][1], BLACK | B_WHITE, 7);
		printBuffer(x + 7, y + 1, "█", BLACK, 1);//투과 현상 예방
		printBuffer(x + 1, y + 2, SpriteCharacter[obj.direction][2], BLACK | B_WHITE, 7);
		printBuffer(x + 1, y + 3, SpriteCharacter[obj.direction][3], BLACK | B_WHITE, 6);
		printBuffer(x + 1, y + 4, SpriteCharacter[obj.direction][4], BLACK, 5);
		printBuffer(x + 2, y + 5, SpriteCharacter[obj.direction][5], BLACK, 4);
		printBuffer(x, y + height, SpriteCharacter_Leg[obj.leg][obj.step], BLACK | B_DARK_GREY, 6);
	}
	else {
		printBuffer(x, y, SpriteCharacter[obj.direction][0], BLACK | B_GREY, 8);
		printBuffer(x, y + 1, SpriteCharacter[obj.direction][1], BLACK | B_WHITE, 7);
		printBuffer(x, y + 2, SpriteCharacter[obj.direction][2], BLACK | B_WHITE, 7);
		printBuffer(x + 1, y + 3, SpriteCharacter[obj.direction][3], BLACK | B_WHITE, 6);
		printBuffer(x + 2, y + 4, SpriteCharacter[obj.direction][4], BLACK, 5);
		printBuffer(x + 2, y + 5, SpriteCharacter[obj.direction][5], BLACK, 4);
		printBuffer(x, y + height, SpriteCharacter_Leg[obj.leg][obj.step], BLACK | B_DARK_GREY, 6);
	}
}
void drawSubway() {
	printBuffer(4, 21, subway[0], GREY | B_DARK_GREY, 110);
	printBuffer(4, 22, subway[1], GREY | B_DARK_GREY, 110);
	for (int i = 0; i < 7; i++) {
		printBuffer(4, 23 + i, subway[2 + i], GREY, 110);
	}
	printBuffer(4, 23, "▗", GREY | B_DARK_GREY, 1);
}
void drawBackground(WORD color, int x, int y, int height) {
	for (y; y < height; y++) {
		printBuffer(x%WIDTH, y, Background, color, WIDTH);

	}
}
void drawChair(int x, int y, WORD color, int height) {
	for (int i = 0; i < 5; i++) {
		printBuffer(x, y + i, SpriteChair[i], color, height);
	}
}
void drawMap() {
	for (int i = 1; i < 8; i++)
		printBuffer(0, i, Background, DARK_GREY, WIDTH);
	for (int i = 8; i < 11; i++)
		printBuffer(0, i, Background, BLACK, WIDTH);
	printBuffer(35, 9, "안녕하세용", WHITE | B_BLACK, 10);
	for (int y = 0; y < 3; y++) {
		printBuffer(0, 26 + y, Background, DARK_GREY, WIDTH);
	}
	printBuffer(0, 0, Background, BLACK, WIDTH);
	printBuffer(0, HEIGHT - 1, Background, BLACK, WIDTH);
	for (int y = 0; y < HEIGHT; y++) {
		printBuffer(0, y, "█", BLACK, 2);
		printBuffer(WIDTH - 1, y, "█", BLACK, 2);
	}
}
void ObjMove(object* Obj) {
	if (GetAsyncKeyState('D') & 0x8000) { Obj->x++; Obj->direction = 0; Obj->leg = 0; Obj->step = !Obj->step; }
	if (GetAsyncKeyState('A') & 0x8000) { Obj->x--; Obj->direction = 1; Obj->leg = 1; Obj->step = !Obj->step; }
	if (GetAsyncKeyState('S') & 0x8000) Obj->y++;
	if (GetAsyncKeyState('W') & 0x8000) Obj->y--;
	if (Obj->x < 2) Obj->x = 2;
	if (Obj->y < 0) Obj->y = 0;
	if (Obj->x > WIDTH - 9) Obj->x = WIDTH - 9;
	if (Obj->y > HEIGHT - 1) Obj->y = HEIGHT - 1;
}
void drawCloud(int x, int y, int index) {
	if (index == 0) {
		printBuffer(x + 7, y, SpriteCloud[index][0], WHITE | B_BLUE, 13);
		printBuffer(x + 7, y + 1, SpriteCloud[index][1], WHITE | B_BLUE, 14);
		printBuffer(x, y + 2, SpriteCloud[index][2], WHITE | B_BLUE, 22);
	}
	if (index == 1) {
		printBuffer(x + 4, y, SpriteCloud[index][0], WHITE | B_BLUE, 13);
		printBuffer(x, y + 1, SpriteCloud[index][1], WHITE | B_BLUE, 17);
		printBuffer(x + 2, y + 2, SpriteCloud[index][2], WHITE | B_BLUE, 14);
	}
	if (index == 2) {
		printBuffer(x + 3, y, SpriteCloud[index][0], WHITE | B_BLUE, 7);
		printBuffer(x+3, y + 1, SpriteCloud[index][1], WHITE | B_BLUE, 7);
	}
}
void drawMountain(int x, int y) {
		printBuffer(x+17, y, SpriteMountain[0], DARK_GREEN|B_BLUE, 2);
		printBuffer(x+15, y+1, SpriteMountain[1], DARK_GREEN | B_BLUE, 6);
		printBuffer(x+13, y+2, SpriteMountain[2], DARK_GREEN | B_BLUE, 10);
		printBuffer(x + 10, y + 3, SpriteMountain[3], DARK_GREEN | B_BLUE, 16);
		printBuffer(x + 7, y + 4, SpriteMountain[4], DARK_GREEN | B_BLUE, 22);
		printBuffer(x + 3, y + 5, SpriteMountain[5], DARK_GREEN | B_BLUE, 30);
		printBuffer(x, y + 6, SpriteMountain[6], DARK_GREEN | B_BLUE, 36);
}
void drawBuilding(int x, int y) {
	printBuffer(x, y, SpriteBuilding[0], DARK_GREY, 10);
	for(int i=1; i<6; i++) printBuffer(x, y+i, SpriteBuilding[1], DARK_GREY|B_BLUE, 10);
}
void drawDoor(int x, int y) {
	printBuffer(x, y, SpriteDoor[0][0], DARK_GREY | B_DARK_GREY, 8);
	printBuffer(x + 8, y, SpriteDoor[1][0], DARK_GREY | B_DARK_GREY, 8);
	for (int i = 1; i < 3; i++) {
		printBuffer(x, y + i, SpriteDoor[0][i], GREY | B_DARK_GREY, 8);
		printBuffer(x + 8, y + i, SpriteDoor[1][i], GREY | B_DARK_GREY, 8);
	}
	for (int i = 3; i < 8; i++) {
		printBuffer(x, y + i, SpriteDoor[0][i], GREY | B_DARK_GREY, 8);
		printBuffer(x + 8, y + i, SpriteDoor[1][i], GREY | B_DARK_GREY, 8);
	}
}
void titleBackground() {
	drawBackground(BLUE, 0, 0, HEIGHT);
	drawMountain(0, 8);
	drawMountain(24, 9);
	drawMountain(80, 8);
	drawBackground(DARK_GREEN,0, 15, HEIGHT);
	drawBuilding(20, 14);
	drawBuilding(27, 15);
	drawBuilding(40, 14);
	drawBuilding(55, 17);
	drawBuilding(67, 14);
	drawBuilding(90, 15);
	drawBackground(DARK_GREY,0, 20, HEIGHT);
	drawCloud(10, 3, 1);
	drawCloud(75, 3, 0);
	drawCloud(55, 2, 2);
	}