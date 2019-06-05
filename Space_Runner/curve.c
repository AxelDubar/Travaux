#include <string.h>
#include <stdio.h>
#include <math.h>
#include "curve.h"

double puissance(double t, int p)
{
	double res=1.0;
	for(int i=0;i<p;i++){
		res=res*t;
	}
	return res;
}

double compute_bezier_cubic (double b[4], double t)
{
	double f=puissance((1-t),3)*b[0]+3*puissance((1-t),2)*t*b[1]+3*(1-t)*puissance(t,2)*b[2]+puissance(t,3)*b[3];
	return f;
}


void convert_bsp3_to_bezier (double p[4], double b[4])
{
	b[0]=(1.0/6.0)*(p[0]+4*p[1]+p[2]);
	b[1]=(1.0/6.0)*(4*p[1]+2*p[2]);
	b[2]=(1.0/6.0)*(2*p[1]+4*p[2]);
	b[3]=(1.0/6.0)*(p[1]+4*p[2]+p[3]);
}

void compute_bezier_points_open(Curve *curve, int i, Control bez_points[4])
{
	double px[4]={curve->controls[i].x, curve->controls[i+1].x, curve->controls[i+2].x, curve->controls[i+3].x};
	double py[4]={curve->controls[i].y, curve->controls[i+1].y, curve->controls[i+2].y, curve->controls[i+3].y};
	double bx[4],by[4];
	convert_bsp3_to_bezier (px, bx);
	convert_bsp3_to_bezier (py, by);
	bez_points[0].x=bx[0];
	bez_points[0].y=by[0];
	bez_points[1].x=bx[1];
	bez_points[1].y=by[1];
	bez_points[2].x=bx[2];
	bez_points[2].y=by[2];
	bez_points[3].x=bx[3];
	bez_points[3].y=by[3];
}

void compute_bezier_points_close(Curve *curve, int i, Control bez_points[4])
{
	if(i>=(curve->control_count-3)){
	double px[4]={curve->controls[i%(curve->control_count)].x, curve->controls[(i+1)%(curve->control_count)].x, curve->controls[(i+2)%(curve->control_count)].x, curve->controls[(i+3)%(curve->control_count)].x};
	double py[4]={curve->controls[i%(curve->control_count)].y, curve->controls[(i+1)%(curve->control_count)].y, curve->controls[(i+2)%(curve->control_count)].y, curve->controls[(i+3)%(curve->control_count)].y};
	double bx[4],by[4];
	convert_bsp3_to_bezier (px, bx);
	convert_bsp3_to_bezier (py, by);
	bez_points[0].x=bx[0];
	bez_points[0].y=by[0];
	bez_points[1].x=bx[1];
	bez_points[1].y=by[1];
	bez_points[2].x=bx[2];
	bez_points[2].y=by[2];
	bez_points[3].x=bx[3];
	bez_points[3].y=by[3];
	}
	else
		compute_bezier_points_open(curve, i, bez_points);
}

void convert_bsp3_to_bezier_prolong_first(double p[3], double b[4]){
	b[0]=p[0];
	b[1]=(2*p[0]+p[1])/3.0;
	b[2]=(p[0]+2*p[1])/3.0;
	b[3]=(p[0]+4*p[1]+p[2])/6.0;
}

void convert_bsp3_to_bezier_prolong_last(double p[3], double b[4]){
	b[0]=(p[0]+4*p[1]+p[2])/6.0;
	b[1]=(2*p[1]+p[2])/3.0;
	b[2]=(p[1]+2*p[2])/3.0;
	b[3]=p[2];
}

void compute_bezier_points_prolong_first(Curve *curve, Control bez_points[4])
{
	double px[3]={curve->controls[0].x, curve->controls[1].x, curve->controls[2].x};
	double py[3]={curve->controls[0].y, curve->controls[1].y, curve->controls[2].y};
	double bx[4],by[4];
	convert_bsp3_to_bezier_prolong_first (px, bx);
	convert_bsp3_to_bezier_prolong_first (py, by);
	bez_points[0].x=bx[0];
	bez_points[0].y=by[0];
	bez_points[1].x=bx[1];
	bez_points[1].y=by[1];
	bez_points[2].x=bx[2];
	bez_points[2].y=by[2];
	bez_points[3].x=bx[3];
	bez_points[3].y=by[3];
}

void compute_bezier_points_prolong_last(Curve *curve, Control bez_points[3])
{
	double px[3]={curve->controls[curve->control_count-3].x, curve->controls[curve->control_count-2].x, curve->controls[curve->control_count-1].x};
	double py[3]={curve->controls[curve->control_count-3].y, curve->controls[curve->control_count-2].y, curve->controls[curve->control_count-1].y};
	double bx[4],by[4];
	convert_bsp3_to_bezier_prolong_last (px, bx);
	convert_bsp3_to_bezier_prolong_last (py, by);
	bez_points[0].x=bx[0];
	bez_points[0].y=by[0];
	bez_points[1].x=bx[1];
	bez_points[1].y=by[1];
	bez_points[2].x=bx[2];
	bez_points[2].y=by[2];
	bez_points[3].x=bx[3];
	bez_points[3].y=by[3];
}

void init_curve_infos (Curve_infos *ci)
{
	ci->current_curve=-1;
	ci->current_control=-1;
	ci->curve_list.curve_count=0;
}

int move_curve(Curve_infos *ci, double dx, double dy){
	if(ci->current_curve<0 || ci->current_curve>ci->curve_list.curve_count-1)return -1;
	for(int i=0;i<ci->curve_list.curves[ci->current_curve].control_count;i++){
		ci->curve_list.curves[ci->current_curve].controls[i].x+=dx;
		ci->curve_list.curves[ci->current_curve].controls[i].y+=dy;
	}
	return 0;
}

int remove_curve(Curve_infos *ci){
	if(ci->current_curve<0 || ci->current_curve>ci->curve_list.curve_count-1)return -1;
	int n=ci->current_curve;
	memmove(ci->curve_list.curves+n, ci->curve_list.curves+1+n, sizeof(Curve)*(ci->curve_list.curve_count-1-n));
	ci->curve_list.curve_count--;
	ci->current_curve=-1;
	return 0;
}

int remove_control(Curve_infos *ci){
	if((ci->current_curve<0 || ci->current_curve>ci->curve_list.curve_count-1) &&(ci->current_control<0 || ci->curve_list.curves[ci->current_curve].control_count-1))return -1;
	if(ci->curve_list.curves[ci->current_curve].control_count==1){
		remove_curve(ci);
		return 0;
	}
	int n=ci->current_control;
	memmove(ci->curve_list.curves[ci->current_curve].controls+n,ci->curve_list.curves[ci->current_curve].controls+n+1,sizeof(Control)*(ci->curve_list.curves[ci->current_curve].control_count-1-n));
	ci->curve_list.curves[ci->current_curve].control_count--;
	ci->current_control=-1; 
	return 0;
}

int find_control(Curve *curve,double x, double y){
	for(int j=0;j<curve->control_count;j++){
		int dx=curve->controls[j].x-x;
		int dy=curve->controls[j].y-y;
		if(dx*dx + dy*dy <=5*5)
			return j;
	}
	return -1;
}

int move_control(Curve_infos *ci, double dx, double dy){
	if((ci->current_curve<0 || ci->current_curve>ci->curve_list.curve_count-1) &&(ci->current_control<0 || ci->curve_list.curves[ci->current_curve].control_count-1))return -1;
	ci->curve_list.curves[ci->current_curve].controls[ci->current_control].x+=dx;
	ci->curve_list.curves[ci->current_curve].controls[ci->current_control].y+=dy;
	return 0;
}

int add_curve(Curve_infos *ci)
{
  if(ci->curve_list.curve_count==CURVE_MAX)
  {
	ci->current_curve=-1;
 	return -1;
  }
  int n=ci->curve_list.curve_count;
  ci->curve_list.curve_count++;
  ci->curve_list.curves[n].control_count=0;
  ci->current_curve=n;
  ci->current_control=-1;
  return n;
}

int add_control (Curve_infos *ci, double x, double y)
{
  int n=ci->current_curve;
  if(n<0 || n>ci->curve_list.curve_count-1)
	return -1;
  Curve *curve=&ci->curve_list.curves[n];
  if(curve->control_count==CONTROL_MAX)
  {
	ci->current_control=-1;
	return -1;
  }
  int k=curve->control_count;
  curve->control_count++;
  curve->controls[k].x=x, curve->controls[k].y=y;
  ci->current_control=k;
  return k;
}

void store_sample (double x, double y, double sx[], double sy[], int *ind, int ind_max)
{
  if (*ind >= ind_max) {
	fprintf (stderr, "%s: capacity exceeded \n", __func__);
	return;
  }
  sx[*ind] = x;
  sy[*ind] = y;
  *ind += 1;
}


void sample_bezier_curve (Control bez_points[4], double theta, double sx[], double sy[], int *ind, int ind_max, int is_first)
{
  double x, y, bx[4], by[4], t;

  for (int j = 0; j <= 3 ; j++) {
	  bx[j] = bez_points[j].x;
	  by[j] = bez_points[j].y;
  }

  for (t = is_first ? 0.0 : theta; t < 1.0; t += theta) {
	x = compute_bezier_cubic (bx, t);
	y = compute_bezier_cubic (by, t);
	store_sample (x, y, sx, sy, ind, ind_max);
  }

  if (t < 1.0) {
	x = compute_bezier_cubic (bx, 1.0);
	y = compute_bezier_cubic (by, 1.0);
	store_sample (x, y, sx, sy, ind, ind_max);
  }
}
