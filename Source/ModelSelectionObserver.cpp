#include "pch.h"
#include "ModelSelectionObserver.h"
#include "Sound.h"

ModelSelectionObserver::ModelSelectionObserver(std::string _modelType, BuildManager* bm) : modelType(_modelType), m_pBuildManager(bm) {
}

void ModelSelectionObserver::OnNotify(Event* event) {
	UNREFERENCED_PARAMETER(event);

	Sound::Fire(L"ModelSelect");
	m_pBuildManager->SetCurrentModel(modelType);
}
