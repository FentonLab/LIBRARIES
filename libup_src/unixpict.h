
#ifndef UNIX_PICT_IN
#define UNIX_PICT_IN

/* to get the sparc to handle variable argument functions */
#ifndef __BUILTIN_VA_ARG_INC
#define __BUILTIN_VA_ARG_INC
#endif

/* machine architecture: BIG_ENDIAN or LITTLE_ENDIAN */

#define BIG_ENDIAN_MACHINE


/* includes */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
/*	#include <varargs.h>	*/
#include "size_types.h"


/* typedefs */

typedef	struct {
		S_I_2	top;
		S_I_2	left;
		S_I_2	bottom;
		S_I_2	right;
	} RECT;

typedef struct {
		S_I_2	integer;
		U_I_2	decimal;
	} FIXED;

typedef struct {
		S_I_2	v;
		S_I_2	h;
	} POINT;

typedef U_I_1 	PATTERN[8];

typedef struct {
		S_I_2	n_points;
		POINT	*points;
	} POLYGON;

typedef struct {
		U_I_2	red, grn, blu;
	} RGBCOLOR;
		

#if defined(__STDC__)  /* function prototypes -> ANSI */

void	BackColor(S_I_4, FILE *);
void	BackPat(PATTERN, FILE *);
void	DrawChar(S_I_1,  FILE *);
void	DrawString(S_I_1 *,  FILE *);
void	CloseGroup(FILE *);
void	ClosePicture(FILE *);
void	EraseArc(RECT *, S_I_2, S_I_2, FILE *);
void	EraseOval(RECT *, FILE *);
void	ErasePoly(POLYGON *, FILE *);
void	EraseRect(RECT *, FILE *);
void	EraseRoundRect(RECT *, S_I_2, S_I_2, FILE *);
void	FillArc(RECT *, S_I_2, S_I_2, FILE *);
void	FillOval(RECT *, FILE *);
void	FillPoly(POLYGON *, FILE *);
void	FillRect(RECT *, FILE *);
void	FillRoundRect(RECT *, S_I_2, S_I_2, FILE *);
void	FloatToFixed(float, FIXED *);
void	ForeColor(S_I_4, FILE *);
void	FrameArc(RECT *, S_I_2, S_I_2, FILE *);
void	FrameOval(RECT *, FILE *);
void	FramePoly(POLYGON *, FILE *);
void	FrameRect(RECT *, FILE *);
void	FrameRoundRect(RECT *, S_I_2, S_I_2, FILE *);
void	GetIndPattern(S_I_2, PATTERN);
void	InvertArc(RECT *, S_I_2, S_I_2, FILE *);
void	InvertOval(RECT *, FILE *);
void	InvertPoly(POLYGON *, FILE *);
void	InvertRect(RECT *, FILE *);
void	InvertRoundRect(RECT *, S_I_2, S_I_2, FILE *);
void	InsetPoly(POLYGON *, S_I_2, S_I_2);
void	InsetRect(RECT *, S_I_2, S_I_2);
void	Line(S_I_2, S_I_2, FILE *);
void	LineTo(S_I_2, S_I_2, FILE *);
void	Move(S_I_2, S_I_2);
void	PenNormal(FILE *);
void	MoveTo(S_I_2, S_I_2);
void	OffsetPoly(POLYGON *, S_I_2, S_I_2);
void	OffsetRect(RECT *, S_I_2, S_I_2);
FILE	*OpenCPicture(S_I_1 *, float, float, S_I_2, S_I_2);
void	OpenGroup(FILE *);
FILE	*OpenPicture(S_I_1 *, S_I_2, S_I_2);
void	PaintArc(RECT *, S_I_2, S_I_2, FILE *);
void	PaintOval(RECT *, FILE *);
void	PaintPoly(POLYGON *, FILE *);
void	PaintRect(RECT *, FILE *);
void	PaintRoundRect(RECT *, S_I_2, S_I_2, FILE *);
void	PenMode(S_I_2, FILE *);
void	PenPat(PATTERN, FILE *);
void	PenSize(S_I_2, S_I_2, FILE *);
void	RGBBackColor(RGBCOLOR *, FILE *);
void	RGBForeColor(RGBCOLOR *, FILE *);
void	SetPattern(PATTERN, S_I_1 *);
void	SetPoint(POINT *, S_I_2, S_I_2);
void	SetPoly(POLYGON *, S_I_2, ...);
void	SetRect(RECT *, S_I_2, S_I_2, S_I_2, S_I_2);
void	SetRGBCol(RGBCOLOR *, S_I_2, S_I_2, S_I_2);
void	TextFace(U_I_1, FILE *);
void	TextFont(S_I_2, FILE *);
void	TextMode(S_I_2, FILE *);
void	TextScale(S_I_2, S_I_2, S_I_2, S_I_2, FILE *);
void	TextSize(S_I_2, FILE *);
void    UPWrite(void *, size_t, size_t, FILE *);
void	WritePoint(POINT *, FILE *);
void	WriteFixed(FIXED *, FILE *);
void	WritePoly(POLYGON *, FILE *);
void	WritePat(PATTERN, FILE *);
void	WriteRect(RECT *, FILE *);
void	WriteRGBCol(RGBCOLOR *, FILE *);

#else /* function declarations -> non-ANSI */

void	BackColor();
void	BackPat();
void	DrawChar();
void	DrawString();
void	CloseGroup();
void	ClosePicture();
void	EraseArc();
void	EraseOval();
void	ErasePoly();
void	EraseRect();
void	EraseRoundRect();
void	FillArc();
void	FillOval();
void	FillPoly();
void	FillRect();
void	FillRoundRect();
void	FloatToFixed();
void	ForeColor();
void	FrameArc();
void	FrameOval();
void	FramePoly();
void	FrameRect();
void	FrameRoundRect();
void	GetIndPattern();
void	InvertArc();
void	InvertOval();
void	InvertPoly();
void	InvertRect();
void	InvertRoundRect();
void	InsetPoly();
void	InsetRect();
void	Line();
void	LineTo();
void	Move();
void	PenNormal();
void	MoveTo();
void	OffsetPoly();
void	OffsetRect();
FILE	*OpenCPicture();
void	OpenGroup();
FILE	*OpenPicture();
void	PaintArc();
void	PaintOval();
void	PaintPoly();
void	PaintRect();
void	PaintRoundRect();
void	PenMode();
void	PenPat();
void	PenSize();
void	RGBBackColor();
void	RGBForeColor();
void	SetPattern();
void	SetPoint();
void	SetPoly();
void	SetRect();
void	SetRGBCol();
void	TextFace();
void	TextFont();
void	TextMode();
void	TextScale();
void	TextSize();
void    UPWrite();
void	WritePoint();
void	WriteFixed();
void	WritePoly();
void	WritePat();
void	WriteRect();
void	WriteRGBCol();

#endif


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

