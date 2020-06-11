#pragma once

enum class EVENTTYPE
{
	CLICK,
	VALCHANGE
};

struct Event
{
	Event(EVENTTYPE type) :
		m_type(type)
	{
	}

	EVENTTYPE m_type;
};

struct ClickEvent :
	public Event
{
	ClickEvent() :
	Event(EVENTTYPE::CLICK)
	{
	}
};

struct ValChangeEvent :
	public Event
{
	ValChangeEvent(float newVal) :
		Event(EVENTTYPE::VALCHANGE),
		m_newVal(newVal)
	{
	}

	float m_newVal;
};