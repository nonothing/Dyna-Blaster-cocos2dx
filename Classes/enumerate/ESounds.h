#ifndef __ESOUNDS_H__
#define __ESOUNDS_H__

#include "cocos2d.h"

enum  ESounds
{
	ES_BATTLE = 0, 	ES_BOMB,	ES_BOMBER_CHAMP,	ES_BONUS,	ES_BOSS,
	ES_DEAD,	ES_DRAW,	ES_ENDING,	ES_GAME_OVER,	ES_INTRO,
	ES_ROUND,	ES_CLEAR,	ES_MUSIC_1,	ES_MUSIC_2,	ES_STAGE,
	ES_TITLE,	ES_WIN_MATCH,
};

const static std::string sSoundsName[] = { "Battle", "bomb", "Bomber_Champ", "bonus", "Boss", "Dead", "Draw_Game", "Ending", "Game_Over",
"Introduction", "Round_Start", "Stage_Clear", "Stage_Music_1", "Stage_Music_2", "Stage_Start", "Title", "Win_Match",  };

#endif // __ESOUNDS_H__
