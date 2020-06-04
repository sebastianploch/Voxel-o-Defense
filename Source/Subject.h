#pragma once

class Observer;

class Subject
{
public:
	void AddObserver(std::shared_ptr<Observer> observer);
	void RemoveObserver(std::shared_ptr<Observer> observer);

protected:
	void Notify(EVENT event);

private:
	std::vector< std::shared_ptr<Observer>> mObservers;
};