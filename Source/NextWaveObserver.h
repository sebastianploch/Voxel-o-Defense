#pragma once
#include "Observer.h"
#include "ObserverEvents.h"
#include "ISOCamera.h"
#include <string>

class NextWaveObserver : public Observer {
private:
	ISOCamera* cam;

public:
	NextWaveObserver(ISOCamera* camera);

	void OnNotify(std::shared_ptr<Event> event) override;
};