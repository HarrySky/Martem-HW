#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
using namespace std;
using namespace boost::filesystem;
using namespace boost::posix_time;

#define STATS 0
#define LIST 1
#define ALL 2

#define STATS_ARG "--stats"
#define LIST_ARG "--list"
#define ALL_ARG "--all"

int checkTSForErrors(int fileLinesAmount, vector<string> linesVector, vector<int> *invalidTimestampsLines);
void showStats(int fileLinesAmount, vector<int> invalidTimestampsLines, int totalInvalidTimestamps);
void showList(int fileLinesAmount, vector<string> linesVector, vector<int> invalidTimestampsLines, int totalInvalidTimestamps);

int main(int argc, char* argv[])
{
	// If no input file argument provided - show error message
	if (argc < 2)
	{
		cout << "Input file missing!\n";
		cout << "Try again: " << argv[0] << " <filename> --optionalArgument\n";
		cout << "\n";
		// Arguments info:
		cout << "--stats\nOutputs statistical information about time stamp anomalies.\n";
		cout << "--list\nOutputs lines with erroneous time stamps along with line numbers.\n";
		cout << "--all\nOutputs lines with erroneous time stamps along with line numbers and statistical information about time stamp anomalies.\n";

		return -1;
	}

	vector<string> linesVector;
	long fileLines;

	try
	{
		path p(argv[1]);
		if (exists(p))
		{
			if (!is_regular_file(p))
			{
				cout << "Not regular file provided!\n";

				return -1;
			}

			// Open file after checking that it exists and is regular
			std::ifstream file(p.c_str());
			fileLines = count(istreambuf_iterator<char>(file), istreambuf_iterator<char>(), '\n') + 1;

			// Go back to beginning of file
			file.clear();
			file.seekg(0, ios::beg);

			// Reading every line into vector
			string str;
			while (getline(file, str)) linesVector.push_back(str);
		}
		else
		{
			cout << "File does not exist!\n";

			return -1;
		}
	}
	catch (const filesystem_error& ex)
	{
		cout << "File reading error!\n" << ex.what() << '\n';

		return -1;
	}

	// Checking additional action argument:
	int action = STATS;
	if (argc > 2 && strcmp(argv[2], STATS_ARG) != 0)
	{
		if (strcmp(argv[2], LIST_ARG) != 0 && strcmp(argv[2], ALL_ARG) != 0)
		{
			cout << "Wrong action. Default action '--stats' selected instead.";
		}
		else
		{
			action = ALL;
			if (strcmp(argv[2], LIST_ARG) == 0) action = LIST;
		}
	}

	vector<int> invalidTimestampsLines;
	int totalInvalidTimestamps = checkTSForErrors(fileLines, linesVector, &invalidTimestampsLines);

	if (action == STATS || action == ALL) showStats(fileLines, invalidTimestampsLines, totalInvalidTimestamps);
	if (action == LIST || action == ALL) showList(fileLines, linesVector, invalidTimestampsLines, totalInvalidTimestamps);

	return 0;
}

int checkTSForErrors(int fileLinesAmount, vector<string> lines, vector<int> *invalidTimestampsLines)
{
	const locale loc = locale(locale::classic(), new time_input_facet("%Y-%m-%d %H:%M:%s"));
	ptime* lastNormalTimestamp = NULL;
	ptime currentTimestamp;
	int i, totalInvalidTimestamps = 0;
	for (i = 0; i < fileLinesAmount; ++i)
	{
		// PARSING PART
		string lineDate = lines[i].substr(0, 23);
		istringstream is(lineDate);
		is.imbue(loc);
		is >> currentTimestamp;

		// ERROR CHECKING PART
		if (lastNormalTimestamp != NULL
			&& (lastNormalTimestamp->time_of_day() > currentTimestamp.time_of_day()
				|| lastNormalTimestamp->date() > currentTimestamp.date()))
		{
			invalidTimestampsLines->push_back(i);
			totalInvalidTimestamps++;
		}
		else
		{
			free(lastNormalTimestamp);
			lastNormalTimestamp = new ptime(currentTimestamp);
		}
	}

	return totalInvalidTimestamps;
}

void showStats(int fileLinesAmount, vector<int> invalidTimestampsLines, int totalInvalidTimestamps)
{
	cout << "Number of lines:\n" << fileLinesAmount << "\n";
	cout << "Invalid time stamps at line(s):\n";

	bool isRange = false;
	string output;
	int i;
	for (i = 0; i < totalInvalidTimestamps; ++i)
	{
		// Starting point ("x")
		if (!isRange) output = to_string(invalidTimestampsLines[i] + 1);

		// Check if next line number is next to this to form a range ("x-")
		if (i + 1 != totalInvalidTimestamps && !isRange
			&& invalidTimestampsLines[i + 1] + 1 == invalidTimestampsLines[i] + 2)
		{
			isRange = true;
			output += "-";
		}

		if (isRange)
		{
			// Check if there is no more line numbers in this range to close it ("x-y")
			if (i + 1 == totalInvalidTimestamps || invalidTimestampsLines[i + 1] + 1 != invalidTimestampsLines[i] + 2)
			{
				output += to_string(invalidTimestampsLines[i] + 1);
				isRange = false;
			}
			else { continue; }
		}

		cout << output << "\n";
	}

	cout << "\n";
}

void showList(int fileLinesAmount, vector<string> lines, vector<int> invalidTimestampsLines, int totalInvalidTimestamps)
{
	int i, currentInvalidTimestampLine;
	for (i = 0; i < totalInvalidTimestamps; ++i)
	{
		currentInvalidTimestampLine = invalidTimestampsLines[i];

		// Normal entry before the invalid timestamp
		if (currentInvalidTimestampLine > 0
			&& (i == 0 || invalidTimestampsLines[i - 1] != currentInvalidTimestampLine - 1))
		{
			cout << "[" << currentInvalidTimestampLine << "] " + lines[currentInvalidTimestampLine - 1] << "\n";
		}

		// Invalid timestamp entry
		cout << "[" << currentInvalidTimestampLine + 1 << "] " + lines[currentInvalidTimestampLine] << "\n";

		// Normal entry after invalid timestamp
		if (currentInvalidTimestampLine + 1 < fileLinesAmount
			&& (i + 1 == totalInvalidTimestamps || invalidTimestampsLines[i + 1] != currentInvalidTimestampLine + 1))
		{
			cout << "[" << currentInvalidTimestampLine + 2 << "] " + lines[currentInvalidTimestampLine + 1] << "\n";
			cout << "---" << "\n";
		}
	}
}