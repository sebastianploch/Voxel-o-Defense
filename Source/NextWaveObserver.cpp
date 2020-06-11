#include "pch.h"
#include "NextWaveObserver.h"
#include "Sound.h"
#include "AiManager.h"
#include <future>

NextWaveObserver::NextWaveObserver(ISOCamera* camera) : cam(camera) {
}

void NextWaveObserver::OnNotify(Event* event) {
	UNREFERENCED_PARAMETER(event);

	Sound::Fire(L"WaveStart");
	cam->SetIsBuildMode(false);
}
