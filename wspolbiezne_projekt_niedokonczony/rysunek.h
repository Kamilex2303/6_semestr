#ifndef RYSUNEK
#define RYSUNEK
#include "stale.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define WIDTH 610
#define HEIGHT 572



Display* mydisplay;
Window mywindow;
XSetWindowAttributes mywindowattributes;
XGCValues mygcvalues;
GC mygc;
Visual* myvisual;
int mydepth;
int myscreen;
Colormap mycolormap;
XColor black, green, dummy;
XEvent myevent;

Pixmap bitmap;
unsigned int bitmap_width, bitmap_height;
int hotspot_x, hotspot_y;

void create_simple_window();
void create_gc();
void load_bitmap();

void draw_background();
void draw_point(int,int,int);
void draw_string(int,int,char *);
void draw_city(int,int,int,char *,int,int);
#endif // RYSUNEK
