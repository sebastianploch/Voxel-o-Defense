#pragma once

class Observer
{
public:
	virtual ~Observer();

	virtual void OnNotify(EVENT event) = 0;
};