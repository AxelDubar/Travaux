#ifndef MYDATA_H
#define MYDATA_H

#include "curve.h"
#include "game.h"

#define MYDATA_MAGIC 0x46EA7E05

enum{ EDIT_NONE, EDIT_ADD_CURVE, EDIT_MOVE_CURVE, EDIT_REMOVE_CURVE, EDIT_ADD_CONTROL, EDIT_MOVE_CONTROL, EDIT_REMOVE_CONTROL, EDIT_ADD_BOOST, EDIT_MOVE_BOOST, EDIT_REMOVE_BOOST, EDIT_LAST};

enum{ BSP_FIRST, BSP_WALL, BSP_CURVE , BSP_TRACK,BSP_LAST};

typedef struct {
	unsigned int magic;
	GtkWidget *window, *status, *hbox1, *hbox_menu, *vbox1, *win_scale, *win_save, *win_load, *scale1, *area1, *menu_bar, *scroll, *frame1, *frame2, *frame_load, *frame_save, *combo_nbjoueur, *combo_lvl, *edit_radios[EDIT_LAST], *bsp_radios[BSP_LAST], *save_radios[11], *load_radios[11];
	GdkPixbuf *pixbuf2, *pixbuf_j1, *pixbuf_j2, *pixbuf_j3, *pixbuf_j4, *pixbuf_road, *pixbuf_boost;
	double click_x, click_y, click_n, last_x, last_y, time;
	char *title, *current_lvl;
	int wind_width, wind_height, show_edit, edit_mode, bsp_mode, current_control, num_lvl, depart;
	Curve_infos curve_infos;
	Game game;
	guint timeout1;
	int flag_key_left, flag_key_right, flag_key_up, flag_key_down; //joueur1
	int flag_key_z, flag_key_q, flag_key_s, flag_key_d; //joueur2
    int flag_key_i, flag_key_j, flag_key_k, flag_key_l; //joueur3
	int flag_key_g, flag_key_v, flag_key_b, flag_key_n; //joueur4
} Mydata;

Mydata *get_mydata (gpointer data);
void init_mydata (Mydata *my);
void set_edit_mode (Mydata *my, int mode);

#endif /* MYDATA_H */
