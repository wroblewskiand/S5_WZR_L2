/**************************************************************************************************
	Obs�uga quaternion�w

	Aby zrealizowa� obr�t nale�y pomno�y� quaternion orientacji przez kwaterion obrotu
	z lewej strony:

	qOrient = qRot*qOrient

	Uwaga! Zak�adam, �e quaterniony qRot i qOrient s� d�ugo�ci 1, inaczej trzeba jeszcze je normalizowa�

	Kwaternion mo�e tez reprezentowa� orientacj� obiektu, je�li za�o�ymy jak�� orientacj�
	pocz�tkow� np. obiekt skierowany w kierunku osi 0x, z normaln� zgodn� z 0y. W�wczas
	quaternion odpowiada obrotowi od po�o�enia poc��tkowego.
	****************************************************************************************************/
#include <stdlib.h>
#include "quaternion.h"


quaternion::quaternion(float x, float y, float z, float w)

{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}


quaternion::quaternion()
{
	x = 0;
	y = 0;
	z = 0;
	w = 1;
}

// mno�enie dw�ch quaternion�w
quaternion quaternion::operator*(quaternion q)
{
	double rx, ry, rz, rw;		// temp result	



	rx = x*q.w + w*q.x - z*q.y + y*q.z;
	ry = y*q.w + w*q.y - x*q.z + z*q.x;
	rz = z*q.w + w*q.z - y*q.x + x*q.y;

	rw = w*q.w - x*q.x - y*q.y - z*q.z;

	return quaternion((float)rx, (float)ry, (float)rz, (float)rw);
}

// zamiana quaterniona na reprezentacj� k�towo-osiow�
// (o� jest reprezentowana wektorem, k�t d�ugo�ci� wektora) 
Vector3 quaternion::AsixAngle()
{
	Vector3 v(x, y, z);

	if (v.length() == 0) return Vector3(0, 0, 0);
	else
	{
		v = v.znorm();
		return Vector3(v.x, v.y, v.z)*2.0*acos(w);
	}
}

quaternion quaternion::operator~ ()
{
	return quaternion(-x, -y, -z, w);
}

quaternion quaternion:: operator+= (quaternion q)	// operator+= is used to add another Vector3D to this Vector3D.
{
	x += q.x;
	y += q.y;
	z += q.z;
	w += q.w;
	return *this;
}

quaternion quaternion:: operator+ (quaternion q)	// operator+= is used to add another Vector3D to this Vector3D.
{
	return quaternion(x + q.x, y + q.y, z + q.z, w + q.w);
}

quaternion quaternion:: operator- (quaternion q)	// operator+= is used to add another Vector3D to this Vector3D.
{
	return quaternion(x - q.x, y - q.y, z - q.z, w - q.w);
}

quaternion quaternion::n() // licz normal_vector z obecnego wektora
{
	float length;

	length = sqrt(x*x + y*y + z*z + w*w);
	if (length == 0) return quaternion(0, 0, 0, 0);
	else return quaternion(x / length, y / length, z / length, w / length);
}

float quaternion::l() // licz normal_vector z obecnego wektora
{
	return sqrt(x*x + y*y + z*z + w*w);
}

quaternion quaternion::operator* (float value)  // mnozenie razy skalar
{
	return quaternion(x*value, y*value, z*value, w*value);
}

quaternion quaternion::operator/ (float value)  // dzielenie przez skalar
{
	return quaternion(x / value, y / value, z / value, w / value);
}



/*  obr�t wektora z u�yciem quaterniona obrotu - do sprawdzenia
macierz obrotu:
M =  [ w^2 + x^2 - y^2 - z^2       2xy - 2wz                   2xz + 2wy
2xy + 2wz                   w^2 - x^2 + y^2 - z^2       2yz - 2wx
2xz - 2wy                   2yz + 2wx                   w^2 - x^2 - y^2 + z^2 ]
*/
Vector3 quaternion::rotate_vector(Vector3 V)
{
	Vector3 Vo;

	quaternion p = quaternion(V.x, V.y, V.z, 0), q = quaternion(x, y, z, w);
	quaternion pp = q*p*~q;
	Vo.x = pp.x; Vo.y = pp.y; Vo.z = pp.z;

	return Vo;
}

// zamiana obrotu wyra�onego reprezentacj� k�towo-osiow� na reprezentacj� quaternionow�  
quaternion AsixToQuat(Vector3 v, float angle)
{
	float x, y, z, w;			// temp vars of vector
	double rad, scale;		// temp vars

	if (v.length() == 0) return quaternion();

	v = v.znorm();
	rad = angle;
	scale = sin(rad / 2.0);


	w = (float)cos(rad / 2.0);

	x = float(v.x * scale);
	y = float(v.y * scale);
	z = float(v.z * scale);

	return quaternion(x, y, z, w);
}
