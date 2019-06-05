#include <gtk/gtk.h>
#include "util.h"
#include "area1.h"
#include "menus.h"
#include "mydata.h"
#include "game.h"
#include "gui.h"

void on_edit_radio_toggled(GtkWidget *radio,gpointer user_data)
{
  Mydata *my = get_mydata(user_data);
  my->edit_mode = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(radio),"mode"));
  printf("edit mod=%d\n",my->edit_mode);
} 

void on_bsp_radio_toggled(GtkWidget *radio,gpointer user_data)
{
  Mydata *my = get_mydata(user_data);
  my->bsp_mode = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(radio),"mode"));
  printf("bsp mod=%d\n",my->bsp_mode);
  refresh_area(my->area1);
} 

void on_button_save_clicked(GtkWidget *button, gpointer user_data){
	Mydata *my = get_mydata(user_data);
	save_road(&my->game.road,my->num_lvl);
	gtk_widget_hide(my->win_save);
}

void on_button_load_clicked(GtkWidget *button, gpointer user_data){
	Mydata *my = get_mydata(user_data);
	load_road(&my->game.road,my->num_lvl);
	refresh_area(my->area1);
	gtk_widget_hide(my->win_load);
}

void on_save_radio_toggled(GtkWidget *radio, gpointer user_data){
	Mydata *my = get_mydata(user_data);
	my->num_lvl = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(radio),"mode"));
}

void on_load_radio_toggled(GtkWidget *radio, gpointer user_data){
	Mydata *my = get_mydata(user_data);
	my->num_lvl = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(radio),"mode"));
}

void editing_init (Mydata *my)
{
  GtkWidget *vbox, *separator;
  vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);

  my->frame1 = gtk_frame_new("Editing");
  
  gtk_container_add (GTK_CONTAINER (my->frame1), vbox);
  char *noms[EDIT_LAST] = {"Add curve", "Move curve", "Remove curve", "Add control", "Move control", "Remove control", "Add Boost", "Move Boost", "Remove Boost"};
  for (int i = 0; i < EDIT_LAST-1; i++) {
	my->edit_radios[i] = gtk_radio_button_new_with_label_from_widget (i == 0 ? NULL : GTK_RADIO_BUTTON(my->edit_radios[0]),noms[i]);

	g_object_set_data (G_OBJECT(my->edit_radios[i]), "mode",GINT_TO_POINTER(i+1));

	g_signal_connect (my->edit_radios[i], "toggled",G_CALLBACK(on_edit_radio_toggled), my);

	gtk_box_pack_start (GTK_BOX (vbox),my->edit_radios[i], FALSE, FALSE, 0);
  }
  
  
  separator = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
  gtk_box_pack_start (GTK_BOX (vbox),separator, FALSE, FALSE, 0);
  
  char *noms_bsp[BSP_LAST] = {"Wall", "Curve", "Track"};
  for (int i = 0; i < BSP_LAST-1; i++) {
	my->bsp_radios[i] = gtk_radio_button_new_with_label_from_widget (i == 0 ? NULL : GTK_RADIO_BUTTON(my->bsp_radios[0]),noms_bsp[i]);

	g_object_set_data (G_OBJECT(my->bsp_radios[i]), "mode",GINT_TO_POINTER(i+1));

	g_signal_connect (my->bsp_radios[i], "toggled",G_CALLBACK(on_bsp_radio_toggled), my);

	gtk_box_pack_start (GTK_BOX (vbox),my->bsp_radios[i], FALSE, FALSE, 0);
  }
  
}

void layout_init(gpointer user_data)
{
  Mydata *my = get_mydata(user_data);
  my->vbox1 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  my->hbox1 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_container_add (GTK_CONTAINER (my->window), my->vbox1);

  //menus_init
  gtk_box_pack_start (GTK_BOX (my->vbox1), my->menu_bar,FALSE, FALSE,0);
  gtk_box_pack_start (GTK_BOX (my->vbox1), my->hbox1,TRUE, TRUE,0);
  //area1_init
  gtk_container_add (GTK_CONTAINER (my->hbox1), my->frame1);//Liste edition
  gtk_container_add (GTK_CONTAINER (my->hbox1), my->hbox_menu);
  gtk_box_pack_start (GTK_BOX (my->hbox1), my->scroll, TRUE, TRUE,0);
  
  gtk_container_add (GTK_CONTAINER(my->scroll), my->area1);
  
 
  //status_init
  gtk_box_pack_start (GTK_BOX (my->vbox1), my->status, FALSE, TRUE,0);
}

void window_init(GtkApplication *app,gpointer user_data)
{
  Mydata *my = get_mydata(user_data);
  my->window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (my->window), my->title);
  gtk_window_set_default_size (GTK_WINDOW (my->window),my->wind_width, my->wind_height);
}

void status_init(gpointer user_data)
{
  Mydata *my = get_mydata(user_data);
  my->status = gtk_statusbar_new();
  set_status(my->status, "Space Runner");
  
}

void win_load_init(Mydata *my)
{
  GtkWidget *load;
  
  my->win_load = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW(my->win_load),"Load");
  g_signal_connect (my->win_load, "delete-event",G_CALLBACK(gtk_widget_hide_on_delete),my);

  load = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_container_add (GTK_CONTAINER (my->win_load), load);
  
  
  char *noms_file[11] = {"Aucun","Level 1","Level 2","Level 3","Level 4","Level 5","Level 6","Level 7","Level 8","Level 9","Level 10",};

  for(int i=0; i<11;i++){
		my->load_radios[i] = gtk_radio_button_new_with_label_from_widget (i == 0 ? NULL : GTK_RADIO_BUTTON(my->load_radios[0]),noms_file[i]);

		g_object_set_data (G_OBJECT(my->load_radios[i]), "mode",GINT_TO_POINTER(i));

		g_signal_connect (my->load_radios[i], "toggled",G_CALLBACK(on_load_radio_toggled), my);

		gtk_box_pack_start (GTK_BOX (load),my->load_radios[i], TRUE, TRUE, 0);
	}
	

  GtkWidget *button_load;
  button_load=gtk_button_new_with_label ("Load");
  gtk_box_pack_start (GTK_BOX (load),button_load, FALSE, FALSE, 0);
  g_signal_connect (button_load, "clicked",G_CALLBACK(on_button_load_clicked), my);
  
  gtk_widget_show_all(my->win_load);
  
  gtk_widget_hide(my->win_load);
}

void win_save_init(Mydata *my)
{
  GtkWidget *save;
  
  my->win_save = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW(my->win_save),"Save");
  g_signal_connect (my->win_save, "delete-event",G_CALLBACK(gtk_widget_hide_on_delete),my);

  save = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_container_add (GTK_CONTAINER (my->win_save), save);
  
  
  char *noms_file[11] = {"Aucun","Level 1","Level 2","Level 3","Level 4","Level 5","Level 6","Level 7","Level 8","Level 9","Level 10",};

  for(int i=0; i<11;i++){
		my->save_radios[i] = gtk_radio_button_new_with_label_from_widget (i == 0 ? NULL : GTK_RADIO_BUTTON(my->save_radios[0]),noms_file[i]);

		g_object_set_data (G_OBJECT(my->save_radios[i]), "mode",GINT_TO_POINTER(i));

		g_signal_connect (my->save_radios[i], "toggled",G_CALLBACK(on_save_radio_toggled), my);

		gtk_box_pack_start (GTK_BOX (save),my->save_radios[i], TRUE, TRUE, 0);
	}
	

  GtkWidget *button_save;
  button_save=gtk_button_new_with_label ("Save");
  gtk_box_pack_start (GTK_BOX (save),button_save, FALSE, FALSE, 0);
  g_signal_connect (button_save, "clicked",G_CALLBACK(on_button_save_clicked), my);
  
  gtk_widget_show_all(my->win_save);
  
  gtk_widget_hide(my->win_save);
}

void on_button_edit_clicked(GtkWidget *button, gpointer data){
	Mydata *my = get_mydata(data);
	set_GS_mode(&my->game,GS_EDIT);
	my->show_edit=TRUE;
	gtk_widget_show(my->frame1);
	gtk_widget_hide(my->hbox_menu);
}

void on_button_play_clicked(GtkWidget *button, gpointer data){
	Mydata *my = get_mydata(data);
	set_GS_mode(&my->game,GS_PLAYING);
	my->time=0;
	my->game.car_count=gtk_combo_box_get_active (GTK_COMBO_BOX(my->combo_nbjoueur))+1;
	my->game.mode=gtk_combo_box_get_active (GTK_COMBO_BOX(my->combo_nbjoueur))+1;
	load_road(&my->game.road,gtk_combo_box_get_active (GTK_COMBO_BOX(my->combo_lvl))+1);
	my->num_lvl=gtk_combo_box_get_active (GTK_COMBO_BOX(my->combo_lvl))+1;
	compute_road_tracks(&my->game.road,0.02);
	place_cars_on_start(&my->game);	
	set_anim1_mode(my, TRUE);
	gtk_widget_hide(my->hbox_menu);
}

void on_button_quit_clicked(GtkWidget *button, gpointer data){
	Mydata *my = get_mydata(data);
	printf ("Closing window and app\n");
	gtk_widget_destroy(my->window);
}

void main_menu_init(Mydata *my){
	GtkWidget /* *hbox2,*/ *button_edit, *button_play, *button_quit;
	
	//my->frame5 = gtk_frame_new(NULL);
	my->hbox_menu = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	
	//gtk_container_add (GTK_CONTAINER (my->frame5), hbox2);
	
	button_edit = gtk_button_new_with_label ("Edit");
	gtk_box_pack_start (GTK_BOX (my->hbox_menu),button_edit, TRUE, TRUE, 0);
	g_signal_connect (button_edit, "clicked",G_CALLBACK(on_button_edit_clicked), my);
 
    button_play = gtk_button_new_with_label ("Play");
	gtk_box_pack_start (GTK_BOX (my->hbox_menu),button_play, TRUE, TRUE, 0);
	g_signal_connect (button_play, "clicked",G_CALLBACK(on_button_play_clicked), my);
 
    my->combo_nbjoueur = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text  (GTK_COMBO_BOX_TEXT (my->combo_nbjoueur), "Solo");
    gtk_combo_box_text_append_text  (GTK_COMBO_BOX_TEXT (my->combo_nbjoueur), "Deux Joueurs");
    gtk_combo_box_text_append_text  (GTK_COMBO_BOX_TEXT (my->combo_nbjoueur), "Trois Joueurs");
    gtk_combo_box_text_append_text  (GTK_COMBO_BOX_TEXT (my->combo_nbjoueur), "Quatre Joueurs");
    gtk_combo_box_set_active (GTK_COMBO_BOX(my->combo_nbjoueur), 0);
    gtk_box_pack_start (GTK_BOX (my->hbox_menu),my->combo_nbjoueur, TRUE, TRUE, 0);
    
    my->combo_lvl = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text  (GTK_COMBO_BOX_TEXT (my->combo_lvl), "Level 1");
    gtk_combo_box_text_append_text  (GTK_COMBO_BOX_TEXT (my->combo_lvl), "Level 2");
    gtk_combo_box_text_append_text  (GTK_COMBO_BOX_TEXT (my->combo_lvl), "Level 3");
    gtk_combo_box_text_append_text  (GTK_COMBO_BOX_TEXT (my->combo_lvl), "Level 4");
    gtk_combo_box_text_append_text  (GTK_COMBO_BOX_TEXT (my->combo_lvl), "Level 5");
    gtk_combo_box_text_append_text  (GTK_COMBO_BOX_TEXT (my->combo_lvl), "Level 6");
    gtk_combo_box_text_append_text  (GTK_COMBO_BOX_TEXT (my->combo_lvl), "Level 7");
    gtk_combo_box_text_append_text  (GTK_COMBO_BOX_TEXT (my->combo_lvl), "Level 8");
    gtk_combo_box_text_append_text  (GTK_COMBO_BOX_TEXT (my->combo_lvl), "Level 9");
    gtk_combo_box_text_append_text  (GTK_COMBO_BOX_TEXT (my->combo_lvl), "Level 10");
    gtk_combo_box_set_active (GTK_COMBO_BOX(my->combo_lvl), 0);
    gtk_box_pack_start (GTK_BOX (my->hbox_menu),my->combo_lvl, TRUE, TRUE, 0);
    
    button_quit = gtk_button_new_with_label ("Quit");
	gtk_box_pack_start (GTK_BOX (my->hbox_menu),button_quit, TRUE, TRUE, 0);
	g_signal_connect (button_quit, "clicked",G_CALLBACK(on_button_quit_clicked), my);
}
