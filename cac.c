#define  EYE_GRAPHICS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <graphics.h>
#include <errno.h>

#define screenWidth		640
#define screenHeight 	480
#define PI 3.141592
#define eps 1e-6
#define BM 19778
#define FREE(X)						\
	do {							\
		if( (X) != NULL )           \
        {                           \
            free( (void *)(X) );    \
            (X)=NULL;               \
        }                           \
    }while(0)

#ifdef _EYE_GRAPHICS
    #define <conio.h>
#endif

/*
 **************************************************数据结构部分************************************************
*/

//全局比例尺变量
double Scale = 1.0;	//坐标比例尺
double s_Scale = 500.0;	//成图比例尺

//点结构体
typedef struct angle {
	int deg;
	int min;
	double sec;
}angle;	

//点结构体
typedef struct Point {
	double x, y, h;
	int ID;
}Point; 

//矢量结构
typedef Point Vector;

//点节点结构体
typedef struct pNode {
	Point p;
	struct pNode *next;
}pNode, *pList; 

enum Type { Line, Region};
enum Type t = Line;

//线节点结构体
typedef struct Entity {
	 unsigned int LID;
	 unsigned int Num;
	 enum Type type;
	 pList pElement;
	 struct Entity *enext;
}Entity, *EntityList; 

//屏幕坐标结构体
typedef struct scrPoint 
{
	int scrX;
	int scrY;
}scrPoint;		

//程序运行状态
enum STATUS {CLOSED, OPENED};
enum STATUS MAIN_STATUS = CLOSED;
enum STATUS CASE1_STATUS = CLOSED;
enum STATUS CASE2_STATUS = CLOSED;
enum STATUS CASE3_STATUS = CLOSED;
enum STATUS CASE4_STATUS = CLOSED;
enum STATUS CASE5_STATUS = CLOSED;
enum STATUS CASE6_STATUS = CLOSED;
enum STATUS CASE7_STATUS = CLOSED;
enum STATUS CASE8_STATUS = CLOSED;


//点和线侧位的关系outLine：线外 onLine线上
enum boolpol { outLine, onLine };

//线段和线段关系
enum boollsols { offLineSeg, coLineSeg,};

//点和面的关系
enum boolpor { outRegion, inRegion, onRegion };

//点和线段的关系
enum boolpols {	outLineSeg, onLineSeg, onExLineSeg };

//线段和面的关系
enum boollsor {	lineOutRegion, lineInterRegion };

//
//位图结构
//
/*位图文件头结构  共14字节*/
typedef struct  BitFILEHEADER {
	unsigned short bfType;/*说明文件的类型，该值必须为0X4D42(字符“BM”)*/
	unsigned long bfSize;/*说明文件的大小，以字节为单位*/
	unsigned short bfReserved1;/*保留位，可设置为0*/
	unsigned short bfReserved2;/*保留位，可设置为0*/
	unsigned long bfOffBits;/*说明从文件头到实际图像数据之间的字节偏移量。不固定(为位图信息头和调色板的长度)*/
}BitFILEHEADER;

/*位图信息头结构  共40字节*/
typedef  struct  BitINFOHEADER {	
	unsigned long biSize;/*说明位图信息头结构的大小*/
	long biWidth;/*说明位图的宽度，单位为像素——biWidth' = ((int)((biWidth * 8 + 31)/32))*4  */
	long biHeight;/*说明位图的高度，单位为像素，正数为倒向位图，负数为正向位图*/
	unsigned short biPlanes;/*说明设备位面数，值为1*/
	unsigned short biBitCount;/*说明数据比特数/像素值，值可为1，4，8，16，24，32*/
	unsigned long biCompression;/*说明位图数据压缩类型*/
	unsigned long biSizeImage;/*说明位图大小，单位为字节*/
	long biXPelsPerMeter;/*说明位图水平分辨率，单位为像素/米*/
	long biYPelsPerMeter;/*说明位图垂直分辨率，单位为像素/米*/
	unsigned long biClrUsed;/*说明位图实际使用的彩色表中的颜色索引数(为0时，说明为调色板)*/
	unsigned long biClrImportant;/*说明对位图图像显示有重要影响的颜色索引的数目(0表示都重要)*/
}BitINFOHEADER;

/*位图颜色表结构 共 4字节*/
typedef  struct  colorTable {
	unsigned char rgbBlue;/*说明蓝色值*/
	unsigned char rgbGreen;/*说明绿色值*/
	unsigned char rgbRed;/*说明红色值*/
	unsigned char rgbReserved;/*保留为，可设置为0*/
}colorTable;

unsigned long offbits = 0;
long BmpWidth = 0L;
long BmpHeight = 0L;
unsigned char *imgTemp;
unsigned short biBitCount;
colorTable *pColorTable;

/*
 ************************************************函数声明部分*****************************************************
 */

double angle2radian(angle x);	//声明度分秒转弧度函数
angle radian2angle(double y);	//声明弧度转度分秒函数
void outputAngle(angle aout);	//输出角度
void outputRadian(double yout);	//输出弧度
Point get_b(Point a1, double dDirection, double dDistance);	//得到B点坐标
void displayPoint(Point pp);	//输出平面点坐标

pList creatNList(int n);	//输入链表大小，创建链表
pList createPointList(FILE *fp);	//创建链表
pList insertPoint(pList plhead, pNode *pp, int loc);	//向点链表中插入指定位置的点
pList deletePoint(pList plhead, int IDinfo);	//删除点链表中指定ID的点
void searchPoint(pList plhead, int IDinfo);	//查询点链表中指定ID的点
int get_Length(pList plhead);	//得到点链表长度
void disPointList(pList plhead);	//显示点链表
void savePointList(pList plhead, FILE *fp);	//保存点链表

EntityList creatEntityList(FILE *p);	//读取线文件，创建线链表
EntityList AddPointInEntity(EntityList ehead, pList pl, unsigned int lloc, int ploc);	//向线链表中插入指定位置点
EntityList DelPointInEntity(EntityList ehead, unsigned LIDinfo, int IDinfo);	//线链表中删除指定点
void SrcPointInEntity(EntityList ehead, unsigned LIDinfo, int IDinfo);	//线链表中查询点
EntityList AddLine(EntityList ehead, Entity *pLine, int alloc);	//向线链表中添加线
EntityList DelLine(EntityList ehead, unsigned int LID);		//向线链表中删除线
void disEntityList(EntityList ehead);	//显示线链表
void saveEntityList(EntityList ehead, FILE *fp);	//保存线链表

double dot(Vector v1, Vector v2);	//矢量点积
double cross(Point p0, Point p1, Point p2);	//矢量叉积
int isRegion(pList pl);	//判断一个链表是否为面
int isRecCross(Point p1, Point p2, Point q1, Point q2);	//判断内接矩形相交
int pointOffLine(Point p, Point p1, Point p2);	//点与线的侧位关系（点在线上,点在线外）
int regionContainsPoint(pList pl, Point p);	//点与面的关系（点在面内，点在面上，点在面外）
int pointOffLineSeg(Point p, Point p1, Point p2);	//点与线段的关系（点在线段上，点在线段外，点在线段延长线上）
int lineSegOffLineSeg(Point p1, Point p2, Point q1, Point q2);	//线段与线段的关系（共位，相离，相交）
int lineSegOffRegion(pList pl,pList reg);	//线段与面的关系（相交，相离）
double getPointToPointDistance(Point p1, Point p2);		//	得到两点距离
double getPointToLineDistance(Point p, Point p1, Point p2);	//点到直线的距离
double getPointToLineSegDistance(Point p, Point p1, Point p2);	//点到线段的距离
double getMinPointToRegionDistance(Point p, pList pl);	//得到点到面或折线最短距离
Point getCenter(pList pl);	//得到地理图形的最小外接矩形的中心
scrPoint geoConvertToScreen(Point p, Point pCenter);	//地理坐标转屏幕坐标
void drawEntity(pList region, Point pCenter);	//画多边形
double Min(double x, double y);	//取最小值
double Max(double x, double y);	//取最大值

int saveBmp(char *bmpName, unsigned char *imgTemp, long width, long height, unsigned short biBitCount, colorTable *pColorTable); //保存bmp文件
void printBmpInfo(BitFILEHEADER *bfh, BitINFOHEADER *bih);	//打印bmp信息
int readBmpHead(FILE *fin);	//读取bmp文件

Point getPoint(pList pl, int i);
pList cjf(pList pl, double d);
double getAngle(Point p2, Point p1, Point p3);
pList pjf(pList pl, double rad);
pList delPoint(pList pl, int i);
pList delPoints(pList pl, int pStart, int pEnd);
pList dgls_pkf(pList pl, int start, int end, double d);
int get_Max(pList pl, int start, int end, double d);


/*
**功能控制语句
*/
void init_Main();
void init_Control1();
void init_Control2();
void init_Control3();
void init_Control4();
void init_Control5();
void init_Control6();
void init_Control7();
void init_Control8();

/*
 ***************************************函数定义部分***********************************************************
*/

//
//功能控制部分
//
void init_Main() {
	printf("----------------------------------------\n\n");
	printf("\t1、	平面坐标与地理坐标转换\n");
	printf("\t2、	平面坐标正反算\n");
	printf("\t3、	点链表操作\n");
	printf("\t4、	线链表操作\n");
	printf("\t5、	图形绘制\n");
	printf("\t6、	点线面关系\n");
	printf("\t7、	读写BMP图片\n");
	printf("\t8、	矢量数据压缩\n");
	printf("\t0、	退出程序\n\n");
	printf("----------------------------------------\n");
	printf("请选择功能编号：");
	MAIN_STATUS = OPENED;
}

void init_Control1() {
	printf("------------请选择-----------\n\n");
	printf("\t1.输入并度分秒转弧度\n");
	printf("\t2.输入并弧度转度分秒\n");
	printf("\t0.返回上一级\n\n");
	printf("-----------------------------\n");
	printf("请选择操作编号：");
	CASE1_STATUS = OPENED;
}

void init_Control2() {
	printf("------------请选择-----------\n\n");
	printf("\t1.坐标反算\n");
	printf("\t0.返回上一级\n\n");
	printf("-----------------------------\n");
	printf("请选择操作编号：");
	CASE2_STATUS = OPENED;
}

void init_Control3() {
	printf("------------请选择-----------\n\n");
	printf("\t1.插入点数据\n");
	printf("\t2.删除点数据\n");
	printf("\t3.查询点数据\n");
	printf("\t4.保存点链表\n");
	printf("\t0.返回上一级\n\n");
	printf("-----------------------------\n");
	printf("请选择操作编号：");
	CASE3_STATUS = OPENED;
}

void init_Control4() {
	printf("------------请选择-----------\n\n");
	printf("\t1.线链表插入点数据\n");
	printf("\t2.线链表删除点数据\n");
	printf("\t3.线链表查询点数据\n");
	printf("\t4.线链表插入线数据\n");
	printf("\t5.线链表删除线数据\n");
	printf("\t6.保存线链表文件\n\n");
	printf("\t0.返回上一级\n\n");
	printf("-----------------------------\n");
	printf("请选择操作编号：");
	CASE4_STATUS = OPENED;
}

void init_Control5() {
	printf("------------请选择-----------\n\n");
	printf("\t1.绘制点\n");
	printf("\t2.绘制线或面\n");
	printf("\t0.返回上一级\n\n");
	printf("-----------------------------\n");
	printf("请选择操作编号：");
	CASE5_STATUS = OPENED;
}

void init_Control6() {
	printf("------------请选择-----------\n\n");
	printf("\t1.点与直线的关系，并输出最短距离\n");
	printf("\t2.点与线段的关系，并输出最短距离\n");
	printf("\t3.点与面的关系，并输出最短和最长距离\n");
	printf("\t0.返回上一级\n\n");
	printf("-----------------------------\n");
	printf("请选择操作编号：\n");
	CASE6_STATUS = OPENED;
}

void init_Control7() {
	printf("------------请选择-----------\n\n");
	printf("\t1.读取bmp文件\n");
	printf("\t0.返回上一级\n\n");
	printf("-----------------------------\n");
	printf("请选择操作编号：");
	CASE7_STATUS = OPENED;
}

void init_Control8() {
	printf("------------请选择-----------\n\n");
	printf("\t1.垂距法压缩\n");
	printf("\t2.偏角法压缩\n");
	printf("\t3.道格拉斯-普克法压缩\n");
	printf("\t0.返回上一级\n\n");
	printf("-----------------------------\n");
	printf("请选择操作编号：");
	CASE8_STATUS = OPENED;
}


//
//绘制图形部分
//
void drawEntity(pList region, Point pCenter) {
	#ifdef EYE_GRAPHICS
	initgraph(screenWidth, screenHeight); // 初始化
	printf("eye graphics\n");
	#else 
	int gd = DETECT, gmode = 0;
	initgraph(&gd, &gmode = 0);
	printf("tc graphics\n");
	#endif

	pList regionTemp;
	scrPoint scrP1, scrP2;
	regionTemp = region ->next;
	while(regionTemp ->next != NULL) {
		scrP1 = geoConvertToScreen(regionTemp ->p, pCenter);
		scrP2 = geoConvertToScreen(regionTemp ->next ->p, pCenter);
		line(scrP1.scrX, scrP1.scrY, scrP2.scrX, scrP2.scrY);
		regionTemp = regionTemp ->next;
	}
	getch();
	closegraph();
}

//地理坐标转为屏幕坐标，输入地理点坐标，和比例尺
scrPoint geoConvertToScreen(Point p, Point pCenter) {
	scrPoint scrp;
	scrp.scrY = abs(-screenHeight/2 + (int)((p.x - pCenter.x)/Scale + 0.5));	//四舍五入
	scrp.scrX = abs(-screenWidth/2 + (int)((pCenter.y - p.y)/Scale + 0.5));
	return scrp;
}

//得到图形中点坐标，首先调用最小外接矩形函数，返回矩形中心坐标
Point getCenter(pList plhead) {
	double xmin, xmax, ymin, ymax;
	pList pl = plhead ->next;
	Point pCenter;
	double Scale1 = 0.0, Scale2 = 0.0;
	if(pl == NULL) {
		printf("取中心坐标时，链表为空！\n");
		exit(0);
	}
	xmin = xmax = pl ->p.x;
	ymin = ymax = pl ->p.y;
	for(; pl ->next != NULL;) {
		xmin = Min(xmin, pl ->next ->p.x);
		xmax = Max(xmax, pl ->next ->p.x);
		ymin = Min(ymin, pl ->next ->p.y);
		ymax = Max(ymax, pl ->next ->p.y);
		pl = pl ->next;
	}
	pCenter.x = xmin + (xmax - xmin)/2;
	pCenter.y = ymin + (ymax - ymin)/2;
	Scale1 = (xmax - xmin)/screenWidth;
	Scale2 = (ymax - ymin)/screenHeight;
	Scale = Scale1 > Scale2 ? Scale1 : Scale2;
	return pCenter;
}

//判断一个点链表是否为面
int isRegion(pList pl) {
	Point p1, p2;
	if(get_Length(pl) < 3) 
		return 0;
	p1 = pl ->next ->p;
	for(; pl ->next != NULL; pl = pl ->next);
	p2 = pl ->p;
	if(p1.ID != p2.ID || p1.x != p2.x || p1.y != p2.y || p1.h != p2.h)
		return 0;
	return 1;
}

double getMinPointToRegionDistance(Point p, pList pl) {
	pList pltemp = pl ->next;
	Point p1, p2;
	double minDis = 0.0, minDisTemp = 0.0;
	p1 = pltemp ->p;
	p2 = pltemp ->next ->p;
	minDis = getPointToLineSegDistance(p, p1, p2);
	pltemp = pltemp ->next;
	while(pltemp ->next != NULL) {
		p1 = pltemp ->p;
		p2 = pltemp ->next ->p;
		minDisTemp = getPointToLineSegDistance(p, p1, p2);
		minDis = minDisTemp < minDis ? minDisTemp : minDis;
		pltemp = pltemp ->next;
	}
	return minDis;
}

//
//点线面关系部分
//
// 叉积
double cross(Point p0, Point p1, Point p2) {
	return (p1.x - p0.x)*(p2.y - p0.y) - (p1.y - p0.y)*(p2.x - p0.x);
}

// 点积
double dot(Vector v1, Vector v2) {
	return v1.x*v2.x + v1.y*v2.y;
}

//得到点到点的距离
double getPointToPointDistance(Point p1, Point p2) {
	return sqrt(pow(p2.x - p1.x,2.0) + pow(p2.y - p1.y,2.0));
}

// 向量构造
Vector getVector(Point p1, Point p2) {
	Vector v;
	v.x = p2.x - p1.x;
	v.y = p2.y - p1.y;
	return v;
}

//得到点到线段的距离(点与线段最近点)
double getPointToLineSegDistance(Point p, Point p1, Point p2) {
	Vector v, v1, v2;
	v = getVector(p1, p2);
	v1 = getVector(p1, p);
	v2 = getVector(p2, p);
	if(dot(v,v1) <= 0) return getPointToPointDistance(p,p1);
	if(dot(v,v2) >= 0) return getPointToPointDistance(p,p2);
	return getPointToLineDistance(p,p1,p2);
}

//得到点到直线的距离
double getPointToLineDistance(Point p, Point p1, Point p2) {
	if(p1.x == p2.x && p1.y == p2.y) {
		printf("直线两点重合！\n");
		return 0.0;
	}
	else return fabs((p.x-p1.x)*(p2.y-p1.y) - (p.y-p1.y)*(p2.x-p1.x))
	/ sqrt(pow((p2.y - p1.y),2.0) + pow((p2.x - p1.x),2.0));
}

// 判断点与直线的侧位关系
int pointOffLine(Point p, Point p1, Point p2) {
	if(((p.x - p1.x) * (p2.y - p1.y) - (p.y - p1.y)*(p2.x - p1.x)) == 0) 
		return onLine;
	return outLine;
}

//
//判断点与线段关系
//1.共线：(p-p1)×(p2 - p1) = 0
//2.p在以p1和p2为对角线的矩形内
//
int pointOffLineSeg(Point p, Point p1, Point p2) {
	if(pointOffLine(p, p1, p2) == onLine) {
		//判断是否在p1,p2构成的矩形内
		if((Min(p1.x, p2.x) <= p.x) && (Max(p1.x, p2.x) >= p.x) && (Min(p1.y, p2.y) <= p.y) && (Max(p1.y, p2.y) >= p.y)) 
			return onLineSeg;
		return onExLineSeg;
	}
	return outLineSeg;
}

//点与面的包含与相离关系（向右射线）
int regionContainsPoint(pList pl, Point p) {
	pList pltemp = pl ->next;
	Point p1, p2;
	int count = 0;		//交点计数
	double t = 0.0, xt = 0.0;
	while(pltemp ->next != NULL) {
		p1 = pltemp ->p;
		p2 = pltemp ->next ->p;
		if((p.y <= p2.y && p.y > p1.y) || (p.y > p2.y && p.y <= p1.y)) {
			t = (p.y - p1.y)/(p2.y - p1.y);		//相似比
			xt = p1.x + t*fabs(p2.x - p1.x);		//点平移到边上对应的x值
			if(p.x == xt) return onRegion;
			if(p.x < xt) count ++;
		}
		pltemp = pltemp ->next;
	}
	printf("count:%d\n", count);
	return count % 2 ? inRegion : outRegion;	//奇数在内部，偶数在外部
}

//线段内接矩形是否相交
int isRecCross(Point p1, Point p2, Point q1, Point q2) {
	if(Min(p1.x, p2.x) > Max(q1.x, q2.x) 
		|| Max(p1.x, p2.x) < Min(q1.x, q2.x))
		return 0;
	else if(Min(p1.y, p2.y) > Max(q1.y, q2.y) 
		|| Max(p1.y, p2.y) < Min(q1.y, q2.y))
		return 0;
	else return 1;
}

//线段与线段的关系
// 首先判断内接矩形关系，快速排斥试验，再利用叉积进行跨立试验
int lineSegOffLineSeg(Point p1, Point p2, Point q1, Point q2) {
	if(!isRecCross(p1, p2, q1, q2)) return offLineSeg;
	if((pointOffLineSeg(p1, q1, q2) == onLineSeg) && (pointOffLineSeg(q1, p1, p2) == onLineSeg)) 
		return coLineSeg;	//相交
	return offLineSeg;	//相离
}

//取两值最小值
double Min(double x, double y) {
	return x < y ? x : y;
}

//取两值最大值
double Max(double x, double y) {
	return x > y ? x : y;
}

//经纬度转弧度
double angle2radian(angle x) {	
	double a1;
	a1 = (x.deg+x.min/60.0+x.sec/3600.0)*PI/180;
	return a1;
}	

//弧度转经纬度
angle radian2angle(double y) {	
	angle b1;
	b1.deg = (int)(y*180.0/PI);
	b1.min = (int)(((y*180.0/PI)-b1.deg)*60.0);
	b1.sec = (double)(((((y*180.0/PI)-b1.deg)*60.0)-b1.min)*60.0);
	return b1;
}

//输出经纬度
void outputAngle(angle aout){
	printf("度分秒分别是:%d°%d′%.1f″\n",aout.deg,aout.min,aout.sec);
}

//输出弧度
void outputRadian(double yout){
	printf("弧度是:%lf\n",yout);
}

//得到B点坐标
Point get_b(Point a1, double dDirection, double dDistance){
	double dx,dy;
	Point b1;
	dx = dDistance * cos(dDirection);
	dy = dDistance * sin(dDirection);
	b1.x = a1.x + dx;
	b1.y = a1.y + dy;
	return b1;
}

//显示点坐标
void displayPoint(Point pp){
	printf("另一点的坐标为:%d,%lf,%lf,%lf\n",pp.ID, pp.x, pp.y, pp.h);
}

//读取文件数据，并创建链表
pList createPointList(FILE *fp) {
	pList head, end, current;
	head = (pList)malloc(sizeof(pNode));
	end = head;
	while(!feof(fp)) {
		current = (pList)malloc(sizeof(pNode));
		fscanf(fp, "%d,%lf,%lf,%lf", &current ->p.ID, &current ->p.x, &current ->p.y, &current ->p.h);
		end -> next = current;
		end = current;
	}
	end -> next = NULL;
	printf("链表已创建！\n");
	return head;
}

//创建某大小的点链表
pList creatNList(int n){
	int i = 0;
	pList head, current, end;
	head = (pList)malloc(sizeof(pNode));
	end = head;
	current = NULL;
	while(n < 1) {
		printf("请输入大于零的整数:\n");
		scanf("%d",&n);
	}
	for(; i < n; i++) {
		current = (pList)malloc(sizeof(pNode));
		if(current) {
			current ->p.ID = 0; current ->p.x = 0.0; current ->p.y = 0.0; current ->p.h = 0.0;
			printf("输入第%d个节点数据：\n",i+1);	
			scanf("%d,%lf,%lf,%lf",&current ->p.ID, &current ->p.x, &current ->p.y, &current ->p.h);
			end ->next = current;
			end = current;
		}
		else
			printf("内存分配错误，请重试！\n");
	}
	end ->next = NULL;
	printf("链表已创建！\n");
	return head;
}

//得到点链表的长度
int get_Length(pList plhead){
	pList pl = plhead ->next;
	int len = 0;
	for(; pl != NULL; pl = pl ->next) {
		len++;
	}
	return len;
}

//向点链表插入点
pList insertPoint(pList plhead, pNode *pp, int loc){
	int i = 0;
	pList pl = plhead;
	int len;
	len = get_Length(pl);
	if(plhead == NULL) {
		printf("链表为空，请重新创建链表！\n");
		return NULL;
	}
	if(loc < 1 || loc > len) {
		printf("插入位置不在链表范围内！\n");
		return NULL;
	}
	else {
		if(loc == 1) {
			pp -> next = plhead ->next;
			plhead -> next = pp;
		}

		else if(loc == len) {
			for(; pl != NULL; pl = pl ->next) {
				i++;
				if(i == loc) 
					break;
			}
			pl ->next = pp;
			pp ->next = NULL;
		}

		else {
			for(; pl != NULL; pl = pl ->next) {
				i++;
				if(i == loc) 
					break;
			}
			pp ->next = pl ->next;
			pl ->next = pp;
		}
		printf("节点已插入！\n");
		return plhead;
	}
}

//删除指定点
pList deletePoint(pList plhead, int IDinfo) {
	pList delp = plhead ->next;
	pList pre;	//删除节点的上一个节点
	if(delp == NULL) {
		printf("链表为空，请重新创建链表！\n");
		return NULL;
	}
	while(delp) {
		if(delp ->p.ID == IDinfo) {
			if(pre) {
				pre ->next = delp ->next;
			}
			//首节点
			if(delp == plhead ->next) {
				plhead ->next = delp ->next;
			}
			break;
		}
		pre = delp;
		delp = delp ->next;
	}
	free(delp);
	return plhead;
}

//查询指定点
void searchPoint(pList plhead, int IDinfo){
	int plsearched;	//标注是否搜索到
	pList pl = plhead;
	if(pl == NULL) {
		printf("链表为空，请重新创建链表！");
	}
	else {
		for(; pl != NULL; pl = pl ->next) {
			if(pl ->p.ID == IDinfo) {
					plsearched = 1;
					break;
				}
			else plsearched = 0;
		}
		if(plsearched == 1) {
			printf("节点已找到!\n");
			displayPoint(pl ->p);
		}
		else
			printf("节点未找到！\n");
	}
}

//输出点链表
void disPointList(pList plhead){
	int n = 0;
	pList h = plhead -> next;
	if(h == NULL) 
		printf("此为空链表！");
	else {
		for(; h != NULL; h = h -> next) {
			n++;
			printf("第%d个链表节点数据：%d,\t(%lf,%lf,%lf)\n", n, h ->p.ID, h -> p.x, h -> p.y, h -> p.h);
		}
	}
}

//保存点链表数据
void savePointList(pList plhead, FILE *fp){
	pList h = plhead -> next;
	if(h == NULL) 
		printf("此为空链表！");
	else {
		for(; h != NULL; h = h -> next) {
			fprintf(fp,"%d,%lf,%lf,%lf\n", h ->p.ID, h -> p.x, h -> p.y, h -> p.h);
		}
	}
	printf("已保存！\n");
}

//创建线链表
EntityList creatEntityList(FILE *p){
	int i = 0;
	unsigned int num;
	enum Type TypeTemp = t;
	EntityList head, current, end;
	pList plhead, plcurrent, plend;
	head = (EntityList)malloc(sizeof(Entity));
	end = head;
	if(TypeTemp == Line) 
		 TypeTemp = Line;
	else
		 TypeTemp = Region;
	if(head != NULL) {
		while(!feof(p)) {
			plhead = (pList)malloc(sizeof(pNode));
			plend = plhead;
			current = (EntityList)malloc(sizeof(Entity));
			current ->type = TypeTemp;
			fscanf(p, "%u,%u", &current ->LID, &current ->Num);
			num = current ->Num;
			while(num--) {
				plcurrent = (pList)malloc(sizeof(pNode));
				fscanf(p, "%d,%lf,%lf,%lf", &plcurrent ->p.ID, &plcurrent ->p.x, &plcurrent ->p.y, &plcurrent ->p.h);
				plend ->next = plcurrent;
				plend = plcurrent;
			}
			plend ->next = NULL;
			current ->pElement = plhead;
			end ->enext = current;
			end = current;
			i++;
		}
		end ->enext = NULL;
	}
	else {
		printf("内存分配错误，请重试！\n");
	}
	printf("链表已创建！\n");
	return head;
}

//显示线链表
void disEntityList(EntityList ehead){
	int n = 0;
	EntityList h = ehead ->enext;
	pList pl = NULL;
	unsigned int num;
	if(h == NULL) {
		printf("此为空链表！");
	}
	else {
		if(t == Line) 
			printf("链表类型为线\n");
		else
			printf("链表类型为面\n");
		for(; h != NULL; h = h -> enext) {
			n++;
			printf("第%d个链表节点数据：%u,%u\n", n, h ->LID, h ->Num);
			num = h ->Num;
			pl = h ->pElement ->next;
			if(pl == NULL) {
				printf("第%d个链表节点数据为空！", n);
				break;
			}
			for(; num; num--) {
				printf("\t%d,(%lf,%lf,%lf)\n",pl ->p.ID, pl ->p.x, pl ->p.y, pl ->p.h);
				pl = pl ->next;
			}
			printf("\n");
		}
		printf("\n");
	}
}

int getLineLength(EntityList ehead){
	int num = 0;
	EntityList eh = ehead;
	for(;eh != NULL; eh = eh ->enext) {
		num ++;
	}
	return num;
}
//向线链表中添加点
EntityList AddPointInEntity(EntityList ehead, pList pl, unsigned int lloc, int ploc){
	EntityList addllist = ehead;
	EntityList ltemp = ehead ->enext;
	unsigned int n = 0;
	pList add;
	if(ltemp == NULL) {
		printf("输入线链表为空！\n");
		return NULL;
	}
	else {
		for(;ltemp != NULL; ltemp = ltemp ->enext) {
			n++;
			if(n == lloc) {
				add = ltemp ->pElement;
				add = insertPoint(add, pl, ploc);
				ltemp ->Num++;
				break;
			}
			if(ltemp ->enext == NULL) {
				printf("未找到该线节点！\n");
				return NULL;
			}
		}
	}
	return addllist;
}

EntityList DelPointInEntity(EntityList ehead, unsigned LIDinfo, int IDinfo){
	EntityList temp = ehead ->enext;
	pList pl = NULL;
	unsigned int lid = 0;
	if(temp == NULL) return NULL;
	else {
		for(; temp != NULL; temp = temp ->enext) {
			lid++;
			if(lid == LIDinfo) {
				pl = temp ->pElement;
				pl = deletePoint(pl, IDinfo);
				temp ->Num--;
				break;
			}
			if(temp ->enext == NULL) {
				printf("未找到该线节点！\n");
				return NULL;
			}
		}
	}
	return ehead;
}

void SrcPointInEntity(EntityList ehead, unsigned LIDinfo, int IDinfo){
	EntityList temp = ehead ->enext;
	unsigned int LID = 0;
	if(temp == NULL) printf("所查链表为空！\n");
	else {
		for(; temp != NULL; temp = temp ->enext) {
			LID++;
			if(LID == LIDinfo) {
				searchPoint(temp ->pElement, IDinfo);
				break;
			}
			if(temp ->enext == NULL) {
				printf("未找到该线节点！\n");
			}
		}
	}
}

EntityList AddLine(EntityList ehead, Entity *pLine, int alloc){
	int i = 0;
	EntityList pl = ehead;
	int len;
	len = getLineLength(pl);

	if(ehead == NULL) {
		printf("链表为空，请重新创建链表！\n");
		
		return NULL;
	}

	if(alloc < 1 || alloc > len) {
		printf("插入位置不在链表范围内！\n");
		return NULL;
	}
	else {
		if(alloc == 1) {
			pLine -> enext = ehead ->enext;
			ehead -> enext = pLine;
		}

		else if(alloc == len) {
			for(; pl != NULL; pl = pl ->enext) {
				i++;
				if(i == alloc) 
					break;
			}
			pl ->enext = pLine;
			pLine ->enext = NULL;
		}

		else {
			for(; pl != NULL; pl = pl ->enext) {
				i++;
				if(i == alloc) 
					break;
			}
			pLine ->enext = pl ->enext;
			pl ->enext = pLine;
		}
		printf("节点已插入！\n");
		return ehead;
	}
}

EntityList DelLine(EntityList ehead, unsigned int LID){
	EntityList dell = ehead;
	EntityList temp = dell;
	int isExist = 0;
	if(dell == NULL) {
		printf("链表为空，请重新创建链表！\n");
		return NULL;
	}
	for(; dell ->enext != NULL; dell = dell ->enext) {
		if(dell ->enext ->LID == LID) {
			isExist = 1;
			break;
		}
	}
	if(isExist == 1) {
		temp = dell ->enext;
		dell ->enext = dell ->enext ->enext;
		free(temp);
		printf("节点已删除！\n");
		return ehead;
	}
	else {
		printf("未找到所要删除数据！\n");
		return NULL;
	}
	return ehead;
}

//
//	保存线链表
//
void saveEntityList(EntityList ehead, FILE *fp){
	EntityList savetemp = ehead ->enext;
	pList pl = NULL;
	if(savetemp == NULL) {
		printf("线链表为空！\n");
	}
	for(; savetemp != NULL; savetemp = savetemp ->enext) {
		fprintf(fp,"%u,%u\n", savetemp ->LID, savetemp ->Num);
		pl = savetemp ->pElement ->next;
		for(; pl != NULL; pl = pl ->next) {
			fprintf(fp,"\t%d,%lf,%lf,%lf\n",pl ->p.ID, pl ->p.x, pl ->p.y, pl ->p.h);
		}
	}
	printf("已保存！\n");
}

//
//	保存bmp文件
//
int saveBmp(FILE *fp, unsigned char *imgTemp, long width, long height, unsigned short biBitCount, colorTable *pColorTable) {
	BitFILEHEADER *fileheader;
	BitINFOHEADER *infoheader;

	int colorTableSize = 0;
	int lineByte;
	if(!imgTemp) return 0;	//data为空

	if(biBitCount == 8) {
		colorTableSize = 1024;
	}

	//
	//	写入fileheader
	//
	fileheader = (BitFILEHEADER*)malloc(sizeof(BitFILEHEADER));
	fileheader->bfType = BM;
	lineByte = (width * biBitCount/8+3)/4*4;
	fileheader->bfSize = 14 + sizeof(BitINFOHEADER) + colorTableSize + lineByte*height;
	fileheader->bfReserved1 = 0;
	fileheader->bfReserved2 = 0;
	fileheader->bfOffBits = 14 + sizeof(BitINFOHEADER) + colorTableSize;
	fwrite(&fileheader->bfType, sizeof(char), 2, fp);
	fwrite(&fileheader->bfSize, sizeof(char), 4, fp);
	fwrite(&fileheader ->bfReserved1, sizeof(char), 2, fp);
	fwrite(&fileheader ->bfReserved2, sizeof(char), 2, fp);
	fwrite(&fileheader ->bfOffBits, sizeof(char), 4, fp);

	//
	//	写入infoheader
	//
	infoheader = (BitINFOHEADER*)malloc(sizeof(BitINFOHEADER));
	infoheader->biBitCount = biBitCount;
	if(biBitCount == 8) {
		infoheader->biClrImportant = 256;
		infoheader->biClrUsed = 256;
	}
	infoheader->biCompression = 0;
	infoheader->biHeight = height;
	infoheader->biPlanes = 1;
	infoheader->biSize = sizeof(BitINFOHEADER);
	infoheader->biSizeImage = lineByte*height;
	infoheader->biWidth = width;
	infoheader->biXPelsPerMeter = 0;
	infoheader->biYPelsPerMeter = 0;
	fwrite(infoheader, sizeof(BitINFOHEADER), 1, fp);

	//
	//	写入颜色表
	//
	if(biBitCount == 8) {
		fwrite(pColorTable, sizeof(colorTable), 256, fp);
	}

	//
	//	写入位图数据
	//
	fwrite(imgTemp, 1, lineByte*height, fp);

	printBmpInfo(fileheader, infoheader);

	//	关闭文件

	return 1;
}

//
//	打印bmp图片信息
//
void printBmpInfo(BitFILEHEADER *bfh, BitINFOHEADER *bih) {
	printf("bfType              is %u.\n", bfh ->bfType);
	printf("bfSize              is %lu.\n", bfh ->bfSize);
	printf("bfReserved1         is %u.\n", bfh ->bfReserved1);
	printf("bfReserved2         is %u.\n", bfh ->bfReserved2);
	printf("bfOffBits           is %lu.\n", bfh ->bfOffBits);
	printf("biSize              is %lu. \n", bih ->biSize);
	printf("biWidth             is %ld.\n", bih ->biWidth);
	printf("biHeight            is %ld.\n", bih ->biHeight);
	printf("biPlanes            is %u. \n", bih ->biPlanes);
	printf("biBitCount          is %u. \n", bih ->biBitCount);
	printf("biCompression       is %lu. \n", bih ->biCompression);
	printf("biSizeImage         is %lu. \n", bih ->biSizeImage);
	printf("biXPelsPerMerer     is %ld.\n", bih ->biXPelsPerMeter);
	printf("biYPelsPerMerer     is %ld.\n", bih ->biYPelsPerMeter);	
	printf("biClrUsed           is %lu. \n", bih ->biClrUsed);
	printf("biClrImportant      is %lu. \n", bih ->biClrImportant);
}

//
//	读取bmp文件	
//
int readBmpHead(FILE *fin) {
	BitFILEHEADER *bfh = (BitFILEHEADER*)malloc(sizeof(BitFILEHEADER));
	BitINFOHEADER *bih = (BitINFOHEADER*)malloc(sizeof(BitINFOHEADER));
	long lineByte;

	fread(&bfh ->bfType, sizeof(char), 2, fin);
	if(BM != bfh ->bfType) {
		printf("this is not bmp！\n");
		return 1;
	}
	fread(&bfh ->bfSize, sizeof(char), 4, fin);
	fread(&bfh ->bfReserved1, sizeof(char), 2, fin);
	fread(&bfh ->bfReserved2, sizeof(char), 2, fin);
	fread(&bfh ->bfOffBits, sizeof(char), 4, fin);
	fread(bih, sizeof(char), 40, fin);
	printBmpInfo(bfh, bih);

	offbits = bfh ->bfOffBits;
	BmpWidth = bih ->biWidth;
	BmpHeight = bih ->biHeight;
	biBitCount = bih->biBitCount;
	lineByte = (BmpWidth*biBitCount/8 + 3)/4*4;	//行字节数（必须为4的倍数）
	FREE(bfh);
	FREE(bih);
	if(biBitCount == 8) {
		printf("eight bits bmp！\n");
		pColorTable = (colorTable*)malloc(sizeof(colorTable));
		perror("提示");
		if(!pColorTable) 
		fread(pColorTable, sizeof(colorTable), 256, fin);
		else printf("%d\n",sizeof(colorTable));
		imgTemp = (unsigned char*)malloc(sizeof(char)*lineByte*BmpHeight);
		perror("提示");
		if(!imgTemp)
		fread(imgTemp, 1, lineByte*BmpHeight, fin);
		else printf("%lu\n",sizeof(char)*lineByte*BmpHeight);
	}

	fclose(fin);
	return 0;
}


/**
*  数据压缩垂距法、偏角法、道格拉斯-普克法
* 
*/	

/*
* 垂距法
*/
pList cjf(pList pl, double d) {
	pList plhead = pl ->next;
	pList pDel;		//释放所删除内存
	Point p1, p2, p3;
	double D;	//垂距
	
	if(plhead ->next == NULL || plhead ->next ->next == NULL) return pl;

	do {	
		p1 = plhead ->p;
		p2 = plhead ->next ->p;
		p3 = plhead ->next ->next ->p;
		
		D = getPointToLineDistance(p2, p1, p3);
		if(D < d) {
			pDel = plhead ->next;
			FREE(pDel);
			plhead ->next = plhead ->next ->next;
			if (plhead ->next ->next == NULL) {
				break;
			}
			continue;
		}
		plhead = plhead ->next;
		
	}while(plhead ->next ->next != NULL);

	return pl;
}

//根据三点求第一个参数对应的夹角
double getAngle(Point p2, Point p1, Point p3) {
	double d1, d2;
	Vector v1, v2;
	d1 = getPointToPointDistance(p2, p1);
	d2 = getPointToPointDistance(p2, p3);
	v1 = getVector(p1, p2);
	v2 = getVector(p1, p3);
	return acos(dot(v1, v2)/abs(d1*d2));
}

/*
* 偏角法
*/
pList pjf(pList pl, double rad) {
	pList plhead = pl ->next;
	pList pDel;
	Point p1, p2, p3;
	double a;
	
	if(plhead ->next == NULL || plhead ->next ->next == NULL) return pl;

	do {	
		p1 = plhead ->p;
		p2 = plhead ->next ->p;
		p3 = plhead ->next ->next ->p;
		
		a = getAngle(p2, p1, p3);
		if(a < rad) {	
			pDel = plhead ->next;
			FREE(pDel);

			plhead ->next = plhead ->next ->next;
			if (plhead ->next ->next == NULL) 
				break;
			continue;
		}
		plhead = plhead ->next;
	}while(plhead ->next ->next != NULL);

	return pl;
}

/*
 *删除点链表pStart到pEnd的点 
*/
pList delPoints(pList pl, int pStart, int pEnd) {
	pList pTop = pl ->next;
	pList pBot = pl ->next;

	int i;

	for (i = 0; i < pStart; ++i)
	{
		pTop = pTop ->next;
	}

	for (i = 0; i < pEnd; ++i)
	{
		pBot = pBot ->next;
	}

	pTop ->next = pBot;

	return pl;
}

/*
* 道格拉斯-普克法
*/
int delNum = 0;
pList dgls_pkf(pList pl, int start, int end, double epsilon) {
	pList result = NULL, pTemp;
	double max = 0, d;
	int index = 0, i;
	for (i = start + 1; i < end; ++i)
		{
			d = getPointToLineDistance(getPoint(pl, i), getPoint(pl, start), getPoint(pl, end));
			if (d > max) 
			{
				index = i;
				max = d;
			}
		}	
		printf("%lf, %d\n", max, index);
	if (max < epsilon)
	{
		result = delPoints(pl, start-delNum, end-delNum);
		delNum += end - start - 1;
	} else {
		pTemp = dgls_pkf(pl, start, index, epsilon);
		pTemp = dgls_pkf(pl, index, end, epsilon);

		result = pTemp;
	}
	return result;
}

/*
 * 得到点链表中第i个点的点数据 
 */
Point getPoint(pList pl, int i) {
	pList pTemp = pl ->next;
	int j = 0;
	for (j = 0; j < i; ++j)
	{
		pTemp = pTemp ->next;
	}

	return pTemp ->p;
}

/*
 ***************************************函数定义部分***********************************************************
*/

// 主函数
int main() {
	int chosenNum;

	do {
		init_Main();
		scanf("%d",&chosenNum);
		switch(chosenNum) {
		//	功能：1、度分秒转弧度； 2、弧度转度分秒
			case 1 : {
				double b;
				angle a;
				do {
					init_Control1();
					scanf("%d",&chosenNum);
					switch(chosenNum) {
						case 1 : 
							printf("度分秒转弧度:	(如： 180,0,0 )\n"); 
							scanf("%d,%d,%lf",&a.deg,&a.min,&a.sec); 
							outputRadian(angle2radian(a)); 
							break;
						case 2 : 
							printf("弧度转度分秒:	(如： 3.141592 )\n"); 
							scanf("%lf",&b);
							outputAngle(radian2angle(b));  
							break;
						case 0 :
							CASE1_STATUS = CLOSED;
							break;
						default : 
							printf("输入错误！\n"); 
					}
				}while(CASE1_STATUS);
				break;
			}
		//	功能：坐标反算，根据一点坐标与到另一点的方位角和距离计算另一点坐标
			case 2 : {
				Point a1,b1;
				double dDirection, dDistance; //dDirection 方位角，dDirection 距离
				do {
					init_Control2();
					scanf("%d",&chosenNum);
					switch(chosenNum) {
						case 1 :
							printf("输入点ID，横坐标，纵坐标，方位角(弧度)，两点距离：(如： 0,0,3.141592,10)\n");
							scanf("%d,%lf,%lf,%lf,%lf",&a1.ID,&a1.x, &a1.y, &dDirection, &dDistance);
							b1 = get_b(a1, dDirection, dDistance);
							displayPoint(b1);
							break;
						case 0 :
							CASE2_STATUS =  CLOSED;
							break;
						default :
							printf("输入错误！\n"); 
					}
				}while(CASE2_STATUS);
				break;
			}
		//	功能： 创建链表，实现插入、删除、查询操作
			case 3 : {
				FILE *fpw;
				char fname[20];
				int createstyle;
				int n = 0, loc, delID, searchID;
				pList phead, pInsert, pDelete;
				pNode *Point = NULL;

				do {
					printf("创建新链表：\t0、返回上一级，1、从文件创建，2、根据大小创建\n"); 
					scanf("%d",&createstyle);
					if(createstyle == 0 ) break;
					if(createstyle == 1) {
						char filename[20];
						FILE *fp;
						printf("请输入所要打开的点文件及路径名:");
						scanf("%s",filename);
						if((fp=fopen(filename, "r")) == NULL) {
							printf("未能打开文件！");
							return 0;
						}
						phead = createPointList(fp);
						break;
					}
					else if(createstyle == 2) {
						printf("请输入所要创建链表大小：（如：10）"); 
						scanf("%d",&n);
						phead = creatNList(n);
						break;
					}
					else printf("请重新选择：\n");
				}while(createstyle);
				if(createstyle == 0 ) break;

				disPointList(phead);
				do {
					init_Control3();
					scanf("%d",&chosenNum);
					switch(chosenNum) {
						case 1 : 
							printf("插入点数据：\n"); 
							printf("请输入所要插入的点数据：(如：1,1.0,1.0,1.0,3)\n"); 
							Point = (pList)malloc(sizeof(pNode));
							scanf("%d,%lf,%lf,%lf,%d",&Point ->p.ID, &Point ->p.x, &Point ->p.y, &Point ->p.h, &loc);
							pInsert = (pList)malloc(sizeof(pNode));
							pInsert = insertPoint(phead, Point, loc);
							if(pInsert != NULL) 
								disPointList(pInsert);
							break;
						case 2 : 
							printf("删除点数据：\n"); 
							printf("请输入所要删除的节点ID：（如：3）\n"); 
							scanf("%d",&delID); 
							pDelete = (pList)malloc(sizeof(pNode));
							pDelete = deletePoint(phead, delID);
							if(pDelete != NULL) disPointList(pDelete);
							break;
						case 3 : 
							printf("查询点数据：\n"); 
							printf("请输入所要查询的节点ID：（如：3）\n"); 
							scanf("%d",&searchID); 
							searchPoint(phead, searchID);
							break;
						case 4 : 
							printf("保存点数据：\n"); 
							printf("请输入所要保存文件名及路径名：");
							scanf("%s",fname);
							if((fpw=fopen(fname, "w")) == NULL) {
								printf("未打开文件！\n");
								return 0;
							}
							else savePointList(phead, fpw);
							fclose(fpw);
							break;
						case 0 :
							CASE3_STATUS = CLOSED;
							break;
						default : 
							printf("请重新选择功能！\n");
					}
				}while(CASE3_STATUS);
				break;
			}
		//  功能:  从文件创建链表，实现插入、删除、查询操作
			case 4 : {
				int n = 0, t;
				unsigned int delLineLID;
				char rfilename[20], wfilename[20];
				int lloc, ploc, alloc, delLID, delID, searchLID, searchID;
			    FILE *fpr,*fpw;			
			    EntityList eList,eInsert,eDelete, addedLine, deledLine;
			    pList pl;

			    printf("请输入要创建的链表类型：0.线\t1.面\n");
			   	scanf("%d",&t);
			   	if(t == Line) 
			    	printf("请输入所要创建线文件名：（如：C:\\line.txt）\n");
			    else
			    	printf("请输入所要创建面文件名：（如：C:\\line.txt）\n");
			    scanf("%s",rfilename);
			    if((fpr=fopen(rfilename,"r"))==NULL) {
			    	printf("未能打开此文件!");
			    	return 0;
			    }
				else {
					eList=creatEntityList(fpr);
					disEntityList(eList);
					fclose(fpr);
				}

				do {
					init_Control4();
					scanf("%d",&chosenNum);
					switch(chosenNum) {
						case 1 :
							printf("线链表插入点数据：\n"); 
							printf("请输入所要插入的点数据及要插入线位置、点位置：(如：1,1.0,1.0,1.0,1,3)\n"); 
							pl = (pList)malloc(sizeof(pNode));
							scanf("%d,%lf,%lf,%lf,%u,%d",&pl->p.ID, &pl->p.x, &pl->p.y, &pl->p.h, &lloc, &ploc);
							eInsert = (EntityList)malloc(sizeof(Entity));
							eInsert = AddPointInEntity(eList, pl, lloc, ploc);
							if(eInsert ->enext != NULL) {
								disEntityList(eInsert);
							}
							break;
						case 2 :
						  	printf("线链表删除点数据：\n"); 
							printf("请输入所要删除的线节点LID和点节点ID：（如：3，3）\n");
						  	scanf("%u,%d",&delLID,&delID); 
							eDelete = (EntityList)malloc(sizeof(Entity));
						 	eDelete = DelPointInEntity(eList, delLID, delID);
						 	if(eDelete ->enext != NULL) {
						 		disEntityList(eDelete);
						 	}
						 	break;
						case 3 :
						 	printf("线链表查询点数据：\n"); 
							printf("请输入所要查询的节点ID：（如：1,1）\n");
							scanf("%u,%d",&searchLID, &searchID); 
						 	SrcPointInEntity(eList, searchLID, searchID);
						 	break;
						case 4 :
							printf("线链表插入线数据：\n");
							printf("请输入所要插入的线数据LID和Num及位置：\n");
							addedLine = (EntityList)malloc(sizeof(Entity));
							scanf("%u,%d,%d",&addedLine ->LID, &addedLine ->Num, &alloc);
							n = addedLine ->Num;
							printf("所要插入的线数据为：\n");
							addedLine ->pElement = creatNList(n);
							addedLine = AddLine(eList, addedLine, alloc);
							if(addedLine ->enext != NULL) 
								disEntityList(addedLine);
						 	break;
						case 5 :
							printf("线链表删除线数据：\n");
							printf("请输入所要删除的线LID：");
							scanf("%u",&delLineLID);
							deledLine = (EntityList)malloc(sizeof(Entity));
							deledLine = DelLine(eList, delLineLID);
							if(deledLine ->enext != NULL) 
								disEntityList(deledLine);
						 	break;
						case 6 :
							printf("保存线链表文件\n"); 
							printf("请输入所要保存的文件名及路径名：");
							scanf("%s",wfilename);
							if((fpw=fopen(wfilename, "w")) == NULL) {
								printf("不能打开此文件！\n");
								return 0;
							}
							else saveEntityList(eList, fpw);
							fclose(fpw);
							break;
						case 0 :
							CASE4_STATUS = CLOSED;
							break;
						default :
							printf("请重新选择功能！\n");
					}
				}while(CASE4_STATUS);
				break;
			}
		//	功能： 图形绘制
			case 5 : 
				do {
					init_Control5();
					scanf("%d",&chosenNum);
					switch(chosenNum) {
						case 1 : 
							scrPoint scrp;
							printf("请输入坐标：（100，30）\n");
							scanf("%d,%d",&scrp.scrX,&scrp.scrY);
							putpixel(scrp.scrX,scrp.scrY,BLUE);
							getch();
							break;
						case 2 : {
							FILE *fpr;
							char rfilename[20];
							pList region;
							Point pCenter;
							int choose = 1, size;

							do {
								printf("请选择创建线面或打开线面文件：1.打开文件   2.新建线面\n");
								scanf("%d",&choose);
								if(choose == 1) {
								    printf("请输入文件名：（如：D:\\region.txt）\n");
								    scanf("%s",rfilename);
								    if((fpr=fopen(rfilename,"r"))==NULL) {
								    	printf("未能打开此文件!");
								    	return 0;
								    }
									else {
										region = createPointList(fpr);
										if(isRegion(region)) {
											disPointList(region);
											fclose(fpr);
											break;
										}
										else printf("该文件不是个面！\n");
										break;
									}
								}
								else if(choose == 2) {
									printf("请输入所创建面的顶点数: ");
									scanf("%d",&size);
									region = creatNList(size);
									disPointList(region);
									break;
								}
								printf("请重新输入！\n");
							}while(choose);
							pCenter = getCenter(region);
							drawEntity(region, pCenter); 
							break;
						}
						case 0 : 
							CASE5_STATUS = CLOSED;
							break;
						default : 
							printf("请重新选择功能！\n");
					}
				}while(CASE5_STATUS);
				break;
		//	功能： 点线面关系
			case 6 : {
				Point p, p1, p2;
				int relation;
				double distance;
				double minDis;

				do {
					init_Control6();
					scanf("%d",&chosenNum);
					switch(chosenNum) {
						case 1 : {
							printf("请输入点坐标：（1.0，1.0）\n");
							scanf("%lf,%lf",&p.x,&p.y);
							printf("请输入直线两点坐标：(如2.0，2.0，3.0，4.0)\n");
							scanf("%lf,%lf,%lf,%lf",&p1.x,&p1.y,&p2.x,&p2.y);
							relation = pointOffLine(p, p1, p2);
							distance = getPointToLineDistance(p, p1, p2);
							if(relation == onLine) {
								printf("点在直线上！,点到该直线的最短距离为：%lf\n", distance);
							}
							else {
								printf("点在直线外！,点到该直线的最短距离为：%lf\n", distance);
							}
						 	break;
						}
						case 2 : {
							printf("请输入点坐标：（如1.0，1.0）\n");
							scanf("%lf,%lf",&p.x,&p.y);
							printf("请输入线段两点坐标：(如2.0，2.0，3.0，4.0)\n");
							scanf("%lf,%lf,%lf,%lf",&p1.x,&p1.y,&p2.x,&p2.y);
							relation = pointOffLineSeg(p, p1, p2);
							distance = getPointToLineSegDistance(p, p1, p2);
							if(relation == onLineSeg) {
								printf("点在线段上！,点到该线段的最短距离为：%lf\n", distance);
							}
							else if(relation == onExLineSeg) {
								printf("点在线段延长线上！,点到该线段的最短距离为：%lf\n", distance);
							}
							else {
								printf("点在线段外！,点到该线段的最短距离为：%lf\n", distance);
							}
							break;
						}
						case 3 : {
							FILE *fpr;
							char rfilename[20];
							pList region;
							int choose = 1;
							int size;
							printf("请选择创建面或打开面文件：1.打开面文件   2.创建面\n");
							scanf("%d",&choose);
							while(choose) {
								if(choose == 1) {
								    printf("请输入面文件名：（如：D:\\region.txt）\n");
								    scanf("%s",rfilename);
								    if((fpr=fopen(rfilename,"r"))==NULL) {
								    	printf("未能打开此文件!");
								    	return 0;
								    }
									else {
										region = createPointList(fpr);
										if(isRegion(region)) {
											disPointList(region);
											fclose(fpr);
											break;
										}
										else printf("该文件不是面！\n");
									}
								}
								else if(choose == 2) {
									printf("请输入所创建面的顶点数: ");
									scanf("%d",&size);
									region = creatNList(size);
									disPointList(region);
									break;
								}
								printf("请重新输入！\n请选择创建面或打开面文件：1.打开面文件   2.创建面\n");
								scanf("%d",&choose);
							}
							printf("请输入点坐标：（1.0，1.0）\n");
							scanf("%lf,%lf",&p.x,&p.y);
							relation = regionContainsPoint(region, p);
							minDis = getMinPointToRegionDistance(p, region);
							if(relation == onRegion) {
								printf("点在面上！,最短距离：%lf\n", minDis);
							}
							else if(relation == inRegion) {
								printf("点在面内！,最短距离：%lf\n", minDis);
							}
							else {
								printf("点在面外！,最短距离：%lf\n", minDis);
							}
							break;
						}
						case 0 :
							CASE6_STATUS = CLOSED;
							break;
						default : {
							printf("请重新选择功能！\n");
						}
					}
				}while(CASE6_STATUS);
				break;
			}
		//	功能：读写BMP图片
			case 7 : {
				char filename[10];
				FILE *fin,*fp;
				char bmpName[10];
				int isReaded, isSaved;

				do {
					init_Control7();
					scanf("%d",&chosenNum);
					switch(chosenNum) {
						case 1 :
							printf("输入bmp文件名：\n");
							scanf("%s",filename);
							fin = fopen(filename, "rb");
							if(fin == NULL) {
								printf("Cannot open this bmp\n");
								return 0;
							}
							isReaded = readBmpHead(fin);
							if(isReaded) {
								printf("fail to read!\n");
							}
							fclose(fin);
							
							printf("输入所要保存的文件名：\n");
							scanf("%s",bmpName);
							fp = fopen(bmpName, "wb");	
							if(fp == NULL) return 0;	//未打开文件
							isSaved = saveBmp(fp, imgTemp, BmpWidth, BmpHeight, biBitCount, pColorTable);
							if(!isSaved) 
								printf("写入失败！\n");
							else printf("已写入%s\n", bmpName);
							fclose(fp);
							break;
						case 0 : 
							CASE7_STATUS = CLOSED;
							break;
						default : 
							printf("请重新选择功能！\n");
					}
				}while(CASE7_STATUS);
				break;
			}

		//	功能：矢量数据压缩
			case 8 : {
				int n, createstyle;
				double d;
				pList pl;
				pList cjfList, pjfList, dgls_pkfList;

				do {
					printf("创建新链表：\t0、返回上一级，1、从文件创建，2、根据大小创建\n"); 
					scanf("%d",&createstyle);
					if(createstyle == 0 ) break;
					if(createstyle == 1) {
						char filename[20];
						FILE *fp;
						printf("请输入所要打开的点文件及路径名:");
						scanf("%s",filename);
						if((fp=fopen(filename, "r")) == NULL) {
							printf("未能打开文件！");
							return 0;
						}
						pl = createPointList(fp);
						break;
					}
					else if(createstyle == 2) {
						printf("请输入所要创建链表大小：（如：10）"); 
						scanf("%d",&n);
						pl = creatNList(n);
						break;
					}
					else printf("请重新选择：\n");
				}while(createstyle);
				if(createstyle == 0 ) break;

				disPointList(pl);

				do {
					init_Control8();
					scanf("%d",&chosenNum);
					switch(chosenNum) {
						case 1 :
							printf("输入垂距法阈值：\n");
							scanf("%lf", &d);
							cjfList = cjf(pl, d);
							printf("压缩结果：\n");
							disPointList(cjfList);
							break;
						case 2 :
							printf("输入偏角法角阈值：\n");
							scanf("%lf", &d);
							pjfList = pjf(pl, d);
							printf("压缩结果：\n");
							disPointList(pjfList);
							break;
						case 3 :
							printf("输入道格拉斯普克法阈值：\n");
							scanf("%lf", &d);
							dgls_pkfList = dgls_pkf(pl, 0, get_Length(pl) - 1, d);
							printf("压缩结果：\n");
							disPointList(dgls_pkfList);
							break;
						case 0 :
							CASE8_STATUS = CLOSED;
							break;
						default : 
							printf("请重新选择功能！\n");
					}
				}while(CASE8_STATUS);

				break;
			}
		//	功能：退出程序
			case 0 :
				MAIN_STATUS = CLOSED;
				break;
			default: {
				printf("输入有误!\n");
			}
		}
	}while(MAIN_STATUS);
	exit(0);
	return 0;
}