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

//ȫ�ֱ����߱���
double Scale = 1.0;	//���������
double s_Scale = 500.0;	//��ͼ������


/*
 ************************************************������������*****************************************************
 */

double angle2radian(angle x);	//�����ȷ���ת���Ⱥ���
angle radian2angle(double y);	//��������ת�ȷ��뺯��
void outputAngle(angle aout);	//����Ƕ�
void outputRadian(double yout);	//�������
Point get_b(Point a1, double dDirection, double dDistance);	//�õ�B������
void displayPoint(Point pp);	//���ƽ�������

pList creatNList(int n);	//���������С����������
pList createPointList(FILE *fp);	//��������
pList insertPoint(pList plhead, pNode *pp, int loc);	//��������в���ָ��λ�õĵ�
pList deletePoint(pList plhead, int IDinfo);	//ɾ����������ָ��ID�ĵ�
void searchPoint(pList plhead, int IDinfo);	//��ѯ��������ָ��ID�ĵ�
int get_Length(pList plhead);	//�õ���������
void disPointList(pList plhead);	//��ʾ������
void savePointList(pList plhead, FILE *fp);	//���������

EntityList creatEntityList(FILE *p);	//��ȡ���ļ�������������
EntityList AddPointInEntity(EntityList ehead, pList pl, unsigned int lloc, int ploc);	//���������в���ָ��λ�õ�
EntityList DelPointInEntity(EntityList ehead, unsigned LIDinfo, int IDinfo);	//��������ɾ��ָ����
void SrcPointInEntity(EntityList ehead, unsigned LIDinfo, int IDinfo);	//�������в�ѯ��
EntityList AddLine(EntityList ehead, Entity *pLine, int alloc);	//���������������
EntityList DelLine(EntityList ehead, unsigned int LID);		//����������ɾ����
void disEntityList(EntityList ehead);	//��ʾ������
void saveEntityList(EntityList ehead, FILE *fp);	//����������

double dot(Vector v1, Vector v2);	//ʸ�����
double cross(Point p0, Point p1, Point p2);	//ʸ�����
int isRegion(pList pl);	//�ж�һ�������Ƿ�Ϊ��
int isRecCross(Point p1, Point p2, Point q1, Point q2);	//�ж��ڽӾ����ཻ
int pointOffLine(Point p, Point p1, Point p2);	//�����ߵĲ�λ��ϵ����������,�������⣩
int regionContainsPoint(pList pl, Point p);	//������Ĺ�ϵ���������ڣ��������ϣ��������⣩
int pointOffLineSeg(Point p, Point p1, Point p2);	//�����߶εĹ�ϵ�������߶��ϣ������߶��⣬�����߶��ӳ����ϣ�
int lineSegOffLineSeg(Point p1, Point p2, Point q1, Point q2);	//�߶����߶εĹ�ϵ����λ�����룬�ཻ��
int lineSegOffRegion(pList pl,pList reg);	//�߶�����Ĺ�ϵ���ཻ�����룩
double getPointToPointDistance(Point p1, Point p2);		//	�õ��������
double getPointToLineDistance(Point p, Point p1, Point p2);	//�㵽ֱ�ߵľ���
double getPointToLineSegDistance(Point p, Point p1, Point p2);	//�㵽�߶εľ���
double getMinPointToRegionDistance(Point p, pList pl);	//�õ��㵽���������̾���
Point getCenter(pList pl);	//�õ�����ͼ�ε���С��Ӿ��ε�����
scrPoint geoConvertToScreen(Point p, Point pCenter);	//��������ת��Ļ����
void drawEntity(pList region, Point pCenter);	//�������
double Min(double x, double y);	//ȡ��Сֵ
double Max(double x, double y);	//ȡ���ֵ

int saveBmp(char *bmpName, unsigned char *imgTemp, long width, long height, unsigned short biBitCount, colorTable *pColorTable); //����bmp�ļ�
void printBmpInfo(BitFILEHEADER *bfh, BitINFOHEADER *bih);	//��ӡbmp��Ϣ
int readBmpHead(FILE *fin);	//��ȡbmp�ļ�

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
//����ͼ�β���
//
void drawEntity(pList region, Point pCenter) {
	#ifdef EYE_GRAPHICS
	initgraph(screenWidth, screenHeight); // ��ʼ��
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

//��������תΪ��Ļ���꣬�����������꣬�ͱ�����
scrPoint geoConvertToScreen(Point p, Point pCenter) {
	scrPoint scrp;
	scrp.scrY = abs(-screenHeight/2 + (int)((p.x - pCenter.x)/Scale + 0.5));	//��������
	scrp.scrX = abs(-screenWidth/2 + (int)((pCenter.y - p.y)/Scale + 0.5));
	return scrp;
}

//�õ�ͼ���е����꣬���ȵ�����С��Ӿ��κ��������ؾ�����������
Point getCenter(pList plhead) {
	double xmin, xmax, ymin, ymax;
	pList pl = plhead ->next;
	Point pCenter;
	double Scale1 = 0.0, Scale2 = 0.0;
	if(pl == NULL) {
		printf("ȡ��������ʱ������Ϊ�գ�\n");
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

//�ж�һ���������Ƿ�Ϊ��
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
//�������ϵ����
//
// ���
double cross(Point p0, Point p1, Point p2) {
	return (p1.x - p0.x)*(p2.y - p0.y) - (p1.y - p0.y)*(p2.x - p0.x);
}

// ���
double dot(Vector v1, Vector v2) {
	return v1.x*v2.x + v1.y*v2.y;
}

//�õ��㵽��ľ���
double getPointToPointDistance(Point p1, Point p2) {
	return sqrt(pow(p2.x - p1.x,2.0) + pow(p2.y - p1.y,2.0));
}

// ��������
Vector getVector(Point p1, Point p2) {
	Vector v;
	v.x = p2.x - p1.x;
	v.y = p2.y - p1.y;
	return v;
}

//�õ��㵽�߶εľ���(�����߶������)
double getPointToLineSegDistance(Point p, Point p1, Point p2) {
	Vector v, v1, v2;
	v = getVector(p1, p2);
	v1 = getVector(p1, p);
	v2 = getVector(p2, p);
	if(dot(v,v1) <= 0) return getPointToPointDistance(p,p1);
	if(dot(v,v2) >= 0) return getPointToPointDistance(p,p2);
	return getPointToLineDistance(p,p1,p2);
}

//�õ��㵽ֱ�ߵľ���
double getPointToLineDistance(Point p, Point p1, Point p2) {
	if(p1.x == p2.x && p1.y == p2.y) {
		printf("ֱ�������غϣ�\n");
		return 0.0;
	}
	else return fabs((p.x-p1.x)*(p2.y-p1.y) - (p.y-p1.y)*(p2.x-p1.x))
	/ sqrt(pow((p2.y - p1.y),2.0) + pow((p2.x - p1.x),2.0));
}

// �жϵ���ֱ�ߵĲ�λ��ϵ
int pointOffLine(Point p, Point p1, Point p2) {
	if(((p.x - p1.x) * (p2.y - p1.y) - (p.y - p1.y)*(p2.x - p1.x)) == 0) 
		return onLine;
	return outLine;
}

//
//�жϵ����߶ι�ϵ
//1.���ߣ�(p-p1)��(p2 - p1) = 0
//2.p����p1��p2Ϊ�Խ��ߵľ�����
//
int pointOffLineSeg(Point p, Point p1, Point p2) {
	if(pointOffLine(p, p1, p2) == onLine) {
		//�ж��Ƿ���p1,p2���ɵľ�����
		if((Min(p1.x, p2.x) <= p.x) && (Max(p1.x, p2.x) >= p.x) && (Min(p1.y, p2.y) <= p.y) && (Max(p1.y, p2.y) >= p.y)) 
			return onLineSeg;
		return onExLineSeg;
	}
	return outLineSeg;
}

//������İ����������ϵ���������ߣ�
int regionContainsPoint(pList pl, Point p) {
	pList pltemp = pl ->next;
	Point p1, p2;
	int count = 0;		//�������
	double t = 0.0, xt = 0.0;
	while(pltemp ->next != NULL) {
		p1 = pltemp ->p;
		p2 = pltemp ->next ->p;
		if((p.y <= p2.y && p.y > p1.y) || (p.y > p2.y && p.y <= p1.y)) {
			t = (p.y - p1.y)/(p2.y - p1.y);		//���Ʊ�
			xt = p1.x + t*fabs(p2.x - p1.x);		//��ƽ�Ƶ����϶�Ӧ��xֵ
			if(p.x == xt) return onRegion;
			if(p.x < xt) count ++;
		}
		pltemp = pltemp ->next;
	}
	printf("count:%d\n", count);
	return count % 2 ? inRegion : outRegion;	//�������ڲ���ż�����ⲿ
}

//�߶��ڽӾ����Ƿ��ཻ
int isRecCross(Point p1, Point p2, Point q1, Point q2) {
	if(Min(p1.x, p2.x) > Max(q1.x, q2.x) 
		|| Max(p1.x, p2.x) < Min(q1.x, q2.x))
		return 0;
	else if(Min(p1.y, p2.y) > Max(q1.y, q2.y) 
		|| Max(p1.y, p2.y) < Min(q1.y, q2.y))
		return 0;
	else return 1;
}

//�߶����߶εĹ�ϵ
// �����ж��ڽӾ��ι�ϵ�������ų����飬�����ò�����п�������
int lineSegOffLineSeg(Point p1, Point p2, Point q1, Point q2) {
	if(!isRecCross(p1, p2, q1, q2)) return offLineSeg;
	if((pointOffLineSeg(p1, q1, q2) == onLineSeg) && (pointOffLineSeg(q1, p1, p2) == onLineSeg)) 
		return coLineSeg;	//�ཻ
	return offLineSeg;	//����
}

//ȡ��ֵ��Сֵ
double Min(double x, double y) {
	return x < y ? x : y;
}

//ȡ��ֵ���ֵ
double Max(double x, double y) {
	return x > y ? x : y;
}

//��γ��ת����
double angle2radian(angle x) {	
	double a1;
	a1 = (x.deg+x.min/60.0+x.sec/3600.0)*PI/180;
	return a1;
}	

//����ת��γ��
angle radian2angle(double y) {	
	angle b1;
	b1.deg = (int)(y*180.0/PI);
	b1.min = (int)(((y*180.0/PI)-b1.deg)*60.0);
	b1.sec = (double)(((((y*180.0/PI)-b1.deg)*60.0)-b1.min)*60.0);
	return b1;
}

//�����γ��
void outputAngle(angle aout){
	printf("�ȷ���ֱ���:%d��%d��%.1f��\n",aout.deg,aout.min,aout.sec);
}

//�������
void outputRadian(double yout){
	printf("������:%lf\n",yout);
}

//�õ�B������
Point get_b(Point a1, double dDirection, double dDistance){
	double dx,dy;
	Point b1;
	dx = dDistance * cos(dDirection);
	dy = dDistance * sin(dDirection);
	b1.x = a1.x + dx;
	b1.y = a1.y + dy;
	return b1;
}

//��ʾ������
void displayPoint(Point pp){
	printf("��һ�������Ϊ:%d,%lf,%lf,%lf\n",pp.ID, pp.x, pp.y, pp.h);
}

//��ȡ�ļ����ݣ�����������
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
	printf("�����Ѵ�����\n");
	return head;
}

//����ĳ��С�ĵ�����
pList creatNList(int n){
	int i = 0;
	pList head, current, end;
	head = (pList)malloc(sizeof(pNode));
	end = head;
	current = NULL;
	while(n < 1) {
		printf("����������������:\n");
		scanf("%d",&n);
	}
	for(; i < n; i++) {
		current = (pList)malloc(sizeof(pNode));
		if(current) {
			current ->p.ID = 0; current ->p.x = 0.0; current ->p.y = 0.0; current ->p.h = 0.0;
			printf("�����%d���ڵ����ݣ�\n",i+1);	
			scanf("%d,%lf,%lf,%lf",&current ->p.ID, &current ->p.x, &current ->p.y, &current ->p.h);
			end ->next = current;
			end = current;
		}
		else
			printf("�ڴ������������ԣ�\n");
	}
	end ->next = NULL;
	printf("�����Ѵ�����\n");
	return head;
}

//�õ�������ĳ���
int get_Length(pList plhead){
	pList pl = plhead ->next;
	int len = 0;
	for(; pl != NULL; pl = pl ->next) {
		len++;
	}
	return len;
}

//�����������
pList insertPoint(pList plhead, pNode *pp, int loc){
	int i = 0;
	pList pl = plhead;
	int len;
	len = get_Length(pl);
	if(plhead == NULL) {
		printf("����Ϊ�գ������´�������\n");
		return NULL;
	}
	if(loc < 1 || loc > len) {
		printf("����λ�ò�������Χ�ڣ�\n");
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
		printf("�ڵ��Ѳ��룡\n");
		return plhead;
	}
}

//ɾ��ָ����
pList deletePoint(pList plhead, int IDinfo) {
	pList delp = plhead ->next;
	pList pre;	//ɾ���ڵ����һ���ڵ�
	if(delp == NULL) {
		printf("����Ϊ�գ������´�������\n");
		return NULL;
	}
	while(delp) {
		if(delp ->p.ID == IDinfo) {
			if(pre) {
				pre ->next = delp ->next;
			}
			//�׽ڵ�
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

//��ѯָ����
void searchPoint(pList plhead, int IDinfo){
	int plsearched;	//��ע�Ƿ�������
	pList pl = plhead;
	if(pl == NULL) {
		printf("����Ϊ�գ������´�������");
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
			printf("�ڵ����ҵ�!\n");
			displayPoint(pl ->p);
		}
		else
			printf("�ڵ�δ�ҵ���\n");
	}
}

//���������
void disPointList(pList plhead){
	int n = 0;
	pList h = plhead -> next;
	if(h == NULL) 
		printf("��Ϊ������");
	else {
		for(; h != NULL; h = h -> next) {
			n++;
			printf("��%d������ڵ����ݣ�%d,\t(%lf,%lf,%lf)\n", n, h ->p.ID, h -> p.x, h -> p.y, h -> p.h);
		}
	}
}

//�������������
void savePointList(pList plhead, FILE *fp){
	pList h = plhead -> next;
	if(h == NULL) 
		printf("��Ϊ������");
	else {
		for(; h != NULL; h = h -> next) {
			fprintf(fp,"%d,%lf,%lf,%lf\n", h ->p.ID, h -> p.x, h -> p.y, h -> p.h);
		}
	}
	printf("�ѱ��棡\n");
}

//����������
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
		printf("�ڴ������������ԣ�\n");
	}
	printf("�����Ѵ�����\n");
	return head;
}

//��ʾ������
void disEntityList(EntityList ehead){
	int n = 0;
	EntityList h = ehead ->enext;
	pList pl = NULL;
	unsigned int num;
	if(h == NULL) {
		printf("��Ϊ������");
	}
	else {
		if(t == Line) 
			printf("��������Ϊ��\n");
		else
			printf("��������Ϊ��\n");
		for(; h != NULL; h = h -> enext) {
			n++;
			printf("��%d������ڵ����ݣ�%u,%u\n", n, h ->LID, h ->Num);
			num = h ->Num;
			pl = h ->pElement ->next;
			if(pl == NULL) {
				printf("��%d������ڵ�����Ϊ�գ�", n);
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
//������������ӵ�
EntityList AddPointInEntity(EntityList ehead, pList pl, unsigned int lloc, int ploc){
	EntityList addllist = ehead;
	EntityList ltemp = ehead ->enext;
	unsigned int n = 0;
	pList add;
	if(ltemp == NULL) {
		printf("����������Ϊ�գ�\n");
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
				printf("δ�ҵ����߽ڵ㣡\n");
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
				printf("δ�ҵ����߽ڵ㣡\n");
				return NULL;
			}
		}
	}
	return ehead;
}

void SrcPointInEntity(EntityList ehead, unsigned LIDinfo, int IDinfo){
	EntityList temp = ehead ->enext;
	unsigned int LID = 0;
	if(temp == NULL) printf("��������Ϊ�գ�\n");
	else {
		for(; temp != NULL; temp = temp ->enext) {
			LID++;
			if(LID == LIDinfo) {
				searchPoint(temp ->pElement, IDinfo);
				break;
			}
			if(temp ->enext == NULL) {
				printf("δ�ҵ����߽ڵ㣡\n");
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
		printf("����Ϊ�գ������´�������\n");
		
		return NULL;
	}

	if(alloc < 1 || alloc > len) {
		printf("����λ�ò�������Χ�ڣ�\n");
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
		printf("�ڵ��Ѳ��룡\n");
		return ehead;
	}
}

EntityList DelLine(EntityList ehead, unsigned int LID){
	EntityList dell = ehead;
	EntityList temp = dell;
	int isExist = 0;
	if(dell == NULL) {
		printf("����Ϊ�գ������´�������\n");
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
		printf("�ڵ���ɾ����\n");
		return ehead;
	}
	else {
		printf("δ�ҵ���Ҫɾ�����ݣ�\n");
		return NULL;
	}
	return ehead;
}

//
//	����������
//
void saveEntityList(EntityList ehead, FILE *fp){
	EntityList savetemp = ehead ->enext;
	pList pl = NULL;
	if(savetemp == NULL) {
		printf("������Ϊ�գ�\n");
	}
	for(; savetemp != NULL; savetemp = savetemp ->enext) {
		fprintf(fp,"%u,%u\n", savetemp ->LID, savetemp ->Num);
		pl = savetemp ->pElement ->next;
		for(; pl != NULL; pl = pl ->next) {
			fprintf(fp,"\t%d,%lf,%lf,%lf\n",pl ->p.ID, pl ->p.x, pl ->p.y, pl ->p.h);
		}
	}
	printf("�ѱ��棡\n");
}

//
//	����bmp�ļ�
//
int saveBmp(FILE *fp, unsigned char *imgTemp, long width, long height, unsigned short biBitCount, colorTable *pColorTable) {
	BitFILEHEADER *fileheader;
	BitINFOHEADER *infoheader;

	int colorTableSize = 0;
	int lineByte;
	if(!imgTemp) return 0;	//dataΪ��

	if(biBitCount == 8) {
		colorTableSize = 1024;
	}

	//
	//	д��fileheader
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
	//	д��infoheader
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
	//	д����ɫ��
	//
	if(biBitCount == 8) {
		fwrite(pColorTable, sizeof(colorTable), 256, fp);
	}

	//
	//	д��λͼ����
	//
	fwrite(imgTemp, 1, lineByte*height, fp);

	printBmpInfo(fileheader, infoheader);

	//	�ر��ļ�

	return 1;
}

//
//	��ӡbmpͼƬ��Ϣ
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
//	��ȡbmp�ļ�	
//
int readBmpHead(FILE *fin) {
	BitFILEHEADER *bfh = (BitFILEHEADER*)malloc(sizeof(BitFILEHEADER));
	BitINFOHEADER *bih = (BitINFOHEADER*)malloc(sizeof(BitINFOHEADER));
	long lineByte;

	fread(&bfh ->bfType, sizeof(char), 2, fin);
	if(BM != bfh ->bfType) {
		printf("this is not bmp��\n");
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
	lineByte = (BmpWidth*biBitCount/8 + 3)/4*4;	//���ֽ���������Ϊ4�ı�����
	FREE(bfh);
	FREE(bih);
	if(biBitCount == 8) {
		printf("eight bits bmp��\n");
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
*  ����ѹ�����෨��ƫ�Ƿ���������˹-�տ˷�
* 
*/	

/*
* ���෨
*/
pList cjf(pList pl, double d) {
	pList plhead = pl ->next;
	pList pDel;		//�ͷ���ɾ���ڴ�
	Point p1, p2, p3;
	double D;	//����
	
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

//�����������һ��������Ӧ�ļн�
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
* ƫ�Ƿ�
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
 *ɾ��������pStart��pEnd�ĵ� 
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
* ������˹-�տ˷�
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
 * �õ��������е�i����ĵ����� 
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


//�������㣺�Ӽ��˳�
/* ����ת�� */
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

/*��������r���*/
void Matrix_rMultiply(Matrix a, double r, Matrix b)
{
	int i;
	b = a;
	for(i = 0;i<b.row*b.col;i++)
	{
		b.data[i] = a.data[i] * r;
	}
}

/* ������� */
void Matrix_Plus(Matrix a, Matrix b, Matrix c)
{
	int i;
	c = a;
	for(i = 0;i<c.row*c.col;i++)
	{
		c.data[i] = a.data[i] + b.data[i];
	}
}

/* ������� */
void Matrix_Minus(Matrix a, Matrix b, Matrix c)
{
	int i;
	c = a;
	for(i = 0;i<c.row*c.col;i++)
	{
		c.data[i] = a.data[i] - b.data[i];
	}
}

/* ��ʾ���� */
void Matrix_Print(Matrix m) {
	int i, j;
	for (i = 0; i < m.row; i++) {
		for (j = 0; j < m.col; j++) {
			printf("%lf ", *(m.data + i * m.col + j));
		}
		printf("\n");
	}
}

/* ������� */
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
	initgraph(screenWidth, screenHeight); // ��ʼ��
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