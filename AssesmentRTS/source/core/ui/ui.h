#pragma once
#include "../libraries/sfwdraw.h"
#include <cstring>
using namespace sfw;

enum TextAlign
{
	CENTER = 0,
	RIGHT = 1,
	LEFT = 2
};

class Button
{
private:
	TextAlign ta;
	float xPos, yPos, height, width;
	char *string;
	bool clickable;
	unsigned tint;
public:
	TextAlign getAllign();
	char* getName();
	float getXPos();
	float getYPos();
	float getHeight();
	float getWidth();
	bool isClickable();
	unsigned getTint();

	void setName(char *n);
	void setXPos(float x);
	void setYPos(float y);
	void setHeight(float h);
	void setWidth(float w);
	void setPos(float x, float y);
	void setDim(float h, float w);
	void setPosDim(float x, float y, float h, float w);
	void setAllign(TextAlign ta);
	void setClickable(bool c);
	void setTint(unsigned color);

	Button(void)
	{
		setName('\0');
	}

	Button(char *name, float x, float y, float h, float w, unsigned tint = WHITE, TextAlign ta = CENTER, bool c = true)
	{
		setName(name);
		setPosDim(x, y, h, w);
		setAllign(ta);
		setClickable(c);
		setTint(tint);
	}
};

enum Menu
{
	DEFAULT = -1,
	MAIN = 0,
	PLAY = 1,
	OPTION = 2,
	OPT_PROFILE = 3,
	GAME = 4,
	OPT_CONTROLS = 5,
	OPT_CTRL_MOVE = 6,
	OPT_CTRL_HOTKEY = 7,
	OPT_COLOR = 8,
	OPT_SCREEN = 9
};
extern Menu m_CurrentMenu;
extern Button m_Dflt[3];
extern Button m_Main[5];
extern Button m_Play[4];
extern Button m_Optn[7];
extern Button m_Ctrl[5];
extern Button m_Scrn[5];
extern Button m_Clr[11];


extern bool isUnitOnOverlay;
extern bool isBuildOnOverlay;

extern int entOnOverlay;

extern float SCREEN[2];
extern float xSpace(float num, float den = 100);
extern float ySpace(float num, float den = 100);

extern unsigned ui_Text;
extern unsigned ui_Human;
extern unsigned ui_Invader;
extern unsigned ui_TownCenter;
extern unsigned ui_Tower;
extern unsigned ui_Barracks;
extern unsigned ui_Overlay;

#define wi_Text			getTextureWidth(ui_Text) / 13
#define hi_Text			getTextureHeight(ui_Text) / 4
#define wi_Human		getTextureWidth(ui_Human) * 2
#define hi_Human		getTextureHeight(ui_Human) * 2
#define wi_Invader	   (getTextureWidth(ui_Invader) / 4) * 6
#define hi_Invader	   (getTextureHeight(ui_Invader) / 4) * 6
#define wi_TownCenter	getTextureWidth(ui_TownCenter) * 4
#define hi_TownCenter	(getTextureHeight(ui_TownCenter) / 2) * 4
#define wi_Tower		(getTextureWidth(ui_Tower) / 2) * 3
#define hi_Tower		getTextureHeight(ui_Tower) * 3
#define wi_Barracks		getTextureWidth(ui_Barracks) * 2
#define hi_Barracks		getTextureHeight(ui_Barracks) * 2
#define wi_Overlay		(SCREEN[0] / getTextureWidth(ui_Overlay)) 
#define hi_Overlay		(SCREEN[1] / getTextureHeight(ui_Overlay))

extern int cti(char a);
extern char itc(int a);

extern void initUI();
extern void drawText(const char *string, float x, float y, int multH = 1, int multW = 1, bool drawUp = false, TextAlign ta = RIGHT, unsigned tint = WHITE);
extern void drawBox(float x, float y, float h, float w, unsigned tint);
extern void drawMouse();

extern int drawMenu(Button *b, int indexLimiter = -1);
extern int getSelectedColor();
extern void drawGameOverlay();
extern void drawSelBox(Button b, unsigned forceTint = NONE);

extern float blerg;
