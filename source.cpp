#include <iostream>
#include <boost/filesystem.hpp>
using namespace std;
using namespace boost::filesystem;

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
		cout << "Input file missing!\n";
		cout << "Try again: " << argv[0] << " <filename> --optionalArgument\n";
		cout << "\n";
		// Arguments Info:
		cout << "--stats\nOutputs statistical information about time stamp anomalies.\n";
		cout << "--list\nOutputs lines with erroneous time stamps along with line numbers.\n";
		cout << "--all\nOutputs lines with erroneous time stamps along with line numbers and statistical information about time stamp anomalies.\n";

		return -1;
	}

	path p(argv[1]);

	try
	{
		if (exists(p))
		{
			if (!is_regular_file(p))
			{
				cout << "Not regular file provided!\n";

				return -1;
			}

			std::ifstream file(p.c_str());

			// Number of lines in the file
			int fileLines = count(istreambuf_iterator<char>(file), istreambuf_iterator<char>(), '\n');

			// Go back to begin
			file.clear();
			file.seekg(0, ios::beg);

			// Test: Read every line
			string str;
			while (getline(file, str))
			{
				cout << str << "\n";
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
				cout << "Number of lines:\n" << fileLines;
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
	}

	return 0;
}
