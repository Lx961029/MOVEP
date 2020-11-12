#pragma once

#ifndef _MYCCA_H_
#define _MYCCA_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include"CSSVEP.h"
#define mydatalength2 1950 //同上一个定义

class _Matrix			//矩阵类matrix
{
private:
	double** arr;   //数组指针
	int rowsize;        //数组元素行数
	int colsize;  //数组元素列数
	double MaxValue(double* v, int num);

public:
	_Matrix(int rs, int cs);  //构造函数
	_Matrix(int a);  //构造单位正函数
	_Matrix(_Matrix& w);
	_Matrix(int rs, int cs, double** a);  //构造函数
	~_Matrix(void);   //析构函数
	double getValue(int row, int col) const;
	void putValue(double a, int row, int col);
	int getRow() const;
	int getCol() const;
	_Matrix operator=(const _Matrix& w);
	_Matrix  operator+(const _Matrix& w);
	void operator+=(const _Matrix& w);
	_Matrix  operator-(const _Matrix& w);
	_Matrix operator*(const _Matrix& w);
	_Matrix operator*(const double a);
	_Matrix TransposeMatrix();
	double MaxEigenValue();//雅库比迭代法求特征值
	_Matrix InverseMatrix();//矩阵求逆——全选主元高斯-约当(Gauss-Jordan)法
	_Matrix Centralization(int flag);//中心化
	_Matrix Covariance(int flag);//自相关
	_Matrix Covariance(_Matrix y_Matrix, int flag);//互相关
};

class MyCCA
{
public:
	MyCCA();
	~MyCCA();
	//返回CCA计算后匹配的索引
	int getTheIndexOfCCA(double DATA[8][mydatalength2]);
	

private:
	//存放脑电数据
	double DATA[8][mydatalength2] = {0};
	//参考信号一
	double REF1[4][mydatalength2] = {0};
	//参考信号二
	double REF2[4][mydatalength2] = { 0 };
	//参考信号三
	double REF3[4][mydatalength2] = { 0 };
	//参考信号四
	double REF4[4][mydatalength2] = { 0 };
	//参考信号五
	double REF5[4][mydatalength2] = { 0 };
	//参考信号六
	double REF6[4][mydatalength2] = { 0 };
	//参考信号七
	double REF7[4][mydatalength2] = { 0 };
	//参考信号八
	double REF8[4][mydatalength2] = { 0 };

	double CCACaculate(double DATA[8][mydatalength2], double REF[4][mydatalength2]);

	double CCA(double** X, double** Y, int p, int q, int n);
	
	//CSSVEP myssvep;
};

#endif