#include <gtk/gtk.h>
#include <math.h>
#include "util.h"
#include "curve.h"
#include "mydata.h"
#include "font.h"
#include "game.h"
#include "area1.h"

void set_anim1_mode (Mydata *my, int flag)
{
	if (flag) {
		if (my->timeout1 != 0) return;
		my->timeout1 = g_timeout_add (ANIM1_TIME, on_timeout1, my);

		set_status (my->status, "Animation 1 started");
	} else {
		if (my->timeout1 == 0) return;
		g_source_remove (my->timeout1);
		my->timeout1 = 0;

		set_status (my->status, "Animation 1 stopped");
	}
}

gboolean on_timeout1 (gpointer data)
{
  Mydata *my = get_mydata(data);
  Game *game = &my->game;
  
  if(game->state==GS_WON){
	  set_anim1_mode(my,FALSE);
	  if(game->road.temps==(-1.000000) || my->time<game->road.temps)
			save_time(&game->road,my->num_lvl,my->time);
			
	  return TRUE;}
  
  my->time+=1.0/26.0;
  printf("%lf\n",my->time);
  
  game->cars[0].direction =(my->flag_key_left  && !my->flag_key_right) ? -1 :
                (my->flag_key_right && !my->flag_key_left ) ?  1 : 0;
  game->cars[0].accelerator =(my->flag_key_up  && !my->flag_key_down) ? 1 :
                (my->flag_key_down && !my->flag_key_up ) ?  -1 : 0;     
                
  game->cars[1].direction =(my->flag_key_q  && !my->flag_key_d) ? -1 :
                (my->flag_key_d && !my->flag_key_q ) ?  1 : 0;
  game->cars[1].accelerator =(my->flag_key_z  && !my->flag_key_s) ? 1 :
                (my->flag_key_s && !my->flag_key_z ) ?  -1 : 0;          
  
  game->cars[2].direction =(my->flag_key_j  && !my->flag_key_l) ? -1 :
                (my->flag_key_l && !my->flag_key_j ) ?  1 : 0;
  game->cars[2].accelerator =(my->flag_key_i  && !my->flag_key_k) ? 1 :
                (my->flag_key_k && !my->flag_key_i ) ?  -1 : 0;                 
  
  game->cars[3].direction =(my->flag_key_v  && !my->flag_key_n) ? -1 :
                (my->flag_key_n && !my->flag_key_v ) ?  1 : 0;
  game->cars[3].accelerator =(my->flag_key_g  && !my->flag_key_b) ? 1 :
                (my->flag_key_b && !my->flag_key_g ) ?  -1 : 0;                 
  
  progress_game_next_step (game);
  
  refresh_area (my->area1);
  return TRUE;
}
/*
void Chrono_depart(Mydata *my, PangoLayout *layout, cairo_t *cr){
	PangoFontDescription *desc;
	desc = pango_font_description_from_string ("Serif, bold 16");
	cairo_set_source_rgb(cr,1.0,1.0,1.0);
	pango_layout_set_font_description (layout, desc);
	pango_font_description_free (desc);
		
		pango_layout_set_text (layout,
		"3", -1);
		cairo_move_to (cr, 220, 200);
		pango_cairo_show_layout (cr, layout);
		refresh_area(my->area1);
		
		sleep(1);
		
		pango_layout_set_text (layout,
		"2", -1);
		cairo_move_to (cr, 220, 200);
		pango_cairo_show_layout (cr, layout);
		refresh_area(my->area1);
		
		sleep(1);
		
		pango_layout_set_text (layout,
		"1", -1);
		cairo_move_to (cr, 220, 200);
		pango_cairo_show_layout (cr, layout);
		refresh_area(my->area1);
	
		sleep(1);
	
		pango_layout_set_text (layout,
		"Partez", -1);
		cairo_move_to (cr, 220, 200);
		pango_cairo_show_layout (cr, layout);
		refresh_area(my->area1);
	g_object_unref (layout);
}
*/
void draw_boost(cairo_t *cr, Mydata *my){
	int x=-1,y=-1;
	for(int i=0;i<my->game.road.boost.boost_count;i++){
		gdk_cairo_set_source_pixbuf(cr,my->pixbuf_boost,my->game.road.boost.boost_x[i]-5,my->game.road.boost.boost_y[i]-5);
		x=my->game.road.boost.boost_x[i], y=my->game.road.boost.boost_y[i];
		cairo_rectangle (cr,x-5,y-5, 11, 11);
		cairo_fill(cr);
	}	
}


void draw_cars(cairo_t *cr, Mydata *my){
	switch(my->game.car_count){
		case 4:
			my->pixbuf_j4=gdk_pixbuf_new_from_file("Image/Vaisseau4.png",NULL);
			gdk_cairo_set_source_pixbuf(cr,my->pixbuf_j4,my->game.cars[3].x-5,my->game.cars[3].y-5);
			cairo_arc(cr,my->game.cars[3].x,my->game.cars[3].y,my->game.cars[3].radius,0,G_PI*2);
			cairo_fill(cr);
		
		case 3 :
			my->pixbuf_j3=gdk_pixbuf_new_from_file("Image/Vaisseau3.png",NULL);
			gdk_cairo_set_source_pixbuf(cr,my->pixbuf_j3,my->game.cars[2].x-5,my->game.cars[2].y-5);
			cairo_arc(cr,my->game.cars[2].x,my->game.cars[2].y,my->game.cars[2].radius,0,G_PI*2);
			cairo_fill(cr);
			
		case 2 :
			my->pixbuf_j2=gdk_pixbuf_new_from_file("Image/Vaisseau2.png",NULL);
			gdk_cairo_set_source_pixbuf(cr,my->pixbuf_j2,my->game.cars[1].x-5,my->game.cars[1].y-5);
			cairo_arc(cr,my->game.cars[1].x,my->game.cars[1].y,my->game.cars[1].radius,0,G_PI*2);
			cairo_fill(cr);
			
		case 1 :
			my->pixbuf_j1=gdk_pixbuf_new_from_file("Image/Vaisseau1.png",NULL);
			gdk_cairo_set_source_pixbuf(cr,my->pixbuf_j1,my->game.cars[0].x-5,my->game.cars[0].y-5);
			cairo_arc(cr,my->game.cars[0].x,my->game.cars[0].y,my->game.cars[0].radius,0,G_PI*2);
			cairo_fill(cr);
	
	}
	cairo_set_source_rgb(cr,1.0,1.0,1.0);
	for(int i=0;i<my->game.car_count;i++){
		cairo_move_to(cr,my->game.cars[i].x+cos(my->game.cars[i].angle)*6,my->game.cars[i].y+sin(my->game.cars[i].angle)*6);
		cairo_line_to(cr,my->game.cars[i].x+cos(my->game.cars[i].angle)*15,my->game.cars[i].y+sin(my->game.cars[i].angle)*15);
		cairo_stroke(cr);
		cairo_move_to(cr,my->game.cars[i].x+cos(my->game.cars[i].angle)*8,my->game.cars[i].y+sin(my->game.cars[i].angle)*8);
		cairo_line_to(cr,my->game.cars[i].x+cos(my->game.cars[i].angle+G_PI/9)*10,my->game.cars[i].y+sin(my->game.cars[i].angle+G_PI/9)*10);
		cairo_line_to(cr,my->game.cars[i].x+cos(my->game.cars[i].angle)*15,my->game.cars[i].y+sin(my->game.cars[i].angle)*15);
		cairo_line_to(cr,my->game.cars[i].x+cos(my->game.cars[i].angle-G_PI/9)*10,my->game.cars[i].y+sin(my->game.cars[i].angle-G_PI/9)*10);
		cairo_fill(cr);
	}
}

void draw_track (cairo_t *cr, Mydata *my)
{
	if (my->game.road.track_central.sample_count == 0) return;
	gdk_cairo_set_source_pixbuf(cr,my->pixbuf_road,0,0);
	cairo_move_to (cr, my->game.road.track_left.sample_x[0], my->game.road.track_left.sample_y[0]);
	for (int k = 1; k < my->game.road.track_left.sample_count; k++)
	  cairo_line_to (cr, my->game.road.track_left.sample_x[k], my->game.road.track_left.sample_y[k]);
	
	for (int k = my->game.road.track_right.sample_count-1; k >= 0; k--)
	  cairo_line_to (cr, my->game.road.track_right.sample_x[k], my->game.road.track_right.sample_y[k]);
	  
	  
	cairo_fill (cr);
}

void draw_road_curve(cairo_t *cr, Road *road, Mydata *my)
{
	
	draw_bezier_curves_prolong(cr,&road->curve_central,0.02);
	cairo_set_source_rgb(cr,0,1.0,0);
	draw_bezier_curves_prolong(cr,&road->curve_left,0.02);
	cairo_set_source_rgb(cr,1.0,0,0);
	draw_bezier_curves_prolong(cr,&road->curve_right,0.02);
}

void draw_road_wall(cairo_t *cr, Road *road, Mydata *my)
{
		draw_bezier_curves_prolong(cr,&my->game.road.curve_central,0.02);
		cairo_set_source_rgb(cr,1.0,0,0);
		for(int i=0;i<road->curve_right.control_count-1;i++){
			cairo_move_to(cr,road->curve_left.controls[i].x,road->curve_left.controls[i].y);
			cairo_line_to(cr,road->curve_left.controls[i+1].x,road->curve_left.controls[i+1].y);
			cairo_stroke(cr);
		}
		cairo_set_source_rgb(cr,0,1.0,0);
		for(int i=0;i<road->curve_right.control_count-1;i++){
			cairo_move_to(cr,road->curve_right.controls[i].x,road->curve_right.controls[i].y);
			cairo_line_to(cr,road->curve_right.controls[i+1].x,road->curve_right.controls[i+1].y);
			cairo_stroke(cr);
		}	
}

void generate_bezier_path (cairo_t *cr, Control bez_points[4], double theta, int is_first)
{
	double bx[4]={bez_points[0].x,bez_points[1].x,bez_points[2].x,bez_points[3].x};
	double by[4]={bez_points[0].y,bez_points[1].y,bez_points[2].y,bez_points[3].y};
	double x=compute_bezier_cubic(bx,0),	y=compute_bezier_cubic(by,0);
	if(is_first==TRUE)cairo_move_to(cr,x,y);
	else cairo_line_to(cr,x,y);
	for(double i=theta;i<1;i+=theta){
		double x2=compute_bezier_cubic(bx,i);
		double y2=compute_bezier_cubic(by,i);
		cairo_line_to(cr,x2,y2);
		x=x2,y=y2;
	}
	double x2=compute_bezier_cubic(bx,1);
	double y2=compute_bezier_cubic(by,1);
	cairo_line_to(cr,x2,y2);
} 


void draw_bezier_curve (cairo_t *cr, Control bez_points[4], double theta)
{
	cairo_set_source_rgb(cr,0.40,0.20,0.27);
	double bx[4]={bez_points[0].x,bez_points[1].x,bez_points[2].x,bez_points[3].x};
	double by[4]={bez_points[0].y,bez_points[1].y,bez_points[2].y,bez_points[3].y};
	double x=compute_bezier_cubic(bx,0),	y=compute_bezier_cubic(by,0);
	for(double i=theta;i<1;i+=theta){
		double x2=compute_bezier_cubic(bx,i);
		double y2=compute_bezier_cubic(by,i);
		cairo_move_to(cr,x,y);
		cairo_line_to(cr,x2,y2);
		cairo_stroke(cr);
		x=x2,y=y2;
	}
	double x2=compute_bezier_cubic(bx,1);
	double y2=compute_bezier_cubic(by,1);
	cairo_move_to(cr,x,y);
	cairo_line_to(cr,x2,y2);
	cairo_stroke(cr);
}

void draw_bezier_curves_prolong (cairo_t *cr, Curve *curve, double theta){
	Control bez_points[4];
		for(int j=0;j<curve->control_count-3;j++){
			compute_bezier_points_open(curve,j,bez_points);
			draw_bezier_curve(cr,bez_points,theta);
		}
		if(curve->control_count>=3){
			compute_bezier_points_prolong_first(curve,bez_points);
			draw_bezier_curve(cr,bez_points,theta);
			compute_bezier_points_prolong_last(curve,bez_points);
			draw_bezier_curve(cr,bez_points,theta);
		}
	}


void draw_bezier_curves_open (cairo_t *cr, Curve_infos *ci, double theta){
	Control bez_points[4];
	for(int i=0;i<ci->curve_list.curve_count;i++){
		for(int j=0;j<ci->curve_list.curves[i].control_count-3;j++){
			compute_bezier_points_open(&ci->curve_list.curves[i],j,bez_points);
			draw_bezier_curve(cr,bez_points,theta);
		}
	}
}


void draw_bezier_polygons_open (cairo_t *cr, Curve_infos *ci)
{
	Control bez_points[4];
	cairo_set_source_rgb(cr,0,1.0,0);
	for(int i=0;i<ci->curve_list.curve_count;i++){
		for(int j=0;j<ci->curve_list.curves[i].control_count-3;j++){
			compute_bezier_points_open(&ci->curve_list.curves[i],j,bez_points);
			cairo_move_to(cr,bez_points[0].x,bez_points[0].y);
			cairo_line_to(cr,bez_points[1].x,bez_points[1].y);
			cairo_move_to(cr,bez_points[2].x,bez_points[2].y);
			cairo_line_to(cr,bez_points[3].x,bez_points[3].y);
			cairo_stroke(cr);
		}
	}
}

void draw_control_labels(cairo_t *cr, PangoLayout *layout, Curve_infos *ci)
{
	font_set_name (layout, "Sans, 8");
	cairo_set_source_rgb(cr,0.25,0.25,0.25);
	for(int i=0;i<ci->curve_list.curve_count;i++)
    {
		for(int j=0;j<ci->curve_list.curves[i].control_count;j++)
		  font_draw_text (cr, layout,FONT_BC, ci->curve_list.curves[i].controls[j].x, ci->curve_list.curves[i].controls[j].y-5,"%d",j);
	}
}

void draw_control_polygons (cairo_t *cr, Road *road)
{  
  int x=-1,y=-1;
  
	for(int j=0;j<road->curve_central.control_count;j++)
	{
		cairo_set_source_rgb (cr, 1.0, 1.0, 0);
		if(j<road->curve_central.control_count-1)
		{
			cairo_move_to(cr,road->curve_central.controls[j].x,road->curve_central.controls[j].y);
			cairo_line_to(cr,road->curve_central.controls[j+1].x,road->curve_central.controls[j+1].y);
			cairo_stroke(cr);
	    }
		if (j==road->curve_central.control_count-1)
			cairo_set_source_rgb (cr, 1.0, 0, 0);
		else
			cairo_set_source_rgb (cr, 0, 0, 1.0);
		x=road->curve_central.controls[j].x, y=road->curve_central.controls[j].y;
		cairo_rectangle (cr,x-3,y-3, 6, 6);
		cairo_stroke(cr);
	  }
}


gboolean on_area1_button_press (GtkWidget *area,GdkEvent *event, gpointer data)
{
  Mydata *my = get_mydata(data);
  GdkEventButton *evb = &event->button;
  printf ("%s: %d %.1f %.1f\n",__func__, evb->button, evb->x, evb->y);
  my->click_x=evb->x, my->click_y=evb->y, my->click_n=evb->button;
  if(my->click_n==1 && my->show_edit==TRUE){
	switch (my->edit_mode) {
		case EDIT_ADD_CURVE :
			if(my->game.road.curve_central.control_count!=0)
				break;
			set_edit_mode(my,EDIT_ADD_CONTROL);
			add_control_from_road(&my->game.road,my->click_x, my->click_y);
			break;
			
		case EDIT_ADD_CONTROL :
			add_control_from_road(&my->game.road,my->click_x, my->click_y);
			break;
		case EDIT_MOVE_CURVE :
			find_control(&my->game.road.curve_central, my->click_x, my->click_y);
			break;
		case EDIT_MOVE_CONTROL :
			find_control(&my->game.road.curve_central, my->click_x, my->click_y);
			break;
		case EDIT_REMOVE_CURVE : 
			if(find_control(&my->game.road.curve_central, my->click_x, my->click_y)!=(-1)){
				remove_curve_from_road(&my->game.road);
			}
			break;
		case EDIT_REMOVE_CONTROL : 
			my->current_control=find_control(&my->game.road.curve_central, my->click_x, my->click_y);
			remove_control_from_road(&my->game.road,my->current_control);
			break;
		case EDIT_ADD_BOOST :
			add_boost(&my->game.road, my->click_x, my->click_y);
			break;
		case EDIT_MOVE_BOOST :
			find_boost(&my->game.road.boost,my->click_x, my->click_y);
			break;
		case EDIT_REMOVE_BOOST:
			remove_boost(&my->game.road,find_boost(&my->game.road.boost,my->click_x,my->click_y));
    }
  }
  refresh_area(my->area1);
  return TRUE;   //  ́evenement trait e
}

gboolean on_area1_button_release (GtkWidget *area,GdkEvent *event, gpointer data)
{
  Mydata *my = get_mydata(data);
  GdkEventButton *evb = &event->button;
  my->click_n=0;
  printf ("%s: %d %.1f %.1f\n",__func__, evb->button, evb->x, evb->y);
  refresh_area(my->area1);
  return TRUE;   //  ́ev ́enement trait ́e
}

gboolean on_area1_draw (GtkWidget *widget,cairo_t *cr, gpointer data)
{
  Mydata *my = get_mydata(data);
  PangoLayout *layout = pango_cairo_create_layout (cr);
  PangoFontDescription *desc;
  int width  = gtk_widget_get_allocated_width  (my->area1);
  int height = gtk_widget_get_allocated_height (my->area1);
  printf("on_area1_draw: %d %d\n",width,height);
  if(my->pixbuf2!=NULL)
  {
	  int width_pix1=gdk_pixbuf_get_width(my->pixbuf2);
	  int height_pix1=gdk_pixbuf_get_height(my->pixbuf2);
	  gdk_cairo_set_source_pixbuf(cr,my->pixbuf2,0,0);
	  cairo_rectangle (cr,0,0, width_pix1, height_pix1);
	  cairo_fill (cr);
  }
  switch(my->game.state){
  case GS_HELLO:
		desc = pango_font_description_from_string ("Serif, bold 16");
		pango_layout_set_font_description (layout, desc);
		pango_font_description_free (desc);
		pango_layout_set_text (layout,
		"		      Space Runner\n\n    Appuyez sur \"m\" pour Acceder au menu", -1);
		cairo_move_to (cr, 220, 200);
		cairo_set_source_rgb(cr,1.0,1.0,1.0);
		pango_cairo_show_layout (cr, layout);
	    
	  break;

  case GS_EDIT:
	  draw_control_polygons(cr,&my->game.road);
	  draw_bezier_polygons_open(cr,&my->curve_infos);
	  draw_control_labels(cr,layout,&my->curve_infos);
	  draw_boost(cr,my);
	  switch (my->bsp_mode) {
		case BSP_WALL :
			draw_road_wall(cr,&my->game.road,my);
			break;
		case BSP_CURVE :
			draw_road_curve(cr,&my->game.road,my);
			break;
		case BSP_TRACK:
			compute_road_tracks(&my->game.road,0.02);
			draw_track(cr,my);
			break;
		
		}
		break;
  case GS_WON:
	desc = pango_font_description_from_string ("Serif, bold 16");
	pango_layout_set_font_description (layout, desc);
	pango_font_description_free (desc);
	char msg[200];
	if(my->game.mode==GM_SINGLE)
		sprintf(msg,"		      Temps :%lfs\n		      Record:%lfs\nAppuyez sur Espace pour jouer le niveau suivant",my->time,my->game.road.temps);
	else{
		int gagnant;
		for(int i=0;i<CAR_MAX;i++)
			if(my->game.cars[i].is_winner==1){
				gagnant=i+1;
				printf("%d est le gagnant\n",i+1);
			}
		sprintf(msg,"	 Le gagnant est le joueur %d en %lfs\n		      Record:%lfs\nAppuyez sur Espace pour jouer le niveau suivant",gagnant,my->time,my->game.road.temps);
	}
	pango_layout_set_text (layout,msg, -1);
	cairo_move_to (cr, 220, 200);
	cairo_set_source_rgb(cr,1.0,1.0,1.0);
	pango_cairo_show_layout (cr, layout);
  case GS_PAUSE:	
  case GS_PLAYING:
	draw_track(cr,my);
	draw_boost(cr,my);
	draw_cars(cr,my);
	switch(my->depart){			
			case 4:
			cairo_set_source_rgb(cr,1.0,1.0,1.0);
			pango_layout_set_text (layout,
			"3", -1);
			cairo_move_to (cr, 220, 200);
			pango_cairo_show_layout (cr, layout);
			my->depart--;
			g_timeout_add_seconds (10,
               NULL,
               data);
			break;
			
			case 3:
			cairo_set_source_rgb(cr,1.0,1.0,1.0);
			pango_layout_set_text (layout,
			"2", -1);
			cairo_move_to (cr, 220, 200);
			pango_cairo_show_layout (cr, layout);
			my->depart--;
			break;
			
			case 2:
			cairo_set_source_rgb(cr,1.0,1.0,1.0);
			pango_layout_set_text (layout,
			"1", -1);
			cairo_move_to (cr, 220, 200);
			pango_cairo_show_layout (cr, layout);
			my->depart--;
			break;
		
			case 1:
			cairo_set_source_rgb(cr,1.0,1.0,1.0);
			pango_layout_set_text (layout,
			"Partez", -1);
			cairo_move_to (cr, 220, 200);
			pango_cairo_show_layout (cr, layout);
			my->depart--;
			break;
			
	}
	
	break;
		
  
	}
  g_object_unref (layout);
  return TRUE;
}

gboolean on_area1_enter_notify (GtkWidget *area,GdkEvent *event, gpointer data)
{
  Mydata *my = get_mydata(data);
  GdkEventCrossing *evc = &event->crossing;
  printf ("%s: %.1f %.1f\n", __func__, evc->x, evc->y);
  gtk_widget_grab_focus (my->area1);
  return TRUE;   //  ́ev ́enement trait ́e
}

gboolean on_area1_leave_notify (GtkWidget *area,GdkEvent *event, gpointer data)
{
  //Mydata *my = get_mydata(user_data);
  GdkEventCrossing *evc = &event->crossing;
  printf ("%s: %.1f %.1f\n", __func__, evc->x, evc->y);
  return TRUE;   //  ́ev ́enement trait ́e
}

gboolean on_area1_key_press (GtkWidget *area,GdkEvent *event, gpointer data)
{
  Mydata *my = get_mydata(data);
  GdkEventKey *evk = &event->key;
  printf ("%s: GDK_KEY_%s\n",__func__, gdk_keyval_name(evk->keyval));
  switch (evk->keyval) {
	case GDK_KEY_Escape : gtk_widget_destroy(my->window); break;
	
	case GDK_KEY_m : 
		gtk_widget_show(my->hbox_menu);
		
		break;
	
	case GDK_KEY_p : if(my->game.state==GS_PLAYING){
					 set_GS_mode(&my->game,GS_PAUSE); set_anim1_mode(my,FALSE);}
					 break;
	case GDK_KEY_space : if(my->game.state==GS_PAUSE){
							set_GS_mode(&my->game,GS_PLAYING); set_anim1_mode(my,TRUE);}
						 
						 else if(my->game.state==GS_WON){
							 my->num_lvl++;
							 if(my->num_lvl==11)
								my->num_lvl=1;
							 load_road(&my->game.road,my->num_lvl);
							 my->time=0;
							 compute_road_tracks(&my->game.road,0.02);
							 place_cars_on_start(&my->game);
							 set_GS_mode(&my->game,GS_PLAYING);
							 set_anim1_mode(my,TRUE);
						 }
					
					 break;
	case GDK_KEY_Left  : my->flag_key_left  = 1; break;
    case GDK_KEY_Right : my->flag_key_right = 1; break;
    case GDK_KEY_Up  : my->flag_key_up  = 1; break;
    case GDK_KEY_Down : my->flag_key_down  = 1; break;
    
    case GDK_KEY_z  : my->flag_key_z  = 1; break;
    case GDK_KEY_q : my->flag_key_q = 1; break;
    case GDK_KEY_s  : my->flag_key_s  = 1; break;
    case GDK_KEY_d : my->flag_key_d  = 1; break;
    
    case GDK_KEY_i  : my->flag_key_i  = 1; break;
    case GDK_KEY_j : my->flag_key_j = 1; break;
    case GDK_KEY_k  : my->flag_key_k  = 1; break;
    case GDK_KEY_l : my->flag_key_l  = 1; break;
    
    case GDK_KEY_g  : my->flag_key_g  = 1; break;
    case GDK_KEY_v : my->flag_key_v = 1; break;
    case GDK_KEY_b  : my->flag_key_b  = 1; break;
    case GDK_KEY_n : my->flag_key_n  = 1; break;
  }
  return TRUE;  //  ́ev ́enement trait ́e
}

gboolean on_area1_key_release (GtkWidget *area,GdkEvent *event, gpointer data)
{
  Mydata *my = get_mydata(data);
  GdkEventKey *evk = &event->key;
  printf ("%s: GDK_KEY_%s\n",__func__, gdk_keyval_name(evk->keyval));
  switch (evk->keyval) {
	  case GDK_KEY_Left  : my->flag_key_left  = 0; break;
      case GDK_KEY_Right : my->flag_key_right = 0; break;
      case GDK_KEY_Up  : my->flag_key_up  = 0; break;
      case GDK_KEY_Down  : my->flag_key_down  = 0; break;
      
      case GDK_KEY_z  : my->flag_key_z  = 0; break;
	  case GDK_KEY_q : my->flag_key_q = 0; break;
      case GDK_KEY_s  : my->flag_key_s  = 0; break;
      case GDK_KEY_d : my->flag_key_d  = 0; break;
    
      case GDK_KEY_i  : my->flag_key_i  = 0; break;
      case GDK_KEY_j : my->flag_key_j = 0; break;
      case GDK_KEY_k  : my->flag_key_k  = 0; break;
      case GDK_KEY_l : my->flag_key_l  = 0; break;
    
      case GDK_KEY_g  : my->flag_key_g  = 0; break;
      case GDK_KEY_v : my->flag_key_v = 0; break;
      case GDK_KEY_b  : my->flag_key_b  = 0; break;
      case GDK_KEY_n : my->flag_key_n  = 0; break;
  }  
  return TRUE;  //  ́ev ́enement trait ́e
}

gboolean on_area1_motion_notify (GtkWidget *area,GdkEvent *event, gpointer data)
{
  Mydata *my = get_mydata(data);
  GdkEventMotion *evm = &event->motion;
  if(my->click_n==1)
  {
	my->last_x=my->click_x;
	my->last_y=my->click_y;
	my->click_x=evm->x;
	my->click_y=evm->y;
	if(my->show_edit==TRUE){
		switch(my->edit_mode){
			case EDIT_MOVE_CONTROL : 
				my->current_control=find_control(&my->game.road.curve_central,my->click_x,my->click_y);
				move_control_from_road(&my->game.road,my->current_control,my->click_x-my->last_x,my->click_y-my->last_y);
				break;
			case EDIT_MOVE_CURVE : 
				move_curve_from_road(&my->game.road,my->click_x-my->last_x,my->click_y-my->last_y);
				break;
			case EDIT_MOVE_BOOST :
				move_boost(&my->game.road,find_boost(&my->game.road.boost,my->click_x, my->click_y),my->click_x-my->last_x,my->click_y-my->last_y);
				break;
		}
	}
	refresh_area(my->area1);
  }

  printf ("%s: %.1f %.1f\n", __func__, evm->x, evm->y);
  return TRUE;   //  ́ev ́enement trait ́e
}

void area1_init(gpointer user_data)
{
  Mydata *my = get_mydata(user_data);
  my->scroll = gtk_scrolled_window_new(NULL,NULL);
  
  my->area1 = gtk_drawing_area_new ();
  

  gtk_widget_add_events (my->area1,GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK |GDK_FOCUS_CHANGE_MASK |GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK |GDK_POINTER_MOTION_MASK |GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK);

  gtk_widget_set_can_focus (my->area1, TRUE);

  g_signal_connect (my->area1, "draw",G_CALLBACK(on_area1_draw), my);
  g_signal_connect (my->area1, "key-press-event",G_CALLBACK(on_area1_key_press), my);
  g_signal_connect (my->area1, "key-release-event",G_CALLBACK(on_area1_key_release), my);
  g_signal_connect (my->area1, "button-press-event",G_CALLBACK(on_area1_button_press), my);
  g_signal_connect (my->area1, "button-release-event",G_CALLBACK(on_area1_button_release), my);
  g_signal_connect (my->area1, "motion-notify-event",G_CALLBACK(on_area1_motion_notify), my);
  g_signal_connect (my->area1, "enter-notify-event",G_CALLBACK(on_area1_enter_notify), my);
  g_signal_connect (my->area1, "leave-notify-event",G_CALLBACK(on_area1_leave_notify), my);
}
