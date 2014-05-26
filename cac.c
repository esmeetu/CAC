#include <stdio.h>
#include <stdlib.h>
// #include <errno.h>

#include "DataStructure.h"
#include "ControlLog.h"
#include "Function.h"


// ������
int main() {
	int chosenNum;
	do {
		init_Main();
		scanf("%d",&chosenNum);
		switch(chosenNum) {
		//	���ܣ�1���ȷ���ת���ȣ� 2������ת�ȷ���
			case 1 : {
				double b;
				angle a;
				do {
					init_Control1();
					scanf("%d",&chosenNum);
					switch(chosenNum) {
						case 1 : 
							printf("�ȷ���ת����:	(�磺 180,0,0 )\n"); 
							scanf("%d,%d,%lf",&a.deg,&a.min,&a.sec); 
							outputRadian(angle2radian(a)); 
							break;
						case 2 : 
							printf("����ת�ȷ���:	(�磺 3.141592 )\n"); 
							scanf("%lf",&b);
							outputAngle(radian2angle(b));  
							break;
						case 0 :
							CASE1_STATUS = CLOSED;
							break;
						default : 
							printf("�������\n"); 
					}
				}while(CASE1_STATUS);
				break;
			}
		//	���ܣ����귴�㣬����һ�������뵽��һ��ķ�λ�Ǻ;��������һ������
			case 2 : {
				Point a1,b1;
				double dDirection, dDistance; //dDirection ��λ�ǣ�dDirection ����
				do {
					init_Control2();
					scanf("%d",&chosenNum);
					switch(chosenNum) {
						case 1 :
							printf("�����ID�������꣬�����꣬��λ��(����)��������룺(�磺 0,0,3.141592,10)\n");
							scanf("%d,%lf,%lf,%lf,%lf",&a1.ID,&a1.x, &a1.y, &dDirection, &dDistance);
							b1 = get_b(a1, dDirection, dDistance);
							displayPoint(b1);
							break;
						case 0 :
							CASE2_STATUS =  CLOSED;
							break;
						default :
							printf("�������\n"); 
					}
				}while(CASE2_STATUS);
				break;
			}
		//	���ܣ� ��������ʵ�ֲ��롢ɾ������ѯ����
			case 3 : {
				FILE *fpw;
				char fname[20];
				int createstyle;
				int n = 0, loc, delID, searchID;
				pList phead, pInsert, pDelete;
				pNode *Point = NULL;

				do {
					printf("����������\t0��������һ����1�����ļ�������2�����ݴ�С����\n"); 
					scanf("%d",&createstyle);
					if(createstyle == 0 ) break;
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
					else printf("������ѡ��\n");
				}while(createstyle);
				if(createstyle == 0 ) break;

				disPointList(phead);
				do {
					init_Control3();
					scanf("%d",&chosenNum);
					switch(chosenNum) {
						case 1 : 
							printf("��������ݣ�\n"); 
							printf("��������Ҫ����ĵ����ݣ�(�磺1,1.0,1.0,1.0,3)\n"); 
							Point = (pList)malloc(sizeof(pNode));
							scanf("%d,%lf,%lf,%lf,%d",&Point ->p.ID, &Point ->p.x, &Point ->p.y, &Point ->p.h, &loc);
							pInsert = (pList)malloc(sizeof(pNode));
							pInsert = insertPoint(phead, Point, loc);
							if(pInsert != NULL) 
								disPointList(pInsert);
							break;
						case 2 : 
							printf("ɾ�������ݣ�\n"); 
							printf("��������Ҫɾ���Ľڵ�ID�����磺3��\n"); 
							scanf("%d",&delID); 
							pDelete = (pList)malloc(sizeof(pNode));
							pDelete = deletePoint(phead, delID);
							if(pDelete != NULL) disPointList(pDelete);
							break;
						case 3 : 
							printf("��ѯ�����ݣ�\n"); 
							printf("��������Ҫ��ѯ�Ľڵ�ID�����磺3��\n"); 
							scanf("%d",&searchID); 
							searchPoint(phead, searchID);
							break;
						case 4 : 
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
						case 0 :
							CASE3_STATUS = CLOSED;
							break;
						default : 
							printf("������ѡ���ܣ�\n");
					}
				}while(CASE3_STATUS);
				break;
			}
		//  ����:  ���ļ���������ʵ�ֲ��롢ɾ������ѯ����
			case 4 : {
				int n = 0, t;
				unsigned int delLineLID;
				char rfilename[20], wfilename[20];
				int lloc, ploc, alloc, delLID, delID, searchLID, searchID;
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

				do {
					init_Control4();
					scanf("%d",&chosenNum);
					switch(chosenNum) {
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
						 	break;
						case 3 :
						 	printf("�������ѯ�����ݣ�\n"); 
							printf("��������Ҫ��ѯ�Ľڵ�ID�����磺1,1��\n");
							scanf("%u,%d",&searchLID, &searchID); 
						 	SrcPointInEntity(eList, searchLID, searchID);
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
						 	break;
						case 5 :
							printf("������ɾ�������ݣ�\n");
							printf("��������Ҫɾ������LID��");
							scanf("%u",&delLineLID);
							deledLine = (EntityList)malloc(sizeof(Entity));
							deledLine = DelLine(eList, delLineLID);
							if(deledLine ->enext != NULL) 
								disEntityList(deledLine);
						 	break;
						case 6 :
							printf("�����������ļ�\n"); 
							printf("��������Ҫ������ļ�����·������");
							scanf("%s",wfilename);
							if((fpw=fopen(wfilename, "w")) == NULL) {
								printf("���ܴ򿪴��ļ���\n");
								return 0;
							}
							else saveEntityList(eList, fpw);
							fclose(fpw);
							break;
						case 0 :
							CASE4_STATUS = CLOSED;
							break;
						default :
							printf("������ѡ���ܣ�\n");
					}
				}while(CASE4_STATUS);
				break;
			}
		//	���ܣ� ͼ�λ���
			case 5 : 
				do {
					init_Control5();
					scanf("%d",&chosenNum);
					switch(chosenNum) {
						case 1 : 
							scrPoint scrp;
							printf("���������꣺��100��30��\n");
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
								printf("��ѡ�񴴽������������ļ���1.���ļ�   2.�½�����\n");
								scanf("%d",&choose);
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
											disPointList(region);
											fclose(fpr);
											break;
										}
										else printf("���ļ����Ǹ��棡\n");
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
							}while(choose);
							pCenter = getCenter(region);
							drawEntity(region, pCenter); 
							break;
						}
						case 0 : 
							CASE5_STATUS = CLOSED;
							break;
						default : 
							printf("������ѡ���ܣ�\n");
					}
				}while(CASE5_STATUS);
				break;
		//	���ܣ� �������ϵ
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
							printf("����������꣺��1.0��1.0��\n");
							scanf("%lf,%lf",&p.x,&p.y);
							printf("������ֱ���������꣺(��2.0��2.0��3.0��4.0)\n");
							scanf("%lf,%lf,%lf,%lf",&p1.x,&p1.y,&p2.x,&p2.y);
							relation = pointOffLine(p, p1, p2);
							distance = getPointToLineDistance(p, p1, p2);
							if(relation == onLine) {
								printf("����ֱ���ϣ�,�㵽��ֱ�ߵ���̾���Ϊ��%lf\n", distance);
							}
							else {
								printf("����ֱ���⣡,�㵽��ֱ�ߵ���̾���Ϊ��%lf\n", distance);
							}
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
								printf("�����߶��ϣ�,�㵽���߶ε���̾���Ϊ��%lf\n", distance);
							}
							else if(relation == onExLineSeg) {
								printf("�����߶��ӳ����ϣ�,�㵽���߶ε���̾���Ϊ��%lf\n", distance);
							}
							else {
								printf("�����߶��⣡,�㵽���߶ε���̾���Ϊ��%lf\n", distance);
							}
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
										else printf("���ļ������棡\n");
									}
								}
								else if(choose == 2) {
									printf("��������������Ķ�����: ");
									scanf("%d",&size);
									region = creatNList(size);
									disPointList(region);
									break;
								}
								printf("���������룡\n��ѡ�񴴽��������ļ���1.�����ļ�   2.������\n");
								scanf("%d",&choose);
							}
							printf("����������꣺��1.0��1.0��\n");
							scanf("%lf,%lf",&p.x,&p.y);
							relation = regionContainsPoint(region, p);
							minDis = getMinPointToRegionDistance(p, region);
							if(relation == onRegion) {
								printf("�������ϣ�,��̾��룺%lf\n", minDis);
							}
							else if(relation == inRegion) {
								printf("�������ڣ�,��̾��룺%lf\n", minDis);
							}
							else {
								printf("�������⣡,��̾��룺%lf\n", minDis);
							}
							break;
						}
						case 0 :
							CASE6_STATUS = CLOSED;
							break;
						default : {
							printf("������ѡ���ܣ�\n");
						}
					}
				}while(CASE6_STATUS);
				break;
			}
		//	���ܣ���дBMPͼƬ
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
							printf("����bmp�ļ�����\n");
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
							
							printf("������Ҫ������ļ�����\n");
							scanf("%s",bmpName);
							fp = fopen(bmpName, "wb");	
							if(fp == NULL) return 0;	//δ���ļ�
							isSaved = saveBmp(fp, imgTemp, BmpWidth, BmpHeight, biBitCount, pColorTable);
							if(!isSaved) 
								printf("д��ʧ�ܣ�\n");
							else printf("��д��%s\n", bmpName);
							fclose(fp);
							break;
						case 0 : 
							CASE7_STATUS = CLOSED;
							break;
						default : 
							printf("������ѡ���ܣ�\n");
					}
				}while(CASE7_STATUS);
				break;
			}

		//	���ܣ�ʸ������ѹ��
			case 8 : {
				int n, createstyle;
				double d;
				pList pl;
				pList cjfList, pjfList, dgls_pkfList;

				do {
					printf("����������\t0��������һ����1�����ļ�������2�����ݴ�С����\n"); 
					scanf("%d",&createstyle);
					if(createstyle == 0 ) break;
					if(createstyle == 1) {
						char filename[20];
						FILE *fp;
						printf("��������Ҫ�򿪵ĵ��ļ���·����:");
						scanf("%s",filename);
						if((fp=fopen(filename, "r")) == NULL) {
							printf("δ�ܴ��ļ���");
							return 0;
						}
						pl = createPointList(fp);
						break;
					}
					else if(createstyle == 2) {
						printf("��������Ҫ���������С�����磺10��"); 
						scanf("%d",&n);
						pl = creatNList(n);
						break;
					}
					else printf("������ѡ��\n");
				}while(createstyle);
				if(createstyle == 0 ) break;

				disPointList(pl);

				do {
					init_Control8();
					scanf("%d",&chosenNum);
					switch(chosenNum) {
						case 1 :
							printf("���봹�෨��ֵ��\n");
							scanf("%lf", &d);
							cjfList = cjf(pl, d);
							printf("ѹ�������\n");
							disPointList(cjfList);
							break;
						case 2 :
							printf("����ƫ�Ƿ�����ֵ��\n");
							scanf("%lf", &d);
							pjfList = pjf(pl, d);
							printf("ѹ�������\n");
							disPointList(pjfList);
							break;
						case 3 :
							printf("���������˹�տ˷���ֵ��\n");
							scanf("%lf", &d);
							dgls_pkfList = dgls_pkf(pl, 0, get_Length(pl) - 1, d);
							printf("ѹ�������\n");
							disPointList(dgls_pkfList);
							break;
						case 0 :
							CASE8_STATUS = CLOSED;
							break;
						default : 
							printf("������ѡ���ܣ�\n");
					}
				}while(CASE8_STATUS);

				break;
			}
		//	���ܣ��˳�����
			case 0 :
				MAIN_STATUS = CLOSED;
				break;
			default: {
				printf("��������!\n");
			}
		}
	}while(MAIN_STATUS);
	exit(0);
	return 0;
}