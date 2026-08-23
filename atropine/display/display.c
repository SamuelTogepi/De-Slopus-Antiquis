#include <display.h>
#include <image.h>
#include <string.h>

display_info_t display_info;

static void display_print_banner_line(void) {
    int count = display_info.width / FONT_WIDTH;
    for (int i = 0; i < count; i++) {
        display_debug_print("=");
    }
    display_debug_print("\n");
}

void display_set_sf(int sf) {
    display_info.sf = sf;
}

uint32_t* display_coords_to_address(int x, int y) {
    if (x < 0 || (uint32_t)x >= display_info.width || y < 0 || (uint32_t)y >= display_info.height) {
        return NULL;
    }
    uint32_t offset = (uint32_t)y * display_info.width + (uint32_t)x;
    return &display_info.framebuffer[offset];
}

void display_write_pixel(int x, int y, uint32_t colour) {
    uint32_t* pixel = display_coords_to_address(x, y);
    if (pixel) {
        *pixel = colour;
    }
}

void display_clear(void) {
    uint32_t total_pixels = display_info.width * display_info.height;
    for (uint32_t i = 0; i < total_pixels; i++) {
        display_info.framebuffer[i] = display_info.background_colour;
    }
}

void display_invert(void) {
    display_info.background_colour = ~display_info.background_colour;
    display_info.foreground_colour = ~display_info.foreground_colour;
    
    uint32_t total_pixels = display_info.width * display_info.height;
    for (uint32_t i = 0; i < total_pixels; i++) {
        display_info.framebuffer[i] = ~display_info.framebuffer[i];
    }
}

void display_debug_print_char(char c) {
    display_set_sf(2);
    display_write_debug_char(c);
}

void display_debug_print(char* str) {
    display_set_sf(2);
    display_write_debug(str);
}

void display_logo(void) {
    int x = (display_info.width / 2) - (image_width / 2);
    int y = (display_info.height / 2) - (image_height / 2);
    draw_image((uint32_t*)image, x, y, image_width, image_height);
}

void display_progress_bar(int progress) {
    int start_x = 50;
    int start_y = (2 * display_info.height) / 3;
    int end_x = display_info.width - 50;
    int end_y = start_y + 25;

    /* Clamp progress value between 0 and 100 */
    if (progress < 0) progress = 0;
    if (progress > 100) progress = 100;

    /* Clear and stroke the outline */
    display_fill_rect(start_x, start_y, end_x, end_y, display_info.background_colour);
    display_stroke_rect(start_x, start_y, end_x, end_y, display_info.foreground_colour);

    /* Fill the progress portion */
    int progress_x = start_x + (int)(((double)(end_x - start_x)) * ((double)progress / 100.0));
    display_fill_rect(start_x, start_y, progress_x, end_y, display_info.foreground_colour);
}

void display_progress_print(char* str) {
    size_t len = strlen(str);
    if (len > 20) {
        return;
    }

    display_set_sf(4);

    int x = (display_info.width / 2) - ((FONT_WIDTH * (int)len) / 2);
    int y = (3 * display_info.height) / 4;

    display_fill_rect(0, y, display_info.width, y + FONT_HEIGHT, display_info.background_colour);
    display_write(str, x, y);
}

void display_init(uint32_t* framebuffer, uint32_t width, uint32_t height) {
    display_info.framebuffer = framebuffer;
    display_info.width = width;
    display_info.height = height;
    display_info.background_colour = display_info.framebuffer[0];
    display_info.foreground_colour = ~display_info.background_colour;

    display_set_sf(2);
    display_clear();
    display_logo();

    set_putchar(&display_debug_print_char);

    display_progress_bar(0);

    display_print_banner_line();
    display_debug_print("atropine by synackuk\n");
    display_debug_print("Part of the n1ghtshade jailbreak\n");
    display_debug_print("With thanks to:\n");
    display_debug_print("axi0mx, nyansatan, iH8sn0w,\n");
    display_debug_print("linus henze, xerub and tihmstar\n\n");
    display_print_banner_line();
}
