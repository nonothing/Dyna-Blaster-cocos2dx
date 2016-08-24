#ifndef __TIMER_H__
#define __TIMER_H__

#include "cocos2d.h"
#include "ui/UIText.h"

namespace dyna
{
	class Timer : public cocos2d::Node
	{
	private:
		cocos2d::ui::Text*	_text;
		int					_ticks;
		int					_defaultTicks;
		bool				_point;
		bool				_isCreated;

		std::string  intToTimeStr(int ticks, bool isPoint);
		std::string  getDigitalStr(int sec);

	public:
		virtual bool init(cocos2d::ui::Text* text);
		static Timer* create(cocos2d::ui::Text* text);
		void update(float dt);
		void setTime(int ticks);
		void start();
		void reset();
		void pause();
		bool canCreateNPC();
	};
};


#endif // __TIMER_H__
