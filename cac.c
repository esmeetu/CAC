#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <graphics.h>
#define screenWidth		640
#define screenHeight 	480
#define PI 3.141592

/*
 **************************************************���ݽṹ����************************************************
*/
typedef struct angle {
	int deg;
	int min;
	double sec;
}angle;	//��ṹ��

typedef struct Point {
	double x, y, h;
	int ID;
}Point; //��ṹ��

typedef struct pNode {
	Point p;
	struct pNode *next;
}pNode, *pList; //��ڵ�ṹ��

enum Type { Line, Region};
enum Type t = Line;

typedef struct Entity {
	 unsigned int LID;
	 unsigned int Num;
	 enum Type type;
	 pList pElement;
	 struct Entity *enext;
}Entity, *EntityList; //�߽ڵ�ṹ��

typedef struct scrPoint 
{
	int scrX;
	int scrY;
}scrPoint;		//��Ļ����ṹ��

//����߲�λ�Ĺ�ϵoutLine������ onLine����
enum boolpol { outLine, onLine };

//�߶κ��߶ι�ϵ
enum boollsols { offLineSeg, coLineSeg,};

//�����Ĺ�ϵ
enum boolpor { outRegion, inRegion, onRegion };

//����߶εĹ�ϵ
enum boolpols {	outLineSeg, onLineSeg, onExLineSeg };

//�߶κ���Ĺ�ϵ
enum boollsor {	lineOutRegion, lineInterRegion };

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
void savePointList(pList plhead, FILE *fp);

EntityList creatEntityList(FILE *p);	//��ȡ���ļ�������������
EntityList AddPointInEntity(EntityList ehead, pList pl, unsigned int lloc, int ploc);	//���������в���ָ��λ�õ�
EntityList DelPointInEntity(EntityList ehead, unsigned LIDinfo, int IDinfo);	//��������ɾ��ָ����
void SrcPointInEntity(EntityList ehead, unsigned LIDinfo, int IDinfo);	//�������в�ѯ��
EntityList AddLine(EntityList ehead, Entity *pLine, int alloc);	//���������������
EntityList DelLine(EntityList ehead, unsigned int LID);		//����������ɾ����
void disEntityList(EntityList ehead);	//��ʾ������
void saveEntityList(EntityList ehead, FILE *fp);	//����������

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
scrPoint geoConvertToScreen(Point p, int drawScale, Point pCenter);
void drawLine(pList region, int scale, Point pCenter);
void drawRegion(pList region, int scale, Point pCenter);
double getXMinBound(pList pl);	
double getYMinBound(pList pl);
double getXMaxBound(pList pl);
double getYMaxBound(pList pl);
double Min(double x, double y);
double Max(double x, double y);

/*
 ***************************************�������岿��***********************************************************
*/
// ������
int main() {
	int chosenNum = 0;
	printf("----------------------------------------\n\n");
	printf("\t1��	ƽ���������������ת��\n");
	printf("\t2��	ƽ������������\n");
	printf("\t3��	���������\n");
	printf("\t4��	���������\n");
	printf("\t5��	ͼ�λ���\n");
	printf("\t6��	�������ϵ\n\n");
	printf("----------------------------------------\n");
	printf("---------------------------------------\n");
	printf("   �����ѡ���ܣ�(����-1�˳�����)   \n");
	printf("---------------------------------------\n");
	scanf("%d",&chosenNum);
	while(chosenNum != -1) {
		switch(chosenNum) {
	//	���ܣ�1���ȷ���ת���ȣ� 2������ת�ȷ���
		case 1 : {
				double b;	//�������뻡��b
				angle a; //��������ȷ���ṹa
				printf("------------��ѡ��-----------\n\n");
				printf("\t1.���벢�ȷ���ת����\n");
				printf("\t2.���벢����ת�ȷ���\n\n");
				printf("-----------------------------\n");
				getchar();
				switch(getchar()) {
					case '1': printf("�ȷ���ת����:	(�磺 180,0,0 )\n"); 
					scanf("%d,%d,%lf",&a.deg,&a.min,&a.sec); outputRadian(angle2radian(a)); break;	//���벢�ȷ���ת���ȣ�aΪʵ��
					case '2': printf("����ת�ȷ���:	(�磺 3.141592 )\n"); 
					scanf("%lf",&b); outputAngle(radian2angle(b));  break;		//���벢����ת�ȷ��룬bΪʵ��
					default : printf("�������\n"); 
				}
				printf("---------------------------------------\n");
				printf("   �����ѡ���ܣ�(����-1�˳�����)   )\n");
				printf("---------------------------------------\n");
				scanf("%d",&chosenNum);
				break;
			}
	//	���ܣ����귴�㣬����һ�������뵽��һ��ķ�λ�Ǻ;��������һ������
		case 2 : {
				Point a1,b1;
				FILE *fpw;
				fpw = fopen("data1.txt", "w");
				if(fpw) {
					double dDirection, dDistance; //dDirection ��λ�ǣ�dDirection ����
					printf("�����ID�������꣬�����꣬��λ��(����)��������룺(�磺 0,0,3.141592,10)\n");
					scanf("%d,%lf,%lf,%lf,%lf",&a1.ID,&a1.x, &a1.y, &dDirection, &dDistance);
					b1 = get_b(a1, dDirection, dDistance);
					displayPoint(b1);
				}
				fclose(fpw);
				system("pause");
				printf("   �����ѡ���ܣ�(����-1�˳�����)   )\n");
				printf("---------------------------------------\n");
				scanf("%d",&chosenNum);
				break;
			}
	//	���ܣ� ��������ʵ�ֲ��롢ɾ������ѯ����
		case 3 : {
				FILE *fpw;
				char fname[20];
				int goon = 1, createstyle;
				int n = 0, loc, delID, searchID, index;
				pList phead, pInsert, pDelete;
				pNode *Point = NULL;
				printf("����������\t0�����أ�1�����ļ�������2�����ݴ�С����\n"); 
				scanf("%d",&createstyle);
				while(createstyle) {
					if(createstyle == 1) {
						char filename[20];
						FILE *fp;
						printf("��������Ҫ�򿪵ĵ��ļ���·����:");
						scanf("%s",filename);
						if((fp=fopen(filename, "r")) == NULL) {
							printf("δ�ܴ��ļ���");
							return 0;
						}
						phead = createPointList(fp);
						break;
					}
					else if(createstyle == 2) {
						printf("��������Ҫ���������С�����磺10��"); 
						scanf("%d",&n);
						phead = creatNList(n);
						break;
					}
					else {
						printf("������ѡ��\n");
						printf("����������\t0�����أ�1�����ļ�������2�����ݴ�С����\n"); 
						scanf("%d",&createstyle);
					}
				}
				if(createstyle == 0) {
					system("pause");
					printf("   �����ѡ���ܣ�(����-1�˳�����)   )\n");
					printf("---------------------------------------\n");
					scanf("%d",&chosenNum);
					break;
				}
				if(phead == NULL) {
					printf("����Ϊ�գ�\n");
					system("pause");
					printf("---------------------------------------\n");
					printf("   �����ѡ���ܣ�(����-1�˳�����)   )\n");
					printf("---------------------------------------\n");
					scanf("%d",&chosenNum);
				}
				else {
					disPointList(phead);
					printf("����0������һ��������1����������\n");
					scanf("%d",&goon);
					while(goon) {
						printf("------------��ѡ��-----------\n\n");
						printf("\t1.���������\n");
						printf("\t2.ɾ��������\n");
						printf("\t3.��ѯ������\n");
						printf("\t4.���������\n\n");
						printf("-----------------------------\n");
						printf("��ѡ�������ţ�\n");
						scanf("%d",&index);
						switch(index) {
							case 1 : {
								printf("��������ݣ�\n"); 
								printf("��������Ҫ����ĵ����ݣ�(�磺1,1.0,1.0,1.0,3)\n"); 
								Point = (pList)malloc(sizeof(pNode));
								scanf("%d,%lf,%lf,%lf,%d",&Point ->p.ID, &Point ->p.x, &Point ->p.y, &Point ->p.h, &loc);
								pInsert = (pList)malloc(sizeof(pNode));
								pInsert = insertPoint(phead, Point, loc);
								if(pInsert != NULL) 
									disPointList(pInsert);
								printf("�Ƿ����ʹ�ø�������������������ǣ�1\t��0\n");
								scanf("%d",&goon);
								break;
							}
							case 2 : {
								printf("ɾ�������ݣ�\n"); 
								printf("��������Ҫɾ���Ľڵ�ID�����磺3��\n"); 
								scanf("%d",&delID); 
								pDelete = (pList)malloc(sizeof(pNode));
								pDelete = deletePoint(phead, delID);
								if(pDelete != NULL) 
									disPointList(pDelete);
								printf("�Ƿ����ʹ�ø�������������������ǣ�1\t��0\n");
								scanf("%d",&goon);
								break;
							}
							case 3 : {
								printf("��ѯ�����ݣ�\n"); 
								printf("��������Ҫ��ѯ�Ľڵ�ID�����磺3��\n"); 
								scanf("%d",&searchID); 
								searchPoint(phead, searchID);
								printf("�Ƿ����ʹ�ø�������������������ǣ�1\t��0\n");
								scanf("%d",&goon);
								break;
							}
							case 4 : {
								printf("��������ݣ�\n"); 
								printf("��������Ҫ�����ļ�����·������");
								scanf("%s",fname);
								if((fpw=fopen(fname, "w")) == NULL) {
									printf("δ���ļ���\n");
									return 0;
								}
								else savePointList(phead, fpw);
								fclose(fpw);
								break;
							}
							default : 
								printf("������ѡ���ܣ�\n");
						}
					}
				}
				printf("---------------------------------------\n");
				printf("   �����ѡ���ܣ�(����-1�˳�����)   )\n");
				printf("---------------------------------------\n");
				scanf("%d",&chosenNum);
				break;
			}
	//  ����:  ���ļ���������ʵ�ֲ��롢ɾ������ѯ����
		case 4 : {
				int n = 0, t;
				unsigned int delLineLID;
				char rfilename[20], wfilename[20];
				int goon, index, lloc, ploc, alloc, delLID, delID, searchLID, searchID;
			    FILE *fpr,*fpw;			
			    EntityList eList,eInsert,eDelete, addedLine, deledLine;
			    pList pl;
			    printf("������Ҫ�������������ͣ�0.��\t1.��\n");
			   	scanf("%d",&t);
			   	if(t == Line) 
			    	printf("��������Ҫ�������ļ��������磺C:\\line.txt��\n");
			    else
			    	printf("��������Ҫ�������ļ��������磺C:\\line.txt��\n");
			    scanf("%s",rfilename);
			    if((fpr=fopen(rfilename,"r"))==NULL) {
			    	printf("δ�ܴ򿪴��ļ�!");
			    	return 0;
			    }
				else {
					eList=creatEntityList(fpr);
					disEntityList(eList);
					fclose(fpr);
				}
				printf("����0������һ��������1����������\n");
				scanf("%d",&goon);
				while(goon) {
					printf("------------��ѡ��-----------\n\n");
					printf("\t1.��������������\n");
					printf("\t2.������ɾ��������\n");
					printf("\t3.�������ѯ������\n");
					printf("\t4.���������������\n");
					printf("\t5.������ɾ��������\n");
					printf("\t6.�����������ļ�\n\n");
					printf("-----------------------------\n");
					printf("��ѡ�������ţ�\n");
					scanf("%d",&index);
					switch(index) {
					case 1 :
						printf("�������������ݣ�\n"); 
						printf("��������Ҫ����ĵ����ݼ�Ҫ������λ�á���λ�ã�(�磺1,1.0,1.0,1.0,1,3)\n"); 
						pl = (pList)malloc(sizeof(pNode));
						scanf("%d,%lf,%lf,%lf,%u,%d",&pl->p.ID, &pl->p.x, &pl->p.y, &pl->p.h, &lloc, &ploc);
						eInsert = (EntityList)malloc(sizeof(Entity));
						eInsert = AddPointInEntity(eList, pl, lloc, ploc);
						if(eInsert ->enext != NULL) {
							disEntityList(eInsert);
						}
						printf("�Ƿ����ʹ�ø�������������������ǣ�1\t��0\n");
						scanf("%d",&goon);
						break;

					case 2 :
					  	printf("������ɾ�������ݣ�\n"); 
						printf("��������Ҫɾ�����߽ڵ�LID�͵�ڵ�ID�����磺3��3��\n");
					  	scanf("%u,%d",&delLID,&delID); 
						eDelete = (EntityList)malloc(sizeof(Entity));
					 	eDelete = DelPointInEntity(eList, delLID, delID);
					 	if(eDelete ->enext != NULL) {
					 		disEntityList(eDelete);
					 	}
						printf("�Ƿ����ʹ�ø�������������������ǣ�1\t��0\n");
					 	scanf("%d",&goon);
					 	break;

					case 3 :
					 	printf("�������ѯ�����ݣ�\n"); 
						printf("��������Ҫ��ѯ�Ľڵ�ID�����磺1,1��\n");
						scanf("%u,%d",&searchLID, &searchID); 
					 	SrcPointInEntity(eList, searchLID, searchID);
					 	printf("�Ƿ����ʹ�ø�������������������ǣ�1\t��0\n");
					 	scanf("%d",&goon);
					 	break;

					case 4 :
						printf("��������������ݣ�\n");
						printf("��������Ҫ�����������LID��Num��λ�ã�\n");
						addedLine = (EntityList)malloc(sizeof(Entity));
						scanf("%u,%d,%d",&addedLine ->LID, &addedLine ->Num, &alloc);
						n = addedLine ->Num;
						printf("��Ҫ�����������Ϊ��\n");
						addedLine ->pElement = creatNList(n);
						addedLine = AddLine(eList, addedLine, alloc);
						if(addedLine ->enext != NULL) 
							disEntityList(addedLine);
						printf("�Ƿ����ʹ�ø�������������������ǣ�1\t��0\n");
					 	scanf("%d",&goon);
					 	break;

					case 5 :
						printf("������ɾ�������ݣ�\n");
						printf("��������Ҫɾ������LID��");
						scanf("%u",&delLineLID);
						deledLine = (EntityList)malloc(sizeof(Entity));
						deledLine = DelLine(eList, delLineLID);
						if(deledLine ->enext != NULL) 
							disEntityList(deledLine);
						printf("�Ƿ����ʹ�ø�������������������ǣ�1\t��0\n");
					 	scanf("%d",&goon);
					 	break;

					case 6 :
						printf("�����������ļ�\n"); 
						printf("��������Ҫ������ļ�����·������");
						scanf("%s",wfilename);
						if((fpw=fopen(wfilename, "w")) == NULL) {
							printf("���ܴ򿪴��ļ���\n");
							return 0;
						}
						else
							saveEntityList(eList, fpw);
						fclose(fpw);
						printf("�Ƿ����ʹ�ø�������������������ǣ�1\t��0\n");
					 	scanf("%d",&goon);
						break;
					default :
						printf("������ѡ���ܣ�\n");
					
					}
					printf("---------------------------------------\n");
					printf("   �����ѡ���ܣ�(����-1�˳�����)   )\n");
					printf("---------------------------------------\n");
					scanf("%d",&chosenNum);
					break;
				}
			}
	//	���ܣ� ͼ�λ���
		case 5 : {
			initgraph(screenWidth, screenHeight); // ��ʼ��
			//BLACK,GREEN,CYAN,RED,MAGENTA,BROWN,LIGHTGRAY,DARKGRAY,LIGHTBLUE,LIGHTGREEN,LIGHTCYAN,LIGHTRED,LIGHTMAGENTA,YELLOW,WHITE
			int control = 1, index;
			while(control) {
				printf("------------��ѡ��-----------\n\n");
				printf("\t1.���Ƶ�\n");
				printf("\t3.�����߻���\n\n");
				printf("-----------------------------\n");
				printf("��ѡ�������ţ�\n");
				scanf("%d",&index);
				switch(index) {
					case 1 : {
						scrPoint scrp;
						printf("���������꣺��100��30��\n");
						scanf("%d,%d",&scrp.scrX,&scrp.scrY);
						putpixel(scrp.scrX,scrp.scrY,BLUE);
						getch();
						break;
					}
					case 2 : {
						FILE *fpr;
						char rfilename[20];
						pList region;
						Point pCenter;
						int choose = 1, ispoly, size, scale;
						printf("��ѡ�񴴽������������ļ���1.���ļ�   2.�½�����\n");
						scanf("%d",&choose);
						while(choose) {
							if(choose == 1) {
							    printf("�������ļ��������磺D:\\region.txt��\n");
							    scanf("%s",rfilename);
							    if((fpr=fopen(rfilename,"r"))==NULL) {
							    	printf("δ�ܴ򿪴��ļ�!");
							    	return 0;
							    }
								else {
									region = createPointList(fpr);
									if(isRegion(region)) {
										ispoly = 1;
										disPointList(region);
										fclose(fpr);
										break;
									}
									else printf("���ļ����Ǹ��棡\n");
									ispoly = 0;
									break;
								}
							}
							else if(choose == 2) {
								printf("��������������Ķ�����: ");
								scanf("%d",&size);
								region = creatNList(size);
								disPointList(region);
								break;
							}
							printf("���������룡\n");
							printf("��ѡ�񴴽��������ļ���1.���ļ�   2.�½�����\n");
							scanf("%d",&choose);
						}
						pCenter = getCenter(region);
						printf("�������ͼ�����ߣ�(ע��:�������������½���������½��������10�ȣ���������ô���1:60, \n");
						printf("��֮�����С��1��300�����򳬳���Ļ��Χ��)\t1  ��  ");
						scanf("%d",&scale);
						xyprintf(10,450,"�����ߣ�1:%d",scale);
						if(ispoly == 1) {
							drawRegion(region, scale, pCenter);
							getch(); // ��ͣһ�µȴ��û�����
						}
						else {
							drawLine(region, scale, pCenter);
							getch();
						}
						break;
					}
					default : {
						printf("������ѡ���ܣ�\n");
					}
				}
				printf("�Ƿ�������������������ǣ�1\t��0\n");
			 	scanf("%d",&control);	
			}
			closegraph(); // �ر�ͼ�ν���
			printf("---------------------------------------\n");
			printf("   �����ѡ���ܣ�(����-1�˳�����)   )\n");
			printf("---------------------------------------\n");
			scanf("%d",&chosenNum);
			break;
			}
	//	���ܣ� �������ϵ
		case 6 : {
			Point p, p1, p2;
			int relation;
			double distance;
			double minDis;
			int control = 1, index;
			while(control) {
				printf("------------��ѡ��-----------\n\n");
				printf("\t1.����ֱ�ߵĹ�ϵ���������̾���\n");
				printf("\t2.�����߶εĹ�ϵ���������̾���\n");
				printf("\t3.������Ĺ�ϵ���������̺������\n\n");
				printf("-----------------------------\n");
				printf("��ѡ�������ţ�\n");
				scanf("%d",&index);
				switch(index) {
					case 1 : {
						printf("����������꣺��1.0��1.0��\n");
						scanf("%lf,%lf",&p.x,&p.y);
						printf("������ֱ���������꣺(��2.0��2.0��3.0��4.0)\n");
						scanf("%lf,%lf,%lf,%lf",&p1.x,&p1.y,&p2.x,&p2.y);
						relation = pointOffLine(p, p1, p2);
						distance = getPointToLineDistance(p, p1, p2);
						if(relation == onLine) {
							printf("����ֱ���ϣ�\n");
							printf("�㵽��ֱ�ߵ���̾���Ϊ��%lf\n", distance);
						}
						else {
							printf("����ֱ���⣡\n");
							printf("�㵽��ֱ�ߵ���̾���Ϊ��%lf\n", distance);
						}
						printf("�Ƿ�������������������ǣ�1\t��0\n");
					 	scanf("%d",&control);
					 	break;
					}
					case 2 : {
						printf("����������꣺����1.0��1.0��\n");
						scanf("%lf,%lf",&p.x,&p.y);
						printf("�������߶��������꣺(��2.0��2.0��3.0��4.0)\n");
						scanf("%lf,%lf,%lf,%lf",&p1.x,&p1.y,&p2.x,&p2.y);
						relation = pointOffLineSeg(p, p1, p2);
						distance = getPointToLineSegDistance(p, p1, p2);
						if(relation == onLineSeg) {
							printf("�����߶��ϣ�\n");
							printf("�㵽���߶ε���̾���Ϊ��%lf\n", distance);
						}
						else if(relation == onExLineSeg) {
							printf("�����߶��ӳ����ϣ�\n");
							printf("�㵽���߶ε���̾���Ϊ��%lf\n", distance);
						}
						else {
							printf("�����߶��⣡\n");
							printf("�㵽���߶ε���̾���Ϊ��%lf\n", distance);
						}
						printf("�Ƿ�������������������ǣ�1\t��0\n");
					 	scanf("%d",&control);
						break;
					}
					case 3 : {
						FILE *fpr;
						char rfilename[20];
						pList region;
						int choose = 1;
						int size;
						printf("��ѡ�񴴽��������ļ���1.�����ļ�   2.������\n");
						scanf("%d",&choose);
						while(choose) {
							if(choose == 1) {
							    printf("���������ļ��������磺D:\\region.txt��\n");
							    scanf("%s",rfilename);
							    if((fpr=fopen(rfilename,"r"))==NULL) {
							    	printf("δ�ܴ򿪴��ļ�!");
							    	return 0;
							    }
								else {
									region = createPointList(fpr);
									if(isRegion(region)) {
										disPointList(region);
										fclose(fpr);
										break;
									}
									else printf("���ļ����Ǹ��棡\n");
								}
							}
							else if(choose == 2) {
								printf("��������������Ķ�����: ");
								scanf("%d",&size);
								region = creatNList(size);
								disPointList(region);
								break;
							}
							printf("���������룡\n");
							printf("��ѡ�񴴽��������ļ���1.�����ļ�   2.������\n");
							scanf("%d",&choose);
						}
						printf("����������꣺��1.0��1.0��\n");
						scanf("%lf,%lf",&p.x,&p.y);
						relation = regionContainsPoint(region, p);
						minDis = getMinPointToRegionDistance(p, region);
						if(relation == onRegion) {
							printf("�������ϣ�\n");
							printf("��̾��룺%lf\n", minDis);
						}
						else if(relation == inRegion) {
							printf("�������ڣ�\n");
							printf("��̾��룺%lf\n", minDis);
						}
						else {
							printf("�������⣡\n");
							printf("��̾��룺%lf\n", minDis);
						}
						printf("�Ƿ�������������������ǣ�1\t��0\n");
					 	scanf("%d",&control);
						break;
					}
					default : {
						printf("������ѡ���ܣ�\n");
					}
				}
			}
			printf("---------------------------------------\n");
			printf("   �����ѡ���ܣ�(����-1�˳�����)   )\n");
			printf("---------------------------------------\n");
			scanf("%d",&chosenNum);
			break;
		}
		default: {
				printf("��������!\n");
				printf("---------------------------------------\n");
				printf("   �����ѡ���ܣ�(����-1�˳�����)   )\n");
				printf("---------------------------------------\n");
				scanf("%d",&chosenNum);
			}
		}
	}
	printf("�������˳���\n");
	exit(0);
	return 0;
}

void drawLine(pList region, int scale, Point pCenter) {
	pList regionTemp;
	Point p1, p2;
	scrPoint scrP1, scrP2;
	regionTemp = region ->next;
	while(regionTemp ->next!= NULL) {
		p1 = regionTemp ->p;
		p2 = regionTemp ->next ->p;
		scrP1 = geoConvertToScreen(p1, scale, pCenter);
		scrP2 = geoConvertToScreen(p2, scale, pCenter);
// printf("%d,%d\n", scrP1.scrX, scrP1.scrY);
// printf("%d,%d\n", scrP2.scrX, scrP2.scrY);
		line(scrP1.scrX, scrP1.scrY, scrP2.scrX, scrP2.scrY);
		regionTemp = regionTemp ->next;
	}
}

void drawRegion(pList region, int scale, Point pCenter) {
	pList regionTemp;
	Point p1, p2;
	scrPoint scrP1, scrP2;
	regionTemp = region ->next;
	while(regionTemp ->next!= NULL) {
		p1 = regionTemp ->p;
		p2 = regionTemp ->next ->p;
		scrP1 = geoConvertToScreen(p1, scale, pCenter);
		scrP2 = geoConvertToScreen(p2, scale, pCenter);
// printf("%d,%d\n", scrP1.scrX, scrP1.scrY);
// printf("%d,%d\n", scrP2.scrX, scrP2.scrY);
		line(scrP1.scrX, scrP1.scrY, scrP2.scrX, scrP2.scrY);
		regionTemp = regionTemp ->next;
	}
}



//��������תΪ��Ļ���꣬�����������꣬�ͱ�����
scrPoint geoConvertToScreen(Point p, int drawScale, Point pCenter) {
	scrPoint scrp;
	scrp.scrX = screenWidth/2 + (int)((p.x - pCenter.x)*drawScale + 0.5);	//��������
	scrp.scrY = screenHeight/2 - (int)((p.y - pCenter.y)*drawScale + 0.5);
	return scrp;
}

//�õ�ͼ���е����꣬���ȵ�����С��Ӿ��κ��������ؾ�����������
Point getCenter(pList pl) {
	Point p;
	p.x = getXMinBound(pl) + (getXMaxBound(pl) - getXMinBound(pl))/2;
	p.y = getYMinBound(pl) + (getYMaxBound(pl) - getYMinBound(pl))/2;
	return p;
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

//�õ��㵽��ľ���
double getPointToPointDistance(Point p1, Point p2) {
	return sqrt(pow(p2.x - p1.x,2.0) + pow(p2.y - p1.y,2.0));
}

//�õ��㵽�߶εľ���(�����߶������)
double getPointToLineSegDistance(Point p, Point p1, Point p2) {
	double leftP, rightP, leftRight;
	double l;
	leftP = getPointToPointDistance(p, p1);
	rightP = getPointToPointDistance(p, p2);
	leftRight = getPointToPointDistance(p1, p2);
	if(leftP < 0.000001) return 0.0;
	if(rightP < 0.000001) return 0.0;
	if(leftRight < 0.000001) {
		printf("�߶����˵��غϣ�\n");
		return leftP;
	}
	//right�۽�
	if(pow(leftP,2.0) > (pow(rightP,2.0) + pow(leftRight,2.0))) 
		return rightP;
	//left�۽�
	if(pow(rightP, 2.0) > (pow(leftP,2.0) + pow(leftRight,2.0))) 
		return leftP;
	//���
	l = (leftP + rightP + leftRight)/2.0;		//�ܳ�
	return 2.0*sqrt(l*(l-leftP)*(l-rightP)*(l-leftRight))/leftRight;	//���׹�ʽ
}

//�õ��㵽ֱ�ߵľ���
double getPointToLineDistance(Point p, Point p1, Point p2) {
	if(p1.x == p2.x && p1.y == p2.y) {
		printf("ֱ�������غϣ�\n");
		return 0.0;
	}
	else return fabs((p2.y - p1.y)*p.x + (p2.x - p1.x)*p.y + (p2.x*p1.y - p1.x*p2.y))
	/ sqrt(pow((p2.y - p1.y),2.0) + pow((p2.x - p1.x),2.0));
}

// �жϵ���ֱ�ߵĲ�λ��ϵ
int pointOffLine(Point p, Point p1, Point p2) {
	if(((p.x - p1.x) * (p2.y - p1.y) - (p.y - p1.y)*(p2.x - p1.x)) == 0) 
		return onLine;
	return outLine;
}

/*
�жϵ����߶ι�ϵ
��������:	1.���ߣ�(p-p1)��(p2 - p1) = 0
			2.p����p1��p2Ϊ�Խ��ߵľ�����
*/
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

//�õ����ߵ���СX�߽�
double getXMinBound(pList pl) {
	pList pltemp = pl ->next;
	double xtemp = pltemp ->p.x;
	while(pltemp ->next != NULL) {
		pltemp = pltemp ->next;
		xtemp = pltemp ->p.x > xtemp ? xtemp : pltemp ->p.x;
	}
	return xtemp;
}

//�õ����ߵ����X�߽�
double getXMaxBound(pList pl) {
	pList pltemp = pl ->next;
	double xtemp = pltemp ->p.x;
	while(pltemp ->next != NULL) {
		pltemp = pltemp ->next;
		xtemp = pltemp ->p.x < xtemp ? xtemp : pltemp ->p.x;
	}
	return xtemp;
}

//�õ����ߵ���СY�߽�
double getYMinBound(pList pl) {
	pList pltemp = pl ->next;
	double ytemp = pltemp ->p.y;
	while(pltemp ->next != NULL) {
		pltemp = pltemp ->next;
		ytemp = pltemp ->p.y > ytemp ? ytemp : pltemp ->p.y;
	}
	return ytemp;
}

//�õ����ߵ����Y�߽�
double getYMaxBound(pList pl) {
	pList pltemp = pl ->next;
	double ytemp = pltemp ->p.y;
	while(pltemp ->next != NULL) {
		pltemp = pltemp ->next;
		ytemp = pltemp ->p.y < ytemp ? ytemp : pltemp ->p.y;
	}
	return ytemp;
}

//ȡ��ֵ��Сֵ
double Min(double x, double y) {
	return x < y ? x : y;
}

//ȡ��ֵ���ֵ
double Max(double x, double y) {
	return x > y ? x : y;
}

double angle2radian(angle x) {	
	double a1;
	a1 = (x.deg+x.min/60.0+x.sec/3600.0)*PI/180;
	return a1;
}	

angle radian2angle(double y) {	
	angle b1;
	b1.deg = (int)(y*180.0/PI);
	b1.min = (int)(((y*180.0/PI)-b1.deg)*60.0);
	b1.sec = (double)(((((y*180.0/PI)-b1.deg)*60.0)-b1.min)*60.0);
	return b1;
}

void outputAngle(angle aout){
	printf("�ȷ���ֱ���:%d��%d��%.1f��\n",aout.deg,aout.min,aout.sec);
}

void outputRadian(double yout){
	printf("������:%lf\n",yout);
}

Point get_b(Point a1, double dDirection, double dDistance){
	double dx,dy;
	Point b1;
	dx = dDistance * cos(dDirection);
	dy = dDistance * sin(dDirection);
	b1.x = a1.x + dx;
	b1.y = a1.y + dy;
	return b1;
}

void displayPoint(Point pp){
	printf("��һ�������Ϊ:%d,%lf,%lf,%lf\n",pp.ID, pp.x, pp.y, pp.h);
}

//��ȡ�ļ����ݣ�����������
pList createPointList(FILE *fp) {
	pList head, end, current;
	head = (pList)malloc(sizeof(pNode));
	end = head;
	while(!feof(fp)) {
		if(head != NULL) {
			current = (pList)malloc(sizeof(pNode));
			if(current != NULL) {
				fscanf(fp, "%d,%lf,%lf,%lf", &current ->p.ID, &current ->p.x, &current ->p.y, &current ->p.h);
				end -> next = current;
				end = current;
			}
			else 
				printf("�ڴ�������");
		}
		else
			printf("�ڴ�������");
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
	if(head != NULL) {
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
	}
	else 
		printf("�ڴ������������ԣ�\n");
	end ->next = NULL;
	printf("�����Ѵ�����\n");
	return head;
}

//�õ�������ĳ���
int get_Length(pList plhead){
	pList pl = plhead;
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
	pList delp = plhead;
	pList temp = delp;
	int isExist;
	if(delp == NULL) {
		printf("����Ϊ�գ������´�������\n");
		return NULL;
	}
	for(; delp ->next != NULL; delp = delp ->next) {
		if(delp ->next ->p.ID == IDinfo) {
			isExist = 1;
			break;
		}
		else isExist = 0;	//�ж���Ҫɾ�����Ƿ����
	}
	if(isExist == 1) {
		temp = delp ->next;
		delp ->next = delp ->next ->next;
		free(temp);
		printf("�ڵ���ɾ����\n");
		return plhead;
	}
	else {
		printf("δ�ҵ���Ҫɾ�����ݣ�\n");
		return NULL;
	}
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
