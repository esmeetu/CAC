/*
 *功能控制语句
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