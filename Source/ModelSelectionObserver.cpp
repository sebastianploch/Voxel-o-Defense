#include "pch.h"
#include "ModelSelectionObserver.h"

ModelSelectionObserver::ModelSelectionObserver(std::string _modelType, BuildManager* bm) : modelType(_modelType), m_pBuildManager(bm) {
}

void ModelSelectionObserver::OnNotify(std::shared_ptr<Event> event) {
	DEBUG_PRINT(modelType);
	m_pBuildManager->SetCurrentModelDir(modelType);
}
