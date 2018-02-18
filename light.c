/*
 ============================================================================
 Name        : light.c
 Author      : Malcolm Herring
 Version     : 0.1
 Description : RaspberryPi TFT backlight timeout
 Copyright   : © 2016 Malcolm Herring.

 light is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, version 3 of the License, or
 any later version.

 light is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 A copy of the GNU General Public License can be found here:
 <http://www.gnu.org/licenses/>.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/ioctl.h>

int main(int argc, const char* argv[]) {

    int dev = 0;
    int ontime = 60;
    struct input_event ev;

    if (argc > 1) {

        if ((dev = open(argv[1], O_RDONLY)) < 0) {
            perror("Device open");
            return 1;
        }
        if (argc > 2) {
            if ((ontime = atoi(argv[2])) < 1) {
                fprintf(stderr, "Invalid timeout\n");
                return 1;
            }
        }
        while (true) {
            sleep(ontime);
            system("echo 0 > /sys/class/backlight/soc:backlight/brightness");
            do {
                read(dev, &ev, sizeof(struct input_event));
            } while ((ev.type != EV_KEY) || (ev.code != BTN_TOUCH) || (ev.value != 0));
            system("echo 1 > /sys/class/backlight/soc:backlight/brightness");
        }
    } else {
        fprintf(stderr, "No device specified\n");
        return 1;
    }
    close(dev);
    return 0;
}
