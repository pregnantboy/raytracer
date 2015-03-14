
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <vector>
#include <iostream>
#include <cassert>

#define PI 3.141592653589793


using namespace std;
float determinant(int n, float mat[4][4]);

class Vector
{
public:
	float x, y, z;
	Vector() : x(float (0)), y(float(0)), z(float(0)) {}
	Vector(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}
	Vector* normalize()
	{
		float length = (x * x )+ (y * y) + (z * z);
		float sqlength = sqrt(length);
		float xx = x / sqlength;
		float yy = y / sqlength;
		float zz = z / sqlength;
		return new Vector (xx,yy,zz);
	}
	Vector* operator * (float f) 
	{ 
		return new Vector(x * f, y * f, z * f); 
	}
	Vector* operator * (double f)
	{
		return new Vector(x * f, y * f, z * f);
	}
	float dot(Vector v)
	{ 
		return x * v.x + y * v.y + z * v.z; 
	}
	Vector* operator* (Vector v){
		Vector* result = new Vector(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
		return result;
	}
	Vector* operator - (Vector v) const 
	{ 
		return new Vector(x - v.x, y - v.y, z - v.z); 
	}
	
	Vector* operator + (Vector v) 
	{ 
		return  new Vector(x + v.x, y + v.y, z + v.z);
	}
	//v = (*v) + (*b);
	void print()
	{
		cout << "[" << x << "," << y << "," << z << "]"<<endl;
		return;
	}
	//(*v).print();
};

class Normal
{
public :
	float x, y, z;
	Normal(){
		x = 0;
		y = 0;
		z = 1;
	}
	Normal(Vector* v)
	{
		//normalize
		float magnitude = sqrt((v->x)*(v->x) + (v->y)*(v->y) + (v->z)*(v->z));
		x = (v->x) / magnitude;
		y = (v->y) / magnitude;
		z = (v->z) / magnitude;
	}
	Normal(float xx, float yy, float zz)
	{
		//normalize
		float magnitude = sqrt((xx)*(xx) + (yy)*(yy) + (zz)*(zz));
		x = (xx) / magnitude;
		y = (yy) / magnitude;
		z = (zz) / magnitude;
	}

	Normal* operator+ (Vector n)
	{
		x = x + n.x;
		y = y + n.y;
		z = z + n.z;
		return new Normal(x, y, z);
	}

	Normal* operator- (Vector n)
	{
		x = x - n.x;
		y = y - n.y;
		z = z - n.z;
		return new Normal(x, y, z);
	}
	void print(){
		cout << "Normal(" << x << "," << y << "," << z << ")"<<endl;
	}
};

class Point
{
public:
	float x, y, z;
	Point(){
		x = 0;
		y = 0;
		z = 0;
	}
	Point(float xx, float yy, float zz)
	{
		x = xx;
		y = yy;
		z = zz;
	}

	Vector* operator+ (Vector n)
	{
		x = x + n.x;
		y = y + n.y;
		z = z + n.z;
		return new Vector(x, y, z);
	}

	Vector operator- (Vector n)
	{
		x = x - n.x;
		y = y - n.y;
		z = z - n.z;
		return Vector(x, y, z);
	}
	Vector* operator- (Point n)
	{
		return new Vector(x - n.x, y - n.y, z - n.z);
	}

	/*Point* v = new Point(1, 2, 3);
	
	Point* b = new Point(3, 4, 5);
	cout << v->x << endl;
	Vector *line = (*v) - *b;
	(*line).print();*/
	void print(){
		cout << "(" << this->x << "," << this->y << "," << this->z << ")" << endl;
	}
};

class Ray
{
public:
	Point *pos;
	Vector *dir;
	float t_min, t_max;
	Ray(){
		pos = new Point();
		dir = new Vector();
		t_min = 0;
		t_max = INFINITY;
	}
	Ray(Point *ppos, Vector *ddir)
	{
		pos = ppos;
		dir = ddir;
		t_min = 0;
		t_max = INFINITY;
	}
	Ray(Point *ppos, Vector *ddir, float min, float max){
		pos = ppos;
		dir = ddir;
		t_min = min;
		t_max = max;
	}
	void print(){
		cout << "r(t) = " << "(" << pos->x << "," << pos->y <<","<<pos->z<< ") + t*[" << dir->x << "," << dir->y << "," << dir->z << "]" << endl;
	}
};

class Matrix{
public:
	float mat[4][4];
	Matrix(){
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (i == j){
					mat[i][j] = 1.0;
				}
				else{
					mat[i][j] = 0.0;
				}
			}
		}
	}
	Matrix(float matt[4][4]){
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				mat[i][j] = matt[i][j];
			}
		}
	}
	void print(){
		for (int i = 0; i < 4; i++)
		{
			if (i == 0)
			{
				cout << "{[" ;
			}
			else
			{
				cout << " [";
			}
			for (int j = 0; j < 4; j++)
			{
				if (j != 3){
					cout << mat[i][j] << ",";
				}
				else{
					cout << mat[i][j];
				}
			}
			if (i == 3)
			{
				cout << "]}" << endl;
			}
			else
			{
				cout << "]" << endl;
			}
		}
	}
	//translation=1, rotation=2 , scaling=3
	Matrix(int transformation, float tx, float ty, float tz)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (i == j){
					mat[i][j] = 1.0;
				}
				else{
					mat[i][j] = 0.0;
				}
			}
		}
		if (transformation== 1){
			mat[0][3] = tx;
			mat[1][3] = ty;
			mat[2][3] = tz;
		}
		if (transformation == 2)
		{
			float rotx[3][3] = { { 1, 0, 0 }, { 0, cos(tx), -sin(tx) }, { 0, sin(tx), cos(tx) } };
			float roty[3][3] = { { cos(ty), 0, sin(ty) }, { 0, 1.0, 0 }, { -sin(ty), 0, cos(ty) } };
			float rotz[3][3] = { { cos(tz), -sin(tz), 0 }, { sin(tz), cos(tz), 0 }, { 0, 0, 1 } };
			float rot[3][3];
			for (int i = 0; i < 3; i++){
				for (int j = 0; j < 3; j++){
					rot[i][j] = 0;
				}
			}
			for (int i = 0; i < 3; i++){
				for (int j = 0; j < 3; j++){
					for (int k = 0; k < 3; k++){
						rot[i][j] += rotx[i][k] * roty[k][j];
					}
				}
			}
			for (int i = 0; i < 3; i++){
				for (int j = 0; j < 3; j++){
					for (int k = 0; k < 3; k++){
						rot[i][j] += rot[i][k] * rotz[k][j];
					}
				}
			}
			for (int i = 0; i < 3; i++){
				for (int j = 0; j < 3; j++){
					mat[i][j] = rot[i][j];
				}
			}
		}
		if (transformation == 3){
			mat[0][0] = tx;
			mat[1][1] = ty;
			mat[2][2] = tz;
		}
	}
	
};
/*
float determinant(int n, float mat[4][4]){
	float d = 0;
	int c, subi, i, j, subj;
	float submat[4][4];
	if (n == 2)
	{
		return((mat[0][0] * mat[1][1]) - (mat[1][0] * mat[0][1]));
	}
	else
	{
		for (c = 0; c < n; c++)
		{
			subi = 0;
			for (i = 1; i < n; i++)
			{
				subj = 0;
				for (j = 0; j < n; j++)
				{
					if (j == c)
					{
						continue;
					}
					submat[subi][subj] = mat[i][j];
					subj++;
				}
				subi++;
			}
			d = d+ (pow(-1, c) * mat[0][c] * determinant(n - 1, submat));
		}
	}
	return d;
}
*/

class Transformation{
public:
	Matrix m;
	Matrix minvt;
	Transformation(Matrix mm){
		m = mm;
		//inverse(mm.mat, minvt.mat);
	}
};

class Color {
public:
	float r, g, b;
	Color(){
		r = 0;
		g = 0;
		b = 0;
	}
	Color(float rr, float gg, float bb){
		r = rr;
		g = gg;
		b = bb;
	}
	Color* operator+ (Color c){
		return new Color (r + c.r, g + c.g, b + c.b);
	}
	//c1 = (*c1) + (*c2);
	Color* operator- (Color c){
		return new Color(r - c.r, g - c.g, b - c.b);
	}
	Color* operator* (float c){
		return new Color(r*c, g*c, b*c);
	}
	void print(){
		cout << "color(" << r << "," << g << "," << b << ")" << endl;
	}
};

class BRDF{
public:
	Color *kd, *ks, *ka, *kr;
	BRDF(){
		kd = new Color();
		ks = new Color();
		ka = new Color();
		kr = new Color();
	}

	BRDF(Color* kkd, Color *kks, Color *kka, Color *kkr){
		kd = kkd;
		ks = kks;
		ka = kka;
		kr = kkr;
	}
	void print(){
		cout << "BRDF:" << endl;
		cout << "kd: ";
		kd->print();
		cout << "ks: ";
		ks->print();
		cout << "ka: ";
		ka->print();
		cout << "kr: ";
		kr->print();
	}
};

class Sample {
public:
	float x, y;
	Sample(){
		x = 0;
		y = 0;
	}
	Sample(float xx, float yy){
		x = xx;
		y = yy;
	}
	void print(){
		cout << "Sample(" << x << "," << y << ")"<<endl;
	}

};

class LocalGeo {
public:
	Point *pos;
	Normal *normal;
	LocalGeo(){
		pos = new Point();
		normal = new Normal();
	}
	LocalGeo(Point *poss, Normal *norm){
		pos = poss;
		normal = norm;
	}
	void print(){
		cout << "LocalGeo:";
		pos->print();
		normal->print();
	}
};

