#include <iostream>
using namespace std;

#define STATS 0
#define LIST 1
#define ALL 2

#define STATS_ARG "--stats"
#define LIST_ARG "--list"
#define ALL_ARG "--all"

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		cout << "Input file missing!\n";
		cout << "Try again: " << argv[0] << " <filename> --optionalArgument\n";
		// Arguments Info:
		cout << "--stats\nOutputs statistical information about time stamp anomalies.\n";
		cout << "--list\nOutputs lines with erroneous time stamps along with line numbers.\n";
		cout << "--all\nOutputs lines with erroneous time stamps along with line numbers and statistical information about time stamp anomalies.\n";

		return -1;
	}
	
	char* filename = argv[1];
	int action = STATS;
	if (argc > 2 && argv[2] != STATS)
	{
		if (argv[2] != LIST_ARG || argv[2] != ALL_ARG)
		{
			cout << "Default action '--stats' selected.";
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

	return 0;
}
