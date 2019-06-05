#ifndef GAME_H
#define GAME_H

#include "curve.h"
#define CAR_MAX 4
#define SAMPLE_MAX 10000

#define ROAD_LARGE 30
#define BOOST_MAX 10

typedef enum{ GS_HELLO, GS_EDIT, GS_PLAYING, GS_PAUSE, GS_WON} Game_state;

typedef enum { GM_NONE, GM_SINGLE, GM_DUO, GM_TRIO, GM_QUATOR} Game_mode;

typedef struct {
	int    sample_count;
	double sample_x[SAMPLE_MAX], sample_y[SAMPLE_MAX];
} Track;

typedef struct {
	int    boost_count;
	double boost_x[BOOST_MAX], boost_y[BOOST_MAX];
} Boost;

typedef struct {
	Curve  curve_right, curve_central, curve_left;
	Track  track_right, track_central, track_left;
	Boost boost;
	double temps;
} Road;

typedef struct {
	double x, y, radius;
	double angle, speed;
	int direction;
	int accelerator;
	int temps_boost;
	int is_winner;
} Car;

typedef struct {
	Game_state state;
	Game_mode  mode;
	int score;
	Car cars[CAR_MAX];
	int car_count;
	Road road;     // par la suite un tableau par niveau
} Game;

void init_game(Game *game);
void init_road(Road *road);
void draw_road_wall_right(cairo_t *cr, Control P0,Control P1,double xn,double yn,Road *road);
void draw_road_wall_left(cairo_t *cr, Control P0,Control P1,double xn,double yn,Road *road);
int remove_control_from_curve(Curve *curve,int n);
int compute_intersection(double xs0,double ys0,double xs1,double ys1,double xt0,double yt0,double xt1,double yt1,double *xp,double *yp);
void do_vector_product(double xa,double ya,double za,double xb,double yb,double zb,double *xn,double *yn,double *zn);
void compute_bezier_track (Curve *curve, Track *tra, double theta);
void compute_road_tracks (Road *road, double theta);
void place_cars_on_start(Game *game);
void progress_game_next_step (Game *game);
void save_road(Road *road, int lvl);
void load_road(Road *road, int lvl);
void set_GS_mode(Game *game,Game_state state);
void add_boost(Road *road,double x,double y);
void remove_boost(Road *road,int indice);
void move_boost(Road *road,int nb_boost, double dx, double dy);
int find_boost(Boost *boost,double x, double y);
void save_time(Road *road,int lvl,double time);

void compute_normale_right(double xa, double xb, double ya, double yb, double *xn, double *yn);
void add_control_from_road (Road *road, double x, double y);
void move_control_from_road(Road *road,int nb_control, double dx, double dy);
int move_curve_from_road(Road *road, double dx, double dy);
int remove_control_from_road(Road *road,int n);
int remove_curve_from_road(Road *road);

#endif /* GAME_H */
