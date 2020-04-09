// SP71: a program for unsupervised learning in the SP framework.

// Copyright (C) 2005 J G Wolff.

// This program is released to the public domain without any restrictions.
// You can redistribute it and/or modify it as you wish.

// In any publication that refers to any version of this program, please
// acknowledge J G Wolff as the originator of the program. 

// A description of SP70, the precursor of this program, may be found
// in "Unsupervised learning in a framework of information compression
// by multiple alignment, unification and search", J G Wolff, March 2002.
// A copy may be obtained from http://uk.arxiv.org/abs/cs.AI/0302015.
// See also http://www.cognitionresearch.org.uk/papers/ul/ul.htm.

// Please retain this notice on all versions.

// Dr J G Wolff, www.CognitionResearch.org.uk.
// Email: jgw@cognitionresearch.org.uk.

// April 2005.

// SP71.cpp : Defines the entry point for the console application.
//

#include "SP71_head.h"

int main(int argc, char* argv[])
{
	// Make sure the program has been invoked with 
	// the correct arguments.

	// NOTE: If this program is developed in some environment other than
	// MS Visual C++, then `SP_main()' should probably become `main()' and the
	// following lines of code (up to `SP_main()') should be moved to
	// the beginning of main().

	if (argc < 6)
	{
		fprintf(stderr, "Usage:\n\t%s <in-file> <parameters-file> <out-file> <latex-file> <plot-file>\n", "sp71");
		return 1;
	}

	char* progname = argv[0];

	in_filename = *++argv; // Name of file containing element
						   // syntax and descriptions.
	parameters_filename = *++argv; // Name of file containing parameters.
	out_filename = *++argv; // Name of file where element
							// syntax and descriptions are written.
	latex_filename = *++argv; // Name of file where the best
							// sequence found is written.
	plot_filename = *++argv; // Name of file where plotting
							// information will be written.
	SP_main();

	return 0;
}


