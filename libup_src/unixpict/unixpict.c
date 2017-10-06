
#include "unixpict.h"


/* QUICKDRAW GLOBALS */

Point	curPos;


/* QUICKDRAW ROUTINES */

void	BackColor(long color, FILE *fp)
{
	short	temp;
	
	/* BkPat opcode */
	temp = 0x000F;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* old background color */
	(void) fwrite((void *) &color, sizeof(long), (size_t) 1, fp);
	
	return;
}


void	BackPat(Pattern pattern, FILE *fp)
{
	short	temp;
	
	/* BkPat opcode */
	temp = 0x0002;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* Pattern */
	WritePat(pattern, fp);
	
	return;
}


void	DrawChar(char c, FILE *fp)
{	
	char	s[2];
	
	s[0] = c;
	s[1] = '\0';
	DrawString(s, fp);
	
	return;
}	


void	DrawString(char *s, FILE *fp)
{
	extern Point	curPos;
	short		len, temp;
	char		*c;
	unsigned char	l;
	
	c = s - 1;
	while (*++c);
	
	if ((len = c - s) > 255) {
		(void) fprintf(stderr, "%cDrawString: string too long... truncating it\n", 7);
		len = 255;
	}
	
	/* LongText opcode */
	temp = 0x0028;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* current pen location */
	WritePoint(&curPos, fp);
	
	/* string length */
	l = (unsigned char) len;
	(void) fwrite((void *) &l, sizeof(unsigned char), (size_t) 1, fp);
	
	/* maintain 2 byte alignment */
	if (!(len % 2))
		++len;
	
	/* string */
	(void) fwrite((void *) s, sizeof(char), (size_t) len, fp);

	return;
}	


void	CloseGroup(FILE *fp)
{
	short	temp;
	
	/* ShortComment opcode */
	temp = 0x00A0;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* open group comment */
	temp = 0x008D;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	return;
}


void	ClosePicture(FILE *fp)
{
	short	temp;
	long	l;

	/* OpEndPic opcode -> end of picture */
	temp = 0x00FF;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* get picture size */
	l = ftell(fp) - 512L;
	
	if (l > 32767)
		l = 32767;

	/* fill in picture size */
	(void) fseek(fp, 512L, SEEK_SET);
	temp = (short) l;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	(void) fclose(fp);
	
	return;
}


void	EraseArc(Rect *rect, short startAngle, short arcAngle, FILE *fp)
{
	short	temp;
	
	/* eraseArc opcode */
	temp = 0x0062;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* write the Rect defining the arc */
	WriteRect(rect, fp);
	
	/* start angle -> 0 to 359, 0 is striaght up, angles increase clockwise */
	(void) fwrite((void *) &startAngle, sizeof(short), (size_t) 1, fp);
	
	/* arc angle -> 0 to 360 from start angle, positive is clockwise */
	(void) fwrite((void *) &arcAngle, sizeof(short), (size_t) 1, fp);
	
	return;
}


void	EraseOval(Rect *rect, FILE *fp)
{
	short	temp;
	
	/* eraseOval opcode */
	temp = 0x0052;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* write the Rect defining the oval */
	WriteRect(rect, fp);
	
	return;
}


void	ErasePoly(Polygon *polygon, FILE *fp)
{	
	short	temp;
	
	/* erasePoly opcode */
	temp = 0x0072;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* polygon data */
	WritePoly(polygon, fp);
	
	return;
}


void	EraseRect(Rect *rect, FILE *fp)
{
	short	temp;
	
	/* eraseRect opcode */
	temp = 0x0032;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* write the Rect */
	WriteRect(rect, fp);
	
	return;
}


void	EraseRoundRect(Rect *rect, short width, short height, FILE *fp)
{
	short	temp;
	Point	point;
	
	/* OvSize opcode */
	temp = 0x000B;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* Oval width and height */
	SetPoint(&point, width, height);
	WritePoint(&point, fp);
	
	/* eraseRRect opcode */
	temp = 0x0042;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* write the Rect */
	WriteRect(rect, fp);
	
	return;
}


void	FillArc(Rect *rect, short startAngle, short arcAngle, Pattern pattern, FILE *fp)
{
	short	temp;
	
	/* set pattern if one is specified, otherwise current fill pattern is used */
	if (pattern != NULL)
		PenPat(pattern, fp);
	
	/* fillArc opcode */
	temp = 0x0064;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* write the Rect defining the arc */
	WriteRect(rect, fp);
	
	/* start angle -> 0 to 359, 0 is striaght up, angles increase clockwise */
	(void) fwrite((void *) &startAngle, sizeof(short), (size_t) 1, fp);
	
	/* arc angle -> 0 to 360 from start angle, positive is clockwise */
	(void) fwrite((void *) &arcAngle, sizeof(short), (size_t) 1, fp);
	
	return;
}


void	FillOval(Rect *rect, Pattern pattern, FILE *fp)
{
	short	temp;
	
	/* set pattern if one is specified, otherwise current fill pattern is used */
	if (pattern != NULL)
		PenPat(pattern, fp);
	
	/* fillOval opcode */
	temp = 0x0054;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* write the Rect defining the oval */
	WriteRect(rect, fp);
	
	return;
}


void	FillPoly(Polygon *polygon, Pattern pattern, FILE *fp)
{	
	short	temp;
	
	/* set pattern if one is specified, otherwise current fill pattern is used */
	if (pattern != NULL)
		PenPat(pattern, fp);
	
	/* fillPoly opcode */
	temp = 0x0074;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);

	/* polygon data */
	WritePoly(polygon, fp);
	
	return;
}

	
void	FillRect(Rect *rect, Pattern pattern, FILE *fp)
{
	short	temp;
	
	/* set pattern if one is specified, otherwise current fill pattern is used */
	if (pattern != NULL)
		PenPat(pattern, fp);

	/* fillRect opcode */
	temp = 0x0034;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* write the Rect */
	WriteRect(rect, fp);
	
	return;
}


void	FillRoundRect(Rect *rect, short width, short height, Pattern pattern, FILE *fp)
{
	short	temp;
	Point	point;
	
	/* set pattern if one is specified, otherwise current fill pattern is used */
	if (pattern != NULL)
		PenPat(pattern, fp);
		
	/* OvSize opcode */
	temp = 0x000B;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* Oval width and height */
	SetPoint(&point, width, height);
	WritePoint(&point, fp);
	
	/* fillRRect opcode */
	temp = 0x0044;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* write the Rect */
	WriteRect(rect, fp);
	
	return;
}


void	ForeColor(long color, FILE *fp)
{
	short	temp;
	
	/* BkPat opcode */
	temp = 0x000E;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* old foreground color */
	(void) fwrite((void *) &color, sizeof(long), (size_t) 1, fp);
	
	return;
}


void	FrameArc(Rect *rect, short startAngle, short arcAngle, FILE *fp)
{
	short	temp;
	
	/* frameArc opcode */
	temp = 0x0060;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* write the Rect defining the arc */
	WriteRect(rect, fp);
	
	/* start angle -> 0 to 359, 0 is striaght up, angles increase clockwise */
	(void) fwrite((void *) &startAngle, sizeof(short), (size_t) 1, fp);
	
	/* arc angle -> 0 to 360 from start angle, positive is clockwise */
	(void) fwrite((void *) &arcAngle, sizeof(short), (size_t) 1, fp);
	
	return;
}


void	FrameOval(Rect *rect, FILE *fp)
{
	short	temp;
	
	/* frameOval opcode */
	temp = 0x0050;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* write the Rect defining the oval */
	WriteRect(rect, fp);
	
	return;
}


void	FramePoly(Polygon *polygon, FILE *fp)
{	
	short	temp;
	
	/* framePoly opcode */
	temp = 0x0070;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* polygon data */
	WritePoly(polygon, fp);
	
	return;
}


void	FrameRect(Rect *rect, FILE *fp)
{
	short	temp;
	
	/* frameRect opcode */
	temp = 0x0030;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* write the Rect */
	WriteRect(rect, fp);
	
	return;
}


void	FrameRoundRect(Rect *rect, short width, short height, FILE *fp)
{
	short	temp;
	Point	point;
	
	/* OvSize opcode */
	temp = 0x000B;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* Oval width and height */
	SetPoint(&point, width, height);
	WritePoint(&point, fp);
	
	/* frameRRect opcode */
	temp = 0x0040;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* write the Rect */
	WriteRect(rect, fp);
	
	return;
}


void	InvertArc(Rect *rect, short startAngle, short arcAngle, FILE *fp)
{
	short	temp;
	
	/* invertArc opcode */
	temp = 0x0063;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* write the Rect defining the arc */
	WriteRect(rect, fp);
	
	/* start angle -> 0 to 359, 0 is striaght up, angles increase clockwise */
	(void) fwrite((void *) &startAngle, sizeof(short), (size_t) 1, fp);
	
	/* arc angle -> 0 to 360 from start angle, positive is clockwise */
	(void) fwrite((void *) &arcAngle, sizeof(short), (size_t) 1, fp);
	
	return;
}


void	InvertOval(Rect *rect, FILE *fp)
{
	short	temp;
	
	/* invertOval opcode */
	temp = 0x0053;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* write the Rect defining the oval */
	WriteRect(rect, fp);
	
	return;
}


void	InvertPoly(Polygon *polygon, FILE *fp)
{	
	short	temp;
	
	/* invertPoly opcode */
	temp = 0x0073;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* polygon data */
	WritePoly(polygon, fp);
	
	return;
}


void	InvertRect(Rect *rect, FILE *fp)
{
	short	temp;
	
	/* invertRect opcode */
	temp = 0x0033;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* write the Rect */
	WriteRect(rect, fp);
	
	return;
}


void	InvertRoundRect(Rect *rect, short width, short height, FILE *fp)
{
	short	temp;
	Point	point;
	
	/* OvSize opcode */
	temp = 0x000B;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* Oval width and height */
	SetPoint(&point, width, height);
	WritePoint(&point, fp);
	
	/* invertRRect opcode */
	temp = 0x0043;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* write the Rect */
	WriteRect(rect, fp);
	
	return;
}


void	Line(short dh, short dv, FILE *fp)
{
	extern Point	curPos;
	short		temp;
	
	/* Line opcode */
	temp = 0x0020;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* current pen location */
	WritePoint(&curPos, fp);

	/* new pen location */
	curPos.h += dh;
	curPos.v += dv;
	WritePoint(&curPos, fp);

	return;
}


void	LineTo(short h, short v, FILE *fp)
{
	extern Point	curPos;
	short		temp;
	
	/* Line opcode */
	temp = 0x0020;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* current pen location */
	WritePoint(&curPos, fp);

	/* new pen location */
	curPos.h = h;
	curPos.v = v;
	WritePoint(&curPos, fp);

	return;
}


void	GetIndPattern(short index, Pattern pattern)
{
	/* the 38 standard Macintosh Patterns */
	switch(index) {
		case 1:
			SetPattern(pattern, "FFFFFFFFFFFFFFFF");
			break;
		case 2:
			SetPattern(pattern, "FFEEFFBBFFEEFFBB");
			break;
		case 3:
			SetPattern(pattern, "BBEEBBEEBBEEBBEE");
			break;
		case 4:
			SetPattern(pattern, "AA55AA55AA55AA55");
			break;
		case 5:
			SetPattern(pattern, "FFAAFFAAFFAAFFAA");
			break;
		case 6:
			SetPattern(pattern, "5555555555555555");;
			break;
		case 7:
			SetPattern(pattern, "BB77EEDDBB77EEDD");
			break;
		case 8:
			SetPattern(pattern, "4444444444444444");
			break;
		case 9:
			SetPattern(pattern, "D8C606606C8D8118");
			break;
		case 10:
			SetPattern(pattern, "1080042002400801");
			break;
		case 11:
			SetPattern(pattern, "44FF444444FF4444");
			break;
		case 12:
			SetPattern(pattern, "40FF040404FF4040");
			break;
		case 13:
			SetPattern(pattern, "0000000000040000");
			break;
		case 14:
			SetPattern(pattern, "0001020400402010");
			break;
		case 15:
			SetPattern(pattern, "22140808081422C9");
			break;
		case 16:
			SetPattern(pattern, "A3C78B11B87C3A11");
			break;
		case 17:
			SetPattern(pattern, "02AA502020AA0502");
			break;
		case 18:
			SetPattern(pattern, "4444448201102844");
			break;
		case 19:
			SetPattern(pattern, "FD85858585FD00FD");
			break;
		case 20:
			SetPattern(pattern, "0000000000000000");
			break;
		case 21:
			SetPattern(pattern, "0040000400400004");
			break;
		case 22:
			SetPattern(pattern, "0044001100440011");
			break;
		case 23:
			SetPattern(pattern, "1144114411441144");
			break;
		case 24:
			SetPattern(pattern, "0055005500550055");
			break;
		case 25:
			SetPattern(pattern, "00FF00FF00FF00FF");
			break;
		case 26:
			SetPattern(pattern, "4488112244881122");
			break;
		case 27:
			SetPattern(pattern, "00FF000000FF0000");
			break;
		case 28:
			SetPattern(pattern, "0408102040800102");
			break;
		case 29:
			SetPattern(pattern, "0055004000440040");
			break;
		case 30:
			SetPattern(pattern, "04FF040404040404");
			break;
		case 31:
			SetPattern(pattern, "02040E11E0408001");
			break;
		case 32:
			SetPattern(pattern, "0044A0110A440055");
			break;
		case 33:
			SetPattern(pattern, "0020500000020500");
			break;
		case 34:
			SetPattern(pattern, "0818244281601008");
			break;
		case 35:
			SetPattern(pattern, "F14040A01F04040A");
			break;
		case 36:
			SetPattern(pattern, "408001A255FF1020");
			break;
		case 37:
			SetPattern(pattern, "7CBBC4C7C7BB4C7C");
			break;
		case 38:
			SetPattern(pattern, "400040A051AA51A0");
			break;
		default:
			(void) fprintf(stderr, "%cGetIndPattern: undefined pattern\n", 7);
			break;
	}
	
	return;
}


FILE *OpenCPicture(char *fname, float native_h_dpi, float native_v_dpi, short native_h_dots, short native_v_dots)
{
	extern Point	curPos;
	Rect		destRect, srcRect;
	FILE		*fp;
	short		i, temp, screen_width, screen_height;
	short		nat_h_dpi, nat_v_dpi;
	long		long_temp;
	Fixed		fixed_temp;
	double		dbl_temp;
	
	/* calculate screen display size */
	dbl_temp = (double) native_h_dots / (double) native_h_dpi * 72.0;
	screen_width = (short) dbl_temp;
	if ((double) screen_width < dbl_temp)
		++screen_width;
	dbl_temp = (double) native_v_dots / (double) native_v_dpi * 72.0;
	screen_height = (short) dbl_temp;
	if ((double) screen_height < dbl_temp)
		++screen_height;
	
	/* initialize pen position*/
	SetPoint(&curPos, 0, 0);
	
	/* open an extended version 2 picture */
	if ((fp = fopen(fname, "wb")) == NULL) {
		(void) printf("%ccan't open file %s\n", 7, fname);
		exit(1);
	}
	
	/* space for application use */
	long_temp = 0x00000000;
	for (i = 0; i < 128; ++i)
		(void) fwrite((void *) &long_temp, sizeof(long), (size_t) 1, fp);	
	
	/* bytes in PICT -> set in ClosePicture */
	temp = 0x0000;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* bounding rectangle at 72 dpi */
	SetRect(&destRect, 0, 0, screen_width, screen_height);
	WriteRect(&destRect, fp);
	
	/* VersionOp opcode -> always 0x0011 for extended version 2 picture */
	temp = 0x0011;	
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* Version opcode -> always 0x02FF for extended version 2 picture */
	temp = 0x02FF;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* HeaderOp opcode -> always 0x0C00 for extended version 2 picture */
	temp = 0x0C00;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* header version -> always 0xFFFE for extended 2 picture */
	temp = 0xFFFE;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* reserved by Apple */
	temp = 0x0000;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);

	/* native horizontal and vertical resolutions -> "Fixed Format" */
	FloatToFixed(native_h_dpi, &fixed_temp);
	WriteFixed(&fixed_temp, fp);
	FloatToFixed(native_v_dpi, &fixed_temp);
	WriteFixed(&fixed_temp, fp);
	
	/* bounding rectangle at native resolution */
	SetRect(&srcRect, 0, 0, native_h_dots, native_v_dots);
	WriteRect(&srcRect, fp);

	/* reserved by Apple */	
	long_temp = 0x00000000;
	(void) fwrite((void *) &long_temp, sizeof(long), (size_t) 1, fp);
	
	/* DefHilite opcode to use default hilite color */
	temp = 0x001E;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
			
	/* Clip opcode to define clipping region for picture */
	temp = 0x0001;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* set clipping region size in bytes -> 10, 2 for size itself & 8 for a Rect */
	temp = 0x000A;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* set clipping region Rect -> use srcRect */
	WriteRect(&srcRect, fp);

	/* set text scale to normal point units */
	nat_h_dpi = (short) (native_h_dpi + 0.5);
	nat_v_dpi = (short) (native_v_dpi + 0.5);
	TextScale(nat_h_dpi, nat_v_dpi, 72, 72, fp);

	/* set pen size to use native resolution */
	PenSize(1, 1, fp);
	
	return(fp);
}


void	OpenGroup(FILE *fp)
{
	short	temp;
	
	/* ShortComment opcode */
	temp = 0x00A0;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* open group comment */
	temp = 0x008C;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	return;
}


FILE *OpenPicture(char *fname, short width, short height)
{
	extern Point	curPos;
	Rect		srcRect;
	FILE		*fp;
	short		i, temp;
	long		long_temp;
	
	/* initialize */
	SetPoint(&curPos, 0, 0);
	SetRect(&srcRect, 0, 0, width, height);
	
	/* open a version 2 picture */
	if ((fp = fopen(fname, "wb")) == NULL) {
		(void) printf("%ccan't open file %s\n", 7, fname);
		exit(1);
	}
	
	/* space for application use */
	long_temp = 0x00000000;
	for (i = 0; i < 128; ++i)
		(void) fwrite((void *) &long_temp, sizeof(long), (size_t) 1, fp);	
	
	/* bytes in PICT -> set in ClosePicture */
	temp = 0x0000;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* bounding rectangle at 72 dpi */
	WriteRect(&srcRect, fp);
	
	/* VersionOp opcode -> always 0x0011 for version 2 picture */
	temp = 0x0011;	
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* Version opcode -> always 0x02FF for version 2 picture */
	temp = 0x02FF;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* HeaderOp opcode -> always 0x00C0 for version 2 picture */
	temp = 0x00C0;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* header version -> always 0xFFFFFFFF for version 2 picture */
	long_temp = 0xFFFFFFFF;
	(void) fwrite((void *) &long_temp, sizeof(long), (size_t) 1, fp);
	
	/* Fixed point bounding Rect for picture */
	temp = 0x0000;
	long_temp = 0x00000000;
	(void) fwrite((void *) &long_temp, sizeof(long), (size_t) 1, fp);
	(void) fwrite((void *) &long_temp, sizeof(long), (size_t) 1, fp);
	(void) fwrite((void *) &width, sizeof(short), (size_t) 1, fp);
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	(void) fwrite((void *) &height, sizeof(short), (size_t) 1, fp);
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* reserved by Apple */
	long_temp = 0x00000000;
	(void) fwrite((void *) &long_temp, sizeof(long), (size_t) 1, fp);
	
	/* DefHilite opcode to use default hilite color */
	temp = 0x001E;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* Clip opcode to define clipping region for picture */
	temp = 0x0001;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* set clipping region size in bytes -> 10, 2 for size itself & 8 for a Rect */
	temp = 0x000A;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* set clipping region Rect -> use srcRect */
	WriteRect(&srcRect, fp);
	
	return(fp);
}


void	PaintArc(Rect *rect, short startAngle, short arcAngle, FILE *fp)
{
	short	temp;
	
	/* paintArc opcode */
	temp = 0x0061;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* write the Rect defining the arc */
	WriteRect(rect, fp);
	
	/* start angle -> 0 to 359, 0 is striaght up, angles increase clockwise */
	(void) fwrite((void *) &startAngle, sizeof(short), (size_t) 1, fp);
	
	/* arc angle -> 0 to 360 from start angle, positive is clockwise */
	(void) fwrite((void *) &arcAngle, sizeof(short), (size_t) 1, fp);
	
	return;
}


void	PaintOval(Rect *rect, FILE *fp)
{
	short	temp;
	
	/* paintOval opcode */
	temp = 0x0051;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* write the Rect defining the oval */
	WriteRect(rect, fp);
	
	return;
}


void	PaintPoly(Polygon *polygon, FILE *fp)
{	
	short	temp;
	
	/* paintPoly opcode */
	temp = 0x0071;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* polygon data */
	WritePoly(polygon, fp);
	
	return;
}


void	PaintRect(Rect *rect, FILE *fp)
{
	short	temp;
	
	/* paintRect opcode */
	temp = 0x0031;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* write the Rect */
	WriteRect(rect, fp);
	
	return;
}


void	PaintRoundRect(Rect *rect, short width, short height, FILE *fp)
{
	short	temp;
	Point	point;
	
	/* OvSize opcode */
	temp = 0x000B;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* Oval width and height */
	SetPoint(&point, width, height);
	WritePoint(&point, fp);
	
	/* paintRRect opcode */
	temp = 0x0041;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* write the Rect */
	WriteRect(rect, fp);
	
	return;
}


void	PenMode(short mode, FILE *fp)
{
	short	temp;
	
	/* PnMode opcode */
	temp = 0x0008;		
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* pen mode */
	(void) fwrite((void *) &mode, sizeof(short), (size_t) 1, fp);
	
	return;
}


void	PenNormal(FILE *fp)
{
	Pattern		pattern;
	
	PenSize(1, 1, fp);
	PenMode(PAT_COPY, fp);
	GetIndPattern(PAT_BLACK, pattern);
	PenPat(pattern, fp);
	
	return;
}


void	PenPat(Pattern pattern, FILE *fp)
{
	short	temp;
	
	/* fillPat opcode */
	temp = 0x000A;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* Pattern */
	WritePat(pattern, fp);
	
	return;
}


void	PenSize(short h, short v, FILE *fp)
{
	short	temp;
	Point	point;
	
	/* PnSize opcode */
	temp = 0x0007;		
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* pen size */
	SetPoint(&point, h, v);
	WritePoint(&point, fp);
	
	return;
}


void	RGBBackColor(RGBColor *color, FILE *fp)
{
	short	temp;
	
	/* RGBBkCol opcode */
	temp = 0x001B;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);

	/* write the color */
	WriteRGBCol(color, fp);
	
	return;
}


void	RGBForeColor(RGBColor *color, FILE *fp)
{
	short	temp;
	
	/* RGBFgCol opcode */
	temp = 0x001A;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);

	/* write the color */
	WriteRGBCol(color, fp);
	
	return;
}


void	TextFace(unsigned char face, FILE *fp)
{
	short	temp;
	
	/* TxFace opcode */
	temp = 0x0004;		
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* style */
	(void) fwrite((void *) &face, sizeof(unsigned char), (size_t) 1, fp);
	
	/* maintain alignment */
	face = 0;
	(void) fwrite((void *) &face, sizeof(unsigned char), (size_t) 1, fp);

	return;
}


void	TextFont(short fontNum, FILE *fp)
{	
	short		temp;
	
	/* TxFont opcode */
	temp = 0x0003;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* write font number */
	(void) fwrite((void *) &fontNum, sizeof(short), (size_t) 1, fp);

	return;
}


void	TextMode(short mode, FILE *fp)
{	
	short	temp;

	/* TxMode opcode */
	temp = 0x0005;		
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* write mode */
	(void) fwrite((void *) &mode, sizeof(short), (size_t) 1, fp);
	
	return;
}


void	TextScale(short h_num, short v_num, short h_denom, short v_denom, FILE *fp)
{
	short	temp;
	Point	point;
	
	/* divides the numerators by the denominators to generate scales
	for both the horizontal and vertical dimensions of the text */
	
	/* TxRatio opcode */
	temp = 0x0010;		
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* write points */
	SetPoint(&point, h_num, v_num);
	WritePoint(&point, fp);
	SetPoint(&point, h_denom, v_denom);
	WritePoint(&point, fp);
	
	return;
}
	
	
void	TextSize(short size, FILE *fp)
{
	short	temp;
	
	if (size < 0) {
		(void) fprintf(stderr, "%cTextSize: negative sizes are not allowed... using 1 point\n", 7);
		size = 1;
	}
	else if (size > 127) {
		(void) fprintf(stderr, "%cTextSize: sizes larger than 127 are not allowed... using 127 point\n", 7);
		size = 127;
	}

	/* TxSize opcode */
	temp = 0x000D;		
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* write mode */
	(void) fwrite((void *) &size, sizeof(short), (size_t) 1, fp);
	
	return;
}
	
	
/* HELPER ROUTINES */

void	FloatToFixed(float num, Fixed *fixed)
{
	long	l;	
		
	l = (long) num;	
	if (l > 32767) {
		(void) fprintf(stderr, "%cMakeFixed: integer too large for Fixed data type\n", 7);
		l = 32767;
	}
	else if (l < -32768) {
		(void) fprintf(stderr, "%cMakeFixed: integer too small for Fixed data type\n", 7);
		l = -32768;
	}	
	fixed->integer = (short) l;
	
	num -= (float) l;	
	fixed->decimal = (short) ((num * 65536.0) + 0.5);

	return;
}


void	InsetPoly(Polygon *polygon, short dh, short dv)
{
	short	i;
	float	width, height, h_scale, v_scale, temp_h, temp_v;
	Rect	polyBBox;
	
	/* get bounding box for polygon */
	SetRect(&polyBBox, 32767, 32767, -32768, -32768);
	for (i = 0; i < polygon->n_points; ++i) {
		if (polygon->points[i].v < polyBBox.top)
			polyBBox.top = polygon->points[i].v;
		else if (polygon->points[i].v > polyBBox.bottom)
			polyBBox.bottom = polygon->points[i].v;
		if (polygon->points[i].h < polyBBox.left)
			polyBBox.left = polygon->points[i].h;
		else if (polygon->points[i].h > polyBBox.right)
			polyBBox.right = polygon->points[i].h;
	}
		
	/* get scale */
	width = (float) polyBBox.right - (float) polyBBox.left;
	height = (float) polyBBox.bottom - (float) polyBBox.top;
	h_scale = (width - ((float) dh * 2.0)) / width;
	v_scale = (height - ((float) dh * 2.0)) / height;

	/* scale */
	for (i = 0; i < polygon->n_points; ++i) {
		temp_h = (float) polygon->points[i].h - (float) polyBBox.left;
		polygon->points[i].h = (short) ((h_scale * temp_h) + 0.5);
		polygon->points[i].h += (polyBBox.left + dh);
		temp_v = (float) polygon->points[i].v - (float) polyBBox.top;
		polygon->points[i].v = (short) ((v_scale * temp_v) + 0.5);
		polygon->points[i].v += (polyBBox.top + dv);
	}
	
	return;
}


void	InsetRect(Rect *rect, short dh, short dv)
{
	/* positive values of dv & dh shrink the rect, negative shrink */
	rect->left += dh;
	rect->top += dv;
	rect->right -= dh;
	rect->bottom -= dv;
	
	return;
}


void	Move(short h, short v)
{
	extern Point	curPos;
	
	curPos.h += h;
	curPos.v += v;
	
	return;
}
	

void	MoveTo(short h, short v)
{
	extern Point	curPos;
	
	curPos.h = h;
	curPos.v = v;
	
	return;
}
	

void	OffsetPoly(Polygon *polygon, short dh, short dv)
{
	short	i;
			
	for (i = 0; i < polygon->n_points; ++i) {
		polygon->points[i].h += dh;
		polygon->points[i].v += dv;
	}
	
	return;
}


void	OffsetRect(Rect *rect, short dh, short dv)
{
	rect->left += dh;
	rect->top += dv;
	rect->right += dh;
	rect->bottom += dv;
	
	return;
}


void	SetPattern(Pattern pattern, char *s)
{
	short	i;
	
	/* converts a string of hexadecimal
	characters to bytes */
	for (i = 0; i < 8; ++i) {
		if (*s >= 'a')
			*s -= 87;
		else if (*s >= 'A')
			*s -= 55;
		else
			*s -= 48;
			
		pattern[i] = *s++ << 4;
	
		if (*s >= 'a')
			*s -= 87;
		else if (*s >= 'A')
			*s -= 55;
		else
			*s -= 48;
			
		pattern[i] |= *s++;
	}
	
	return;
}


void	SetPoint(Point *point, short h, short v)
{
	point->h = h;
	point->v = v;
	
	return;
}


void	SetPoly(Polygon *polygon, short n_points, ...)
{
	va_list		ap;
	short		i, v, h;
		
	/* set polygon "n_points" field */
	polygon->n_points = n_points;
	
	/* allocate polygon Points */
	polygon->points = (Point *) calloc((size_t) n_points, sizeof(Point));
	
	/* fill in points */
	va_start(ap, n_points);
	for (i = 0; i < n_points; ++i) {
		/* use ints since unlabelled numbers default to ints
		in c and this seems the most useful way to use this function */
		h = (short) va_arg(ap, int);
		v = (short) va_arg(ap, int); 
		SetPoint(&polygon->points[i], h, v);
	}
	va_end(ap);
	
	return;
}
	

void	SetRect(Rect *rect, short left, short top, short right, short bottom)
{
	rect->left = left;
	rect->top = top;
	rect->right = right;
	rect->bottom = bottom;
	
	return;
}


void	SetRGBCol(RGBColor *color, short red, short grn, short blu)
{
	color->red = red;
	color->grn = grn;
	color->blu = blu;
	
	return;
}			


void	WritePat(Pattern pattern, FILE *fp)
{
	short	i;

	for (i = 0; i < 8; ++i)
		(void) fwrite((void *) (pattern + i), sizeof(unsigned char), (size_t) 1, fp);
	
	return;
}


void	WriteFixed(Fixed *fixed, FILE *fp)
{
	short	temp;
	
	temp = fixed->integer;	
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	temp = fixed->decimal;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);

	return;
}


void	WritePoint(Point *point, FILE *fp)
{
	short	temp;
	
	temp = point->v;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	temp = point->h;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	return;
}


void	WritePoly(Polygon *polygon, FILE *fp)
{	
	short	i, temp;
	Rect	polyBBox;
		
	/* write polygon size */
	temp = 10 + ((polygon->n_points + 1) * 4);
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	/* write polygon Rect */
	SetRect(&polyBBox, 32767, 32767, -32768, -32768);
	for (i = 0; i < polygon->n_points; ++i) {
		if (polygon->points[i].v < polyBBox.top)
			polyBBox.top = polygon->points[i].v;
		else if (polygon->points[i].v > polyBBox.bottom)
			polyBBox.bottom = polygon->points[i].v;
		if (polygon->points[i].h < polyBBox.left)
			polyBBox.left = polygon->points[i].h;
		else if (polygon->points[i].h > polyBBox.right)
			polyBBox.right = polygon->points[i].h;
	}		
	WriteRect(&polyBBox, fp);

	/* write polygon */
	for (i = 0; i < polygon->n_points; ++i)
		WritePoint(&polygon->points[i], fp);
	WritePoint(&polygon->points[0], fp);
	
	return;
}
	
	
void	WriteRect(Rect *rect, FILE *fp)
{
	short	temp;
	
	temp = rect->top;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);		
	temp = rect->left;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	temp = rect->bottom;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);	
	temp = rect->right;
	(void) fwrite((void *) &temp, sizeof(short), (size_t) 1, fp);
	
	return;
}


void	WriteRGBCol(RGBColor *color, FILE *fp)
{
	(void) fwrite((void *) &color->red, sizeof(short), (size_t) 1, fp);
	(void) fwrite((void *) &color->grn, sizeof(short), (size_t) 1, fp);
	(void) fwrite((void *) &color->blu, sizeof(short), (size_t) 1, fp);
	
	return;
}
