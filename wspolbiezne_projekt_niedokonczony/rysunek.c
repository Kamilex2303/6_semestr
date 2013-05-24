 #include "rysunek.h"

/* inicjalizacja okna */
void create_simple_window() {
  mydisplay = XOpenDisplay("");
  int screen_num = DefaultScreen(mydisplay);
  int win_border_width = 2;
  mywindow = XCreateSimpleWindow(mydisplay, RootWindow(mydisplay, screen_num),
                            0, 0, WIDTH, HEIGHT, win_border_width,
                            BlackPixel(mydisplay, screen_num),
                            WhitePixel(mydisplay, screen_num));
  XMapWindow(mydisplay, mywindow);
  XFlush(mydisplay);

}

void create_gc() {
  unsigned long valuemask = 0;
  XGCValues values;
  unsigned int line_width = 2;
  int line_style = LineSolid;
  int cap_style = CapButt;
  int join_style = JoinBevel;
  int screen_num = DefaultScreen(mydisplay);

  mygc = XCreateGC(mydisplay, mywindow, valuemask, &values);
  if (mygc < 0) {
	fprintf(stderr, "XCreateGC: \n");
  }

  // czcionka; do napisow
  XFontStruct* font_info;
  char* font_name = "*-arial-bold*";
  font_info = XLoadQueryFont(mydisplay, "fixed");
  if (!font_info) {
      fprintf(stderr, "XLoadQueryFont: failed loading font %s\n", font_name);
      exit(0);
  }
  XSetFont(mydisplay, mygc, font_info->fid);

  XSetForeground(mydisplay, mygc, BlackPixel(mydisplay, screen_num));
  XSetBackground(mydisplay, mygc, WhitePixel(mydisplay, screen_num));
  XSetLineAttributes(mydisplay, mygc, line_width, line_style, cap_style, join_style);
  XSetFillStyle(mydisplay, mygc, FillSolid);

  myvisual = DefaultVisual(mydisplay, myscreen);
  mydepth = DefaultDepth(mydisplay, myscreen);
  mywindowattributes.background_pixel = XWhitePixel(mydisplay, myscreen);
  mywindowattributes.override_redirect = False;

  XSelectInput(mydisplay, mywindow, ExposureMask|KeyPressMask);
  mycolormap = DefaultColormap(mydisplay, myscreen);
  XAllocNamedColor(mydisplay, mycolormap,"black", &black, &dummy);
  XAllocNamedColor(mydisplay, mycolormap,"green", &green, &dummy);
  XMapWindow(mydisplay, mywindow);
}

/* wczytywanie rysunku */
void load_bitmap() {
    int rc = XReadBitmapFile(mydisplay, mywindow,
                             "polska.bmp",
                             &bitmap_width, &bitmap_height,
                             &bitmap,
                             &hotspot_x, &hotspot_y);

    switch (rc) {
        case BitmapOpenFailed:
            fprintf(stderr, "XReadBitmapFile - nie udalo sie wczytac 'polska.bmp'.\n");
            exit(1);
        break;
        case BitmapFileInvalid:
            fprintf(stderr,"XReadBitmapFile - plik nieprawidlowy.\n");
            exit(1);
        break;
        case BitmapNoMemory:
            fprintf(stderr, "XReadBitmapFile - za malo pamieci.\n");
            exit(1);
        break;
    }
}

/* skladowe rysunku */
// kontury Polski
void draw_background() {
    XCopyPlane(mydisplay, bitmap, mywindow, mygc, 0, 0,bitmap_width, bitmap_height,0, 0,1);
}

// tworzenie napisu
void draw_string(int x,int y,char *text) {
    XDrawString(mydisplay, mywindow, mygc, x, y, text, strlen(text));
}

// punkt na mapie
void draw_point(int x,int y,int mine) {
    if (mine) XSetForeground(mydisplay, mygc, green.pixel);
    else XSetForeground(mydisplay, mygc, black.pixel);
    XFillRectangle(mydisplay, mywindow, mygc, x, y, 8, 8);
}

// miasto z pogoda
void draw_city(int x,int y, int mine, char *city, int deg, int pressure) {
    // 0 - miasto, 1 - stopnie, 2 - cisnienie
    draw_point(x,y,mine);
    draw_string(x+10,y,city);
    char buffer[10] = {};
    snprintf(buffer, 10,"%i C",deg);
    draw_string(x+10,y+10,buffer);
    snprintf(buffer, 10,"%i hPa",pressure);
    draw_string(x+10,y+20,buffer);
}
