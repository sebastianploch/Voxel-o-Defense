#include "pch.h"

#include "Leaderboard.h"

Leaderboard::Leaderboard() :
	m_maxEntryNum(10)
{
}

void Leaderboard::setMaxEntryNum(unsigned int maxEntryNum)
{
	// Set the number of max entries
	m_maxEntryNum = maxEntryNum;

	// Ensure that the number of entries does not exceed the new number of max entries
	while (m_entries.size() > m_maxEntryNum)
	{
		m_entries.pop_back();
	}
}

unsigned int Leaderboard::getMaxEntryNum()
{
	return m_maxEntryNum;
}

bool Leaderboard::addEntry(std::pair<std::string, float> entry)
{
	// Find the address of the first entry that has a score that is less than the score of the entry being inserted
	std::vector<std::pair<std::string, float>>::iterator iter = m_entries.begin();
	for (std::pair<std::string, float> e : m_entries)
	{
		if (e.second < entry.second)
		{
			break;
		}
		iter++;
	}
	// Insert the new entry
	m_entries.insert(iter, entry);

	// If the number of entries exceeds the maximum, remove the lowest scores
	while (m_entries.size() > m_maxEntryNum)
	{
		m_entries.pop_back();
	}
}

void Leaderboard::saveEntries(const std::string& filePath)
{
	ScoresCfg scfg;
	scfg.entries = m_entries;
	scfg.entryNum = m_entries.size();

	JSONLoader::CreateConfig(scfg, filePath, "scores");
}

void Leaderboard::loadEntries(const std::string& filePath)
{
	ScoresCfg scfg;
	JSONLoader::LoadFile(filePath, scfg, "scores");

	m_entries = scfg.entries;
	while (m_entries.size() > m_maxEntryNum)
	{
		m_entries.pop_back();
	}
}