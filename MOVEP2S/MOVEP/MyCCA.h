#pragma once

#ifndef _MYCCA_H_
#define _MYCCA_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include"CSSVEP.h"
#define mydatalength2 1950 //ͬ��һ������

class _Matrix			//������matrix
{
private:
	double** arr;   //����ָ��
	int rowsize;        //����Ԫ������
	int colsize;  //����Ԫ������
	double MaxValue(double* v, int num);

public:
	_Matrix(int rs, int cs);  //���캯��
	_Matrix(int a);  //���쵥λ������
	_Matrix(_Matrix& w);
	_Matrix(int rs, int cs, double** a);  //���캯��
	~_Matrix(void);   //��������
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
	double MaxEigenValue();//�ſ�ȵ�����������ֵ
	_Matrix InverseMatrix();//�������桪��ȫѡ��Ԫ��˹-Լ��(Gauss-Jordan)��
	_Matrix Centralization(int flag);//���Ļ�
	_Matrix Covariance(int flag);//�����
	_Matrix Covariance(_Matrix y_Matrix, int flag);//�����
};

class MyCCA
{
public:
	MyCCA();
	~MyCCA();
	//����CCA�����ƥ�������
	int getTheIndexOfCCA(double DATA[8][mydatalength2]);
	

private:
	//����Ե�����
	double DATA[8][mydatalength2] = {0};
	//�ο��ź�һ
	double REF1[4][mydatalength2] = {0};
	//�ο��źŶ�
	double REF2[4][mydatalength2] = { 0 };
	//�ο��ź���
	double REF3[4][mydatalength2] = { 0 };
	//�ο��ź���
	double REF4[4][mydatalength2] = { 0 };
	//�ο��ź���
	double REF5[4][mydatalength2] = { 0 };
	//�ο��ź���
	double REF6[4][mydatalength2] = { 0 };
	//�ο��ź���
	double REF7[4][mydatalength2] = { 0 };
	//�ο��źŰ�
	double REF8[4][mydatalength2] = { 0 };

	double CCACaculate(double DATA[8][mydatalength2], double REF[4][mydatalength2]);

	double CCA(double** X, double** Y, int p, int q, int n);
	
	//CSSVEP myssvep;
};

#endif