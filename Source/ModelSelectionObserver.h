#pragma once
#include "Observer.h"
#include "ObserverEvents.h"
#include "BuildManager.h"
#include <string>

class ModelSelectionObserver : public Observer {
private:
	std::string modelType;
	BuildManager* m_pBuildManager;

public:
	ModelSelectionObserver(std::string _modelType, BuildManager* bm);

	void OnNotify(std::shared_ptr<Event> event) override;
};