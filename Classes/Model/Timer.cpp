#include "Model/Timer.h"

USING_NS_CC;
using namespace ui;

dyna::Timer* dyna::Timer::create(Text* text)
{
	dyna::Timer* timer = new dyna::Timer();
	if (timer && timer->init(text))
	{
		return (dyna::Timer*)timer->autorelease();
	}

	CC_SAFE_DELETE(timer);

	return timer;
}

bool dyna::Timer::init(Text* text)
{
	if (!Node::init())
		return false;
	_text = text;
	_defaultTicks = 180;
	_ticks = 0;
	schedule(schedule_selector(Timer::update), 0.5f);
	return true;
}

void dyna::Timer::update(float dt)
{
	_point = !_point;
	if (_point)
	{
		if (_ticks > 0)
		{
			_ticks--;
		}
	}

	_text->setString(intToTimeStr(_ticks, _point));
}

void dyna::Timer::setTime(int ticks)
{
	_defaultTicks = _ticks = ticks;
	_text->setString(intToTimeStr(_ticks, _point));
}

void dyna::Timer::start()
{
	schedule(schedule_selector(Timer::update), 0.5f);
}

void dyna::Timer::reset()
{
	_ticks = _defaultTicks;
	_text->setString(intToTimeStr(_ticks, _point));
}

void dyna::Timer::pause()
{
	unschedule(schedule_selector(Timer::update));
}

std::string dyna::Timer::intToTimeStr(int ticks, bool isPoint)
{
	std::string result;

	int min = ticks / 60;
	int sec = ticks - min * 60;
	result += getDigitalStr(min);
    result += isPoint ? ":" : " ";
	result += getDigitalStr(sec);

	return result;
}

std::string dyna::Timer::getDigitalStr(int sec)
{
	return sec < 10 ? "0" + std::to_string(sec) : std::to_string(sec);
}

bool dyna::Timer::isGameOver()
{
	return _ticks == 0;
}
