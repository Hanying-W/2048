#include "easyx.h"
#include "ctime"
#include "iostream"
#include "conio.h"
#include "cmath"
#include "sstream"
#include "wingdi.h"
using namespace std;
MOUSEMSG M;																									//����һ�������Ϣ
int my_map[6][6];																							//�趨��ͼ
IMAGE image[40];																							//���巽��ͼƬ
IMAGE tu;																									//����һ��ͼ��
int now_score = 0;																							//��ǰ�÷�
int history_max_score;																						//��ʷ��ߵ÷�
bool over = false;																							//����Ƿ������Ϸ
int mode;																									//���ģʽ
/********************************************�������***************************************************/	//����
void Empty_map() {
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			my_map[x][y] = 0;
		}
	}
}
/***************************************����Ƿ��п�λ************************************************/		//����
bool check_null() {
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			if (!my_map[x][y])
				return true;
	return false;
}
/****************************************����Ƿ��ܺϲ�*************************************************/	//����
bool check_merge(int x1, int y1, int x2, int y2) {
	if (x1 == x2) {																							//���в���
		for (int y = y1 + 1; y < y2; y++)
			if (my_map[x1][y])
				return false;
		return true;
	}
	if (y1 == y2) {																							//���в���
		for (int x = x1 + 1; x < x2; x++)
			if (my_map[x][y1])
				return false;
		return true;
	}
}
/****************************************�����Ϸ�Ƿ����************************************************/	//����
bool check_overgame() {
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (!my_map[x][y])																				//��ǰλ��Ϊ��
				return false;
			if (my_map[x][y] == my_map[x][y + 1])															//�ұ�һλ���ڵ�ǰ�����Ժϲ���
				return false;
			if (my_map[x][y] == my_map[x + 1][y])															//�±�һλ���ڵ�ǰ�����Ժϲ���
				return false;
		}
	}
	return true;
}
/*************************************�Դ浵�ļ����д�ȡ����*********************************************/	//����
void write() {
	if (now_score > history_max_score)
		cout << now_score;
	else
		cout << history_max_score;
}
/**************************************����������ʽ����ɫ************************************************/	//����
void settext(int height, int weight, UINT color) {
	settextstyle(height, 0, _T("Arial"), 0, 0, weight, false, false, false, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH);
	settextcolor(color);
}
/*****************************************����ͼƬ����***************************************************/	//����
void CreateImage(IMAGE* img, LPCTSTR num, COLORREF imgColor, int fontSize, COLORREF fontColor) {
	SetWorkingImage(img);																					//��img���л�ͼ
	setbkmode(TRANSPARENT);																					//���ñ�����ʽΪ͸��
	setbkcolor(0x9eaebb);																					//��ʮ�����Ʊ�ʾ������ɫ
	settext(fontSize, 1000, fontColor);																		//������ʽ
	setfillcolor(imgColor);																					//���������ɫ
	settextcolor(fontColor);																				//����������ɫ
	cleardevice();																							//�����Ļ����������(0,0)��
	solidroundrect(0, 0, img->getwidth() - 1, img->getheight() - 1, 10, 10);								//�滭Բ�Ǿ���
	RECT r = { 0,0,img->getwidth() - 1,img->getheight() - 1 };												//����һ����������
	drawtext(num, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);												//��ָ��λ�ð���ָ����ʽ���ָ���ַ���
}
/*****************************************����ͼƬ����**************************************************/	//����
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
/**********************************��ָ�����������ھ�������ַ���****************************************/		//����
void printtext(LPCTSTR s, int left, int top, int right, int width)
{
	RECT r = { left, top, right, width };																	//ȷ��һ��Բ�Ǿ��β���
	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);												//��ָ����������ָ����ʽ����ַ���
}
/*******************************************����ͼ��ֵ*************************************************/		//����
void set_map(int k) {
	if (!check_null())
		return;
	int x, y, a;
	do {
		x = rand() % k;																						//�������һ��x����
		y = rand() % k;																						//�������һ��y����
	} while (my_map[x][y]);																					//���ԭ��λ�������֣�����������
	do {
		a = 1 + rand() % 4;																					//�������һ��ż��������Ӧ��λ��
	} while (a % 2);
	my_map[x][y] = a;
}
/********************************************�����ͼ**************************************************/		//����
void get_map(int k) {
	for (int x = 0; x < k; x++) {
		for (int y = 0; y < k; y++) {
			int p = (log(my_map[x][y]) / log(2));															//�����ֵ��2�ļ�����
			putimage(25 + y * 100, 225 + x * 100, &image[p]);
		}
	}
}
/************************************������ģʽ�¶Ե�ͼ���в���*****************************************/		//���� �����ֺϲ�
void move_entertainment(int k) {
	int sign = 0;																							//���ڱ���Ƿ��ܲ���
	if (k == 1) {																							//����
		for (int y = 0; y < 4; y++) {																		//�̶���
			for (int x = 0; x < 4; x++) {																	//�д��ϵ���Ѱ������ֵ��λ��
				bool merge_image = false;																	//��һ���Ƿ�ϲ���
				for (int z = x + 1; z < 4; z++) {															//����Ѱ�������ֵ�λ��
					if (my_map[z][y]) {																		//�ҵ���
						if (my_map[x][y] && !merge_image && my_map[z][y] == my_map[x][y]) {					//���ԭ����λ������
							merge_image = true;																//��ʾ�Ѿ��ϳɹ�
							my_map[x][y] += my_map[z][y];
							my_map[z][y] = 0;
							sign = 1;
							now_score += my_map[x][y];														//���·���
							break;
						}
						if (!my_map[x][y]) {																//ԭ��λ��û����
							my_map[x][y] = my_map[z][y];
							my_map[z][y] = 0;
							sign = 1;
						}
					}
				}
			}
		}
	}
	else if (k == 2) {																						//����
		for (int x = 0; x < 4; x++) {																		//�̶���
			for (int y = 0; y < 4; y++) {																	//�д�����Ѱ��
				bool merge_image = false;																	//����Ƿ�ϳɹ�
				for (int z = y + 1; z < 4; z++) {															//����Ѱ�������ֵ�λ��
					if (my_map[x][z]) {																		//�ҵ���
						if (my_map[x][y] && !merge_image && my_map[x][z] == my_map[x][y]) {					//���ԭ����λ������
							merge_image = true;																//��ʾ�Ѿ��ϳɹ�
							my_map[x][y] += my_map[x][z];
							my_map[x][z] = 0;
							sign = 1;
							now_score += my_map[x][y];														//���·���
							break;
						}
						if (!my_map[x][y]) {																//ԭ��λ��û����
							my_map[x][y] = my_map[x][z];
							my_map[x][z] = 0;
							sign = 1;
						}
					}
				}
			}
		}
	}
	else if (k == 3) {																						//����
		for (int x = 0; x < 4; x++) {																		//�̶���
			for (int y = 4 - 1; y >= 0; y--) {																//�д��ҵ���Ѱ��
				bool merge_image = false;																	//����Ƿ�ϳɹ�
				for (int z = y - 1; z >= 0; z--) {															//����Ѱ�������ֵ�λ��
					if (my_map[x][z]) {																		//�ҵ���
						if (my_map[x][y] && !merge_image && my_map[x][z] == my_map[x][y]) {					//���ԭ����λ������
							merge_image = true;																//��ʾ�Ѿ��ϳɹ�
							my_map[x][y] += my_map[x][z];
							my_map[x][z] = 0;
							sign = 1;
							now_score += my_map[x][y];														//���·���
							break;
						}
						if (!my_map[x][y]) {																//ԭ��λ��û����
							my_map[x][y] = my_map[x][z];
							my_map[x][z] = 0;
							sign = 1;
						}
					}
				}
			}
		}
	}
	else {																									//����
		for (int y = 0; y < 4; y++) {																		//�̶���
			for (int x = 4 - 1; x >= 0; x--) {																//�д��µ���Ѱ������ֵ��λ��
				bool merge_image = false;																	//��һ���Ƿ�ϲ���
				for (int z = x - 1; z >= 0; z--) {															//����Ѱ�������ֵ�λ��
					if (my_map[z][y]) {																		//�ҵ���
						if (my_map[x][y] && !merge_image && my_map[z][y] == my_map[x][y]) {					//���ԭ����λ������
							merge_image = true;																//��ʾ�Ѿ��ϳɹ�
							my_map[x][y] += my_map[z][y];
							my_map[z][y] = 0;
							sign = 1;
							now_score += my_map[x][y];														//���·���
							break;
						}
						if (!my_map[x][y]) {																//ԭ��λ��û����
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
/************************************������ģʽ�¶Ե�ͼ���в���*****************************************/		//����
void move_normal(int k) {
	int sign = 0;																							//���ڱ���Ƿ��ܲ���
	if (k == 1) {																							//����
		for (int y = 0; y < 4; y++) {																		//�̶���
			for (int x = 0; x < 4; x++) {																	//�д��ϵ���Ѱ������ֵ��λ��
				bool merge_image = false;																	//��һ���Ƿ�ϲ���
				for (int z = x + 1; z < 4; z++) {															//����Ѱ�������ֵ�λ��
					if (my_map[z][y]) {																		//�ҵ���
						if (my_map[x][y] && !merge_image &&
							my_map[z][y] == my_map[x][y] && check_merge(x, y, z, y)) {						//���ԭ����λ������
							merge_image = true;																//��ʾ�Ѿ��ϳɹ�
							my_map[x][y] += my_map[z][y];
							my_map[z][y] = 0;
							sign = 1;
							now_score += my_map[x][y];														//���·���
							break;
						}
						if (!my_map[x][y]) {																//ԭ��λ��û����
							my_map[x][y] = my_map[z][y];
							my_map[z][y] = 0;
							sign = 1;
						}
					}
				}
			}
		}
	}
	else if (k == 2) {																						//����
		for (int x = 0; x < 4; x++) {																		//�̶���
			for (int y = 0; y < 4; y++) {																	//�д�����Ѱ��
				bool merge_image = false;																	//����Ƿ�ϳɹ�
				for (int z = y + 1; z < 4; z++) {															//����Ѱ�������ֵ�λ��
					if (my_map[x][z]) {																		//�ҵ���
						if (my_map[x][y] && !merge_image &&
							my_map[x][z] == my_map[x][y] && check_merge(x, y, x, z)) {						//���ԭ����λ������
							merge_image = true;																//��ʾ�Ѿ��ϳɹ�
							my_map[x][y] += my_map[x][z];
							my_map[x][z] = 0;
							sign = 1;
							now_score += my_map[x][y];														//���·���
							break;
						}
						if (!my_map[x][y]) {																//ԭ��λ��û����
							my_map[x][y] = my_map[x][z];
							my_map[x][z] = 0;
							sign = 1;
						}
					}
				}
			}
		}
	}
	else if (k == 3) {																						//����
		for (int x = 0; x < 4; x++) {																		//�̶���
			for (int y = 4 - 1; y >= 0; y--) {																//�д��ҵ���Ѱ��
				bool merge_image = false;																	//����Ƿ�ϳɹ�
				for (int z = y - 1; z >= 0; z--) {															//����Ѱ�������ֵ�λ��
					if (my_map[x][z]) {																		//�ҵ���
						if (my_map[x][y] && !merge_image &&
							my_map[x][z] == my_map[x][y] && check_merge(x, z, x, y)) {						//���ԭ����λ������
							merge_image = true;																//��ʾ�Ѿ��ϳɹ�
							my_map[x][y] += my_map[x][z];
							my_map[x][z] = 0;
							sign = 1;
							now_score += my_map[x][y];														//���·���
							break;
						}
						if (!my_map[x][y]) {																//ԭ��λ��û����
							my_map[x][y] = my_map[x][z];
							my_map[x][z] = 0;
							sign = 1;
						}
					}
				}
			}
		}
	}
	else {																									//����
		for (int y = 0; y < 4; y++) {																		//�̶���
			for (int x = 4 - 1; x >= 0; x--) {																//�д��µ���Ѱ������ֵ��λ��
				bool merge_image = false;																	//��һ���Ƿ�ϲ���
				for (int z = x - 1; z >= 0; z--) {															//����Ѱ�������ֵ�λ��
					if (my_map[z][y]) {																		//�ҵ���
						if (my_map[x][y] && !merge_image &&
							my_map[z][y] == my_map[x][y] && check_merge(z, y, x, y)) {						//���ԭ����λ������
							merge_image = true;																//��ʾ�Ѿ��ϳɹ�
							my_map[x][y] += my_map[z][y];
							my_map[z][y] = 0;
							sign = 1;
							now_score += my_map[x][y];														//���·���
							break;
						}
						if (!my_map[x][y]) {																//ԭ��λ��û����
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
/*******************************************��ȡ��������***********************************************/		//����
void set_keyboard() {
	char k;
	k = _getch();
	if (k == 'w' || k == 'W') {																				//����
		if (mode == 1)																						//����ģʽ
			move_normal(1);
		else																								//����ģʽ
			move_entertainment(1);
	}
	else if (k == 'a' || k == 'A') {																		//����
		if (mode == 1)																						//����ģʽ
			move_normal(2);
		else																								//����ģʽ
			move_entertainment(2);
	}
	else if (k == 'd' || k == 'D') {																		//����
		if (mode == 1)																						//����ģʽ
			move_normal(3);
		else																								//����ģʽ
			move_entertainment(3);
	}
	else if (k == 's' || k == 'S') {																		//����
		if (mode == 1)																						//����ģʽ
			move_normal(4);
		else																								//����ģʽ
			move_entertainment(4);
	}
}
/***********************************************�˵�**************************************************/		//����
void menu() {
	settextcolor(WHITE);																					//����������ɫΪ��ɫ
	settextstyle(50, 0, "����");																				//����������ʽΪ����
	outtextxy(125, 150, "��Ϸģʽ");
	outtextxy(125, 300, "����ģʽ");
	outtextxy(125, 450, "����ģʽ");
}
/*****************************************�������������Ϣ*******************************************/		//����
int get_mousemsg() {
	int sign;
	do {
		sign = 0;																							//��ʼ����ʾ
		do {
			M = GetMouseMsg();																				//��ȡ�����Ϣ
		} while (!M.mkLButton);
		if (M.x >= 125 && M.x <= 315) {
			if (M.y >= 300 && M.y < 450)																	//����ģʽ
				return 1;
			else if (M.y >= 450 && M.y < 600)																//����ģʽ
				return 2;
			else
				sign = 1;																					//�������λ�ò��ڹ涨��λ����
		}
		else
			sign = 1;																						//�������λ�ò��ڹ涨��λ����
	} while (sign);
}
/********************************************ѡ����Ϸģʽ*********************************************/		//����
void set_mode() {
	menu();																									//����˵�����ѡ��
	mode = get_mousemsg();																					//�����Ϸ���Ѷ�
	cleardevice();																							//�����Ļ
}
/**********************************************��ʼ��Ϸ***********************************************/		//����
void play() {
	set_keyboard();																							//�Ӽ��̻�ȡ����
	set_map(4);																								//����ͼ��ֵ
	get_map(4);																								//�����ͼ
}
/********************************************���һ������*********************************************/		//����
void outintxy(int x, int dx, int y, int dy, const char* format, int n) {
	char tmp[11];
	sprintf(tmp, format, n);
	solidroundrect(x, y, x + 100, y + 100, 50, 50);															//��һ��Բ�ǵľ���
	settextstyle(77, 0, _T("Arial"));																		//�������ֵĸ�ʽ
	settextcolor(WHITE);																					//�������ֵ���ɫ
	outtextxy(x + dx, y + dy, tmp);																			//�������
}
/*********************************************��Ϸҳ��************************************************/		//����
void page() {
	//������Ϸͼ��
	putimage(10, 30, &tu);
	// ���Ƶ�ǰ����
	solidroundrect(112, 30, 264, 119, 10, 10);																//��Բ�Ǿ���
	settext(28, 800, 0xdbe6ee);																				//�������ַ��
	printtext(_T("SCORE"), 112, 40, 264, 69);																//���������ʾ
	settext(44, 800, WHITE);																				//�������ַ��
	char tmp[11];
	sprintf(tmp, "%d", now_score);																			//���ֺ��ַ�����ת��
	printtext(tmp, 112, 70, 264, 114);																		//�����ǰ����

	// ������߷���
	solidroundrect(275, 30, 427, 119, 10, 10);																//��Բ�Ǿ���
	settext(28, 800, 0xdbe6ee);																				//�������ַ��
	printtext(_T("BEST"), 275, 40, 427, 69);																//�����ʷ��߷�����ʾ
	settext(44, 800, WHITE);																				//�������ַ��
	sprintf(tmp, "%d", history_max_score);																	//ת��
	printtext(tmp, 275, 70, 427, 114);										 								//�����ʷ��߷���
}
/*******************************************��Ϸ����ҳ��**********************************************/		//����
void over_game() {
	// Game Over
	settext(60, 1000, 0x696f78);																			//�������ָ�ʽ
	printtext(_T("Game Over!"), 0, 0, 439, 199);															//�������
	// ReStart
	setfillcolor(0x9dadba);																					//���������ɫ
	solidroundrect(120, 310, 319, 389, 10, 10);																//��Բ�Ǿ���
	settext(36, 1000, WHITE);																				//�������ָ�ʽ
	printtext(_T("ReStart"), 120, 310, 319, 389);															//�������
	// Exit
	solidroundrect(120, 460, 319, 539, 10, 10);																//��Բ�Ǿ���
	printtext(_T("Exit"), 120, 460, 319, 539);																//�������
	FlushBatchDraw();																						//ִ��δ��ɵĻ�������
	FlushMouseMsgBuffer();																					//��������Ϣ����
	//��ȡ�����Ϣ
	int sign;
	do {
		sign = 0;																							//��ʼ����ʾ
		do {
			M = GetMouseMsg();																				//��ȡ�����Ϣ
		} while (!M.mkLButton);
		if (M.x >= 120 && M.x <= 319) {
			if (M.y >= 310 && M.y < 389)																	//���¿�ʼ
				over = true;
			else if (M.y >= 460 && M.y < 539)																//�˳���Ϸ
				over = false;
			else
				sign = 1;																					//�������λ�ò��ڹ涨��λ����
		}
		else
			sign = 1;																						//�������λ�ò��ڹ涨��λ����
	} while (sign);

	over = false;
}
/**********************************************������*************************************************/		//����
int main() {
	srand((int)time(NULL));																					//����һ�����������
	initgraph(440, 650);																					//��ʼ������ ��ɫĬ�Ϻ�ɫ
	freopen("max.in", "r", stdin);																			//�򿪴浵���ļ�
	freopen("max.in", "w", stdout);																			//�򿪴浵���ļ�
	cin >> history_max_score;																				//������ʷ�浵
	loadimage(&tu, "����.jpg");																				//����ͼƬ
	page();																									//���ҳ��
	Load();																									//��������
	do {
		set_mode();																							//��ȡ��Ϸģʽ
		Empty_map();																						//�������
		now_score = 0;																						//��շ���
		set_map(4);																							//����ͼ��ֵ
		get_map(4);																							//�����ͼ
		do {
			page();																							//�����Ϸҳ��
			play();																							//��ʼ��Ϸ
		} while (!over);
		write();																							//д��浵
		over_game();																						//����ҳ��
		cleardevice();																						//�����Ļ
	} while (!over);
	closegraph();																							//�رմ���
	fclose(stdin);																							//�رմ浵�ļ�
	fclose(stdout);																							//�رմ浵�ļ�
	return 0;
}