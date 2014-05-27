/*
 **************************************************数据结构部分************************************************
*/

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

//矩阵
typedef struct Matrix
{
	int row, col;
	double *data;
}Matrix;

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
	long biWidth;/*说明位图的宽度，单位为像素――biWidth' = ((int)((biWidth * 8 + 31)/32))*4  */
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