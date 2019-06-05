#include <gtk/gtk.h>
#include "util.h"
#include "area1.h"
#include "mydata.h"
#include "menus.h"


void on_item_about_activate (GtkWidget *widget,gpointer data)
{
  Mydata *my = get_mydata(data);
  char *auteurs[] = {"Axel Dubar axel.dubar@gmail.com",""};
  gtk_show_about_dialog (NULL,"program-name",my->title,"version","2.4","website","http://j.mp/optigra","authors",auteurs,"logo-icon-name","face-laugh",NULL);

}

void on_item_save_activate(GtkWidget *widget, gpointer data){
	Mydata *my = get_mydata(data);
	if(my->game.state==GS_EDIT)
	  gtk_widget_show(my->win_save);	
}

void on_item_load_activate(GtkWidget *widget, gpointer data){
	Mydata *my = get_mydata(data);
	if(my->game.state==GS_EDIT)
	  gtk_widget_show(my->win_load);	
}


void on_item_quit_activate (GtkWidget *widget, gpointer data)
{
  Mydata *my = get_mydata(data);
  printf ("Closing window and app\n");
  gtk_widget_destroy(my->window);
}

void on_item_scale_activate (GtkWidget *widget,gpointer user_data)
{
  Mydata *my = get_mydata(user_data);
  gtk_window_present(GTK_WINDOW (my->win_scale));
  gtk_widget_show(my->frame2);
}

void menus_init(gpointer user_data)
{
  Mydata *my = get_mydata(user_data);
  GtkWidget *item_file, *item_save, *item_help, *sub_file, *item_load, *item_quit, *sub_help, *item_about;
// menu bar
  my->menu_bar = gtk_menu_bar_new ();


  item_file = gtk_menu_item_new_with_label ("File");
  gtk_menu_shell_append (GTK_MENU_SHELL(my->menu_bar), item_file);

  item_help = gtk_menu_item_new_with_label ("Help");
  gtk_menu_shell_append (GTK_MENU_SHELL(my->menu_bar), item_help);

  g_object_set (gtk_settings_get_default(),"gtk-shell-shows-menubar", FALSE, NULL);

//sub file
  sub_file = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM(item_file), sub_file);

  item_load = gtk_menu_item_new_with_label ("Load"); 
  gtk_menu_shell_append (GTK_MENU_SHELL(sub_file), item_load);
  g_signal_connect (item_load, "activate",G_CALLBACK(on_item_load_activate), my);
  
  item_save = gtk_menu_item_new_with_label ("Save"); 
  gtk_menu_shell_append (GTK_MENU_SHELL(sub_file), item_save);
  g_signal_connect (item_save, "activate",G_CALLBACK(on_item_save_activate), my);
  

  item_quit = gtk_menu_item_new_with_label ("Quit");
  gtk_menu_shell_append (GTK_MENU_SHELL(sub_file), item_quit);
  g_signal_connect (item_quit, "activate",G_CALLBACK(on_item_quit_activate), my);

//sub help
  sub_help = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM(item_help), sub_help);

  item_about = gtk_menu_item_new_with_label ("About");
  gtk_menu_shell_append (GTK_MENU_SHELL(sub_help), item_about);
  g_signal_connect (item_about, "activate",G_CALLBACK(on_item_about_activate),my);
}
