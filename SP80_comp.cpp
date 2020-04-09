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

#define PROGRAM "SP71"

#define VERSION "9.0"

/*
**
** ABSTRACT:	Development of the SP model for unsupervised inductive
**				learning.
**
** ENVIRONMENT: The module requires the header file "SP71_head.h"
**				to be on the `include' path. It also makes use of
**				routines defined in "SP71_lib.cpp"
**
** AUTHOR:		Gerry Wolff
**
** MODIFIED BY:
**
** SP71:
**
** 1.0 JGW 18/6/03 -	Derived from SP70, v 9.2. The main objectives in this
**						version are described in sp71_od, %1.
**
**						Since mis-matches do not occur with simple examples, the
**						first step will be to try to implement item 2 in that
**						discription. Because learning during parsing results
**						in the creation of 'composite' alignments, each of which
**						is also a 'full' alignment, it should not be necessary to
**						re-parse the PFNs in order to derive frequency values needed
**						for the process of compiling grammars. After a single parsing
**						of each PFN, it should be possible to move straight on to
**						compiling alternative grammars. In this version, compiling
**						of grammars is done after the parsing of each PFN. To allow
**						for the fact that early parses are poor (because the system
**						has not yet acquired much knowledge), compiling of grammars
**						is based on a subset of the PFNs and these are always the
**						most recently parsed.
** 1.1 JGW 12/7/03 -	After each PFN has been processed, the best one or two grammars
**						are selected, the patterns in them are cleaned up by removing
**						unnecessary ID-symbols, ID-symbols are renamed in a tidy manner
**						and Old is purged of all patterns that are not in those grammars.
**						The variables for numbering ID-symbols are reset in an appropriate
**						manner. See sp71_OD, %4.
** 1.2 JGW 20/10/03 -	Re-introduces re-parsing as a way of achieving accurate
**						frequency values for patterns prior to the compiling
**						of grammars (see sp71_od, %6).
** 1.3 JGW 27/10/03 -	This version attempts to keep track of discontinuous
**						dependencies by including within each grammar, the
**						encoded form of each of the original patterns. The program
**						minimises (G + E), as before.
** 1.4 JGW 4/11/03 -	New patterns are created from a partial match between
**						*two* patterns, as described in sp71_od, %11.
** 1.5 JGW 9/11/03 -	New patterns created from an alignment of two
**						patterns as described in sp71_od, %12.
** 1.6 JGW 10/11/03 -	When a mis-match between Old patterns is detected
**						during parsing in Phase 1, the program invokes
**						extract_patterns() to create new patterns to be
**						added to old_patterns. In this case, the alignment
**						containing the mis-match is treated as an alignment
**						between the driving pattern and the target pattern
**						and each of these are treated as simple patterns
**						(see sp71_od, %13).
** 1.7 JGW 11/11/03 -	Continues development of 1.6 (which now includes
**						functions for displaying hit sequences in
**						horizontal or vertical format) and revises
**						extract_patterns() to work with hit sequences
**						rather than alignments. This saves having to
**						construct alignments which are then discarded.
** 1.8 JGW 5/12/03 -	Returns to alignments that omit columns for
**						non-hit New symbols. Continues development.
**						Tries a new version of compare_patterns that
**						ensures that, if two left brackets are matched,
**						then the corresponding pair of right brackets
**						must be matched. This attempt fails: checking
**						the validity of bracket matches at the time
**						hits are recorded is not straightforward.
** 1.9 JGW 8/12/03 -	This version checks the validity of bracket
**						matchings just before a hit sequence is
**						converted into an alignment. If the hit sequence
**						contains invalid matchings of brackets, it is
**						rejected.
** 1.10 JGW 9/12/03 -	Modified so that brackets no longer have the
**						status IDENTIFICATION and discrimination symbols
**						are added only when they are actually needed
**						(see sp71_od, %16).
** 2.0 JGW 9/12/03 -	Creates version 3 of extract_patterns().
** 2.1 JGW 12/12/03 -	Introduces index for matches between brackets, as
**						described in sp70_od, %18.
** 2.2 JGW 22/1/04 -	Scraps the system for checking legal matches between
**						brackets in v 2.1 and replaces it with a system that
**						checks left and right brackets in any one pattern (see
**						sp71_od, %20). This version turns out to be very
**						inefficient owing to the multiplicity of hit
**						sequences arising from unconstrained matching of
**						brackets.
** 2.3 JGW 22/1/04 -	This enforces legal matches between brackets in
**						the same way as in SP62 - by only matching 'outer'
**						brackets with a legal pair of 'inner' brackets.
** 2.4 JGW 23/1/04 -	Refinements as described in sp71_od, %21.
** 2.5 JGW 28/1/04 -	Refinement of cleaning up process for grammars,
**						as described in sp71_od, %23.
** 3.0 JGW 13/5/04 -	1 Any kind of mismatch or incomplete matching of
**						a pattern during parsing should lead to the
**						creation of new coded patterns to be added to Old.
**						2 After each pattern from new is parsed, with
**						the creation of new patterns for Old, all the
**						New patterns up to and including the current
**						New pattern are reparsed, sifting and sorting
**						is applied, and all Old patterns other than those
**						in the best one or two grammars are discarded.
**						This should help to reduce the size of Old and
**						speed up later processing. It also enables one
**						to get an idea of how the grammars are developing.
**						See sp71_od, %27.
** 4.1 JGW 2/6/04 -		Derived from v 3.0. This version attempts to
**						concatenate parsings when the main parsing
**						process is finished and then to learn new
**						structures from such parsings. See sp71_od, %31.
** 5.0 JGW 7/6/04 -		Only 'full' alignments are accepted at the end
**						of each cycle for passing on to the next cycle,
**						ie alignments in which all the C-symbols are
**						matched. At the end of each cycle, the system
**						tries to combine sub-alignments to find about 2
**						'good' combinations of sub-alignments.
**						See sp71_od, %33.
** 5.1 JGW 11/6/04 -	Modifies the functions for writing out alignments
**						so that all symbols are seen in their correct
**						positions relative to other symbols.
**						See sp71_od, %34.
** 5.2 JGW 12/6/04 -	Re-writes compare_patterns_with_brackets() as
**						described in sp71_od, %35.
** 5.3 JGW 15/6/04 -	Sorting out problems with parsing and formation
**						of composite alignments as described in
**						sp71_od, %36.
** 5.4 JGW 15/6/04 -	Creates abstract patterns and composite alignments
**						from 'good' combinations (see sp71_od, %37).
** 5.5 JGW 21/6/04 -	Distinguishes 'driving' alignments from all of
**						current_alignments and adds code pattern to Old
**						for every completed alignment, as described in
**						sp71_od, %38 and %39.
** 5.6 JGW 25/6/04 -	Re-uses CLASS_SYMBOLs in different contexts.
**						(see sp71_od, %41.)
** 6.0 JGW 23/7/04 -	Reorganisation of SP71 as described in sp71_od,
**						%45 (10, step 1). The overall aim is integrate
**						recursive checking of newly-created patterns
**						with the recursive building of multiple
**						alignments. Explicit building of multiple
**						alignments will be replaced by the successive
**						processing of code patterns. If explicit
**						multiple alignments are required, they
**						can be derived from the encodings as required.
**						This version does parsing with code patterns but
**						does not yet integrate the generation of code
**						patterns with parsing (see sp71_od, %47).
** 6.1 JGW 28/7/04 -	Attempts to integrate the creation of code patterns
**						with parsing, as described in sp71_od, %47.
** 6.2 JGW 30/7/04 -	Continues integration and implements composite rule
**						described in sp71_od, %50.
** 6.3 JGW 3/8/04 -		Develops the idea that each unified pattern assumes
**						the role of the abstract pattern and a succession
**						of unmatched portions of an alignment are encoded
**						as a single sequence, not a sucession of discrete
**						sequences. See sp71_od, %52.
** 7.0 JGW 5/8/04 -		Returns to alignments as the basis for learning
**						rather than code patterns (see sp71_od, %53). This
**						version builds on the developments in v 6.3.
** 7.1 JGW 10/8/04 -	Re-introduces combine_alignments(), retains the
**						requirement that only 'full' alignments can proceed
**						to the next cycle of recognise() and postpones
**						learning until recognise() is finished. (See
**						sp71_od, %55 to %59). This version not completed.
** 7.2 JGW 6/12/04 -	A new start as described in sp71_od, %62.
** 7.3 JGW 17/12/04 -	Further development with the main goal to provide
**						*alternative* encodings of null elements (sp71_od,
**						%65) with subsidiary goals as described in that
**						section.
** 7.4 JGW 20/12/04 -	Creates new abstract patterns when required rather
**						than in response to null patterns (see sp71_od, %67).
** 7.5 JGW 28/12/04 -	Tries to sort out apparent anomalies in the
**						sifting and sorting part of the program as
**						described in sp71_od, %72.
** 7.6 JGW 5/1/05 -		Rationalisation of class symbols and discrimination
**						symbols (to become context symbols and unique
**						identifiers) as described in sp71_od, %77.
** 7.7 JGW 10/1/05 -	Tries learning by merging all the Old patterns in
**						the reference alignment as described in sp71_od, %79.
**						This version was not completed and contains
**						programming errors.
** 7.8 JGW 12/1/05 -	Attempts learning that preserves intermediate-level
**						structures, as described in sp71_od, %81.
** 7.9 JGW 25/1/05 -	Further development as described in sp71_od, %83
**						(3): To ensure lossless compression,
**						create code patterns for each New pattern and include
**						them with the grammars for these patterns.
**						After reflection, this item is dropped
**						(see sp71_od, %84).
**						This version also introduces the status
**						BOUNDARY_MARKER for LEFT_BRACKET or RIGHT_BRACKET
**						at the beginnings and ends of patterns (as in
**						SP62). In Phase 2, only FULL_B alignments are selected
**						as described in sp71_od, %84.
** 7.10 JGW 31/1/05 -	Further development as described in sp71_od, %83
**						(4 and 5):  4 To ensure that the abstract
**						patterns have a function in compression, only
**						introduce '#' symbols when they are actually
**						needed to differentiate alternatives, re-use
**						the same symbols in different contexts, and give
**						these symbols the minimum number of bits needed
**						to differentiate them from each other. On reflection,
**						unique ID symbols are better than discrimination
**						symbols and they will be retained (see sp71_od,
**						%86). 5 Clean up grammars more often.
** 7.12 JGW 31/1/05 -	Derived from v 7.10. This version has two separate
**						learning procedures for creating new patterns to be
**						added to Old: one applies *within* the scope of
**						an alignment, as before, and the other applies
**						*outside* the scope of one or more alignments and
**						can be applied to composite alignments as well as
**						single alignments. The program also enforces the
**						rule that an alignment can only proceed from one
**						parsing cycle to the next if all its CONTENTS symbols
**						are matched (as before) and all the New hit symbols
**						within the scope of the alignment form a coherent
**						sequence with no gaps in the New pattern.
**						(see sp71_od, %91).
** 7.13 JGW 21/2/05 -	Modifies compiling of grammars so that, for any
**						given pattern from New, an alignment that is
**						contained within another alignment is not used.
**						(see sp71_od, %97). Unique ID symbols are only
**						added to patterns if there is a genuine choice
**						between two alternatives in a given context or
**						if the pattern is a top-level pattern (see
**						sp71_od, %99).
** 8.0 JGW 1/3/05 -		Attempts to introduce generalisation and the
**						correction of over-generations (see sp71_od,
**						%102).
** 8.1 JGW 17/3/05 -	Attempts to fix problems when short sentences
**						come before long ones. This version successfully
**						abstracts 'correct' grammars when short sentences
**						come before long ones and vice versa (see sp71_od,
**						%109).
** 8.2 JGW 22/3/05 -	Further refinement of the program. [Converted
**						into v 9.0].
** 9.0 JGW 30/3/05 -	Attempts recursive application of matching
**						two patterns and deriving new patterns from
**						partial matches, as outlined in sp71_od, %112
**						and %113.
**
*/

#include "SP71_head.h"

#define PRINT_CD_VALUES FALSE
#define MAX_DR_PATTS_ALIGNMENT 10
#define CONTAINED_IN FALSE
#define NUMBER_OF_PRESENTATION_GRAMMARS 3

double size_of_best_grammar;
char* in_filename;
char* parameters_filename;
char* out_filename;
char* latex_filename;
char* plot_filename;
FILE* input_file;
FILE* parameters_file;
FILE* output_file;
FILE* latex_file;
FILE* plot_file;

group* corpus;
group* new_patterns; // New patterns supplied at the start of processing
group* old_patterns; // Old patterns supplied at the start of processing.
	// In 'learning' mode, there may not be any patterns in old_patterns.
group* original_symbols_in_corpus;
group* symbol_types_in_old;

group* parsing_alignments; // This holds alignments for the
	// current New pattern.
group* cumulative_parsing_alignments; // In Phase 2, this holds 
	// alignments for all the new patterns up to and including
	// the current New pattern.
group* selected_cumulative_parsing_alignments;
group* symbols_in_new; // A list of the types of symbols (symbols with
	// a given name) found in new_patterns.
group* set_of_grammars;
group* set_of_combinations;
group* best_combinations;
sequence* current_new_pattern;
grammar* naive_grammar; // Contains the patterns from New, each one
	// with added code symbols so that it can be used for encoding
	// data.
hit_node* hit_root; /* A pointer to the root of the hit structure */
hit_node* hn_master; /* Used in add_hit(). */
int original_alphabet_size; // The size of the alphabet of symbols 
	// used in the original corpus.
int current_alphabet_size; // The size of the alphabet after learning
	// has been done, including the creation of new code symbols.
int group_ID_number = 1;
int sequence_ID_number = 1;
int hit_node_ID_number = 1;
int combination_ID_number = 1;
int grammar_ID_number = 1;
int cycle;
int phase;
int new_patterns_counter;
double score_for_best_grammar, G_for_best_grammar,
E_for_best_grammar, G_naive = 0, E_naive = 0, GE_naive,
raw_data = 0, compression;

/*** ARRAYS, CONSTANTS AND INDICES FOR STORING SYMBOL COUNTS FOR
EACH SYMBOL IN NEW AND FOR STORING THE HIT STRUCTURE ***/

/*** STRUCTS, ARRAYS, CONSTANTS AND INDICES ***/

struct pnc_entry
{
	sequence* al1; // The alignment from which the pnc comes.
	sequence* pnc; // A 2-row alignment which represents the pnc.
	int next_set; // An index pointer to the next set of pncs.
	int next_pnc; // An index pointer to the next pnc in
				// given set.
}; // Used in probabilities_of_inferences

hit_node** leaf_array; /* leaf_array[] is used for recording the
	leaf nodes of the hit structure. */

int* sort_array; /* sort_array[] is used for sorting the
	entries in leaf_array[]. */

int fe_sort; /* The first empty entry in sort_array[]. */

sequence** alignments_array;

sequence* original_patterns_array[MEDIUM_SCRATCH_ARRAY_SIZE];

int half_hit_structure_rows;

struct patterns_counts_record
{
	sequence* pattern1;
	int frequency, max_cioa, count_in_one_alignment;
	// max_cioa = "maximum count in one alignment".
	// frequency is the cumulative value of max_cioas
	// for successive New patterns.
};

struct patterns_counts_record* patterns_counts_array;

int size_of_old_patterns;

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	For the grammars in set_of_grammars, this function writes
**	out each grammar.
**
** CALLING SEQUENCE:
**
**	void write_grammars(int limit, bool write_derived_from)
**
** FORMAL ARGUMENTS:
**
**	Return value:			void
**
**	limit:					The number of grammars to be written.
**
**	write_derived_from:		If true, 'derived_from' field is written,
**							otherwise it is not.
**
** IMPLICIT INPUTS:
**
**	set_of_grammars
**
** IMPLICIT OUTPUTS:
**
**	Cleaned up grammars.
**
** SIDE EFFECTS:
**
**	NONE
*/

void write_grammars(int limit, bool write_derived_from)
{
	grammar* grammar1;
	int counter = 0;

	list_for(grammar1, grammar, set_of_grammars)
	{
		grammar1->write_grammar(write_derived_from, false);
		grammar1->write_grammar(write_derived_from, true);
		if (++counter >= limit) break;
	}
} // write_grammars

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	For each pattern in old_patterns, this function counts the number
**	of times it is recognised in full_alignments (all the
**	full alignments formed for ONE pattern from New). This
**	count is the maximum number of times it is 'completely' matched
**	in any ONE alignment for a given pattern from New. In this context,
**	'complete' matching means that all the CONTENTS symbols of
**	the given pattern are matched and that all the CONTENTS
**	symbols of all the other patterns in the alignment are matched.
**
**	Notice that this method of counting avoids spurious double counting
**	that might arise from the fact that any given pattern from New
**	may appear in several different alternative alignments. Amongst
**	these alternative alignments, we are only interested in the
**	*maximum* count in any *one* alignment for a given pattern from New.
**	These maximum counts for each pattern from New are summed across
**	the one or more patterns in New.
**
** CALLING SEQUENCE:
**
**	void count_patterns_and_symbol_types(sequence *cnp)
**
**	cnp:		The last New pattern to be processed in
**				the main program. If the value is NIL,
**				all patterns are to be processed.
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
** IMPLICIT INPUTS:
**
**	old_patterns, full_alignments
**
** IMPLICIT OUTPUTS:
**
**	NONE
**
** SIDE EFFECTS:
**
**	NONE
**
*/

void count_patterns_and_symbol_types(sequence* cnp)
{
	sequence* pattern1, * pattern2, * alignment1, * pattern3, * pattern4;
	int sequence_depth, row;

	delete symbol_types_in_old; // This deletes the group and
		// all the symbols in the group. The next function 
		// re-creates everything from scratch.

	symbol_types_in_old =
		old_patterns->compile_alphabet(&current_alphabet_size);

	size_of_old_patterns = old_patterns->count_number_of_children();
	patterns_counts_array = new patterns_counts_record[size_of_old_patterns];

	int index1 = 0;

	list_for(pattern1, sequence, old_patterns)
	{
		patterns_counts_array[index1].pattern1 = pattern1;
		patterns_counts_array[index1].frequency = 0;
		index1++;
	}

	// For each 'full' alignment, count the number of occurrences
	// of each pattern from Old in the alignment. For each New pattern,
	// and any given Old pattern, find the maximum number of times that
	// Old pattern appears any alignment that parses the New pattern. Add
	// that value to the frequency value for the Old pattern.

	// Print alignments where the New pattern and all the Old patterns
	// are fully matched.

	fprintf(output_file, "CUMULATIVE 'FULL_A' ALIGNMENTS FROM PARSING (");
	print_pattern_cycle(false, cnp);
	fprintf(output_file, "):\n\n");

	list_for(alignment1, sequence, cumulative_parsing_alignments)
	{
		if (alignment1->get_degree_of_matching() != FULL_A) continue;

		fprintf(output_file, "%s%d%s",
			"ID",
			alignment1->get_ID(),
			": ");
		alignment1->write_tree_object(PRINT_SEQUENCE_FREQUENCY);
	}

	cumulative_parsing_alignments->sort_by_compression_difference();

	// For each of the patterns in new_patterns, obtain values
	// for max_cioa and add them to the frequency values of
	// relevant patterns.

	list_for(pattern3, sequence, new_patterns)
	{
		for (index1 = 0; index1 < size_of_old_patterns; index1++)
			patterns_counts_array[index1].max_cioa = 0;

		// Obtain values for count_in_one_alignment and max_cioa.

		list_for(alignment1, sequence, cumulative_parsing_alignments)
		{
			if (alignment1->get_degree_of_matching() != FULL_A) continue;
			pattern4 = alignment1->get_row_pattern(0);
			if (pattern4 != pattern3) continue;

			// We now processing only those alignments that have
			// pattern3 as their New pattern.

			// For each of the patterns in alignment1
			// (except the current New pattern), update
			// count_in_one_alignment and max_cioa.

			for (index1 = 0; index1 < size_of_old_patterns; index1++)
				patterns_counts_array[index1].count_in_one_alignment = 0;

			sequence_depth = alignment1->get_sequence_depth();
			for (row = 1; row < sequence_depth; row++)
			{
				pattern1 = alignment1->get_row_pattern(row);
				for (index1 = 0; index1 < size_of_old_patterns; index1++)
				{
					pattern2 = patterns_counts_array[index1].pattern1;
					if (pattern1 == pattern2) break;
				}

				if (index1 >= size_of_old_patterns)
					abort_run("pattern1 not found in \
						count_patterns_and_symbol_types()");

				(patterns_counts_array[index1].count_in_one_alignment)++;
				if (patterns_counts_array[index1].count_in_one_alignment >
					patterns_counts_array[index1].max_cioa)
					patterns_counts_array[index1].max_cioa =
					patterns_counts_array[index1].count_in_one_alignment;
			}
		}

		// Now add max_cioa values to frequency counts for the patterns
		// in old_patterns.

		for (index1 = 0; index1 < size_of_old_patterns; index1++)
			patterns_counts_array[index1].frequency +=
			patterns_counts_array[index1].max_cioa;

		if (pattern3 == cnp) break;
	} // Get next New pattern.

	// Transfer final frequency values to patterns in Old and print
	// them out at the same time.

	fprintf(output_file, "PATTERNS IN OLD (");
	print_pattern_cycle(false, cnp);
	fprintf(output_file, "):\n\n");

	int frequency;
	for (index1 = 0; index1 < size_of_old_patterns; index1++)
	{
		pattern1 = patterns_counts_array[index1].pattern1;
		frequency = patterns_counts_array[index1].frequency;
		pattern1->set_frequency(frequency);
		pattern1->write_pattern(true, true);
	}

	// Find the frequency of each symbol type by scanning
	// over the patterns in the corpus and adding the frequency
	// of the pattern to the frequency of the symbol type for
	// each occurrence of each symbol in the pattern in which
	// it occurs.

	find_symbol_frequencies(symbol_types_in_old, false, true,
		cnp);

	// Compute bit_cost for each symbol type. Assign costs to
	// instances of the symbols in new_patterns and old_patterns.

	calculate_and_assign_frequencies_and_costs(symbol_types_in_old, false);

	delete[] patterns_counts_array;

} // count_patterns_and_symbol_types

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Checks whether or not an alignment has all its New symbols matched
**	and all CONTENTS symbols in its Old patterns matched. If all New symbols
**	and all CONTENTS symbols in Old patterns are matched, the alignment is
**	marked as FULL_A. If all CONTENTS symbols in Old patterns are matched
**	but not all New symbols are matched, the alignment is marked as FULL_B.
**	Otherwise, the alignment is marked as PARTIAL.
**
** CALLING SEQUENCE:
**
**	int sequence::find_degree_of_matching(sequence *cnp)
**
** FORMAL ARGUMENTS:
**
**	Return value:		FULL_A, FULL_B or PARTIAL.
**
**	cnp:				The New pattern being processed.
**
** IMPLICIT INPUTS:
**
**	NONE
**
** IMPLICIT OUTPUTS:
**
**	NONE
**
** SIDE EFFECTS:
**
**	NONE
**
*/

int sequence::find_degree_of_matching(sequence* cnp)
{
	symbol* col1, * symbol1;
	int i, return_value;
	bool all_new_symbols_are_matched,
		all_old_contents_symbols_matched = true;

	// Check to see whether all the New symbols are matched.

	all_new_symbols_are_matched = this->all_new_symbols_matched(cnp);

	// Check whether all the CONTENTS symbols of all the Old
	// patterns are matched.

	list_for(col1, symbol, this)
	{
		if (col1->is_a_hit()) continue;

		// Find the single symbol in an Old pattern in this column.
		// There should not be any New symbol because, in general,
		// unmatched New symbols are not entered into alignments.

		symbol1 = col1->find_unmatched_symbol();

		if (symbol1 == NIL)
			abort_run("Empty column in sequence::find_degree_of_matching().");

		if (symbol1->get_status() == CONTENTS)
		{
			all_old_contents_symbols_matched = false;
			break;
		}
	}

	// If all the New symbols are not matched, check to see whether
	// any of the unmatched symbols fall within the scope of
	// of any one Old pattern. Strictly speaking, it should only
	// ever be necessary to check the bottom Old pattern in any
	// alignment because the rules for the formation of alignments
	// require that only FULL_B alignments can proceed from one
	// parsing cycle to the next or be involved in the formation
	// of composite alignments.

	int row, sequence_length = count_number_of_children(),
		index1, index2, new_int_pos1, new_int_pos2;
	symbol** col_array = NIL, * new_symbol, * old_symbol;
	bool backwards_symbol_found, forwards_symbol_found,
		unmatched_new_within_scope = false;
	if (all_new_symbols_are_matched == false)
	{
		// Set up the sequence as an array.

		col_array = new symbol * [sequence_length];
		i = 0;
		list_for(col1, symbol, this)
		{
			col_array[i] = col1;
			i++;
		}

		// Now search for gaps in the New-symbol hit sequence ;

		new_int_pos2 = NULL_VALUE;
		for (index1 = 0; index1 < sequence_length; index1++)
		{
			col1 = col_array[index1];
			new_symbol = col1->get_row_symbol(0);
			if (new_symbol == NIL) continue;

			new_int_pos1 = col1->get_row_orig_int_pos(0);
			if (new_int_pos2 == NULL_VALUE) goto L1;

			if (new_int_pos1 - new_int_pos2 > 1)
			{
				// For each row, search backwards from index2
				// and forwards from index1 to see whether there
				// is any non-NIL symbol in the row. If there is
				// in both directions, then the gap falls within
				// the scope of the Old pattern on this row.

				for (row = sequence_depth - 1; row >= 1; row--)
				{
					backwards_symbol_found =
						forwards_symbol_found = false;

					// Search backwards first.

					for (i = index2; i >= 0; i--)
					{
						col1 = col_array[i];
						old_symbol = col1->get_row_symbol(row);
						if (old_symbol != NIL)
						{
							backwards_symbol_found = true;
							break;
						}
					}

					// Now search forwards.

					for (i = index1; i < sequence_length; i++)
					{
						col1 = col_array[i];
						old_symbol = col1->get_row_symbol(row);
						if (old_symbol != NIL)
						{
							forwards_symbol_found = true;
							break;
						}
					}

					if (backwards_symbol_found
						&& forwards_symbol_found)
					{
						// We have found a break in the New hit
						// sequence within the scope of an
						// Old pattern

						unmatched_new_within_scope = true;
						if (row < sequence_depth - 1)
							abort_run("Anomalous alignment in \
								sequence::find_degree_of_matching().");
						goto L2;
					}
				}
			}

		L1: new_int_pos2 = new_int_pos1;
			index2 = index1;
		}

		delete[] col_array;
	}

L2:;

	if (all_new_symbols_are_matched && all_old_contents_symbols_matched)
	{
		set_degree_of_matching(FULL_A);
		return_value = FULL_A;
	}
	else if (all_old_contents_symbols_matched
		&& unmatched_new_within_scope == false)
	{
		set_degree_of_matching(FULL_B);
		return_value = FULL_B;
	}
	else if (all_old_contents_symbols_matched
		&& unmatched_new_within_scope == true)
	{
		set_degree_of_matching(FULL_C);
		return_value = FULL_C;
	}
	else
	{
		set_degree_of_matching(PARTIAL);
		return_value = PARTIAL;
	}

	return(return_value);

} // sequence::find_degree_of_matching

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Checks whether or not an alignment has all the DATA_SYMBOLs in
**	it Old patterns matched to symbols in New.
**
** CALLING SEQUENCE:
**
**	bool sequence::has_all_data_symbols_matched()
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if all DATA_SYMBOLs are matched to symbols in New,
**					false otherwise.
**
** IMPLICIT INPUTS:
**
**	NONE
**
** IMPLICIT OUTPUTS:
**
**	NONE
**
** SIDE EFFECTS:
**
**	NONE
**
*/

bool sequence::has_all_data_symbols_matched()
{
	symbol* col1, * symbol1, * new_symbol;
	int row;

	list_for(col1, symbol, this)
	{
		// Look in rows below row 0 to see that any Old symbols have
		// type DATA_SYMBOL. Any such symbol must be matched to
		// a New symbol, otherwise the function returns false.

		for (row = 1; row < sequence_depth; row++)
		{
			symbol1 = col1->get_row_symbol(row);
			if (symbol1 == NIL) continue;
			if (symbol1->get_type() != DATA_SYMBOL) break;

			// We have an Old symbol that is a DATA_SYMBOL.

			new_symbol = col1->get_row_symbol(0);
			if (new_symbol == NIL) return(false); // We have found
				// an Old DATA_SYMBOL that is not matched with a
				// New symbol.
			break; // There is no need to carry on testing
				// other symbols in the column.
		}
	}

	return(true); // All the DATA_SYMBOLs in the alignment are matched
		// to New symbols.

} // sequence::has_all_data_symbols_matched

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	For the 'full' alignments stored in
**	full_alignments, re-compute codes and
**	encoding costs. Compute total_cost for each of the patterns in
**	new_patterns and old_patterns.
**
** CALLING SEQUENCE:
**
**	void compute_scores(sequence *cnp)
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	cnp:			The last New pattern to be processed. If the
**					value is NIL, all New patterns are to be
**					processed.
**
** IMPLICIT INPUTS:
**
**	full_alignments
**
** IMPLICIT OUTPUTS:
**
**	NONE
**
** SIDE EFFECTS:
**
**	NONE
**
*/

void compute_scores(sequence* cnp)
{
	sequence* al1, * pattern1;
	list_element* el_pos1 = NIL;

	list_for_el_pos(al1, sequence, cumulative_parsing_alignments, el_pos1)
	{
		if (al1->get_degree_of_matching() != FULL_A) continue;
		al1->correct_column_values(); // Each column receives the
			// frequency and bit_cost values for the symbol type
			// within the column (changed as a result of the
			// application of count_patterns_and_symbol_types()).
		al1->make_code(false);
	}

	// Compute costs for patterns in new_patterns

	fprintf(output_file, "Encoding costs for New patterns:\n\n");

	el_pos1 = NIL;
	list_for_el_pos(pattern1, sequence, new_patterns, el_pos1)
	{
		pattern1->compute_costs();

		fprintf(output_file, "%s%d%s%1.2f%s",
			"ID",
			pattern1->get_ID(),
			": ",
			pattern1->get_encoding_cost(),
			" ");
		pattern1->write_tree_object(PRINT_NO_FREQUENCIES);

		if (pattern1 == cnp) break;
	}

	// Compute costs for patterns in old_patterns.

	fprintf(output_file, "Encoding costs for Old patterns:\n\n");

	el_pos1 = NIL;
	list_for_el_pos(pattern1, sequence, old_patterns, el_pos1)
	{
		pattern1->compute_costs();

		fprintf(output_file, "%s%d%s%1.2f%s",
			"ID",
			pattern1->get_ID(),
			": ",
			pattern1->get_encoding_cost(),
			" ");
		pattern1->write_tree_object(PRINT_SEQUENCE_FREQUENCY);
	}

} // compute_scores

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	This function copies the patterns in the three best grammars
**	into three new grammars and then cleans out unnecessary symbols,
**	and renumbers the symbols tidily. Copying is necessary because it
**	often happen that a given pattern appears in more than one of the
**	best grammars and cleaning and tidying may give a different result
**	in each case.
**
** CALLING SEQUENCE:
**
**	void clean_and_tidy_best_grammars()
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
** IMPLICIT INPUTS:
**
**	NONE
**
** IMPLICIT OUTPUTS:
**
**	NONE
**
** SIDE EFFECTS:
**
**	NONE
*/

void clean_and_tidy_best_grammars()
{
	grammar* grammar1, * grammar2;
	int counter = 0;

	fprintf(output_file, "CLEANING AND TIDYING OF BEST GRAMMARS:\n\n");

	list_for(grammar1, grammar, set_of_grammars)
	{
		if (++counter > NUMBER_OF_PRESENTATION_GRAMMARS) break;
		grammar2 = new grammar;
		grammar2->copy_details_and_patterns(grammar1);
		grammar2->clean_up(grammar1->get_ID());
		grammar2->tidy_up_code_symbols(grammar1->get_ID());
		grammar2->delete_patterns();
		delete grammar2;
	}

} // clean_and_tidy_best_grammars

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Given a list of 'full' alignments from the parsing of patterns in New
**	(in full_alignments), this function compiles
**	a list of alternative grammars for the patterns in New and, for
**	each one, calculates a value for G (the size of the grammar)
**	and E (the size of New when it has been encoded in terms of
**	the grammar).
**
**	This function uses two constraints: full_alignment_limit (which
**	restricts the number of alternative alignments that may be
**	considered for any one pattern from New; and grammar_limit (which
**	restricts the number of alternative grammars that may be developed
**	at any one time.
**
**	This function also measures G and E for the best grammars found
**	for successive patterns from New and outputs this information in
**	a form that is suitable for plotting.
**
**	The code patterns used to derive E are included with the patterns
**	that are used to derive G.
**
** CALLING SEQUENCE:
**
**	void compile_alternative_grammars(sequence *cnp)
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	cnp:			The last New pattern to be processed in
**					the main program.
**
** IMPLICIT INPUTS:
**
**	full_alignments
**
** IMPLICIT OUTPUTS:
**
**	set_of_grammars
**
** SIDE EFFECTS:
**
**	NONE
**
*/

void compile_alternative_grammars(sequence* cnp)
{
	sequence* pattern_new, * coded_pattern_new = NIL, * pattern_row_0,
		* full_alignment;
	int alignment_counter, grammar_counter, np_counter;
	group* temp_set_of_grammars = new group;
	grammar* grammar1, * grammar2, * best_grammar = NIL, * grammar3,
		* grammar_tree = new grammar; // A NULL grammar that
			// forms the root of the tree of grammars.
	double lowest_score, temp_score;
	bool identical_grammars_found;

	fprintf(output_file, "START OF COMPILING ALTERNATIVE GRAMMARS:\n\n");

	compute_scores(cnp); // For the 'full' alignments 
		// stored in cumulative_parsing_alignments, re-compute codes and 
		// encoding costs. Compute total_cost for each of the patterns in
		// new_patterns and old_patterns. This function also corrects
		// the values for frequency and bit_cost for each column
		// in each alignment in full_alignments and it generates
		// explicit code patterns from each full alignment.

	// Now compile grammars.

	cumulative_parsing_alignments->sort_by_compression_difference();

	fprintf(output_file, "Cumulative parsing alignments in order of CD:\n\n");

	list_for(full_alignment, sequence, cumulative_parsing_alignments)
	{
		fprintf(output_file, "%s%d%s%3.2f%s",
			"ID",
			full_alignment->get_ID(),
			", CD = ",
			full_alignment->get_compression_difference(),
			"\n");
	}

	fprintf(output_file, "\n");

	naive_grammar->initialise();
	list_element* el_pos1 = NIL;
	np_counter = 0;
	list_for_el_pos(pattern_new, sequence, new_patterns, el_pos1)
	{
		np_counter++;
		coded_pattern_new = naive_grammar->get_next_pattern();
		alignment_counter = 0;
		list_for(full_alignment, sequence, selected_cumulative_parsing_alignments)
		{
			if (full_alignment->is_composite_alignment()) continue;

			if (full_alignment->get_degree_of_matching() != FULL_A) continue;

			// Select alignments that contain pattern_new in row 0.

			pattern_row_0 = full_alignment->get_row_pattern(0);
			if (pattern_row_0 != pattern_new) continue;

			alignment_counter++;
			if (alignment_counter > full_alignment_limit)
			{
				fprintf(output_file, "%s%d%s",
					"FULL_ALIGNMENT_LIMIT (",
					full_alignment_limit,
					") has been reached for New pattern:\n");
				pattern_new->write_pattern(true, false);
				break;
				// This cut-off assumes that full alignments in 
				// full_alignments have been sorted in
				// descending order of CD score.
			}
			// full_alignment has pattern_new in its row 0.

			if (set_of_grammars->is_empty())
			{
				grammar1 = new grammar;
				grammar_tree->add_derived_grammar(grammar1);
				grammar1->
					set_derived_from_alignment(full_alignment);
				temp_set_of_grammars->add_child(grammar1);
				grammar1->compile_grammar(full_alignment);

				grammar1->compute_G();
				grammar1->increment_E(full_alignment->
					get_encoding_cost());
				grammar1->sum_G_and_E();

				// Diagnostic

#if DIAGNOSTIC1
				fprintf(output_file, "DIAGNOSTIC:\n\n");
				grammar1->write_grammar(true);
				fprintf(output_file, "END DIAGNOSTIC\n\n");
				fflush(output_file);
#endif
			}
			else
			{
				// Step through the list of grammars as it was
				// at the end of processing the previous pattern
				// from New.

				list_for(grammar1, grammar, set_of_grammars)
				{
					grammar2 = new grammar(*grammar1);
					grammar1->add_derived_grammar(grammar2);
					grammar2->set_derived_from_grammar(grammar1->get_ID());
					grammar2->
						set_derived_from_alignment(full_alignment);
					grammar2->compile_grammar(full_alignment);

					grammar2->compute_G();
					grammar2->increment_E(full_alignment->
						get_encoding_cost());
					grammar2->sum_G_and_E();

					// Diagnostic

#if DIAGNOSTIC1
					fprintf(output_file, "DIAGNOSTIC:\n\n");
					grammar2->write_grammar(true);
					fprintf(output_file, "END DIAGNOSTIC\n\n");
					fflush(output_file);
#endif

					// Compare grammar2 with the grammars that are
					// already in temp_set_of_grammars. If a match
					// is found with any of them, determine which one
					// has the worst (highest) score and 
					// remove it from further consideration.

					identical_grammars_found = false;
					list_for(grammar3, grammar, temp_set_of_grammars)
					{
						if (grammar2->matches(grammar3))
						{
							identical_grammars_found = true;
							break;
						}
					}

					if (identical_grammars_found)
					{
						if (grammar2->get_score() > grammar3->get_score())
						{
#if DIAGNOSTIC2
							fprintf(output_file, "%s%d%s%3.2f%s%d%s%3.2f%s",
								"New grammar GR",
								grammar2->get_ID(),
								" (score = ",
								grammar2->get_score(),
								") is the same as the previous grammar\nGR",
								grammar3->get_ID(),
								" (score = ",
								grammar3->get_score(),
								") and is not considered further.\n\n");
#endif
							// delete grammar2 ;
						}
						else
						{
#if DIAGNOSTIC2
							fprintf(output_file, "%s%d%s%3.2f%s%d%s%3.2f%s",
								"Previous grammar GR",
								grammar3->get_ID(),
								" (score = ",
								grammar3->get_score(),
								") is the same as the new grammar\nGR",
								grammar2->get_ID(),
								" (score = ",
								grammar2->get_score(),
								") and is not considered further.\n\n");
#endif
							temp_set_of_grammars->extract_child(grammar3);
							// delete grammar3 ;
							temp_set_of_grammars->add_child(grammar2);
						}
					}
					else temp_set_of_grammars->add_child(grammar2);
				}
			}
		}

		// Release all the grammars in set_of_grammars

		set_of_grammars->release_children();

		// Compute the score of each grammar in temp_set_of_grammars
		// as G + E. At the same time, count the number of grammars
		// that have been formed.

		grammar_counter = 0;
		list_for(grammar1, grammar, temp_set_of_grammars)
		{
			grammar_counter++;
			grammar1->sum_G_and_E();

			// Diagnostic

#if DIAGNOSTIC2
			fprintf(output_file, "DIAGNOSTIC:\n\n");
			grammar1->write_grammar(true);
			fprintf(output_file, "END DIAGNOSTIC\n\n");
			fflush(output_file);
#endif
		}

		if (verbose)
			fprintf(output_file, "%s%d%s%d%s",
				"Number of grammars formed for New pattern ID",
				pattern_new->get_ID(),
				" = ",
				grammar_counter,
				"\n\n");

		// Now transfer newly-updated grammars from temp_set_of_grammars
		// to set_of_grammars in ascending order of their scores (smaller
		// scores are better than bigger ones). When
		// the number of grammars has reached grammar_limit, stop
		// transfering grammars and release the remaining grammars
		// instead. Print out grammars as they are transferred.

		grammar_counter = 0;
#if DIAGNOSTIC2
		fprintf(output_file, "Grammars created for New pattern:\n");
		pattern_new->write_pattern(true, false);
#endif
		while (temp_set_of_grammars->is_empty() == false)
		{
			if (grammar_counter >= grammar_limit)
			{
				temp_set_of_grammars->release_children();

				fprintf(output_file, "%s%d%s%d%s",
					"GRAMMAR_LIMIT (",
					grammar_limit,
					") reached for New pattern, ID",
					pattern_new->get_ID(),
					"\n\n");

				break;
			}
			else
			{
				lowest_score = HIGH_VALUE;
				list_for(grammar1, grammar, temp_set_of_grammars)
				{
					temp_score = grammar1->get_score();
					if (temp_score < lowest_score)
					{
						lowest_score = temp_score;
						best_grammar = grammar1;
					}
				}

#if DIAGNOSTIC2
				fprintf(output_file, "%s%d%s",
					"GR",
					best_grammar->get_ID(),
					"\n");
#endif
				temp_set_of_grammars->extract_child(best_grammar);

				set_of_grammars->add_child(best_grammar);
				grammar_counter++;
				if (verbose) best_grammar->write_grammar(true, false);
			}
		}

		if (pattern_new == cnp) break;
	} // Repeat the cycle for the next pattern from New.

	if (set_of_grammars->is_empty() == false)
	{
		best_grammar = (grammar*)set_of_grammars->get_first_child();
		G_for_best_grammar = best_grammar->get_G();
		E_for_best_grammar = best_grammar->get_E();
		score_for_best_grammar = best_grammar->get_score();
		G_naive += coded_pattern_new->get_total_cost();
		raw_data += pattern_new->get_total_cost();
		E_naive += coded_pattern_new->get_encoding_cost();
		GE_naive = G_naive + E_naive;
		naive_grammar->increment_E(E_naive);
		naive_grammar->increment_G(G_naive);
		naive_grammar->sum_G_and_E();
		compression = score_for_best_grammar / raw_data;

		fprintf(plot_file,
			"%d%s%3.2f%s%3.2f%s%3.2f%s%3.2f%s%3.2f%s%3.2f%s%3.2f%s%3.2f%s",
			new_patterns_counter,
			", ",
			G_for_best_grammar,
			", ",
			E_for_best_grammar,
			", ",
			score_for_best_grammar,
			", ",
			G_naive,
			", ",
			E_naive,
			", ",
			GE_naive,
			", ",
			raw_data,
			", ",
			compression,
			"\n");
	}

	// Now write out the grammars in set_of_grammars.

	fprintf(output_file, "FINAL GRAMMARS (");
	print_pattern_cycle(false, cnp);
	fprintf(output_file, "):\n\n");

	list_for(grammar1, grammar, set_of_grammars)
		grammar1->sort_by_ID();

	if (set_of_grammars->is_empty())
		fprintf(output_file, "No grammars formed.\n\n");
	else write_grammars(3, false);

	if (new_patterns_counter < number_of_patterns_in_new)
	{
		// Delete all Old patterns and replace them with patterns
		// in the n_grammars best grammars.

		purge_old_patterns(n_grammars, cnp);
	}
	else clean_and_tidy_best_grammars(); // This is for final presentation.

	// Release the grammars in set_of_grammars. They are still linked
	// in to the grammar_tree and will be deleted when that tree is
	// deleted (below).

	set_of_grammars->release_children();

#if DIAGNOSTIC1
	fprintf(output_file, "DIAGNOSTIC:\n\n");

	// Write out a trace of how the grammars were formed.

	grammar_tree->write_trace(0);
	fprintf(output_file, "\n");

	fprintf(output_file, "END DIAGNOSTIC\n\n");
	fflush(output_file);
#endif

	// Delete all the grammars in the grammar tree recursively.

	delete grammar_tree;

} // compile_alternative_grammars

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Makes a selection of cumulative_parsing_alignments and puts them
**	in selected_cumulative_parsing_alignments. The alignments that are
**	selected are those that are FULL_A and are not driving alignments
**	for any other alignment.
**
** CALLING SEQUENCE:
**
**	void select_alignments()
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
** IMPLICIT INPUTS:
**
**	cumulative_parsing_alignments
**
** IMPLICIT OUTPUTS:
**
**	selected_cumulative_parsing_alignments
**
** SIDE EFFECTS:
**
**	NONE
*/

void select_alignments()
{
	sequence* alignment1, * al1;
	list_element* el_pos1;
	bool is_driving_alignment;
	int full_alignment_ID;

	cumulative_parsing_alignments->sort_by_ID();

	list_for(alignment1, sequence, cumulative_parsing_alignments)
	{
		if (alignment1->get_degree_of_matching() != FULL_A) continue;

		// Check that full_alignment is not the driving pattern
		// for another alignment that is also in
		// cumulative_parsing_alignments.

		el_pos1 = NIL;
		is_driving_alignment = false;
		full_alignment_ID = alignment1->get_ID();
		list_for_el_pos(al1, sequence, cumulative_parsing_alignments,
			el_pos1)
		{
			if (al1->get_driving_ID() == full_alignment_ID)
			{
				is_driving_alignment = true;
				break;
			}
		}

		if (is_driving_alignment) continue;

		// alignment1 is FULL_A and it is not a driving alignment
		// for any other alignment.

		selected_cumulative_parsing_alignments->add_child(alignment1);
	}

} // select_alignments

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Looks for alignments in which are 'fully' matched in the sense that
**	all the New symbols are matched and all the CONTENTS symbols are matched.
**	From these, the function counts frequencies of patterns and symbols.
**	The function ends by computing a set of alternative grammars.
**
** CALLING SEQUENCE:
**
**	void sifting_and_sorting(sequence *cnp)
**
** FORMAL ARGUMENTS:
**
**	Return value:		void
**
**	cnp:				The last New pattern to be processed in
**						the main program.
**
** IMPLICIT INPUTS:
**
**	patterns in New
**
** IMPLICIT OUTPUTS:
**
**	Sets of alternative grammars
**
** SIDE EFFECTS:
**
**	NONE
**
*/

void sifting_and_sorting(sequence* cnp)
{
	fprintf(output_file, "START OF SIFTING AND SORTING (");
	print_pattern_cycle(false, cnp);
	fprintf(output_file, ")\n\n");

	select_alignments();

	count_patterns_and_symbol_types(cnp);

	fprintf(output_file, "OLD PATTERNS WITH DETAILS (");
	print_pattern_cycle(false, current_new_pattern);
	fprintf(output_file, ")\n\n");
	old_patterns->write_patterns_with_details();

	compile_alternative_grammars(cnp);

	fprintf(output_file, "END OF SIFTING AND SORTING (");
	print_pattern_cycle(false, cnp);
	fprintf(output_file, ")\n\n");

} // sifting_and_sorting

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Holding function for recognise() for one New pattern.
**
** CALLING SEQUENCE:
**
**	void recognition_cycles(sequence *pattern1, bool derive_patterns)
**
** FORMAL ARGUMENTS:
**
**	Return value:		void
**
**	pattern1:			A new pattern or derived pattern to be recognised.
**	derive_patterns:	If true, new patterns are derived from partial
**						alignments, otherwise they are not.
**
** IMPLICIT INPUTS:
**
**	NONE
**
** IMPLICIT OUTPUTS:
**
**	Alignments in parsing_alignments.
**
** SIDE EFFECTS:
**
**	NONE
*/

void recognition_cycles(sequence* pattern1, bool derive_patterns)
{
	sequence* al1;
	int counter, degree_of_matching_value;

	cycle = 0;

	while (recognise(pattern1));

	set_of_combinations->delete_children();

	if (derive_patterns == false) goto L1;

	// Derive new patterns from partial alignments.

	counter = 0;

	parsing_alignments->sort_by_compression_difference();

	fprintf(output_file, "Start of learning:\n(");
	print_pattern_cycle(true, pattern1);
	fprintf(output_file, ")\n\n");

	list_for(al1, sequence, parsing_alignments)
	{
		// if (al1->is_composite())
		// {
		// 	fprintf(output_file, "%s%d%s",
		// 		"Alignment ID",
		// 		al1->get_ID(),
		// 		" is composite and is excluded from learning.\n\n") ;
		// 	continue ;
		// }

		degree_of_matching_value = al1->get_degree_of_matching();
		if (degree_of_matching_value != FULL_A)
		{
			if (++counter > extraction_limit) break;

#if DIAGNOSTIC3
			fprintf(output_file, "DIAGNOSTIC 3A:\n\n");
			old_patterns->write_patterns("OLD PATTERNS:",
				NULL_VALUE, NULL_VALUE);
			fprintf(output_file, "END DIAGNOSTIC 3A:\n\n");
			fflush(output_file);
#endif

			al1->create_patterns();
		}
	}

L1: parsing_alignments->delete_children();

} // recognition_cycles

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Scans the Old patterns looking for ID-symbols that have no function,
**	either as identifiers within another pattern, or as discrimination
**	symbols, or as top-level identifiers. These symbols are removed.
**
** CALLING SEQUENCE:
**
**	void find_and_remove_unnecessary_ID_symbols()
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
** IMPLICIT INPUTS:
**
**	old_patterns
**
** IMPLICIT OUTPUTS:
**
**	Modification of one or more patterns in old_patterns.
**
** SIDE EFFECTS:
**
**	NONE
*/

void find_and_remove_unnecessary_ID_symbols()
{
	symbol* symbol1, * symbol2;
	sequence* pattern1;
	int status;
	group* ID_symbols_without_any_function = new group;

	list_for(pattern1, sequence, old_patterns)
	{
		list_for(symbol1, symbol, pattern1)
		{
			status = symbol1->get_status();
			if (status == CONTENTS) break;
			if (status != IDENTIFICATION) continue;

			// Keep record of ID symbols that have no function
			// amongst the Old patterns.

			if (symbol1->has_no_function())
				ID_symbols_without_any_function->add_child(symbol1);
		}

		if (ID_symbols_without_any_function->is_empty())
			continue;

		// Now remove symbols from ID_symbols_without_any_function
		// one by one, remove them from pattern1 and delete them.

		while (symbol2 = (symbol*)
			ID_symbols_without_any_function->extract_first_child())
		{
			pattern1->extract_child(symbol2);
			delete symbol2;
		}

		fprintf(output_file, "PATTERN HAS BEEN REDUCED:\n\n");
		pattern1->write_pattern(true, false);
	}

	delete ID_symbols_without_any_function;

} // find_and_remove_unnecessary_ID_symbols

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Compress the patterns in New in terms of patterns in Old (if any).
**
** CALLING SEQUENCE:
**
**	void compress_new_into_old()
**
** FORMAL ARGUMENTS:
**
**	Return value:			void
**
** IMPLICIT INPUTS:
**
**	The corpus
**
** IMPLICIT OUTPUTS:
**
**	Alignments and new patterns
**
** SIDE EFFECTS:
**
**	NONE
**
*/

void compress_new_into_old()
{
	sequence // *copy_of_receptacle_pattern,
		* cnp;
	list_element* el_pos1, * el_pos2;

	fprintf(output_file, "START OF PARSING AND LEARNING\n\n");

	el_pos1 = NIL;
	new_patterns_counter = 0;
	list_for_el_pos(current_new_pattern, sequence, new_patterns, el_pos1)
	{
		new_patterns_counter++;
		fprintf(output_file, "NEXT PATTERN FROM NEW:\n\n");
		current_new_pattern->write_with_details(true);

		// PHASE 1

		phase = 1;

		fprintf(output_file, "%s%d%s",
			"PHASE ",
			phase,
			"\n\n");

		// Prepare a "receptacle_pattern".

		receptacle_pattern = new sequence(BASIC_PATTERN); // Receives 
			// symbols from current_new_pattern, one by one, after 
			// each one has been matched against symbols in Old 
			// (including any symbols in receptacle_pattern).

		old_patterns->add_child(receptacle_pattern);

		// Setting up.

		// Do the current_new_pattern.

		recognition_cycles(current_new_pattern, true);

		// Print out receptacle pattern now that it has been
		// completed. Also, count the number of children in the pattern.

		fprintf(output_file, "%s%d%s",
			"From current_new_pattern ID",
			current_new_pattern->get_ID(),
			",\nthe 'receptacle' pattern stored in Old is:\n\n");

		receptacle_pattern->write_with_details(true);
		receptacle_pattern->count_number_of_children();

		naive_grammar->add_pattern(receptacle_pattern);

		fprintf(output_file, "End of learning: (");
		print_pattern_cycle(false, current_new_pattern);
		fprintf(output_file, ")\n\n");

		old_patterns->write_patterns("OLD PATTERNS AFTER LEARNING AND WITH GENERALISATIONS:", NULL_VALUE,
			NULL_VALUE);
		fprintf(output_file, "OLD PATTERNS WITH DETAILS: (");
		print_pattern_cycle(false, current_new_pattern);
		fprintf(output_file, ")\n\n");
		old_patterns->write_patterns_with_details();

		// PHASE 2

		phase = 2;

		fprintf(output_file, "%s%d%s",
			"PHASE ",
			phase,
			"\n\n");

		fprintf(output_file, "CURRENT PATTERN FROM NEW:\n\n");
		current_new_pattern->write_with_details(true);

		find_and_remove_unnecessary_ID_symbols();

		sequence* al1;

		el_pos2 = NIL;
		list_for_el_pos(cnp, sequence, new_patterns, el_pos2)
		{
			recognition_cycles(cnp, false);

			parsing_alignments->sort_by_ID();

			fprintf(output_file, "%s%d%s",
				"Full alignments for New pattern ID",
				cnp->get_ID(),
				" (");
			print_pattern_cycle(true, cnp);
			fprintf(output_file, ")\n\n");

			parsing_alignments->
				write_patterns("FULL_A", NULL_VALUE, FULL_A);
			parsing_alignments->
				write_patterns("FULL_B", NULL_VALUE, FULL_B);
			parsing_alignments->
				write_patterns("FULL_C", NULL_VALUE, FULL_C);

			// Transfer alignments from parsing_alignments to
			// cumulative_parsing_alignments.

			while (al1 = (sequence*)
				parsing_alignments->extract_first_child())
				cumulative_parsing_alignments->add_child(al1);

			if (cnp == current_new_pattern) break;
		}

		// Using frequencies of patterns and symbols,
		// re-compute codes and encoding costs for 'full' alignments.
		// From this information, compile one or more sets of 'good'
		// grammars describing the New patterns.

		sifting_and_sorting(current_new_pattern);

		fprintf(output_file,
			"All alignments are deleted ready for next New pattern.\n\n");

		selected_cumulative_parsing_alignments->release_children();
		cumulative_parsing_alignments->delete_children();

		fprintf(output_file, "END OF ");
		print_pattern_cycle(false, current_new_pattern);
		fprintf(output_file, "\n\n");

		old_patterns->write_patterns("OLD PATTERNS:", NULL_VALUE,
			NULL_VALUE);

	} // Get next current_new_pattern from New.

} // compress_new_into_old

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Assigns status and types to symbols in corpus, compiles an alphabet
**	of symbols in the corpus and prints out its size,
**	finds frequencies of symbols, computes corresponding
**	information costs and assigns these values to symbols in the
**	corpus.
**
**	This function also scans the corpus looking for pre-assigned
**	IDENTIFICATION symbols. Amongst those that are numeric, it looks
**	for the highest number and then sets the global variable 'context_number'
**	to be one higher. This ensures that new IDENTIFICATION symbols that
**	are assigned by the program have higher numbers than any pre-assigned
**	numeric ID symbols.
**
** CALLING SEQUENCE:
**
**	void process_symbols()
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
** IMPLICIT INPUTS:
**
**	The corpus, original_alphabet_size
**
** IMPLICIT OUTPUTS:
**
**	Sets the value of context_number
**
** SIDE EFFECTS:
**
**	NONE
**
*/

void process_symbols(void)
{
	if (!identification_symbols_marked)
		assign_status_to_symbols();
	else
	{
		// Make sure that left and right brackets at the beginnings
		// and ends of patterns are marked as having
		// BOUNDARY_MARKER status.

		sequence* pattern1;
		symbol* symbol1;

		list_for(pattern1, sequence, new_patterns)
		{
			symbol1 = (symbol*)pattern1->get_first_child();
			if (*(symbol1->get_name()) == '<')
				symbol1->set_status(BOUNDARY_MARKER);
			symbol1 = (symbol*)pattern1->get_last_child();
			if (*(symbol1->get_name()) == '>')
				symbol1->set_status(BOUNDARY_MARKER);
		}

		list_for(pattern1, sequence, old_patterns)
		{
			symbol1 = (symbol*)pattern1->get_first_child();
			if (*(symbol1->get_name()) == '<')
				symbol1->set_status(BOUNDARY_MARKER);
			symbol1 = (symbol*)pattern1->get_last_child();
			if (*(symbol1->get_name()) == '>')
				symbol1->set_status(BOUNDARY_MARKER);
		}
	}

	assign_type_to_symbols();

	original_symbols_in_corpus =
		corpus->compile_alphabet(&original_alphabet_size);
	// This function needs to follow the previous one to ensure 
	// that type and status information is on each symbol in 
	// the alphabet.

	fprintf(output_file, "%s%d%s",
		"Original alphabet size = ",
		original_alphabet_size,
		"\n\n");

	// find_symbol_frequencies(original_symbols_in_corpus, 
	//	true, true, NIL) ; /* Find the frequency of
	//	each symbol type by scanning over the patterns in the corpus
	//	and adding the frequency of the pattern to the frequency of the
	//	symbol type for each occurrence of each symbol in the
	//	pattern in which it occurs. */

	// At the beginning, every New symbol has a frequency of 1,
	// assigned by default when it is created. The system derives
	// frequencies of symbols only from Old patterns created
	// as learning proceeds.

	calculate_and_assign_frequencies_and_costs(original_symbols_in_corpus, true);
	/* From the frequencies of symbol types this
	function computes an information cost for each symbol type
	in the corpus. Calculates average_symbol_type_cost.
	In addition, the function assigns the frequency of its type
	to each symbol in the corpus and the associated cost,
	calculated by the Shannon-Fano-Elias method. */

	// Until some Old patterns have been generated, the symbol types in
	// the Old patterns are to be the same as original_symbols_in_corpus.
	// This will be corrected as learning proceeds.

	symbol* symbol1, * symbol2;
	symbol_types_in_old = new group;
	list_for(symbol1, symbol, original_symbols_in_corpus)
	{
		symbol2 = new symbol(*symbol1);
		symbol_types_in_old->add_child(symbol2);
	}

	// Process the old_patterns looking for numeric IDENTIFICATION symbols.
	// Out of these, find the highest number. Set context_number to be one
	// higher than that.

	sequence* pattern1;
	int highest_number = 0, string_number;
	char* symbol_name;

	list_for(pattern1, sequence, old_patterns)
	{
		list_for(symbol1, symbol, pattern1)
		{
			if (symbol1->get_status() != IDENTIFICATION) continue;
			symbol_name = symbol1->get_name();

			// Check whether all characters are digits.

			if (isnumeric(symbol_name) == false) continue;

			string_number = atoi(symbol_name);
			if (string_number > highest_number)
				highest_number = string_number;
		}
	}

	context_number = highest_number + 1;
	unique_id_number = 1;

} // process_symbols

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Creates new patterns and grammars by parsing.
**
** CALLING SEQUENCE:
**
**	void create_patterns_and_grammars()
**
** FORMAL ARGUMENTS:
**
**	Return value:	void.
**
** IMPLICIT INPUTS:
**
**	NONE
**
** IMPLICIT OUTPUTS:
**
**	NONE
**
** SIDE EFFECTS:
**
**	NONE
**
*/

void create_patterns_and_grammars()
{
	grammar* grammar1;

	// Preliminaries.

	// Set up arrays and clear them.

	sort_array = new int[hit_structure_rows];
	leaf_array = new hit_node * [hit_structure_rows];
	half_list = new hit_node * [half_hit_structure_rows];
	alignments_array = new sequence * [hit_structure_rows];

	int index, row1, col1;

	for (index = 0; index < hit_structure_rows; index++)
		sort_array[index] = NULL_VALUE;

	for (index = 0; index < hit_structure_rows; index++)
		leaf_array[index] = NIL;

	for (index = 0; index < half_hit_structure_rows; index++)
		half_list[index] = NIL;

	for (index = 0; index < hit_structure_rows; index++)
		alignments_array[index] = NIL;

	// Clear hit_node_array[].

	for (index = 0; index < HIT_NODE_ARRAY_SIZE; index++)
	{
		hit_node_array[index].new_hit_node = NIL;
		hit_node_array[index].hn_parent = NIL;
	}

	// Clear symbol_selection_array[][].

	for (row1 = 0; row1 < MAX_NEW_ARRAY_ROWS; row1++)
	{
		for (col1 = 0; col1 < MCOLS; col1++)
			symbol_selection_array[row1][col1] = NIL;
	}

	// Make counts.

	number_of_symbols_in_new =
		new_patterns->count_number_of_symbols(); // Count the
			// number of symbols in New.

	number_of_symbols_in_old =
		old_patterns->count_number_of_symbols(); // Count the
	// number of symbols in Old.

	// Find the numbers of patterns in New and Old.

	number_of_patterns_in_new =
		new_patterns->count_number_of_children();

	original_number_of_patterns_in_old =
		old_patterns->count_number_of_children();

	// Print out the corpus.

	sequence* pattern1;

	fprintf(output_file,
		"THE WHOLE CORPUS (INCL. FREQUENCIES GREATER THAN ONE)\n\n");

	if (new_patterns->is_empty())
		fprintf(output_file, "NEW PATTERNS: None\n\n");
	else
	{
		fprintf(output_file, "NEW PATTERNS:\n\n");
		list_for(pattern1, sequence, new_patterns)
		{
			pattern1->print_ID();
			fprintf(output_file, " ");
			pattern1->write_tree_object(PRINT_ALL_FREQUENCIES);
		}
	}

	if (old_patterns->is_empty())
		fprintf(output_file, "OLD PATTERNS: None\n\n");
	else old_patterns->write_patterns("OLD PATTERNS:", NULL_VALUE,
		NULL_VALUE);

	process_symbols();

	// Compute the size of the corpus in bits.

	double size_of_corpus_at_start = corpus->total_of_symbol_sizes();

	fprintf(output_file, "%s%1.2g%s",
		"Size of corpus at start: ",
		size_of_corpus_at_start,
		"\n\n");

	// Now find EC for patterns in Old.

	list_for(pattern1, sequence, old_patterns)
		pattern1->compute_costs();

	// Notice that the following block must come after the function
	// assign_status_to_symbols() (called in process_symbols(), above)
	// because the former depends on the original order of the symbols.

	write_new_and_old();

	/* Compress the corpus. */

	compress_new_into_old(); // This is where the real work is done.

	delete[] sort_array;
	delete[] leaf_array;
	delete[] half_list;
	delete[] alignments_array;

	fprintf(output_file, "%s%1.2f%s%1.2f%s",
		"Size of corpus at start = ",
		size_of_corpus_at_start,
		" bits.\n\nSize of best grammar = ",
		size_of_best_grammar,
		" bits.\n\n");

	grammar1 = (grammar*)set_of_grammars->get_first_child();
	if (grammar1 == NIL)
		fprintf(output_file, "No grammar produced in repeat_cpas().\n\n");
	else
	{
		fprintf(output_file, "NAIVE GRAMMAR:\n\n");

		naive_grammar->write_grammar(false, false);
	}

	final_number_of_patterns_in_old =
		old_patterns->count_number_of_children();

	/* Now write out statistics */

	fprintf(output_file, "STATISTICS\n\n");
	fprintf(output_file, "%s%d%s%d%s%d%s%d%s",
		"Original size of alphabet = ", original_alphabet_size,
		"\n\nFinal size of alphabet = ", current_alphabet_size,
		"\n\nOriginal number of patterns in Old = ",
		original_number_of_patterns_in_old,
		"\n\nFinal number of patterns in Old = ",
		final_number_of_patterns_in_old,
		"\n\n");

} // create_patterns_and_grammars

/*********************************************************************/

/* The main part of SP */

/*
** FUNCTIONAL DESCRIPTION:
**
**	Entry point to the SP system.
**
** CALLING SEQUENCE:
**
**	void SP_main()
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
** IMPLICIT INPUTS:
**
**	NONE
**
** IMPLICIT OUTPUTS:
**
**	'output_file', 'latex_file'
**
** SIDE EFFECTS:
**
**	If the session logfile cannot be successfully opened, the function
**	exits with an error code.
**	As described in `show_menu', the order of options displayed in
**	that function is implied here.	If the order in `show_menu' were
**	to change then the `switch (selection)' case labels would have to
**	be adjusted accordingly. Selecting menu option 1 (edit) has no effect.
*/

void SP_main()
{
	int i;

	if ((input_file = fopen(in_filename, "r")) == NIL)
		abort_run("Fatal: Unable to open input_file\n");
	if ((parameters_file = fopen(parameters_filename, "r")) == NIL)
		abort_run("Unable to open parameters file.\n");
	if ((output_file = fopen(out_filename, "w")) == NIL)
		abort_run("Unable to open write file.\n");
	if ((latex_file = fopen(latex_filename, "w")) == NIL)
		abort_run("Fatal: Unable to open latex_file\n");
	if ((plot_file = fopen(plot_filename, "w")) == NIL)
		abort_run("Fatal: Unable to open plot_file\n");

	fprintf(output_file, "%s%s%s",
		"SP71 version ", VERSION, ". Copyright (C) ");
	year();
	fprintf(output_file, "%s%s%s%s%s%s", " J G Wolff.\n",
		"This program is released to the public domain without\n",
		"any restrictions. You can redistribute it and/or modify\n",
		"it as you wish. Please retain this notice on all\n",
		"versions and acknowledge J G Wolff as the originator\n",
		"of the program in all publications.\n\n");

	fprintf(output_file, "%s%s%s%s%s",
		"RESULTS FROM ", PROGRAM, ", VERSION ", VERSION, "\n\n");

	time_now();

	fprintf(output_file, "%s%s%s",
		"INPUT FILE: ", in_filename, "\n\n");

	// Read parameters.

	read_parameters();

	half_hit_structure_rows = hit_structure_rows / 2;

	// Print out parameters.

	fprintf(output_file, "PARAMETERS:\n\n");

	fprintf(output_file, "%s%d%s",
		"PROBABILITIES = ", probabilities, "\n\n");
	fprintf(output_file, "%s%d%s",
		"SHOW_AL_STRUCTURE = ", show_al_structure, "\n\n");
	fprintf(output_file, "%s%d%s",
		"SHOW_HIT_STRUCTURE = ", show_hit_structure, "\n\n");
	fprintf(output_file, "%s%d%s",
		"SHOW_UNSELECTED_ALIGNMENTS = ",
		show_unselected_alignments, "\n\n");
	fprintf(output_file, "%s%d%s",
		"VERBOSE = ",
		verbose, "\n\n");
	fprintf(output_file, "%s%d%s",
		"USE_ABBREVIATIONS = ",
		use_abbreviations, "\n\n");
	fprintf(output_file, "%s%d%s",
		"SHOW_ALL_PARSING_ALIGNMENTS = ",
		show_all_parsing_alignments, "\n\n");
	fprintf(output_file, "%s%d%s",
		"ID_C_SYMBOL_CONSTRAINT = ",
		id_c_symbol_constraint, "\n\n");
	fprintf(output_file, "%s%c%s",
		"ALIGNMENT_FORMAT = ",
		alignment_format, "\n\n");
	fprintf(output_file, "%s%d%s",
		"HIT_STRUCTURE_ROWS = ", hit_structure_rows, "\n\n");
	fprintf(output_file, "%s%d%s",
		"FAIL_SCORE = ", fail_score, "\n\n");
	fprintf(output_file, "%s%1.2f%s",
		"COST_FACTOR = ", cost_factor, "\n\n");
	fprintf(output_file, "%s%d%s",
		"KEEP_ROWS = ", keep_rows, "\n\n");
	fprintf(output_file, "%s%d%s",
		"FULL_ALIGNMENT_LIMIT = ",
		full_alignment_limit, "\n\n");
	fprintf(output_file, "%s%d%s",
		"GRAMMAR_LIMIT = ",
		grammar_limit, "\n\n");
	fprintf(output_file, "%s%d%s",
		"EXTRACTION_LIMIT = ",
		extraction_limit, "\n\n");
	fprintf(output_file, "%s%s%s",
		"FIGURE_ID = ",
		figure_ID, "\n\n");
	fprintf(output_file, "%s%d%s",
		"COMBINATION_LIMIT = ",
		combination_limit, "\n\n");
	fprintf(output_file, "%s%d%s",
		"MAX_OLD_GAP = ",
		max_old_gap, "\n\n");
	fprintf(output_file, "%s%d%s",
		"MAX_DRIVING_GAP = ",
		max_driving_gap, "\n\n");
	fprintf(output_file, "%s%d%s",
		"MAX_TARGET_GAP = ",
		max_target_gap, "\n\n");
	fprintf(output_file, "%s%d%s",
		"MAX_UNSUPPORTED_CYCLES = ",
		max_unsupported_cycles, "\n\n");
	fprintf(output_file, "%s%d%s",
		"MAX_ALIGNMENTS_IN_ONE_CYCLE = ",
		max_alignments_in_one_cycle, "\n\n");
	fprintf(output_file, "%s%d%s",
		"ORIENTATION = ",
		orientation, "\n\n");
	fprintf(output_file, "%s%d%s",
		"WRITE_AL_CHARS_LENGTH = ",
		write_al_chars_length, "\n\n");
	fprintf(output_file, "%s%d%s",
		"WRITE_SECTION_CHARS_LENGTH = ",
		write_section_chars_length, "\n\n");
	fprintf(output_file, "%s%1.2f%s",
		"MINIMUM_FONT_SIZE = ",
		minimum_font_height, "\n\n");
	fprintf(output_file, "%s%1.2f%s",
		"MAXIMUM_FONT_SIZE = ",
		maximum_font_height, "\n\n");
	fprintf(output_file, "%s%d%s",
		"N_GRAMMARS = ",
		n_grammars, "\n\n");
	fprintf(output_file, "%s%d%s",
		"CREATE_PATTERNS_DIAGNOSTIC = ",
		create_patterns_diagnostic, "\n\n");

	/* Load SP structure */

	load();

	/* The first object in the corpus is taken to be 'New' */

	corpus->initialise();
	new_patterns = (group*)corpus->get_next_child();
	if (new_patterns == NIL) abort_run("main(): invalid New object");

	old_patterns = (group*)corpus->get_next_child();
	if (old_patterns == NIL) abort_run("main(): invalid Old object");

	// Internal constants not set in input file.

	fprintf(output_file, "%s%d%s",
		"MAX_ALIGNMENT_LENGTH = ",
		max_alignment_length, "\n\n");

	fflush(output_file);

	if (fe_key_array == 0 && use_abbreviations != OFF) // fe_key_array
		// is used to detect whether a key to the abbreviations 
		// of symbols has been supplied or not.
	{
		if (use_abbreviations == LETTERS)
			make_letter_abbreviations();

		// if (use_abbreviations == DIGITS) 
		//	make_digit_abbreviations() ; Development of the
		// use of digit abbreviations has been halted.
	}

	write_key(); // Writes out the key to abbreviations of symbols
	// used in the corpus.

	// Make global objects and arrays.

	hn_master = new hit_node;
	hit_root = new hit_node;
	parsing_alignments = new group;
	cumulative_parsing_alignments = new group;
	selected_cumulative_parsing_alignments = new group;
	symbols_in_new = new_patterns->symbol_set();
	set_of_grammars = new group;
	naive_grammar = new grammar;
	brackets_list = new group;
	set_of_combinations = new group;
	best_combinations = new group;
	created_patterns = new group;
	generalisation_list1 = new group;
	generalisation_list2 = new group;

	write_al_chars = new char* [MAX_ALIGNMENT_DEPTH];
	write_al_vacant_slots = new char* [MAX_ALIGNMENT_DEPTH];

	for (i = 0; i < MAX_ALIGNMENT_DEPTH; i++)
	{
		write_al_chars[i] = new char[write_al_chars_length];
		write_al_vacant_slots[i] = new char[write_al_chars_length];
	}

	// Put headings into plot_file.

	fprintf(plot_file, "%s%s%s%s%s%s%s%s%s%s",
		"new pattern, ",
		"G, ",
		"E, ",
		"G + E, ",
		"G (naive), ",
		"E (naive), ",
		"G + E (naive), ",
		"O, ",
		"(G + E) / O",
		"\n");

	create_patterns_and_grammars();

	exit_routine(0);
} /* main */