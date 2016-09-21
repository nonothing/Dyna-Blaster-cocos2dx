#ifndef __EEVNT_TYPE_H__
#define __EEVNT_TYPE_H__

#include "utils/Events.h"

enum  EEventType
{
	ECREATEBOMB = 0, EEXPLODE, EPAUSE, EQUIT, UPDATE_LIFE,
};

typedef EventTempl<EEventType, int>					CustomEvent;
#endif // __EEVNT_TYPE_H__
