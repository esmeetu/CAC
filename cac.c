#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <graphics.h>
#define screenWidth		640
#define screenHeight 	480
#define PI 3.141592

/*
 **************************************************数据结构部分************************************************
*/
typedef struct angle {
	int deg;
	int min;
	double sec;
}angle;	//点结构体

typedef struct Point {
	double x, y, h;
	int ID;
}Point; //点结构体

typedef struct pNode {
	Point p;
	struct pNode *next;
}pNode, *pList; //点节点结构体

enum Type { Line, Region};
enum Type t = Line;

typedef struct Entity {
	 unsigned int LID;
	 unsigned int Num;
	 enum Type type;
	 pList pElement;
	 struct Entity *enext;
}Entity, *EntityList; //线节点结构体

typedef struct scrPoint 
{
	int scrX;
	int scrY;
}scrPoint;		//屏幕坐标结构体

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
void savePointList(pList plhead, FILE *fp);

EntityList creatEntityList(FILE *p);	//读取线文件，创建线链表
EntityList AddPointInEntity(EntityList ehead, pList pl, unsigned int lloc, int ploc);	//向线链表中插入指定位置点
EntityList DelPointInEntity(EntityList ehead, unsigned LIDinfo, int IDinfo);	//线链表中删除指定点
void SrcPointInEntity(EntityList ehead, unsigned LIDinfo, int IDinfo);	//线链表中查询点
EntityList AddLine(EntityList ehead, Entity *pLine, int alloc);	//向线链表中添加线
EntityList DelLine(EntityList ehead, unsigned int LID);		//向线链表中删除线
void disEntityList(EntityList ehead);	//显示线链表
void saveEntityList(EntityList ehead, FILE *fp);	//保存线链表

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
 ***************************************函数定义部分***********************************************************
*/
// 主函数
int main() {
	int chosenNum = 0;
	printf("----------------------------------------\n\n");
	printf("\t1、	平面坐标与地理坐标转换\n");
	printf("\t2、	平面坐标正反算\n");
	printf("\t3、	点链表操作\n");
	printf("\t4、	线链表操作\n");
	printf("\t5、	图形绘制\n");
	printf("\t6、	点线面关系\n\n");
	printf("----------------------------------------\n");
	printf("---------------------------------------\n");
	printf("   请继续选择功能：(输入-1退出程序)   \n");
	printf("---------------------------------------\n");
	scanf("%d",&chosenNum);
	while(chosenNum != -1) {
		switch(chosenNum) {
	//	功能：1、度分秒转弧度； 2、弧度转度分秒
		case 1 : {
				double b;	//定义输入弧度b
				angle a; //定义输入度分秒结构a
				printf("------------请选择-----------\n\n");
				printf("\t1.输入并度分秒转弧度\n");
				printf("\t2.输入并弧度转度分秒\n\n");
				printf("-----------------------------\n");
				getchar();
				switch(getchar()) {
					case '1': printf("度分秒转弧度:	(如： 180,0,0 )\n"); 
					scanf("%d,%d,%lf",&a.deg,&a.min,&a.sec); outputRadian(angle2radian(a)); break;	//输入并度分秒转弧度，a为实参
					case '2': printf("弧度转度分秒:	(如： 3.141592 )\n"); 
					scanf("%lf",&b); outputAngle(radian2angle(b));  break;		//输入并弧度转度分秒，b为实参
					default : printf("输入错误！\n"); 
				}
				printf("---------------------------------------\n");
				printf("   请继续选择功能：(输入-1退出程序)   )\n");
				printf("---------------------------------------\n");
				scanf("%d",&chosenNum);
				break;
			}
	//	功能：坐标反算，根据一点坐标与到另一点的方位角和距离计算另一点坐标
		case 2 : {
				Point a1,b1;
				FILE *fpw;
				fpw = fopen("data1.txt", "w");
				if(fpw) {
					double dDirection, dDistance; //dDirection 方位角，dDirection 距离
					printf("输入点ID，横坐标，纵坐标，方位角(弧度)，两点距离：(如： 0,0,3.141592,10)\n");
					scanf("%d,%lf,%lf,%lf,%lf",&a1.ID,&a1.x, &a1.y, &dDirection, &dDistance);
					b1 = get_b(a1, dDirection, dDistance);
					displayPoint(b1);
				}
				fclose(fpw);
				system("pause");
				printf("   请继续选择功能：(输入-1退出程序)   )\n");
				printf("---------------------------------------\n");
				scanf("%d",&chosenNum);
				break;
			}
	//	功能： 创建链表，实现插入、删除、查询操作
		case 3 : {
				FILE *fpw;
				char fname[20];
				int goon = 1, createstyle;
				int n = 0, loc, delID, searchID, index;
				pList phead, pInsert, pDelete;
				pNode *Point = NULL;
				printf("创建新链表：\t0、返回，1、从文件创建，2、根据大小创建\n"); 
				scanf("%d",&createstyle);
				while(createstyle) {
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
					else {
						printf("请重新选择：\n");
						printf("创建新链表：\t0、返回，1、从文件创建，2、根据大小创建\n"); 
						scanf("%d",&createstyle);
					}
				}
				if(createstyle == 0) {
					system("pause");
					printf("   请继续选择功能：(输入-1退出程序)   )\n");
					printf("---------------------------------------\n");
					scanf("%d",&chosenNum);
					break;
				}
				if(phead == NULL) {
					printf("链表为空！\n");
					system("pause");
					printf("---------------------------------------\n");
					printf("   请继续选择功能：(输入-1退出程序)   )\n");
					printf("---------------------------------------\n");
					scanf("%d",&chosenNum);
				}
				else {
					disPointList(phead);
					printf("输入0返回上一级，输入1继续操作！\n");
					scanf("%d",&goon);
					while(goon) {
						printf("------------请选择-----------\n\n");
						printf("\t1.插入点数据\n");
						printf("\t2.删除点数据\n");
						printf("\t3.查询点数据\n");
						printf("\t4.保存点链表\n\n");
						printf("-----------------------------\n");
						printf("请选择操作编号：\n");
						scanf("%d",&index);
						switch(index) {
							case 1 : {
								printf("插入点数据：\n"); 
								printf("请输入所要插入的点数据：(如：1,1.0,1.0,1.0,3)\n"); 
								Point = (pList)malloc(sizeof(pNode));
								scanf("%d,%lf,%lf,%lf,%d",&Point ->p.ID, &Point ->p.x, &Point ->p.y, &Point ->p.h, &loc);
								pInsert = (pList)malloc(sizeof(pNode));
								pInsert = insertPoint(phead, Point, loc);
								if(pInsert != NULL) 
									disPointList(pInsert);
								printf("是否继续使用该链表进行其他操作？是：1\t否：0\n");
								scanf("%d",&goon);
								break;
							}
							case 2 : {
								printf("删除点数据：\n"); 
								printf("请输入所要删除的节点ID：（如：3）\n"); 
								scanf("%d",&delID); 
								pDelete = (pList)malloc(sizeof(pNode));
								pDelete = deletePoint(phead, delID);
								if(pDelete != NULL) 
									disPointList(pDelete);
								printf("是否继续使用该链表进行其他操作？是：1\t否：0\n");
								scanf("%d",&goon);
								break;
							}
							case 3 : {
								printf("查询点数据：\n"); 
								printf("请输入所要查询的节点ID：（如：3）\n"); 
								scanf("%d",&searchID); 
								searchPoint(phead, searchID);
								printf("是否继续使用该链表进行其他操作？是：1\t否：0\n");
								scanf("%d",&goon);
								break;
							}
							case 4 : {
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
							}
							default : 
								printf("请重新选择功能！\n");
						}
					}
				}
				printf("---------------------------------------\n");
				printf("   请继续选择功能：(输入-1退出程序)   )\n");
				printf("---------------------------------------\n");
				scanf("%d",&chosenNum);
				break;
			}
	//  功能:  从文件创建链表，实现插入、删除、查询操作
		case 4 : {
				int n = 0, t;
				unsigned int delLineLID;
				char rfilename[20], wfilename[20];
				int goon, index, lloc, ploc, alloc, delLID, delID, searchLID, searchID;
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
				printf("输入0返回上一级，输入1继续操作！\n");
				scanf("%d",&goon);
				while(goon) {
					printf("------------请选择-----------\n\n");
					printf("\t1.线链表插入点数据\n");
					printf("\t2.线链表删除点数据\n");
					printf("\t3.线链表查询点数据\n");
					printf("\t4.线链表插入线数据\n");
					printf("\t5.线链表删除线数据\n");
					printf("\t6.保存线链表文件\n\n");
					printf("-----------------------------\n");
					printf("请选择操作编号：\n");
					scanf("%d",&index);
					switch(index) {
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
						printf("是否继续使用该链表进行其他操作？是：1\t否：0\n");
						scanf("%d",&goon);
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
						printf("是否继续使用该链表进行其他操作？是：1\t否：0\n");
					 	scanf("%d",&goon);
					 	break;

					case 3 :
					 	printf("线链表查询点数据：\n"); 
						printf("请输入所要查询的节点ID：（如：1,1）\n");
						scanf("%u,%d",&searchLID, &searchID); 
					 	SrcPointInEntity(eList, searchLID, searchID);
					 	printf("是否继续使用该链表进行其他操作？是：1\t否：0\n");
					 	scanf("%d",&goon);
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
						printf("是否继续使用该链表进行其他操作？是：1\t否：0\n");
					 	scanf("%d",&goon);
					 	break;

					case 5 :
						printf("线链表删除线数据：\n");
						printf("请输入所要删除的线LID：");
						scanf("%u",&delLineLID);
						deledLine = (EntityList)malloc(sizeof(Entity));
						deledLine = DelLine(eList, delLineLID);
						if(deledLine ->enext != NULL) 
							disEntityList(deledLine);
						printf("是否继续使用该链表进行其他操作？是：1\t否：0\n");
					 	scanf("%d",&goon);
					 	break;

					case 6 :
						printf("保存线链表文件\n"); 
						printf("请输入所要保存的文件名及路径名：");
						scanf("%s",wfilename);
						if((fpw=fopen(wfilename, "w")) == NULL) {
							printf("不能打开此文件！\n");
							return 0;
						}
						else
							saveEntityList(eList, fpw);
						fclose(fpw);
						printf("是否继续使用该链表进行其他操作？是：1\t否：0\n");
					 	scanf("%d",&goon);
						break;
					default :
						printf("请重新选择功能！\n");
					
					}
					printf("---------------------------------------\n");
					printf("   请继续选择功能：(输入-1退出程序)   )\n");
					printf("---------------------------------------\n");
					scanf("%d",&chosenNum);
					break;
				}
			}
	//	功能： 图形绘制
		case 5 : {
			initgraph(screenWidth, screenHeight); // 初始化
			//BLACK,GREEN,CYAN,RED,MAGENTA,BROWN,LIGHTGRAY,DARKGRAY,LIGHTBLUE,LIGHTGREEN,LIGHTCYAN,LIGHTRED,LIGHTMAGENTA,YELLOW,WHITE
			int control = 1, index;
			while(control) {
				printf("------------请选择-----------\n\n");
				printf("\t1.绘制点\n");
				printf("\t3.绘制线或面\n\n");
				printf("-----------------------------\n");
				printf("请选择操作编号：\n");
				scanf("%d",&index);
				switch(index) {
					case 1 : {
						scrPoint scrp;
						printf("请输入坐标：（100，30）\n");
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
						printf("请选择创建线面或打开线面文件：1.打开文件   2.新建线面\n");
						scanf("%d",&choose);
						while(choose) {
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
										ispoly = 1;
										disPointList(region);
										fclose(fpr);
										break;
									}
									else printf("该文件不是个面！\n");
									ispoly = 0;
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
							printf("请选择创建面或打开面文件：1.打开文件   2.新建线面\n");
							scanf("%d",&choose);
						}
						pCenter = getCenter(region);
						printf("请输入绘图比例尺：(注意:若地理坐标左下角坐标距右下角坐标大于10度，比例尺最好大于1:60, \n");
						printf("反之，最好小于1：300，否则超出屏幕范围。)\t1  ：  ");
						scanf("%d",&scale);
						xyprintf(10,450,"比例尺：1:%d",scale);
						if(ispoly == 1) {
							drawRegion(region, scale, pCenter);
							getch(); // 暂停一下等待用户按键
						}
						else {
							drawLine(region, scale, pCenter);
							getch();
						}
						break;
					}
					default : {
						printf("请重新选择功能！\n");
					}
				}
				printf("是否继续进行其他操作？是：1\t否：0\n");
			 	scanf("%d",&control);	
			}
			closegraph(); // 关闭图形界面
			printf("---------------------------------------\n");
			printf("   请继续选择功能：(输入-1退出程序)   )\n");
			printf("---------------------------------------\n");
			scanf("%d",&chosenNum);
			break;
			}
	//	功能： 点线面关系
		case 6 : {
			Point p, p1, p2;
			int relation;
			double distance;
			double minDis;
			int control = 1, index;
			while(control) {
				printf("------------请选择-----------\n\n");
				printf("\t1.点与直线的关系，并输出最短距离\n");
				printf("\t2.点与线段的关系，并输出最短距离\n");
				printf("\t3.点与面的关系，并输出最短和最长距离\n\n");
				printf("-----------------------------\n");
				printf("请选择操作编号：\n");
				scanf("%d",&index);
				switch(index) {
					case 1 : {
						printf("请输入点坐标：（1.0，1.0）\n");
						scanf("%lf,%lf",&p.x,&p.y);
						printf("请输入直线两点坐标：(如2.0，2.0，3.0，4.0)\n");
						scanf("%lf,%lf,%lf,%lf",&p1.x,&p1.y,&p2.x,&p2.y);
						relation = pointOffLine(p, p1, p2);
						distance = getPointToLineDistance(p, p1, p2);
						if(relation == onLine) {
							printf("点在直线上！\n");
							printf("点到该直线的最短距离为：%lf\n", distance);
						}
						else {
							printf("点在直线外！\n");
							printf("点到该直线的最短距离为：%lf\n", distance);
						}
						printf("是否继续进行其他操作？是：1\t否：0\n");
					 	scanf("%d",&control);
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
							printf("点在线段上！\n");
							printf("点到该线段的最短距离为：%lf\n", distance);
						}
						else if(relation == onExLineSeg) {
							printf("点在线段延长线上！\n");
							printf("点到该线段的最短距离为：%lf\n", distance);
						}
						else {
							printf("点在线段外！\n");
							printf("点到该线段的最短距离为：%lf\n", distance);
						}
						printf("是否继续进行其他操作？是：1\t否：0\n");
					 	scanf("%d",&control);
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
									else printf("该文件不是个面！\n");
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
							printf("请选择创建面或打开面文件：1.打开面文件   2.创建面\n");
							scanf("%d",&choose);
						}
						printf("请输入点坐标：（1.0，1.0）\n");
						scanf("%lf,%lf",&p.x,&p.y);
						relation = regionContainsPoint(region, p);
						minDis = getMinPointToRegionDistance(p, region);
						if(relation == onRegion) {
							printf("点在面上！\n");
							printf("最短距离：%lf\n", minDis);
						}
						else if(relation == inRegion) {
							printf("点在面内！\n");
							printf("最短距离：%lf\n", minDis);
						}
						else {
							printf("点在面外！\n");
							printf("最短距离：%lf\n", minDis);
						}
						printf("是否继续进行其他操作？是：1\t否：0\n");
					 	scanf("%d",&control);
						break;
					}
					default : {
						printf("请重新选择功能！\n");
					}
				}
			}
			printf("---------------------------------------\n");
			printf("   请继续选择功能：(输入-1退出程序)   )\n");
			printf("---------------------------------------\n");
			scanf("%d",&chosenNum);
			break;
		}
		default: {
				printf("输入有误!\n");
				printf("---------------------------------------\n");
				printf("   请继续选择功能：(输入-1退出程序)   )\n");
				printf("---------------------------------------\n");
				scanf("%d",&chosenNum);
			}
		}
	}
	printf("程序已退出！\n");
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



//地理坐标转为屏幕坐标，输入地理点坐标，和比例尺
scrPoint geoConvertToScreen(Point p, int drawScale, Point pCenter) {
	scrPoint scrp;
	scrp.scrX = screenWidth/2 + (int)((p.x - pCenter.x)*drawScale + 0.5);	//四舍五入
	scrp.scrY = screenHeight/2 - (int)((p.y - pCenter.y)*drawScale + 0.5);
	return scrp;
}

//得到图形中点坐标，首先调用最小外接矩形函数，返回矩形中心坐标
Point getCenter(pList pl) {
	Point p;
	p.x = getXMinBound(pl) + (getXMaxBound(pl) - getXMinBound(pl))/2;
	p.y = getYMinBound(pl) + (getYMaxBound(pl) - getYMinBound(pl))/2;
	return p;
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

//得到点到点的距离
double getPointToPointDistance(Point p1, Point p2) {
	return sqrt(pow(p2.x - p1.x,2.0) + pow(p2.y - p1.y,2.0));
}

//得到点到线段的距离(点与线段最近点)
double getPointToLineSegDistance(Point p, Point p1, Point p2) {
	double leftP, rightP, leftRight;
	double l;
	leftP = getPointToPointDistance(p, p1);
	rightP = getPointToPointDistance(p, p2);
	leftRight = getPointToPointDistance(p1, p2);
	if(leftP < 0.000001) return 0.0;
	if(rightP < 0.000001) return 0.0;
	if(leftRight < 0.000001) {
		printf("线段两端点重合！\n");
		return leftP;
	}
	//right钝角
	if(pow(leftP,2.0) > (pow(rightP,2.0) + pow(leftRight,2.0))) 
		return rightP;
	//left钝角
	if(pow(rightP, 2.0) > (pow(leftP,2.0) + pow(leftRight,2.0))) 
		return leftP;
	//锐角
	l = (leftP + rightP + leftRight)/2.0;		//周长
	return 2.0*sqrt(l*(l-leftP)*(l-rightP)*(l-leftRight))/leftRight;	//海伦公式
}

//得到点到直线的距离
double getPointToLineDistance(Point p, Point p1, Point p2) {
	if(p1.x == p2.x && p1.y == p2.y) {
		printf("直线两点重合！\n");
		return 0.0;
	}
	else return fabs((p2.y - p1.y)*p.x + (p2.x - p1.x)*p.y + (p2.x*p1.y - p1.x*p2.y))
	/ sqrt(pow((p2.y - p1.y),2.0) + pow((p2.x - p1.x),2.0));
}

// 判断点与直线的侧位关系
int pointOffLine(Point p, Point p1, Point p2) {
	if(((p.x - p1.x) * (p2.y - p1.y) - (p.y - p1.y)*(p2.x - p1.x)) == 0) 
		return onLine;
	return outLine;
}

/*
判断点与线段关系
两个条件:	1.共线：(p-p1)×(p2 - p1) = 0
			2.p在以p1和p2为对角线的矩形内
*/
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

//得到多线的最小X边界
double getXMinBound(pList pl) {
	pList pltemp = pl ->next;
	double xtemp = pltemp ->p.x;
	while(pltemp ->next != NULL) {
		pltemp = pltemp ->next;
		xtemp = pltemp ->p.x > xtemp ? xtemp : pltemp ->p.x;
	}
	return xtemp;
}

//得到多线的最大X边界
double getXMaxBound(pList pl) {
	pList pltemp = pl ->next;
	double xtemp = pltemp ->p.x;
	while(pltemp ->next != NULL) {
		pltemp = pltemp ->next;
		xtemp = pltemp ->p.x < xtemp ? xtemp : pltemp ->p.x;
	}
	return xtemp;
}

//得到多线的最小Y边界
double getYMinBound(pList pl) {
	pList pltemp = pl ->next;
	double ytemp = pltemp ->p.y;
	while(pltemp ->next != NULL) {
		pltemp = pltemp ->next;
		ytemp = pltemp ->p.y > ytemp ? ytemp : pltemp ->p.y;
	}
	return ytemp;
}

//得到多线的最大Y边界
double getYMaxBound(pList pl) {
	pList pltemp = pl ->next;
	double ytemp = pltemp ->p.y;
	while(pltemp ->next != NULL) {
		pltemp = pltemp ->next;
		ytemp = pltemp ->p.y < ytemp ? ytemp : pltemp ->p.y;
	}
	return ytemp;
}

//取两值最小值
double Min(double x, double y) {
	return x < y ? x : y;
}

//取两值最大值
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
	printf("度分秒分别是:%d°%d′%.1f″\n",aout.deg,aout.min,aout.sec);
}

void outputRadian(double yout){
	printf("弧度是:%lf\n",yout);
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
	printf("另一点的坐标为:%d,%lf,%lf,%lf\n",pp.ID, pp.x, pp.y, pp.h);
}

//读取文件数据，并创建链表
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
				printf("内存分配错误！");
		}
		else
			printf("内存分配错误！");
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
	if(head != NULL) {
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
	}
	else 
		printf("内存分配错误，请重试！\n");
	end ->next = NULL;
	printf("链表已创建！\n");
	return head;
}

//得到点链表的长度
int get_Length(pList plhead){
	pList pl = plhead;
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
	pList delp = plhead;
	pList temp = delp;
	int isExist;
	if(delp == NULL) {
		printf("链表为空，请重新创建链表！\n");
		return NULL;
	}
	for(; delp ->next != NULL; delp = delp ->next) {
		if(delp ->next ->p.ID == IDinfo) {
			isExist = 1;
			break;
		}
		else isExist = 0;	//判断所要删除点是否存在
	}
	if(isExist == 1) {
		temp = delp ->next;
		delp ->next = delp ->next ->next;
		free(temp);
		printf("节点已删除！\n");
		return plhead;
	}
	else {
		printf("未找到所要删除数据！\n");
		return NULL;
	}
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
