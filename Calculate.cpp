#include "pch.h"
/*
**�ׂ���v�Z
*/
double ipow(double p, int n)
{
	int k;
	double s = 1;

	if (p == 0.0f)
		return(0.0f);


	for (k = 1; k <= n; k++)
		s *= p;
	return s;
}
/*
** �ŏ�2��@
*/
#define M 2      // �\���Ȑ��̎���
#define N 20      // �f�[�^��
double calcLeastSquaresMethod(int x, double dTable[])
{
	double Sx, Sy, Sxy, Sxx, a, b, D;
	double	dx;
	double	dy;

	int i;
	Sx = 0;
	Sy = 0;
	Sxy = 0;
	Sxx = 0;

	if (x <= N)
		return(0.0f);

	for (i = x - N; i < x; i++) {
		dx = (double)i * 0.1f;
		Sx += dx;
		Sy += dTable[i];
		Sxy += (dx * dTable[i]);
		Sxx += (dx * dx);
	}


	D = N * Sxx - Sx * Sx;
	a = (N * Sxy - Sx * Sy) / D;
	b = (Sxx * Sy - Sxy * Sx) / D;

	dx = (double)x * 0.1f;
	dy = a * dx + b;
	return(a);

#if 0
	double a[M + 1][M + 2], s[2 * M + 1], t[M + 1];
	int i, j, k;
	double p, d, px;
	double dx;

	for (i = 0; i <= 2 * M; i++)
		s[i] = 0;
	for (i = 0; i <= M; i++)
		t[i] = 0;

	if (x <= N) {
		return(0.0f);
#if 0
		// s[], t[] �v�Z
		for (i = 0; i < x; i++) {
			dx = (double)i * 0.1f;
			for (j = 0; j <= 2 * M; j++)
				s[j] += ipow(dx, j);
			for (j = 0; j <= M; j++)
				t[j] += ipow(dx, j) * dTable[i];
		}
#endif
	}
	else {
		// s[], t[] �v�Z
		for (i = x - N; i <= x; i++) {
			dx = (double)i * 0.1f;
			for (j = 0; j <= 2 * M; j++)
				s[j] += ipow(dx, j);
			for (j = 0; j <= M; j++)
				t[j] += ipow(dx, j) * dTable[i];
		}
	}

	// a[][] �� s[], t[] ���
	for (i = 0; i <= M; i++) {
		for (j = 0; j <= M; j++)
			a[i][j] = s[i + j];
		a[i][M + 1] = t[i];
	}
	// �|���o��
	for (k = 0; k <= M; k++) {
		p = a[k][k];
		for (j = k; j <= M + 1; j++)
			a[k][j] /= p;
		for (i = 0; i <= M; i++) {
			if (i != k) {
				d = a[i][k];
				for (j = k; j <= M + 1; j++)
					a[i][j] -= d * a[k][j];
			}
		}
	}

	// y �l�v�Z�����ʏo��
	p = 0;
	for (k = 0; k <= M; k++)
		p += a[k][M + 1] * ipow(dx, k);
	return(p);
#endif
}
