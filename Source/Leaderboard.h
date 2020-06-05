#include <string>
#include <vector>

class Leaderboard
{
public:
	Leaderboard();

	void setMaxEntryNum(unsigned int maxEntryNum);
	unsigned int getMaxEntryNum();

	bool addEntry(std::pair<std::string, float> entry);
	void saveEntries(const std::string& filePath);
	void loadEntries(const std::string& filePath);

	// TODO: Leaderboard UI
private:
	// Leaderboard entries - automatically sorted by insertion - will always be in order
	// from highest at index 0
	std::vector<std::pair<std::string, float>> m_entries;

	// The number of entries to store - 10 by default
	unsigned int m_maxEntryNum;
};