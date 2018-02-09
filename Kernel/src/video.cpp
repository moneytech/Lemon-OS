#include <video.h>
#include <vga.h>
#include <graphicsresources.h>

video_mode_t video_mode;

uint8_t* video_memory;

void video_initialize(video_mode_t v) {
	video_mode = v;
	video_memory = (uint8_t*)v.address;
	screen_clear();
	
}

void screen_clear() {
	if (video_mode.type == Text) {
		VGA::clearscreen();
	}
	else if (video_mode.type == Graphical) {
		for (int w = 0; w < video_mode.height * video_mode.pitch; w++) {
			video_memory[w] = 0;
		}

	}
}

void screen_clear(uint8_t r, uint8_t g, uint8_t b) {
	if (video_mode.type != Graphical) return; // Text Mode is only 16-colors
	screen_fillrect(0, 0, video_mode.width, video_mode.height, r, g, b);
}

void screen_clear(uint8_t colour) {
	if (video_mode.type != Text) return; // Text Mode is the only 16-color Mode
	VGA::clearscreen(colour);
}

void screen_putpixel(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
	if (video_mode.type != Graphical) return; // Text Mode uses characters not pixels
	unsigned pos = y * video_mode.pitch + (x * (video_mode.bpp / 8));
	video_memory[pos + 2] = r;
	video_memory[pos + 1] = g;
	video_memory[pos + 0] = b;
}

void screen_fillrect(int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b) {
	uint32_t where = 0;
	for (int i = 0; i<h; i++) {
		for (int j = 0; j<w; j++) {
			where = (y + i) * video_mode.pitch + ((x + j) * (video_mode.bpp / 8));
			video_memory[where] = b & 255; // BLUE
			video_memory[where + 1] = g;  // GREEN
			video_memory[where + 2] = r; // RED
		}
	}
}

void drawchar(char c, int x, int y, uint8_t r, uint8_t g, uint8_t b, int scale) {
	for (int i = 0; i < 8; i++)
	{
		int row = font_default[c][i];
		for (int j = 0; j < 8; j++)
		{
			if ((row & (1 << j)) >> j)
				screen_fillrect(x + j * scale, y + i * scale, scale, scale, r, g, b);
		}
	}
}

void drawstring(char* str, int x, int y, uint8_t r, uint8_t g, uint8_t b, int scale) {
	int xOffset = 0;
	while (*str != 0) {
		drawchar(*str, x + xOffset, y, r, g, b, scale);
		xOffset += scale * 8;
		str++;
	}
}

void drawstring(char** str, int x, int y, uint8_t r, uint8_t g, uint8_t b, int scale) {
	for (int i = 0; str[i] != '\0'; i++) {
		drawstring(str, x, y, r, g, b, scale);
	}
}

/*void drawbitmap(bitmap_t *bmp, int x, int y, int w, int h)
{
	for (int j = 0; j < h && j < bmp->height; j++) {
		for (int i = 0; i < w*video_mode.bpp && i < bmp->width*(video_mode.bpp/8); i++)
		{
			int video_memory_pos = j*video_mode.pitch + i + x*(video_mode.bpp/8);
			int bmp_pos = j*(video_mode.width*video_mode.bpp/8) + i;
			video_memory[video_memory_pos] = bmp->data[bmp_pos];
		}
	}
}*/

/*void screen_update() {
	int bpp = video_mode.bpp;
	int pitch = video_mode.pitch;
	uint32_t pos = 0;
	for (int y = 0; y < video_mode.height*bpp/8; y++) {
		pos = pitch * y;
		for (int x = 0; x < video_mode.width; x++) {
			video_memory[pos] = video_memory_buffer[pos];
			pos++;
		}
	}
}*/