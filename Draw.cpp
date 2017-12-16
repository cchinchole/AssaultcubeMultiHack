#include "stdafx.h"
#include <stdio.h>
#include <gl/GLU.h>
#include "Draw.h"
#include <glut.h>




void Draw::StringSizable(GLfloat x, GLfloat y, GLfloat r, GLfloat g, GLfloat b, float size, char const * const pFmtText, ...)
{
	
	int newLineDist = size-1;
	//Allocate memory for a temporary result string. You could probably do much less
	char *const pTemp = new char[5000];

	//Put all of the text into pTemp
	va_list vaList;
	va_start(vaList, pFmtText);
	int const TextLen = vsprintf(pTemp, pFmtText, vaList);
	va_end(vaList);

	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(1 / 152.38, 1 / 152.38, 1 / 152.38);

	//Draw the string
	for (unsigned int Counter = 0; Counter < TextLen; ++Counter)
	{
		if (pTemp[Counter] == '\n')
		{
			y += newLineDist;
			glRasterPos2i(x, y);
			continue;
		}
		glScalef(0.005, 0.005, 1);
		glutStrokeCharacter(GLUT_STROKE_ROMAN, pTemp[Counter]);
	
	}
	glPopMatrix();

	//Prevent memory leaks
	delete[] pTemp;
}

void Draw::stringD(GLfloat x, GLfloat y, GLfloat r, GLfloat g, GLfloat b, void * font, char const * const pFmtText, ...)
{
int newLineDist;

//Set the distance dynamically for new lines
switch ((int)font)
{
case (int)GLUT_BITMAP_9_BY_15:
newLineDist = 16;
break;

case (int)GLUT_BITMAP_8_BY_13:
newLineDist = 14;
break;

case (int)GLUT_BITMAP_TIMES_ROMAN_10:
newLineDist = 11;
break;

case (int)GLUT_BITMAP_TIMES_ROMAN_24:
newLineDist = 25;
break;

case (int)GLUT_BITMAP_HELVETICA_10:
newLineDist = 11;
break;

case (int)GLUT_BITMAP_HELVETICA_12:
newLineDist = 13;
break;

case (int)GLUT_BITMAP_HELVETICA_18:
newLineDist = 19;
break;

default:
newLineDist = 19;
break;
}

//Set our attributes
glColor3ub(r, g, b);
glRasterPos2i(x, y);

//Allocate memory for a temporary result string. You could probably do much less
char *const pTemp = new char[5000];

//Put all of the text into pTemp
va_list vaList;
va_start(vaList, pFmtText);
int const TextLen = vsprintf(pTemp, pFmtText, vaList);
va_end(vaList);

//Draw the string
for (unsigned int Counter = 0; Counter < TextLen; ++Counter)
{
if (pTemp[Counter] == '\n')
{
y += newLineDist;
glRasterPos2i(x, y);
continue;
}

glutBitmapCharacter(font, pTemp[Counter]);
}

//Prevent memory leaks
delete[] pTemp;
}


void Draw::box(GLfloat x, GLfloat y, GLfloat xLen, GLfloat yLen, GLfloat r, GLfloat g, GLfloat b)
{
	//Set our attributes
	glColor3ub(r, g, b);

	//Draw our box
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x + xLen, y);
	glVertex2f(x + xLen, y + yLen);
	glVertex2f(x, y + yLen);
	glEnd();
}

void Draw::box(GLfloat x, GLfloat y, GLfloat xLen, GLfloat yLen, GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	//Set our attributes
	glColor4ub(r, g, b, a);
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//Draw our box
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x + xLen, y);
	glVertex2f(x + xLen, y + yLen);
	glVertex2f(x, y + yLen);
	glEnd();
	glDisable(GL_BLEND);
}

void Draw::boxLine(GLfloat width, GLfloat x, GLfloat y, GLfloat xLen, GLfloat yLen, GLfloat r, GLfloat g, GLfloat b)
{
	//Grab the current line width to prevent clashes
	GLfloat glTemp[1];
	glGetFloatv(GL_LINE_WIDTH, glTemp);

	//Set our attributes
	glColor3ub(r, g, b);
	glLineWidth(width);

	//Draw out box
	glBegin(GL_LINE_LOOP);
	glVertex2f(x, y);
	glVertex2f(x + xLen, y);
	glVertex2f(x + xLen, y + yLen);
	glVertex2f(x, y + yLen);
	glEnd();

	//Restore the line width
	glLineWidth(glTemp[0]);
}

void Draw::line(GLfloat width, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat r, GLfloat g, GLfloat b)
{
	//Grab the current line width to prevent clashes
	GLfloat glTemp[1];
	glGetFloatv(GL_LINE_WIDTH, glTemp);

	//Set our attributes
	glColor3ub(r, g, b);
	glLineWidth(width);

	//Draw our line
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();

	//Restore the line width
	glLineWidth(glTemp[0]);
}

void Draw::dot(GLfloat size, GLfloat x, GLfloat y, GLfloat r, GLfloat g, GLfloat b)
{
	//Grab the current line width to prevent clashes
	GLfloat glTemp[1];
	glGetFloatv(GL_POINT_SIZE, glTemp);

	//Set our attributes
	glColor3ub(r, g, b);
	glPointSize(size);

	//Draw our point
	glBegin(GL_POINTS);
	glVertex2f(x, y);
	glEnd();

	//Restore to prevent clashing
	glPointSize(glTemp[0]);

}

void Draw::BuildFonts()
{
	fontHDC = wglGetCurrentDC();
	hackFont = glGenLists(96);
	HFONT hFont = CreateFontA(-15, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_DONTCARE | DEFAULT_PITCH, "Consolas");
	HFONT hOldFont = (HFONT)SelectObject(fontHDC, hFont);
	wglUseFontBitmaps(fontHDC, 32, 96, hackFont);
	SelectObject(fontHDC, hOldFont);
	DeleteObject(hFont);

	bFontBuilt = true;
}

void Draw::Print(float x, float y, const unsigned char color[3], const char *format, ...)
{

	glColor3ub(color[0], color[1], color[2]);
	glRasterPos2f(x, y);

	char text[100] = "test string";
	va_list args;

	va_start(args, format);
	vsprintf_s(text, 100, format, args);
	va_end(args);

	glPushAttrib(GL_LIST_BIT);
	glListBase(hackFont - 32);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();
}

float Draw::centerText(float x, float width, float textWidth)
{
	if (width > textWidth)
	{
		float difference = width - textWidth;
		return (x + (difference / 2));
	}

	else
	{
		float difference = textWidth - width;
		return (x - (difference / 2));
	}
}