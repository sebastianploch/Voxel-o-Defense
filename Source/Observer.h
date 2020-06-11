#pragma once

#include "ObserverEvents.h"
#include "Subject.h"

class Observer
{
public:
	virtual ~Observer()
	{
		for (Subject* subject : m_subjects)
		{
			subject->RemoveObserver(this);
		}
	}

	virtual void OnNotify(Event* event) = 0;

private:
	std::vector<Subject*> m_subjects;

};