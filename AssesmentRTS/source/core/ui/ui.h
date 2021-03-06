#pragma once
#include "../libraries/sfwdraw.h"
#include <cstring>
#include <string>
using namespace std;
using namespace sfw;

#define PRINT cout <<
#define _ << ", " <<
#define NL << endl

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
	char _string[256];
	bool clickable;
	bool usesSprite = false; bool highlighted = false;
	unsigned tint;
	unsigned sprite;
public:
	TextAlign getAllign();
	const char* getName() const;
	float getXPos();
	float getYPos();
	float getHeight();
	float getWidth();
	bool isClickable();
	bool isButtonHighlighted();
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
	void isHighlighted(bool ih);

	Button(void)
	{
		setName("");
	}

	Button(char *name, float x, float y, float h, float w, unsigned tint = clr_WHITE, TextAlign ta = CENTER, bool c = true)
	{
		setName(name);
		setPosDim(x, y, h, w);
		setAllign(ta);
		setClickable(c);
		setTint(tint);
	}
	Button(unsigned sprite, float x, float y, float h, float w, unsigned tint = clr_WHITE, TextAlign ta = CENTER, bool c = true)
	{

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
	OPT_SCREEN = 9,
	CLR_SLIDER = 10
};
extern Menu m_CurrentMenu;
extern Button m_Dflt[3];
extern Button m_Main[5];
extern Button m_Play[4];
extern Button m_Optn[7];
extern Button m_Ctrl[5];
extern Button m_Scrn[5];
extern Button m_Clr[12];
extern Button m_End[3];
extern Button m_ChatBox[2];
extern Button m_Slider[8];


extern bool isUnitOnOverlay;
extern bool isBuildOnOverlay;
extern bool isResourceOnOverlay;

extern int overrideMouse;
extern int entOnOverlay;
extern unsigned RGBA[4];

const int gridX = 10;
const int gridY = 8;
extern float buildGrid[gridY][gridX];
extern float SCREEN[2];
extern float xSpace(float num, float den = 100);
extern float ySpace(float num, float den = 100);

extern unsigned mouseTint;
extern unsigned customColor;

extern unsigned ui_Text;
extern unsigned ui_Human;
extern unsigned ui_Invader;
extern unsigned ui_TownCenter;
extern unsigned ui_Tower;
extern unsigned ui_Barracks;
extern unsigned ui_Overlay;
extern unsigned ui_Background;
extern unsigned ui_Hotdog;
extern unsigned ui_Resource;
extern unsigned ui_Collector;
extern unsigned ui_Farm;

#define wi_Text			getTextureWidth(ui_Text) / 13
#define hi_Text			getTextureHeight(ui_Text) / 4
#define wi_Human		(getTextureWidth(ui_Human) / 2) * 2.5
#define hi_Human		getTextureHeight(ui_Human) * 2.5
#define wi_Invader		(getTextureWidth(ui_Invader) / 8) * 6
#define hi_Invader		(getTextureHeight(ui_Invader) / 4) * 6
#define wi_TownCenter	(getTextureWidth(ui_TownCenter) / 2) * 4
#define hi_TownCenter	(getTextureHeight(ui_TownCenter) / 2) * 4
#define wi_Tower		(getTextureWidth(ui_Tower) / 2) * 3
#define hi_Tower		(getTextureHeight(ui_Tower) / 2) * 3.25
#define wi_Barracks		getTextureWidth(ui_Barracks)
#define hi_Barracks		getTextureHeight(ui_Barracks) * 2
#define wi_Overlay		(SCREEN[0] / getTextureWidth(ui_Overlay)) 
#define hi_Overlay		(SCREEN[1] / getTextureHeight(ui_Overlay))
#define wi_Hotdog		(getTextureWidth(ui_Hotdog) / 2) * 3
#define hi_Hotdog		getTextureHeight(ui_Hotdog) * 3
#define wi_Food			getTextureWidth(ui_Resource) * (9 / 3)
#define hi_Food			(getTextureHeight(ui_Resource) / 2) * 3
#define wi_Steel		getTextureWidth(ui_Resource) * (8 / 3)
#define hi_Steel		(getTextureHeight(ui_Resource) / 2) * 3
#define wi_Collector	(getTextureWidth(ui_Collector) / 2) * 3
#define hi_Collector	getTextureHeight(ui_Collector) * 5 / 3  

//Returns the index for a given char to be used with the font atlas
extern int cti(char a);
//Converts (int)0-9 to (char)0-9
extern char itc(int a);
//Converts a font atlas index to a char
extern char ktc(int index, bool shift);

extern void initUI();
extern void initRGBA();
extern void initBuildGrid();
extern void drawText(const char *string, float x, float y, int multH = 1, int multW = 1, bool drawUp = false, TextAlign ta = RIGHT, unsigned tint = clr_WHITE);
extern void drawBox(float x, float y, float h, float w, unsigned tint = clr_WHITE);
extern void drawMouse(unsigned sprite = ui_Text, int index = 49, float h = hi_Text, float w = wi_Text, float multH = 3, float multW = 3);
extern void drawBuildGrid();

extern int drawMenu(Button *b, int indexLimiter = -1, bool constHighlight = false, bool makeLeftCarry = false);
extern int getSelectedColor();
extern void drawGameOverlay();
extern void drawSelBox(Button b, unsigned forceTint = clr_NONE);

extern float blerg;
