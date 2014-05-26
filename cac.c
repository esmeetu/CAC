#include <stdio.h>
#include <stdlib.h>
// #include <errno.h>

#include "DataStructure.h"
#include "ControlLog.h"
#include "Function.h"


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