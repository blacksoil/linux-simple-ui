#include "minui.h"
#include <linux/input.h>
#include "utils.h"
#include <pthread.h>

static void* input_thread(void* args) {
    DEBUG_PRINT("input_thread: started");
    for (;;) {
        struct input_event ev;
        char name[80];
        if(!ev_get(&ev, name, 0)) {
            DEBUG_PRINT("input_thread: name=%s type=%d code=%d val=%d\n", name, ev.type, ev.code, ev.value);
        }
    }
   return NULL; 
}

static pthread_t thread1;
static void start_updater_thread() {
    DEBUG_PRINT("start_updater_thread: start");
    pthread_create(&thread1, NULL, input_thread, NULL);
}

int main(int argc, char** argv) {
    ev_init();
    start_updater_thread();

    while(1) {
        sleep(1);
    }
    return 0;
}



