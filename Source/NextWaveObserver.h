#pragma once
#include "Observer.h"
#include "ObserverEvents.h"
#include "ISOCamera.h"
#include <string>
#include "AiManager.h"

class NextWaveObserver : public Observer {
private:
	ISOCamera* cam;

public:
	NextWaveObserver(ISOCamera* camera);
	void OnNotify(Event* event) override;
};