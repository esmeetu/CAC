#define  _EYE_GRAPHICS

#include <graphics.h>
#include <math.h>

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
    #include <conio.h>
#endif

//全局比例尺变量
double Scale = 1.0;	//坐标比例尺
double s_Scale = 500.0;	//成图比例尺


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
int curveFit(pList pl, double t);


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
		if(!pColorTable) 
		fread(pColorTable, sizeof(colorTable), 256, fin);
		else printf("%d\n",sizeof(colorTable));
		imgTemp = (unsigned char*)malloc(sizeof(char)*lineByte*BmpHeight);
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


//矩阵运算：加减乘除
/* 矩阵转置 */
void Matrix_Transpose(Matrix a, Matrix b)
{
	int i,j;
	b.row = a.col;
	b.col = a.row;
	for(i = 0; i<b.row; i++)
	{
		for(j = 0;j<b.col;j++)
		{
			b.data[i*b.col+j] = a.data[j*b.row+i];
		}
	}
}

/*矩阵与数r相乘*/
void Matrix_rMultiply(Matrix a, double r, Matrix b)
{
	int i;
	b = a;
	for(i = 0;i<b.row*b.col;i++)
	{
		b.data[i] = a.data[i] * r;
	}
}

/* 矩阵相加 */
void Matrix_Plus(Matrix a, Matrix b, Matrix c)
{
	int i;
	c = a;
	for(i = 0;i<c.row*c.col;i++)
	{
		c.data[i] = a.data[i] + b.data[i];
	}
}

/* 矩阵相减 */
void Matrix_Minus(Matrix a, Matrix b, Matrix c)
{
	int i;
	c = a;
	for(i = 0;i<c.row*c.col;i++)
	{
		c.data[i] = a.data[i] - b.data[i];
	}
}

/* 显示矩阵 */
void Matrix_Print(Matrix m) {
	int i, j;
	for (i = 0; i < m.row; i++) {
		for (j = 0; j < m.col; j++) {
			printf("%lf ", *(m.data + i * m.col + j));
		}
		printf("\n");
	}
}

/* 矩阵相乘 */
void Matrix_Multiply(Matrix a, Matrix b, Matrix c) {
	int i, j, k;
	c.row = a.row;
	c.col = b.col;
	double sum;

	for (i = 0; i < c.row; ++i)
	{
		for (j = 0; j < c.col; ++j)
		{
			sum = 0.0;
			for (k = 0; k < a.col; ++k)
			{
				sum +=  (*(a.data+i*a.col+k)) * (*(b.data+k*c.col+j));
			}
			*(c.data+i*c.col+j) = sum;
		}
	}
}

int curveFit(pList pl, double t) {

	double xi, yi;
	double x1, x2, x3, x4;
	double y1, y2, y3, y4;
	double i;

	#ifdef EYE_GRAPHICS
	initgraph(screenWidth, screenHeight); // 初始化
	printf("eye graphics\n");
	#else 
	int gd = DETECT, gmode = 0;
	initgraph(&gd, &gmode = 0);
	printf("tc graphics\n");
	#endif

	if (get_Length(pl) < 5)
	{
		return -1;
	}

	x1 = pl ->next ->p.x;
	x2 = pl ->next ->next ->p.x;
	x3 = pl ->next ->next ->next ->p.x;
	x4 = pl ->next ->next ->next ->next ->p.x;
	
	y1 = pl ->next ->p.y;
	y2 = pl ->next ->next ->p.y;
	y3 = pl ->next ->next ->next ->p.y;
	y4 = pl ->next ->next ->next ->next ->p.y;

	for (i = 0; i < t; i = i + t/1000)
	{
		xi = x2 - (x1 - x3) * i + 2 * (2 * x1 - 5 * x2 + 4 * x3 - x4) * i * i - 4 * (x1 - 3 * x2 + 3 * x3 - x4) * i * i * i;
		yi = y2 - (y1 - y3) * i + 2 * (2 * y1 - 5 * y2 + 4 * y3 - y4) * i * i - 4 * (y1 - 3 * y2 + 3 * y3 - y4) * i * i * i;
		// printf("%lf, %lf\n", xi, yi);
		putpixel((int)(xi + 10), (int)(yi + 10), RED);	
	}

	getch();
	closegraph();
	return 1;
}