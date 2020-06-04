#pragma once

class Event;
class Observer;

class Subject
{
public:
	void AddObserver(std::shared_ptr<Observer> observer);
	void RemoveObserver(std::shared_ptr<Observer> observer);

	void Notify(std::shared_ptr<Event> event);

private:
	std::vector< std::shared_ptr<Observer>> mObservers;
};