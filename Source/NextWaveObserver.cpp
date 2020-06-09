#include "pch.h"
#include "NextWaveObserver.h"

NextWaveObserver::NextWaveObserver(ISOCamera* camera) : cam(camera) {
}

void NextWaveObserver::OnNotify(std::shared_ptr<Event> event) {
	cam->SetIsBuildMode(false);
}
