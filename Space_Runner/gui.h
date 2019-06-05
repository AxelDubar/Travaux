#ifndef GUI_H
#define GUI_H

void editing_init (Mydata *my);
void layout_init(gpointer user_data);
void window_init(GtkApplication *app,gpointer user_data);
void status_init(gpointer user_data);
void win_gs_init(Mydata *my);
void main_menu_init(Mydata *my);
void win_save_init(Mydata *my);
void win_load_init(Mydata *my);

#endif /* GUI_H */

