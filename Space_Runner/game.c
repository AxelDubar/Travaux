#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include "curve.h"
#include "game.h"
#include <math.h>

void set_GS_mode(Game *game,Game_state state){
	game->state=state;
	/*switch(game->state){
		
	}*/
}


void init_game(Game *game){
	init_road(&game->road);
	
	game->state=GS_HELLO;
	game->mode=GM_SINGLE;
	game->score=0;
}

void add_boost(Road *road,double x,double y){
	if(road->boost.boost_count==BOOST_MAX)return;
	road->boost.boost_x[road->boost.boost_count]=x;
	road->boost.boost_y[road->boost.boost_count]=y;
	road->boost.boost_count++;
}

void remove_boost(Road *road,int indice){
	if(indice==-1)return;
	for(int i=indice;i<road->boost.boost_count-1;i++){
			road->boost.boost_x[indice]=road->boost.boost_x[indice+1];
			road->boost.boost_y[indice]=road->boost.boost_y[indice+1];
	}	
	road->boost.boost_count--;
}

void move_boost(Road *road,int nb_boost, double dx, double dy){
	if(nb_boost==-1)return;
	road->boost.boost_x[nb_boost]+=dx;
	road->boost.boost_y[nb_boost]+=dy;
}

int find_boost(Boost *boost,double x, double y){
	for(int j=0;j<boost->boost_count;j++){
		int dx=boost->boost_x[j]-x;
		int dy=boost->boost_y[j]-y;
		if(dx*dx + dy*dy <=5*5)
			return j;
	}
	return -1;
}

double distance(double xA, double yA, double xB,double yB,double xC,double yC){
	double det=(xC-xA)*(yB-yA)-(yC-yA)*(xB-xA);
	double norme=sqrt((xA-xB)*(xA-xB)+(yA-yB)*(yA-yB));
	return 2*det/norme;
} 

double get_horiz_angle(double xA, double yA, double xB, double yB){
	double dy=yB-yA;
	double dx=xB-xA;
	double n=sqrt(dx*dx+dy*dy);
	if(n==0)return 0;
	if(dy>0)return acos(dx/n);
	return -acos(dx/n);
}

double get_angle_reflechi(double xA, double yA, double xB, double yB, double xC, double yC, double xD, double yD){
	double alpha=get_horiz_angle(xA,yA,xB,yB);
	double beta=get_horiz_angle(xC,yC,xD,yD);
	double theta=2*alpha-beta;
	if(theta>2*G_PI)theta-=2*G_PI;
	if(theta<0)theta+=2*G_PI;
	return theta;
}

void progress_game_next_step (Game *game){
	for(int i=0;i<game->car_count;i++){
		game->cars[i].angle+=game->cars[i].direction*G_PI/45;
		
		double x=game->cars[i].x,y=game->cars[i].y;
		for(int i=0;i<game->road.boost.boost_count;i++){
			int dx=fabs(game->road.boost.boost_x[i]-x);
			int dy=fabs(game->road.boost.boost_y[i]-y);
				if(dx<10 && dy<10)
					game->cars[i].temps_boost=250;
		}
		
		double frottement=0;
		if(game->cars[i].speed>=0.05)frottement=0.05;
		else if(game->cars[i].speed<=-0.05)frottement=-0.05;
		
		if(game->cars[i].temps_boost>0){
			game->cars[i].speed+=game->cars[i].accelerator*0.2-frottement;
			game->cars[i].temps_boost--;
		}
		else
			game->cars[i].speed+=game->cars[i].accelerator*0.1-frottement;
			
		if(game->cars[i].speed>6)game->cars[i].speed=6;
		else if(game->cars[i].speed<-1)game->cars[i].speed=-1;
		
		if(game->cars[i].angle>2*G_PI)game->cars[i].angle-=2*G_PI;
		if( game->cars[i].angle<0)game->cars[i].angle+=2*G_PI;
		
		game->cars[i].x+=game->cars[i].speed*cos(game->cars[i].angle);
		game->cars[i].y+=game->cars[i].speed*sin(game->cars[i].angle);
		
		for(int j=0;j<game->road.track_left.sample_count-1;j++){
			double x1=game->road.track_left.sample_x[j+1], y1=game->road.track_left.sample_y[j+1];
			double x2=game->road.track_left.sample_x[j], y2=game->road.track_left.sample_y[j];
			double r=game->cars[i].radius+3;
			if(game->cars[i].x>=x1-r && game->cars[i].x<=x2+r && game->cars[i].y>=y1-r && game->cars[i].y<=y2+r){
				if(fabs(distance(x1,y1,x2,y2,game->cars[i].x,game->cars[i].y))<=game->cars[i].radius+1){
					game->cars[i].angle=get_angle_reflechi(x1,y1,x2,y2,x,y,game->cars[i].x,game->cars[i].y);
					game->cars[i].x+=game->cars[i].speed*cos(game->cars[i].angle);
					game->cars[i].y+=game->cars[i].speed*sin(game->cars[i].angle);
					game->cars[i].speed=game->cars[i].speed/3;
					break;
				}
			}
		}
		
		for(int j=0;j<game->road.track_right.sample_count-1;j++){
			double x1=game->road.track_right.sample_x[j], y1=game->road.track_right.sample_y[j];
			double x2=game->road.track_right.sample_x[j+1], y2=game->road.track_right.sample_y[j+1];
			double r=game->cars[i].radius+3;
			if(game->cars[i].x>=x1-r && game->cars[i].x<=x2+r && game->cars[i].y>=y1-r && game->cars[i].y<=y2+r){
				if(fabs(distance(x1,y1,x2,y2,game->cars[i].x,game->cars[i].y))<=game->cars[i].radius+1){
					game->cars[i].angle=get_angle_reflechi(x1,y1,x2,y2,x,y,game->cars[i].x,game->cars[i].y);
					game->cars[i].x+=fabs(game->cars[i].speed)*cos(game->cars[i].angle);
					game->cars[i].y+=fabs(game->cars[i].speed)*sin(game->cars[i].angle);
					game->cars[i].speed=game->cars[i].speed/3;
					break;
				}
			}
		}
		
		double x1=game->road.track_left.sample_x[game->road.track_left.sample_count-1], y1=game->road.track_left.sample_y[game->road.track_left.sample_count-1];
		double x2=game->road.track_right.sample_x[game->road.track_right.sample_count-1], y2=game->road.track_right.sample_y[game->road.track_right.sample_count-1];
		double r=game->cars[i].radius+50;
		if(game->cars[i].x>=x1-r && game->cars[i].x<=x2+r && game->cars[i].y>=y1-r && game->cars[i].y<=y2+r){
			if(fabs(distance(x1,y1,x2,y2,game->cars[i].x,game->cars[i].y))<=game->cars[i].radius+5){
				set_GS_mode(game, GS_WON);
				game->cars[i].is_winner=1;
			}
		}
	}
}


void place_cars_on_start(Game *game){
	double alpha=get_horiz_angle(game->road.curve_central.controls[0].x,
										game->road.curve_central.controls[0].y,
										game->road.curve_central.controls[1].x,
										game->road.curve_central.controls[1].y);
	switch(game->car_count){
		case 4 :
			game->cars[0].x=(game->road.curve_central.controls[0].x+game->road.curve_right.controls[0].x)/2+cos(alpha)*25;
			game->cars[0].y=(game->road.curve_central.controls[0].y+game->road.curve_right.controls[0].y)/2+sin(alpha)*25;
			
			game->cars[1].x=(game->road.curve_central.controls[0].x+game->road.curve_left.controls[0].x)/2+cos(alpha)*25;
			game->cars[1].y=(game->road.curve_central.controls[0].y+game->road.curve_left.controls[0].y)/2+sin(alpha)*25;
			
			game->cars[2].x=(game->road.curve_central.controls[0].x+game->road.curve_left.controls[0].x)/2;
			game->cars[2].y=(game->road.curve_central.controls[0].y+game->road.curve_left.controls[0].y)/2;
			
			game->cars[3].x=(game->road.curve_central.controls[0].x+game->road.curve_right.controls[0].x)/2;
			game->cars[3].y=(game->road.curve_central.controls[0].y+game->road.curve_right.controls[0].y)/2;
			break;
			
		case 3 :
			game->cars[0].x=(game->road.curve_central.controls[0].x+game->road.curve_right.controls[0].x)/2+cos(alpha)*25;
			game->cars[0].y=(game->road.curve_central.controls[0].y+game->road.curve_right.controls[0].y)/2+sin(alpha)*25;
			
			game->cars[1].x=(game->road.curve_central.controls[0].x+game->road.curve_right.controls[0].x)/2;
			game->cars[1].y=(game->road.curve_central.controls[0].y+game->road.curve_right.controls[0].y)/2;
			
			game->cars[2].x=(game->road.curve_central.controls[0].x+game->road.curve_left.controls[0].x)/2;
			game->cars[2].y=(game->road.curve_central.controls[0].y+game->road.curve_left.controls[0].y)/2;
			break;
			
		case 2 :
			game->cars[0].x=(game->road.curve_central.controls[0].x+game->road.curve_left.controls[0].x)/2;
			game->cars[0].y=(game->road.curve_central.controls[0].y+game->road.curve_left.controls[0].y)/2;
			
			game->cars[1].x=(game->road.curve_central.controls[0].x+game->road.curve_right.controls[0].x)/2;
			game->cars[1].y=(game->road.curve_central.controls[0].y+game->road.curve_right.controls[0].y)/2;
			break;
			
		case 1 :
			game->cars[0].x=game->road.curve_central.controls[0].x;
			game->cars[0].y=game->road.curve_central.controls[0].y;
			break;		
	}
	game->cars[0].angle=alpha;
	game->cars[0].speed=0;
	game->cars[1].speed=0;
	game->cars[1].angle=game->cars[0].angle;
	game->cars[2].angle=game->cars[0].angle;
	game->cars[3].angle=game->cars[0].angle;
	
	
	game->cars[0].radius=5;
	game->cars[1].radius=5;
	game->cars[2].radius=5;
	game->cars[3].radius=5;
	
	game->cars[0].accelerator=0;
	game->cars[1].accelerator=0;
	game->cars[2].accelerator=0;
	game->cars[3].accelerator=0;
	
	game->cars[0].temps_boost=0;
	game->cars[1].temps_boost=0;
	game->cars[2].temps_boost=0;
	game->cars[3].temps_boost=0;
	
	game->cars[0].is_winner=0;
	game->cars[1].is_winner=0;
	game->cars[2].is_winner=0;
	game->cars[3].is_winner=0;
	
}


void init_road(Road *road)
{
	road->curve_central.control_count=0;
	road->curve_left.control_count=0;
	road->curve_right.control_count=0;
	road->boost.boost_count=0;
}

void do_vector_product(double xa,double ya,double za,double xb,double yb,double zb,double *xn,double *yn,double *zn){
	*xn=ya*zb-yb*za;
	*yn=xa*zb-xb*za;
	*zn=xa*yb-xb*ya;
}

int compute_intersection(double xs0,double ys0,double xs1,double ys1,double xt0,double yt0,double xt1,double yt1,double *xp,double *yp){
	double as,bs,cs,at,bt,ct,xi,yi,zi;
	//calcul eq(S0S1)
	do_vector_product(xs0,ys0,1,xs1,ys1,1,&as,&bs,&cs);
	//calcul eq(T0T1)
	do_vector_product(xt0,yt0,1,xt1,yt1,1,&at,&bt,&ct);
	//intersection
	do_vector_product(as,bs,cs,at,bt,ct,&xi,&yi,&zi);
	if(zi==0)//pas d'intersection
		return 0;
	*xp=xi/zi;
	*yp=yi/zi;
	return 1;
}

void draw_road_wall_right(cairo_t *cr, Control P0,Control P1,double xn,double yn,Road *road){
	cairo_set_source_rgb(cr,0,1.0,0);
	double x0d, y0d, x1d, y1d;
	x0d=P0.x+xn*ROAD_LARGE;
	y0d=P0.y+yn*ROAD_LARGE;
	x1d=P1.x+xn*ROAD_LARGE;
	y1d=P1.y+yn*ROAD_LARGE;
	cairo_move_to(cr,x0d,y0d);
	cairo_line_to(cr,x1d,y1d);
	cairo_stroke(cr);
}

void draw_road_wall_left(cairo_t *cr, Control P0,Control P1,double xn,double yn,Road *road){
	cairo_set_source_rgb(cr,1.0,0,0);
	double x0l, y0l, x1l, y1l;
	x0l=P0.x-xn*ROAD_LARGE;
	y0l=P0.y-yn*ROAD_LARGE;
	x1l=P1.x-xn*ROAD_LARGE;
	y1l=P1.y-yn*ROAD_LARGE;
	cairo_move_to(cr,x0l,y0l);
	cairo_line_to(cr,x1l,y1l);
	cairo_stroke(cr);
}

void add_control_from_curve (Curve *curve, double x, double y)
{
  if(curve->control_count==CONTROL_MAX)
	return;
  int k=curve->control_count;
  curve->control_count++;
  curve->controls[k].x=x, curve->controls[k].y=y;
  return;
}

void add_control_from_road (Road *road, double x, double y)
{
  add_control_from_curve(&road->curve_central,x,y);
  if(road->curve_central.control_count>=2){
	  double xn,yn;
	  Control P0=road->curve_central.controls[road->curve_central.control_count-1], P1=road->curve_central.controls[road->curve_central.control_count-2];
	  compute_normale_right(P0.x,P1.x,P0.y, P1.y,&xn,&yn);
	  if(road->curve_central.control_count>=3){
		  //intersection a droite
		  double xp=0,yp=0;
		  double xs0=road->curve_right.controls[road->curve_right.control_count-2].x, xs1=road->curve_right.controls[road->curve_right.control_count-1].x, xt0=P1.x+xn*ROAD_LARGE,	xt1=P0.x+xn*ROAD_LARGE;
		  double ys0=road->curve_right.controls[road->curve_right.control_count-2].y, ys1=road->curve_right.controls[road->curve_right.control_count-1].y, yt0=P1.y+yn*ROAD_LARGE, yt1=P0.y+yn*ROAD_LARGE;
		  compute_intersection(xs0,ys0,xs1,ys1,xt0,yt0,xt1,yt1,&xp,&yp);
		  remove_control_from_curve(&road->curve_right,road->curve_right.control_count-1);
		  add_control_from_curve(&road->curve_right,xp,yp);
		  add_control_from_curve(&road->curve_right,xt1,yt1);
		  
		  //intersection a gauche
		  xs0=road->curve_left.controls[road->curve_left.control_count-2].x, xs1=road->curve_left.controls[road->curve_left.control_count-1].x, xt0=P1.x-xn*ROAD_LARGE,	xt1=P0.x-xn*ROAD_LARGE;
		  ys0=road->curve_left.controls[road->curve_left.control_count-2].y, ys1=road->curve_left.controls[road->curve_left.control_count-1].y, yt0=P1.y-yn*ROAD_LARGE, yt1=P0.y-yn*ROAD_LARGE;
		  compute_intersection(xs0,ys0,xs1,ys1,xt0,yt0,xt1,yt1,&xp,&yp);
		  remove_control_from_curve(&road->curve_left,road->curve_left.control_count-1);
		  add_control_from_curve(&road->curve_left,xp,yp);
		  add_control_from_curve(&road->curve_left,xt1,yt1);
	  }
	  else{
		  add_control_from_curve(&road->curve_right,P1.x+xn*ROAD_LARGE,P1.y+yn*ROAD_LARGE);
		  add_control_from_curve(&road->curve_right,P0.x+xn*ROAD_LARGE,P0.y+yn*ROAD_LARGE);
		  add_control_from_curve(&road->curve_left,P1.x-xn*ROAD_LARGE,P1.y-yn*ROAD_LARGE);
		  add_control_from_curve(&road->curve_left,P0.x-xn*ROAD_LARGE,P0.y-yn*ROAD_LARGE);
	  }
  }
}

int remove_control_from_curve(Curve *curve,int n){
	if(n==(-1))return -1;
	memmove(curve->controls+n,curve->controls+n+1,sizeof(Control)*(curve->control_count-1-n));
	curve->control_count--; 
	return 0;
}

int remove_control_from_road(Road *road,int n){
	if(n==(-1))return -1;
	memmove(road->curve_central.controls+n,road->curve_central.controls+n+1,sizeof(Control)*(road->curve_central.control_count-1-n));
	memmove(road->curve_left.controls+n,road->curve_left.controls+n+1,sizeof(Control)*(road->curve_left.control_count-1-n));
	memmove(road->curve_right.controls+n,road->curve_right.controls+n+1,sizeof(Control)*(road->curve_right.control_count-1-n));
	if(road->curve_central.control_count<=2){
		road->curve_central.control_count--;
		road->curve_right.control_count--;
		road->curve_left.control_count--; 
		return 0;
	}
	else if(n==0){
		double xn,yn;
		Control P0=road->curve_central.controls[1], P1=road->curve_central.controls[0];
		compute_normale_right(P0.x,P1.x,P0.y, P1.y,&xn,&yn);
		road->curve_left.controls[0].x=P1.x-xn*ROAD_LARGE, road->curve_left.controls[0].y=P1.y-yn*ROAD_LARGE;
		road->curve_right.controls[0].x=P1.x+xn*ROAD_LARGE, road->curve_right.controls[0].y=P1.y+yn*ROAD_LARGE;
	}
	else if(n==road->curve_central.control_count-2 && road->curve_central.control_count>=4){
		double xn,yn;
		Control P0=road->curve_central.controls[road->curve_central.control_count-1], P1=road->curve_central.controls[road->curve_central.control_count-3];
		compute_normale_right(P0.x,P1.x,P0.y, P1.y,&xn,&yn);
		//intersection a droite
		double xp=0,yp=0;
		double xs0=road->curve_right.controls[road->curve_right.control_count-4].x, xs1=road->curve_right.controls[road->curve_right.control_count-3].x, xt0=P1.x+xn*ROAD_LARGE, xt1=P0.x+xn*ROAD_LARGE;
		double ys0=road->curve_right.controls[road->curve_right.control_count-4].y, ys1=road->curve_right.controls[road->curve_right.control_count-3].y, yt0=P1.y+yn*ROAD_LARGE, yt1=P0.y+yn*ROAD_LARGE;
		compute_intersection(xs0,ys0,xs1,ys1,xt0,yt0,xt1,yt1,&xp,&yp);
		road->curve_right.controls[road->curve_central.control_count-3].x=xp;
		road->curve_right.controls[road->curve_central.control_count-3].y=yp;
		road->curve_right.controls[road->curve_central.control_count-2].x=xt1;
		road->curve_right.controls[road->curve_central.control_count-2].y=yt1;
		//intersection a gauche
		xs0=road->curve_left.controls[road->curve_left.control_count-4].x, xs1=road->curve_left.controls[road->curve_left.control_count-3].x, xt0=P1.x-xn*ROAD_LARGE, xt1=P0.x-xn*ROAD_LARGE;
		ys0=road->curve_left.controls[road->curve_left.control_count-4].y, ys1=road->curve_left.controls[road->curve_left.control_count-3].y, yt0=P1.y-yn*ROAD_LARGE, yt1=P0.y-yn*ROAD_LARGE;
		compute_intersection(xs0,ys0,xs1,ys1,xt0,yt0,xt1,yt1,&xp,&yp);
		road->curve_left.controls[road->curve_central.control_count-3].x=xp;
		road->curve_left.controls[road->curve_central.control_count-3].y=yp;
		road->curve_left.controls[road->curve_central.control_count-2].x=xt1;
		road->curve_left.controls[road->curve_central.control_count-2].y=yt1;
	}
	else if(n==1 && road->curve_central.control_count>=4){
		double xn,yn;
		Control P0=road->curve_central.controls[0], P1=road->curve_central.controls[1];
		compute_normale_right(P0.x,P1.x,P0.y, P1.y,&xn,&yn);
		//intersection a droite
		double xp=0,yp=0;
		double xs0=road->curve_right.controls[2].x, xs1=road->curve_right.controls[1].x, xt0=P1.x-xn*ROAD_LARGE, xt1=P0.x-xn*ROAD_LARGE;
		double ys0=road->curve_right.controls[2].y, ys1=road->curve_right.controls[1].y, yt0=P1.y-yn*ROAD_LARGE, yt1=P0.y-yn*ROAD_LARGE;
		compute_intersection(xs0,ys0,xs1,ys1,xt0,yt0,xt1,yt1,&xp,&yp);
		road->curve_right.controls[1].x=xp;
		road->curve_right.controls[1].y=yp;
		road->curve_right.controls[0].x=xt1;
		road->curve_right.controls[0].y=yt1;
		//intersection a gauche
		xs0=road->curve_left.controls[2].x, xs1=road->curve_left.controls[1].x, xt0=P1.x+xn*ROAD_LARGE, xt1=P0.x+xn*ROAD_LARGE;
		ys0=road->curve_left.controls[2].y, ys1=road->curve_left.controls[1].y, yt0=P1.y+yn*ROAD_LARGE, yt1=P0.y+yn*ROAD_LARGE;
		compute_intersection(xs0,ys0,xs1,ys1,xt0,yt0,xt1,yt1,&xp,&yp);
		road->curve_left.controls[1].x=xp;
		road->curve_left.controls[1].y=yp;
		road->curve_left.controls[0].x=xt1;
		road->curve_left.controls[0].y=yt1;
	}	
	else if(n==road->curve_central.control_count-1){
		double xn,yn;
		Control P0=road->curve_central.controls[n-1], P1=road->curve_central.controls[n-2];
		compute_normale_right(P0.x,P1.x,P0.y, P1.y,&xn,&yn);
		road->curve_left.controls[n-1].x=P0.x-xn*ROAD_LARGE, road->curve_left.controls[n-1].y=P0.y-yn*ROAD_LARGE;
		road->curve_right.controls[n-1].x=P0.x+xn*ROAD_LARGE, road->curve_right.controls[n-1].y=P0.y+yn*ROAD_LARGE;
	}
	else if(road->curve_central.control_count==3){
		double xn,yn;
		Control P0=road->curve_central.controls[1], P1=road->curve_central.controls[0];
		compute_normale_right(P0.x,P1.x,P0.y, P1.y,&xn,&yn);
		road->curve_left.controls[0].x=P1.x-xn*ROAD_LARGE, road->curve_left.controls[0].y=P1.y-yn*ROAD_LARGE;
		road->curve_right.controls[0].x=P1.x+xn*ROAD_LARGE, road->curve_right.controls[0].y=P1.y+yn*ROAD_LARGE;
		road->curve_left.controls[1].x=P0.x-xn*ROAD_LARGE, road->curve_left.controls[1].y=P0.y-yn*ROAD_LARGE;
		road->curve_right.controls[1].x=P0.x+xn*ROAD_LARGE, road->curve_right.controls[1].y=P0.y+yn*ROAD_LARGE;
	}
	else{
		double xn,yn;
		Control P0=road->curve_central.controls[n], P1=road->curve_central.controls[n-1];
		compute_normale_right(P0.x,P1.x,P0.y, P1.y,&xn,&yn);
		//intersection a droite
		double xp=0,yp=0;
		double xs0=road->curve_right.controls[n-2].x, xs1=road->curve_right.controls[n-1].x, xt0=P1.x+xn*ROAD_LARGE, xt1=P0.x+xn*ROAD_LARGE;
		double ys0=road->curve_right.controls[n-2].y, ys1=road->curve_right.controls[n-1].y, yt0=P1.y+yn*ROAD_LARGE, yt1=P0.y+yn*ROAD_LARGE;
		compute_intersection(xs0,ys0,xs1,ys1,xt0,yt0,xt1,yt1,&xp,&yp);
		road->curve_right.controls[n-1].x=xp;
		road->curve_right.controls[n-1].y=yp;
		road->curve_right.controls[n].x=xt1;
		road->curve_right.controls[n].y=yt1;
		//intersection a gauche
		xs0=road->curve_left.controls[n-2].x, xs1=road->curve_left.controls[n-1].x, xt0=P1.x-xn*ROAD_LARGE, xt1=P0.x-xn*ROAD_LARGE;
		ys0=road->curve_left.controls[n-2].y, ys1=road->curve_left.controls[n-1].y, yt0=P1.y-yn*ROAD_LARGE, yt1=P0.y-yn*ROAD_LARGE;
		compute_intersection(xs0,ys0,xs1,ys1,xt0,yt0,xt1,yt1,&xp,&yp);
		road->curve_left.controls[n-1].x=xp;
		road->curve_left.controls[n-1].y=yp;
		road->curve_left.controls[n].x=xt1;
		road->curve_left.controls[n].y=yt1;
		
		
		P0=road->curve_central.controls[n+1], P1=road->curve_central.controls[n];
		compute_normale_right(P0.x,P1.x,P0.y, P1.y,&xn,&yn);
		//intersection a droite
		xp=0,yp=0;
		xs0=road->curve_right.controls[n-1].x, xs1=road->curve_right.controls[n].x, xt0=P1.x+xn*ROAD_LARGE, xt1=P0.x+xn*ROAD_LARGE;
		ys0=road->curve_right.controls[n-1].y, ys1=road->curve_right.controls[n].y, yt0=P1.y+yn*ROAD_LARGE, yt1=P0.y+yn*ROAD_LARGE;
		compute_intersection(xs0,ys0,xs1,ys1,xt0,yt0,xt1,yt1,&xp,&yp);
		road->curve_right.controls[n].x=xp;
		road->curve_right.controls[n].y=yp;
		road->curve_right.controls[n+1].x=xt1;
		road->curve_right.controls[n+1].y=yt1;
		//intersection a gauche
		xs0=road->curve_left.controls[n-1].x, xs1=road->curve_left.controls[n].x, xt0=P1.x-xn*ROAD_LARGE, xt1=P0.x-xn*ROAD_LARGE;
		ys0=road->curve_left.controls[n-1].y, ys1=road->curve_left.controls[n].y, yt0=P1.y-yn*ROAD_LARGE, yt1=P0.y-yn*ROAD_LARGE;
		compute_intersection(xs0,ys0,xs1,ys1,xt0,yt0,xt1,yt1,&xp,&yp);
		road->curve_left.controls[n].x=xp;
		road->curve_left.controls[n].y=yp;
		road->curve_left.controls[n+1].x=xt1;
		road->curve_left.controls[n+1].y=yt1;
		
	}
	
	road->curve_central.control_count--;
	road->curve_right.control_count--;
	road->curve_left.control_count--; 
	return 0;
}



void move_control_from_road(Road *road,int nb_control, double dx, double dy){
	if(nb_control!=(-1)){
		road->curve_central.controls[nb_control].x+=dx;
		road->curve_central.controls[nb_control].y+=dy;
		
		road->curve_left.controls[nb_control].x+=dx;
		road->curve_left.controls[nb_control].y+=dy;
		
		road->curve_right.controls[nb_control].x+=dx;
		road->curve_right.controls[nb_control].y+=dy;
	}
}

int move_curve_from_road(Road *road, double dx, double dy){
	for(int i=0;i<road->curve_central.control_count;i++){
		road->curve_central.controls[i].x+=dx;
		road->curve_central.controls[i].y+=dy;
		road->curve_left.controls[i].x+=dx;
		road->curve_left.controls[i].y+=dy;
		road->curve_right.controls[i].x+=dx;
		road->curve_right.controls[i].y+=dy;
	}
	return 0;
}

int remove_curve_from_road(Road *road){
	Curve *curve=&road->curve_central;
	while(curve->control_count!=0)
		 remove_control_from_road(road,0);
return 0;
}

void compute_normale_right(double xa, double xb, double ya, double yb, double *xn, double *yn){
	double x=yb-ya;
	double y=xa-xb;
	double n=sqrt((x*x)+(y*y));
	if(n==0){
		*xn=0;
		*yn=0;
	}
	else{
		*xn=x/n;
		*yn=y/n;
	}
}

void compute_bezier_track (Curve *curve, Track *tra, double theta)
{
	Control bez_points[4];

	tra->sample_count = 0;
	if (curve->control_count < 3) return;

	compute_bezier_points_prolong_first (curve, bez_points);
	sample_bezier_curve (bez_points, theta, tra->sample_x, tra->sample_y,
		&tra->sample_count, SAMPLE_MAX, 1);

	for (int k = 0; k < curve->control_count-3; k++) {
	  compute_bezier_points_open (curve, k, bez_points);
	  sample_bezier_curve (bez_points, theta, tra->sample_x, tra->sample_y,
		  &tra->sample_count, SAMPLE_MAX, 0);
	}

	compute_bezier_points_prolong_last (curve, bez_points);
	sample_bezier_curve (bez_points, theta, tra->sample_x, tra->sample_y,
		&tra->sample_count, SAMPLE_MAX, 0);
}

void compute_road_tracks (Road *road, double theta)
{
	compute_bezier_track (&road->curve_left,    &road->track_left,    theta);
	compute_bezier_track (&road->curve_central, &road->track_central, theta);
	compute_bezier_track (&road->curve_right,   &road->track_right,   theta);
}

void save_road(Road *road, int lvl){
	if(lvl==0)return;
	FILE *svg;
	char level_file[20];
	sprintf(level_file,"Level/lvl%d",lvl);
	svg=fopen(level_file,"w");
	if(svg==NULL)return;
	fprintf(svg,"%lf\n",-1.0);
	fprintf(svg,"%d\n",road->curve_central.control_count);
	for(int i=0;i<road->curve_central.control_count;i++){
		fprintf(svg,"%lf %lf\n",road->curve_central.controls[i].x,road->curve_central.controls[i].y);
	}
	fprintf(svg,"%d\n",road->boost.boost_count);
	for(int i=0;i<road->boost.boost_count;i++){
		fprintf(svg,"%lf %lf\n",road->boost.boost_x[i],road->boost.boost_y[i]);
	}
	fclose(svg);
}

void save_time(Road *road,int lvl,double time){
	FILE *svg;
	char level_file[20];
	sprintf(level_file,"Level/lvl%d",lvl);
	svg=fopen(level_file,"w");
	if(svg==NULL)return;
	fprintf(svg,"%lf\n",time);
	fprintf(svg,"%d\n",road->curve_central.control_count);
	for(int i=0;i<road->curve_central.control_count;i++){
		fprintf(svg,"%lf %lf\n",road->curve_central.controls[i].x,road->curve_central.controls[i].y);
	}
	fprintf(svg,"%d\n",road->boost.boost_count);
	for(int i=0;i<road->boost.boost_count;i++){
		fprintf(svg,"%lf %lf\n",road->boost.boost_x[i],road->boost.boost_y[i]);
	}
	fclose(svg);
}

void load_road(Road *road, int lvl){
	int tmp;
	if(lvl==0)return;
	
	remove_curve_from_road(road);
	while(road->boost.boost_count>0)
		remove_boost(road,0);
	FILE *svg;
	char level_file[20];
	sprintf(level_file,"Level/lvl%d",lvl);
	printf("%s\n",level_file);
	svg=fopen(level_file,"r");
	int nb_control=0;
	tmp=fscanf(svg,"%lf",&road->temps);
	tmp=fscanf(svg,"%d",&nb_control);
	double x,y;
	for(int i=0;i<nb_control;i++){
		tmp=fscanf(svg,"%lf %lf",&x,&y);
		add_control_from_road(road,x,y);
	}
	nb_control=0;
	tmp=fscanf(svg,"%d",&nb_control);
	for(int i=0;i<nb_control;i++){
		tmp=fscanf(svg,"%lf %lf",&x,&y);
		add_boost(road,x,y);
	}
	nb_control=tmp;
}
