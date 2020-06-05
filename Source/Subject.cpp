#include "pch.h"
#include "Subject.h"

#include "Observer.h"

void Subject::AddObserver(std::shared_ptr<Observer> observer)
{
	mObservers.push_back(observer);
}

void Subject::RemoveObserver(std::shared_ptr<Observer> observer)
{
	mObservers.erase(std::remove(mObservers.begin(), mObservers.end(), observer), mObservers.end());
}

void Subject::Notify(std::shared_ptr<Event> event)
{
	for (std::shared_ptr<Observer> observer : mObservers)
	{
		observer->OnNotify(event);
	}
}