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
		cout << "Input file missing!";
		cout << "Try again: " << argv[0] << " <filename>\n";
		return -1;
	}
	
	char* filename = argv[1];
	int action = STATS;
	if (argc > 2 && argv[2] != STATS_ARG)
	{

	}

	return 0;
}