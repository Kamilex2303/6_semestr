#include "stale.h"
#include "rysunek.h"
#include "shared_memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

int display_at() {
    pogoda *weather = (pogoda *)shared_memory;
    draw_background();
    int i;
    for (i=0;i<weather->last;i++) {
        if (i == my_id) draw_city(weather->location[i][0],weather->location[i][1],1,weather->city[i],weather->deg[i],weather->pressure[i]);
        else draw_city(weather->location[i][0],weather->location[i][1],0,weather->city[i],weather->deg[i],weather->pressure[i]);
    }

    XSetForeground(mydisplay, mygc, black.pixel);
    XFlush(mydisplay);

    unsigned int keycode;
    while (1) {
        XNextEvent(mydisplay, &myevent);
        switch (myevent.type) {
        case Expose:
    draw_background();
    for (i=0;i<weather->last;i++) {
        if (i == my_id) draw_city(weather->location[i][0],weather->location[i][1],1,weather->city[i],weather->deg[i],weather->pressure[i]);
        else draw_city(weather->location[i][0],weather->location[i][1],0,weather->city[i],weather->deg[i],weather->pressure[i]);
    }
    XSetForeground(mydisplay, mygc, black.pixel);
    XFlush(mydisplay);
        break;
        case KeyPress:
            switch(myevent.xkey.keycode) {
                case 25: // W
                    weather->deg[my_id]++;
                break;
                case 26: // E
                    weather->pressure[my_id]++;
                break;
                case 39: // S
                    weather->deg[my_id]--;
                break;
                case 40: // D
                    weather->pressure[my_id]--;
                break;
            }
            semaphore_v(sem_id, weather->current);
        break;
        }
    }
}

void *hmm(void *arg) {
    pogoda *weather = (pogoda *)shared_memory;
    int i;
    semaphore_v(sem_id, weather->last);
    while(1) {
        semaphore_p(sem_id, 0);
        usleep(20000);
        draw_background();
    for (i=0;i<weather->last;i++) {
        if (i == my_id) draw_city(weather->location[i][0],weather->location[i][1],1,weather->city[i],weather->deg[i],weather->pressure[i]);
        else draw_city(weather->location[i][0],weather->location[i][1],0,weather->city[i],weather->deg[i],weather->pressure[i]);
    }
    XSetForeground(mydisplay, mygc, black.pixel);
    XFlush(mydisplay);
    }

}

int main() {
    srand(time(NULL));
    init_cities();
    init_shared_memory();
    init_semaphores();
    create_simple_window();
    create_gc();
    //XSync(mydisplay, 0);
    load_bitmap();
    pthread_t thread;
    pthread_create(&thread, NULL, hmm, NULL);
    display_at();
    //XFlush(mydisplay);
    return 0;
}
