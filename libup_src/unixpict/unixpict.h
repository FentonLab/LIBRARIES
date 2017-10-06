#ifndef UNIX_PICT_IN
#define UNIX_PICT_IN


/* includes */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>


/* typedefs */

typedef	struct {
		short	top;
		short	left;
		short	bottom;
		short	right;
	} Rect;
		
typedef struct {
		short		integer;
		unsigned short	decimal;
	} Fixed;
		
typedef struct {
		short	v;
		short	h;
	} Point;
	
typedef unsigned char 	Pattern[8];

typedef struct {
		short	n_points;
		Point	*points;
	} Polygon;
		
typedef struct {
		unsigned short	red, grn, blu;
	} RGBColor;
		

/* prototypes */

void	BackColor(long, FILE *);
void	BackPat(Pattern, FILE *);
void	DrawChar(char,  FILE *);
void	DrawString(char *,  FILE *);
void	CloseGroup(FILE *);
void	ClosePicture(FILE *);
void	EraseArc(Rect *, short, short, FILE *);
void	EraseOval(Rect *, FILE *);
void	ErasePoly(Polygon *, FILE *);
void	EraseRect(Rect *, FILE *);
void	EraseRoundRect(Rect *, short, short, FILE *);
void	FillArc(Rect *, short, short, Pattern, FILE *);
void	FillOval(Rect *, Pattern, FILE *);
void	FillPoly(Polygon *, Pattern, FILE *);
void	FillRect(Rect *, Pattern, FILE *);
void	FillRoundRect(Rect *, short, short, Pattern, FILE *);
void	FloatToFixed(float, Fixed *);
void	ForeColor(long, FILE *);
void	FrameArc(Rect *, short, short, FILE *);
void	FrameOval(Rect *, FILE *);
void	FramePoly(Polygon *, FILE *);
void	FrameRect(Rect *, FILE *);
void	FrameRoundRect(Rect *, short, short, FILE *);
void	InvertArc(Rect *, short, short, FILE *);
void	InvertOval(Rect *, FILE *);
void	InvertPoly(Polygon *, FILE *);
void	InvertRect(Rect *, FILE *);
void	InvertRoundRect(Rect *, short, short, FILE *);
void	GetIndPattern(short, Pattern);
void	InsetPoly(Polygon *, short, short);
void	InsetRect(Rect *, short, short);
void	Line(short, short, FILE *);
void	LineTo(short, short, FILE *);
void	Move(short, short);
void	PenNormal(FILE *);
void	MoveTo(short, short);
void	OffsetPoly(Polygon *, short, short);
void	OffsetRect(Rect *, short, short);
FILE	*OpenCPicture(char *, float, float, short, short);
void	OpenGroup(FILE *);
FILE	*OpenPicture(char *, short, short);
void	PaintArc(Rect *, short, short, FILE *);
void	PaintOval(Rect *, FILE *);
void	PaintPoly(Polygon *, FILE *);
void	PaintRect(Rect *, FILE *);
void	PaintRoundRect(Rect *, short, short, FILE *);
void	PenMode(short, FILE *);
void	PenPat(Pattern, FILE *);
void	PenSize(short, short, FILE *);
void	RGBBackColor(RGBColor *, FILE *);
void	RGBForeColor(RGBColor *, FILE *);
void	SetPattern(Pattern, char *);
void	SetPoint(Point *, short, short);
void	SetPoly(Polygon *, short, ...);
void	SetRect(Rect *, short, short, short, short);
void	SetRGBCol(RGBColor *, short, short, short);
void	TextFace(unsigned char, FILE *);
void	TextFont(short, FILE *);
void	TextMode(short, FILE *);
void	TextScale(short, short, short, short, FILE *);
void	TextSize(short, FILE *);
void	WritePoint(Point *, FILE *);
void	WriteFixed(Fixed *, FILE *);
void	WritePoly(Polygon *, FILE *);
void	WritePat(Pattern, FILE *);
void	WriteRect(Rect *, FILE *);
void	WriteRGBCol(RGBColor *, FILE *);


/* defines */

#define	PAT_WHITE	20
#define	PAT_LT_GRAY	23
#define	PAT_GRAY	4
#define	PAT_DK_GRAY	3
#define PAT_BLACK	1

#define	PAT_COPY	0x0008
#define PAT_OR		0x0009
#define PAT_XOR		0x000A
#define PAT_BIC		0x000B
#define PAT_NOT_COPY	0x000C
#define PAT_NOT_OR	0x000D
#define PAT_NOT_XOR	0x000E
#define PAT_NOT_BIC	0x000F

#define	TXT_COPY	0x0000
#define TXT_OR		0x0001
#define TXT_XOR		0x0002
#define TXT_BIC		0x0003
#define TXT_NOT_COPY	0x0004
#define TXT_NOT_OR	0x0005
#define TXT_NOT_XOR	0x0006
#define TXT_NOT_BIC	0x0007

#define PLAIN		0x0000
#define BOLD		0x0001
#define ITALIC		0x0002
#define UNDERLINE	0x0004
#define OUTLINE		0x0008
#define SHADOW		0x0010
#define CONDENSE	0x0020
#define EXTEND		0x0040

#define CHICAGO		0
#define NEW_YORK	2
#define GENEVA		3
#define MONACO		4
#define ZAPF_DINGBATS	13
#define PALATINO	16
#define TIMES		20
#define HELVETICA	21
#define COURIER		22
#define SYMBOL		23

#define OC_BLACK	33L
#define OC_WHITE	30L
#define	OC_RED		205L
#define OC_GREEN	341L
#define OC_BLUE		409L
#define OC_CYAN		273L
#define OC_MAGENTA	137L
#define OC_YELLOW	69L


#endif
