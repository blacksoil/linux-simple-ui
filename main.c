#include "minui.h"
#include <linux/input.h>
#include "utils.h"
#include <pthread.h>
#include <sys/reboot.h>

static void* input_thread(void* args) {
    DEBUG_PRINT("input_thread: started");
    int power_key_count = 0;
    for (;;) {
        struct input_event ev;
        char name[80];
        if(!ev_get(&ev, name, 0)) {
            DEBUG_PRINT("input_thread: name=%s type=%d code=%d val=%d\n", name, ev.type, ev.code, ev.value);
            if (ev.code == 116 && ev.value == 1) {
                power_key_count++;
                ALOGD("input_thread: power_key_count=%d\n", power_key_count);
                if (power_key_count == 5) {
                    ALOGD("input_thread: shutting down device!\n");
                    reboot(RB_POWER_OFF);
                }
            }
        }
    }
   return NULL; 
}

static void* graphic_thread(void* args) {
    const int text_len = 3;
    char *texts[] = { "Hello World!", "Welcome to Android!", "FB is cool!", NULL };
    const long sleep_time = 3;
    DEBUG_PRINT("graphic_thread: started");

    int text_idx = 0;
    int y = 20;
    for (;;) {
        text_idx = (text_idx + 1) % text_len;
        gr_color(207, 217, 145, 60);
        gr_fill(0, 0, gr_fb_width(), gr_fb_height()); 
        gr_text(50, y, texts[text_idx]);
        y += 20;
        gr_flip();
        sleep(sleep_time); 
    }
    return NULL;
}

static pthread_t thread1;
static void start_input_thread() {
    DEBUG_PRINT("start_input_thread: start");
    pthread_create(&thread1, NULL, input_thread, NULL);
}


static pthread_t thread2;
static void start_graphic_thread() {
    DEBUG_PRINT("start_graphic_thread: start");
    pthread_create(&thread2, NULL, graphic_thread, NULL);
}

int main(int argc, char** argv) {
    if (!gr_init()) {
        ALOGD("main: gr_init succeeded!\n");
        DEBUG_PRINT("gr_fb_width=%d gr_fb_height=%d\n", gr_fb_width(), gr_fb_height());
        start_graphic_thread();
    } else {
        ALOGE("main: gr_init failed");
    }

    ev_init();
    start_input_thread();
    while(1) {
        sleep(1);
    }
    return 0;
}
