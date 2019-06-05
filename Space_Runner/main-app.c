#include <gtk/gtk.h>
#include "mydata.h"
#include "util.h"
#include "curve.h"
#include "menus.h"
#include "area1.h"
#include "gui.h"

void on_app_activate (GtkApplication* app, gpointer user_data)
{
  Mydata *my = get_mydata(user_data);
  

  window_init(app,my);
  
  menus_init(my);

  area1_init(my);

  status_init(my);
  
  main_menu_init(my);
  
  win_load_init(my);
  
  win_save_init(my);
  
  editing_init(my);

  
  layout_init(my); 

  gtk_widget_show_all (my->window);

  gtk_widget_hide (my->frame1);
  
  gtk_widget_hide (my->hbox_menu);
  
}

int main (int argc, char *argv[])
{
  Mydata my;
  init_mydata (&my);

  GtkApplication *app;
  int status;
  app = gtk_application_new (NULL, G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate",G_CALLBACK(on_app_activate), &my);
  status = g_application_run (G_APPLICATION(app), argc, argv);
  g_object_unref (app);

  return status;
}
