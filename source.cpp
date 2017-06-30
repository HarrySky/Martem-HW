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

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		// If no input file argument provided - error
		cout << "Input file missing!\n";
		cout << "Try again: " << argv[0] << " <filename> --optionalArgument\n";
		cout << "\n";
		// Arguments info:
		cout << "--stats\nOutputs statistical information about time stamp anomalies.\n";
		cout << "--list\nOutputs lines with erroneous time stamps along with line numbers.\n";
		cout << "--all\nOutputs lines with erroneous time stamps along with line numbers and statistical information about time stamp anomalies.\n";

		return -1;
	}

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
			long fileLines = count(istreambuf_iterator<char>(file), istreambuf_iterator<char>(), '\n') + 1;

			// Go back to beginning of file
			file.clear();
			file.seekg(0, ios::beg);

			// Reading every line into vector
			string str;
			vector<string> lines;
			while (getline(file, str))
			{
				lines.push_back(str);
			}

			// Parsing date-time and checking for errors
			const locale loc = locale(locale::classic(), new time_input_facet("%Y-%m-%d %H:%M:%s"));

			ptime* lastNormalTimestamp = NULL;
			int lastNormalTimestampLine = 1;

			vector<int> invalidTimestampsLines;

			for (int i = 0; i < fileLines; ++i)
			{
				// PARSING PART
				string lineDate = lines[i].substr(0, 23);
				istringstream is(lineDate);
				is.imbue(loc);
				ptime currentTimestamp;
				is >> currentTimestamp;

				// ERROR CHECKING PART
				if (lastNormalTimestamp != NULL
					&& (lastNormalTimestamp->time_of_day() > currentTimestamp.time_of_day()
						|| lastNormalTimestamp->date() > currentTimestamp.date()))
				{
					invalidTimestampsLines.push_back(i + 1);
					cout << "Line invalid : " << (i + 1) << "\n";
				}
				else
				{
					free(lastNormalTimestamp);
					lastNormalTimestamp = new ptime(currentTimestamp);
				}
			}

			// Checking additional action argument:
			int action = STATS;
			if (argc > 2 && argv[2] != STATS)
			{
				if (argv[2] != LIST_ARG && argv[2] != ALL_ARG)
				{
					cout << "Wrong action. Default action '--stats' selected instead.";
				}
				else
				{
					action = ALL;
					if (argv[2] == LIST_ARG)
					{
						action = LIST;
					}

				}
			}

			if (action == STATS || action == ALL)
			{
				cout << "Number of lines:\n" << fileLines << "\n";
				cout << "Invalid time stamps at line(s):\n";

				// Showing line numbers here

				cout << "\n";
			}

			if (action == LIST || action == ALL)
			{
				// Showing lines here
			}
		}
		else
		{
			cout << "File does not exist!\n";

			return -1;
		}
	}
	catch (const filesystem_error& ex)
	{
		cout << "Error occured!\n" << ex.what() << '\n';

		return -1;
	}


	return 0;
}
