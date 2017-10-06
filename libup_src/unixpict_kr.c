
#include "unixpict.h"


/* QUICKDRAW GLOBALS */

POINT	curPos;


/* QUICKDRAW ROUTINES */

void	BackColor(color, fp)
	S_I_4 color;
	FILE	*fp;
{
	S_I_2	temp_si2;
	
	/* BkCol opcode */
	temp_si2 = 0x000F;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* old background color */
	UPWrite((void *) &color, sizeof(S_I_4), (size_t) 1, fp);
	
	return;
}


void	BackPat(pattern, fp)
	PATTERN pattern;
	 FILE *fp;
{
	S_I_2	temp_si2;
	
	/* BkPat opcode */
	temp_si2 = 0x0002;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* Pattern */
	WritePat(pattern, fp);
	
	return;
}


void	DrawChar(c, fp)
	S_I_1 c;
	FILE *fp;
{	
	S_I_1	s[2];
	
	s[0] = c;
	s[1] = '\0';
	DrawString(s, fp);
	
	return;
}	


void	DrawString(s, fp)
	S_I_1 *s;
	 FILE *fp;
{
	extern POINT	curPos;
	S_I_2		len, temp_si2;
	S_I_1		*c;
	U_I_1		l;
	
	c = s - 1;
	while (*++c);
	
	if ((len = c - s) > 255) {
		(void) fprintf(stderr, "%cDrawString: string too long... truncating it\n", 7);
		len = 255;
	}
	
	/* LongText opcode */
	temp_si2 = 0x0028;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* current pen location */
	WritePoint(&curPos, fp);
	
	/* string length */
	l = (U_I_1) len;
	UPWrite((void *) &l, sizeof(U_I_1), (size_t) 1, fp);
	
	/* maS_I_4ain 2 byte alignment */
	if (!(len % 2))
		++len;
	
	/* string */
	UPWrite((void *) s, sizeof(S_I_1), (size_t) len, fp);

	return;
}	


void	CloseGroup(fp)
	FILE *fp;
{
	S_I_2	temp_si2;
	
	/* ShortComment opcode */
	temp_si2 = 0x00A0;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* open group comment */
	temp_si2 = 0x008D;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	return;
}


void	ClosePicture(fp)
	FILE	*fp;
{
	S_I_2	temp_si2;
	S_I_4	l;

	/* OpEndPic opcode -> end of picture */
	temp_si2 = 0x00FF;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* get picture size */
	l = ftell(fp) - 512L;
	
	if (l > 32767)
		l = 32767;

	/* fill in picture size */
	(void) fseek(fp, 512L, 0);
	temp_si2 = (S_I_2) l;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	(void) fclose(fp);
	
	return;
}


void	EraseArc(rect, startAngle, arcAngle, fp)
	RECT *rect;
 	S_I_2 startAngle, arcAngle;
	FILE *fp;
{
	S_I_2	temp_si2;
	
	/* eraseArc opcode */
	temp_si2 = 0x0062;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* write the Rect defining the arc */
	WriteRect(rect, fp);
	
	/* start angle -> 0 to 359, 0 is striaght up, angles increase clockwise */
	UPWrite((void *) &startAngle, sizeof(S_I_2), (size_t) 1, fp);
	
	/* arc angle -> 0 to 360 from start angle, positive is clockwise */
	UPWrite((void *) &arcAngle, sizeof(S_I_2), (size_t) 1, fp);
	
	return;
}


void	EraseOval(rect, fp)
	RECT *rect;
	FILE *fp;
{
	S_I_2	temp_si2;
	
	/* eraseOval opcode */
	temp_si2 = 0x0052;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* write the Rect defining the oval */
	WriteRect(rect, fp);
	
	return;
}


void	ErasePoly(polygon, fp)
	POLYGON *polygon;
	FILE *fp;
{	
	S_I_2	temp_si2;
	
	/* erasePoly opcode */
	temp_si2 = 0x0072;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* polygon data */
	WritePoly(polygon, fp);
	
	return;
}


void	EraseRect(rect, fp)
	RECT *rect;
	FILE *fp;
{
	S_I_2	temp_si2;
	
	/* eraseRect opcode */
	temp_si2 = 0x0032;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* write the Rect */
	WriteRect(rect, fp);
	
	return;
}


void	EraseRoundRect(rect, width, height, fp)
	RECT *rect;
	S_I_2 width, height;
	FILE *fp;
{
	S_I_2	temp_si2;
	POINT	point;
	
	/* OvSize opcode */
	temp_si2 = 0x000B;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* Oval width and height */
	SetPoint(&point, width, height);
	WritePoint(&point, fp);
	
	/* eraseRRect opcode */
	temp_si2 = 0x0042;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* write the Rect */
	WriteRect(rect, fp);
	
	return;
}


void	FillArc(rect, startAngle, arcAngle, fp)
	RECT *rect;
	S_I_2 startAngle, arcAngle;
	FILE *fp;
{
	S_I_2	temp_si2;
	
	/* fillArc opcode */
	temp_si2 = 0x0064;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* write the Rect defining the arc */
	WriteRect(rect, fp);
	
	/* start angle -> 0 to 359, 0 is straight up, angles increase clockwise */
	UPWrite((void *) &startAngle, sizeof(S_I_2), (size_t) 1, fp);
	
	/* arc angle -> 0 to 360 from start angle, positive is clockwise */
	UPWrite((void *) &arcAngle, sizeof(S_I_2), (size_t) 1, fp);
	
	return;
}


void	FillOval(rect, fp)
	RECT *rect;
	FILE *fp;
{
	S_I_2	temp_si2;
	
	/* fillOval opcode */
	temp_si2 = 0x0054;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* write the Rect defining the oval */
	WriteRect(rect, fp);
	
	return;
}


void	FillPoly(polygon, fp)
	POLYGON *polygon;
	FILE *fp;
{	
	S_I_2	temp_si2;
	
	/* fillPoly opcode */
	temp_si2 = 0x0074;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);

	/* polygon data */
	WritePoly(polygon, fp);
	
	return;
}

	
void	FillRect(rect, fp)
	RECT *rect;
	FILE *fp;
{
	S_I_2	temp_si2;
	
	/* fillRect opcode */
	temp_si2 = 0x0034;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* write the Rect */
	WriteRect(rect, fp);
	
	return;
}


void	FillRoundRect(rect, width, height, fp)
	RECT *rect;
	S_I_2 width, height;
	FILE *fp;
{
	S_I_2	temp_si2;
	POINT	point;
	
	/* OvSize opcode */
	temp_si2 = 0x000B;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* Oval width and height */
	SetPoint(&point, width, height);
	WritePoint(&point, fp);
	
	/* fillRRect opcode */
	temp_si2 = 0x0044;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* write the Rect */
	WriteRect(rect, fp);
	
	return;
}


void	ForeColor(color, fp)
	S_I_4 color;
	FILE *fp;
{
	S_I_2	temp_si2;
	
	/* FgCol opcode */
	temp_si2 = 0x000E;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* old foreground color */
	UPWrite((void *) &color, sizeof(S_I_4), (size_t) 1, fp);
	
	return;
}


void	FrameArc(rect, startAngle, arcAngle, fp)
	RECT *rect;
	S_I_2 startAngle, arcAngle;
	FILE *fp;
{
	S_I_2	temp_si2;
	
	/* frameArc opcode */
	temp_si2 = 0x0060;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* write the Rect defining the arc */
	WriteRect(rect, fp);
	
	/* start angle -> 0 to 359, 0 is striaght up, angles increase clockwise */
	UPWrite((void *) &startAngle, sizeof(S_I_2), (size_t) 1, fp);
	
	/* arc angle -> 0 to 360 from start angle, positive is clockwise */
	UPWrite((void *) &arcAngle, sizeof(S_I_2), (size_t) 1, fp);
	
	return;
}


void	FrameOval(rect, fp)
	RECT *rect;
	FILE *fp;
{
	S_I_2	temp_si2;
	
	/* frameOval opcode */
	temp_si2 = 0x0050;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* write the Rect defining the oval */
	WriteRect(rect, fp);
	
	return;
}


void	FramePoly(polygon, fp)
	POLYGON *polygon;
	FILE *fp;
{	
	S_I_2	temp_si2;
	
	/* framePoly opcode */
	temp_si2 = 0x0070;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* polygon data */
	WritePoly(polygon, fp);
	
	return;
}


void	FrameRect(rect, fp)
	RECT *rect;
	FILE *fp;
{
	S_I_2	temp_si2;
	
	/* frameRect opcode */
	temp_si2 = 0x0030;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* write the Rect */
	WriteRect(rect, fp);
	
	return;
}


void	FrameRoundRect(rect, width, height, fp)
	RECT *rect;
	S_I_2 width, height;
	FILE *fp;
{
	S_I_2	temp_si2;
	POINT	point;
	
	/* OvSize opcode */
	temp_si2 = 0x000B;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* Oval width and height */
	SetPoint(&point, width, height);
	WritePoint(&point, fp);
	
	/* frameRRect opcode */
	temp_si2 = 0x0040;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* write the Rect */
	WriteRect(rect, fp);
	
	return;
}


void	InvertArc(rect, startAngle, arcAngle, fp)
	RECT *rect;
	S_I_2 startAngle, arcAngle;
	FILE *fp;
{
	S_I_2	temp_si2;
	
	/* invertArc opcode */
	temp_si2 = 0x0063;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* write the Rect defining the arc */
	WriteRect(rect, fp);
	
	/* start angle -> 0 to 359, 0 is striaght up, angles increase clockwise */
	UPWrite((void *) &startAngle, sizeof(S_I_2), (size_t) 1, fp);
	
	/* arc angle -> 0 to 360 from start angle, positive is clockwise */
	UPWrite((void *) &arcAngle, sizeof(S_I_2), (size_t) 1, fp);
	
	return;
}


void	InvertOval(rect, fp)
	RECT *rect;
	FILE *fp;
{
	S_I_2	temp_si2;
	
	/* invertOval opcode */
	temp_si2 = 0x0053;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* write the Rect defining the oval */
	WriteRect(rect, fp);
	
	return;
}


void	InvertPoly(polygon, fp)
	POLYGON *polygon;
	FILE *fp;
{	
	S_I_2	temp_si2;
	
	/* invertPoly opcode */
	temp_si2 = 0x0073;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* polygon data */
	WritePoly(polygon, fp);
	
	return;
}


void	InvertRect(rect, fp)
	RECT *rect;
	FILE *fp;
{
	S_I_2	temp_si2;
	
	/* invertRect opcode */
	temp_si2 = 0x0033;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* write the Rect */
	WriteRect(rect, fp);
	
	return;
}


void	InvertRoundRect(rect, width, height, fp)
	RECT *rect; 
	S_I_2 width, height;
 	FILE *fp;
{
	S_I_2	temp_si2;
	POINT	point;
	
	/* OvSize opcode */
	temp_si2 = 0x000B;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* Oval width and height */
	SetPoint(&point, width, height);
	WritePoint(&point, fp);
	
	/* invertRRect opcode */
	temp_si2 = 0x0043;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* write the Rect */
	WriteRect(rect, fp);
	
	return;
}


void	Line(dh, dv, fp)
	S_I_2 dh, dv;
	 FILE *fp;
{
	extern POINT	curPos;
	S_I_2		temp_si2;
	
	/* Line opcode */
	temp_si2 = 0x0020;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* current pen location */
	WritePoint(&curPos, fp);

	/* new pen location */
	curPos.h += dh;
	curPos.v += dv;
	WritePoint(&curPos, fp);

	return;
}


void	LineTo(h, v, fp)
	S_I_2 h, v; 
	FILE *fp;
{
	extern POINT	curPos;
	S_I_2		temp_si2;
	
	/* Line opcode */
	temp_si2 = 0x0020;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* current pen location */
	WritePoint(&curPos, fp);

	/* new pen location */
	curPos.h = h;
	curPos.v = v;
	WritePoint(&curPos, fp);

	return;
}


void	GetIndPattern(index, pattern)
	S_I_2 index;
	 PATTERN pattern;
{
	/* the 38 standard MacS_I_4osh Patterns */
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


FILE *OpenCPicture(fname, native_h_dpi, native_v_dpi, native_h_dots, native_v_dots)
	S_I_1 *fname;
	S_F_4 native_h_dpi, native_v_dpi;
	S_I_2 native_h_dots, native_v_dots;
{
	extern POINT	curPos;
	RECT		destRect, srcRect;
	FILE		*fp;
	S_I_2		i, temp_si2, screen_width, screen_height;
	S_I_2		nat_h_dpi, nat_v_dpi;
	S_I_4		temp_si4;
	FIXED		fixed_temp;
	S_F_8		temp_S_F_8;
	
	/* calculate screen display size */
	temp_S_F_8 = (S_F_8) native_h_dots / (S_F_8) native_h_dpi * 72.0;
	screen_width = (S_I_2) temp_S_F_8;
	if ((S_F_8) screen_width < temp_S_F_8)
		++screen_width;
	temp_S_F_8 = (S_F_8) native_v_dots / (S_F_8) native_v_dpi * 72.0;
	screen_height = (S_I_2) temp_S_F_8;
	if ((S_F_8) screen_height < temp_S_F_8)
		++screen_height;
	
	/* initialize pen position*/
	SetPoint(&curPos, 0, 0);
	
	/* open an extended version 2 picture */
	if ((fp = fopen(fname, "wb")) == NULL) {
		(void) printf("%ccan't open file %s\n", 7, fname);
		exit(1);
	}
	
	/* space for application use */
	temp_si4 = 0x00000000;
	for (i = 0; i < 128; ++i)
		UPWrite((void *) &temp_si4, sizeof(S_I_4), (size_t) 1, fp);	
	
	/* bytes in PICT -> set in ClosePicture */
	temp_si2 = 0x0000;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* bounding rectangle at 72 dpi */
	SetRect(&destRect, 0, 0, screen_width, screen_height);
	WriteRect(&destRect, fp);
	
	/* VersionOp opcode -> always 0x0011 for extended version 2 picture */
	temp_si2 = 0x0011;	
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* Version opcode -> always 0x02FF for extended version 2 picture */
	temp_si2 = 0x02FF;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* HeaderOp opcode -> always 0x0C00 for extended version 2 picture */
	temp_si2 = 0x0C00;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* header version -> always 0xFFFE for extended 2 picture */
	temp_si2 = 0xFFFE;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* reserved by Apple */
	temp_si2 = 0x0000;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);

	/* native horizontal and vertical resolutions -> "Fixed Format" */
	FloatToFixed(native_h_dpi, &fixed_temp);
	WriteFixed(&fixed_temp, fp);
	FloatToFixed(native_v_dpi, &fixed_temp);
	WriteFixed(&fixed_temp, fp);
	
	/* bounding rectangle at native resolution */
	SetRect(&srcRect, 0, 0, native_h_dots, native_v_dots);
	WriteRect(&srcRect, fp);

	/* reserved by Apple */	
	temp_si4 = 0x00000000;
	UPWrite((void *) &temp_si4, sizeof(S_I_4), (size_t) 1, fp);
	
	/* DefHilite opcode to use default hilite color */
	temp_si2 = 0x001E;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
			
	/* Clip opcode to define clipping region for picture */
	temp_si2 = 0x0001;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* set clipping region size in bytes -> 10, 2 for size itself & 8 for a Rect */
	temp_si2 = 0x000A;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* set clipping region Rect -> use srcRect */
	WriteRect(&srcRect, fp);

	/* set text scale to normal point units */
	nat_h_dpi = (S_I_2) (native_h_dpi + 0.5);
	nat_v_dpi = (S_I_2) (native_v_dpi + 0.5);
	TextScale(nat_h_dpi, nat_v_dpi, 72, 72, fp);

	/* set pen size to use native resolution */
	PenSize(1, 1, fp);
	
	return(fp);
}


void	OpenGroup(fp)
	FILE *fp;
{
	S_I_2	temp_si2;
	
	/* ShortComment opcode */
	temp_si2 = 0x00A0;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* open group comment */
	temp_si2 = 0x008C;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	return;
}


FILE *OpenPicture(fname, width, height)
	S_I_1 *fname;
	S_I_2 width, height;
{
	extern POINT	curPos;
	RECT		srcRect;
	FILE		*fp;
	S_I_2		i, temp_si2;
	S_I_4		temp_si4;
	
	/* initialize */
	SetPoint(&curPos, 0, 0);
	SetRect(&srcRect, 0, 0, width, height);
	
	/* open a version 2 picture */
	if ((fp = fopen(fname, "wb")) == NULL) {
		(void) printf("%ccan't open file %s\n", 7, fname);
		exit(1);
	}
	
	/* space for application use */
	temp_si4 = 0x00000000;
	for (i = 0; i < 128; ++i)
		UPWrite((void *) &temp_si4, sizeof(S_I_4), (size_t) 1, fp);	
	
	/* bytes in PICT -> set in ClosePicture */
	temp_si2 = 0x0000;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* bounding rectangle at 72 dpi */
	WriteRect(&srcRect, fp);
	
	/* VersionOp opcode -> always 0x0011 for version 2 picture */
	temp_si2 = 0x0011;	
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* Version opcode -> always 0x02FF for version 2 picture */
	temp_si2 = 0x02FF;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* HeaderOp opcode -> always 0x00C0 for version 2 picture */
	temp_si2 = 0x00C0;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* header version -> always 0xFFFFFFFF for version 2 picture */
	temp_si4 = 0xFFFFFFFF;
	UPWrite((void *) &temp_si4, sizeof(S_I_4), (size_t) 1, fp);
	
	/* Fixed point bounding Rect for picture */
	temp_si2 = 0x0000;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	UPWrite((void *) &width, sizeof(S_I_2), (size_t) 1, fp);
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	UPWrite((void *) &height, sizeof(S_I_2), (size_t) 1, fp);
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* reserved by Apple */
	temp_si4 = 0x00000000;
	UPWrite((void *) &temp_si4, sizeof(S_I_4), (size_t) 1, fp);
	
	/* DefHilite opcode to use default hilite color */
	temp_si2 = 0x001E;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* Clip opcode to define clipping region for picture */
	temp_si2 = 0x0001;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* set clipping region size in bytes -> 10, 2 for size itself & 8 for a Rect */
	temp_si2 = 0x000A;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* set clipping region Rect -> use srcRect */
	WriteRect(&srcRect, fp);
	
	return(fp);
}


void	PaintArc(rect, startAngle, arcAngle, fp)
	RECT *rect;
	S_I_2 startAngle, arcAngle;
	FILE *fp;
{
	S_I_2	temp_si2;
	
	/* paS_I_4Arc opcode */
	temp_si2 = 0x0061;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* write the Rect defining the arc */
	WriteRect(rect, fp);
	
	/* start angle -> 0 to 359, 0 is striaght up, angles increase clockwise */
	UPWrite((void *) &startAngle, sizeof(S_I_2), (size_t) 1, fp);
	
	/* arc angle -> 0 to 360 from start angle, positive is clockwise */
	UPWrite((void *) &arcAngle, sizeof(S_I_2), (size_t) 1, fp);
	
	return;
}


void	PaintOval(rect, fp)
	RECT *rect;
	FILE *fp;
{
	S_I_2	temp_si2;
	
	/* paS_I_4Oval opcode */
	temp_si2 = 0x0051;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* write the Rect defining the oval */
	WriteRect(rect, fp);
	
	return;
}


void	PaintPoly(polygon, fp)
	POLYGON *polygon;
	FILE *fp;
{	
	S_I_2	temp_si2;
	
	/* paS_I_4Poly opcode */
	temp_si2 = 0x0071;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* polygon data */
	WritePoly(polygon, fp);
	
	return;
}


void	PaintRect(rect, fp)
	RECT *rect;
	FILE *fp;
{
	S_I_2	temp_si2;
	
	/* paintRect opcode */
	temp_si2 = 0x0031;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* write the Rect */
	WriteRect(rect, fp);
	
	return;
}


void	PaintRoundRect(rect, width, height, fp)
	RECT *rect;
	S_I_2 width, height;
	FILE *fp;
{
	S_I_2	temp_si2;
	POINT	point;
	
	/* OvSize opcode */
	temp_si2 = 0x000B;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* Oval width and height */
	SetPoint(&point, width, height);
	WritePoint(&point, fp);
	
	/* paintRRect opcode */
	temp_si2 = 0x0041;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* write the Rect */
	WriteRect(rect, fp);
	
	return;
}


void	PenMode(mode, fp)
	S_I_2 mode;
	FILE *fp;
{
	S_I_2	temp_si2;
	
	/* PnMode opcode */
	temp_si2 = 0x0008;		
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* pen mode */
	UPWrite((void *) &mode, sizeof(S_I_2), (size_t) 1, fp);
	
	return;
}


void	PenNormal(fp)
	FILE *fp;
{
	PATTERN		pattern;
	
	PenSize(1, 1, fp);
	PenMode(PAT_COPY, fp);
	GetIndPattern(PAT_BLACK, pattern);
	PenPat(pattern, fp);
	
	return;
}


void	PenPat(pattern, fp)
	PATTERN pattern;
	FILE *fp;
{
	S_I_2	temp_si2;
	
	/* fillPat opcode */
	temp_si2 = 0x000A;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* Pattern */
	WritePat(pattern, fp);
	
	return;
}


void	PenSize(h, v, fp)
	S_I_2 h, v; 
	FILE *fp;
{
	S_I_2	temp_si2;
	POINT	point;
	
	/* PnSize opcode */
	temp_si2 = 0x0007;		
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* pen size */
	SetPoint(&point, h, v);
	WritePoint(&point, fp);
	
	return;
}


void	RGBBackColor(color, fp)
	RGBCOLOR *color;
	FILE *fp;
{
	S_I_2	temp_si2;
	
	/* RGBBkCol opcode */
	temp_si2 = 0x001B;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);

	/* write the color */
	WriteRGBCol(color, fp);
	
	return;
}


void	RGBForeColor(color, fp)
	RGBCOLOR *color;
	FILE *fp;
{
	S_I_2	temp_si2;
	
	/* RGBFgCol opcode */
	temp_si2 = 0x001A;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);

	/* write the color */
	WriteRGBCol(color, fp);
	
	return;
}


void	TextFace(face, fp)
	U_I_1 face;
	FILE *fp;
{
	S_I_2	temp_si2;
	
	/* TxFace opcode */
	temp_si2 = 0x0004;		
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* style */
	UPWrite((void *) &face, sizeof(U_I_1), (size_t) 1, fp);
	
	/* maS_I_4ain alignment */
	face = 0;
	UPWrite((void *) &face, sizeof(U_I_1), (size_t) 1, fp);

	return;
}


void	TextFont(fontNum, fp)
	S_I_2 fontNum;
	FILE *fp;
{	
	S_I_2		temp_si2;
	
	/* TxFont opcode */
	temp_si2 = 0x0003;
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* write font number */
	UPWrite((void *) &fontNum, sizeof(S_I_2), (size_t) 1, fp);

	return;
}


void	TextMode(mode, fp)
	S_I_2 mode;
	FILE *fp;
{	
	S_I_2	temp_si2;

	/* TxMode opcode */
	temp_si2 = 0x0005;		
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* write mode */
	UPWrite((void *) &mode, sizeof(S_I_2), (size_t) 1, fp);
	
	return;
}


void	TextScale(h_num, v_num, h_denom, v_denom, fp)
	S_I_2 h_num, v_num, h_denom, v_denom;
	FILE *fp;
{
	S_I_2	temp_si2;
	POINT	point;
	
	/* divides the numerators by the denominators to generate scales
	for both the horizontal and vertical dimensions of the text */
	
	/* TxRatio opcode */
	temp_si2 = 0x0010;		
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* write points */
	SetPoint(&point, h_num, v_num);
	WritePoint(&point, fp);
	SetPoint(&point, h_denom, v_denom);
	WritePoint(&point, fp);
	
	return;
}
	
	
void	TextSize(size, fp)
	S_I_2 size;
	FILE *fp;
{
	S_I_2	temp_si2;
	
	if (size < 0) {
		(void) fprintf(stderr, "%cTextSize: negative sizes are not allowed... using 1 point\n", 7);
		size = 1;
	}
	else if (size > 127) {
		(void) fprintf(stderr, "%cTextSize: sizes larger than 127 are not allowed... using 127 point\n", 7);
		size = 127;
	}

	/* TxSize opcode */
	temp_si2 = 0x000D;		
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
	/* write mode */
	UPWrite((void *) &size, sizeof(S_I_2), (size_t) 1, fp);
	
	return;
}
	
	
/* HELPER ROUTINES */

void	FloatToFixed(num, fixed)
	S_F_4 num;
	FIXED *fixed;
{
	S_I_4	l;	
		
	l = (S_I_4) num;	
	if (l > 32767) {
		(void) fprintf(stderr, "%cMakeFixed: integer too large for Fixed data type\n", 7);
		l = 32767;
	}
	else if (l < -32768) {
		(void) fprintf(stderr, "%cMakeFixed: integer too small for Fixed data type\n", 7);
		l = -32768;
	}	
	fixed->integer = (S_I_2) l;
	
	num -= (S_F_4) l;	
	fixed->decimal = (S_I_2) ((num * 65536.0) + 0.5);

	return;
}


void	InsetPoly(polygon, dh, dv)
	POLYGON *polygon;
	S_I_2 dh, dv;
{
	S_I_2	i;
	S_F_4	width, height, h_scale, v_scale, temp_h, temp_v;
	RECT	polyBBox;
	
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
	width = (S_F_4) polyBBox.right - (S_F_4) polyBBox.left;
	height = (S_F_4) polyBBox.bottom - (S_F_4) polyBBox.top;
	h_scale = (width - ((S_F_4) dh * 2.0)) / width;
	v_scale = (height - ((S_F_4) dh * 2.0)) / height;

	/* scale */
	for (i = 0; i < polygon->n_points; ++i) {
		temp_h = (S_F_4) polygon->points[i].h - (S_F_4) polyBBox.left;
		polygon->points[i].h = (S_I_2) ((h_scale * temp_h) + 0.5);
		polygon->points[i].h += (polyBBox.left + dh);
		temp_v = (S_F_4) polygon->points[i].v - (S_F_4) polyBBox.top;
		polygon->points[i].v = (S_I_2) ((v_scale * temp_v) + 0.5);
		polygon->points[i].v += (polyBBox.top + dv);
	}
	
	return;
}


void	InsetRect(rect, dh, dv)
	RECT *rect;
	S_I_2 dh, dv;
{
	/* positive values of dv & dh shrink the rect, negative shrink */
	rect->left += dh;
	rect->top += dv;
	rect->right -= dh;
	rect->bottom -= dv;
	
	return;
}


void	Move(h, v)
	S_I_2 h, v;
{
	extern POINT	curPos;
	
	curPos.h += h;
	curPos.v += v;
	
	return;
}
	

void	MoveTo(h, v)
	S_I_2 h, v;
{
	extern POINT	curPos;
	
	curPos.h = h;
	curPos.v = v;
	
	return;
}
	

void	OffsetPoly(polygon, dh, dv)
	POLYGON *polygon;
	S_I_2 dh, dv;
{
	S_I_2	i;
			
	for (i = 0; i < polygon->n_points; ++i) {
		polygon->points[i].h += dh;
		polygon->points[i].v += dv;
	}
	
	return;
}


void	OffsetRect(rect, dh, dv)
	RECT *rect;
	S_I_2 dh, dv;
{
	rect->left += dh;
	rect->top += dv;
	rect->right += dh;
	rect->bottom += dv;
	
	return;
}


void	SetPattern(pattern, s)
	PATTERN pattern;
	S_I_1 *s;
{
	S_I_2	i;
	
	/* converts a string of hexadecimal
	S_I_1acters to bytes */
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


void	SetPoint(point, h, v)
	POINT *point;
	S_I_2 h, v;
{
	point->h = h;
	point->v = v;
	
	return;
}


/*
void	SetPoly(polygon, n_points, ...)
	POLYGON *polygon;
	S_I_2 n_points;
{
	va_list		ap;
	S_I_2		i, v, h;
		
	** set polygon "n_points" field **
	polygon->n_points = n_points;
	
	** allocate polygon Points **
	polygon->points = (POINT *) calloc((unsigned) n_points, sizeof(POINT));
	
	** fill in points **
	va_start(ap, n_points); Capatilized to prevent macro substitution
	for (i = 0; i < n_points; ++i) {
		use ints since unlabelled numbers default to ints in c
		and this seems the most useful way to use this function 
		h = (S_I_2) va_arg(ap, int);
		v = (S_I_2) va_arg(ap, int); 
		SetPoint(&polygon->points[i], h, v);
	}
	va_end(ap); Capatilized to prevent macro substitution
	
	
	return;
}
*/
	

void	SetRect(rect, left, top, right, bottom)
	RECT *rect;
	S_I_2 left, top, right, bottom;
{
	rect->left = left;
	rect->top = top;
	rect->right = right;
	rect->bottom = bottom;
	
	return;
}


void	SetRGBCol(color, red, grn, blu)
	RGBCOLOR *color;
	S_I_2 red, grn, blu;
{
	color->red = red;
	color->grn = grn;
	color->blu = blu;
	
	return;
}			


void	UPWrite(data, size, n, fp)
	void *data;
	size_t size, n;
	FILE *fp;
{

#if defined(BIG_ENDIAN_MACHINE)

	(void) fwrite((char *)data, size, n, fp);

#elif defined(LITTLE_ENDIAN_MACHINE)

	register U_I_1	*a, *b;
	register S_I_4	i, j;

	a = (U_I_1 *) data;
	for (i = n; i--;) {	
		a += size;
		b = a;
		for (j = size; j--;)
			(void) fwrite((char *) (--b), sizeof(U_I_1), (size_t) 1, fp);
	}

#else

	(void) fprintf(stderr, "%c\n\tMachine architecture not defined in header file.\n\tDefine and recompile.\n\n", 7);
	exit(1);

#endif

	return;
}


void	WritePat(pattern, fp)
	PATTERN pattern;
	FILE *fp;
{
	S_I_2	i;

	for (i = 0; i < 8; ++i)
		UPWrite((void *) (pattern + i), sizeof(U_I_1), (size_t) 1, fp);
	
	return;
}


void	WriteFixed(fixed, fp)
	FIXED *fixed;
	FILE *fp;
{
	UPWrite((void *) &fixed->integer, sizeof(S_I_2), (size_t) 1, fp);
	UPWrite((void *) &fixed->decimal, sizeof(S_I_2), (size_t) 1, fp);

	return;
}


void	WritePoint(point, fp)
	POINT *point;
	FILE *fp;
{
	UPWrite((void *) &point->v, sizeof(S_I_2), (size_t) 1, fp);
	UPWrite((void *) &point->h, sizeof(S_I_2), (size_t) 1, fp);
	
	return;
}


void	WritePoly(polygon, fp)
	POLYGON *polygon;
	FILE *fp;
{	
	S_I_2	i, temp_si2;
	RECT	polyBBox;
		
	/* write polygon size */
	temp_si2 = 10 + (polygon->n_points * 4);
	UPWrite((void *) &temp_si2, sizeof(S_I_2), (size_t) 1, fp);
	
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
	
	return;
}
	
	
void	WriteRect(rect, fp)
	RECT *rect;
	FILE *fp;
{
	UPWrite((void *) &rect->top, sizeof(S_I_2), (size_t) 1, fp);		
	UPWrite((void *) &rect->left, sizeof(S_I_2), (size_t) 1, fp);
	UPWrite((void *) &rect->bottom, sizeof(S_I_2), (size_t) 1, fp);	
	UPWrite((void *) &rect->right, sizeof(S_I_2), (size_t) 1, fp);
	
	return;
}


void	WriteRGBCol(color, fp)
	RGBCOLOR *color; 
	FILE *fp;
{
	UPWrite((void *) &color->red, sizeof(S_I_2), (size_t) 1, fp);
	UPWrite((void *) &color->grn, sizeof(S_I_2), (size_t) 1, fp);
	UPWrite((void *) &color->blu, sizeof(S_I_2), (size_t) 1, fp);
	
	return;
}
