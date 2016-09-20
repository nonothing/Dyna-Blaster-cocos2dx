#ifndef __ID_BONUS_H__
#define __ID_BONUS_H__

enum  ID_BONUS
{
	BFire = 0,
	BBomb,
	BSpeed,
	BHeart,
	BLife,
	BWall,
	BEBomb, 
	BImmortal,
	BRSLOW,
	BRFAST,
	BRSPAWN,
	BRNOSPAWN,
	BNone

};

const static std::string sBonusName[] = { 
	"bonus_fire", "bonus_bomb", "bonus_speed", "bonus_heart",
	"bonus_life", "bonus_wall", "bonus_ebomb", "bonus_immortal",
	"bonus_random", "bonus_random", "bonus_random", "bonus_random" };
const static std::string sBonus[] = { "F", "B", "S", "H", "L", "W", "E", "I" };

#endif // __ID_BONUS_H__
