#include <gtk/gtk.h>
#include "curve.h"
#include "game.h"
#include "mydata.h"


Mydata *get_mydata (gpointer data)
{
  if (data == NULL) {
  	fprintf (stderr, "get_mydata: NULL data\n"); return NULL;
  }
  if (((Mydata *)data)->magic != MYDATA_MAGIC) {
  	fprintf (stderr, "get_mydata: bad magic number\n"); return NULL;
  }
  return data;
}

void init_mydata (Mydata *my)
{
  my->title = "Space Runner";
  my->num_lvl =1;
  my->click_x=0;
  my->click_y=0;
  my->click_n=0;
  my->last_x=0;
  my->last_y=0;
  my->depart=4;
  my->time=0;
  my->magic = MYDATA_MAGIC;
  my->pixbuf2 = gdk_pixbuf_new_from_file("Image/Fond.png",NULL);
  my->pixbuf_road = gdk_pixbuf_new_from_file("Image/Route_arc_en_ciel.png",NULL);
  my->pixbuf_j1=NULL;
  my->pixbuf_j2=NULL;
  my->pixbuf_j3=NULL;
  my->pixbuf_j4=NULL;
  my->pixbuf_boost=gdk_pixbuf_new_from_file("Image/Boost.png",NULL);
  my->show_edit=FALSE;
  my->wind_width=1000;
  my->wind_height=600;
  my->edit_mode=EDIT_ADD_CURVE;
  my->bsp_mode=BSP_WALL;
  my->current_control=-1;
  init_curve_infos(&my->curve_infos);
  init_game(&my->game);
  my->timeout1 = 0;
  
  my->flag_key_left = 0, my->flag_key_right = 0, my->flag_key_up = 0, my->flag_key_down = 0; //joueur1
  my->flag_key_z = 0, my->flag_key_q = 0, my->flag_key_s = 0, my->flag_key_d = 0; //joueur2
  my->flag_key_i = 0, my->flag_key_j = 0, my->flag_key_k = 0, my->flag_key_l = 0; //joueur3
  my->flag_key_g = 0, my->flag_key_v = 0, my->flag_key_b = 0, my->flag_key_n = 0; //joueur4
}

void set_edit_mode (Mydata *my, int mode)
{
  if(mode<EDIT_NONE || mode>EDIT_LAST) return;
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(my->edit_radios[mode-1]),TRUE);
  
}
