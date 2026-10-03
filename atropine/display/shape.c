#include <display.h>

void display_stroke_rect(int start_x, int start_y, int end_x, int end_y, uint32_t colour) {
    /* Normalize coordinates if passed in reverse order */
    if (start_x > end_x) { int temp = start_x; start_x = end_x; end_x = temp; }
    if (start_y > end_y) { int temp = start_y; start_y = end_y; end_y = temp; }

    /* Draw horizontal top and bottom edges */
    for (int x = start_x; x <= end_x; x++) {
        display_write_pixel(x, start_y, colour);
        display_write_pixel(x, end_y, colour);
    }

    /* Draw vertical left and right edges */
    for (int y = start_y; y <= end_y; y++) {
        display_write_pixel(start_x, y, colour);
        display_write_pixel(end_x, y, colour);
    }
}

void display_fill_rect(int start_x, int start_y, int end_x, int end_y, uint32_t colour) {
    /* Normalize coordinates */
    if (start_x > end_x) { int temp = start_x; start_x = end_x; end_x = temp; }
    if (start_y > end_y) { int temp = start_y; start_y = end_y; end_y = temp; }

    /* Clip to physical framebuffer dimensions */
    if (start_x < 0) start_x = 0;
    if (start_y < 0) start_y = 0;
    if ((uint32_t)end_x >= display_info.width)  end_x = (int)display_info.width - 1;
    if ((uint32_t)end_y >= display_info.height) end_y = (int)display_info.height - 1;

    /* Optimized row-based bulk memory filling */
    for (int y = start_y; y <= end_y; y++) {
        uint32_t* row_start = display_coords_to_address(start_x, y);
        if (row_start) {
            int line_width = end_x - start_x + 1;
            for (int i = 0; i < line_width; i++) {
                row_start[i] = colour;
            }
        }
    }
}
