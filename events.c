/*

 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/poll.h>
#include <errno.h>

#include <linux/input.h>

#include "minui.h"
#include "utils.h"
#include <linux/input.h>

#define MAX_DEVICES 16
#define DEVICE_NAME_LENGTH 80

static struct pollfd ev_fds[MAX_DEVICES];
static char poll_device_name[MAX_DEVICES][DEVICE_NAME_LENGTH];
static unsigned ev_count = 0;

static char *input_device_whitelist[] = {"mahimahi-nav", "mahimahi-keypad"};
int is_device_whitelisted(char input_device[]) {
    int i;
    for (i=0; i < sizeof(input_device_whitelist) / sizeof(*input_device_whitelist) ; i++) {
        if (!strcmp(input_device_whitelist[i], input_device)) {
            return 0;
        }
    }    
    return -EINVAL;
}

int ev_init(void)
{
    DIR *dir;
    struct dirent *de;
    int fd;
    int ret;
    char name[80];

    memset(poll_device_name, 0, sizeof(MAX_DEVICES*DEVICE_NAME_LENGTH));

    dir = opendir("/dev/input");
    if(dir != 0) {
        while((de = readdir(dir))) {
//            fprintf(stderr,"/dev/input/%s\n", de->d_name);
            if(strncmp(de->d_name,"event",5)) continue;
            fd = openat(dirfd(dir), de->d_name, O_RDONLY);

            memset(name, 0, sizeof(name));
            if ((ret = ioctl(fd, EVIOCGNAME(sizeof(name) - 1), name)) == -1) {
                ret = errno;
                ALOGE("ev_init: ioctl error for %s=%d", de->d_name, errno);
                continue;
            } else {
                ALOGD("ev_init: input found=%s\n", name);
                memcpy(poll_device_name[ev_count], name, strlen(name));
                if (ENABLE_INPUT_WHITELISTING && is_device_whitelisted(name)) {
                    continue;
                }
            }
            if(fd < 0) continue;
            ev_fds[ev_count].fd = fd;
            ev_fds[ev_count].events = POLLIN;
            ev_count++;
            if(ev_count == MAX_DEVICES) break;
        }
    }

    return 0;
}

void ev_exit(void)
{
    while (ev_count > 0) {
        close(ev_fds[--ev_count].fd);
    }
}

int ev_get(struct input_event *ev, char *device_name, unsigned dont_wait)
{
    int r;
    unsigned n;

    do {
        r = poll(ev_fds, ev_count, dont_wait ? 0 : -1);

        if(r > 0) {
            for(n = 0; n < ev_count; n++) {
                if(ev_fds[n].revents & POLLIN) {
                    r = read(ev_fds[n].fd, ev, sizeof(*ev));
                    strcpy(device_name, poll_device_name[n]);
                    if(r == sizeof(*ev)) return 0;
                }
            }
        }
    } while(dont_wait == 0);

    return -1;
}
