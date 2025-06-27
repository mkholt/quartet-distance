#include <iostream>
#include <vector>
#include <ctime>
#include <string.h>
#include <algorithm>
#include "int_stuff.h"
#include "rooted_tree.h"
#include "unrooted_tree.h"
#include "newick_parser.h"
#include "triplet_calc.h"
#include "quartet_calc.h"
#include "soda13_calc.h"
#include "hdt_factory.h"
#include "hdt.h"

#ifndef _MSC_VER
	#define _stricmp strcasecmp
#endif

using namespace std;

// Global vars :x
RootedTree *rt1 = NULL, *rt2 = NULL;
UnrootedTree *ut1 = NULL, *ut2 = NULL;
NewickParser *parser = NULL;
TripletCalc *tripletCalc;
QuartetCalc *quartetCalc;

void usage(char *programName)
{
	cerr << endl;
	cerr << "Usage of the program is as follows:" << endl;
	cerr << programName << " (calcTripDist | ";
#ifdef quartetsToo
	cerr << "calcQuartDist | ";
#endif
	cerr << "printTree | printHDT) [filename1 [filename2]]" << endl;
	cerr << endl;

	cerr << "calcTripDist" << "\t" << "Calculates the triplet distance between the two input files." << endl
			<< "\t\t" << "Thus requires two filenames!" << endl;
#ifdef quartetsToo
	cerr << "\t\tNOTE: This is a quartet-compile, so this operation will be slower than need-be!" << endl;
#endif
	cerr << endl;

#ifdef quartetsToo
	cerr << "calcQuartDist" << "\t" << "Calculates the quartet distance between the two input files." << endl
			<< "\t\t" << "Thus requires two filenames!" << endl << endl;
#endif

	cerr << "printTree" << "\t" << "Prints the parsed tree (last param!) as dot format." << endl << endl;

	cerr << "printRootedTree" << "\t" << "Roots the parsed tree (last param!) and prints it as dot format." << endl << endl;

	cerr << "printHDT" << "\t" << "Build an HDT of the tree (last param!) and print it as dot format." << endl << endl;

	cerr << endl;
	cerr << "Filenames are expected to point to files in the Newick fileformat." << endl;
	cerr << endl;
}

/*
	Print debug stuff if we're in debug mode
	Free stuff (if allocated)
	Etc.
*/
unsigned int programStart;
void program_exiting()
{
	if (rt1 != NULL) delete rt1->factory;
	if (rt2 != NULL) delete rt2->factory;
	if (ut1 != NULL) delete ut1;
	if (ut2 != NULL) delete ut2;
	if (parser != NULL) delete parser;
	if (tripletCalc != NULL) delete tripletCalc;
	if (quartetCalc != NULL) delete quartetCalc;

	#ifdef _MSC_VER
		#ifdef _DEBUG
			_CrtDumpMemoryLeaks();
			system("pause");
		#endif
	#endif
}

int main(int argc, char** argv)
{
	// Register exit-handler (for debug printing etc)
	atexit(program_exiting);
	
	bool calcTripDist = false;
	bool calcQuartDist = false;
	bool printTree = false;
	bool printRootedTree = false;
	bool printHDT = false;
	char *filename1 = NULL, *filename2 = NULL;

	// Parse commandline
	for(int i = 1; i < argc; i++)
	{
		calcTripDist = calcTripDist || (_stricmp(argv[i], "calcTripDist") == 0);
#ifdef quartetsToo
		calcQuartDist = calcQuartDist || (_stricmp(argv[i], "calcQuartDist") == 0);
#endif
		printTree = printTree || (_stricmp(argv[i], "printTree") == 0);
		printRootedTree = printRootedTree || (_stricmp(argv[i], "printRootedTree") == 0);
		printHDT = printHDT || (_stricmp(argv[i], "printHDT") == 0);
	}

	if (!calcTripDist && !calcQuartDist && !printTree && !printRootedTree && !printHDT)
	{
		// No (reasonable) commands given!
		usage(argv[0]);
		return -1;
	}

	// If calculating distance, check that the last 2 inputs are files
	if (calcTripDist || calcQuartDist)
	{
		if (argc < 4)
		{
			// Error!
			cerr << "Not enough parameters for calculating distance! (" << argc << " < 4)" << endl;
			usage(argv[0]);
			return -1;
		}
		filename1 = argv[argc-2];
		filename2 = argv[argc-1];
	}

	// If parsing or printing, require 1 input file!
	if (printTree || printRootedTree || printHDT)
	{
		if (argc < 3)
		{
			// Error!
			cerr << "Not enough parameters for parsing or printing! (" << argc << " < 3)" << endl;
			usage(argv[0]);
			return -1;
		}
		filename2 = argv[argc-1];
	}

	// Parse the input files
	parser = new NewickParser();
	if (filename1 != NULL)
	{
		ut1 = parser->parseFile(filename1);
		if (ut1 == NULL || parser->isError())
		{
			cerr << "Parsing of file \"" << filename1 << "\" failed." << endl;
			cerr << "Aborting!" << endl;
			return -1;
		}
	}

	if (filename2 != NULL)
	{
		ut2 = parser->parseFile(filename2);
		if (ut2 == NULL || parser->isError())
		{
			cerr << "Parsing of file \"" << filename2 << "\" failed." << endl;
			cerr << "Aborting!" << endl;
			return -1;
		}
	}

	// Initialize calculators
	if (calcTripDist) tripletCalc = new Soda13Calc();
	if (calcQuartDist) quartetCalc = new Soda13Calc();

	// Do as told!
	INTTYPE_N4 lastResult = -1;
	if (calcTripDist)
	{
		if (rt1 == NULL) rt1 = ut1->convertToRootedTree(NULL);
		if (rt2 == NULL) rt2 = ut2->convertToRootedTree(rt1->factory);
		lastResult = tripletCalc->calculateTripletDistance(rt1, rt2);
		cout << lastResult << endl;
	}
	if (calcQuartDist)
	{
		// Smallest D as t1
		if (ut1->maxDegree <= ut2->maxDegree)
			lastResult = quartetCalc->calculateQuartetDistance(ut1, ut2);
		else
			lastResult = quartetCalc->calculateQuartetDistance(ut2, ut1);
		cout << lastResult << endl;
	}
	if (printTree)
	{
		ut2->toDot();
	}
	if (printRootedTree)
	{
		rt2 = ut2->convertToRootedTree(NULL);
		rt2->toDot();
	}
	if (printHDT)
	{
		rt2 = ut2->convertToRootedTree(NULL);
		HDT *h = HDT::constructHDT(rt2, 0, NULL); // doesn't need degree here!
		h->toDot();
		delete h->factory;
	}

	return 0;
}
