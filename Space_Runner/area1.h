#ifndef AREA1_H
#define AREA1_H

#include "curve.h"
#include "mydata.h"

#define ANIM1_TIME 20

void draw_road_wall(cairo_t *cr, Road *road, Mydata *my);
void draw_road_curve(cairo_t *cr, Road *road, Mydata *my);
void set_anim1_mode (Mydata *my, int flag);
gboolean on_timeout1 (gpointer data);

void draw_control_polygons (cairo_t *cr, Road *road);
void area1_init(gpointer user_data);
void draw_control_labels(cairo_t *cr, PangoLayout *layout, Curve_infos *ci);
void draw_bezier_polygons_open (cairo_t *cr, Curve_infos *ci);
void draw_bezier_curves_open (cairo_t *cr, Curve_infos *ci, double theta);
void draw_bezier_curves_prolong (cairo_t *cr, Curve *curve, double theta);
void generate_bezier_path (cairo_t *cr, Control bez_points[4], double theta, int is_first);
void draw_track (cairo_t *cr, Mydata *my);
void sample_bezier_curve (Control bez_points[4], double theta, double sx[], double sy[], int *ind, int ind_max, int is_first);
void set_anim1_mode (Mydata *my, int flag);


#endif /* AREA1_H */
