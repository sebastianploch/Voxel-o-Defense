#pragma once

struct Event;
class Observer;

class Subject
{
public:
	void AddObserver(Observer* observer);
	void RemoveObserver(Observer* observer);

	void Notify(Event* event);

private:
	std::vector<Observer*> mObservers;
};