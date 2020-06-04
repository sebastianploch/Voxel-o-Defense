#pragma once

#include "ObserverEvents.h"

class Observer
{
public:
	virtual ~Observer() {}

	virtual void OnNotify(std::shared_ptr<Event> event) = 0;
};