#include "pch.h"
#include "MyCCA.h"


double _Matrix::MaxValue(double* v, int num)
{
	double max;
	if (num == 1)
		return v[0];
	else
	{
		for (int i = 0; i < num - 1; i++)
			max = (v[i] > v[i + 1] ? v[i] : v[i + 1]);
	}
	return max;
}

_Matrix::_Matrix(int rs, int cs)
{
	rowsize = rs;
	colsize = cs;
	arr = new double* [rs];
	for (int i = 0; i < rs; i++)
	{
		arr[i] = new double[cs];
		for (int j = 0; j < colsize; j++)
			arr[i][j] = 0;
	}
}

_Matrix::_Matrix(int a)  //构造单位正函数
{
	rowsize = a;
	colsize = a;
	arr = new double* [a];
	for (int i = 0; i < a; i++)
	{
		arr[i] = new double[a];
		for (int j = 0; j < a; j++)
		{
			if (j != i)
				arr[i][j] = 0;
			else
				arr[i][j] = 1;
		}
	}
}

_Matrix::_Matrix(_Matrix& w)
{
	rowsize = w.getRow();
	colsize = w.getCol();
	arr = new double* [rowsize];
	for (int i = 0; i < rowsize; i++)
	{
		arr[i] = new double[colsize];
		for (int j = 0; j < colsize; j++)
			arr[i][j] = w.getValue(i, j);
	}
}

_Matrix::_Matrix(int rs, int cs, double** a)  //构造函数
{
	int i, j;
	rowsize = rs;
	colsize = cs;
	arr = new double* [rs];
	for (i = 0; i < rs; i++)
		arr[i] = new double[cs];
	for (i = 0; i < rs; i++)
		for (j = 0; j < cs; j++)
		{
			arr[i][j] = a[i][j];
		}
}

_Matrix::~_Matrix(void)   //析构函数
{
	for (int i = 0; i < rowsize; i++)
	{
		delete[] arr[i];
	}

	delete[]arr;
}

double _Matrix::getValue(int row, int col)  const
{
	return arr[row][col];
}

void _Matrix::putValue(double a, int row, int col)
{
	arr[row][col] = a;
}

int _Matrix::getRow()  const
{
	return rowsize;
}

int _Matrix::getCol()  const
{
	return colsize;
}

_Matrix _Matrix::operator=(const _Matrix& w)
{
	if ((rowsize != w.getRow()) || (colsize != w.getCol()))
	{
		//	cout<<"矩阵不一致，无法传值"<<endl;
	   //  ShowMessage("矩阵不一致，无法传值");
		return (*this);
	}
	for (int i = 0; i < rowsize; i++)
		for (int j = 0; j < colsize; j++)
			arr[i][j] = w.getValue(i, j);
	return (*this);
}

_Matrix  _Matrix::operator+(const _Matrix& w)
{
	_Matrix a(this->rowsize, this->colsize);
	if ((this->rowsize != w.getRow()) || (this->colsize != w.getCol()))
	{
		//cout<<"加法矩阵不一致"<<endl;
//                ShowMessage("加法矩阵不一致");
		return a;
	}
	for (int i = 0; i < this->rowsize; i++)
		for (int j = 0; j < this->colsize; j++)
			a.putValue(this->arr[i][j] + w.getValue(i, j), i, j);
	return a;
}

void _Matrix::operator+=(const _Matrix& w)
{
	//		_Matrix* a= new _Matrix(this->rowsize, this->colsize);
	if ((this->rowsize != w.getRow()) || (this->colsize != w.getCol()))
	{
		//                ShowMessage("加法矩阵不一致");
		return;
	}
	for (int i = 0; i < this->rowsize; i++)
		for (int j = 0; j < this->colsize; j++)
			this->arr[i][j] = this->arr[i][j] + w.getValue(i, j);
}

_Matrix  _Matrix::operator-(const _Matrix& w)
{
	_Matrix a(this->rowsize, this->colsize);
	if ((this->rowsize != w.getRow()) || (this->colsize != w.getCol()))
	{
		//		ShowMessage("减法矩阵不一致");
		return a;
	}
	for (int i = 0; i < this->rowsize; i++)
		for (int j = 0; j < this->colsize; j++)
			a.putValue(this->arr[i][j] - w.getValue(i, j), i, j);
	return a;
}

_Matrix _Matrix::operator*(const _Matrix& w)
{
	_Matrix m(this->rowsize, w.getCol());
	double a;

	if (this->colsize != w.getRow())
	{
		//		ShowMessage("矩阵无法相乘");
		return m;
	}

	for (int i = 0; i < this->rowsize; i++)
		for (int k = 0; k < w.getCol(); k++)
		{
			a = 0;
			for (int j = 0; j < this->colsize; j++)
				a = a + this->arr[i][j] * w.arr[j][k];
			//				m.arr[i][k]=a;
			m.putValue(a, i, k);
		}

	return m;
}

_Matrix _Matrix::operator*(const double a)
{
	_Matrix m(this->rowsize, this->colsize);

	for (int i = 0; i < this->rowsize; i++)
		for (int j = 0; j < this->colsize; j++)
		{
			m.arr[i][j] = arr[i][j] * a;
			//	m.putValue(a, i, k);
		}

	return m;
}

_Matrix _Matrix::TransposeMatrix()
{
	_Matrix transposeMatrix(colsize, rowsize);
	for (int i = 0; i < rowsize; i++)
		for (int j = 0; j < colsize; j++)
		{
			transposeMatrix.putValue(arr[i][j], j, i);
		}
	return transposeMatrix;
}

double _Matrix::MaxEigenValue()//雅库比迭代法
{
	if (this->rowsize != this->colsize)
	{
		////		ShowMessage("不是4阶矩阵");
				//printf("不是方正");
		return 0;
	}

	int i;
	int n = this->rowsize;

	double* v = new double[n];
	double* x = new double[n];

	for (i = 0; i < n; i++)
	{
		v[i] = 0;
		x[i] = 1;
	}

	double e = 1e-5;
	double delta = 1;
	double eigenvalue1 = 0;
	double eigenvalue2 = 1;

	while (delta >= e)
	{
		eigenvalue1 = eigenvalue2;
		for (i = 0; i < n; i++)
		{
			v[i] = 0;
			for (int j = 0; j < n; j++)
				v[i] += arr[i][j] * x[j];
		}
		eigenvalue2 = MaxValue(v, n);
		for (i = 0; i < n; i++)
			x[i] = v[i] / eigenvalue2;

		delta = fabs(eigenvalue2 - eigenvalue1);
	}

	return eigenvalue2;
}

_Matrix _Matrix::InverseMatrix()
{
	_Matrix inverseMatrix(rowsize, colsize);

	if (rowsize != colsize)
	{
		//printf("非单位矩阵");
		return inverseMatrix;
	}

	for (int i = 0; i < rowsize; i++)
		for (int j = 0; j < colsize; j++)
		{
			inverseMatrix.arr[i][j] = arr[i][j];
		}


	double d, p;
	int i, j, n, * is, * js, k;
	n = rowsize;
	is = new int[n];
	js = new int[n];
	for (k = 0; k <= n - 1; k++)
	{
		d = 0.000;
		for (i = k; i <= n - 1; i++)
		{
			for (j = k; j <= n - 1; j++)
			{
				p = fabs(inverseMatrix.arr[i][j]);
				if (p > d)
				{
					d = p;
					is[k] = i;
					js[k] = j;
				}
			}
		}
		if (1.0 == d + 1.0)
		{
			delete[]is;
			delete[]js;
			//printf ("矩阵求逆出错!");
			return inverseMatrix;
		}
		if (is[k] != k)
		{ /*行交换*/
			for (j = 0; j <= n - 1; j++)
			{
				p = inverseMatrix.arr[k][j];
				inverseMatrix.arr[k][j] = inverseMatrix.arr[is[k]][j];
				inverseMatrix.arr[is[k]][j] = p;
			}
		}
		if (js[k] != k)
		{ /*列交换*/
			for (i = 0; i <= n - 1; i++)
			{
				p = inverseMatrix.arr[i][k];
				inverseMatrix.arr[i][k] = inverseMatrix.arr[i][js[k]];
				inverseMatrix.arr[i][js[k]] = p;
			}
		}

		inverseMatrix.arr[k][k] = 1 / inverseMatrix.arr[k][k];

		for (j = 0; j <= n - 1; j++)
		{
			if (j != k)
			{
				inverseMatrix.arr[k][j] = inverseMatrix.arr[k][j] * inverseMatrix.arr[k][k];
			}
		}
		for (i = 0; i <= n - 1; i++)
		{
			if (i != k)
			{
				for (j = 0; j <= n - 1; j++)
				{
					if (j != k)
					{
						inverseMatrix.arr[i][j] = inverseMatrix.arr[i][j] - inverseMatrix.arr[i][k] * inverseMatrix.arr[k][j];
					}
				}
			}
		}
		for (i = 0; i <= n - 1; i++)
		{
			if (i != k)
			{
				inverseMatrix.arr[i][k] = -inverseMatrix.arr[i][k] * inverseMatrix.arr[k][k];
			}
		}
	}

	for (k = n - 1; k >= 0; k--)
	{
		if (js[k] != k)
		{
			for (j = 0; j <= n - 1; j++)
			{
				p = inverseMatrix.arr[k][j];
				inverseMatrix.arr[k][j] = inverseMatrix.arr[js[k]][j];
				inverseMatrix.arr[js[k]][j] = p;
			}
		}
		if (is[k] != k)
		{
			for (i = 0; i <= n - 1; i++)
			{
				p = inverseMatrix.arr[i][k];
				inverseMatrix.arr[i][k] = inverseMatrix.arr[i][is[k]];
				inverseMatrix.arr[i][is[k]] = p;
			}
		}
	}

	return inverseMatrix;
}

_Matrix _Matrix::Centralization(int flag)
{
	_Matrix m(rowsize, colsize);
	if (flag == 1)
	{
		double* a = new double[this->rowsize];
		for (int i = 0; i < rowsize; i++)
		{
			a[i] = 0.0;
			for (int j = 0; j < colsize; j++)
			{
				a[i] += arr[i][j];
			}
			a[i] = a[i] / colsize;
		}

		for (int i = 0; i < rowsize; i++)
		{
			for (int j = 0; j < colsize; j++)
			{
				m.arr[i][j] = arr[i][j] - a[i];
			}
		}
		delete[] a;
		return m;
	}
	if (flag == 2)
	{
		double* a = new double[this->colsize];
		for (int i = 0; i < colsize; i++)
		{
			for (int j = 0; j < rowsize; j++)
			{
				a[i] += arr[j][i];
			}
			a[i] = a[i] / rowsize;
		}

		for (int i = 0; i < rowsize; i++)
		{
			for (int j = 0; j < colsize; j++)
			{
				m.arr[i][j] = arr[i][j] - a[j];
			}
		}
		delete[] a;
		return m;
	}
	else
	{
		printf("参数无法识别");////其他参数不识别
	}
	return m;
}
_Matrix _Matrix::Covariance(int flag)
{
	if (flag == 1)
	{
		_Matrix SxxT(rowsize, rowsize);

		SxxT = (*this) * (this->TransposeMatrix()) * (1.0 / colsize);
		return SxxT;
	}
	if (flag == 2)
	{
		_Matrix SxTx(colsize, colsize);
		SxTx = (this->TransposeMatrix()) * (*this) * (1.0 / rowsize);
		return SxTx;
	}
}

_Matrix _Matrix::Covariance(_Matrix y_Matrix, int flag)
{
	if (flag == 1)
	{
		if (colsize != y_Matrix.colsize)

			printf("无法求协方差矩阵");
		else
		{
			_Matrix SxyT(rowsize, y_Matrix.rowsize);
			SxyT = (*this) * (y_Matrix.TransposeMatrix()) * (1.0 / colsize);
			return SxyT;
		}
	}
	if (flag == 2)
	{
		if (rowsize != y_Matrix.rowsize)
			printf("无法求协方差矩阵");
		else
		{
			_Matrix SxTy(colsize, y_Matrix.colsize);
			SxTy = (this->TransposeMatrix()) * y_Matrix * (1.0 / rowsize);
			return SxTy;
		}
	}
	else
	{
		printf("参数无法识别");
	}
	_Matrix nullMatrix(0, 0);
	return nullMatrix;
}

double MyCCA::CCA(double** X, double** Y, int p, int q, int n)//X 和Y分别为pxn,qxn维的样本矩阵，
												  // p,q 代表变量个数，n代表样本数;
{
	/*	double ** XX = new double *[p];
		for(int i=0; i<p; i++)
		{
			XX[i] = new double[n];
			for(int j =0; j<n; j++)
			{
				XX[i][j] = X[i][j];
			}
		}

		double ** YY = new double *[q];
		for(i=0; i<q; i++)
		{
			YY[i] = new double[n];
			for(int j =0; j<n; j++)
			{
				YY[i][j] = Y[i][j];
			}
		}*/

	_Matrix x(p, n, X);
	_Matrix y(q, n, Y);
	x = x.Centralization(1);
	y = y.Centralization(1);
	_Matrix Sxx(p, p);
	_Matrix Syy(q, q);
	_Matrix Sxy(p, q);
	_Matrix Syx(q, p);
	Sxx = x.Covariance(1);
	Syy = y.Covariance(1);
	Sxy = x.Covariance(y, 1);
	Syx = Sxy.TransposeMatrix();
	if (p <= q)
	{
		_Matrix R(p, p);
		R = Sxx.InverseMatrix() * Sxy * Syy.InverseMatrix() * Syx;
		return R.MaxEigenValue();
	}
	else
	{
		_Matrix R(q, q);
		R = Syy.InverseMatrix() * Syx * Sxx.InverseMatrix() * Sxy;
		return sqrt(R.MaxEigenValue());
	}
}

double MyCCA::CCACaculate(double DATA[8][mydatalength2], double REF[4][mydatalength2])
{
	double** data = new double* [8];

	for (int i = 0; i < 8; i++)
	{
		data[i] = new double[mydatalength2];
	}

	for (int i = 0; i < mydatalength2; i++)
		for (int j = 0; j < 8; j++)
		{
			//注意数据的结构，main函数用下面这个，MOVEP项目用下下面那个
			//data[j][i] = DATA[j * 1950 + i];
			data[j][i] = DATA[j][i];
		}

	double** ref = new double* [4];

	for (int i = 0; i < 4; i++)
	{
		ref[i] = new double[mydatalength2];
	}

	for (int i = 0; i < mydatalength2; i++)
		for (int j = 0; j < 4; j++)
		{
			ref[j][i] = REF[j][i];
		}

	double ans = CCA(data, ref, 8, 4, mydatalength2);

	delete[] data;

	delete[] ref;

	return ans;
}

MyCCA::MyCCA()
{
	double pi = 3.14159265358979323846;

	for (size_t i = 0; i < mydatalength2; i++)
	{
		//DATA:脑电数据的初始化
		DATA[0][i] = 0;
		DATA[1][i] = 0;
		DATA[2][i] = 0;
		DATA[3][i] = 0;
		DATA[4][i] = 0;
		DATA[5][i] = 0;
		DATA[6][i] = 0;
		DATA[7][i] = 0;
		//DATA[8][i] = 0;

		//参考信号一：4Hz
		REF1[0][i] = sin(4.000000000000000 * 2 * pi * (double)i / 1000.0);
		REF1[1][i] = cos(4.000000000000000 * 2 * pi * (double)i / 1000.0);
		REF1[2][i] = sin(8.000000000000000 * 2 * pi * (double)i / 1000.0);
		REF1[3][i] = cos(8.000000000000000 * 2 * pi * (double)i / 1000.0);

		//参考信号一：5.454Hz
		REF2[0][i] = sin( 5.454545454545455 * 2 * pi * (double)i / 1000.0);
		REF2[1][i] = cos( 5.454545454545455 * 2 * pi * (double)i / 1000.0);
		REF2[2][i] = sin(10.909090909090909 * 2 * pi * (double)i / 1000.0);
		REF2[3][i] = cos(10.909090909090909 * 2 * pi * (double)i / 1000.0);
		//参考信号二：6.667Hz
		REF3[0][i] = sin( 6.666666666666667 * 2 * pi * (double)i / 1000.0);
		REF3[1][i] = cos( 6.666666666666667 * 2 * pi * (double)i / 1000.0);
		REF3[2][i] = sin(13.333333333333333 * 2 * pi * (double)i / 1000.0);
		REF3[3][i] = cos(13.333333333333333 * 2 * pi * (double)i / 1000.0);
		//参考信号三:7.5Hz
		REF4[0][i] = sin( 7.500000000000000 * 2 * pi * (double)i / 1000.0);
		REF4[1][i] = cos( 7.500000000000000 * 2 * pi * (double)i / 1000.0);
		REF4[2][i] = sin(15.000000000000000 * 2 * pi * (double)i / 1000.0);
		REF4[3][i] = cos(15.000000000000000 * 2 * pi * (double)i / 1000.0);
		//参考信号四:8.5714Hz
		REF5[0][i] = sin( 8.571428571428571 * 2 * pi * (double)i / 1000.0);
		REF5[1][i] = cos( 8.571428571428571 * 2 * pi * (double)i / 1000.0);
		REF5[2][i] = sin(17.142857142857142 * 2 * pi * (double)i / 1000.0);
		REF5[3][i] = cos(17.142857142857142 * 2 * pi * (double)i / 1000.0);
		
		//参考信号六:12Hz
		REF7[0][i] = sin(12.000000000000000 * 2 * pi * (double)i / 1000.0);
		REF7[1][i] = cos(12.000000000000000 * 2 * pi * (double)i / 1000.0);
		REF7[2][i] = sin(24.000000000000000 * 2 * pi * (double)i / 1000.0);
		REF7[3][i] = cos(24.000000000000000 * 2 * pi * (double)i / 1000.0);

		//参考信号七:10Hz
		REF6[0][i] = sin(10.000000000000000 * 2 * pi * (double)i / 1000.0);
		REF6[1][i] = cos(10.000000000000000 * 2 * pi * (double)i / 1000.0);
		REF6[2][i] = sin(20.000000000000000 * 2 * pi * (double)i / 1000.0);
		REF6[3][i] = cos(20.000000000000000 * 2 * pi * (double)i / 1000.0);

		//参考信号八：4.6Hz
		REF8[0][i] = sin(4.629629629629629 * 2 * pi * (double)i / 1000.0);
		REF8[1][i] = cos(4.629629629629629 * 2 * pi * (double)i / 1000.0);
		REF8[2][i] = sin(9.259259259259259 * 2 * pi * (double)i / 1000.0);
		REF8[3][i] = cos(9.259259259259259 * 2 * pi * (double)i / 1000.0);

		////参考信号四:14Hz
		//REF4[i + 1950 * 0] = sin(14.000000000000000 * 2 * pi * (double)i / 1000.0);
		//REF4[i + 1950 * 1] = cos(14.000000000000000 * 2 * pi * (double)i / 1000.0);
		//REF4[i + 1950 * 2] = sin(28.000000000000000 * 2 * pi * (double)i / 1000.0);
		//REF4[i + 1950 * 3] = cos(28.000000000000000 * 2 * pi * (double)i / 1000.0);
		////参考信号五:10Hz
		//REF5[i + 1950 * 0] = sin(16.000000000000000 * 2 * pi * (double)i / 1000.0);
		//REF5[i + 1950 * 1] = cos(16.000000000000000 * 2 * pi * (double)i / 1000.0);
		//REF5[i + 1950 * 2] = sin(32.000000000000000 * 2 * pi * (double)i / 1000.0);
		//REF5[i + 1950 * 3] = cos(32.000000000000000 * 2 * pi * (double)i / 1000.0);
		////参考信号六:12Hz
		//REF6[i + 1950 * 0] = sin(18.000000000000000 * 2 * pi * (double)i / 1000.0);
		//REF6[i + 1950 * 1] = cos(18.000000000000000 * 2 * pi * (double)i / 1000.0);
		//REF6[i + 1950 * 2] = sin(36.000000000000000 * 2 * pi * (double)i / 1000.0);
		//REF6[i + 1950 * 3] = cos(36.000000000000000 * 2 * pi * (double)i / 1000.0);
	}
}

MyCCA::~MyCCA()
{
}

int MyCCA::getTheIndexOfCCA(double DATA[8][mydatalength2])  //把指令实时输出在
{
	int index = 0;

	double cca1 = CCACaculate(DATA, REF1);
	double cca2 = CCACaculate(DATA, REF2);
	double cca3 = CCACaculate(DATA, REF3);
	double cca4 = CCACaculate(DATA, REF4);
	double cca5 = CCACaculate(DATA, REF5);
	double cca6 = CCACaculate(DATA, REF6);
	double cca7 = CCACaculate(DATA, REF7);//12Hz
	double cca8 = CCACaculate(DATA, REF8);//2Hz

	//printf("%f %f %f %f %f %f %f\n", cca1, cca2, cca3, cca4, cca5, cca6, cca7,cca8);

	//设定一个CCA计算后的阈值，小于该阈值直接赋值为0，减少噪声和自发脑电的影响
	if (cca1 <= 0.15)
	{
		cca1 = 0.0;
	}
	if (cca2 <= 0.15)
	{
		cca2 = 0.0;
	}
	if (cca3 <= 0.15)
	{
		cca3 = 0.0;
	}
	if (cca4 <= 0.15)
	{
		cca4 = 0.0;
	}
	if (cca5 <= 0.15)
	{
		cca5 = 0.0;
	}
	if (cca6 <= 0.15)
	{
		cca6 = 0.0;
	}
	if (cca7 <= 0.15)
	{
		cca7 = 0.0;
	}
	if (cca8 <= 0.15)
	{
		cca8 = 0.0;
	}

	/*if ((0.0 == cca1) && (0.0 == cca2) && (0.0 == cca3) && (0.0 == cca4) && (0.0 == cca5) && (0.0 == cca6))
	{
		return index;
	}*/

	double theMax = fmax(cca1, fmax(cca2, fmax(cca3, fmax(cca4, fmax(cca5, fmax(cca6, fmax( cca7,cca8)))))));
	//double theMax = fmax(cca1, fmax(cca2, fmax(cca3, fmax(cca4, fmax(cca5, fmax(cca6,cca7))))));

	printf("%f\n", theMax);

	if (theMax == cca1)
	{
		index = 1;
		//myssvep.addcommnd(index);
		//return index;
	}
	if (theMax == cca2)
	{
		index = 2;
		//myssvep.addcommnd(index);
		//return index;
	}
	if (theMax == cca3)
	{
		index = 3;
		//myssvep.addcommnd(index);
		//return index;
	}
	if (theMax == cca4)
	{
		index = 4;
		//myssvep.addcommnd(index);
		//return index;
	}
	if (theMax == cca5)
	{
		index = 5;
		//myssvep.addcommnd(index);
		//return index;
	}
	if (theMax == cca6)
	{
		index = 6;
		//myssvep.addcommnd(index);
		//return index;
	}
	if (theMax == cca7)
	{
		index = 7;
		//myssvep.addcommnd(index);
		//return index;
	}
	if (theMax == cca8)
	{
		index = 8;
		//myssvep.addcommnd(index);
		//return index;
	}
	//myssvep.addcommnd(index);
	return index;
}

//int main()
//{
//	double DATA[15600];
//
//	double pi = 3.14159265358979323846;
//
//	for (size_t i = 0; i < 1950; i++)
//	{
//		DATA[i + 1950 * 0] = sin(11.000000000000000 * 2 * pi * i / 1000.0);
//		DATA[i + 1950 * 1] = sin(11.000000000000000 * 2 * pi * i / 1000.0);
//		DATA[i + 1950 * 2] = sin(11.000000000000000 * 2 * pi * i / 1000.0);
//		DATA[i + 1950 * 3] = sin(11.000000000000000 * 2 * pi * i / 1000.0);
//		DATA[i + 1950 * 4] = sin(11.000000000000000 * 2 * pi * i / 1000.0);
//		DATA[i + 1950 * 5] = sin(11.000000000000000 * 2 * pi * i / 1000.0);
//		DATA[i + 1950 * 6] = sin(11.000000000000000 * 2 * pi * i / 1000.0);
//		DATA[i + 1950 * 7] = sin(11.000000000000000 * 2 * pi * i / 1000.0);
//	}
//
//	MyCCA mycca;
//
//	printf("%d\n", mycca.getTheIndexOfCCA(DATA));
//
//	return 0;
//}