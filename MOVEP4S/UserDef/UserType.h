// UserType.h
//

#pragma once

#include "UserGdiplus.h"

// ��������
enum DATA_TYPE{
	DT_DOUBLE,	// double
	DT_USHORT	// unsigned short
};

// ����������
enum RULER_TYPE{
	RT_LINE = 0,	// ����
	RT_LOG,		// ����
	RT_COUNT
};
static CString STR_RULER_TYPE[] = {_T("����"), _T("����")};

typedef CArray<int> CIntArray;
typedef CArray<double> CDoubleArray;
typedef CArray<Point> PointArray;

// double���͵�2D����
typedef struct _COOR2D {
	double x;
	double y;
}COOR2D;
typedef CArray<COOR2D> CCoord2DArray;

// ����ŵ�double���͵�2D����
typedef struct _INDEX_COOR2D {
	int index;
	double x;
	double y;
}INDEX_COOR2D;
typedef CArray<INDEX_COOR2D> CIndexCoord2DArray;

// ��������ϵ
typedef struct _DRIVER_RELATION {
	int index;
	int r1;
	int r2;
	int r3;
}DRIVER_RELATION;
typedef CArray<DRIVER_RELATION> CDriverRelationArray;

// double���͵�3D����
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
