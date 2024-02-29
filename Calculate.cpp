#include "pch.h"
/*
**べき乗計算
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
** 最小2乗法
*/
#define M 2      // 予測曲線の次数
#define N 20      // データ数
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
		// s[], t[] 計算
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
		// s[], t[] 計算
		for (i = x - N; i <= x; i++) {
			dx = (double)i * 0.1f;
			for (j = 0; j <= 2 * M; j++)
				s[j] += ipow(dx, j);
			for (j = 0; j <= M; j++)
				t[j] += ipow(dx, j) * dTable[i];
		}
	}

	// a[][] に s[], t[] 代入
	for (i = 0; i <= M; i++) {
		for (j = 0; j <= M; j++)
			a[i][j] = s[i + j];
		a[i][M + 1] = t[i];
	}
	// 掃き出し
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

	// y 値計算＆結果出力
	p = 0;
	for (k = 0; k <= M; k++)
		p += a[k][M + 1] * ipow(dx, k);
	return(p);
#endif
}
