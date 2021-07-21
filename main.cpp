#include "easyx.h"
#include "ctime"
#include "iostream"
#include "conio.h"
#include "cmath"
#include "sstream"
#include "wingdi.h"
using namespace std;
MOUSEMSG M;																									//定义一个鼠标信息
int my_map[6][6];																							//设定地图
IMAGE image[40];																							//定义方块图片
IMAGE tu;																									//定义一个图标
int now_score = 0;																							//当前得分
int history_max_score;																						//历史最高得分
bool over = false;																							//标记是否结束游戏
int mode;																									//标记模式
/********************************************清空数组***************************************************/	//正常
void Empty_map() {
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			my_map[x][y] = 0;
		}
	}
}
/***************************************检查是否还有空位************************************************/		//正常
bool check_null() {
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			if (!my_map[x][y])
				return true;
	return false;
}
/****************************************检查是否能合并*************************************************/	//正常
bool check_merge(int x1, int y1, int x2, int y2) {
	if (x1 == x2) {																							//按列查找
		for (int y = y1 + 1; y < y2; y++)
			if (my_map[x1][y])
				return false;
		return true;
	}
	if (y1 == y2) {																							//按行查找
		for (int x = x1 + 1; x < x2; x++)
			if (my_map[x][y1])
				return false;
		return true;
	}
}
/****************************************检查游戏是否结束************************************************/	//正常
bool check_overgame() {
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (!my_map[x][y])																				//当前位置为空
				return false;
			if (my_map[x][y] == my_map[x][y + 1])															//右边一位等于当前（可以合并）
				return false;
			if (my_map[x][y] == my_map[x + 1][y])															//下边一位等于当前（可以合并）
				return false;
		}
	}
	return true;
}
/*************************************对存档文件进行存取操作*********************************************/	//正常
void write() {
	if (now_score > history_max_score)
		cout << now_score;
	else
		cout << history_max_score;
}
/**************************************设置文字样式和颜色************************************************/	//正常
void settext(int height, int weight, UINT color) {
	settextstyle(height, 0, _T("Arial"), 0, 0, weight, false, false, false, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH);
	settextcolor(color);
}
/*****************************************生成图片方块***************************************************/	//正常
void CreateImage(IMAGE* img, LPCTSTR num, COLORREF imgColor, int fontSize, COLORREF fontColor) {
	SetWorkingImage(img);																					//对img进行绘图
	setbkmode(TRANSPARENT);																					//设置背景样式为透明
	setbkcolor(0x9eaebb);																					//用十六进制表示背景颜色
	settext(fontSize, 1000, fontColor);																		//设置形式
	setfillcolor(imgColor);																					//设置填充颜色
	settextcolor(fontColor);																				//设置文字颜色
	cleardevice();																							//清除屏幕并将点移至(0,0)处
	solidroundrect(0, 0, img->getwidth() - 1, img->getheight() - 1, 10, 10);								//绘画圆角矩形
	RECT r = { 0,0,img->getwidth() - 1,img->getheight() - 1 };												//画出一个矩形区域
	drawtext(num, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);												//在指定位置按照指定格式输出指定字符串
}
/*****************************************制作图片缓存**************************************************/	//正常
void Load() {
	IMAGE temp(90, 90);
	CreateImage(&temp, _T(""), 0xb5becc, 72, WHITE);
	image[0] = temp;
	CreateImage(&temp, _T("2"), 0xdbe6ee, 72, 0x707b83);
	image[1] = temp;
	CreateImage(&temp, _T("4"), 0xc7e1ed, 72, 0x707b83);
	image[2] = temp;
	CreateImage(&temp, _T("8"), 0x78b2f4, 72, WHITE);
	image[3] = temp;
	CreateImage(&temp, _T("16"), 0x538ded, 72, WHITE);
	image[4] = temp;
	CreateImage(&temp, _T("32"), 0x607df6, 72, WHITE);
	image[5] = temp;
	CreateImage(&temp, _T("64"), 0x3958e9, 72, WHITE);
	image[6] = temp;
	CreateImage(&temp, _T("128"), 0x6bd9f5, 56, WHITE);
	image[7] = temp;
	CreateImage(&temp, _T("256"), 0x4bd0f2, 56, WHITE);
	image[8] = temp;
	CreateImage(&temp, _T("512"), 0x2ac0e4, 56, WHITE);
	image[9] = temp;
	CreateImage(&temp, _T("1024"), 0x13b8e3, 40, WHITE);
	image[10] = temp;
	CreateImage(&temp, _T("2048"), 0x00c5eb, 40, WHITE);
	image[11] = temp;
	CreateImage(&temp, _T("4096"), 0x3958e9, 40, WHITE);
	image[12] = temp;
	CreateImage(&temp, _T("8192"), 0x3958e9, 40, WHITE);
	image[13] = temp;
	SetWorkingImage(NULL);
}
/**********************************在指定矩形区域内居中输出字符串****************************************/		//正常
void printtext(LPCTSTR s, int left, int top, int right, int width)
{
	RECT r = { left, top, right, width };																	//确定一个圆角矩形参数
	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);												//在指定区域内以指定格式输出字符串
}
/*******************************************给地图赋值*************************************************/		//正常
void set_map(int k) {
	if (!check_null())
		return;
	int x, y, a;
	do {
		x = rand() % k;																						//随机产生一个x坐标
		y = rand() % k;																						//随机产生一个y坐标
	} while (my_map[x][y]);																					//如果原来位置有数字，则重新生成
	do {
		a = 1 + rand() % 4;																					//随机产生一个偶数填入相应的位置
	} while (a % 2);
	my_map[x][y] = a;
}
/********************************************输出地图**************************************************/		//正常
void get_map(int k) {
	for (int x = 0; x < k; x++) {
		for (int y = 0; y < k; y++) {
			int p = (log(my_map[x][y]) / log(2));															//获得数值是2的几次幂
			putimage(25 + y * 100, 225 + x * 100, &image[p]);
		}
	}
}
/************************************在娱乐模式下对地图进行操作*****************************************/		//正常 跨数字合并
void move_entertainment(int k) {
	int sign = 0;																							//用于标记是否还能操作
	if (k == 1) {																							//向上
		for (int y = 0; y < 4; y++) {																		//固定列
			for (int x = 0; x < 4; x++) {																	//行从上到下寻找有数值的位置
				bool merge_image = false;																	//这一行是否合并过
				for (int z = x + 1; z < 4; z++) {															//往下寻找有数字的位置
					if (my_map[z][y]) {																		//找到了
						if (my_map[x][y] && !merge_image && my_map[z][y] == my_map[x][y]) {					//如果原来的位置有数
							merge_image = true;																//表示已经合成过
							my_map[x][y] += my_map[z][y];
							my_map[z][y] = 0;
							sign = 1;
							now_score += my_map[x][y];														//更新分数
							break;
						}
						if (!my_map[x][y]) {																//原来位置没有数
							my_map[x][y] = my_map[z][y];
							my_map[z][y] = 0;
							sign = 1;
						}
					}
				}
			}
		}
	}
	else if (k == 2) {																						//向左
		for (int x = 0; x < 4; x++) {																		//固定行
			for (int y = 0; y < 4; y++) {																	//列从左到右寻找
				bool merge_image = false;																	//标记是否合成过
				for (int z = y + 1; z < 4; z++) {															//往右寻找有数字的位置
					if (my_map[x][z]) {																		//找到了
						if (my_map[x][y] && !merge_image && my_map[x][z] == my_map[x][y]) {					//如果原来的位置有数
							merge_image = true;																//表示已经合成过
							my_map[x][y] += my_map[x][z];
							my_map[x][z] = 0;
							sign = 1;
							now_score += my_map[x][y];														//更新分数
							break;
						}
						if (!my_map[x][y]) {																//原来位置没有数
							my_map[x][y] = my_map[x][z];
							my_map[x][z] = 0;
							sign = 1;
						}
					}
				}
			}
		}
	}
	else if (k == 3) {																						//向右
		for (int x = 0; x < 4; x++) {																		//固定行
			for (int y = 4 - 1; y >= 0; y--) {																//列从右到左寻找
				bool merge_image = false;																	//标记是否合成过
				for (int z = y - 1; z >= 0; z--) {															//往右寻找有数字的位置
					if (my_map[x][z]) {																		//找到了
						if (my_map[x][y] && !merge_image && my_map[x][z] == my_map[x][y]) {					//如果原来的位置有数
							merge_image = true;																//表示已经合成过
							my_map[x][y] += my_map[x][z];
							my_map[x][z] = 0;
							sign = 1;
							now_score += my_map[x][y];														//更新分数
							break;
						}
						if (!my_map[x][y]) {																//原来位置没有数
							my_map[x][y] = my_map[x][z];
							my_map[x][z] = 0;
							sign = 1;
						}
					}
				}
			}
		}
	}
	else {																									//向下
		for (int y = 0; y < 4; y++) {																		//固定列
			for (int x = 4 - 1; x >= 0; x--) {																//行从下到上寻找有数值的位置
				bool merge_image = false;																	//这一行是否合并过
				for (int z = x - 1; z >= 0; z--) {															//往上寻找有数字的位置
					if (my_map[z][y]) {																		//找到了
						if (my_map[x][y] && !merge_image && my_map[z][y] == my_map[x][y]) {					//如果原来的位置有数
							merge_image = true;																//表示已经合成过
							my_map[x][y] += my_map[z][y];
							my_map[z][y] = 0;
							sign = 1;
							now_score += my_map[x][y];														//更新分数
							break;
						}
						if (!my_map[x][y]) {																//原来位置没有数
							my_map[x][y] = my_map[z][y];
							my_map[z][y] = 0;
							sign = 1;
						}
					}
				}
			}
		}
	}
	if (check_overgame())
		over = true;
}
/************************************在正常模式下对地图进行操作*****************************************/		//正常
void move_normal(int k) {
	int sign = 0;																							//用于标记是否还能操作
	if (k == 1) {																							//向上
		for (int y = 0; y < 4; y++) {																		//固定列
			for (int x = 0; x < 4; x++) {																	//行从上到下寻找有数值的位置
				bool merge_image = false;																	//这一行是否合并过
				for (int z = x + 1; z < 4; z++) {															//往下寻找有数字的位置
					if (my_map[z][y]) {																		//找到了
						if (my_map[x][y] && !merge_image &&
							my_map[z][y] == my_map[x][y] && check_merge(x, y, z, y)) {						//如果原来的位置有数
							merge_image = true;																//表示已经合成过
							my_map[x][y] += my_map[z][y];
							my_map[z][y] = 0;
							sign = 1;
							now_score += my_map[x][y];														//更新分数
							break;
						}
						if (!my_map[x][y]) {																//原来位置没有数
							my_map[x][y] = my_map[z][y];
							my_map[z][y] = 0;
							sign = 1;
						}
					}
				}
			}
		}
	}
	else if (k == 2) {																						//向左
		for (int x = 0; x < 4; x++) {																		//固定行
			for (int y = 0; y < 4; y++) {																	//列从左到右寻找
				bool merge_image = false;																	//标记是否合成过
				for (int z = y + 1; z < 4; z++) {															//往右寻找有数字的位置
					if (my_map[x][z]) {																		//找到了
						if (my_map[x][y] && !merge_image &&
							my_map[x][z] == my_map[x][y] && check_merge(x, y, x, z)) {						//如果原来的位置有数
							merge_image = true;																//表示已经合成过
							my_map[x][y] += my_map[x][z];
							my_map[x][z] = 0;
							sign = 1;
							now_score += my_map[x][y];														//更新分数
							break;
						}
						if (!my_map[x][y]) {																//原来位置没有数
							my_map[x][y] = my_map[x][z];
							my_map[x][z] = 0;
							sign = 1;
						}
					}
				}
			}
		}
	}
	else if (k == 3) {																						//向右
		for (int x = 0; x < 4; x++) {																		//固定行
			for (int y = 4 - 1; y >= 0; y--) {																//列从右到左寻找
				bool merge_image = false;																	//标记是否合成过
				for (int z = y - 1; z >= 0; z--) {															//往右寻找有数字的位置
					if (my_map[x][z]) {																		//找到了
						if (my_map[x][y] && !merge_image &&
							my_map[x][z] == my_map[x][y] && check_merge(x, z, x, y)) {						//如果原来的位置有数
							merge_image = true;																//表示已经合成过
							my_map[x][y] += my_map[x][z];
							my_map[x][z] = 0;
							sign = 1;
							now_score += my_map[x][y];														//更新分数
							break;
						}
						if (!my_map[x][y]) {																//原来位置没有数
							my_map[x][y] = my_map[x][z];
							my_map[x][z] = 0;
							sign = 1;
						}
					}
				}
			}
		}
	}
	else {																									//向下
		for (int y = 0; y < 4; y++) {																		//固定列
			for (int x = 4 - 1; x >= 0; x--) {																//行从下到上寻找有数值的位置
				bool merge_image = false;																	//这一行是否合并过
				for (int z = x - 1; z >= 0; z--) {															//往上寻找有数字的位置
					if (my_map[z][y]) {																		//找到了
						if (my_map[x][y] && !merge_image &&
							my_map[z][y] == my_map[x][y] && check_merge(z, y, x, y)) {						//如果原来的位置有数
							merge_image = true;																//表示已经合成过
							my_map[x][y] += my_map[z][y];
							my_map[z][y] = 0;
							sign = 1;
							now_score += my_map[x][y];														//更新分数
							break;
						}
						if (!my_map[x][y]) {																//原来位置没有数
							my_map[x][y] = my_map[z][y];
							my_map[z][y] = 0;
							sign = 1;
						}
					}
				}
			}
		}
	}
	if (check_overgame() && !sign)
		over = true;
}
/*******************************************获取键盘输入***********************************************/		//正常
void set_keyboard() {
	char k;
	k = _getch();
	if (k == 'w' || k == 'W') {																				//向上
		if (mode == 1)																						//正常模式
			move_normal(1);
		else																								//娱乐模式
			move_entertainment(1);
	}
	else if (k == 'a' || k == 'A') {																		//向左
		if (mode == 1)																						//正常模式
			move_normal(2);
		else																								//娱乐模式
			move_entertainment(2);
	}
	else if (k == 'd' || k == 'D') {																		//向右
		if (mode == 1)																						//正常模式
			move_normal(3);
		else																								//娱乐模式
			move_entertainment(3);
	}
	else if (k == 's' || k == 'S') {																		//向下
		if (mode == 1)																						//正常模式
			move_normal(4);
		else																								//娱乐模式
			move_entertainment(4);
	}
}
/***********************************************菜单**************************************************/		//正常
void menu() {
	settextcolor(WHITE);																					//设置字体颜色为白色
	settextstyle(50, 0, "宋体");																				//设置字体样式为宋体
	outtextxy(125, 150, "游戏模式");
	outtextxy(125, 300, "正常模式");
	outtextxy(125, 450, "娱乐模式");
}
/*****************************************处理鼠标点击的信息*******************************************/		//正常
int get_mousemsg() {
	int sign;
	do {
		sign = 0;																							//初始化提示
		do {
			M = GetMouseMsg();																				//获取鼠标信息
		} while (!M.mkLButton);
		if (M.x >= 125 && M.x <= 315) {
			if (M.y >= 300 && M.y < 450)																	//正常模式
				return 1;
			else if (M.y >= 450 && M.y < 600)																//娱乐模式
				return 2;
			else
				sign = 1;																					//鼠标点击的位置不在规定的位置内
		}
		else
			sign = 1;																						//鼠标点击的位置不在规定的位置内
	} while (sign);
}
/********************************************选择游戏模式*********************************************/		//正常
void set_mode() {
	menu();																									//输出菜单进行选择
	mode = get_mousemsg();																					//获得游戏的难度
	cleardevice();																							//清除屏幕
}
/**********************************************开始游戏***********************************************/		//正常
void play() {
	set_keyboard();																							//从键盘获取输入
	set_map(4);																								//给地图赋值
	get_map(4);																								//输出地图
}
/********************************************输出一个整数*********************************************/		//正常
void outintxy(int x, int dx, int y, int dy, const char* format, int n) {
	char tmp[11];
	sprintf(tmp, format, n);
	solidroundrect(x, y, x + 100, y + 100, 50, 50);															//画一个圆角的矩形
	settextstyle(77, 0, _T("Arial"));																		//设置文字的格式
	settextcolor(WHITE);																					//设置文字的颜色
	outtextxy(x + dx, y + dy, tmp);																			//输出分数
}
/*********************************************游戏页面************************************************/		//正常
void page() {
	//绘制游戏图标
	putimage(10, 30, &tu);
	// 绘制当前分数
	solidroundrect(112, 30, 264, 119, 10, 10);																//画圆角矩形
	settext(28, 800, 0xdbe6ee);																				//设置文字风格
	printtext(_T("SCORE"), 112, 40, 264, 69);																//输出分数提示
	settext(44, 800, WHITE);																				//设置文字风格
	char tmp[11];
	sprintf(tmp, "%d", now_score);																			//数字和字符串的转换
	printtext(tmp, 112, 70, 264, 114);																		//输出当前分数

	// 绘制最高分数
	solidroundrect(275, 30, 427, 119, 10, 10);																//画圆角矩形
	settext(28, 800, 0xdbe6ee);																				//设置文字风格
	printtext(_T("BEST"), 275, 40, 427, 69);																//输出历史最高分数提示
	settext(44, 800, WHITE);																				//设置文字风格
	sprintf(tmp, "%d", history_max_score);																	//转换
	printtext(tmp, 275, 70, 427, 114);										 								//输出历史最高分数
}
/*******************************************游戏结束页面**********************************************/		//正常
void over_game() {
	// Game Over
	settext(60, 1000, 0x696f78);																			//设置文字格式
	printtext(_T("Game Over!"), 0, 0, 439, 199);															//输出文字
	// ReStart
	setfillcolor(0x9dadba);																					//设置填充颜色
	solidroundrect(120, 310, 319, 389, 10, 10);																//画圆角矩形
	settext(36, 1000, WHITE);																				//设置文字格式
	printtext(_T("ReStart"), 120, 310, 319, 389);															//输出文字
	// Exit
	solidroundrect(120, 460, 319, 539, 10, 10);																//画圆角矩形
	printtext(_T("Exit"), 120, 460, 319, 539);																//输出文字
	FlushBatchDraw();																						//执行未完成的绘制任务
	FlushMouseMsgBuffer();																					//清空鼠标消息缓存
	//获取鼠标信息
	int sign;
	do {
		sign = 0;																							//初始化提示
		do {
			M = GetMouseMsg();																				//获取鼠标信息
		} while (!M.mkLButton);
		if (M.x >= 120 && M.x <= 319) {
			if (M.y >= 310 && M.y < 389)																	//从新开始
				over = true;
			else if (M.y >= 460 && M.y < 539)																//退出游戏
				over = false;
			else
				sign = 1;																					//鼠标点击的位置不在规定的位置内
		}
		else
			sign = 1;																						//鼠标点击的位置不在规定的位置内
	} while (sign);

	over = false;
}
/**********************************************主函数*************************************************/		//正常
int main() {
	srand((int)time(NULL));																					//设置一个随机数种子
	initgraph(440, 650);																					//初始化窗口 颜色默认黑色
	freopen("max.in", "r", stdin);																			//打开存档的文件
	freopen("max.in", "w", stdout);																			//打开存档的文件
	cin >> history_max_score;																				//读入历史存档
	loadimage(&tu, "下载.jpg");																				//读入图片
	page();																									//输出页面
	Load();																									//制作方格
	do {
		set_mode();																							//获取游戏模式
		Empty_map();																						//清空数组
		now_score = 0;																						//清空分数
		set_map(4);																							//给地图赋值
		get_map(4);																							//输出地图
		do {
			page();																							//输出游戏页面
			play();																							//开始游戏
		} while (!over);
		write();																							//写入存档
		over_game();																						//结束页面
		cleardevice();																						//清空屏幕
	} while (!over);
	closegraph();																							//关闭窗口
	fclose(stdin);																							//关闭存档文件
	fclose(stdout);																							//关闭存档文件
	return 0;
}