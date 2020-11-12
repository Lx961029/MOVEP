// UserType.h
//

#pragma once

#include "UserGdiplus.h"

// 数据类型
enum DATA_TYPE{
	DT_DOUBLE,	// double
	DT_USHORT	// unsigned short
};

// 坐标标尺类型
enum RULER_TYPE{
	RT_LINE = 0,	// 线性
	RT_LOG,		// 对数
	RT_COUNT
};
static CString STR_RULER_TYPE[] = {_T("线性"), _T("对数")};

typedef CArray<int> CIntArray;
typedef CArray<double> CDoubleArray;
typedef CArray<Point> PointArray;

// double类型的2D坐标
typedef struct _COOR2D {
	double x;
	double y;
}COOR2D;
typedef CArray<COOR2D> CCoord2DArray;

// 带编号的double类型的2D坐标
typedef struct _INDEX_COOR2D {
	int index;
	double x;
	double y;
}INDEX_COOR2D;
typedef CArray<INDEX_COOR2D> CIndexCoord2DArray;

// 驱动器联系
typedef struct _DRIVER_RELATION {
	int index;
	int r1;
	int r2;
	int r3;
}DRIVER_RELATION;
typedef CArray<DRIVER_RELATION> CDriverRelationArray;

// double类型的3D坐标
struct COOR3D {
	double x;
	double y;
	double z;

	COOR3D(){
		x = 0;
		y = 0;
		z = 0;
	}

	COOR3D(COOR3D& coor){
		x = coor.x;
		y = coor.y;
		z = coor.z;
	}

	COOR3D(double nx, double ny, double nz){
		x = nx;
		y = ny;
		z = nz;
	}
};
typedef CArray<COOR3D> CCoor3DArray;
