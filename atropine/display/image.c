#include <display.h>

void draw_image(const uint32_t* img, int x, int y, int width, int height) {
    if (!img || width <= 0 || height <= 0) {
        return;
    }

    /* Direct pointer indexing across image dimensions */
    for (int img_y = 0; img_y < height; img_y++) {
        int screen_y = y + img_y;

        /* Skip out-of-bounds rows early if display height is known */
        if (screen_y < 0 || (uint32_t)screen_y >= display_info.height) {
            continue;
        }

        const uint32_t* row = &img[img_y * width];

        for (int img_x = 0; img_x < width; img_x++) {
            int screen_x = x + img_x;

            if (screen_x < 0 || (uint32_t)screen_x >= display_info.width) {
                continue;
            }

            display_write_pixel(screen_x, screen_y, row[img_x]);
        }
    }
}
