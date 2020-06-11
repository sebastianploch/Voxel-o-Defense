#include "pch.h"
#include "Subject.h"

#include "Observer.h"

void Subject::AddObserver(Observer* observer)
{
	mObservers.push_back(observer);
}

void Subject::RemoveObserver(Observer* observer)
{
	mObservers.erase(std::remove(mObservers.begin(), mObservers.end(), observer), mObservers.end());
}

void Subject::Notify(Event* event)
{
	for (Observer* observer : mObservers)
	{
		observer->OnNotify(event);
	}
}