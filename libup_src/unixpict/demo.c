
#include <stdlib.h>
#include <stdio.h>
#include "unixpict.h"

void	main(void);
void	set_map_colors(RGBColor *);

#define N_MAP_COLORS	9
#define WHITE		0
#define YELLOW		1
#define ORANGE		2
#define RED		3
#define GREEN		4
#define BLUE		5
#define PURPLE		6
#define GRAY		7
#define BLACK		8

void	main(void)
{
	int		i, j, rand_col;
	FILE		*fp;
	Rect		rect;
	RGBColor	colors[N_MAP_COLORS];
	Pattern		pat;

	set_map_colors(colors);

	fp = OpenCPicture("demo_pict", 300.0, 300.0, 1800, 1800);

	SetRect(&rect, 100, 100, 120, 120);
	OpenGroup(fp);
	for (i = 0; i < 64; ++i) {
		for (j = 0; j < 64; ++j) {
			rand_col = rand() % N_MAP_COLORS;
			RGBForeColor(&colors[rand_col], fp);
			PaintRect(&rect, fp);
			OffsetRect(&rect, 25, 0);
		}
		OffsetRect(&rect, 0, 25);
		rect.left = 100;
		rect.right = 120;
	}
	CloseGroup(fp);

	ClosePicture(fp);

	return;
}


void set_map_colors(RGBColor *colors)
{
	colors[WHITE].red = 0xFFFF; colors[WHITE].grn = 0xFFFF; colors[WHITE].blu = 0xFFFF;
	colors[YELLOW].red = 0xFFFF; colors[YELLOW].grn = 0xFFFF; colors[YELLOW].blu = 0x0000;
	colors[ORANGE].red = 0xFFFF; colors[ORANGE].grn = 0x7FFF; colors[ORANGE].blu = 0x0000;
	colors[RED].red = 0xFFFF; colors[RED].grn = 0x0000; colors[RED].blu = 0x0000;
	colors[GREEN].red = 0x0000; colors[GREEN].grn = 0xBFFF; colors[GREEN].blu = 0x0000;
	colors[BLUE].red = 0x0000; colors[BLUE].grn = 0x0000; colors[BLUE].blu = 0xFFFF;
	colors[PURPLE].red = 0x7FFF; colors[PURPLE].grn = 0x0000; colors[PURPLE].blu = 0x7FFF;
	colors[GRAY].red = 0x7FFF; colors[GRAY].grn = 0x7FFF; colors[GRAY].blu = 0x7FFF;
	colors[BLACK].red = 0x0000; colors[BLACK].grn = 0x0000; colors[BLACK].blu = 0x0000;

	return;
}
