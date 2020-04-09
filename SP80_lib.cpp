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

/*
** TITLE:		SP71_lib.cpp
**
** VERSION: 	SP71, v 9.0
**
** ABSTRACT:	General purpose methods and functions for the SP system.
**
** ENVIRONMENT: The file "SP71_head.h" must be on the `include'
**				path at compile time.
**
** AUTHOR:		Gerry Wolff
**
** MODIFIED BY:
**
** SP71
**
** 1.0 JGW 18/6/03 -	Derived from SP70, v 9.2.
** 1.1 JGW 12/7/03 -	See SP71_comp.cpp.
** 1.2 JGW 20/10/03 -	ditto.
** 1.3 JGW 27/10/03 -	ditto.
** 1.4 JGW 3/11/03 -	ditto.
** 1.5 JGW 9/11/03 -	ditto.
** 1.6 JGW 10/11/03 -	ditto.
** 1.7 JGW 11/11/03 -	ditto.
** 1.8 JGW 5/12/03 -	ditto.
** 1.9 JGW 8/12/03 -	ditto.
** 1.10 JGW 9/12/03 -	ditto.
** 2.0 JGW 9/12/03 -	ditto.
** 2.1 JGW 12/12/03 -	ditto.
** 2.2 JGW 22/1/04 -	ditto.
** 2.3 JGW 22/1/04 -	ditto.
** 2.4 JGW 23/1/04 -	ditto.
** 2.5 JGW 28/1/04 -	ditto.
** 3.0 JGW 13/5/04 -	ditto.
** 4.1 JGW 2/6/04 -		ditto.
** 5.0 JGW 7/6/04 -		ditto.
** 5.1 JGW 11/6/04 -	ditto.
** 5.2 JGW 12/6/04 -	ditto.
** 5.3 JGW 15/6/04 -	ditto.
** 5.4 JGW 15/6/04 -	ditto.
** 5.5 JGW 21/6/04 -	ditto.
** 5.6 JGW 25/6/04 -	ditto.
** 6.0 JGW 23/7/04 -	ditto.
** 6.1 JGW 28/7/04 -	ditto.
** 6.2 JGW 30/7/04 -	ditto.
** 6.3 JGW 3/8/04 -		ditto.
** 7.0 JGW 5/8/04 -		ditto.
** 7.1 JGW 10/8/04 -	ditto.
** 7.2 JGW 6/12/04 -	ditto.
** 7.3 JGW 17/12/04 -	ditto.
** 7.4 JGW 20/12/04 -	ditto.
** 7.5 JGW 28/12/04 -	ditto.
** 7.6 JGW 5/1/05 -		ditto.
** 7.7 JGW 10/1/05 -	ditto.
** 7.8 JGW 12/1/05 -	ditto.
** 7.9 JGW 25/1/05 -	ditto.
** 7.10 JGW 31/1/05 -	ditto.
** 7.12 JGW 31/1/05 -	ditto.
** 7.13 JGW 21/2/05 -	ditto.
** 8.0 JGW 1/3/05 -		ditto.
** 8.1 JGW 17/3/05 -	ditto.
** 8.2 JGW 22/3/05 -	ditto.
** 9.0 JGW 30/3/05 -	ditto.
*/

#include "SP71_head.h"

#define MAKE_DIGIT_ABBREVIATIONS FALSE

/* PARAMETERS. */

/* Optional, ON or OFF. */

bool probabilities; /* Calculations of probabilities at the end
	of each program run. */
bool show_al_structure; // If true, the structure of each column of each
	// alignment is printed out, otherwise they are not printed.
bool show_hit_structure; // If true, the hit structure is printed,
	// otherwise it is not printed.
bool show_unselected_alignments; /* If ON, all the alignments which are
	formed are shown before selection (as well as showing the
	alignments which are selected. */
bool verbose; // If ON, lots of messages are printed. Otherwise,
	// minimal messages are printed.
int use_abbreviations; /* When LETTERS, alignments are printed
	using abbreviated names of symbols in alphabetic form (any symbol which
	starts with a digit is left as it is). When DIGITS, symbols are
	abbreviated as digits (which may include any trailing or leading '#'
	symbol which appears in the original symbol). When OFF, no abbreviations
	are used. The development of digit abbreviations has been halted. */
int show_all_parsing_alignments;  /* If ON, all alignments produced by
	by compressing New against Old are shown. If OFF, only the best
	ones for any given pattern from New are shown. */
int id_c_symbol_constraint; /* If this is ON, ID-symbols can only ever be matched
	with C-symbols and vice versa. If the parameter is OFF, the constraint is
	not applied. */

	/* Other Parameters. */

char alignment_format; /* The format of alignments ('V' for vertical
	and 'H' for horizontal) */

int hit_structure_rows; /* The number of rows in the
	sort_array[] and leaf_array[]. */

int fail_score; /* Any compression score which is equal to this or
	smaller counts as a fail. */

double cost_factor; /* The multiplier to calculate the bit_cost of
	each data symbol from the minimum cost needed to discriminate it
	from other symbols. */

double average_symbol_type_cost;
double average_CONTEXT_SYMBOL_type_cost; // Includes UNIQUE_ID_SYMBOLs.

int keep_rows; /* Controls the number of rows in the
	symbol_selection_array[][] which are used to determine which
	sequences are *not* purged at the end of each cycle. */

int driving_keep_rows; /* The best driving_keep_rows are selected to
						be driving patterns for the next cycle.
						NOT CURRENTLY USED. */

int full_alignment_limit; /* This limits the number of alternative
	alignments (for a given pattern from New) that are considered
	during the compiling of alternative grammars. */

int grammar_limit; /* This limits the number of alternative grammars
	that may be developed at any one time (in
	compile_alternative_grammars()) */

int extraction_limit; /* This limits the number of hit sequences for a given
	a given driving pattern that are used for learning. */

int not_used; /* Not used. */

char figure_ID[20]; /* Records the id for the data, for printing
	out in the latex_file. */

int combination_limit; /* The number of combinations to be selected
	on each phase of combine_alignments(). */

int max_old_gap; /* Sets the maximum permitted gap between hit symbols
	in any original old pattern (not an alignment formed during
	current processing). */

int max_driving_gap; /* Sets the maximum permitted gap between hit symbols
	in any driving pattern. */

int max_target_gap; /* Sets the maximum permitted gap between hit symbols
	in any target pattern. */

int max_unsupported_cycles; /* For any one pattern from New, this parameter
	sets the maximum number of cycles which build alignments without
	any gain in maximum compression score compared with the best
	cycle for the current pattern from New. */

int max_alignments_in_one_cycle; /* This can be used to limit the number
	of alignments formed in one cycle. It is applied as alignments are
	formed, in order of the approximate compression scores calculated for
	the hit structure. If this limit is applied, there is a possibility
	of cutting short the alignments before the best one has been made. */

int orientation; /* Orientation of the output may be specified as
	LANDSCAPE or PORTRAIT. */

int write_al_chars_length; // The maximum size of any alignment when written
	// out. Any excess is truncated with ellipses.

int write_section_chars_length; // The maximum size of a section of
	// any alignment when written out.

double minimum_font_height; /* The smallest font to be used in printing
	alignments. */

double maximum_font_height; /* The largest font to be used in printing
	alignments. */

int n_grammars; /* This is the number of grammars, over and obove the
	'naive' grammar, which provide a set of Old patterns after
	each New pattern has been processed. */

int create_patterns_diagnostic; /* When ON, prints out diagnostic
	information for sequence::create_patterns() methods. Otherwise,
	no diagnostic information is printed. */

bool identification_symbols_marked; // This is set by examining the symbols in the
	// corpus. If any of them have a '!' prefix, then IDENTIFICATION
	// symbols are set in the input by the use of this mark
	// and CONTENTS symbols are those without it.

/* End of Parameters. */

char* syntax_buffer;
char* syn_buf_ptr;

int context_number;
int unique_id_number;
int number_of_alignments_purged;
int total_number_of_alignments;
int number_of_alignments_retained_this_cycle;
int new_hits; /* Used in update_hit_structure() and in add_hit(). */
int driving_span;
int target_span;
int span;
int number_of_parsing_alignments;
int best_cycle_this_new_pattern; // For a given pattern from New,
	// this is the cycle in which the best score for the
	// pattern was obtained.
int number_of_symbols_in_new, number_of_symbols_in_old;
int number_of_patterns_in_new, original_number_of_patterns_in_old,
final_number_of_patterns_in_old;

group* brackets_list; // Used in compare_patterns().
group* created_patterns; // Used in sequence::created_new_patterns().
group* generalisation_list1; // Used in generalise_patterns().
group* generalisation_list2; // ditto.

const double log_10_2 = log10(2);

clock_t time1;
clock_t time2;
double old_symbol_bit_cost, adjusted_symbol_cost;

sequence* best_alignment_this_new_pattern; // The alignment giving
	// the best compression score in the current pattern from New.

/*** ARRAYS ETC USED IN WRITE_ALIGNMENT ***/

double font_heights[FONT_SET_SIZE] =
{ 7, 8, 9, 10, 11, 12, 14, 17.28, 20.74, 24.88 };

struct key_entry key_array[KEY_ARRAY_SIZE]; // Stores the key to the abbreviations of
// symbols in the input file.

int fe_key_array;

const int max_alignment_depth = 50; // Determines the maximum
	// depth of an alignment, measured in rows. (Currently not
	// read in and not used).
const int max_alignment_length = 200; // Determines the maximum length of
	// an alignment, measured in symbols. (Currently not
	// read in but it is used).

const int write_alignment_char_rows = MAX_ALIGNMENT_DEPTH * 2; /* The maximum
	depth of an sequence that can be written (characters). */

char** write_al_chars;
char** write_al_vacant_slots;

struct row_entry row_array[MAX_ALIGNMENT_DEPTH]; // Used to keep track of
	// symbols and positions for each row in
	// fill_write_al_chars.

char complete_symbol[MEDIUM_SCRATCH_ARRAY_SIZE],
new_abbreviation[MEDIUM_SCRATCH_ARRAY_SIZE]; // Used in
// make_letter_abbreviations.

bool al1_rows[MEDIUM_SCRATCH_ARRAY_SIZE];

struct hit_entry
{
	hit_node* h_node;
	symbol* newly_created_col;
} hit_entries[MEDIUM_SCRATCH_ARRAY_SIZE]; // Used in unify_best.

int fe_hit_entries;

sequence* symbol_selection_array[MAX_NEW_ARRAY_ROWS][MCOLS]; /* Each
	symbol corresponding to a symbol in New (in new_OAO_symbols[])
	receives zero or more pointers to sequences which contains that
	symbol. At the end of each cycle, The array is used for selection of
	target sequences for the following cycle. */

sequence* receptacle_pattern;

hit_node** half_list; // Used to store the worst 50% of the leaf nodes in the
	// leaf_array[].

int fe_half_list;

struct hn_entry hit_node_array[HIT_NODE_ARRAY_SIZE];

int fe_hit_node_array;

struct best_alignment_entry
{
	sequence* al1;
	group* contained_in; /* List of the other best_alignments
	which contain 'alignment'. */
	double rel_P;
	bool done;
} best_alignments[MEDIUM_SCRATCH_ARRAY_SIZE]; /* Stores a set of
	alignments, including best_alignment, which code the
	same symbols from New as best_alignment. It also stores
	the abs_P and rel_P values calculated for each of
	these alignments. Used in probabilities_of_inferences(). */

int fe_best_alignments;

bool driving_brackets_array[MEDIUM_SCRATCH_ARRAY_SIZE];
bool target_brackets_array[MEDIUM_SCRATCH_ARRAY_SIZE];

struct names_entry
{
	char* old_name_number_chars;
	int new_name_number;
} *names_array;

group* set_of_symbols;

symbol* dr_array[MEDIUM_SCRATCH_ARRAY_SIZE], // Used in extract_patterns().
* t_array[MEDIUM_SCRATCH_ARRAY_SIZE];

int fe_dr_array, fe_t_array;

struct ref_struct
{
	sequence* target_pattern, * driving_pattern;
	symbol* left_t_bracket, * t_class_symbol, * right_t_bracket,
		* left_dr_bracket, * dr_class_symbol, * right_dr_bracket;
	list_element* t_el_pos, * dr_el_pos;
	int left_t_bracket_int_pos, t_class_symbol_int_pos,
		right_t_bracket_int_pos, left_dr_bracket_int_pos,
		dr_class_symbol_int_pos, right_dr_bracket_int_pos;
} reference_structure;

/*** Mirror fields for hn_master ***/

int master_driving_int_pos;
sequence* master_driving_pattern;
symbol* master_driving_symbol;
int master_target_int_pos;
sequence* master_target_pattern;
symbol* master_target_symbol;

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Checks whether or not a character string is numeric.
**
** CALLING SEQUENCE:
**
**	bool isnumeric(char *s)
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if the string is numeric, false otherwise.
**
**	s:				The character string to be tested.
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

bool isnumeric(char* s)
{
	char* c;
	bool string_is_numeric = true;

	for (c = s; *c != '\0'; c++)
	{
		if (isdigit(*c) == false)
		{
			string_is_numeric = false;
			break;
		}
	}

	return(string_is_numeric);

} // isnumeric

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Finds zero or more 'good' combinations of alignments in any given
**	cycle of the alignment-building process and adds them to
**	parsing_alignments.
**
**	The alignments that are combined are 'full' alignments in the sense
**	that all their Old CONTENTS symbols must be matched but the New
**	symbols are not necessarily all matched (if they were, then no
**	combination could be made as described next).
**
**	Alignments can be combined if, for any two alignments, none of the New
**	symbols between the first New symbol in alignment A and the last New
**	symbol in alignment A falls between the first New symbol in alignment
**	B and the last New symbol in alignment B - and vice versa. However,
**	there may be one or more unmatched New symbols between the last New
**	symbol of the left alignment and the first New symbol of the
**	right alignment.
**
**	Because the alignments in combine_al_array[] are in descending
**	order of compression difference, the simplifying assumption is
**	made that good combinations of alignments can be found by
**	adding alignments from successive entries in the array rather
**	than considering all possible combinations of alignments. Because
**	combinations are formed by adding alignments in this way, it
**	is assumed that combinations containing n + 1 alignments are
**	always better than combinations containing n alignments.
**
** CALLING SEQUENCE:
**
**	void combine_alignments(sequence *pattern1)
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	pattern1:		The pattern being parsed.
**
** IMPLICIT INPUTS:
**
**	alignments in parsing_alignments.
**
** IMPLICIT OUTPUTS:
**
**	Alignments added to parsing_alignments.
**
** SIDE EFFECTS:
**
**	NONE
*/

void combine_alignments(sequence* pattern1)
{
	struct combine_al_record
	{
		sequence* alignment;
		int new_int_pos_first, new_int_pos_last;
	} *combine_al_array;

	int number_of_combinable_alignments;
	group* current_set_of_combinations = new group;

	sequence* al1;

	parsing_alignments->sort_by_compression_difference();

	// Compile information about each full (B) alignment that is needed
	// for the creation of combinations.

	number_of_combinable_alignments = 0;
	list_for(al1, sequence, parsing_alignments)
	{
		if (al1->get_degree_of_matching() != FULL_B) continue;
		if (al1->get_row_pattern(0) != pattern1) continue;
		number_of_combinable_alignments++;
	}

	int counter, new_int_pos;
	int new_int_pos_first = 0;
	int new_int_pos_last = 0;
	symbol* symbol1, * symbol2;

	combine_al_array =
		new struct combine_al_record[number_of_combinable_alignments];

	counter = 0;
	list_for(al1, sequence, parsing_alignments)
	{
		if (al1->get_degree_of_matching() != FULL_B) continue;
		if (al1->get_row_pattern(0) != pattern1) continue;
		new_int_pos_first = NULL_VALUE;
		combine_al_array[counter].alignment = al1;
		list_for(symbol1, symbol, al1)
		{
			symbol2 = symbol1->get_row_symbol(0);
			if (symbol2 == NIL) continue;
			new_int_pos = symbol1->get_row_orig_int_pos(0);
			if (new_int_pos_first == NULL_VALUE)
				new_int_pos_first = new_int_pos;
			new_int_pos_last = new_int_pos;
		}
		combine_al_array[counter].new_int_pos_first = new_int_pos_first;
		combine_al_array[counter].new_int_pos_last = new_int_pos_last;
		counter++;
	}

	// Now combine alignments, in descending order of CD score and growing
	// each combination as large as possible. Any alignment that does not
	// fit into the current series is used to start a new series.

	int i, j, length_pattern1 = pattern1->get_number_of_children();
	combination* comb1, * comb2;
	bool second_series_started;
	for (i = 0; i < number_of_combinable_alignments; i++)
	{
		new_int_pos_first = combine_al_array[i].new_int_pos_first;
		new_int_pos_last = combine_al_array[i].new_int_pos_last;
		al1 = combine_al_array[i].alignment;
		comb1 = new combination(length_pattern1);
		comb1->add_sub_alignment(al1,
			new_int_pos_first, new_int_pos_last, pattern1);
		second_series_started = false;
		for (j = 0; j < number_of_combinable_alignments; j++)
		{
			if (j == i) continue;
			new_int_pos_first = combine_al_array[j].new_int_pos_first;
			new_int_pos_last = combine_al_array[j].new_int_pos_last;
			al1 = combine_al_array[j].alignment;
			if (comb1->can_accept(new_int_pos_first, new_int_pos_last))
				comb1->add_sub_alignment(al1, new_int_pos_first,
					new_int_pos_last, pattern1);
		}

		if (comb1->number_of_alignments() > 1)
		{
			// Check to make sure that the alignments in comb1 are not
			// simply a subset of any combinations in
			// current_set_of_combinations or set_of_combinations
			// (from previous cycles). If it is, delete comb1,
			// otherwise add it to current_set_of_combinations.

			list_for(comb2, combination, current_set_of_combinations)
			{
				if (comb1->is_subset_of(comb2))
				{
#if DIAGNOSTIC5

					fprintf(output_file, "%s%d%s%d%s",
						"Combination C_ID",
						comb1->get_C_ID(),
						" is a subset of, or the same as, combination C_ID",
						comb2->get_C_ID(),
						" and is deleted.\n\n");

#endif

					delete comb1;
					goto L1;
				}
			}

			list_for(comb2, combination, set_of_combinations)
			{
				if (comb1->is_subset_of(comb2))
				{
#if DIAGNOSTIC5

					fprintf(output_file, "%s%d%s%d%s",
						"Combination C_ID",
						comb1->get_C_ID(),
						" is a subset of, or the same as, combination C_ID",
						comb2->get_C_ID(),
						" and is deleted.\n\n");

#endif

					delete comb1;
					goto L1;
				}
			}

			current_set_of_combinations->add_child(comb1);
		}
		else
		{
#if DIAGNOSTIC5

			fprintf(output_file, "%s%d%s",
				"Combination C_ID",
				comb1->get_C_ID(),
				" contains only one alignment and is deleted.\n\n");

#endif

			delete comb1;
		}
	L1:;
	}

	// Find the best combinations up to combination_limit, print them
	// out and make them into composite alignments. At the same time,
	// add the combination to set_of_combinations.

	fprintf(output_file, "Start of combining alignments.\n(");
	print_pattern_cycle(true, pattern1);
	fprintf(output_file, ")\n\n");

	double best_score, temp_score;
	combination* best_comb = { NIL };
	int total_number_of_combinations =
		current_set_of_combinations->count_number_of_children();

	fprintf(output_file, "%s%d%s",
		"Total number of combinations = ",
		total_number_of_combinations,
		"\n\n");

	sequence* composite_alignment;
	counter = 0;
	while (counter < combination_limit)
	{
		best_score = NULL_VALUE;
		list_for(comb1, combination, current_set_of_combinations)
		{
			temp_score = comb1->get_combination_score();
			if (temp_score > best_score)
			{
				best_comb = comb1;
				best_score = temp_score;
			}
		}

		if (best_score > NULL_VALUE)
		{
			current_set_of_combinations->extract_child(best_comb);
			best_comb->print_combination();
			composite_alignment =
				best_comb->make_composite_alignment(pattern1);
			composite_alignment->find_degree_of_matching(pattern1);
			parsing_alignments->add_child(composite_alignment);
			set_of_combinations->add_child(best_comb);
		}
		else break;
		counter++;
	}

	fprintf(output_file, "End of combining alignments.\n(");
	print_pattern_cycle(true, pattern1);
	fprintf(output_file, ")\n\n");

	// Delete arrays and temporary objects.

	current_set_of_combinations->delete_children();
	delete current_set_of_combinations;
	delete[] combine_al_array;

} // combine_alignments

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	This function deletes all the Old patterns and replaces them
**	with the patterns in the best n_grammars.
**
** CALLING SEQUENCE:
**
**	void purge_old_patterns(int n_grammars, sequence *cnp)
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	n_grammars:		The number of grammars to be preserved within the
**					set of Old patterns.
**	cnp:			Current New pattern.
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

void purge_old_patterns(int n_grammars, sequence* cnp)
{
	fprintf(output_file, "Start of purge_old_patterns()\n\n");

	if (set_of_grammars->is_empty())
	{
		fprintf(output_file,
			"There is no best grammar so no patterns to be purged.\n\n");
		return;
	}

	int counter = 0;
	grammar* grammar1,
		* old_grammar = new grammar; // This is a temporary vehicle
		// for the patterns in a new version of old_patterns. The
		// reason a grammar is used is that this allows access to
		// the methods clean_up(), tidy_up_code_symbols() and
		// merge_patterns().
	sequence* pattern1;
	bool pattern_added;

	list_for(grammar1, grammar, set_of_grammars)
	{
		for (pattern1 = grammar1->get_first_pattern();
			pattern1 != NIL; pattern1 =
			grammar1->get_next_pattern())
		{
			// pattern2 = new sequence(*pattern1) ;
			pattern_added = old_grammar->add_pattern(pattern1);
			// if (pattern_added == false)
			//	delete pattern2 ;
		}
		if (++counter >= n_grammars) break;
	}

	// Add patterns from naive grammar.

	for (pattern1 = naive_grammar->get_first_pattern();
		pattern1 != NIL; pattern1 =
		naive_grammar->get_next_pattern())
	{
		pattern_added = old_grammar->add_pattern(pattern1);
	}

	fprintf(output_file, "%s%d%s%d%s",
		"New grammar created by combining of best ",
		n_grammars,
		" original grammars and naive grammar is GR",
		old_grammar->get_ID(),
		"\n\n");

	old_grammar->write_grammar(true, false);

	// Now clean up unnecessary code symbols, [merge patterns where
	// possible] and [renumber code symbols in a tidy manner].

	old_grammar->clean_up(NULL_VALUE);

	// old_grammar->merge_patterns() ;

	// old_grammar->tidy_up_code_symbols() ;

	// Now transfer the patterns in old_grammar to
	// the (empty) old_patterns. All the patterns that are
	// currently in old_patterns are removed and those that
	// are not in old_grammar are deleted.

	while (pattern1 = (sequence*)old_patterns->extract_first_child())
	{
		if (old_grammar->contains(pattern1) == false)
			delete pattern1;
	}

	while (pattern1 = old_grammar->extract_first_pattern())
		old_patterns->add_child(pattern1);

	fprintf(output_file, "%s%d%s",
		"OLD PATTERNS DERIVED FROM THE BEST ",
		n_grammars,
		" GRAMMARS AT THE END OF ");
	print_pattern_cycle(false, cnp);
	fprintf(output_file, ":\n\n");

	if (old_patterns->is_empty())
		fprintf(output_file, "old_patterns is empty.\n\n");
	else
	{
		list_for(pattern1, sequence, old_patterns)
		{
			pattern1->print_ID();
			fprintf(output_file, " ");
			pattern1->write_tree_object(PRINT_SEQUENCE_FREQUENCY);
		}
	}

	fprintf(output_file, "End of purge_old_patterns()\n\n");
} // purge_old_patterns

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Compares the list of IDENTIFICATION symbols in two patterns (excluding
**	bracket symbols) and returns values to show whether the list is
**	the same in the two patterns, or the first list is a subset
**	of the other, or vice versa, or neither list is a subset of the other.
**
** CALLING SEQUENCE:
**
**	int compare_classes(sequence *pattern1, int p1_classes,
**		sequence *pattern2, int p2_classes)
**
** FORMAL ARGUMENTS:
**
**	Return value:	A_DIFFERENT_FROM_B if the pattern1 list of
**					IDENTIFICATION symbols is not a subset of the
**					pattern2 list and vice versa, A_SUBSET_B if the
**					pattern1 list is a subset of the pattern2 list,
**					B_SUBSET_A if the pattern2 list is a subset of
**					the pattern1 list, and A_SAME_AS_B if the two
**					lists are the same (not necessarily in the same
**					order).
**	pattern1:		One of the two patterns.
**	p1_classes:		The number of non-bracket IDENTIFICATION symbols
**					in pattern1.
**	pattern2:		The second of the two patterns.
**	p2_classes:		The number of non-bracket IDENTIFICATION symbols
**					in pattern2.
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

int compare_classes(sequence* pattern1, int p1_classes,
	sequence* pattern2, int p2_classes)
{
	symbol* symbol1, * symbol2;
	int symbol_type, fe_class_array = 0, i;
	struct class_entry
	{
		symbol* class1, * class2;
	} class_array[MEDIUM_SCRATCH_ARRAY_SIZE];

	if (pattern1 == pattern2)
		abort_run("Anomalous input in compare_classes()");

	list_for(symbol1, symbol, pattern1)
	{
		symbol_type = symbol1->get_type();
		if (symbol_type == LEFT_BRACKET) continue;
		if (symbol_type == CONTENTS) break;

		for (i = 0; i < fe_class_array; i++)
			if (symbol1 == class_array[i].class1)
				goto L1;

		list_for(symbol2, symbol, pattern2)
		{
			symbol_type = symbol2->get_type();
			if (symbol_type == LEFT_BRACKET) continue;
			if (symbol_type == CONTENTS) break;

			for (i = 0; i < fe_class_array; i++)
				if (symbol2 == class_array[i].class2)
					goto L2;

			if (symbol1->name_matches(symbol2))
			{
				class_array[fe_class_array].class1 = symbol1;
				class_array[fe_class_array].class2 = symbol2;
				plus_one(&fe_class_array, MEDIUM_SCRATCH_ARRAY_SIZE,
					"Overflow of class_array[] in compare_classes().");
				break;
			}

		L2:;
		}

	L1:;
	}

	if (p1_classes == fe_class_array)
	{
		if (p2_classes == fe_class_array)
			return(A_SAME_AS_B);
		else if (p2_classes > fe_class_array)
			return(A_SUBSET_B);
	}
	else if (p2_classes == fe_class_array)
	{
		if (p1_classes > fe_class_array)
			return(B_SUBSET_A);
	}
	else return(A_DIFFERENT_FROM_B);

} // compare_classes

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Checks whether two patterns belong in the same class.
**
** CALLING SEQUENCE:
**
**	symbol *same_class(sequence *pattern1, sequence *pattern2)
**
** FORMAL ARGUMENTS:
**
**	Return value:	NIL if they are not in the same class, otherwise
**					the class symbol of the shared class is returned.
**
**	pattern1:		One of the two patterns to be assessed.
**	pattern2:		The other pattern.
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

symbol* same_class(sequence* pattern1, sequence* pattern2)
{
	symbol* symbol1, * symbol2;

	list_for(symbol1, symbol, pattern1)
	{
		if (symbol1->get_type() == LEFT_BRACKET) continue;
		if (symbol1->get_status() == CONTENTS) break;
		list_for(symbol2, symbol, pattern2)
		{
			if (symbol2->get_type() == LEFT_BRACKET) continue;
			if (symbol2->get_status() == CONTENTS) break;

			if (symbol1->name_matches(symbol2)) return(symbol1);
		}
	}
	return(NIL);
} // same_class

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Makes a new left or right bracket symbol.
**
** CALLING SEQUENCE:
**
**	symbol *make_bracket_symbol(int type, int status, al_depth)
**
** FORMAL ARGUMENTS:
**
**	Return value:	A new bracket symbol.
**
**	type:			LEFT_BRACKET or RIGHT_BRACKET.
**	status:			BOUNDARY_MARKER or CONTENTS.
**	al_depth:		The number of rows.
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

symbol* make_bracket_symbol(int type, int status, int al_depth)
{
	symbol* symbol1 = new symbol(NIL, al_depth, NULL_VALUE);
	symbol1->set_type(type);
	if (type == LEFT_BRACKET) symbol1->set_name("<");
	else if (type == RIGHT_BRACKET) symbol1->set_name(">");
	else abort_run("Invalid type for make_bracket_symbol().");
	symbol1->set_status(status);
	symbol1->assign_frequency_and_cost();
	symbol1->set_is_a_hit(false);

	return(symbol1);
} // make_bracket_symbol

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Increments and index by 1, checks that it does not exceed a limit
**	and aborts the program, printing out a message if the limit is
**	exceeded.
**
** CALLING SEQUENCE:
**
**	void increment_index(int *index, int limit, char *message)
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	index:			A pointer to the index to be incremented.
**	limit:			The limit to be checked.
**	message:		The message to be printed out.
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

void increment_index(int* index, int limit, char* message)
{
	(*index)++;
	if ((*index) >= limit)
		abort_run(message);

} // increment_index

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Prints the number of the pattern from New and, within that pattern,
**	the cycle.
**
** CALLING SEQUENCE:
**
**	void print_pattern_cycle(bool print_cycle, sequence *pattern1)
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	print_cycle:	If true, print the current value of cycle,
**					otherwise don't.
**	pattern1:		The pattern currently being parsed.
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

void print_pattern_cycle(bool print_cycle, sequence* pattern1)
{
	if (print_cycle == false)
		fprintf(output_file, "%s%d%s%d",
			"PATTERN ID",
			pattern1->get_ID(),
			", PHASE ",
			phase);
	else
	{
		fprintf(output_file, "%s%d",
			"PATTERN ID",
			pattern1->get_ID());
		if (phase == 2)
			fprintf(output_file, "%s%d%s%d%s%d",
				" (ID",
				current_new_pattern->get_ID(),
				"), CYCLE ",
				cycle,
				", PHASE ",
				phase);
		else fprintf(output_file, "%s%d%s%d",
			", CYCLE ",
			cycle,
			", PHASE ",
			phase);
	}
	fflush(output_file);
} // print_pattern_cycle

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Makes a new column for insertion in new_alignment
**	(in unify_best).
**
** CALLING SEQUENCE:
**
**	symbol *make_column(int new_alignment_depth,
**			sequence *driving_pattern, symbol *driving_symbol,
**			sequence *target_pattern, symbol *target_symbol,
**			int target_pattern_row_in_new_alignment, int hn_ID,
**			bool hit_this_cycle)
**
** FORMAL ARGUMENTS:
**
**	Return value:			A pointer to the newly-created column. Value
**							is NIL if no column is created.
**
**	new_alignment_depth:	The depth of the new alignment.
**	driving_pattern:		The driving pattern.
**	driving_symbol: 		The driving symbol containing information to be
**							copied into the new column.
**	driving_int_pos:		The int position of the driving_symbol in
**							the driving pattern.
**	target_pattern: 		The target pattern.
**	target_symbol:			The target symbol containing information to be
**							copied into the new column.
**	target_int_pos: 		The int position of the target_symbol in
**							the target pattern.
**	target_pattern_row_in_new_alignment: The row of the new column into which the
**							first row of the target symbol should be copied.
**	hn_ID:					The ID of the hit node (corresponding to this
**							new column. NULL_VALUE if there is no hit node.
**	hit_this_cycle: 		true if the column records a hit on this cycle,
**							false otherwise.
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

symbol* make_column(int new_alignment_depth,
	sequence* driving_pattern, symbol* driving_symbol,
	sequence* target_pattern, symbol* target_symbol,
	int target_pattern_row_in_new_alignment, int hn_ID,
	bool hit_this_cycle)
{
	symbol* new_al_col1, * symbol1 = { NIL }, * driving_new_symbol;
	alignment_element* al_el1, * al_el2;
	int driving_pattern_depth, target_pattern_depth, i;

	if (driving_symbol != NIL) symbol1 = driving_symbol;
	else if (target_symbol != NIL) symbol1 = target_symbol;
	else abort_run("driving_symbol and target_symbol are both NIL in \
		make_column");

	new_al_col1 = new symbol(symbol1->get_name(),
		new_alignment_depth, hn_ID);

	if (hit_this_cycle) new_al_col1->set_is_a_hit(true);

	new_al_col1->set_bit_cost(symbol1->get_bit_cost());
	new_al_col1->set_type(symbol1->get_type());

	if (driving_symbol != NIL)
	{
		driving_pattern_depth = driving_pattern->get_sequence_depth();
		if (driving_pattern_depth == 1) // Driving pattern is New
		{
			al_el1 = new_al_col1->get_al_el(0);
			al_el1->set_el_obj(driving_symbol);
			al_el1->set_original_pattern(driving_pattern);
		}
		else for (i = 0; i < driving_pattern_depth; i++)
		{
			al_el1 = new_al_col1->get_al_el(i);
			al_el2 = driving_symbol->get_al_el(i);
			*al_el1 = *al_el2;
		}
		if (driving_symbol->is_a_hit())
			new_al_col1->set_is_a_hit(true);
	}

	if (target_symbol != NIL)
	{
		target_pattern_depth = target_pattern->get_sequence_depth();
		if (target_pattern_depth == 1) // Target pattern is one of
			// original patterns in Old
		{
			al_el1 = new_al_col1->
				get_al_el(target_pattern_row_in_new_alignment);
			driving_new_symbol = (symbol*)al_el1->get_el_obj();
			if (driving_new_symbol != NIL &&
				driving_new_symbol != target_symbol)
				abort_run("Anomaly in make_column \
				(target_pattern_depth == 1)");
			al_el1->set_el_obj(target_symbol);
			al_el1->set_original_pattern(target_pattern);
		}
	}

	// Now put the correct values in for
	// same_column_above and same_column_below.

	new_al_col1->set_symbol_matches();

	return(new_al_col1);
} // make_column

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Prints the current time.
**
** CALLING SEQUENCE:
**
**	void time_now()
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
** IMPLICIT INPUTS:
**
**	From the computer clock.
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

void time_now()
{
	struct tm* time_now;
	time_t secs_now;
	char str[80];

	tzset();
	time(&secs_now);
	time_now = localtime(&secs_now);
	strftime(str, 80,
		"Time: %H:%M, %A, %B %d, 20%y",
		time_now);
	fprintf(output_file, "%s\n\n", str);
} // time_now

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Prints the year.
**
** CALLING SEQUENCE:
**
**	void year()
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
** IMPLICIT INPUTS:
**
**	From the computer clock.
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

void year()
{
	struct tm* time_now;
	time_t secs_now;
	char str[20];

	tzset();
	time(&secs_now);
	time_now = localtime(&secs_now);
	strftime(str, 20,
		"20%y",
		time_now);
	fprintf(output_file, "%s", str);
} // time_now

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Prepares the ground for unify_best by copying the nodes of
**	a hit sequence into hit_entries[]. This means that it is easy
**	to step through the nodes in left-to-right order - which is
**	more intuitive than always using them in reverse order.
**
** CALLING SEQUENCE:
**
**	void prepare_hit_sequence(hit_node *leaf_node)
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	leaf_node:	The leaf node of the hit sequence in the hit structure.
**
** IMPLICIT INPUTS:
**
**	The hit structure
**
** IMPLICIT OUTPUTS:
**
**	hit_entries[]
**
** SIDE EFFECTS:
**
**	NONE
**
*/

void prepare_hit_sequence(hit_node* leaf_node)
{
	hit_node* h_node, * left_h_node;
	int node_position;

	// First count the number of nodes in the hit sequence
	// so that a check can be made that it does not exceed the
	// size of hit_entries[] and to find the correct place
	// in hit_entries[] to begin loading the nodes.

	node_position = -1;
	for (h_node = leaf_node; h_node != hit_root;
		h_node = h_node->get_hn_parent())
		node_position++;

	fe_hit_entries = node_position + 1;
	if (fe_hit_entries >= MEDIUM_SCRATCH_ARRAY_SIZE)
		abort_run("Chain of hit_nodes is too long in unify_best");

	for (left_h_node = leaf_node; left_h_node != hit_root;
		left_h_node = left_h_node->get_hn_parent())
		hit_entries[node_position--].h_node = left_h_node;
} // prepare_hit_sequence

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Checks for mismatches in a hit sequence (in unify_best).
**
** CALLING SEQUENCE:
**
**	bool mismatch_found(hit_node *leaf_node, sequence *cnp)
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if there are mismatches, false otherwise.
**
**	leaf_node:		The last node of the hit structure sequence being
**					processed.
**	cnp:			The New pattern being processed.
**
** IMPLICIT INPUTS:
**
**	hit_entries[] and fe_hit_entries
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

bool mismatch_found(hit_node* leaf_node, sequence* cnp)
{
	hit_node* h_node, * left_h_node, * right_h_node;
	sequence* driving_pattern, * target_pattern;
	symbol* last_driving_symbol, * last_target_symbol;

	// Unless the driving pattern is the same as New, a 'mis-match'
	// occurs if, at any point in the alignment between one hit (for the
	// the current cycle) and the next hit (for the current cycle), there 
	// are one or more non-hit symbols from the driving pattern AND
	// one or more non-hit symbols from the target pattern. If there
	// is any mis-match in an alignment, it is not valid and should
	// not be formed.

	h_node = leaf_node;
	driving_pattern = h_node->get_driving_pattern();
	if (driving_pattern == cnp) return(false);

	// The driving_pattern is not the same as New.

	// Check for mis-matches *after* the last hit (for the current cycle).

	target_pattern = h_node->get_target_pattern();
	last_driving_symbol = (symbol*)
		driving_pattern->get_last_child();
	last_target_symbol = (symbol*)
		target_pattern->get_last_child();
	if (last_driving_symbol != h_node->get_driving_symbol()
		&& last_target_symbol != h_node->get_target_symbol())
		return(true);

	// Check for mis-matches between the first and last hits.

	left_h_node = NIL;
	for (int i = 0; i < fe_hit_entries; i++)
	{
		right_h_node = hit_entries[i].h_node;
		if (left_h_node == NIL)
		{
			left_h_node = right_h_node;
			continue;
		}
		if (right_h_node->get_driving_int_pos() -
			left_h_node->get_driving_int_pos() > 1
			&& right_h_node->get_target_int_pos() -
			left_h_node->get_target_int_pos() > 1)
			return(true);
		left_h_node = right_h_node;
	}

	// Now check for mis-matches before the first hit.

	h_node = hit_entries[0].h_node;
	if (h_node->get_driving_int_pos() > 0
		&& h_node->get_target_int_pos() > 0)
		return(true);

	// No mismatch has been detected.

	return(false);
} // mismatch_found

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Checks a newly-constructed alignment to make sure that symbols from
**	cnp are in their correct order.
**
** CALLING SEQUENCE:
**
**	bool check_ordering_of_new(sequence *new_alignment, sequence *cnp)
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if the symbols from cnp
**					are in their correct order, false otherwise.
**
**	new_alignment:		The alignment to be checked.
**	cnp:			The New pattern being processed.
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

bool check_ordering_of_new(sequence* new_alignment, sequence* cnp)
{
	int new_symbol_int_pos1 = NULL_VALUE, new_symbol_int_pos2;
	symbol* symbol1;

	for (symbol1 = (symbol*)new_alignment->get_first_child();
		symbol1 != NIL; symbol1 = (symbol*)
		new_alignment->get_next_child())
	{
		new_symbol_int_pos2 = symbol1->get_row_orig_int_pos(0);
		if (new_symbol_int_pos2 != NULL_VALUE)
		{
			if (new_symbol_int_pos1 != NULL_VALUE)
			{
				if (new_symbol_int_pos2 <= new_symbol_int_pos1)
					return(false);
			}
			new_symbol_int_pos1 = new_symbol_int_pos2;
		}
	}

	return(true);
} // check_ordering_of_new

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Checks that gaps between hit symbols in a newly-formed alignment
**	conform to relevant parameters.
**
** CALLING SEQUENCE:
**
**	bool al_gaps_OK(sequence *al1)
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if the gaps are within limits, false otherwise.
**
**	al1:			The alignment to be tested.
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

bool al_gaps_OK(sequence* al1)
{
	symbol* al_col1;
	int previous_new_hit_int_pos = NULL_VALUE, this_new_hit_int_pos;

	// Test gaps between hit symbols in New.

	al1->initialise();
	while (al_col1 = (symbol*)al1->get_next_child())
	{
		this_new_hit_int_pos = al_col1->get_row_orig_int_pos(0);

		if (this_new_hit_int_pos == NULL_VALUE)
		{
			previous_new_hit_int_pos = this_new_hit_int_pos;
			continue;
		}
		if (previous_new_hit_int_pos == NULL_VALUE) continue;

		// if ((this_new_hit_int_pos - previous_new_hit_int_pos - 1) >
		//	max_new_gap)
		//	return(false) ;

		previous_new_hit_int_pos = this_new_hit_int_pos;
	}
	return(true);
} // al_gaps_OK

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Forms an alignment from a hit sequence. This function is called
**	from unify_best.
**
** CALLING SEQUENCE:
**
**	sequence *make_one_alignment(hit_node *leaf_node, sequence *cnp)
**
** FORMAL ARGUMENTS:
**
**	Return value:	The alignment which is formed. NIL if, for some reason,
**					the hit sequence proves to be invalid.
**
**	leaf_node:		The leaf node of the hit sequence from which the new
**					alignment is to be formed.
**	cnp:			The New pattern being processed.
**
** IMPLICIT INPUTS:
**
**	hit_entries[], fe_hit_entries
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

sequence* make_one_alignment(hit_node* leaf_node, sequence* cnp)
{
	sequence* driving_pattern, * target_pattern, * temp_patt = { NIL }, * new_alignment;
	symbol* newly_created_col, * driving_symbol, * target_symbol,
		* hit_target_symbol, * newly_created_hit_col, * symbol1;
	int driving_pattern_depth,
		new_alignment_depth, hit_node_index,
		target_pattern_row_in_new_alignment,
		driving_int_pos, target_int_pos, row1;
	hit_node* h_node;
	alignment_element* al_el1, * al_el2;

	// 1 CALCULATE THE DEPTH OF THE NEW ALIGNMENT AS THE SUM OF
	// OF THE DEPTHS OF THE DRIVING PATTERN AND THE TARGET PATTERN.
	// IT IS ASSUMED THAT THE TARGET PATTERN IS ALWAYS AN OLD PATTERN,
	// WITH A DEPTH OF 1.

	int start_of_new_target_pattern = NULL_VALUE;
	driving_pattern = leaf_node->get_driving_pattern();
	target_pattern = leaf_node->get_target_pattern();
	driving_pattern_depth = driving_pattern->get_sequence_depth();

	new_alignment_depth = driving_pattern_depth + 1;

	if (new_alignment_depth >= MAX_ALIGNMENT_DEPTH)
		abort_run("Too many rows in alignment in unify_best");

	// Determine the row position of the target pattern in the
	// new alignment.

	target_pattern_row_in_new_alignment = driving_pattern_depth;

	// 2 CREATE AN OBJECT FOR A NEW ALIGNMENT. 

	new_alignment = new sequence;
	new_alignment->set_driving_ID(driving_pattern->get_ID());
	new_alignment->set_target_ID(target_pattern->get_ID());

	// 3 IF THE DRIVING PATTERN IS NOT THE SAME AS NEW, COPY IN
	// ALL THE COLUMNS FOR THE DRIVING PATTERN BUT
	// USING COLUMNS WITH A LENGTH OF AL_CONSTR_DEPTH. IN hit_entries[]
	// KEEP A RECORD OF WHICH OF THE NEWLY-CREATED COLUMNS REPRESENTS
	// A HIT.

	// IF THE DRIVING PATTERN IS THE SAME AS NEW,
	// DO THE SAME BUT ONLY FOR THOSE SYMBOLS IN THE DRIVING_PATTERN
	// THAT FORM HITS WITH THE TARGET PATTERN.

	hit_node_index = 0;
	if (driving_pattern == cnp)
	{
		// The driving pattern is the same as New. Only make
		// columns for those symbols in the driving pattern which
		// form hits with the target pattern.

		for (hit_node_index = 0; hit_node_index < fe_hit_entries;
			hit_node_index++)
		{
			h_node = hit_entries[hit_node_index].h_node;
			driving_symbol = h_node->get_driving_symbol();
			driving_pattern = h_node->get_driving_pattern();
			driving_int_pos = driving_pattern->
				get_int_pos_by_child(driving_symbol);
			target_symbol = h_node->get_target_symbol();
			target_pattern = h_node->get_target_pattern();
			target_int_pos = target_pattern->
				get_int_pos_by_child(target_symbol);
			newly_created_col = make_column(new_alignment_depth,
				driving_pattern, driving_symbol,
				target_pattern, target_symbol,
				target_pattern_row_in_new_alignment,
				h_node->get_ID(), true);

			new_alignment->add_child(newly_created_col);

			hit_entries[hit_node_index].newly_created_col =
				newly_created_col;
		}
	}
	else
	{
		// The driving pattern is not the same as New.

		driving_pattern->initialise();
		while (driving_symbol = (symbol*)
			driving_pattern->get_next_child())
		{
			driving_int_pos = driving_pattern->
				get_int_pos_by_child(driving_symbol);

			// Note: the last field below (for whether or not
			// this column contains a hit) is set to false
			// as a default. The value is set properly a few
			// lines down.

			newly_created_col = make_column(new_alignment_depth,
				driving_pattern, driving_symbol,
				target_pattern, NIL,
				target_pattern_row_in_new_alignment,
				NULL_VALUE, false);

			new_alignment->add_child(newly_created_col);

			// Now check to see whether this driving symbol is
			// part of a hit. If it is, make a note in hit_entries
			// to show the correspondence between this hit
			// and newly_created_col.

			if (hit_node_index >= fe_hit_entries) continue;
			h_node = hit_entries[hit_node_index].h_node;
			if (driving_symbol == h_node->get_driving_symbol())
			{
				hit_entries[hit_node_index].newly_created_col =
					newly_created_col;
				newly_created_col->
					set_h_node_ID(h_node->get_ID());
				newly_created_col->
					set_is_a_hit(true);
				hit_node_index++;
			}
		}
	}

	// 4 STEP THROUGH THE HIT SEQUENCE FILLING IN THE DETAILS OF
	// THE TARGET SYMBOL IN EACH COLUMN REPRESENTING A HIT.

	symbol* last_column_added = { NIL }; // This is needed to determine the
		// the correct positioning of trailing unmatched target symbols,
		// if any.
	for (hit_node_index = 0; hit_node_index < fe_hit_entries;
		hit_node_index++)
	{
		h_node = hit_entries[hit_node_index].h_node;
		newly_created_col =
			hit_entries[hit_node_index].newly_created_col;

		target_symbol = h_node->get_target_symbol();

		target_pattern = h_node->get_target_pattern();
		target_int_pos = h_node->get_target_int_pos();
		al_el2 = newly_created_col->
			get_al_el(target_pattern_row_in_new_alignment);
		al_el2->set_el_obj(target_symbol);
		al_el2->set_original_pattern(target_pattern);
		last_column_added = newly_created_col;
	}

	// 5 STEP THROUGH THE TARGET PATTERN AND THE TABLE OF HIT_ENTRIES[],
	// INSERTING NEW COLUMNS IN new_alignment CORRESPONDING TO SYMBOLS
	// IN THE TARGET PATTERN WHICH HAVE NOT FORMED HITS
	// WITH THE DRIVING PATTERN.

	target_symbol = (symbol*)target_pattern->get_first_child();
	for (hit_node_index = 0; hit_node_index < fe_hit_entries;
		hit_node_index++)
	{
		h_node = hit_entries[hit_node_index].h_node;
		newly_created_hit_col =
			hit_entries[hit_node_index].newly_created_col;
		hit_target_symbol = h_node->get_target_symbol();
		while (target_symbol != hit_target_symbol)
		{
			newly_created_col = make_column(new_alignment_depth,
				driving_pattern, NIL,
				target_pattern, target_symbol,
				target_pattern_row_in_new_alignment,
				NULL_VALUE, false);
			new_alignment->precede(newly_created_col,
				newly_created_hit_col);
			target_symbol = (symbol*)
				target_pattern->get_next_child();
		}

		// Step past the target symbol which corresponds to the
		// current hit_target_symbol and get the next target
		// symbol.

		target_symbol = (symbol*)target_pattern->get_next_child();
	}

	// 6 DO TAIL OF THE TARGET PATTERN (IF ANY).

	while (target_symbol != NIL)
	{
		newly_created_col = make_column(new_alignment_depth,
			driving_pattern, NIL,
			target_pattern, target_symbol,
			target_pattern_row_in_new_alignment, NULL_VALUE,
			false);
		new_alignment->follow(last_column_added, newly_created_col);
		last_column_added = newly_created_col;
		target_symbol = (symbol*)target_pattern->get_next_child();
	}

	// Check for anomalies in the ordering of symbols in
	// cnp within the alignment.

	if (check_ordering_of_new(new_alignment, cnp) == false)
	{
		delete new_alignment;
		new_alignment = NIL;
		if (verbose)
		{
			fprintf(output_file,
				"Anomalous ordering of symbols from New in hit sequence ");
			leaf_node->print_ID();
			fprintf(output_file, ". No new alignment is formed.\n\n");
		}
		return(NIL);
	}

	// Check that gaps between hits conform with relevant parameters.

	if (al_gaps_OK(new_alignment) == false)
	{
		delete new_alignment;
		new_alignment = NIL;
		if (verbose)
		{
			fprintf(output_file, "Gaps too large in hit sequence ");
			leaf_node->print_ID();
			fprintf(output_file, ". Alignment is discarded.\n\n");
		}
		return(NIL);
	}

	new_alignment->set_sequence_depth(new_alignment_depth);
	new_alignment->set_leaf_node_ID(leaf_node->get_ID());

	// 7 MAKE SURE THAT EVERY CELL OF EACH ROW OF THE ALIGNMENT CONTAINS
	// A REFERENCE TO THE PATTERN FOR THAT ROW. NB This information
	// is needed in compute_score_with_gaps (later), hence it needs to be
	// done before compute_score_with_gaps is called.

	for (row1 = 0; row1 < new_alignment_depth; row1++)
	{
		// First find the first reference to the pattern for
		// this row.

		new_alignment->initialise();
		while (symbol1 = (symbol*)
			new_alignment->get_next_child())
		{
			temp_patt = symbol1->get_row_pattern(row1);
			if (temp_patt != NIL) break;
		}

		// Now put the value of temp_patt in every cell for this row.

		new_alignment->initialise();
		while (symbol1 = (symbol*)
			new_alignment->get_next_child())
		{
			al_el1 = symbol1->get_al_el(row1);
			al_el1->set_original_pattern(temp_patt);
		}
	}

	// 8 MARK POSITIONS OF SYMBOLS IN INTEGERS AND SHOW PARENT
	// OF EACH SYMBOL

	new_alignment->mark_parent_and_int_positions_non_recursive();

	// 9 MARK STATUS OF SYMBOLS

	list_element* pos1 = NIL;
	symbol* first_symbol = (symbol*)new_alignment->get_first_child(),
		* last_symbol = (symbol*)new_alignment->get_last_child(),
		* symbol2;
	list_for_el_pos(symbol1, symbol, new_alignment, pos1)
	{
		if (symbol1->is_a_hit())
			symbol1->set_status(CONTENTS);
		else
		{
			symbol2 = symbol1->find_unmatched_symbol();
			if (symbol2 == NIL)
				abort_run("Anomaly in make_one_alignment().");
			symbol1->set_status(symbol2->get_status());
		}
	}

	new_alignment->set_symbol_matches();

	return(new_alignment);
} // make_one_alignment

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Using a hit sequence recorded in hit_entries[], write the
**	corresponding alignment horizontally.
**
** CALLING SEQUENCE:
**
**	void write_hit_sequence_horizontal()
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
** IMPLICIT INPUTS:
**
**	hit_entries[]
**
** IMPLICIT OUTPUTS:
**
**	NONE
**
** SIDE EFFECTS:
**
**	NONE
*/

void write_hit_sequence_horizontal()
{
	hit_node* leaf_node = hit_entries[fe_hit_entries - 1].h_node,
		* h_node;
	int hit_index, char_row, char_col;
	char character_array[3][LARGE_SCRATCH_ARRAY_SIZE];
	sequence* driving_pattern = leaf_node->get_driving_pattern(),
		* target_pattern = leaf_node->get_target_pattern();
	symbol* d_symbol, * hit_d_symbol, * t_symbol, * hit_t_symbol;

	// Clear character_array[][].

	for (char_row = 0; char_row < 3; char_row++)
		for (char_col = 0; char_col < LARGE_SCRATCH_ARRAY_SIZE;
			char_col++)
			character_array[char_row][char_col] = ' ';

	fprintf(output_file, "%s%d%s",
		"HIT SEQUENCE #",
		leaf_node->get_ID(),
		":\n\n");

	// Step through hit_entries[] filling in the names of symbols
	// from the driving_pattern and the target_pattern.

	int d_char_pos = 0, t_char_pos = 0, name_index, hit_char_pos,
		temp_pos, c_col_for_link, symbol_length;
	char* symbol_name, c;
	driving_pattern->initialise();
	target_pattern->initialise();
	for (hit_index = 0; hit_index < fe_hit_entries; hit_index++)
	{
		h_node = hit_entries[hit_index].h_node;
		hit_d_symbol = h_node->get_driving_symbol();
		hit_t_symbol = h_node->get_target_symbol();

		// Fill in the names of the non-hit symbols before
		// hit_d_symbol and hit_t_symbol.

		while (d_symbol = (symbol*)driving_pattern->get_next_child())
		{
			if (d_symbol == hit_d_symbol) break;

			symbol_name = d_symbol->get_name();
			name_index = 0;
			c = *symbol_name;
			while (c != '\0')
			{
				character_array[0][d_char_pos++] = c;
				c = symbol_name[++name_index];
			}

			// We need one space between symbol names.
			plus_one(&d_char_pos, LARGE_SCRATCH_ARRAY_SIZE,
				"Overflow in write_hit_sequence_horizontal()");
		}

		while (t_symbol = (symbol*)target_pattern->get_next_child())
		{
			if (t_symbol == hit_t_symbol) break;

			symbol_name = t_symbol->get_name();
			name_index = 0;
			c = *symbol_name;
			while (c != '\0')
			{
				character_array[2][t_char_pos++] = c;
				c = symbol_name[++name_index];
			}
			// We need one space between symbol names.
			plus_one(&t_char_pos, LARGE_SCRATCH_ARRAY_SIZE,
				"Overflow in write_hit_sequence_horizontal()");
		}

		// Now fill in the hit symbols and the '|' character between them.

		temp_pos = // Maximum of d_char_pos and t_char_pos.
			(d_char_pos > t_char_pos) ? d_char_pos : t_char_pos;

		symbol_name = hit_d_symbol->get_name();
		name_index = 0;
		c = *symbol_name;
		hit_char_pos = temp_pos;
		while (c != '\0')
		{
			if (hit_char_pos > LARGE_SCRATCH_ARRAY_SIZE)
				abort_run("Overflow in write_hit_sequence_horizontal()");
			character_array[0][hit_char_pos++] = c;
			c = symbol_name[++name_index];
		}

		d_char_pos = hit_char_pos + 1; // Add 1 for space character.

		symbol_name = hit_t_symbol->get_name();
		name_index = 0;
		c = *symbol_name;
		hit_char_pos = temp_pos;
		while (c != '\0')
		{
			character_array[2][hit_char_pos++] = c;
			c = symbol_name[++name_index];
		}

		t_char_pos = hit_char_pos + 1; // Add 1 for space character.

		// The '|' should be half way along the length of the hit
		// symbol name.

		symbol_length = (int)strlen(symbol_name);
		c_col_for_link = temp_pos + (symbol_length / 2);

		character_array[1][c_col_for_link] = '|';
	}

	// Now do trailing non-hit symbols, if any.

	while (d_symbol = (symbol*)driving_pattern->get_next_child())
	{
		symbol_name = d_symbol->get_name();
		name_index = 0;
		c = *symbol_name;
		while (c != '\0')
		{
			character_array[0][d_char_pos++] = c;
			c = symbol_name[++name_index];
		}
		// We need one space between symbol names.
		plus_one(&d_char_pos, LARGE_SCRATCH_ARRAY_SIZE,
			"Overflow in write_hit_sequence_horizontal()");
	}

	while (t_symbol = (symbol*)target_pattern->get_next_child())
	{
		symbol_name = t_symbol->get_name();
		name_index = 0;
		c = *symbol_name;
		while (c != '\0')
		{
			character_array[2][t_char_pos++] = c;
			c = symbol_name[++name_index];
		}
		// We need one space between symbol names.
		plus_one(&t_char_pos, LARGE_SCRATCH_ARRAY_SIZE,
			"Overflow in write_hit_sequence_horizontal()");
	}

	temp_pos = // Maximum of d_char_pos and t_char_pos.
		(d_char_pos > t_char_pos) ? d_char_pos : t_char_pos;

	// Now write out character_array[][].

	for (char_row = 0; char_row < 3; char_row++)
	{
		if (char_row == 0)
			fprintf(output_file, "%d%c",
				0,
				' ');
		else if (char_row == 1) fprintf(output_file, "  ");
		else fprintf(output_file, "%d%c",
			1,
			' ');

		for (char_col = 0; char_col < temp_pos; char_col++)
		{
			fprintf(output_file, "%c",
				character_array[char_row][char_col]);
		}
		if (char_row == 0)
			fprintf(output_file, "%d%c",
				0,
				'\n');
		else if (char_row == 1) fprintf(output_file, "\n");
		else fprintf(output_file, "%d%c",
			1,
			'\n');
	}

	fprintf(output_file, "\n");

} // write_hit_sequence_horizontal

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Using a hit sequence recorded in hit_entries[], write the
**	corresponding alignment vertically.
**
** CALLING SEQUENCE:
**
**	void write_hit_sequence_vertical()
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
** IMPLICIT INPUTS:
**
**	hit_entries[]
**
** IMPLICIT OUTPUTS:
**
**	NONE
**
** SIDE EFFECTS:
**
**	NONE
*/

void write_hit_sequence_vertical(void)
{
	hit_node* leaf_node = hit_entries[fe_hit_entries - 1].h_node,
		* h_node;
	int hit_index, char_row, char_col, d_char_pos = 0,
		t_char_pos = 0, name_index, hit_char_pos, symbol_length;
	char character_array[LARGE_SCRATCH_ARRAY_SIZE]
		[MEDIUM_SCRATCH_ARRAY_SIZE], * symbol_name, c;
	sequence* driving_pattern = leaf_node->get_driving_pattern(),
		* target_pattern = leaf_node->get_target_pattern();
	symbol* d_symbol, * hit_d_symbol, * t_symbol, * hit_t_symbol;

	// Clear character_array[][].

	for (char_row = 0; char_row < LARGE_SCRATCH_ARRAY_SIZE; char_row++)
		for (char_col = 0; char_col < MEDIUM_SCRATCH_ARRAY_SIZE;
			char_col++)
			character_array[char_row][char_col] = ' ';

	fprintf(output_file, "%s%d%s",
		"HIT SEQUENCE #",
		leaf_node->get_ID(),
		":\n\n");

	// Find width of column 1 - the length of the longest symbol
	// name in the driving_pattern.

	int col_1_width = 0;
	list_for(d_symbol, symbol, driving_pattern)
	{
		symbol_name = d_symbol->get_name();
		symbol_length = (int)strlen(symbol_name);
		if (symbol_length > col_1_width)
			col_1_width = symbol_length;
	}

	int start_col_2 = col_1_width + 3; // Allow enough space
		// for at least ' - ' between columns.
	int end_broken_line = start_col_2 - 1;

	// Check that the character_array[][] is wide enough.

	int col_2_width = 0;
	list_for(t_symbol, symbol, target_pattern)
	{
		symbol_name = t_symbol->get_name();
		symbol_length = (int)strlen(symbol_name);
		if (symbol_length > col_2_width)
			col_2_width = symbol_length;
	}

	int overall_width = start_col_2 + col_2_width, hit_row = 0;
	if (overall_width > MEDIUM_SCRATCH_ARRAY_SIZE)
		abort_run("Array overflow in write_hit_sequence_vertical().");

	// Step through hit_entries[] filling in the names of symbols
	// from the driving_pattern and the target_pattern.

	driving_pattern->initialise();
	target_pattern->initialise();
	int d_row = 0, t_row = 0;
	for (hit_index = 0; hit_index < fe_hit_entries; hit_index++)
	{
		h_node = hit_entries[hit_index].h_node;
		hit_d_symbol = h_node->get_driving_symbol();
		hit_t_symbol = h_node->get_target_symbol();

		// Fill in the names of the non-hit symbols before
		// hit_d_symbol and hit_t_symbol.

		while (d_symbol = (symbol*)driving_pattern->get_next_child())
		{
			if (d_symbol == hit_d_symbol) break;

			d_char_pos = 0;
			symbol_name = d_symbol->get_name();
			name_index = 0;
			c = *symbol_name;
			while (c != '\0')
			{
				character_array[d_row][d_char_pos++] = c;
				c = symbol_name[++name_index];
			}

			d_row++;
		}

		while (t_symbol = (symbol*)target_pattern->get_next_child())
		{
			if (t_symbol == hit_t_symbol) break;

			t_char_pos = start_col_2;
			symbol_name = t_symbol->get_name();
			name_index = 0;
			c = *symbol_name;
			while (c != '\0')
			{
				character_array[t_row][t_char_pos++] = c;
				c = symbol_name[++name_index];
			}

			t_row++;
		}

		hit_row = (d_row > t_row) ? d_row : t_row;

		// Now fill in the hit symbols.

		hit_char_pos = 0;
		symbol_name = hit_d_symbol->get_name();
		name_index = 0;
		c = *symbol_name;
		while (c != '\0')
		{
			character_array[hit_row][hit_char_pos++] = c;
			c = symbol_name[++name_index];
		}

		// Now put in a broken line up to the beginning of
		// the second column.

		hit_char_pos++;
		while (hit_char_pos < end_broken_line)
			character_array[hit_row][hit_char_pos++] = '-';

		hit_char_pos = start_col_2;
		symbol_name = hit_t_symbol->get_name();
		name_index = 0;
		c = *symbol_name;
		while (c != '\0')
		{
			character_array[hit_row][hit_char_pos++] = c;
			c = symbol_name[++name_index];
		}
	}

	// Now do trailing non-hit symbols, if any.

	d_row = hit_row + 1;
	while (d_symbol = (symbol*)driving_pattern->get_next_child())
	{
		d_char_pos = 0;
		symbol_name = d_symbol->get_name();
		name_index = 0;
		c = *symbol_name;
		while (c != '\0')
		{
			character_array[d_row][d_char_pos++] = c;
			c = symbol_name[++name_index];
		}
		d_row++;
	}

	t_row = hit_row + 1;
	while (t_symbol = (symbol*)target_pattern->get_next_child())
	{
		t_char_pos = start_col_2;
		symbol_name = t_symbol->get_name();
		name_index = 0;
		c = *symbol_name;
		while (c != '\0')
		{
			character_array[t_row][t_char_pos++] = c;
			c = symbol_name[++name_index];
		}
		t_row++;
	}

	// Now write out character_array[][].

	// First write column numbers at the top.

	fprintf(output_file, "%c", '0');
	for (int i = 1; i < start_col_2; i++)
		fprintf(output_file, "%c", ' ');
	fprintf(output_file, "%c%s",
		'1',
		"\n\n");

	int row_limit = (d_row > t_row) ? d_row : t_row;

	for (char_row = 0; char_row < row_limit; char_row++)
	{
		for (char_col = 0; char_col < overall_width; char_col++)
		{
			fprintf(output_file, "%c",
				character_array[char_row][char_col]);
		}
		fprintf(output_file, "\n");
	}

	// Write column numbers at the bottom.

	fprintf(output_file, "\n0");
	for (int i = 1; i < start_col_2; i++)
		fprintf(output_file, "%c", ' ');
	fprintf(output_file, "%c%s",
		'1',
		"\n\n");

} // write_hit_sequence_vertical

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Displays a hit sequence in horizontal or vertical format.
**
** CALLING SEQUENCE:
**
**	void write_hit_sequence()
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

void write_hit_sequence()
{
	if (alignment_format == 'H')
		write_hit_sequence_horizontal();
	else write_hit_sequence_vertical();
} // write_hit_sequence

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	From the best hit sequence in the hit_structure, this function
**	forms a new alignment. It is assumed that every CONTENTS symbol
**	in the target pattern forms a hit with a symbol in the driving
**	pattern.
**
** CALLING SEQUENCE:
**
**	sequence *unify_best(int leaf_index, sequence *cnp)
**
** FORMAL ARGUMENTS:
**
**	Return value:		The new sequence, NIL if none is formed.
**
**	leaf_index:			The position of the last node of
**						the current hit sequence in the leaf_array[].
**	cnp:				The New pattern being processed.
**
** IMPLICIT INPUTS:
**
**	The hit_structure
**
** IMPLICIT OUTPUTS:
**
**	NONE
**
** SIDE EFFECTS:
**
**	NONE
*/

sequence* unify_best(int leaf_index, sequence* cnp)
{
	hit_node* leaf_node = leaf_array[leaf_index];
	sequence* matching_sequence, * new_alignment = NIL,
		* driving_pattern = leaf_node->get_driving_pattern();

	if (leaf_node == NIL)
		abort_run("NIL value for leaf_node in unify_best");

	if (verbose)
	{
		fprintf(output_file, "STARTING unify_best (");
		print_pattern_cycle(true, cnp);
		fprintf(output_file, ") LEAF NODE ID = ");
		leaf_node->print_ID();
		fprintf(output_file, "\n\n");
		fflush(output_file);
	}

	// 1 LOAD NODES OF THE HIT SEQUENCE INTO hit_entries[] SO THAT
	// THEY CAN BE ACCESSED EASILY IN LEFT-TO-RIGHT ORDER (WHICH
	// IS MORE INTUITIVE).

	prepare_hit_sequence(leaf_node);

	// 3 FORM AN ALIGNMENT

	new_alignment = make_one_alignment(leaf_node, cnp);
	if (new_alignment == NIL) goto L1;

	// Check to see that the new alignment is legal in terms of the
	// embedding of brackets. If it is not, delete it.

	if (new_alignment->is_legal() == false)
	{
		fprintf(output_file, "%s%d%s",
			"The following alignment (ID",
			new_alignment->get_ID(),
			") is not legal and is deleted.\n\n");
		new_alignment->write_alignment(output_file,
			write_section_chars_length, NULL_VALUE, alignment_format);
		delete new_alignment;
		new_alignment = NIL;
		goto L1;
	}

	// Check to see whether new alignment is composite and mark it
	// as such if it is.

	new_alignment->check_for_composite_structure();

	// 4 CLASSIFY ALIGNMENT BY DEGREE OF MATCHING
	// AND RECOMPUTE SCORES OF THE NEW ALIGNMENT

	new_alignment->find_degree_of_matching(cnp);
	new_alignment->make_code(false);

	leaf_node->set_new_symbols_cost(new_alignment->get_new_symbols_cost());
	leaf_node->set_encoding_cost(new_alignment->get_encoding_cost());
	leaf_node->set_compression_ratio(new_alignment->get_compression_ratio());
	leaf_node->set_compression_difference(new_alignment->
		get_compression_difference());

	// 5 CHECK FOR LOW SCORING ALIGNMENT

	if (new_alignment->get_compression_difference() <= fail_score)
	{
		if (verbose)
		{
			fprintf(output_file, "Alignment ");
			new_alignment->print_ID();
			fprintf(output_file, " for node ");
			leaf_node->print_ID();
			fprintf(output_file, " has a fail score and is deleted.\n\n");
			fflush(output_file);
		}

		leaf_node->set_compression_difference(fail_score);

		delete new_alignment;
		new_alignment = NIL;
		goto L1;
	}

	if (show_al_structure)
		new_alignment->show_al_structure(leaf_node);

	// 6 CHECK WHETHER NEW ALIGNMENT IS A DUPLICATE OF ANY
	// PREVIOUSLY-FORMED ALIGNMENT

	// Match the new sequence against sequences formed earlier in
	// this cycle and previous cycles. If there is a match, delete
	// new sequence and return NIL.

	if (matching_sequence =
		new_alignment->matches_earlier_alignment())
	{
		if (verbose)
		{
			fprintf(output_file, "Alignment ");
			new_alignment->print_ID();
			fprintf(output_file, " for node ");
			leaf_node->print_ID();
			fprintf(output_file, " matches sequence ");
			matching_sequence->print_ID(),
				fprintf(output_file, " and is discarded.\n\n");
			fflush(output_file);
		}

		leaf_node->set_compression_difference(fail_score);
		delete new_alignment;
		new_alignment = NIL;
		goto L1;
	}

	// 8 DELIVER NEW ALIGNMENT

	alignments_array[leaf_index] = new_alignment;

L1:;

	if (verbose)
	{
		fprintf(output_file, "FINISHED unify_best (");
		print_pattern_cycle(true, cnp);
		fprintf(output_file, ") LEAF NODE ID = ");
		leaf_node->print_ID();
		fprintf(output_file, "\n\n");
		fflush(output_file);
	}

	return(new_alignment);

} /* unify_best */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Removes from alignments_array[] any sequences and corresponding codes
**	which are not marked as KEEP. At the same time, clear the
**	symbol_selection_array[][] ready for the next cycle.
**
** CALLING SEQUENCE:
**
**	void purge_parsing_alignments(sequence *cnp)
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	cnp:			Current New pattern.
**
** IMPLICIT INPUTS:
**
**	alignments_array[]
**
** IMPLICIT OUTPUTS:
**
**	Alignments and codes removed from alignments_array[]
**
** SIDE EFFECTS:
**
**	NONE
**
*/

void purge_parsing_alignments(sequence* cnp)
{
	sequence* sequence2;
	int row1, col1, leaf_index;
	hit_node* h_node;

	fprintf(output_file, "STARTING purge_parsing_alignments (");
	print_pattern_cycle(true, cnp);
	fprintf(output_file, ")\n\n");
	fflush(output_file);

	// Clear symbol_selection_array[][] to avoid dangling pointers.

	for (row1 = 0; row1 < MAX_NEW_ARRAY_ROWS; row1++)
	{
		for (col1 = 0; col1 < MCOLS; col1++)
			symbol_selection_array[row1][col1] = NIL;
	}

	/* Delete the sequences that are not marked as KEEP
	and retain the ones that are. */

	number_of_alignments_purged = 0;
	row1 = 0;
	while (h_node = get_leaf_nodes_in_order(&row1, &sequence2, &leaf_index))
	{
		if (h_node == NIL) break;
		if (sequence2 == NIL) continue;

		/* Check to see if sequence2 is marked as KEEP. */

		if (sequence2->get_keep() == false)
		{
			if (verbose)
			{
				fprintf(output_file, "Alignment ");
				sequence2->print_ID();
				fprintf(output_file, "%s%d%s",
					" (hit node #",
					sequence2->get_leaf_node_ID(),
					"), is purged\n\n");
			}

			delete sequence2;
			alignments_array[leaf_index] = NIL;
			number_of_alignments_purged++;
			continue;
		}
	}

	fprintf(output_file, "%s%d%s",
		"Number of alignments purged = ",
		number_of_alignments_purged,
		"\n\n");

	fprintf(output_file, "FINISHED purge_parsing_alignments (");
	print_pattern_cycle(true, cnp);
	fprintf(output_file, ")\n\n");
	fflush(output_file);

} /* purge_parsing_alignments */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Selects a sub-set of the alignments formed in the current cycle.
**
**	This function uses the symbol_selection_array[][] to ensure that
**	as many sections of New as possible are represented in the
**	selection.
**
** CALLING SEQUENCE:
**
**	void make_selection(sequence *cnp)
**
** FORMAL ARGUMENTS:
**
**	Return value:		void
**
**	cnp:				The New pattern being processed.
**
** IMPLICIT INPUTS:
**
**	old_patterns
**
** IMPLICIT OUTPUTS:
**
** SIDE EFFECTS:
**
**	NONE
**
*/

void make_selection(sequence* cnp)
{
	symbol* B_symbol1;
	alignment_element* al_el1;
	int row1, row2, col, type_of_alignment,
		max_entries_in_symbol, col_n, symbol_counter, leaf_index,
		length_of_new = cnp->get_number_of_children();
	sequence* sequence1;
	hit_node* h_node;

	row1 = 0;
	while (h_node = get_leaf_nodes_in_order(&row1, &sequence1, &leaf_index))
	{
		if (sequence1 == NIL) continue;

		type_of_alignment = sequence1->get_degree_of_matching();

		if (phase == 2 && type_of_alignment == PARTIAL) continue;

		/* Look for symbols from New in the sequence which form
		hits with symbols in Old. Make entries in the corresponding
		symbols of symbol_selection_array[][]. */

		list_for(B_symbol1, symbol, sequence1)
		{
			al_el1 = B_symbol1->get_al_el(0);
			if (al_el1->get_el_obj() == NIL) continue;
			if (al_el1->get_same_column_below() == NULL_VALUE)
				continue;

			/* al_el1 is in a symbol/column which contains at
			least one hit. Find the position of the
			symbol in New linked to this al_el. */

			col_n = al_el1->get_orig_patt_int_pos();

			/* Check whether there are any empty slots for
			this symbol position in symbol_selection_array[][].
			If there are, insert a pointer to sequence1
			and quit. If not, proceed to next section. */

			for (row2 = 0; row2 < keep_rows; row2++)
			{
				if (symbol_selection_array[row2][col_n] == NIL)
				{
					symbol_selection_array[row2][col_n] =
						sequence1;
					break;
				}
			}
		}
	}

	/* Now traverse the symbol_selection_array[][] compiling a list of the
	alignments to be retained and used as driving patterns on the
	next cycle. */

	// Driving alignments:

	for (row1 = 0; row1 < keep_rows; row1++)
	{
		for (col = 0; col < length_of_new; col++)
		{
			sequence1 = symbol_selection_array[row1][col];
			if (sequence1 == NIL) continue;
			sequence1->set_keep(true);
		}
	}

	/* Find the maximum number of entries in any symbol of the
	symbol_selection_array[][] and print it out. */

	max_entries_in_symbol = 0;
	for (col = 0; col < length_of_new; col++)
	{
		symbol_counter = 0;
		for (row1 = 0; row1 < keep_rows; row1++)
		{
			if (symbol_selection_array[row1][col] == NIL) break;
			symbol_counter++;
		}
		if (symbol_counter > max_entries_in_symbol)
			max_entries_in_symbol = symbol_counter;
	}

	fprintf(output_file, "FOR ");
	print_pattern_cycle(1, cnp);
	fprintf(output_file, "%s%s%d%s",
		",\nTHE MAXIMUM NUMBER OF ENTRIES IN A COLUMN OF ",
		"symbol_selection_array[][] IS ",
		max_entries_in_symbol,
		"\n\n");

	// Remove from alignments_array[] any alignment which is not
	// marked as KEEP. At the same time, clear the
	// symbol_selection_array[][] ready for the next cycle.

	purge_parsing_alignments(cnp);

	// Transfer alignments to parsing_alignments.

	int hit_node_row = 0;
	sequence* al1;

	while (h_node = get_leaf_nodes_in_order(&hit_node_row, &al1,
		&leaf_index))
	{
		if (al1 == NIL) continue;
		parsing_alignments->add_child(al1);
	}

	parsing_alignments->sort_by_compression_difference();

	if (show_all_parsing_alignments)
	{
		// Print out selected alignments.

		fprintf(output_file,
			"SELECTED SET OF ALIGNMENTS FORMED IN ");
		print_pattern_cycle(1, cnp);
		fprintf(output_file, "\n\n");

		list_for(al1, sequence, parsing_alignments)
		{
			if (al1->get_keep() == false) continue;
			al1->write_out_fully("SELECTED ALIGNMENT", h_node,
				write_section_chars_length, NULL_VALUE, true,
				cnp);
		}
	}

} /* make_selection */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Creates Shannon-Fano-Elias codes according to the method in
**	Cover and Thomas, pp 101-103 but modified to remove rounding
**	of values and the addition of 1. The output is put in sfe_array[].
**
**	The S-F-E method gives a code with this size:
**
**		l(x) = ceil(log_2(1 / p(x)) + 1.
**
**	This function does not construct an explicit code. It simply
**	calculates:
**
**		l(x) = log_2(1 / p(x)).
**
**	If a symbol has a frequency of 0, its bit cost is set to
**	average_symbol_type_cost. This is a purely arbitrary value
**	failing anything better.
**
** CALLING SEQUENCE:
**
**	void modified_sizes_of_sfe_codes(group *object_list)
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	object_list:	The list of objects for which sizes of codes are to
**					be calculated.
**
** IMPLICIT INPUTS:
**
**	NONE
**
** IMPLICIT OUTPUTS:
**
**	Codewords and lengths for codewords in sfe_array[].
**
** SIDE EFFECTS:
**
**	NONE
*/

void modified_sizes_of_sfe_codes(group* object_list)
{
	double total_frequency = 0;
	base_object* obj1;
	int i, fe_sfe_array, size_of_array =
		object_list->get_number_of_children();
	struct sfe_entry
	{
		base_object* obj;
		double prob_x;
		double code_size;
	} *sfe_array;

	sfe_array = new sfe_entry[size_of_array];

	/* Clear sfe_array[]. */

	for (i = 0; i < size_of_array; i++)
	{

		sfe_array[i].obj = NIL;
		sfe_array[i].prob_x = 0;
		sfe_array[i].code_size = 0;
	}

	/* Enter objects in sfe_array[] and find total frequency for objects. */

	i = 0;
	object_list->initialise();
	while (obj1 = object_list->get_next_child())
	{
		sfe_array[i].obj = obj1;
		plus_one(&i, LARGE_SCRATCH_ARRAY_SIZE,
			"sfe_array[] is too small");
		total_frequency += obj1->get_frequency();
	}

	fe_sfe_array = i;

	/* Calculate the probability of each object and enter values
	in sfe_array[]. */

	int obj_frequency;
	for (i = 0; i < fe_sfe_array; i++)
	{
		obj_frequency = sfe_array[i].obj->get_frequency();
		if (obj_frequency == 0) sfe_array[i].prob_x = 0;
		else sfe_array[i].prob_x = obj_frequency / total_frequency;
	}

	/* Calculate values for code_size. Each one is the size of the
	code for the corresponding object. */

	double prob_x;
	for (i = 0; i < fe_sfe_array; i++)
	{
		prob_x = sfe_array[i].prob_x;
		if (prob_x == 0) sfe_array[i].code_size =
			average_symbol_type_cost;
		else sfe_array[i].code_size = log_2(1 / prob_x);
	}

	/* Apply the sizes to the objects in object_list. */

	for (i = 0; i < fe_sfe_array; i++)
		sfe_array[i].obj->set_encoding_cost(sfe_array[i].code_size);

	delete[] sfe_array;

} /* modified_sizes_of_sfe_codes */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Tests whether the symbols up to and including 'end_symbol' in
**	'pattern' render that pattern unique within Old.
**
** CALLING SEQUENCE:
**
**	bool is_unique(sequence *test_pattern, symbol *end_symbol,
**		sequence **matching_pattern)
**
** FORMAL ARGUMENTS:
**
**	Return value:		true if the combination of symbols up to and
**						including 'end_symbol' render the
**						test_pattern unique within old_patterns,
**						false otherwise.
**
**	test_pattern:		A pattern within old_patterns.
**	end_symbol: 		A symbol in test_pattern which marks the end
**						of the set of symbols to be tested.
**	matching_pattern:	A pointer to a variable which is NIL if
**						test_pattern is unique or takes the value
**						of a pattern where the leading symbols match
**						the test_pattern.
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

bool is_unique(sequence* test_pattern, symbol* end_symbol,
	sequence** matching_pattern)
{
	list_element* el_pos1 = NIL, * el_pos2, * el_pos3;
	sequence* pattern;
	symbol* symbol1, * symbol2;

	while (pattern = (sequence*)
		new_patterns->get_next_child_by_el_pos(&el_pos1))
	{
		if (pattern == test_pattern) continue;
		el_pos2 = el_pos3 = NIL;
		list_for_el_pos(symbol1, symbol, test_pattern, el_pos2)
		{
			symbol2 = (symbol*)
				pattern->get_next_child_by_el_pos(&el_pos3);
			if (symbol2 == NIL)
			{
				*matching_pattern = pattern;
				return(false);
			}
			if (!symbol1->name_matches(symbol2)) break;
			if (symbol1 == end_symbol)
			{
				*matching_pattern = pattern;
				return(false);
			}
		}
	}

	el_pos1 = NIL;

	while (pattern = (sequence*)
		old_patterns->get_next_child_by_el_pos(&el_pos1))
	{
		if (pattern == test_pattern) continue;
		el_pos2 = el_pos3 = NIL;
		while (symbol1 = (symbol*)
			test_pattern->get_next_child_by_el_pos(&el_pos2))
		{
			symbol2 = (symbol*)
				pattern->get_next_child_by_el_pos(&el_pos3);
			if (symbol2 == NIL)
			{
				*matching_pattern = pattern;
				return(false);
			}
			if (!symbol1->name_matches(symbol2)) break;
			if (symbol1 == end_symbol)
			{
				*matching_pattern = pattern;
				return(false);
			}
		}
	}
	*matching_pattern = NIL;
	return(true);
} /* is_unique */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	A function that prints out, in sorted order,
**	the compression scores of all the hit sequences in the
**	hit structure.
**
** CALLING SEQUENCE:
**
**	void print_hit_scores()
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
**
*/

void print_hit_scores()
{
	int row, leaf_index;
	sequence* al1;
	hit_node* h_node;

	fprintf(output_file, "COMPRESSION SCORES OF ALIGNMENTS:\n\n");

	row = 0;
	while (h_node = get_leaf_nodes_in_order(&row, &al1, &leaf_index))
		fprintf(output_file, "%s%1.2f%s%1.2f%s",
			"CR = ",
			h_node->get_compression_ratio(),
			", CD = ",
			h_node->get_compression_difference(),
			"\n");
	fprintf(output_file, "\n");
	fflush(output_file);
} /* print_hit_scores */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Sets up a new (empty) structure for storing hit nodes.
**
** CALLING SEQUENCE:
**
**	void set_up_hit_structure()
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

void set_up_hit_structure()
{
	int i;

	hit_root = new hit_node; // It is assumed that any pre-existing
		// tree of hit nodes was deleted towards the end of recognise().

	for (i = 0; i < hit_structure_rows; i++)
	{
		leaf_array[i] = NIL;
		alignments_array[i] = NIL;
		sort_array[i] = i;
	}
	fe_sort = 0;
} /* set_up_hit_structure */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Called from write_hit_structure().
**
** CALLING SEQUENCE:
**
**	void shs(hit_node *h_node, int indentation)
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	h_node:			The top node of the structure.
**	indendation:	The amount of indentation.
**
** IMPLICIT INPUTS:
**
**	The hit structure
**
** IMPLICIT OUTPUTS:
**
**	Output
**
** SIDE EFFECTS:
**
**	NONE
*/

void shs(hit_node* h_node, int indentation)
{
	hit_node* child;
	int i;

	for (i = 0; i < indentation; i++) fprintf(output_file, "  ");

	if (h_node != hit_root)
	{
		h_node->print_ID();
		fprintf(output_file, ": ");
		(h_node->get_driving_pattern())->print_ID();
		fprintf(output_file, "%s%s%s%d%s",
			", ",
			(h_node->get_driving_symbol())->get_name(),
			", d_pos ",
			h_node->get_driving_int_pos(),
			"; ");
		(h_node->get_target_pattern())->print_ID();
		fprintf(output_file, "%s%s%s%d%s%1.2f%s%1.2f%s",
			", ",
			(h_node->get_target_symbol())->get_name(),
			", t_pos ",
			h_node->get_target_int_pos(),
			" (CR = ",
			h_node->get_compression_ratio(),
			", CD = ",
			h_node->get_compression_difference(),
			")");
	}
	else fprintf(output_file, "hit_root");
	fprintf(output_file, "\n");

	h_node->initialise();
	while (child = (hit_node*)h_node->get_next_child())
		shs(child, indentation + 1);
} /* shs */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Prints out hit structure (for diagnosis).
**
** CALLING SEQUENCE:
**
**	void write_hit_structure(hit_node *h_node, int indentation) ;
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	h_node:			The top node of the structure.
**	indendation:	The amount of indentation.
**
** IMPLICIT INPUTS:
**
**	The hit structure
**
** IMPLICIT OUTPUTS:
**
**	Output
**
** SIDE EFFECTS:
**
**	NONE
*/

void write_hit_structure(hit_node* h_node, int indentation)
{
	write_message("HIT STRUCTURE:");
	shs(h_node, indentation);
	fprintf(output_file, "\n");
	fflush(output_file);
} /* write_hit_structure */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Returns a new unique id number from unique_id_number.
**
** CALLING SEQUENCE:
**
**	int get_new_unique_id_number()
**
** FORMAL ARGUMENTS:
**
**	Return value:	The new unique id number.
**
** IMPLICIT INPUTS:
**
**	context_number
**
** IMPLICIT OUTPUTS:
**
**	A new value for unique_id_number.
**
** SIDE EFFECTS:
**
**	NONE
**
*/

int get_new_unique_id_number()
{
	double unique_id_number_length = ceil(log10(unique_id_number));
	if (unique_id_number_length > (double)STRING_LENGTH - 2)
		abort_run("String too long in get_new_unique_id_number()");
	return(unique_id_number++);
} // get_new_unique_id_number

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Creates a symbol from context_number and adds it to
**	generalisation_list1.
**
** CALLING SEQUENCE:
**
**	void add_symbol_to_generalisation_list1(int context_number)
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
**	Symbol added to generalisation_list1.
**
** SIDE EFFECTS:
**
**	NONE
*/

void add_symbol_to_generalisation_list1(int context_number)
{
	char symbol_name[STRING_LENGTH];
	sprintf(symbol_name, "%d", context_number);

	symbol* cs = new symbol("", 1, NULL_VALUE);
	cs->set_name(symbol_name);
	bool symbol_added =
		generalisation_list1->add_symbol_without_copies_or_duplicates(cs);
	if (symbol_added == false)
		delete cs;
} // add_symbol_to_generalisation_list1

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Returns a new context number from code_number. This function also
**	creates a symbol with a name which is the string version of the
**	context number and adds this symbol to generalisation_list1.
**
** CALLING SEQUENCE:
**
**	int get_new_context_number()
**
** FORMAL ARGUMENTS:
**
**	Return value:	The new context number.
**
** IMPLICIT INPUTS:
**
**	context_number
**
** IMPLICIT OUTPUTS:
**
**	A new value for context_number.
**
** SIDE EFFECTS:
**
**	NONE
**
*/

int get_new_context_number()
{
	double context_number_length = ceil(log10(context_number));
	if (context_number_length > (double)STRING_LENGTH - 2)
		abort_run("String too long in get_new_context_number()");

	return(context_number++);
} // get_new_context_number

/***********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Makes a new discrimination symbol using the integer value
**	stored in an internal static variable.
**
** CALLING SEQUENCE:
**
**	symbol *make_unique_id_symbol()
**
** FORMAL ARGUMENTS:
**
**	Return value:			An object which is a new code symbol.
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

symbol* make_unique_id_symbol()
{
	symbol* cs;
	char symbol_name[STRING_LENGTH];

	// Make new unique id symbol.

	cs = new symbol("", 1, NULL_VALUE);

	int unique_id_num = get_new_unique_id_number();
	sprintf(symbol_name, "%s%d",
		"#",
		unique_id_num);
	cs->set_type(UNIQUE_ID_SYMBOL);
	cs->set_status(IDENTIFICATION);
	cs->set_name(symbol_name);

	// Assign a value for the bit_cost. For the time
	// being, all code symbols will have a bit_cost of CODE_BIT_COST bits.

	cs->assign_frequency_and_cost();

	return(cs);
} /* make_unique_id_symbol */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	The frequencies of symbol types are used to compute the
**	information cost of each symbol type using a simplified version
**	of the Shannon-Fano-Elias method. Symbols with the type DATA_SYMBOL
**	have their information costs increased by cost_factor. This is
**	so that they can be treated as relatively large 'chunks' of
**	information, so that it becomes feasible to encode relatively
**	small patterns.
**
**	Calculates average_symbol_type_cost.
**
**	In addition, the function assigns the frequency of its type
**	to each symbol in the corpus and the associated bit_cost.
**
** CALLING SEQUENCE:
**
**	void calculate_and_assign_frequencies_and_costs(group *symbol_types,
**		bool ref_is_corpus)
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	symbol_types:	A set of symbol types, each one with
**					an associated frequency.
**	ref_is_corpus:	if true, the reference patterns are the whole corpus,
**					(normally just the New patterns), otherwise Old.
**
** IMPLICIT INPUTS:
**
**	The corpus
**
** IMPLICIT OUTPUTS:
**
**	bit_cost value on each symbol in the corpus.
**
** SIDE EFFECTS:
**
**	NONE
*/

void calculate_and_assign_frequencies_and_costs(group* symbol_types, bool ref_is_corpus)
{
	symbol* symbol1;
	double total_symbol_type_cost = 0;
	double temp_cost;
	double total_CONTEXT_SYMBOL_type_cost = 0;

	int alphabet_size = symbol_types->get_number_of_children(),
		number_of_CONTEXT_types = 0;

	// Calculate sizes of symbols in symbol_types using a modified
	// version of the SFE method.

	modified_sizes_of_sfe_codes(symbol_types);

	// For all symbols that are DATA_SYMBOLs, inflate the bit_cost
	// by the cost_factor.

	list_for(symbol1, symbol, symbol_types)
	{
		if (symbol1->get_type() == DATA_SYMBOL)
		{
			temp_cost = symbol1->get_bit_cost();
			temp_cost *= cost_factor;
			symbol1->set_bit_cost(temp_cost);
		}
	}

	/* Print results. */

	if (ref_is_corpus)
		fprintf(output_file, "%s%s",
			"FREQUENCIES OF SYMBOL TYPES IN CORPUS (NORMALLY JUST NEW PATTERNS)\n",
			"AND INFORMATION COSTS (IN BITS)\n\n");
	else
	{
		fprintf(output_file, "%s%s",
			"FREQUENCIES OF SYMBOL TYPES IN OLD PATTERNS\n",
			"AND INFORMATION COSTS (IN BITS)\n(");
		print_pattern_cycle(false, current_new_pattern);
		fprintf(output_file, ")\n\n");
	}
	write_lines(output_file, "", START);
	list_for(symbol1, symbol, symbol_types)
	{
		fprintf(output_file, "%s%s%d%s%1.2f%s",
			symbol1->get_name(),
			", Fr = ", symbol1->get_frequency(),
			", bit_cost = ", symbol1->get_bit_cost(),
			"\n");
		total_symbol_type_cost += symbol1->get_bit_cost();

		if (symbol1->get_type() != CONTEXT_SYMBOL
			&& symbol1->get_type() != UNIQUE_ID_SYMBOL)
			continue;

		total_CONTEXT_SYMBOL_type_cost += symbol1->get_bit_cost();
		number_of_CONTEXT_types++;
	}

	fprintf(output_file, "\n");

	average_symbol_type_cost =
		total_symbol_type_cost / alphabet_size;
	if (total_CONTEXT_SYMBOL_type_cost <= 0
		|| number_of_CONTEXT_types <= 0)
		average_CONTEXT_SYMBOL_type_cost = CODE_BIT_COST;
	else average_CONTEXT_SYMBOL_type_cost =
		total_CONTEXT_SYMBOL_type_cost / number_of_CONTEXT_types;

	fprintf(output_file, "%s%1.2f%s",
		"Average of bit_costs for symbol types = ",
		average_symbol_type_cost, "\n\n");

	// Now assign the values for frequency and bit_cost
	// to the symbol instances in the corpus.

	assign_symbol_frequencies_and_costs(symbol_types);

} /* calculate_and_assign_frequencies_and_costs */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Gets a new empty entry in leaf_array[]. If the number of entries
**	is exhausted, it returns false, otherwise true.
**
** CALLING SEQUENCE:
**
**	int get_new_leaf_entry() ;
**
** FORMAL ARGUMENTS:
**
**	Return value:	The leaf_index value for the next available empty
**					cell in leaf_array. If none is available, NULL_VALUE
**					is returned.
**
** IMPLICIT INPUTS:
**
**	fe_sort
**
** IMPLICIT OUTPUTS:
**
**	new value for fe_sort
**
** SIDE EFFECTS:
**
**	NONE
*/

int get_new_leaf_entry()
{
	int leaf_index;

	if (fe_sort >= hit_structure_rows) return(NULL_VALUE);
	leaf_index = sort_array[fe_sort++];
	return(leaf_index);
} /* get_new_leaf_entry */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Compiles an unordered list of the leaf nodes with the smallest
**	values for CD.
**
** CALLING SEQUENCE:
**
**	void find_worst_leaf_nodes(sequence *cnp)
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	cnp:			Current New pattern.
**
** IMPLICIT INPUTS:
**
**	The hit structure.
**
** IMPLICIT OUTPUTS:
**
**	An unordered list of the worst leaf nodes in half_list[].
**
** SIDE EFFECTS:
**
**	NONE
**
*/

void find_worst_leaf_nodes(sequence* cnp)
{
	int half_fe_sort = fe_sort / 2, row1, row2, best_worst_hl_index = 0;
	double best_worst_CD, temp_CD, time_started = clock(),
		time_finished, time_difference_secs, time_per_item_secs;
	hit_node* h_node1;

	fe_half_list = 0;

	if (half_fe_sort < 1) abort_run("Too few entries in half_list[]");
	if (half_fe_sort > half_hit_structure_rows)
		abort_run("Anomalous value for half_fe_sort in find_mid");

	// Start by filling up half_list with the first half of the
	// nodes in leaf_array[]. At the same time, find the
	// best leaf node in the half list.

#if PRINT_CD_VALUES
	fprintf(output_file, "CD values for leaf nodes:\n\n");
#endif

	best_worst_CD = fail_score;
	for (row1 = 0; row1 < half_fe_sort; row1++)
	{
		h_node1 = leaf_array[sort_array[row1]];
		if (h_node1 == NIL)
			abort_run("NIL value for h_node1 \
		in find_worst_leaf_nodes");
		half_list[fe_half_list] = h_node1;
		temp_CD = h_node1->get_compression_difference();

#if PRINT_CD_VALUES
		fprintf(output_file, "%1.2f", temp_CD);
		print_counter++;
		if (print_counter < 10) fprintf(output_file, ", ");
		else
		{
			fprintf(output_file, ",\n");
			print_counter = 0;
		}
#endif

		if (temp_CD > best_worst_CD)
		{
			best_worst_CD = temp_CD;
			best_worst_hl_index = fe_half_list;
		}
		fe_half_list++;
	}

	// Now continue with the second half of sort array[] comparing CD
	// values in the second half with CD values in half_list[]. If
	// a CD value is worse than the best value in half_list, it replaces
	// that value.

	for (row1 = row1; row1 < fe_sort; row1++)
	{
		h_node1 = leaf_array[sort_array[row1]];
		if (h_node1 == NIL)
			abort_run("NIL value for h_node1 \
		in find_worst_leaf_nodes");
		temp_CD = h_node1->get_compression_difference();

#if PRINT_CD_VALUES
		fprintf(output_file, "%1.2f", temp_CD);
		print_counter++;
		if (row1 + 1 >= fe_sort) fprintf(output_file, ".\n\n");
		else if (print_counter < 10) fprintf(output_file, ", ");
		else
		{
			fprintf(output_file, ",\n");
			print_counter = 0;
		}
#endif

		if (temp_CD >= best_worst_CD) continue;

		// temp_CD is worse than best_worst_CD so the corresponding
		// hit node replaces the hit node for the
		// best_worst_CD in half_list[]. Then it is necesssary to
		// find a new value for best_worst_CD and its corresponding
		// value for best_worst_hl_index.

		half_list[best_worst_hl_index] = h_node1;

		best_worst_CD = fail_score;
		for (row2 = 0; row2 < half_fe_sort; row2++)
		{
			temp_CD = half_list[row2]->
				get_compression_difference();
			if (temp_CD > best_worst_CD)
			{
				best_worst_CD = temp_CD;
				best_worst_hl_index = row2;
			}
		}
	}

#if PRINT_CD_VALUES

	fprintf(output_file, "%s%1.2f%s",
		"Value of best_worst_CD = ",
		best_worst_CD,
		"\n\n");

	fprintf(output_file, "Values in half_list[]:\n\n");

	print_counter = 0;
	for (row1 = 0; row1 < half_fe_sort; row1++)
	{
		h_node1 = half_list[row1];
		temp_CD = h_node1->get_compression_difference();
		fprintf(output_file, "%1.2f", temp_CD);
		print_counter++;
		if (row1 + 1 >= half_fe_sort) fprintf(output_file, ".\n\n");
		else if (print_counter < 10) fprintf(output_file, ", ");
		else
		{
			fprintf(output_file, ",\n");
			print_counter = 0;
		}
	}
#endif

	time_finished = clock();
	time_difference_secs = (time_finished - time_started) / CLOCKS_PER_SEC;
	time_per_item_secs = time_difference_secs / (double)fe_sort;

	fprintf(output_file,
		"Finding worst leaf nodes started and finished.\n(");

	print_pattern_cycle(true, cnp);

	fprintf(output_file, "%s%d%s%g%s%g%s",
		")\n\nNumber of items = ",
		fe_sort,
		",\nSifting time = ",
		time_difference_secs,
		" seconds,\nSifting time per item = ",
		time_per_item_secs,
		" seconds\n\n");
} // find_worst_leaf_nodes

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Purges the hit structure. There is an assumption that the
**	leaf nodes have been sorted.
**
** CALLING SEQUENCE:
**
**	void purge_hit_structure(sequence *cnp) ;
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	cnp:			Current New pattern
**
** IMPLICIT INPUTS:
**
**	The hit structure
**
** IMPLICIT OUTPUTS:
**
**	The purged hit structure
**
** SIDE EFFECTS:
**
**	NONE
*/

void purge_hit_structure(sequence* cnp)
{
	hit_node* h_node, * parent_h_node, ** leaf_entry;
	int row, fe_sort_at_start = fe_sort;
	double start_end_fe_sort;

	fprintf(output_file, "PURGING OF HIT_STRUCTURE (");
	print_pattern_cycle(true, cnp);
	fprintf(output_file, "%s%d%s",
		")\n\nValue of fe_sort at start = ",
		fe_sort,
		"\n\n");

	find_worst_leaf_nodes(cnp); // Compiles an unordered list of the
	// worst leaf nodes. Then these nodes can be purged.

	/* half_list[] contains the bottom 50% of the leaf nodes. Delete
	each of these hit nodes, pruning each branch
	up to the first node that has more than one child. */

	for (row = 0; row < fe_half_list; row++)
	{
		h_node = half_list[row];
		if (h_node == NIL)
			abort_run("NIL value for h_node in \
		purge_hit_structure");

		leaf_entry = h_node->get_leaf_entry(); // This is the cell
			// in leaf_array[] which contains a pointer to
			// a pointer to a leaf node.
		(*leaf_entry) = NIL;
		while (h_node != hit_root)
		{
			parent_h_node = h_node->get_hn_parent();
			parent_h_node->extract_child(h_node);
			delete h_node;

			/* Test whether the parent_h_node has any other
			children. If so, stop extracting nodes
			up this branch of the tree. */

			if (parent_h_node->get_first_child() != NIL) break;
			h_node = parent_h_node;
		}
	}

	// Now compute new values for fe_sort so that the NIL values in
	// leaf_array[] are not referenced by any cells in fe_sort[] between
	// 0 and fe_sort.

	fe_sort = 0;
	for (row = 0; row < hit_structure_rows; row++)
		if (leaf_array[row] != NIL) sort_array[fe_sort++] = row;

	int i = fe_sort;
	for (row = 0; row < hit_structure_rows; row++)
		if (leaf_array[row] == NIL) sort_array[i++] = row;

	start_end_fe_sort = (double)fe_sort / (double)fe_sort_at_start;
	fprintf(output_file, "%s%d%s%1.2f%s",
		"Value of fe_sort at end = ",
		fe_sort,
		"\n\nCurrent value of fe_sort is ",
		start_end_fe_sort,
		" of value before purging\n\n");

	fprintf(output_file, "PURGE IS COMPLETED (");
	print_pattern_cycle(true, cnp);
	fprintf(output_file, ")\n\n");

} /* purge_hit_structure */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Estimates compression scores for hit sequences added to the
**	hit structure.
**
**	The adjustment for gaps between hits used in earlier versions
**	has been dropped.
**
** CALLING SEQUENCE:
**
**	void estimate_compression_scores(double *NSC,
**		double *EC, hit_node *parent_h_node)
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	NSC:			A pointer to a double variable in the calling function.
**	EC:				A pointer to a double variable in the calling function.
**	parent_h_node:	The parent hit node.
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

void estimate_compression_scores(double* NSC,
	double* EC, hit_node* parent_h_node)
{
	// The adjustment for gaps between hits used in earlier versions
	// has been dropped.

	if (cycle == 1)
	{
		/* Driving pattern is one of the New patterns. */

		if (parent_h_node == hit_root)
		{
			*NSC = master_driving_symbol->get_bit_cost() +
				master_target_pattern->get_new_symbols_cost();
			*EC = master_target_pattern->get_encoding_cost();
		}
		else
		{
			*NSC = parent_h_node->get_new_symbols_cost() +
				master_driving_symbol->get_bit_cost();
			*EC = parent_h_node->get_encoding_cost();
		}
	}
	else
	{
		// Driving pattern is an alignment from a previous cycle.

		if (parent_h_node == hit_root)
		{
			*NSC = master_driving_pattern->get_new_symbols_cost() +
				master_target_pattern->get_new_symbols_cost();
			*EC = master_driving_pattern->get_encoding_cost() +
				master_target_pattern->get_encoding_cost();
		}
		else
		{
			*NSC = parent_h_node->get_new_symbols_cost();
			*EC = parent_h_node->get_encoding_cost();
		}

		// Make an adjustment to EC if the current hit involves at
		// least one IDENTIFICATION symbol.

		if (master_driving_symbol->get_status() == IDENTIFICATION
			|| master_target_symbol->get_status() == IDENTIFICATION)
			*EC -= master_target_symbol->get_bit_cost();
	}
} // estimate_compression_scores

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	This function makes a hit_node. It is called from add_hit().
**
** CALLING SEQUENCE:
**
**	void make_hit_node(hit_node *parent_h_node, sequence *pattern1)
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	parent_h_node:	The parent of the hit_node to be created.
**	pattern1:		The pattern being parsed.
**
** IMPLICIT INPUTS:
**
**	NONE
**
** IMPLICIT OUTPUTS:
**
**	A new hit_node attached to a chain of 'pending' hit nodes.
**
** SIDE EFFECTS:
**
**	NONE
**
*/

void make_hit_node(hit_node* parent_h_node, sequence* pattern1)
{
	hit_node* copy_h_node = new hit_node(*hn_master);
	double NSC, EC;

	/* Estimate the compression score for the hit sequence
	up to and including the new hit. */

	estimate_compression_scores(&NSC, &EC, parent_h_node);

	if (EC < 0) EC = 0;

	copy_h_node->set_new_symbols_cost(NSC);
	copy_h_node->set_encoding_cost(EC);
	copy_h_node->set_compression_ratio(EC / NSC);
	copy_h_node->set_compression_difference(NSC - EC);

	/* Attach the new copy to the hit_node_array[]. */

	hit_node_array[fe_hit_node_array].new_hit_node = copy_h_node;
	hit_node_array[fe_hit_node_array].hn_parent = parent_h_node;
	plus_one(&fe_hit_node_array, HIT_NODE_ARRAY_SIZE,
		"hit_node_array[] is too small.");
	copy_h_node->set_hn_parent(parent_h_node);

	new_hits++;

} /* make_hit_node */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Adds one or more records of a hit to the hit structure.
**	The function traverses the tree of hit_nodes recursively.
**	New nodes representing the hit are created wherever they are needed.
**	They are not added to the hit structure immediately but are
**	attached to a chain of hit nodes and then (in
**	update_hit_structure()) added	to the hit structure if there
**	is enough space to accommodate them. If there is not enough space,
**	the hit structure is purged and add_hit() is called again.
**
** CALLING SEQUENCE:
**
**	bool add_hit(hit_node *parent_h_node, sequence *cnp)
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if a hit is added, false otherwise.
**
**	parent_h_node:	The top node of the (sub) tree.
**	cnp:			The New pattern being processed.
**
** IMPLICIT INPUTS:
**
**	Global structures, including hn_master.
**
** IMPLICIT OUTPUTS:
**
**	Global structures
**
** SIDE EFFECTS:
**
**	NONE
*/

bool add_hit(hit_node* parent_h_node, sequence* cnp)
{
	hit_node* child;
	bool hit_added = false;

	parent_h_node->initialise();
	while (child = (hit_node*)parent_h_node->get_next_child())
	{
		// compression_ratio == fail_score marks the child as
		// 'dead'. Nothing must be added below this node.

		if (child->get_compression_difference() <= fail_score)
			continue;

		if (master_driving_pattern !=
			child->get_driving_pattern()) continue;

		if (master_driving_int_pos <=
			child->get_driving_int_pos())
			continue;

		if (master_target_pattern !=
			child->get_target_pattern()) continue;

		if (master_target_int_pos <=
			child->get_target_int_pos())
			continue;

		hit_added = add_hit(child, cnp);
	}

	if (hit_added) return(true);

	// There is no child at this level which match all the criteria
	// for going deeper in the tree of hit nodes. So the hit can be
	// installed as a new leaf node child of parent_h_node,
	// provided the gap between driving symbols in the new node and
	// its parent are less than max_driving_gap and likewise for
	// the target symbols.

	if (parent_h_node != hit_root)
	{
		if ((master_driving_int_pos - parent_h_node->
			get_driving_int_pos() - 1) > max_driving_gap)
			return(false);
		if ((master_target_int_pos - parent_h_node->
			get_target_int_pos() - 1) > max_target_gap)
			return(false);
	}

	make_hit_node(parent_h_node, cnp);

	return(true);
} /* add_hit */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Updates the data structures which record hits.
**
** CALLING SEQUENCE:
**
**	void update_hit_structure(sequence *cnp)
**
** FORMAL ARGUMENTS:
**
**	Return value:		void
**
**	cnp:				The New pattern being processed.
**
** IMPLICIT INPUTS:
**
**	The hit structure
**
** IMPLICIT OUTPUTS:
**
**	The hit structure
**
** SIDE EFFECTS:
**
**	NONE
*/

void update_hit_structure(sequence* cnp)
{
	hit_node* parent_h_node, * node,
		** leaf_entry, * new_h_node;
	int i, leaf_index;

	new_hits = 0;
	fe_hit_node_array = 0;

	add_hit(hit_root, cnp);

	/* Check whether there is enough space in leaf_array[]. If not,
	purge the hit structure, clear the chain of hit nodes
	and try again. */

	if (fe_sort + new_hits >= hit_structure_rows)
	{
		purge_hit_structure(cnp);

		/* Now delete the new_hit_nodes in the hit_node_array[]. */

		for (i = 0; i < fe_hit_node_array; i++)
		{
			delete hit_node_array[i].new_hit_node;
			hit_node_array[i].hn_parent = NIL;
		}

		new_hits = 0;
		fe_hit_node_array = 0;

		add_hit(hit_root, cnp);

		if (fe_sort + new_hits >= hit_structure_rows)
			abort_run("Too little space in leaf_array[]");
	}

	/* Now put the copies of hn_master in their proper places in
	the hit_structure. */

	for (i = 0; i < fe_hit_node_array; i++)
	{
		new_h_node = hit_node_array[i].new_hit_node;
		parent_h_node = hit_node_array[i].hn_parent;

		/* Check whether the parent_h_node is a leaf node. If it is,
		use the parent_h_node's entry in leaf_array[]. If not,
		check whether the new_h_node is NIL. If it is, take a
		new entry in leaf_array[]. If the parent_h_node is
		the hit_root then, although at first it is a leaf node,
		its entry in leaf_array[] is not available so a new
		entry must be taken. */

		if (parent_h_node == hit_root)
		{
			leaf_index = get_new_leaf_entry();
			if (leaf_index == NULL_VALUE)
				abort_run("No leaf_array[] entries left");
			leaf_array[leaf_index] = new_h_node;
			new_h_node->set_leaf_entry(&leaf_array[leaf_index]);
		}
		else
		{
			parent_h_node->initialise();
			node = (hit_node*)parent_h_node->get_next_child();
			if (node == NIL)
			{
				// parent_h_node is itself a leaf node.

				leaf_entry = parent_h_node->get_leaf_entry();
				(*leaf_entry) = new_h_node;
				new_h_node->set_leaf_entry(leaf_entry);
				parent_h_node->set_leaf_entry(NIL);
			}
			else
			{
				// parent_h_node is not a leaf node but
				// the new_hit_node will be a new leaf node.

				leaf_index = get_new_leaf_entry();
				if (leaf_index == NULL_VALUE)
					abort_run("No leaf_array[] entries \
				left");
				leaf_array[leaf_index] = new_h_node;
				new_h_node->
					set_leaf_entry(&leaf_array[leaf_index]);
			}
		}
		parent_h_node->add_child(new_h_node);
	}

	/* Now clear the hit_node_array[] to avoid problems when
	hit nodes are deleted. */

	for (i = 0; i < HIT_NODE_ARRAY_SIZE; i++)
	{
		hit_node_array[i].new_hit_node = NIL;
		hit_node_array[i].hn_parent = NIL;
	}
} /* update_hit_structure */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Tests whether any one symbol from Old is being matched
**	with itself. If it is, this is an illegal hit which is ignored.
**	If two matching columns contain the same symbol from New, this
**	is accepted because there is still a bona fide match between symbols
**	in Old. When the new alignment is formed, the two appearances
**	of one symbol in New can be merged.
**
**	If a New pattern is matched with itself, any one instance of a
**	symbol cannot be matched with itself.
**
** CALLING SEQUENCE:
**
**	bool anomalous_hit(symbol *driving_symbol, int driving_pattern_depth,
**		symbol *target_symbol, int target_pattern_depth)
**
** FORMAL ARGUMENTS:
**
**	Return value:			true if the hit anomalous. false otherwise.
**
**	driving_symbol:			The driving symbol.
**	driving_pattern_depth:	The depth of the driving pattern.
**	target_symbol:			The target symbol.
**	target_pattern_depth:	The depth of the target pattern.
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

bool anomalous_hit(symbol* driving_symbol, int driving_pattern_depth,
	symbol* target_symbol, int target_pattern_depth)
{
	symbol* d_symbol, * t_symbol;
	int driving_row, target_row;

	if (driving_symbol == target_symbol) return(true);

	if (driving_pattern_depth == 1 && target_pattern_depth == 1)
		// Since the two symbols have already been compared
		// and given a negative result, we know that
		// the two symbols are not the same.
		return(false);

	if (driving_pattern_depth == 1)
	{
		// We know that the target_pattern_depth must be > 1.

		for (target_row = 0; target_row < target_pattern_depth;
			target_row++)
		{
			t_symbol = target_symbol->get_row_symbol(target_row);
			if (t_symbol == NIL) continue;
			if (driving_symbol == t_symbol) return(true);
		}
		return(false);
	}

	if (target_pattern_depth == 1)
	{
		// We know that the driving_pattern_depth must be > 1.

		for (driving_row = 0; driving_row < driving_pattern_depth;
			driving_row++)
		{
			d_symbol = driving_symbol->get_row_symbol(driving_row);
			if (d_symbol == NIL) continue;
			if (target_symbol == d_symbol) return(true);
		}
		return(false);
	}

	// driving_pattern_depth and target_pattern_depth must both be > 1.

	for (driving_row = 0; driving_row < driving_pattern_depth;
		driving_row++)
	{
		d_symbol = driving_symbol->get_row_symbol(driving_row);
		if (d_symbol == NIL) continue;
		for (target_row = 0; target_row < target_pattern_depth;
			target_row++)
		{
			t_symbol = target_symbol->get_row_symbol(target_row);
			if (t_symbol == NIL) continue;
			if (d_symbol == t_symbol) return(true);
		}
	}
	return(false);
} /* anomalous_hit */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Compares two symbols and makes various tests to ensure that any
**	hit is a valid hit.
**
** CALLING SEQUENCE:
**
**	bool match_symbols(sequence *driving_pattern, symbol *driving_symbol,
**		list_element *dr_el_pos, sequence *target_pattern,
**		symbol *target_symbol, list_element *ts_el_pos)
**
** FORMAL ARGUMENTS:
**
**	Return value:		true if all tests are passed, false otherwise.
**
**	driving_pattern:	The driving pattern.
**	driving_symbol: 	The driving symbol.
**	dr_el_pos:			The el_pos of the driving symbol.
**	target_pattern: 	The target pattern.
**	target_symbol:		The target symbol.
**	ts_el_pos:			The el_pos of the target symbol.
**
** IMPLICIT INPUTS:
**
**	NONE
**
** IMPLICIT OUTPUTS:
**
**	Additions to the hit structure
**
** SIDE EFFECTS:
**
**	NONE
**
*/

bool match_symbols(sequence* driving_pattern, symbol* driving_symbol,
	list_element* dr_el_pos, sequence* target_pattern,
	symbol* target_symbol, list_element* ts_el_pos)
{
	if (driving_symbol->name_matches(target_symbol) == false)
		return(false);

	int driving_pattern_depth = driving_pattern->get_sequence_depth(),
		target_pattern_depth = target_pattern->get_sequence_depth();

	if (anomalous_hit(driving_symbol, driving_pattern_depth, target_symbol,
		target_pattern_depth))
		return(false);

	return(true);

} // match_symbols

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Compares a driving pattern/alignment with a target pattern or
**	alignment. Brackets are matched only in a 'legal' manner.
**
** CALLING SEQUENCE:
**
**	void compare_patterns(sequence *driving_pattern,
**		sequence *target_pattern, sequence *cnp)
**
** FORMAL ARGUMENTS:
**
**	Return value:			void.
**
**	driving_pattern:		The pattern from Old which 'drives' the
**							search for matching patterns.
**	target_pattern:			The target pattern.
**	cnp:					The New pattern being processed.
**
** IMPLICIT INPUTS:
**
**	old_patterns
**
** IMPLICIT OUTPUTS:
**
**	NONE
**
** SIDE EFFECTS:
**
**	NONE
*/

void compare_patterns(sequence* driving_pattern,
	sequence* target_pattern, sequence* cnp)
{
	symbol* driving_symbol, * target_symbol;
	int driving_symbol_int_pos, target_symbol_int_pos, dr_status, t_status;
	list_element* dr_el_pos = NIL, * t_el_pos;

	list_for_el_pos(driving_symbol, symbol, driving_pattern, dr_el_pos)
	{
		hn_master->set_driving_symbol(driving_symbol);
		master_driving_symbol = driving_symbol;
		driving_symbol_int_pos = dr_el_pos->get_position();
		hn_master->set_driving_int_pos(driving_symbol_int_pos);
		master_driving_int_pos = driving_symbol_int_pos;

		t_el_pos = NIL;
		while (target_symbol = (symbol*)target_pattern->
			get_next_child_by_el_pos(&t_el_pos))
		{
			// When the driving pattern is the same as the
			// target pattern, the test which follows
			// prevents the comparison of symbol A with symbol B
			// and then, later, the comparison of symbol B
			// with symbol A.

			target_symbol_int_pos = t_el_pos->get_position();

			if (driving_pattern == target_pattern)
				if (target_symbol_int_pos >= driving_symbol_int_pos)
					break;

			// Constraint on the matching of IDENTIFICATION (or BOUNDARY_MARKER)
			// symbols against CONTENTS symbols and vice versa.

			if (id_c_symbol_constraint)
			{
				dr_status = driving_symbol->get_status();
				t_status = target_symbol->get_status();
				if (dr_status == IDENTIFICATION
					|| dr_status == BOUNDARY_MARKER)
				{
					if (t_status != CONTENTS) continue;
				}
				else if (dr_status == CONTENTS)
				{
					if (t_status != IDENTIFICATION
						&& t_status != BOUNDARY_MARKER)
						continue;
				}
			}

			if (match_symbols(driving_pattern, driving_symbol,
				dr_el_pos, target_pattern, target_symbol, t_el_pos))
			{
				hn_master->set_target_symbol(target_symbol);
				master_target_symbol = target_symbol;
				hn_master->set_target_int_pos(target_symbol_int_pos);
				master_target_int_pos = target_symbol_int_pos;

				update_hit_structure(cnp);
			}
		}

		if (phase == 1 && cycle == 1)
		{
			if (driving_pattern == current_new_pattern
				&& target_pattern == receptacle_pattern)
			{
				symbol* copy_driving_symbol = new symbol(*driving_symbol);
				copy_driving_symbol->set_status(CONTENTS);
				receptacle_pattern->add_child(copy_driving_symbol);
			}
		}
	}
} // compare_patterns

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Finds a match for a 'reference' that has been found by find_reference()
**	and creates enties in the hit structure.
**
** CALLING SEQUENCE:
**
**	bool match_reference()
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

bool match_reference()
{
	sequence* driving_pattern = reference_structure.driving_pattern;
	symbol* t_class_symbol = reference_structure.t_class_symbol,
		* symbol1, * symbol2, * symbol3;
	list_element* el_pos1 = reference_structure.dr_el_pos, * el_pos2,
		* el_pos3;
	int depth_of_bracketting,
		driving_pattern_depth = driving_pattern->get_sequence_depth(),
		target_pattern_depth = 1, dr_status, t_status;

	while (symbol1 = (symbol*)
		driving_pattern->get_next_child_by_el_pos(&el_pos1))
	{
		if (symbol1->get_type() != LEFT_BRACKET) continue;
		el_pos2 = el_pos1;

		// Look for matching CONTEXT_SYMBOL.

		while (symbol2 = (symbol*)
			driving_pattern->get_next_child_by_el_pos(&el_pos2))
		{
			// The CONTEXT_SYMBOL must follow the LEFT_BRACKET without
			// any other symbol types in between.

			if (symbol2->get_type() != CONTEXT_SYMBOL) goto L1;

			// Constraint on the matching of IDENTIFICATION symbols against
			// CONTENTS symbols and vice versa.

			if (id_c_symbol_constraint)
			{
				dr_status = symbol2->get_status();
				t_status = t_class_symbol->get_status();
				if (dr_status == IDENTIFICATION
					|| dr_status == BOUNDARY_MARKER)
				{
					if (t_status != CONTENTS) continue;
				}
				else if (dr_status == CONTENTS)
				{
					if (t_status != IDENTIFICATION
						&& t_status != BOUNDARY_MARKER)
						continue;
				}
			}

			if (symbol2->name_matches(t_class_symbol)) break;
		}

		if (symbol2 == NIL) continue;

		// Make sure that symbol2 does not contain t_class_symbol.

		if (anomalous_hit(symbol2, driving_pattern_depth,
			t_class_symbol, 1))
			return(false);

		// Look for matching RIGHT_BRACKET, taking account of the 
		// (relative) depth of backetting.

		el_pos3 = el_pos2;
		depth_of_bracketting = 0;
		while (symbol3 = (symbol*)
			driving_pattern->get_next_child_by_el_pos(&el_pos3))
		{
			if (symbol3->get_type() == LEFT_BRACKET)
				depth_of_bracketting++;
			if (symbol3->get_type() == RIGHT_BRACKET)
			{
				if (depth_of_bracketting == 0)
					// We have a RIGHT_BRACKET at the right level.
					break;
				depth_of_bracketting--; // Carry on searching.
			}
		}

		// We have a valid match for all three symbols.

		reference_structure.left_dr_bracket = symbol1;
		reference_structure.left_dr_bracket_int_pos =
			el_pos1->get_position();
		reference_structure.dr_class_symbol = symbol2;
		reference_structure.dr_class_symbol_int_pos =
			el_pos2->get_position();
		reference_structure.right_dr_bracket = symbol3;
		reference_structure.right_dr_bracket_int_pos =
			el_pos3->get_position();
		reference_structure.dr_el_pos = el_pos3;
		return(true);

	L1:;
	}
	return(false);
} // match_reference

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Called from compare_patterns_with_brackets(). This function finds
**	the next 'reference' in the target_pattern, meaning a left bracket
**	followed by a class symbol followed by a right bracket. The position
**	that searching starts is recorded in the global structure
**	reference_structure and that value is updated ready for the next
**	search if a reference is found. That structure also records the
**	values of the symbols that have been found.
**
** CALLING SEQUENCE:
**
**	bool find_reference()
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if a 'reference' is found, false otherwise.
**
** IMPLICIT INPUTS:
**
**	reference_structure
**
** IMPLICIT OUTPUTS:
**
**	reference_structure
**
** SIDE EFFECTS:
**
**	NONE
*/

bool find_reference()
{
	sequence* target_pattern = reference_structure.target_pattern;
	list_element* el_pos1 = reference_structure.t_el_pos,
		* el_pos2, * el_pos3;
	symbol* symbol1, * symbol2, * symbol3;

	while (symbol1 = (symbol*)
		target_pattern->get_next_child_by_el_pos(&el_pos1))
	{
		if (symbol1->get_type() != LEFT_BRACKET) continue;
		el_pos2 = el_pos1;
		symbol2 = (symbol*)
			target_pattern->get_next_child_by_el_pos(&el_pos2);
		if (symbol2->get_type() != CONTEXT_SYMBOL) continue;
		el_pos3 = el_pos2;
		symbol3 = (symbol*)
			target_pattern->get_next_child_by_el_pos(&el_pos3);
		if (symbol3->get_type() != RIGHT_BRACKET) continue;

		reference_structure.left_t_bracket = symbol1;
		reference_structure.left_t_bracket_int_pos =
			el_pos1->get_position();
		reference_structure.t_class_symbol = symbol2;
		reference_structure.t_class_symbol_int_pos =
			el_pos2->get_position();
		reference_structure.right_t_bracket = symbol3;
		reference_structure.right_t_bracket_int_pos =
			el_pos3->get_position();
		reference_structure.t_el_pos = el_pos3;
		return(true);
	}

	reference_structure.t_el_pos = NIL;
	return(false);
} // find_reference

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Matches one pattern against another when both patterns contain brackets.
**
** CALLING SEQUENCE:
**
**	void compare_patterns_with_brackets(sequence *driving_pattern,
**		sequence *target_pattern, sequence *cnp)
**
** FORMAL ARGUMENTS:
**
**	Return value:		void
**
**	driving_pattern:	The driving pattern.
**	target_pattern:		The target pattern.
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
*/

void compare_patterns_with_brackets(sequence* driving_pattern,
	sequence* target_pattern, sequence* cnp)
{
	if (target_pattern->is_abstract_pattern() == false)
		return;

	reference_structure.target_pattern = target_pattern;
	reference_structure.driving_pattern = driving_pattern;
	reference_structure.left_t_bracket = NIL;
	reference_structure.t_class_symbol = NIL;
	reference_structure.right_t_bracket = NIL;
	reference_structure.left_dr_bracket = NIL;
	reference_structure.dr_class_symbol = NIL;
	reference_structure.right_dr_bracket = NIL;
	reference_structure.t_el_pos = NIL;
	reference_structure.dr_el_pos = NIL;

	while (find_reference())
	{
		if (match_reference())
		{
			master_driving_int_pos =
				reference_structure.left_dr_bracket_int_pos;
			master_driving_pattern =
				reference_structure.driving_pattern;
			master_driving_symbol =
				reference_structure.left_dr_bracket;
			master_target_int_pos =
				reference_structure.left_t_bracket_int_pos;
			master_target_pattern =
				reference_structure.target_pattern;
			master_target_symbol =
				reference_structure.left_t_bracket;
			hn_master->set_driving_int_pos(master_driving_int_pos);
			hn_master->set_driving_pattern(master_driving_pattern);
			hn_master->set_driving_symbol(master_driving_symbol);
			hn_master->set_target_int_pos(master_target_int_pos);
			hn_master->set_target_pattern(master_target_pattern);
			hn_master->set_target_symbol(master_target_symbol);
			update_hit_structure(cnp);

			master_driving_int_pos =
				reference_structure.dr_class_symbol_int_pos;
			master_driving_symbol =
				reference_structure.dr_class_symbol;
			master_target_int_pos =
				reference_structure.t_class_symbol_int_pos;
			master_target_symbol =
				reference_structure.t_class_symbol;
			hn_master->set_driving_int_pos(master_driving_int_pos);
			hn_master->set_driving_symbol(master_driving_symbol);
			hn_master->set_target_int_pos(master_target_int_pos);
			hn_master->set_target_symbol(master_target_symbol);
			update_hit_structure(cnp);

			master_driving_int_pos =
				reference_structure.right_dr_bracket_int_pos;
			master_driving_symbol =
				reference_structure.right_dr_bracket;
			master_target_int_pos =
				reference_structure.right_t_bracket_int_pos;
			master_target_symbol =
				reference_structure.right_t_bracket;
			hn_master->set_driving_int_pos(master_driving_int_pos);
			hn_master->set_driving_symbol(master_driving_symbol);
			hn_master->set_target_int_pos(master_target_int_pos);
			hn_master->set_target_symbol(master_target_symbol);
			update_hit_structure(cnp);
		}
	}

} // compare_patterns_with_brackets

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Matches a driving pattern with target patterns.
**
** CALLING SEQUENCE:
**
**	void match_driving_pattern(sequence *driving_pattern,
**		sequence *cnp)
**
** FORMAL ARGUMENTS:
**
**	Return value:		void
**
**	driving_pattern:	The driving pattern.
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
*/

void match_driving_pattern(sequence* driving_pattern, sequence* cnp)
{
	list_element* el_pos1;
	sequence* target_pattern;

	fprintf(output_file, "NEXT DRIVING PATTERN (");
	print_pattern_cycle(true, cnp);
	fprintf(output_file, ")\n\n");

	driving_pattern->write_pattern(true, true);

	if (driving_pattern->get_number_of_children() >= MCOLS)
		abort_run("MCOLS is too small.");

	hn_master->set_driving_pattern(driving_pattern);
	master_driving_pattern = driving_pattern;

	// Take target patterns from old_patterns.

	el_pos1 = NIL;
	list_for_el_pos(target_pattern, sequence, old_patterns, el_pos1)
	{
		if (target_pattern->get_number_of_children() >= MCOLS)
			abort_run("MCOLS is too small.");

		hn_master->set_target_pattern(target_pattern);
		master_target_pattern = target_pattern;

		if (cycle == 1)
			compare_patterns(driving_pattern, target_pattern, cnp);
		else compare_patterns_with_brackets(driving_pattern,
			target_pattern, cnp);
	}

	// In this version, it appears that there is no need to compare
	// the driving pattern with any of the parsing_alignments. Therefore,
	// this portion of code has been removed (to odds_and_ends.cpp).

} // match_driving_pattern

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Finds hit sequences between driving patterns and patterns in
**	Old and in alignments already formed.
**
** CALLING SEQUENCE:
**
**	bool find_best_matches(sequence *cnp)
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if one or more hit sequences are
**					formed. false otherwise.
**
**	cnp:			The New pattern being processed.
**
** IMPLICIT INPUTS:
**
**	old_patterns
**
** IMPLICIT OUTPUTS:
**
**	NONE
**
** SIDE EFFECTS:
**
**	NONE
*/

bool find_best_matches(sequence* cnp)
{
	sequence* driving_pattern;
	list_element* el_pos1;

	if (cycle == 1)
	{
		driving_pattern = cnp;
		match_driving_pattern(driving_pattern, cnp);
	}
	else
	{
		el_pos1 = NIL;
		list_for_el_pos(driving_pattern, sequence,
			parsing_alignments, el_pos1)
		{
			if (driving_pattern->is_new_this_cycle() == false) continue;
			// We only want to use alignments that were newly created
			// on the previous cycle.

			if (driving_pattern->get_degree_of_matching() != PARTIAL
				// We need to allow FULL_A alignments to be tried because
				// some of them may be composite.
				&& driving_pattern->new_hits_form_coherent_sequence())
				match_driving_pattern(driving_pattern, cnp);
		}
	}

	if (hit_root->get_first_element() == NIL)
	{
		fprintf(output_file, "FOR ");
		print_pattern_cycle(true, cnp);
		fprintf(output_file,
			", NO HIT SEQUENCES HAVE BEEN FOUND.\n\n");
		return(false);
	}
	else if (show_hit_structure)
		write_hit_structure(hit_root, 0);

	return(true);

} /* find_best_matches */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Writes the best alignment to latex_file in a form ready
**	for printing using latex.
**
** CALLING sequence:
**
**	void output_alignment(sequence *sequence1)
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	sequence1:	The sequence to be output.
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

void output_alignment(sequence* sequence1)
{
	int al_length_in_characters = 0, al_lines, i;
	double target_font_height_in_points,
		actual_font_height_in_points, actual_font_width_in_points,
		actual_font_width_in_mm,
		baselineskip_in_points, base_line_skip_in_mm,
		printing_height, printing_width,
		al_actual_depth_in_mm,
		font_heights[FONT_SET_SIZE] =
	{ 7, 8, 9, 10, 11, 12, 14, 17.28, 20.74, 24.88 };

	// double actual_font_height_in_mm ;

	/* Note re fontsizes: there are 72.27 'points' to an inch which
	is 2.8452756 points to 1 mm (assuming 1 inch = 25.4 mm). This
	is defined in sp.h as PRINTING_POINTS_PER_MM.

	Here are values to go in \fontsize{17.28pt}{20.74pt} in Latex files:

		\fontsize{07.00pt}{08.40pt}
		\fontsize{08.00pt}{09.60pt}
		\fontsize{09.00pt}{10.80pt}
		\fontsize{10.00pt}{12.00pt}
		\fontsize{12.00pt}{14.40pt}
		\fontsize{14.00pt}{16.80pt}
		\fontsize{17.28pt}{20.74pt}
		\fontsize{20.74pt}{24.89pt}
		\fontsize{24.88pt}{29.86pt}

		The second figure is 1.2 times the first figure.

		A 'standard' (12 pt?) character is 2.125 mm wide and 7.6 mm high.

		An A4 sheet of paper has dimensions as follows:

		Width: 210 mm. With margins of 25 mm, this gives a
		printing space of 160 mm (6.25 inch).

		Height: 297 mm. With margins of 25 mm, this gives a
		printing space of 247 mm (9.6 inch).

		For the two orientations, and assuming the letter size above,
		these figures mean:

		Portrait: 75.29 letters / line, 32.5 lines.

		Landscape: 116.24 letters / line, 21 lines.

		Font families: \fontfamily{family}, families include:
		cmr for Computer Modern Roman, cmss for Computer Modern Sans Serif,
		cmtt for Computer Modern Typewriter.

		Standard fontsizes in LaTeX are 7, 8, 9, 10, 11, 12, 14, 17.28, 20.74,
		and 24.88. The baselineskip_in_points (distance from the
		bottom of one line to the bottom of the next) is normally 1.2 times
		the point size. */

	if (orientation == LANDSCAPE)
	{
		printing_height = PRINTING_SPACE_SHORT_SIDE;
		printing_width = PRINTING_SPACE_LONG_SIDE;
	}
	else
	{
		printing_height = PRINTING_SPACE_LONG_SIDE;
		printing_width = PRINTING_SPACE_SHORT_SIDE;
	}

	/* Find the length of the sequence in characters. 1 is
	added to the length of each symbol except the last to represent
	the space between symbols. */

	al_length_in_characters = sequence1->compute_character_length();
	al_lines = (sequence1->get_sequence_depth() * 2) - 1; // The number
	// of lines in the printed alignment is twice the number
	// of rows (to allow for an extra line between each row)
	// less 1 because the extra line is not needed for the
	// last row.

	// Calculate a 'target' font size. This may then be adjusted in
	// the light of available font sizes and maximum_font_height
	// and minimum_font_height.

	target_font_height_in_points = (printing_width /
		al_length_in_characters)
		* PRINTING_POINTS_PER_MM
		* CHARACTER_HEIGHT_WIDTH_RATIO;

	// Find available font size which is >= minimum_font_height
	// and <= maximum_font_height and is nearest to the target_font size.
	// Start by looking for the available font size which is nearest
	// to the target font size.

	double difference, smallest_difference = HIGH_VALUE;
	int index_best;

	for (i = 0; i < FONT_SET_SIZE; i++)
	{
		if (target_font_height_in_points < font_heights[i])
			difference = font_heights[i] -
			target_font_height_in_points;
		else difference = target_font_height_in_points -
			font_heights[i];
		if (difference < smallest_difference)
		{
			smallest_difference = difference;
			index_best = i;
		}
	}

	actual_font_height_in_points = font_heights[index_best];

	if (actual_font_height_in_points < minimum_font_height)
		actual_font_height_in_points = minimum_font_height;

	if (actual_font_height_in_points > maximum_font_height)
		actual_font_height_in_points = maximum_font_height;

	actual_font_width_in_points = actual_font_height_in_points /
		CHARACTER_HEIGHT_WIDTH_RATIO;

	// Calculated the distance between the bottom of one line and
	// the bottom of the next line below.

	baselineskip_in_points = actual_font_height_in_points * 1.2;

	// Convert measurements in points to measurements in mm.

	// actual_font_height_in_mm = actual_font_height_in_points /
	// PRINTING_POINTS_PER_MM ;
	actual_font_width_in_mm = actual_font_width_in_points /
		PRINTING_POINTS_PER_MM;
	base_line_skip_in_mm = baselineskip_in_points / PRINTING_POINTS_PER_MM;

	fprintf(output_file, "%s%1.2f%s",
		"actual_font_height_in_points = ",
		actual_font_height_in_points,
		" pt\n\n");

	// Calculate how many character columns of an alignment can
	// be fitted across the printing_width without, at this stage,
	// allowing for the row number printed at the beginning and end
	// of each row. This adjustment will be made in write_alignment.
	// Most sections will be shorter than the calculated number of
	// columns because alignments will only be split at blank columns.

	int write_section_chars_length_latex = (int)
		ceil(printing_width / actual_font_width_in_mm);

	// Calculate the number of sections of the alignment can
	// be fitted into the printing_height.

	al_actual_depth_in_mm = base_line_skip_in_mm * (double)al_lines + 2.0;
	// al_lines is increased by 2 to allow for the 2 blank
	// lines between one section and the next or between
	// the last section and the "Figure xxx" label. No attempt
	// has been made to allow for this bottom line because
	// there are likely to be few occasions when there is
	// not a spare line to accommodate it.

	int sections_per_page = (int)floor(printing_height /
		al_actual_depth_in_mm);

	fprintf(latex_file, "%s%s%s%s%s%s",
		"\\documentclass{article}\n",
		"\\textheight 254mm\n",
		"\\textwidth 177.8mm\n",
		"\\topmargin -20mm\n",
		"\\oddsidemargin 0mm\n",
		"\\evensidemargin 0mm\n");


	if (orientation == LANDSCAPE)
		fprintf(latex_file, "\\usepackage{lscape}\n");

	fprintf(latex_file, "\\begin{document}\n");

	if (orientation == LANDSCAPE)
		fprintf(latex_file, "\\begin{landscape}\n");

	fprintf(latex_file, "%s%1.2f%s%1.2f%s",
		"\\fontsize{",
		actual_font_height_in_points,
		"pt}{",
		baselineskip_in_points,
		"pt}\n");

	fprintf(latex_file, "%s%s",
		"\\pagestyle{empty}\n",
		"\\begin{verbatim}\n");

	sequence1->write_alignment_horizontal(latex_file,
		write_section_chars_length_latex,
		sections_per_page);

	fprintf(latex_file, "%s%s%s",
		"\nFigure ",
		figure_ID,
		"\n");

	fprintf(latex_file, "\\end{verbatim}\n");

	if (orientation == LANDSCAPE)
		fprintf(latex_file, "\\end{landscape}\n");

	fprintf(latex_file, "\\end{document}\n");

	fflush(latex_file);
} /* output_alignment */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Compares two double variables.
**
** CALLING SEQUENCE:
**
**	bool is_better_than(double A, double B)
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if A is better than B, false otherwise
**
**	A, B:			The two variables to be compared.
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

bool is_better_than(double A, double B)
{
	if (A < B) return(true);
	else return(false);
} // is_better_than

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Calculates the logarithm to the base 2 according to the formula
**	given in Knuth's "The Art of Computer Programming", vol 2,
**	page 23.
**
** CALLING SEQUENCE:
**
**	double log_2(double x)
**
** FORMAL ARGUMENTS:
**
**	Return value:	The required log to the base 2 of x.
**
**	x:				The number whose logarithm is to be calculated.
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

double log_2(double x)
{
	return(log10(x) / log_10_2);
} // log_2

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Looks up a symbol name in key_array[] to find the corresponding
**	abbreviation. If no abbreviation can be found, the original
**	string is returned. Any symbol beginning with '#' is
**	returned directly.
**
** CALLING SEQUENCE:
**
**	char *find_symbol_abbreviation(char *symbol_name_1)
**
** FORMAL ARGUMENTS:
**
**	Return value:	A character string for the abbreviation.
**
**	symbol_name_1:	The name whose abbreviation is to be found.
**
** IMPLICIT INPUTS:
**
**	key_array[]
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

char* find_symbol_abbreviation(char* symbol_name_1)
{
	int key_index;

	for (key_index = 0; key_index < fe_key_array; key_index++)
	{
		if (strcmp(symbol_name_1,
			key_array[key_index].complete_symbol) == 0)
			return(key_array[key_index].abbreviated_symbol);
	}

	// If this point is reached, then no match was found for symbol_name_2.
	// In this case, the function returns symbol_name_1.

	return(symbol_name_1);
} // find_symbol_abbreviation

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	In write_alignment, fills in the non-hit symbols before a hit
**	symbol.
**
** CALLING SEQUENCE:
**
**	bool fill_in_non_hit_symbols(int s_row1, symbol *hit_symbol,
**		int *fe_col)
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if the write_al_chars_length has been breached.
**					false otherwise.
**
**	s_row1: 	The relevant row in row_array[].
**	hit_symbol: The hit symbol which terminates the sequence of
**				symbols. If hit_symbol == NIL then all the trailing
**				symbols for each row are added to the alignment.
**	fe_col: 	The first empty column in the given row of
**				write_al_chars[][] when the last non-hit symbol
**				before the hit_symbol has been filled in.
**
** IMPLICIT INPUTS:
**
**	arrays for write_alignment.
**
** IMPLICIT OUTPUTS:
**
**	arrays for write_alignment.
**
** SIDE EFFECTS:
**
**	NONE
**
*/

bool fill_in_non_hit_symbols(int s_row1, symbol* hit_symbol, int* fe_col)
{
	symbol* symbol1;
	bool char_column_not_OK;
	sequence* pattern1 = row_array[s_row1].row_pattern;
	int end_col, c_col1, c_col2, slot_width, c_row1 = s_row1 * 2;
	char* c, * symbol_name;

	pattern1->set_current_el_pos(row_array[s_row1].patt_last_el_pos);
	c_col1 = row_array[s_row1].fe_col_in_write_al_chars;
	while (symbol1 = (symbol*)pattern1->get_next_child())
	{
		if (symbol1 == hit_symbol) break;

		if (use_abbreviations == LETTERS || use_abbreviations == DIGITS)
			symbol_name = find_symbol_abbreviation(symbol1->
				get_name());
		else symbol_name = symbol1->get_name();

		// Find the next available space in this row of
		// write_al_chars[][] to take the symbol name plus
		// space for one space character.

		slot_width = (int)strlen(symbol_name) + 1; // slot_width
		// is the enough space for the symbol name
		// plus 1 character for the space following the symbol.

		char_column_not_OK = true;
		while (char_column_not_OK)
		{
			char_column_not_OK = false;
			end_col = c_col1 + slot_width;
			for (c_col2 = c_col1; c_col2 < end_col; c_col2++)
			{
				if (write_al_vacant_slots[s_row1][c_col2] ==
					'x')
				{
					char_column_not_OK = true;
					c_col1 = c_col2 + 1;
					break;
				}
			}
		}

		// Check that we are still within the bounds
		// of write_al_chars[][].

		if (c_col1 + slot_width >= write_al_chars_length)
		{
			// The writing out of this row of the alignment
			// will be truncated at this point, with "..." to
			// show that there is more to be written.

			*fe_col = c_col1;
			return(true);
		}
		else
		{
			// Fill in the symbol name.

			for (c = symbol_name; *c != '\0'; c++)
				write_al_chars[c_row1][c_col1++] = *c;
		}

		// Add 1 to c_col to allow for the space character following
		// the symbol.

		c_col1++;
	}

	// Test to see whether the end of the pattern has been reached.

	if (symbol1 == NIL) row_array[s_row1].pattern_is_finished = true;

	// Update the value of patt_last_el_pos for this row. At this
	// point, there is no need to update the value of
	// fe_col_in_write_al_chars because this will be updated
	// in write_alignment after the hit symbol has been written.

	row_array[s_row1].patt_last_el_pos = pattern1->get_current_el_pos();

	*fe_col = c_col1;
	return(false);

} // fill_in_non_hit_symbols

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Partitions sort_array[]. (Used by sort_leaf_array()).
**
** CALLING SEQUENCE:
**
**	void partition(int low, int high, int *mid)
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	low:		The 'low' position in sort_array[].
**	high:		The 'high' position in sort_array[].
**	mid:		A pointer to the 'mid' position in sort_array[]
**				(in the calling function).
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

void partition(int low, int high, int* mid)
{
	int left_ptr, right_ptr, temp_num;
	double pivot_score;
	hit_node* h_node;

	left_ptr = low;
	right_ptr = high;
	while (left_ptr < right_ptr)
	{
		/* Search for the rightmost element greater than
		h_node->get_compression_difference() for the 'low' position
		in leaf_array[] (which is the 'pivot'), starting from
		the right. */

		h_node = leaf_array[sort_array[low]];
		pivot_score = h_node->get_compression_difference();
		h_node = leaf_array[sort_array[right_ptr]];
		while (h_node->get_compression_difference() < pivot_score)
			h_node = leaf_array[sort_array[--right_ptr]];

		/* Seach for the leftmost element greater than the
		pivot score, starting from the left. */

		h_node = leaf_array[sort_array[left_ptr]];
		while (left_ptr < right_ptr
			&& (h_node->get_compression_difference() >=
				pivot_score))
			h_node = leaf_array[sort_array[++left_ptr]];

		/* If the pointers have not crossed, switch the values. */

		if (left_ptr < right_ptr)
		{
			temp_num = sort_array[left_ptr];
			sort_array[left_ptr] = sort_array[right_ptr];
			sort_array[right_ptr] = temp_num;
		}
	}
	*mid = right_ptr;
	temp_num = sort_array[*mid];
	sort_array[*mid] = sort_array[low];
	sort_array[low] = temp_num;
} /* partition */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Sorts rows of leaf_array[] by compression_ratio in leaf_node.
**	This version uses the Quicksort method.
**
** CALLING SEQUENCE:
**
**	void sort_leaf_array(int low, int high) ;
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	low:			The 'low' position in sort_array[].
**	high:			The 'high' position in sort_array[].
**
** IMPLICIT INPUTS:
**
**	sort_array[] and leaf_array[]
**
** IMPLICIT OUTPUTS:
**
**	sort_array[] and leaf_array[]
**
** SIDE EFFECTS:
**
**	NONE
*/

void sort_leaf_array(int low, int high)
{
	int mid;

	if (low < high)
	{
		partition(low, high, &mid); /* Partition sort_array[] */
		sort_leaf_array(low, mid - 1); /* Sort the lower portion */
		sort_leaf_array(mid + 1, high); /* Sort the upper portion */
	}
} /* sort_leaf_array */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Makes new alignments.
**
** CALLING SEQUENCE:
**
**	bool make_alignments(sequence *cnp)
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if one or more new sequences
**					are formed, false otherwise.
**
**	cnp:			The New pattern being processed.
**
** IMPLICIT INPUTS:
**
**	Global data structures
**
** IMPLICIT OUTPUTS:
**
**	New unifications
**
** SIDE EFFECTS:
**
**	NONE
*/

bool make_alignments(sequence* cnp)
{
	sequence* al1, * al2;
	hit_node* node1;
	int hit_node_row, leaf_index;
	bool new_alignment_formed = false;

	fprintf(output_file, "STARTING make_alignments (");
	print_pattern_cycle(true, cnp);
	fprintf(output_file, ")\n\n");
	fflush(output_file);

	number_of_parsing_alignments = 0;

	sort_leaf_array(0, fe_sort - 1); // Sort using approximate,
		// cheap-to-compute scores.

	hit_node_row = 0;
	while (node1 = get_leaf_nodes_in_order(&hit_node_row, &al1,
		&leaf_index))
	{
		if (node1->get_compression_difference() <= fail_score)
			continue;

		/* Make a new sequence and, if it is not NIL and
		does not match a sequence formed on this cycle and all
		earlier cycles, add it to alignments_array[]. */

		// Scores are re-computed for each new alignment
		// that is formed.

		al2 = unify_best(leaf_index, cnp);

		if (al2 == NIL) continue;

		if (show_unselected_alignments)
		{
			fprintf(output_file, "NEW ALIGNMENT (");
			print_pattern_cycle(true, cnp);
			fprintf(output_file, ")\n\n");
			al2->print_ID();
			fprintf(output_file, " : ");
			(node1->get_driving_pattern())->print_ID();
			fprintf(output_file, " : ");
			(node1->get_target_pattern())->print_ID();
			fprintf(output_file, " : ");
			node1->print_ID();
			fprintf(output_file,
				"%s%1.2f%s%1.2f%s%1.2f%s%1.2f%s%1.2f%s",
				" NSC = ", al2->get_new_symbols_cost(),
				", EC = ", al2->get_encoding_cost(),
				", CR = ", al2->get_compression_ratio(),
				", CD = ", al2->get_compression_difference(),
				",\nAbsolute P = ", al2->get_abs_P(),
				"\n\n");

			al2->write_alignment(output_file,
				write_section_chars_length, NULL_VALUE,
				alignment_format);

			fflush(output_file);
		}

		new_alignment_formed = true;

		number_of_parsing_alignments++;

		if (number_of_parsing_alignments > max_alignments_in_one_cycle)
		{
			fprintf(output_file, "%s%s%d%s",
				"Formation of alignments cut short by ",
				"MAX_ALIGNMENTS_IN_ONE_CYCLE (",
				max_alignments_in_one_cycle,
				")\n\n");
			break;
		}
	}

	// Resort using re-computed scores.

	sort_leaf_array(0, fe_sort - 1);

	fprintf(output_file, "FINISHED make_alignments (");
	print_pattern_cycle(true, cnp);
	fprintf(output_file, ")\n\n");
	fflush(output_file);

	return(new_alignment_formed);

} /* make_alignments */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**
**
** CALLING SEQUENCE:
**
**	bool max_unsupported_cycles_reached(sequence *cnp)
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if, in this pattern from New, there have been
**					more than max_unsupported_cycles since the
**					best CD value for the pattern was reached.
**					false otherwise.
**
**	cnp:			The current New pattern.
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

bool max_unsupported_cycles_reached(sequence* cnp)
{
	if (cycle - best_cycle_this_new_pattern >= max_unsupported_cycles)
	{
		// We have reached the set limit on the number of
		// cycles which may be performed without any gain
		// in compression score.

		fprintf(output_file, "FOR ");
		print_pattern_cycle(true, cnp);
		fprintf(output_file, "%s%d%s",
			", THE MAXIMUM NUMBER OF UNSUPPORTED CYCLES (",
			max_unsupported_cycles,
			") HAS BEEN REACHED\n\n");

		return(true);
	}
	else return(false);
} // max_unsupported_cycles_reached

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Compresses a sequence from New by repeated applications of
**	find_best_matches.
**
** CALLING SEQUENCE:
**
**	bool recognise(sequence *pattern1)
**
** FORMAL ARGUMENTS:
**
**	Return value:			true if unifiable matchings have been found,
**							false otherwise.
**
**	pattern1:				A new pattern or a derived pattern to be
**							recognised.
**
** IMPLICIT INPUTS:
**
**	old_patterns
**
** IMPLICIT OUTPUTS:
**
**	old_patterns
**
** SIDE EFFECTS:
**
**	NONE
*/

bool recognise(sequence* pattern1)
{
	cycle++;
	bool matches_have_been_found;

	fprintf(output_file, "START OF ");
	print_pattern_cycle(true, pattern1);
	fprintf(output_file, ":\n\n");

	set_up_hit_structure();

	matches_have_been_found = find_best_matches(pattern1);

	// Now that the values of 'new_this_cycle' in parsing_alignments
	// from the previous cycle have served their purpose in controlling
	// which alignments are used as 'driving' alignments in this cycle,
	// they may all be set to false.

	sequence* pattern2;
	list_for(pattern2, sequence, parsing_alignments)
		pattern2->set_new_this_cycle(false);

	if (phase == 1 && cycle == 1)
	{
		receptacle_pattern->add_ID_symbols(NIL, true); // The receptacle
			// pattern is a 'top level' pattern so it needs a
			// unique ID symbol.
		fprintf(output_file, "Receptacle pattern: ");
		receptacle_pattern->write_with_details(true);
	}

	if (matches_have_been_found == false)
	{
		delete hit_root;
		return(false);
	}

	if (show_unselected_alignments)
	{
		fprintf(output_file,
			"SET OF ALIGNMENTS FORMED, BEFORE SELECTION, IN ");
		print_pattern_cycle(true, pattern1);
		fprintf(output_file, "\n\n");
		fflush(output_file);
	}

	if (make_alignments(pattern1) == false)
	{
		fprintf(output_file, "FOR ");
		print_pattern_cycle(true, pattern1);
		fprintf(output_file,
			"\nNO NEW ALIGNMENTS HAVE BEEN FOUND.\n\n");

		fprintf(output_file, "END OF ");
		print_pattern_cycle(true, pattern1);
		fprintf(output_file, "\n\n");

		delete hit_root; // This deletes the whole tree of hit
			// nodes, recursively.

		return(false);
	}

	make_selection(pattern1);

	// Determine which of FULL_A, FULL_B or PARTIAL alignments
	// have been found.

	sequence* al1;
	bool partial_al_found = false, full_A_al_found = false,
		full_B_al_found = false;
	int type_of_alignment;

	list_for(al1, sequence, parsing_alignments)
	{
		type_of_alignment = al1->get_degree_of_matching();
		if (type_of_alignment == FULL_A) full_A_al_found = true;
		if (type_of_alignment == FULL_B) full_B_al_found = true;
		if (type_of_alignment == PARTIAL) partial_al_found = true;
	}

	parsing_alignments->sort_by_compression_difference();

	if (full_B_al_found)
		combine_alignments(pattern1);

	parsing_alignments->sort_by_ID();

	// Print ID numbers of alignments.

	fprintf(output_file, "%s%s%s",
		"In the following lists, alignments that are new on this cycle\n",
		"are marked with an asterisk ('*').\n\n",
		"SELECTED ALIGNMENTS FROM ");
	print_pattern_cycle(true, pattern1);
	fprintf(output_file, "\n\n");

	parsing_alignments->write_IDs(NULL_VALUE, pattern1);

	fflush(output_file);

	if (full_A_al_found)
	{
		fprintf(output_file, "SELECTED FULL ALIGNMENTS (A) FROM ");
		print_pattern_cycle(true, pattern1);
		fprintf(output_file, "\n\n");

		parsing_alignments->write_IDs(FULL_A, pattern1);
	}
	else
	{
		fprintf(output_file, "NO FULL_A ALIGNMENTS FROM ");
		print_pattern_cycle(true, pattern1);
		fprintf(output_file, "\n\n");
	}

	if (full_B_al_found)
	{
		fprintf(output_file, "SELECTED FULL ALIGNMENTS (B) FROM ");
		print_pattern_cycle(true, pattern1);
		fprintf(output_file, "\n\n");

		parsing_alignments->write_IDs(FULL_B, pattern1);
	}
	else
	{
		fprintf(output_file, "NO FULL_B ALIGNMENTS FROM ");
		print_pattern_cycle(true, pattern1);
		fprintf(output_file, "\n\n");
	}

	if (partial_al_found)
	{
		fprintf(output_file, "SELECTED PARTIAL ALIGNMENTS FROM ");
		print_pattern_cycle(true, pattern1);
		fprintf(output_file, "\n\n");
		parsing_alignments->write_IDs(PARTIAL, pattern1);
	}
	else
	{
		fprintf(output_file, "NO PARTIAL ALIGNMENTS FROM ");
		print_pattern_cycle(true, pattern1);
		fprintf(output_file, "\n\n");
	}

	delete hit_root; // This deletes the whole tree of hit
		// nodes, recursively.

	fprintf(output_file, "END OF ");
	print_pattern_cycle(true, pattern1);
	fprintf(output_file, "\n\n");

	fflush(output_file);

	if (max_unsupported_cycles_reached(pattern1)
		|| (full_A_al_found == false && full_B_al_found == false))
		return(false);

	return(true);
} /* recognise */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Called from find_containing_sequences.
**
** CALLING SEQUENCE:
**
**	bool contained_in(sequence *sequence1, sequence *test_sequence)
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if sequence1 is contained
**					in test_sequence,
**					false otherwise.
**
**	sequence1:		The given sequence.
**	test_sequence:	The sequence being tested to see whether it
**					contains the given sequence or not.
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

#if CONTAINED_IN

bool contained_in(sequence* sequence1, sequence* test_sequence)
{
	symbol* B_symbol1, * al_col2;
	sequence* pattern_of_B_symbol1, * pattern_of_al_col2;
	symbol* first_symbol;
	int col1, col2, number_of_children, test_number_of_children,
		sequence_depth = 1, test_sequence_depth = 1,
		ra_index, fe_rows_array = 0, i, offset;
	struct rows_entry
	{
		symbol* B_symbol1, * al_col2;
		sequence* pattern_of_B_symbol1, * pattern_of_al_col2;
		bool match_found;
	} rows_array[MEDIUM_SCRATCH_ARRAY_SIZE];
	bool match_found, original_pattern_match_found;

	if (sequence1 == NIL || test_sequence == NIL)
		abort_run("NIL parameter(s) for contained_in");
	if (typeof(sequence1) != typeof(test_sequence)) return(false);
	if (typeof(sequence) == SYMBOL || typeof(test_sequence) == SYMBOL)
		abort_run("SYMBOL parameter(s) for contained_in");

	/* Compare the lengths of the sequences. */

	number_of_children = sequence1->number_of_children;
	test_number_of_children = test_sequence->number_of_children;

	if (number_of_children > test_number_of_children) return(false);

	/* Find sequence depth. */

	sequence1->initialise();
	while (B_symbol1 = (symbol*)sequence1->get_next_child())
		sequence_depth++;

	/* Find test_sequence_depth and compare with sequence_depth. */

	test_sequence->initialise();
	while (B_symbol1 = (symbol*)test_sequence->get_next_child())
		test_sequence_depth++;

	if (sequence_depth > test_sequence_depth) return(false);

	/* Initialise rows_array[]. */

	for (ra_index = 0; ra_index < MEDIUM_SCRATCH_ARRAY_SIZE; ra_index++)
	{
		rows_array[ra_index].B_symbol1 = NIL;
		rows_array[ra_index].pattern_of_B_symbol1 = NIL;
		rows_array[ra_index].al_col2 = NIL;
		rows_array[ra_index].pattern_of_al_col2 = NIL;
		rows_array[ra_index].match_found = false;
	}

	/* Now fill in values for rows_array[]. The original pattern
	of each Old row in sequence1 is compared with the original_pattern
	each Old row in test sequence1. If there are no matches,
	this means that sequence1 is not contained in
	test_sequence. Where there are one or more matches,
	these are entered in rows_array[]. */

	sequence1->initialise();
	B_symbol1 = (symbol*)sequence1->get_next_child();
	while (B_symbol1 = (symbol*)sequence1->get_next_child())
	{
		original_pattern_match_found = false;
		test_sequence->initialise();
		B_symbol1 = (symbol*)test_sequence->get_next_child();
		while (B_symbol1 = (symbol*)
			test_sequence->get_next_child())
		{
			if (B_symbol1->original_pattern != al_col2->el_obj)
				continue;

			original_pattern_match_found = true;

			/* We have a positive match of the original_patterns.
			Now record it in rows_array[]. */

			rows_array[fe_rows_array].B_symbol1 = B_symbol1;
			rows_array[fe_rows_array].pattern_of_B_symbol1 =
				B_symbol1->el_obj;
			rows_array[fe_rows_array].al_col2 = al_col2;
			rows_array[fe_rows_array].pattern_of_al_col2 =
				al_col2->el_obj;
			fe_rows_array = plus_one(fe_rows_array,
				MEDIUM_SCRATCH_ARRAY_SIZE,
				"rows_array[] too small in contained_in");
		}
		if (original_pattern_match_found == false) return(false);
	}

	/* Now check to see whether the pattern of hits for each Old row
	of sequence1 matches a pattern of hits (for the same original_pattern)
	in an Old row of test_sequence. The order of matching rows need not
	be the same in the two sequences. It is not safe to assume that the
	two sequences have the same length: if test_sequence is longer
	than sequence1, it is not safe to assume that the pattern of hits
	in any one row in sequence1 starts at the same symbol as
	test_sequence. Likewise for the end of a pattern of hits.
	If the symbols do not coincide, then, for each row, symbols
	in test_sequence which are outside the pattern of hits must all
	be NIL. */

	/* Find an Old row in sequence1 where the first symbol in the
	row is not NIL. */

	sequence1->initialise();
	B_symbol1 = (symbol*)sequence1->get_next_child();
	while (B_symbol1 = (symbol*)sequence1->get_next_child())
	{
		if (B_symbol1->symbol_sequence[0] != NIL)
		{
			pattern_of_B_symbol1 = B_symbol1->el_obj;
			first_symbol = B_symbol1->symbol_sequence[0];
			break;
		}
	}

	/* Now find row(s) in test_sequence with the same original_pattern and
	find the position in each of these rows of the first symbol of
	the original_pattern. From the one which starts furthest to the left,
	we can find the offset of the start of sequence1
	relative to the start of test_sequence. */

	pattern_of_al_col2 = NIL;
	offset = 1000000;
	test_sequence->initialise();
	B_symbol1 = (symbol*)test_sequence->get_next_child();
	while (B_symbol1 = (symbol*)test_sequence->get_next_child())
	{
		if (al_col2->el_obj == pattern_of_B_symbol1)
		{
			for (col2 = 0; col2 < test_number_of_children; col2++)
			{
				if (al_col2->symbol_sequence[col2] !=
					first_symbol) continue;
				if (col2 < offset) offset = col2;
				break;
			}
			if (col2 >= test_number_of_children)
				abort_run("Error in contained_in");
		}
	}

	if (offset >= test_number_of_children)
		abort_run("Error in contained_in");

	ra_index = 0;
	sequence1->initialise();
	B_symbol1 = (symbol*)sequence1->get_next_child();
	while (B_symbol1 = (symbol*)sequence1->get_next_child())
	{
		match_found = false;
		while (rows_array[ra_index].B_symbol1 == B_symbol1)
		{
			al_col2 = rows_array[ra_index].al_col2;

			/* Check that this al_col2 has
			not already been 'used'. */

			for (i = 0; i < ra_index; i++)
				if (rows_array[i].al_col2 == al_col2
					&& rows_array[i].match_found)
					goto L1;

			/* Step over the offset in al_col2, checking that
			it contains nothing other than NIL values. */

			for (col2 = 0; col2 < offset; col2++)
				if (al_col2->symbol_sequence[col2] != NIL)
					goto L1;

			/* Now compare the pattern of hits in B_symbol1 and
			al_col2. */

			for (col1 = 0; col1 < number_of_children; col1++)
			{
				if (col2 >= test_number_of_children) goto L1;
				if (B_symbol1->symbol_sequence[col1] !=
					al_col2->symbol_sequence[col2])
					goto L1;
				col2++;
			}

			/* Check that any trailing symbols in al_col2 are
			all NIL. */

			for (col2 = col2; col2 < test_number_of_children; col2++)
				if (al_col2->symbol_sequence[col2] != NIL)
					goto L1;

			match_found = true;
			rows_array[ra_index].match_found = true;
			while (rows_array[ra_index].B_symbol1 == B_symbol1)
				if (++ra_index >= fe_rows_array) break;

			break;

		L1:;
			if (++ra_index >= fe_rows_array) break;
		}
		if (!match_found) return(false);
	}
	return(true);
} /* contained_in */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	For each of the sequences in best_sequences[], this function
**	compiles a list of the other best sequences, if any, which
**	contain the given sequence.
**
** CALLING SEQUENCE:
**
**	void find_containing_sequences()
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
** IMPLICIT INPUTS:
**
**	best_sequences[]
**
** IMPLICIT OUTPUTS:
**
**	For each best_sequence[], a list of containing sequences which
**	may be NIL.
**
** SIDE EFFECTS:
**
**	NONE
**
*/

void find_containing_sequences()
{
	int i, j;

	for (i = 0; i < fe_best_sequences; i++)
	{
		for (j = 0; j < fe_best_sequences; j++)
		{
			if (i == j) continue;
			if (contained_in(best_sequences[i].al_el,
				best_sequences[j].al_el))
			{
				fprintf(output_file, "Alignment ");
				best_sequences[i].al_el->print_ID();
				fprintf(output_file, " is contained in ");
				best_sequences[j].al_el->print_ID();
				fprintf(output_file, "\n\n");
			}
		}
	}
} /* find_containing_sequences */

#endif

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Fetches symbols by row order and symbol order from an sequence,
**	excluding any symbols in the first row (which represents New)
**	which are not aligned with any symbol in Old. Each symbol type
**	is returned only once, ie second and subsequent instances of a symbol
**	type are skipped.
**
** CALLING SEQUENCE:
**
**	symbol *get_next_symbol_from_sequence(sequence *al1, int status)
**
** FORMAL ARGUMENTS:
**
**	Return value:	The next symbol type in the sequence.
**
**	al1:			The sequence from which symbol types are fetched.
**	status: 		If START, the sequence is searched from the
**					beginning, otherwise it continues from where it
**					was previously.
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

symbol* get_next_symbol_from_sequence(sequence* al1, int status)
{
	static symbol* B_symbol1;
	static alignment_element* al_el1;
	static symbol* symbol_array[MEDIUM_SCRATCH_ARRAY_SIZE];
	symbol* symbol1;
	int i, row1, sequence_depth = al1->get_sequence_depth();

	if (al1 == NIL) abort_run("Invalid value for al1 in \
		get_next_symbol_from_sequence");

	if (status == START)
	{
		// Initialise symbol_array[] (which is used for checking for 
		// duplicates in symbols returned).

		for (i = 0; i < MEDIUM_SCRATCH_ARRAY_SIZE; i++)
			symbol_array[i] = NIL;
		return(NIL);
	}

	al1->initialise();
	while (B_symbol1 = (symbol*)al1->get_next_child())
	{
		for (row1 = 0; row1 < sequence_depth; row1++)
		{
			al_el1 = B_symbol1->get_al_el(row1);
			symbol1 = (symbol*)al_el1->get_el_obj();
			if (symbol1 == NIL) continue;

			// Check whether al_el1 is at the top of a symbol.
			// If it is below the top of a symbol,
			// skip to next symbol.

			if (al_el1->get_same_column_above() != NULL_VALUE)
				break;

			// Check whether al_el1 is in the first row
			// (corresponding to New) and is also not part part
			// of any hit. If so, skip.

			if (row1 == 0 && al_el1->get_same_column_below() ==
				NULL_VALUE)
				break;

			// Now check whether a matching symbol has been
			// retrieved from this sequence before. If it has,
			// skip to next symbol. Otherwise add symbol1 to 
			// symbol_array[] and return it as the next symbol.

			for (i = 0; i < MEDIUM_SCRATCH_ARRAY_SIZE; i++)
			{
				if (symbol_array[i] == NIL)
				{
					symbol_array[i] = symbol1;
					return(symbol1);
				}

				if (strcmp(symbol1->get_name(),
					symbol_array[i]->get_name()) == 0)

					// There is no need to
					// continue down this symbol
					// because only symbols of
					// the same type will be found.

					goto L1;
			}

			// If the program reaches this point, it means that
			// symbol_array[] is too small.

			abort_run("symbol_array[] is too small in \
				get_next_symbol_from_sequence");

		}
	L1:;
	}

	return(NIL);
} // get_next_symbol_from_sequence

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Checks whether an alignment contains one or more redundant rows.
**
**	A row is redundant if it is in Old and if every symbol in it
**	forms a hit with a symbol in one other row in Old (see sp61_od, %49).
**	No attempt is made to check for rows which may be redundant
**	because they form hits across two or more rows in Old.
**
**	If one or more rows are redundant, the alignment is edited to remove
**	the redundant rows and the function returns true. Otherwise it
**	returns false.
**
** CALLING SEQUENCE:
**
**	bool check_for_redundant_rows(sequence *al1)
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if al1 has been edited. false otherwise.
**
**	al1:			The alignment to be checked.
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

bool check_for_redundant_rows(sequence* al1)
{
	// Check each row in the alignment against every other row.
	// A row is redundant if:
	// * Every symbol in the row is a hit symbol.
	// * There is at least one other row which forms a hit with
	//	every symbol of the given row.
	//
	// The second condition means that the other row must be at least
	// as long as the given row. If they are equal in size, then one
	// of them must be chosen arbitrarily as the redundant row.

	symbol* al_col1, * al_col2,
		* new_alignment_array[LARGE_SCRATCH_ARRAY_SIZE] = { 0 };
	alignment_element* al_el1 = { NIL };
	int row1, row2, al1_depth = al1->get_sequence_depth(),
		al1_length = al1->get_number_of_children(),
		this_row_pattern_length, that_row_pattern_length,
		this_row_int_pos, temp_row_int_pos;
	bool redundant_rows_array[MEDIUM_SCRATCH_ARRAY_SIZE], // Used to
		// record whether a row is redundant and is to
		// be deleted (true) or is to be preserved (false).
		that_row_matches_this_row[MEDIUM_SCRATCH_ARRAY_SIZE]; // Used
	// to record when 'that' row matches 'this' row.
	sequence* this_row_pattern, * that_row_pattern;
	list_element* this_el_pos_start, * pos1;

	if (al1_depth >= MEDIUM_SCRATCH_ARRAY_SIZE)
		abort_run("redundant_rows_array[] is too small in \
	check_for_redundant_rows");
	if (al1_length >= LARGE_SCRATCH_ARRAY_SIZE)
		abort_run("new_alignment_array[] is too short in \
		check_for_redundant_row");

	redundant_rows_array[0] = false; // Make sure that the first
		// row (New) is preserved in all circumstances.


	for (row1 = 1; row1 < al1_depth; row1++)
	{
		redundant_rows_array[row1] = false; // By default, each row
		// is to be preserved, not edited out.
		this_row_pattern = al1->get_row_pattern(row1);
		this_row_pattern_length =
			this_row_pattern->get_number_of_children();

		// Search for the first non-NIL entry in row1.

		this_el_pos_start = NIL;
		while (al_col1 = (symbol*)
			al1->get_next_child_by_el_pos(&this_el_pos_start))
		{
			al_el1 = al_col1->get_al_el(row1);
			if (al_el1->get_el_obj() != NIL) break;
		}

		// Check whether the first hit symbol for this row is
		// the first symbol in the pattern for this row. If it
		// is not, then this row cannot be contained in any other
		// row.

		this_row_int_pos = al_el1->get_orig_patt_int_pos();
		if (this_row_int_pos != 0) continue;

		// Now step along 'this' row, comparing it with every other
		// row except row 0. If there are any unmatched symbols in
		// 'this' row, break out of the loop because 'this' row
		// cannot be completely matched to any other row.
		// Whenever a mis-match appears between 'that'
		// row and 'this' row, a false value is entered into
		// that_row_matches_this_row[].

		// Initialise that_row_matches_this_row[].
		// Check the lengths of the patterns in each
		// row against the length of the pattern for 'this' row.
		// If a row contains a pattern that is shorter than the 
		// pattern in 'this' row, then 'this' row cannot be 
		// completely matched against it.

		for (row2 = 1; row2 < al1_depth; row2++)
		{
			if (row2 == row1)
			{
				that_row_matches_this_row[row2] = false;
				continue;
			}

			// Compare lengths of patterns.

			that_row_pattern = al1->get_row_pattern(row2);
			that_row_pattern_length =
				that_row_pattern->get_number_of_children();
			if (that_row_pattern_length < this_row_pattern_length)
			{
				that_row_matches_this_row[row2] = false;
				continue;
			}

			// Check whether there is a symbol in al_col1 in
			// this row.

			al_el1 = al_col1->get_al_el(row2);
			if (al_el1->get_el_obj() == NIL)
				that_row_matches_this_row[row2] = false;
			else that_row_matches_this_row[row2] = true;
		}

		// At this point, we know that this_row_int_pos == 0.

		pos1 = this_el_pos_start;
		while (al_col1 = (symbol*)
			al1->get_next_child_by_el_pos(&pos1))
		{
			al_el1 = al_col1->get_al_el(row1);

			// Skip over any column which does not contain
			// a symbol for row1.

			if (al_el1->get_el_obj() == NIL) continue;

			// Check whether there are any unmatched symbols
			// in 'this' row between the current column and
			// the last one. If there is, 'this' row cannot
			// be completely contained in any other.

			temp_row_int_pos = al_el1->get_orig_patt_int_pos();
			if (temp_row_int_pos - this_row_int_pos > 1) break;

			// 'this' row contains a symbol and there are no
			//	unmatched symbols preceding the current symbol
			// in this row.

			this_row_int_pos = temp_row_int_pos;

			// Now examine all the other rows for this column.
			// If there is no symbol or an unmatched symbol
			// in any row then it cannot be a matching row
			// for 'this' row. Also, there is no point checking
			// rows which are already known to be false.

			for (row2 = 1; row2 < al1_depth; row2++)
			{
				if (that_row_matches_this_row[row2] == false)
					continue;

				if (row2 == row1)
				{
					that_row_matches_this_row[row2] =
						false;
					continue;
				}

				// Check whether there is a symbol in
				// al_col1 in this row.

				al_el1 = al_col1->get_al_el(row2);
				if (al_el1->get_el_obj() == NIL)
					that_row_matches_this_row[row2] =
					false;
				else that_row_matches_this_row[row2] = true;
			}
		}

		// Now examine that_row_matches_this_row[] to see if
		// there are any rows that are true. If there are, this
		// means that row1 is redundant and this can be entered
		// into redundant_rows_array[].

		// If Old contains duplicate
		// patterns (which is possible but unlikely), then there
		// may be two rows which match each other completely and
		// are the same length. It is assumed here that Old does
		// not contain duplicate patterns.

		for (row2 = 1; row2 < al1_depth; row2++)
			if (that_row_matches_this_row[row2] == true) break;

		if (row2 < al1_depth)
		{
			// This means that there is at least one row
			// which matches row1 completely.

			redundant_rows_array[row1] = true;
		}
	}

	// Now check to see whether any rows need to be edited out and 
	// do it where necessary.

	int new_al_depth = al1_depth;
	for (row1 = 0; row1 < al1_depth; row1++)
		if (redundant_rows_array[row1] == true) new_al_depth--;

	if (new_al_depth == al1_depth) return(false);

	// al1 is now to be edited. Do this by replacing each column by
	// a new column. The 'shell' of al1 is preserved to avoid upsetting
	// any pointers to it which may exist in the global data structure.

	alignment_element* al_el2 = NULL;
	int col1 = -1;
	al1->initialise();
	while (al_col1 = (symbol*)al1->get_next_child())
	{
		al_col2 = new symbol(al_col1->get_name(), new_al_depth,
			NULL_VALUE);
		plus_one(&col1, al1_length, "col1 is too large in \
				check_for_redundant_rows");
		new_alignment_array[col1] = al_col2;
		row2 = -1;
		for (row1 = 0; row1 < al1_depth; row1++)
		{
			if (redundant_rows_array[row1] == true) continue;
			// This row is to be omitted from the
			// edited version of al1.

			// This row is to be preserved.

			plus_one(&row2, new_al_depth,
				"row2 is too large in check_for_redundant_rows");

			al_el1 = al_col1->get_al_el(row1);
			al_el2 = al_col2->get_al_el(row2);
			*al_el2 = *al_el1;
		}
	}

	// Delete all the columns in al1.

	while (al_col1 = (symbol*)al1->get_first_child())
	{
		al1->extract_child(al_col1);
		delete al_col1;
	}

	// Now put in the new columns currently stored in
	// new_alignment_array[].

	for (col1 = 0; col1 < al1_length; col1++)
		al1->add_child(new_alignment_array[col1]);

	// Re-set sequence_depth in al1.

	al1->set_sequence_depth(new_al_depth);

	// Re-set values in each column of al1.

	al1->initialise();
	int symbol_count;
	while (al_col1 = (symbol*)al1->get_next_child())
	{
		al_col1->set_sequence_depth(new_al_depth);

		// Does this column contain at least one hit?

		symbol_count = 0;
		for (row1 = 0; row1 < new_al_depth; row1++)
		{
			al_el1 = al_col1->get_al_el(row1);
			if (al_el1->get_el_obj() != NIL)
				symbol_count++;
		}

		if (symbol_count > 1) al_col1->set_is_a_hit(true);
		else al_col1->set_is_a_hit(false);

		// For each columnm, put the correct values in for 
		// same_column_above and same_column_below.

		for (row1 = 0; row1 < new_al_depth; row1++)
		{
			al_el1 = al_col1->get_al_el(row1);
			if (al_el1->get_el_obj() == NIL) continue;
			for (row2 = row1 + 1; row2 < new_al_depth; row2++)
			{
				al_el2 = al_col1->get_al_el(row2);
				if (al_el2->get_el_obj() != NIL) break;
			}
			if (row2 >= new_al_depth) break;
			al_el1->set_same_column_below(row2);
			al_el2->set_same_column_above(row1);
		}
	}

	return(true);

} // check_for_redundant_rows

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	If there is a best sequence, this function calculates the
**	probabilities of inferences from the best sequence and all
**	other sequences which encode the same symbols from New.
**
**	The function calculates 'Absolute P' which is the absolute probability
**	of each sequence (derived directly from its 'old symbols cost')
**	and 'Relative P' which is the probability of each sequence relative
**	to other sequences which encode the same symbols from New.
**
**	The function also calculates the probabilities of patterns and
**	symbols within the sequences as described in sp_ideas6, %37.
**	For each pattern found anywhere amongst the set of sequences
**	which encode the same symbols from New, the function adds up
**	the values of Small p for the sequences in which it is found,
**	giving a value of finding at least one instance of the pattern
**	in an alignment. Likewise for individual symbols.
**
** CALLING SEQUENCE:
**
**	void probabilities_of_inferences(sequence *best_alignment)
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	best_alignment: The best sequence found at the end of compression.
**
** IMPLICIT INPUTS:
**
**	Global variables
**
** IMPLICIT OUTPUTS:
**
**	Prints all sequences which code the same symbols from New as
**	the best sequence and calculates probabilities of the
**	associated inferences.
**
** SIDE EFFECTS:
**
**	NONE
*/

void probabilities_of_inferences(sequence* best_alignment)
{
	symbol* B_symbol1;
	alignment_element* al_el1;
	sequence* pattern;
	symbol* new_hit_symbol, * symbol1, * symbol2, * hit_symbol;
	group* new_hits = new group(); /* This is
		the head of a list of symbols from New
		which have formed hits in the best_alignment. */
	sequence* al1, * al2;
	struct best_alignment_entry* sequence_entry;
	struct patterns_entry
	{
		sequence* pattern;
		double probability;
		bool in_current_sequence, done;
	} patterns_in_sequences[MEDIUM_SCRATCH_ARRAY_SIZE];

	struct symbols_entry
	{
		symbol* symbol1;
		double probability;
		bool in_current_sequence, done;
	} symbols_in_sequences[MEDIUM_SCRATCH_ARRAY_SIZE];

	double total_abs_P;
	int i, j, k, fe_p_in_als, fe_s_in_als, row1,
		sequence_depth;
	bool sequence_found, symbol_found;

	if (best_alignment == NIL)
	{
		fprintf(output_file,
			"NO GOOD ALIGNMENTS OR INFERENCES FOUND\n\n");
		return;
	}

	fe_best_alignments = 0;

	best_alignments[fe_best_alignments].al1 = best_alignment;
	best_alignments[fe_best_alignments].contained_in = NIL;
	fe_best_alignments++;

	/* Compile symbols from New which are hit_symbols. */

	best_alignment->initialise();
	while (B_symbol1 = (symbol*)best_alignment->get_next_child())
	{
		al_el1 = B_symbol1->get_al_el(0);
		if (al_el1->get_same_column_below() == NULL_VALUE) continue;

		hit_symbol = (symbol*)al_el1->get_el_obj();
		new_hits->add_child(hit_symbol);
	}

	/* Now look for sequences (other than best_alignment), which
	have the same hit_symbols in New, neither more nor less.
	For each one found, check for redundant rows and remove them
	(see sp61_od, %49). For each alignment which is edited in this
	way, check to see that it is not identical to one already in
	the reference set of alignments. If it is, do not add it to the
	set. */

	bool alignment_is_OK;
	sequence* modified_alignments[MEDIUM_SCRATCH_ARRAY_SIZE];
	int fe_modified_alignments = 0, modified_alignments_index;

	list_for(al1, sequence, parsing_alignments)
	{
		if (al1 == best_alignment) continue;

		new_hits->initialise();
		new_hit_symbol = (symbol*)new_hits->get_next_child();
		al1->initialise();
		while (B_symbol1 = (symbol*)al1->get_next_child())
		{
			al_el1 = B_symbol1->get_al_el(0);
			if (al_el1->get_el_obj() == NIL) continue;
			if (al_el1->get_same_column_below() == NULL_VALUE)
				continue;

			hit_symbol = (symbol*)al_el1->get_el_obj();
			if (hit_symbol != new_hit_symbol)
				/* There is a mis-match. */
				goto L1;

			new_hit_symbol = (symbol*)new_hits->get_next_child();
		}

		if (new_hit_symbol != NIL) continue;

		/* All the symbols in new_hits have been matched
		and none others - which means that this
		sequence is OK.

		Check to see whether the alignment contains
		any redundant rows. If it has, remove it or them, then place
		the modified alignment in modified_alignments[] to
		be checked later to see whether it duplicates any other
		alignment in parsing_alignments. */

		if (check_for_redundant_rows(al1))
		{
			fprintf(output_file, "%s%d%s",
				"Alignment %",
				al1->get_ID(),
				" has been edited.\n\n");
			al1->write_alignment(output_file,
				write_section_chars_length, NULL_VALUE,
				alignment_format);
			modified_alignments[fe_modified_alignments] = al1;
			plus_one(&fe_modified_alignments,
				MEDIUM_SCRATCH_ARRAY_SIZE,
				"modified_alignments[] is too small");
		}
		else
		{
			best_alignments[fe_best_alignments].al1 = al1;
			plus_one(&fe_best_alignments,
				MEDIUM_SCRATCH_ARRAY_SIZE,
				"best_alignments[] too small.");
		}

	L1:;
	}

	for (modified_alignments_index = 0; modified_alignments_index <
		fe_modified_alignments; modified_alignments_index++)
	{
		al1 = modified_alignments[modified_alignments_index];
		alignment_is_OK = true;
		list_for(al2, sequence, parsing_alignments)
		{
			if (al2 == al1) continue;
			if (al2->alignment_matches(al1))
			{
				if (verbose)
				{
					fprintf(output_file, "Alignment ");
					al1->print_ID();
					fprintf(output_file, " matches sequence ");
					al2->print_ID(),
						fprintf(output_file, " and is discarded.\n\n");
					fflush(output_file);
				}

				parsing_alignments->extract_child(al1);
				delete al1;
				alignment_is_OK = false;
				break;
			}
		}

		if (alignment_is_OK)
		{
			fprintf(output_file, "%s%d%s",
				"After editing, alignment %",
				al1->get_ID(),
				" does not match any earlier alignment\n\n");
			al1->make_code(false);
			fprintf(output_file, "%s%d%s",
				"New scores for alignment %",
				al1->get_ID(),
				" are:\n\n");
			fprintf(output_file,
				"%s%1.2f%s%1.2f%s%1.2f%s%1.2f%s%1.12g%s",
				"NSC = ", al1->get_new_symbols_cost(),
				", EC = ", al1->get_encoding_cost(),
				", CR = ", al1->get_compression_ratio(),
				", CD = ", al1->get_compression_difference(),
				",\nAbsolute P = ", al1->get_abs_P(),
				"\n\n");

			best_alignments[fe_best_alignments].al1 = al1;
			plus_one(&fe_best_alignments,
				MEDIUM_SCRATCH_ARRAY_SIZE,
				"best_alignments[] too small.");
		}
	}

#if CONTAINED_IN

	/* For each sequence in best_alignments[], compile a list of the
	other best sequences which contain it, if any. */

	find_containing_sequences();

#endif

	/* Calculate total_abs_P. */

	total_abs_P = 0;
	for (i = 0; i < fe_best_alignments; i++)
	{
		al1 = best_alignments[i].al1;
		total_abs_P += al1->get_abs_P();
	}

	/* Calculate rel_P for each of the alignments in best_alignments[].
	Then write out the alignments and probabilities in order of
	relative probability. */

	fprintf(output_file, "%s%s%1.12g%s",
		"BEST ALIGNMENTS AND PROBABILITIES\n\n",
		"Total absolute P = ",
		total_abs_P, "\n\n");
	for (i = 0; i < fe_best_alignments; i++)
	{
		sequence_entry = &best_alignments[i];
		al1 = sequence_entry->al1;
		sequence_entry->rel_P = al1->get_abs_P() / total_abs_P;
		sequence_entry->done = false; // To be used in printing.
	}

	double biggest_rel_P, temp_rel_P;
	int index_of_biggest;

	while (true)
	{
		biggest_rel_P = NULL_VALUE;
		index_of_biggest = NULL_VALUE;
		for (i = 0; i < fe_best_alignments; i++)
		{
			sequence_entry = &best_alignments[i];
			if (sequence_entry->done == true) continue;
			temp_rel_P = sequence_entry->rel_P;
			if (temp_rel_P > biggest_rel_P)
			{
				biggest_rel_P = temp_rel_P;
				index_of_biggest = i;
			}
		}

		if (index_of_biggest == NULL_VALUE) break;
		sequence_entry = &best_alignments[index_of_biggest];
		sequence_entry->done = true;
		al1 = sequence_entry->al1;

		fprintf(output_file, "ALIGNMENT:\n\n");
		sequence_entry->al1->print_ID();
		fprintf(output_file,
			"%s%1.2f%s%1.2f%s%1.2f%s%1.2f%s%1.12g%s%1.12g%s",
			", NSC = ",
			al1->get_new_symbols_cost(),
			", EC = ",
			al1->get_encoding_cost(),
			", CR = ",
			al1->get_compression_ratio(),
			", CD = ",
			al1->get_compression_difference(),
			",\nAbsolute P = ", al1->get_abs_P(),
			", Relative P = ", sequence_entry->rel_P,
			"\n\n");

		al1->write_alignment(output_file,
			write_section_chars_length, NULL_VALUE,
			alignment_format);
	}

	/* Compile a set of patterns each of which appears at least
	once in one of best_alignments[]. At the same time, calculate
	the probability of each pattern as the sum of the rel_P
	probabilities of the sequences in which it appears. */

	for (i = 0; i < MEDIUM_SCRATCH_ARRAY_SIZE; i++)
	{
		patterns_in_sequences[i].pattern = NIL;
		patterns_in_sequences[i].probability = 0;
		patterns_in_sequences[i].in_current_sequence = false;
		patterns_in_sequences[i].done = false;
	}

	fe_p_in_als = 0;

	for (i = 0; i < fe_best_alignments; i++)
	{
		al1 = best_alignments[i].al1;

		/* Initialise values of in_current_sequence in
		patterns_in_sequences[]. */

		for (j = 0; j < fe_p_in_als; j++)
			patterns_in_sequences[j].in_current_sequence =
			false;

		B_symbol1 = (symbol*)al1->get_first_child();
		al1->set_current_el_pos(al1->get_first_el_pos());
		sequence_depth = al1->get_sequence_depth();

		/* Exclude the first row of the sequence, which
		corresponds to the cnp. */

		for (row1 = 1; row1 < sequence_depth; row1++)
		{
			pattern = B_symbol1->get_row_pattern(row1);

			/* Check patterns_in_sequences[] to see if there
			is already an entry for pattern. If there is,
			use that entry, otherwise make a new entry. */

			sequence_found = false;
			for (k = 0; k < fe_p_in_als; k++)
			{
				if (patterns_in_sequences[k].pattern ==
					pattern)
				{
					sequence_found = true;
					break;
				}
			}

			if (!sequence_found)
			{
				patterns_in_sequences[fe_p_in_als].pattern =
					pattern;

				k = fe_p_in_als;
				plus_one(&fe_p_in_als,
					MEDIUM_SCRATCH_ARRAY_SIZE,
					"patterns_in_sequences[] \
					is too small");
			}

			/* Now add to the probability value for this
			pattern if it has not already been done in
			this sequence. */

			if (patterns_in_sequences[k].in_current_sequence ==
				true)
				continue;

			patterns_in_sequences[k].in_current_sequence = true;
			patterns_in_sequences[k].probability +=
				best_alignments[i].rel_P;
		}
	}

	fprintf(output_file, "PROBABILITIES OF PATTERNS:\n\n");

	double biggest_probability, temp_probability;

	while (true)
	{
		biggest_probability = NULL_VALUE;
		index_of_biggest = NULL_VALUE;
		for (i = 0; i < fe_p_in_als; i++)
		{
			if (patterns_in_sequences[i].done == true) continue;
			temp_probability =
				patterns_in_sequences[i].probability;
			if (temp_probability > biggest_probability)
			{
				biggest_probability = temp_probability;
				index_of_biggest = i;
			}
		}

		if (index_of_biggest == NULL_VALUE) break;
		patterns_in_sequences[index_of_biggest].done = true;

		pattern = patterns_in_sequences[index_of_biggest].pattern;
		pattern->print_ID();
		fprintf(output_file, "%s%1.12g%s",
			", p = ",
			patterns_in_sequences[index_of_biggest].probability,
			" ");

		pattern->write_tree_object(PRINT_SEQUENCE_FREQUENCY);
	}

	/* Compile a set of symbols each of which appears at least
	once in one of best_alignments[]. At the same time, calculate
	the probability of each symbol as the sum of the rel_P
	probabilities of the sequences in which it appears. */

	for (i = 0; i < MEDIUM_SCRATCH_ARRAY_SIZE; i++)
	{
		symbols_in_sequences[i].symbol1 = NIL;
		symbols_in_sequences[i].probability = 0;
		symbols_in_sequences[i].in_current_sequence = false;
		symbols_in_sequences[i].done = false;
	}

	fe_s_in_als = 0;

	for (i = 0; i < fe_best_alignments; i++)
	{
		al1 = best_alignments[i].al1;

		/* Initialise values of in_current_sequence in
		symbols_in_sequences[]. */

		for (j = 0; j < fe_s_in_als; j++)
			symbols_in_sequences[j].in_current_sequence =
			false;

		get_next_symbol_from_sequence(al1, START);
		while (symbol2 = get_next_symbol_from_sequence(al1, CONTINUE))
		{
			/* Check symbols_in_sequences[] to see if there
			is already an entry for symbol2. If there is,
			use that entry, otherwise make a new entry. */

			symbol_found = false;
			for (k = 0; k < fe_s_in_als; k++)
			{
				symbol1 = symbols_in_sequences[k].symbol1;
				if (strcmp(symbol1->get_name(),
					symbol2->get_name()) == 0)
				{
					symbol_found = true;
					break;
				}
			}

			if (!symbol_found)
			{
				symbols_in_sequences[fe_s_in_als].symbol1 =
					symbol2;

				k = fe_s_in_als;
				plus_one(&fe_s_in_als,
					MEDIUM_SCRATCH_ARRAY_SIZE,
					"symbols_in_alingments[] \
					is too small");
			}

			/* Now add to the probability value for this
			symbol if it has not already been done in
			this sequence. */

			if (symbols_in_sequences[k].in_current_sequence ==
				true)
				continue;

			symbols_in_sequences[k].in_current_sequence = true;
			symbols_in_sequences[k].probability +=
				best_alignments[i].rel_P;
		}
	}

	fprintf(output_file, "PROBABILITIES OF SYMBOLS:\n\n");

	while (true)
	{
		biggest_probability = NULL_VALUE;
		index_of_biggest = NULL_VALUE;
		for (i = 0; i < fe_s_in_als; i++)
		{
			if (symbols_in_sequences[i].done == true) continue;
			temp_probability =
				symbols_in_sequences[i].probability;
			if (temp_probability > biggest_probability)
			{
				biggest_probability = temp_probability;
				index_of_biggest = i;
			}
		}

		if (index_of_biggest == NULL_VALUE) break;
		symbols_in_sequences[index_of_biggest].done = true;

		symbol2 = symbols_in_sequences[index_of_biggest].symbol1;
		fprintf(output_file, "%s%s%1.12g%s",
			symbol2->get_name(), ", p = ",
			symbols_in_sequences[index_of_biggest].probability,
			" ");
		fprintf(output_file, "\n");
	}

	fprintf(output_file, "\n");

	new_hits->release_children();
	delete new_hits;
} /* probabilities_of_inferences */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Using existing markings of New or Old symbols with IDENTIFICATION
**	status, this function compiles a list of symbol types that have
**	that status, excluding LEFT_BRACKET and RIGHT_BRACKET. It
**	then applies the list to patterns in Old and New,
**	marking all symbols that appear on the list as being type
**	CONTEXT_SYMBOL. All other symbols are marked
**	as type DATA_SYMBOL.
**
** CALLING SEQUENCE:
**
**	void assign_type_to_symbols()
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
** IMPLICIT INPUTS:
**
**	Patterns in Old.
**
** IMPLICIT OUTPUTS:
**
**	Marking of symbols in Old and New with type CONTEXT_SYMBOL or DATA_SYMBOL.
**
** SIDE EFFECTS:
**
**	NONE
**
*/

void assign_type_to_symbols()
{
	group* css = new group;
	symbol* symbol1, * symbol2, * symbol3;
	sequence* pattern1;
	char c;

	// Compile a list of symbol types which are of CONTEXT_SYMBOL type.
	// It is assumed that these symbol types are the types of the
	// symbols in New or Old that have IDENTIFICATION status. The
	// symbols '<' and '>' are, slightly redundantly, given the type
	// LEFT_BRACKET and RIGHT_BRACKET respectively.

	list_for(pattern1, sequence, old_patterns)
	{
		list_for(symbol1, symbol, pattern1)
		{
			c = *(symbol1->get_name());
			if (symbol1->get_status() != IDENTIFICATION) continue;

			// if (c == '<' || c == '>') continue ;

			// Check whether the type of symbol1 is already registered
			// in css. If it is, continue. If not, add a copy of symbol1
			// to this list.

			list_for(symbol2, symbol, css)
				if (symbol2->name_matches(symbol1)) goto L1;

			// No matching symbol type has been found. Add a copy
			// of symbol1 to css.

			symbol3 = symbol1->shallow_copy();
			css->add_child(symbol3);

		L1:;
		}
	}

	list_for(pattern1, sequence, new_patterns)
	{
		list_for(symbol1, symbol, pattern1)
		{
			c = *(symbol1->get_name());
			if (symbol1->get_status() != IDENTIFICATION) continue;

			// if (c == '<' || c == '>') continue ;

			// Check whether the type of symbol1 is already registered
			// in css. If it is, continue. If not, add a copy of symbol1
			// to the list.

			list_for(symbol2, symbol, css)
				if (symbol2->name_matches(symbol1)) goto L2;

			// No matching symbol type has been found. Add a copy
			// of symbol1 to css.

			symbol3 = symbol1->shallow_copy();
			css->add_child(symbol3);

		L2:;
		}
	}

	// Traverse the New patterns marking all symbols as DATA symbols.

	list_for(pattern1, sequence, new_patterns)
	{
		list_for(symbol1, symbol, pattern1)
			symbol1->set_type(DATA_SYMBOL);
	}

	// Now traverse Old, marking symbols as type
	// CONTEXT_SYMBOL if a copy appears in css,
	// and marking them as type DATA_SYMBOL if a copy does not appear
	// on the list. The symbols '<' and '>' are marked as
	// LEFT_BRACKET and RIGHT_BRACKET respectively.

	list_for(pattern1, sequence, old_patterns)
	{
		list_for(symbol1, symbol, pattern1)
		{
			if (*(symbol1->get_name()) == '<')
			{
				symbol1->set_type(LEFT_BRACKET);
				continue;
			}

			if (*(symbol1->get_name()) == '>')
			{
				symbol1->set_type(RIGHT_BRACKET);
				continue;
			}

			// Check whether a copy of symbol1 appears
			// in css. If it does, mark symbol1 as
			// the type CONTEXT_SYMBOL. If symbol1 does not appear
			// on the list, mark it as a DATA_SYMBOL.

			if (css->contains_copy_of(symbol1))
				symbol1->set_type(CONTEXT_SYMBOL);
			else symbol1->set_type(DATA_SYMBOL);
		}
	}

	// Delete css and all its children.

	delete css;

} // assign_type_to_symbols

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	For each pattern in Old, identifies BOUNDARY_MARKERs, IDENTIFICATION
**	symbols and CONTENTS symbols. Finds the coding cost of each
**	pattern in Old as described in sp70_od, %3 - total of the
**	bit_costs for the 'coding' symbols for each pattern.
**
**	The IDENTIFICATION symbols for any pattern are: the one
**	or more symbols at the start of the pattern which are needed to
**	identify the pattern uniquely amongst the patterns in the corpus plus
**	the termination symbol for the pattern (if any) but excluding
**	brackets at the start and end of the pattern.
**
**	The status of symbols in New are either the default status of
**	CONTENTS, or they are explicitly marked in the input, or they
**	acquired their status in a previous iteration of
**	created_patterns_and_grammars().
**
** CALLING SEQUENCE:
**
**	void assign_status_to_symbols()
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
**
*/

void assign_status_to_symbols()
{
	sequence* pattern1, * matching_pattern = NULL;
	symbol* symbol1, * symbol2;
	bool end_of_leading_code_sequence_found;
	char c, * name;
	list_element* el_pos1;

	list_for(pattern1, sequence, new_patterns)
	{
		list_for(symbol1, symbol, pattern1)
			symbol1->set_status(IDENTIFICATION);
	}

	list_for(pattern1, sequence, old_patterns)
	{
		end_of_leading_code_sequence_found = false;
		el_pos1 = NIL;
		list_for_el_pos(symbol1, symbol, pattern1, el_pos1)
		{
			if (symbol1->name_is("<"))
			{
				symbol2 = (symbol*)pattern1->get_first_child();
				if (symbol1 == symbol2) continue;
			}

			symbol1->set_status(IDENTIFICATION);
			if (is_unique(pattern1, symbol1, &matching_pattern))
			{
				end_of_leading_code_sequence_found = true;
				break;
			}
		}

		if (!end_of_leading_code_sequence_found)
		{
			fprintf(output_file, "FIRST PATTERN: ");
			pattern1->print_ID();
			fprintf(output_file, "\n");
			fprintf(output_file, "SECOND PATTERN: ");
			matching_pattern->print_ID();
			fprintf(output_file, "\n\n");
			abort_run("Non-unique pattern found in old_patterns");
		}

		// Now mark the remaining symbols in pattern1 as CONTENTS,
		// except the last symbol if its contents character string
		// begins with '#' or is '>'.

		while (symbol1 = (symbol*)pattern1->get_next_child())
		{
			if (pattern1->this_is_last_child(symbol1))
			{
				name = symbol1->get_name();
				c = *name;
				if (c == '#' || strcmp(name, ">") == 0)
				{
					symbol1->set_status(IDENTIFICATION);
					break;
				}
			}
			symbol1->set_status(CONTENTS);
		}
	}
} // assign_status_to_symbols

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Using the frequencies and costs of symbol types recorded in
**	set_of_symbols, this function assigns these values to
**	instances of symbols in the corpus.
**
** CALLING SEQUENCE:
**
**	void assign_symbol_frequencies_and_costs(group *set_of_symbols)
**
** FORMAL ARGUMENTS:
**
**	Return value:		void
**
**	set_of_symbols:		The alphabet of symbol types (each with values
**						for frequency and bit_cost) that
**						will be used in this function. This will either
**						be original_symbols_in_corpus or it will be
**						symbol_types_in_old.
**
** IMPLICIT INPUTS:
**
**	original_symbols_in_corpus
**
** IMPLICIT OUTPUTS:
**
**	frequency values of symbols in old_patterns
**
** SIDE EFFECTS:
**
**	NONE
*/

void assign_symbol_frequencies_and_costs(group* set_of_symbols)
{
	sequence* pattern1;
	symbol* symbol_type, * symbol1;

	/* Run through all symbols in the corpus and, for each one,
	look for the matching symbol type in set_of_symbols. When
	a match is found, give the symbol in the corpus the frequency
	of the matching symbol type in set_of_symbols and the costs of
	that symbol type. */

	fprintf(output_file,
		"NEW FREQUENCIES AND COSTS ASSIGNED TO PATTERNS AND SYMBOLS:\n\n");

	list_for(pattern1, sequence, new_patterns)
	{
		list_for(symbol1, symbol, pattern1)
		{
			list_for(symbol_type, symbol, set_of_symbols)
			{
				if (strcmp(symbol_type->get_name(),
					symbol1->get_name()) != 0)
					continue;
				symbol1->set_frequency(symbol_type->
					get_frequency());
				symbol1->set_bit_cost(symbol_type->
					get_bit_cost());
				break;
			}
		}
	}

	list_for(pattern1, sequence, old_patterns)
	{
		list_for(symbol1, symbol, pattern1)
		{
			list_for(symbol_type, symbol, set_of_symbols)
			{
				if (strcmp(symbol_type->get_name(),
					symbol1->get_name()) != NIL)
					continue;
				symbol1->set_frequency(symbol_type->
					get_frequency());
				symbol1->set_bit_cost(symbol_type->
					get_bit_cost());
				break;
			}
		}
	}

} /* assign_symbol_frequencies_and_costs */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	This function finds the frequencies of symbol types by adding
**	the frequency of the pattern in which it appears to the
**	symbol type for each appearance of a symbol type in the corpus.
**
** CALLING SEQUENCE:
**
**	void find_symbol_frequencies(group *symbol_set, bool do_new,
**		bool do_old, sequence *cnp)
**
** FORMAL ARGUMENTS:
**
**	Return value:			void
**
**	symbol_set:				The alphabet of symbols whose frequencies
**							are to be set.
**	do_new:					Include new_patterns in the count.
**	do_old:					Include old_patterns in the count.
**	cnp:		The last New pattern to be processed in
**							the main program. If the value is NIL,
**							all New patterns are to be processed.
**
** IMPLICIT INPUTS:
**
**	NONE
**
** IMPLICIT OUTPUTS:
**
**	frequencies of symbols recorded in symbol_set.
**
** SIDE EFFECTS:
**
**	NONE
*/

void find_symbol_frequencies(group* symbol_set, bool do_new,
	bool do_old, sequence* cnp)
{
	symbol* symbol1;

	/* Initialise frequencies of symbols in symbol_set. */

	symbol_set->initialise();
	while (symbol1 = (symbol*)symbol_set->get_next_child())
		symbol1->set_frequency(0);

	if (do_new) new_patterns->compile_frequencies(symbol_set, cnp);

	if (do_old) old_patterns->compile_frequencies(symbol_set, NIL);

} /* find_symbol_frequencies */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Write out New and Old patterns for reference.
**
** CALLING SEQUENCE:
**
**	void write_new_and_old()
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
** IMPLICIT INPUTS:
**
**	Data structures
**
** IMPLICIT OUTPUTS:
**
**	Output to output_file.
**
** SIDE EFFECTS:
**
**	NONE
*/

void write_new_and_old()
{
	/* Identify alignments in New and Old and write them out for reference,
	with the coding costs for patterns in Old. */

	fprintf(output_file, "PATTERNS IN NEW:\n\n");
	new_patterns->initialise();
	if (new_patterns->is_empty()) fprintf(output_file, "NONE\n\n");
	else new_patterns->write_patterns_with_details();
	fprintf(output_file, "%s%d%s",
		"Size of New (symbols): ", number_of_symbols_in_new, "\n");
	fprintf(output_file, "%s%d%s",
		"Size of New (patterns): ", number_of_patterns_in_new, "\n\n");

	fprintf(output_file, "ORIGINAL PATTERNS IN OLD:\n\n");
	old_patterns->initialise();
	if (old_patterns->is_empty()) fprintf(output_file, "NONE\n\n");
	else old_patterns->write_patterns_with_details();
	fprintf(output_file, "%s%d%s",
		"Size of Old (symbols): ", number_of_symbols_in_old, "\n");
	fprintf(output_file, "%s%d%s",
		"Size of Old (patterns): ", original_number_of_patterns_in_old, "\n\n");

	fflush(output_file);

} /* write_new_and_old */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Called from find_containing_sequences.
**
** CALLING SEQUENCE:
**
**	bool contained_in(sequence *sequence1, sequence *test_sequence)
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if sequence1 is contained
**					in test_sequence,
**					false otherwise.
**
**	sequence1:		The given sequence.
**	test_sequence:	The sequence being tested to see whether it
**					contains the given sequence or not.
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

#if CONTAINED_IN

bool contained_in(sequence* sequence1, sequence* test_sequence)
{
	symbol* B_symbol1, * al_col2;
	sequence* pattern_of_B_symbol1, * pattern_of_al_col2;
	symbol* first_symbol;
	int col1, col2, number_of_children, test_number_of_children,
		sequence_depth = 1, test_sequence_depth = 1,
		ra_index, fe_rows_array = 0, i, offset;
	struct rows_entry
	{
		symbol* B_symbol1, * al_col2;
		sequence* pattern_of_B_symbol1, * pattern_of_al_col2;
		bool match_found;
	} rows_array[MEDIUM_SCRATCH_ARRAY_SIZE];
	bool match_found, original_pattern_match_found;

	if (sequence1 == NIL || test_sequence == NIL)
		abort_run("NIL parameter(s) for contained_in");
	if (typeof(sequence1) != typeof(test_sequence)) return(false);
	if (typeof(sequence) == SYMBOL || typeof(test_sequence) == SYMBOL)
		abort_run("SYMBOL parameter(s) for contained_in");

	/* Compare the lengths of the sequences. */

	number_of_children = sequence1->number_of_children;
	test_number_of_children = test_sequence->number_of_children;

	if (number_of_children > test_number_of_children) return(false);

	/* Find sequence depth. */

	sequence1->initialise();
	while (B_symbol1 = (symbol*)sequence1->get_next_child())
		sequence_depth++;

	/* Find test_sequence_depth and compare with sequence_depth. */

	test_sequence->initialise();
	while (B_symbol1 = (symbol*)test_sequence->get_next_child())
		test_sequence_depth++;

	if (sequence_depth > test_sequence_depth) return(false);

	/* Initialise rows_array[]. */

	for (ra_index = 0; ra_index < MEDIUM_SCRATCH_ARRAY_SIZE; ra_index++)
	{
		rows_array[ra_index].B_symbol1 = NIL;
		rows_array[ra_index].pattern_of_B_symbol1 = NIL;
		rows_array[ra_index].al_col2 = NIL;
		rows_array[ra_index].pattern_of_al_col2 = NIL;
		rows_array[ra_index].match_found = false;
	}

	/* Now fill in values for rows_array[]. The original pattern
	of each Old row in sequence1 is compared with the original_pattern
	each Old row in test sequence1. If there are no matches,
	this means that sequence1 is not contained in
	test_sequence. Where there are one or more matches,
	these are entered in rows_array[]. */

	sequence1->initialise();
	B_symbol1 = (symbol*)sequence1->get_next_child();
	while (B_symbol1 = (symbol*)sequence1->get_next_child())
	{
		original_pattern_match_found = false;
		test_sequence->initialise();
		B_symbol1 = (symbol*)test_sequence->get_next_child();
		while (B_symbol1 = (symbol*)
			test_sequence->get_next_child())
		{
			if (B_symbol1->original_pattern != al_col2->el_obj)
				continue;

			original_pattern_match_found = true;

			/* We have a positive match of the original_patterns.
			Now record it in rows_array[]. */

			rows_array[fe_rows_array].B_symbol1 = B_symbol1;
			rows_array[fe_rows_array].pattern_of_B_symbol1 =
				B_symbol1->el_obj;
			rows_array[fe_rows_array].al_col2 = al_col2;
			rows_array[fe_rows_array].pattern_of_al_col2 =
				al_col2->el_obj;
			fe_rows_array = plus_one(fe_rows_array,
				MEDIUM_SCRATCH_ARRAY_SIZE,
				"rows_array[] too small in contained_in");
		}
		if (original_pattern_match_found == false) return(false);
	}

	/* Now check to see whether the pattern of hits for each Old row
	of sequence1 matches a pattern of hits (for the same original_pattern)
	in an Old row of test_sequence. The order of matching rows need not
	be the same in the two sequences. It is not safe to assume that the
	two sequences have the same length: if test_sequence is longer
	than sequence1, it is not safe to assume that the pattern of hits
	in any one row in sequence1 starts at the same symbol as
	test_sequence. Likewise for the end of a pattern of hits.
	If the symbols do not coincide, then, for each row, symbols
	in test_sequence which are outside the pattern of hits must all
	be NIL. */

	/* Find an Old row in sequence1 where the first symbol in the
	row is not NIL. */

	sequence1->initialise();
	B_symbol1 = (symbol*)sequence1->get_next_child();
	while (B_symbol1 = (symbol*)sequence1->get_next_child())
	{
		if (B_symbol1->symbol_sequence[0] != NIL)
		{
			pattern_of_B_symbol1 = B_symbol1->el_obj;
			first_symbol = B_symbol1->symbol_sequence[0];
			break;
		}
	}

	/* Now find row(s) in test_sequence with the same original_pattern and
	find the position in each of these rows of the first symbol of
	the original_pattern. From the one which starts furthest to the left,
	we can find the offset of the start of sequence1
	relative to the start of test_sequence. */

	pattern_of_al_col2 = NIL;
	offset = 1000000;
	test_sequence->initialise();
	B_symbol1 = (symbol*)test_sequence->get_next_child();
	while (B_symbol1 = (symbol*)test_sequence->get_next_child())
	{
		if (al_col2->el_obj == pattern_of_B_symbol1)
		{
			for (col2 = 0; col2 < test_number_of_children; col2++)
			{
				if (al_col2->symbol_sequence[col2] !=
					first_symbol) continue;
				if (col2 < offset) offset = col2;
				break;
			}
			if (col2 >= test_number_of_children)
				abort_run("Error in contained_in");
		}
	}

	if (offset >= test_number_of_children)
		abort_run("Error in contained_in");

	ra_index = 0;
	sequence1->initialise();
	B_symbol1 = (symbol*)sequence1->get_next_child();
	while (B_symbol1 = (symbol*)sequence1->get_next_child())
	{
		match_found = false;
		while (rows_array[ra_index].B_symbol1 == B_symbol1)
		{
			al_col2 = rows_array[ra_index].al_col2;

			/* Check that this al_col2 has
			not already been 'used'. */

			for (i = 0; i < ra_index; i++)
				if (rows_array[i].al_col2 == al_col2
					&& rows_array[i].match_found)
					goto L1;

			/* Step over the offset in al_col2, checking that
			it contains nothing other than NIL values. */

			for (col2 = 0; col2 < offset; col2++)
				if (al_col2->symbol_sequence[col2] != NIL)
					goto L1;

			/* Now compare the pattern of hits in B_symbol1 and
			al_col2. */

			for (col1 = 0; col1 < number_of_children; col1++)
			{
				if (col2 >= test_number_of_children) goto L1;
				if (B_symbol1->symbol_sequence[col1] !=
					al_col2->symbol_sequence[col2])
					goto L1;
				col2++;
			}

			/* Check that any trailing symbols in al_col2 are
			all NIL. */

			for (col2 = col2; col2 < test_number_of_children; col2++)
				if (al_col2->symbol_sequence[col2] != NIL)
					goto L1;

			match_found = true;
			rows_array[ra_index].match_found = true;
			while (rows_array[ra_index].B_symbol1 == B_symbol1)
				if (++ra_index >= fe_rows_array) break;

			break;

		L1:;
			if (++ra_index >= fe_rows_array) break;
		}
		if (!match_found) return(false);
	}
	return(true);
} /* contained_in */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	For each of the sequences in best_sequences[], this function
**	compiles a list of the other best sequences, if any, which
**	contain the given sequence.
**
** CALLING SEQUENCE:
**
**	void find_containing_sequences()
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
** IMPLICIT INPUTS:
**
**	best_sequences[]
**
** IMPLICIT OUTPUTS:
**
**	For each best_sequence[], a list of containing sequences which
**	may be NIL.
**
** SIDE EFFECTS:
**
**	NONE
**
*/

void find_containing_sequences()
{
	int i, j;

	for (i = 0; i < fe_best_sequences; i++)
	{
		for (j = 0; j < fe_best_sequences; j++)
		{
			if (i == j) continue;
			if (contained_in(best_sequences[i].al_el,
				best_sequences[j].al_el))
			{
				fprintf(output_file, "Alignment ");
				best_sequences[i].al_el->print_ID();
				fprintf(output_file, " is contained in ");
				best_sequences[j].al_el->print_ID();
				fprintf(output_file, "\n\n");
			}
		}
	}
} /* find_containing_sequences */

#endif

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Called from grammar::tidy_up_code_symbols(). This method looks
**	for CLASS_SYMBOLs and renumbers them from 1. Likewise for
**	UNIQUE_ID_SYMBOLs.
**
** CALLING SEQUENCE:
**
**	void grammar::renumber_code_symbols(int symbol_type)
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	symbol_type:	The type of symbol to be renumbered (it should
**					be CONTEXT_SYMBOL or UNIQUE_ID_SYMBOL.
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

void grammar::renumber_code_symbols(int symbol_type)
{
	if (symbol_type != CONTEXT_SYMBOL
		&& symbol_type != UNIQUE_ID_SYMBOL)
		abort_run("Invalid parameter for \
				grammar::renumber_code_symbols().");

	int index, index_limit, new_name_number;
	symbol* symbol1;

	// Scan over set_of_symbols putting the numeric part of the
	// names of symbols with type CONTEXT_SYMBOL into the first column of
	// names_array[]. Only numeric parts of names of the symbols
	// are listed.

	index = 0;
	char* symbol_name;
	list_for(symbol1, symbol, set_of_symbols)
	{
		if (symbol1->get_type() != symbol_type) continue;
		symbol_name = symbol1->get_name();

		if (index >= LARGE_SCRATCH_ARRAY_SIZE)
			abort_run("Overflow of names_array[] in \
				grammar::tidy_up_code_symbols()");

		if (symbol_type == CONTEXT_SYMBOL)
			names_array[index++].old_name_number_chars =
			symbol_name;
		else names_array[index++].old_name_number_chars =
			symbol_name + 1;
	}

	index_limit = index;

	// Scan over names_array[] putting in numbers for new names.

	new_name_number = 0;
	for (index = 0; index < index_limit; index++)
		names_array[index].new_name_number = ++new_name_number;

	// Now scan over symbols in list_of_patterns changing names
	// of symbol_type symbols.

	sequence* pattern1;
	char new_name[SMALL_SCRATCH_ARRAY_SIZE],
		* old_name_number_chars;
	int new_name_length;
	list_for(pattern1, sequence, list_of_patterns)
	{
		list_for(symbol1, symbol, pattern1)
		{
			if (symbol1->get_type() != symbol_type) continue;
			symbol_name = symbol1->get_name();

			// Find the entry in names_array[] where old_name_number_chars
			// matches the corresponding part of symbol_name.

			if (symbol_type == CONTEXT_SYMBOL)
			{
				old_name_number_chars = symbol_name;

				for (index = 0; index < index_limit; index++)
				{
					if (strcmp(names_array[index].old_name_number_chars,
						old_name_number_chars) == 0) break;
				}

				if (index >= index_limit)
					abort_run("Symbol name not found in \
						grammar::tidy_up_code_symbols()");

				new_name_length = sprintf(new_name, "%d",
					names_array[index].new_name_number);
			}
			else
			{
				old_name_number_chars = symbol_name + 1;

				for (index = 0; index < index_limit; index++)
				{
					if (strcmp(names_array[index].old_name_number_chars,
						old_name_number_chars) == 0) break;
				}

				if (index >= index_limit)
					abort_run("Symbol name not found in \
						grammar::tidy_up_code_symbols()");

				new_name_length =
					sprintf(new_name, "%c%d",
						'#',
						names_array[index].new_name_number);
			}

			if (new_name_length + 1 >= SMALL_SCRATCH_ARRAY_SIZE
				|| new_name_length < 0)
				abort_run("Error in name construction in \
						grammar::tidy_up_code_symbols()");

			symbol_name = NIL; // To avoid dangling pointer to 
				// deleted old name.

			symbol1->set_name(new_name);
		}
	}
} // grammar::renumber_code_symbols

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Adds a pattern to a grammar checking that there are no identical
**	patterns or patterns that contain the same symbols. The method
**	also checks that the pattern to be added does not have exactly
**	the same CONTENTS symbols as any pattern already in the grammar.
**	If it does, its IDENTIFICATION symbols (other than its
**	UNIQUE_ID_SYMBOL) are copied into the pattern already in the
**	grammar, avoiding duplicates.
**
** CALLING SEQUENCE:
**
**	bool grammar::add_pattern(sequence *pattern1)
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if pattern1 is added, false otherwise.
**
**	pattern1:		The pattern to be added.
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

bool grammar::add_pattern(sequence* pattern1)
{
	sequence* pattern2;

	// Check that pattern1 is not identical with any
	// pattern already in the grammar.

	list_for(pattern2, sequence, list_of_patterns)
		if (pattern2 == pattern1) return(false);

	// Now check to see whether pattern1 has all and only the
	// same CONTENTS symbols as any pattern already in the grammar.
	// If it does, it is not added to the grammar but its CONTENTS
	// symbols, other than its UNIQUE_ID_SYMBOL, are copied into
	// the pattern already in the grammar, avoiding duplicates.

	symbol* symbol1, * symbol2;
	int type1, type2;
	bool symbol_match_found;
	int id_number_array[MEDIUM_SCRATCH_ARRAY_SIZE],
		fe_id_number_array, i;
	list_for(pattern2, sequence, list_of_patterns)
	{
		if (pattern1->contents_symbols_match(pattern2) == false)
			continue;

		// A CONTENTS symbols match has been found.
		// Copy over non-duplicate IDENTIFICATION symbols.

		fe_id_number_array = 0;
		list_for(symbol1, symbol, pattern1)
		{
			type1 = symbol1->get_type();
			if (type1 == LEFT_BRACKET) continue;
			if (type1 == UNIQUE_ID_SYMBOL
				|| symbol1->get_status() == IDENTIFICATION)
				break;

			symbol_match_found = false;
			list_for(symbol2, symbol, pattern2)
			{
				type2 = symbol1->get_type();
				if (type2 == LEFT_BRACKET) continue;
				if (type2 == UNIQUE_ID_SYMBOL
					|| symbol2->get_status() == IDENTIFICATION)
					break;
				if (symbol1->name_matches(symbol2))
				{
					symbol_match_found = true;
					break;
				}
			}

			if (symbol_match_found == false)
			{
				id_number_array[fe_id_number_array] =
					atoi(symbol1->get_name());
				plus_one(&fe_id_number_array,
					MEDIUM_SCRATCH_ARRAY_SIZE,
					"Array overflow in grammar::add_pattern().");
			}
		}

		// Now add symbols to pattern2 with IDENTIFICATION symbol
		// names generated from id_number_array[].

		for (i = 0; i < fe_id_number_array; i++)
			pattern2->add_context_symbol(id_number_array[i]);

		return(false);
	}

	list_of_patterns->add_child(pattern1);

	return(true);

} // grammar::add_pattern

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	This function renames the IDENTIFICATION symbols in a grammar so
**	that numbering starts from 1. UNIQUE_ID_SYMBOLs have a different
**	sequence from ID symbols that are CONTEXT_SYMBOLS.
**	The bit_costs of symbols are not changed.
**
** CALLING SEQUENCE:
**
**	void grammar::tidy_up_code_symbols(int original_ID)
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	original_ID		Where a grammar has been copied, this is the ID
**					of the original grammar to be used in writing out.
**					If it is NULL_VALUE, the ID of the current grammar
**					is used.
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

void grammar::tidy_up_code_symbols(int original_ID)
{
	int alphabet_size;

	names_array = new struct names_entry[LARGE_SCRATCH_ARRAY_SIZE];

	set_of_symbols = list_of_patterns->
		compile_alphabet(&alphabet_size); // This compiles
// a list of symbols in list_of_patterns and sorts them
// in name order.

	renumber_code_symbols(CONTEXT_SYMBOL);
	renumber_code_symbols(UNIQUE_ID_SYMBOL);

	// Finally, print out list_of_patterns with new names of symbols.

	fprintf(output_file, "TIDIED UP GR");
	if (original_ID == NULL_VALUE)
		fprintf(output_file, "%d", this->get_ID());
	else fprintf(output_file, "%d", original_ID);
	fprintf(output_file, "\n\n");

	write_grammar(false, false);

	delete set_of_symbols;
	delete[] names_array;

} // grammar::tidy_up_code_symbols

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	This function, called from compile_alternative_grammars(),
**	adds Old patterns in a 'full' alignment to the patterns in
**	the grammar, checking that there are no duplicates in the grammar,
**	either the same pattern entered twice or two different patterns
**	that contain exactly the same symbols.
**
**	If include_code is true, the code pattern derived from the
**	alignment is included in the grammar.
**
** CALLING SEQUENCE:
**
**	void grammar::compile_grammar(sequence *full_alignment)
**
** FORMAL ARGUMENTS:
**
**	Return value:		void
**
**	full_alignment:		The 'full' alignment whose patterns are to
**						be added to the grammar (without duplicates).
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

void grammar::compile_grammar(sequence* full_alignment)
{
	int row, sequence_depth = full_alignment->get_sequence_depth();
	sequence* row_pattern; //, *copy_pattern ;

	for (row = 1; row < sequence_depth; row++)
	{
		row_pattern = full_alignment->get_row_pattern(row);
		// copy_pattern = new sequence(*row_pattern) ;
		add_pattern(row_pattern);
	}

	// Diagnostic

#if DIAGNOSTIC4
	fprintf(output_file, "DIAGNOSTIC:\n\n");
	write_grammar(true);
	fprintf(output_file, "END DIAGNOSTIC\n\n");
	fflush(output_file);
#endif

} // grammar::compile_grammar

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Writes a recursive trace of how a compilation of grammars
**	was formed.
**
** CALLING SEQUENCE:
**
**	void grammar::write_trace(int indentation)
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	indentation:	The distance from the left margin to start.
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

void grammar::write_trace(int indentation)
{
	int i;
	grammar* child;

	for (i = 0; i < indentation; i++)
		fprintf(output_file, "  ");

	if (indentation == 0)
		fprintf(output_file, "Root\n");
	else fprintf(output_file, "%s%d%s%d%s%3.2f%s%3.2f%s%3.2f%s",
		"GR",
		ID,
		" (",
		derived_from_grammar,
		"), G = ",
		G,
		", E = ",
		E,
		", score = ",
		score,
		"\n");

	if (basis_for != NIL)
	{
		list_for(child, grammar, basis_for)
			child->write_trace(indentation + 1);
	}

} // grammar::write_trace

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Writes out the patterns in the grammar with scores etc.
**
** CALLING SEQUENCE:
**
**	void grammar::write_grammar(bool write_derived_from,
**		bool write_details)
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	write_derived_from:		If true, 'derived_from' field is written,
**							otherwise it is not.
**	write_details:			The details of the grammar patterns are
**							are written if true, not otherwise.
**
** IMPLICIT INPUTS:
**
**	NONE
**
** IMPLICIT OUTPUTS:
**
**	Output to output_file
**
** SIDE EFFECTS:
**
**	NONE
**
*/

void grammar::write_grammar(bool write_derived_from,
	bool write_details)
{
	sequence* pattern1;

	fprintf(output_file, "%s%d%s",
		"GRAMMAR GR",
		ID,
		" (");
	print_pattern_cycle(false, current_new_pattern);

	if (write_details)
		fprintf(output_file, ") WITH DETAILS\n");
	else fprintf(output_file, ")\n");

	if (write_derived_from)
	{
		fprintf(output_file, "derived from grammar ");

		if (derived_from_grammar == NULL_VALUE)
			fprintf(output_file, "NULL");
		else fprintf(output_file, "%s%d",
			"GR",
			derived_from_grammar);

		fprintf(output_file, " and alignment:\n");
		if (derived_from_alignment == NIL)
			fprintf(output_file, "NIL\n");
		else derived_from_alignment->write_pattern(true, false);
		fprintf(output_file, "G = ");
	}
	else fprintf(output_file, "G = ");

	fprintf(output_file, "%3.2f%s%3.2f%s%3.2f%s",
		G,
		", E = ",
		E,
		", score = ",
		score,
		".\n\nGrammar patterns:\n\n");

	list_for(pattern1, sequence, list_of_patterns)
	{
		if (write_details)
			pattern1->write_with_details(true);
		else pattern1->write_pattern(true, true);
	}
} // grammar::write_grammar

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Makes a copy of a grammar. Notice that the list_of_patterns is
**	copied, but the individual patterns within the list are *not*
**	copied.
**
** CALLING SEQUENCE:
**
**	grammar::grammar(grammar &gr) : base_object(gr)
**
** FORMAL ARGUMENTS:
**
**	Return value:	A new grammar.
**
**	gr:				The grammar to be copied.
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

grammar::grammar(grammar& gr) : base_object(gr)
{
	derived_from_grammar = gr.get_derived_from_grammar();
	derived_from_alignment = gr.get_derived_from_alignment();
	G = gr.get_G();
	E = gr.get_E();
	score = gr.get_score();
	ID = grammar_ID_number++;
	basis_for = NIL;
	list_of_patterns = new group;

	sequence* pattern1;
	gr.initialise();
	while (pattern1 = gr.get_next_pattern())
		list_of_patterns->add_child(pattern1);

} // grammar::grammar(grammar &gr)

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Copies the details of one grammar into another and makes literal
**	copies of the patterns in the one grammar for inclusion in the
**	other.
**
** CALLING SEQUENCE:
**
**	void grammar::copy_details_and_patterns(grammar *gr)
**
** FORMAL ARGUMENTS:
**
**	Return value:	void.
**
**	gr:				The grammar whose details and patterns are to
**					be copied.
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

void grammar::copy_details_and_patterns(grammar* gr)
{
	derived_from_grammar = gr->get_derived_from_grammar();
	derived_from_alignment = gr->get_derived_from_alignment();
	G = gr->get_G();
	E = gr->get_E();
	score = gr->get_score();
	basis_for = NIL;

	sequence* pattern1, * pattern2;
	gr->initialise();
	while (pattern1 = gr->get_next_pattern())
	{
		pattern2 = new sequence(*pattern1);
		list_of_patterns->add_child(pattern2);
	}

} // grammar::copy_details_and_patterns

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Computes G a grammar.
**
** CALLING SEQUENCE:
**
**	void grammar::compute_G()
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
**	Value for G for the grammar.
**
** SIDE EFFECTS:
**
**	NONE
**
*/

void grammar::compute_G()
{
	sequence* pattern1;
	double cumulative_G = 0;

	list_for(pattern1, sequence, list_of_patterns)
		cumulative_G += pattern1->get_total_cost();

	G = cumulative_G;
} // grammar::compute_G

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Creates an alignment from a list of basic alignments.
**
** CALLING SEQUENCE:
**
**	sequence *combination::make_composite_alignment(sequence *cnp)
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	cnp:			The 'current' New pattern
**
** IMPLICIT INPUTS:
**
**	NONE
**
** IMPLICIT OUTPUTS:
**
**	An encoding of the compposite alignment is added to
**	current_code_patterns.
**
** SIDE EFFECTS:
**
**	NONE
**
*/

sequence* combination::make_composite_alignment(sequence* cnp)
{
	sequence* sub_alignment,
		* composite_alignment = new sequence(COMPOSITE_ALIGNMENT);
	int overall_depth = 0, ba_depth, new_row1, new_row2 = 0, old_row;
	symbol* sub_alignment_symbol, * newly_made_column,
		* symbol1;
	alignment_element* al_el1, * al_el2;
	char* symbol_name;

	composite_alignment->set_composite_alignment(true);

	// Calculate depth of new composite alignment as
	// D = SUM (d_i - 1) + 1, summing over the set of basic alignments
	// and with d representing the depth of each individual alignment.
	// The reasoning is that row 0 is the same for all alignments so
	// we don't need to include it in the depth of each alignment, but
	// we need to add in row 0.

	list_for(sub_alignment, sequence, sub_alignment_list)
	{
		ba_depth = sub_alignment->get_sequence_depth() - 1;
		overall_depth += ba_depth;
	}

	overall_depth += 1; // Add 1 for the New pattern in row 0.

	composite_alignment->set_sequence_depth(overall_depth);

	// Now copy the basic alignments into a new composite alignment
	// with each symbol having a depth equal to overall_depth and
	// with each row of each basic alignment given a unique row
	// in the composite alignment.

	symbol* first_symbol, * last_symbol;

	new_row1 = 1; // Row 0 is the same in the original symbol and
		// the new symbol. So the first row in the new symbol
		// for purposes of copying is row 1.

	list_for(sub_alignment, sequence, sub_alignment_list)
	{
		ba_depth = sub_alignment->get_sequence_depth();

		first_symbol = (symbol*)sub_alignment->get_first_child();
		last_symbol = (symbol*)sub_alignment->get_last_child();

		list_for(sub_alignment_symbol, symbol, sub_alignment)
		{
			new_row2 = new_row1;

			symbol_name = sub_alignment_symbol->get_name();

			newly_made_column = new symbol(symbol_name, overall_depth,
				NULL_VALUE);
			newly_made_column->set_bit_cost(sub_alignment_symbol->
				get_bit_cost());
			newly_made_column->set_is_a_hit(sub_alignment_symbol->
				is_a_hit());
			newly_made_column->set_status(sub_alignment_symbol->
				get_status());
			newly_made_column->set_type(sub_alignment_symbol->
				get_type());

			// Now set values for row 0.

			al_el1 = sub_alignment_symbol->get_al_el(0);
			al_el2 = newly_made_column->get_al_el(0);
			al_el2->set_el_obj(al_el1->get_el_obj());
			al_el2->set_original_pattern(al_el1->
				get_original_pattern());

			// Set values for other rows.

			for (old_row = 1; old_row < ba_depth; old_row++)
			{
				if (new_row2 >= overall_depth)
					abort_run("Anomaly in value of new_row \
						in make_composite_alignment()");
				al_el1 = sub_alignment_symbol->get_al_el(old_row);
				al_el2 = newly_made_column->get_al_el(new_row2);
				al_el2->set_el_obj(al_el1->get_el_obj());
				al_el2->set_original_pattern(al_el1->
					get_original_pattern());
				new_row2++;
			}

			// Set values for same_column_above,
			// same_column_below and symbol_is_a_hit.

			newly_made_column->set_symbol_matches();

			composite_alignment->add_child(newly_made_column);
		} // End of one basic alignment
		new_row1 = new_row2;
	} // End of sub_alignment_list

	composite_alignment->mark_parent_and_int_positions_non_recursive();

	// Fill in values for original_pattern in every row of every
	// column. First, find the first column that has a non-NIL
	// value for original_pattern in this row.

	sequence* original_pattern = NULL;
	for (new_row1 = 0; new_row1 < overall_depth; new_row1++)
	{
		// Find the original_pattern for this row.

		for (symbol1 = (symbol*)composite_alignment->get_first_child();
			symbol1 != NIL; symbol1 = (symbol*)
			composite_alignment->get_next_child())
		{
			original_pattern = symbol1->get_row_pattern(new_row1);
			if (original_pattern != NIL) break;
		}

		// Now fill it in right across the alignment.

		for (symbol1 = (symbol*)composite_alignment->get_first_child();
			symbol1 != NIL; symbol1 = (symbol*)
			composite_alignment->get_next_child())
		{
			al_el1 = symbol1->get_al_el(new_row1);
			al_el1->set_original_pattern(original_pattern);
		}
	}

	// Calculate values for NSC, EC, CD, CR and abs_P for
	// the alignment.

	composite_alignment->make_code(false);

	composite_alignment->set_symbol_matches();

	// Print new composite alignment as alignment and as
	// a flat pattern, with and without details.

	composite_alignment->write_out_fully("COMPOSITE ALIGNMENT",
		NIL, write_section_chars_length, NULL_VALUE, true,
		cnp);

	if (show_al_structure)
		composite_alignment->show_al_structure(NIL);

	return(composite_alignment);

} // combination::make_composite_alignment

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Tests whether the list of basic alignments in 'this' is a subset
**	of the list of basic alignments in test_comb.
**
** CALLING SEQUENCE:
**
**		bool combination::is_subset_of(combination *test_comb)
**
** FORMAL ARGUMENTS:
**
**		Return value:	true if the basic alignments in 'this' are a subset
**						of the basic alignments in test_comb, false otherwise.
**
**		test_comb:		The combination against which 'this' is to be tested.
**
** IMPLICIT INPUTS:
**
**		NONE
**
** IMPLICIT OUTPUTS:
**
**		NONE
**
** SIDE EFFECTS:
**
**		NONE
**
*/

bool combination::is_subset_of(combination* test_comb)
{
	group* test_sub_alignment_list =
		test_comb->get_sub_alignment_list();
	sequence* ba_this, * ba_test;

	// 'this' combination must not be the same as test_comb.

	if (this == test_comb)
		abort_run("Invalid comparison in combination::is_subset_of().");

	list_for(ba_this, sequence, sub_alignment_list)
	{
		list_for(ba_test, sequence, test_sub_alignment_list)
		{
			if (ba_this == ba_test) break;
		}
		if (ba_test == NIL) return(false); // An instance of ba_this
					// has been found which is not present on
					// test_sub_alignment_list. Therefore,
					// sub_alignment_list is *not* a subset of
					// test_sub_alignment_list.
	}
	return(true); // Every instance of ba_this is found on
				// test_sub_alignment_list. Therefore,
				// sub_alignment_list *is* a subset of
				// test_sub_alignment_list.
} // combination::is_subset_of

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Prints the score and the list of basic alignments in a combination.
**
** CALLING SEQUENCE:
**
**	void combination::print_combination()
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
**	Output to file
**
** SIDE EFFECTS:
**
**	NONE
**
*/

void combination::print_combination()
{
	sequence* sub_alignment;

	fprintf(output_file, "%s%d%s%1.2f%s",
		"Combination C_ID",
		C_ID,
		" (score ",
		combination_score,
		") sub-alignments:\n\n");

	bool first_in_list = true;
	list_for(sub_alignment, sequence, sub_alignment_list)
	{
		if (first_in_list)
		{
			fprintf(output_file, "%s%d",
				"ID",
				sub_alignment->get_ID());
			first_in_list = false;
		}
		else
		{
			fprintf(output_file, "%s%d",
				", ID",
				sub_alignment->get_ID());
		}
	}

	fprintf(output_file, "\n\n");
} // combination::print_sub_alignments

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Copy constructor for class combination.
**
** CALLING SEQUENCE:
**
**	combination::combination(combination &comb1) : base_object(comb1)
**
** FORMAL ARGUMENTS:
**
**	Return value:	Creates a copy of the input combination
**
**	comb1:			The combination to be copied.
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

combination::combination(combination& comb1) : base_object(comb1)
{
	sequence* sub_alignment;
	int i;

	C_ID = combination_ID_number++;

	combination_score = comb1.combination_score;

	len_fit_seq = comb1.len_fit_seq;

	if (len_fit_seq >= MEDIUM_SCRATCH_ARRAY_SIZE)
		abort_run("len_fit_seq too long in combination");
	for (i = 0; i < len_fit_seq; i++)
		fitting_sequence[i] = comb1.fitting_sequence[i];

	group* temp_list = comb1.sub_alignment_list;
	sub_alignment_list = new group;
	for (sub_alignment = (sequence*)temp_list->get_first_child();
		sub_alignment != NIL; sub_alignment = (sequence*)
		temp_list->get_next_child())
	{
		sub_alignment_list->add_child(sub_alignment);
	}
} // combination::combination(combination &comb1)

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	This method in class combination tests whether or not a given basic
**	alignment 'fits' into the combination. A basic combination fits if
**	all the symbols from New that it covers have not already been marked
**	as true in the fitting _sequence of 'this'.
**
** CALLING SEQUENCE:
**
**	bool combination::can_accept(int pos_new_first,
**		int pos_new_last)
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if sub_alignment fits into
**					fitting _sequence, otherwise false.
**
**	pos_new_first:	The integer position of the first hit
**					symbol from current_new_pattern in bal1.
**	pos_new_last:	The integer position of the last hit
**					symbol from current_new_pattern in bal1.
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

bool combination::can_accept(int pos_new_first,
	int pos_new_last)
{
	for (int i = pos_new_first; i <= pos_new_last; i++)
		if (fitting_sequence[i] == true) return(false);

	return(true);
} // combination::can_accept

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Adds a 'basic' alignment to a combination.
**
** CALLING SEQUENCE:
**
**	void combination::add_sub_alignment(sequence *bal1,
**		int bal1_pos_new_first, int bal1_pos_new_last,
**		sequence *cnp)
**
** FORMAL ARGUMENTS:
**
**	Return value:		void
**
**	bal1:				The sub_alignment to be added to 'this'.
**	bal1_pos_new_first:	The integer position of the first hit
**						symbol from current_new_pattern in bal1.
**	bal1_pos_new_last:	The integer position of the last hit
**						symbol from current_new_pattern in bal1.
**	cnp:				The New pattern being processed.
**
** IMPLICIT INPUTS:
**
**	NONE
**
** IMPLICIT OUTPUTS:
**
**	Addition to the structure of 'this'. The 'coverage' of the
**	current pattern from New by the newly-added basic alignment
**	is marked in fitting_sequence.
**
** SIDE EFFECTS:
**
**	NONE
**
*/

void combination::add_sub_alignment(sequence* bal1,
	int bal1_pos_new_first, int bal1_pos_new_last,
	sequence* cnp)
{
	sequence* bal2;
	symbol* symbol1, * symbol_new;
	int bal2_pos_new_first;

	// Add sub_alignment to sub_alignment_list in the
	// position in the list that corresponds to the order
	// of the alignments relative to the current pattern from New.
	// This is done to facilitate the building of composite alignments
	// in make_composite_alignment.

	list_for(bal2, sequence, sub_alignment_list)
	{
		// Find the first symbol in bal2 that contains a symbol
		// from current_new_pattern and find the position of
		// that symbol in current_new_pattern.

		list_for(symbol1, symbol, bal2)
		{
			symbol_new = symbol1->get_row_symbol(0);
			if (symbol_new != NIL) break;
		}

		if (symbol1 == NIL) abort_run("Error in add_sub_alignment()");

		bal2_pos_new_first = symbol1->get_row_orig_int_pos(0);

		// Compare this with the corresponding integer position
		// of the first hit symbol from current_new_pattern
		// in sub_alignment.

		if (bal1_pos_new_first < bal2_pos_new_first) break;
	}

	if (bal2 == NIL) sub_alignment_list->add_child(bal1);
	else sub_alignment_list->precede(bal1, bal2);

	combination_score += bal1->get_compression_difference();

	// Now mark the entries in 'fitting_sequence' that are
	// 'covered' by the newly-added basic alignment.

	for (int i = bal1_pos_new_first; i <= bal1_pos_new_last; i++)
		fitting_sequence[i] = true;

#if DIAGNOSTIC5

	fprintf(output_file, "%s%d%s",
		"Combination C_ID",
		C_ID,
		" is derived from alignments: ");

	sub_alignment_list->write_IDs(NULL_VALUE, cnp);

#endif

} // combination::add_sub_alignment

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Checks to see whether a given 'reference' class symbol (with
**	status CONTENTS) occurs once or more than once as an IDENTIFICATION
**	symbol in the same grammar. If a single pattern is found that contains
**	the given class symbol as an IDENTIFICATION symbol, then the function
**	returns that pattern. If more than one pattern is found, the function
**	returns NIL.
**
** CALLING SEQUENCE:
**
**	sequence *single_reference(symbol *ref1)
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

sequence* single_reference(symbol* ref1, grammar* gr1)
{
	sequence* pattern1;
	sequence* single_referent = NULL;
	symbol* symbol1;
	int ref_counter = 0, id_counter = 0;
	list_element* el_pos1, * el_pos2;
	group* list_of_patterns = gr1->list_of_patterns;

	// Check that the ref1 symbol type occurs only once as
	// a CONTENTS symbol and only once as an IDENTIFICATION symbol.

	el_pos1 = NIL;
	list_for_el_pos(pattern1, sequence, list_of_patterns, el_pos1)
	{
		if (pattern1->get_origin() == CODE_PATTERN) continue;
		el_pos2 = NIL;
		list_for_el_pos(symbol1, symbol, pattern1, el_pos2)
		{
			if (symbol1->name_matches(ref1))
			{
				if (symbol1->get_status() == CONTENTS)
					ref_counter++;
				else
				{
					id_counter++;
					single_referent = pattern1;
				}
			}
		}
	}

	if (ref_counter == 1 && id_counter == 1)
		return(single_referent);
	else return(NIL);
} // single_reference

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Examines a grammar to find patterns that are referenced from one
**	context and only one context and where there is no other pattern
**	that is referenced from that context. Any such pattern is merged
**	with the context pattern in a newly-created pattern that
**	replaces the context pattern and the contained pattern as
**	they appear in the grammar. Corresponding adjustments are made
**	in the CODE_PATTERNs.
**
**	This kind of merging can mean that other patterns fall into the
**	same category. So the process needs to be repeated to check for
**	that kind of possibility - and so on until no more merging can be
**	done.
**
** CALLING SEQUENCE:
**
**	void grammar::merge_patterns()
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

void grammar::merge_patterns()
{
	sequence* pattern1, * new_pattern, * single_ref;
	symbol* symbol1, * symbol2, * symbol3, * symbol4,
		* left_bracket = NULL, * right_bracket = NULL;
	list_element* el_pos1, * el_pos2, * el_pos3;
	bool done = false, patterns_have_been_merged = false;
	int fr_single_ref, fr_pattern1, lower_frequency;

	while (done == false)
	{
		done = true;
		el_pos1 = NIL;
		list_for_el_pos(pattern1, sequence, list_of_patterns, el_pos1)
		{
			if (pattern1->get_origin() == CODE_PATTERN) continue;
			el_pos2 = NIL;
			list_for_el_pos(symbol1, symbol, pattern1, el_pos2)
			{
				if (symbol1->get_status() != CONTENTS) continue;
				if (symbol1->get_type() != CONTEXT_SYMBOL) continue;
				single_ref = single_reference(symbol1, this);
				if (single_ref == NIL) continue;

				// Merge single_ref with pattern1 (in a new copy).

				new_pattern = new sequence(*pattern1);

				// Find the 'reference' to single_ref, copy in the
				// CONTENTS symbols from single_ref and
				// extract the 'reference' symbols.

				list_for(symbol2, symbol, new_pattern)
				{
					if (symbol2->get_type() == LEFT_BRACKET)
						left_bracket = symbol2;
					if (symbol2->name_matches(symbol1))
					{
						right_bracket =
							(symbol*)new_pattern->get_next_child();
						if (right_bracket->get_type() != RIGHT_BRACKET)
							abort_run("Anomaly in \
								grammar::merge_patterns().");
						break;
					}
				}

				if (symbol2 == NIL)
					abort_run("Anomaly in grammar::merge_patterns().");

				// Copy the CONTENTS symbols of single_ref into new_pattern
				// immediately before left_bracket.

				list_for(symbol3, symbol, single_ref)
				{
					if (symbol3->get_status() != CONTENTS) continue;
					symbol4 = new symbol(*symbol3);
					new_pattern->precede(symbol4, left_bracket);
				}

				// Delete the 'reference' symbols in new_pattern.

				new_pattern->delete_child(left_bracket);
				new_pattern->delete_child(symbol2);
				new_pattern->delete_child(right_bracket);

				new_pattern->
					mark_parent_and_int_positions_non_recursive();

				// Set the frequency of new_pattern to be the lower
				// of single_ref and pattern1.

				fr_single_ref = single_ref->get_frequency();
				fr_pattern1 = pattern1->get_frequency();
				lower_frequency = (fr_pattern1 < fr_single_ref)
					? fr_pattern1 : fr_single_ref;
				new_pattern->set_frequency(lower_frequency);

				list_of_patterns->delete_child(pattern1);
				list_of_patterns->delete_child(single_ref);
				list_of_patterns->add_child(new_pattern);

				patterns_have_been_merged = true;
				done = false;
				goto L1;
			}
		}
	L1:;
	}

	if (patterns_have_been_merged)
	{
		// Examine the CODE_PATTERNs and remove all
		// CLASS_SYMBOLs that do not appear anywhere in
		// the main grammar.

		// First, compile a list of the CLASS_SYMBOLs in
		// the CODE_PATTERNs.

		group* class_symbols_in_code_patterns = new group;
		bool class_symbol_found;

		el_pos3 = NIL;
		list_for_el_pos(pattern1, sequence, list_of_patterns, el_pos3)
		{
			if (pattern1->get_origin() != CODE_PATTERN) continue;
			list_for(symbol1, symbol, pattern1)
			{
				if (symbol1->get_type() != CONTEXT_SYMBOL) continue;
				if (class_symbols_in_code_patterns->contains_copy_of(symbol1) == NIL)
				{
					symbol2 = new symbol(*symbol1);
					class_symbols_in_code_patterns->add_child(symbol2);
				}
			}
		}

		// Now look at the symbols in class_symbols_in_code_patterns
		// to find any that do not have copies in the patterns of
		// the main grammar. Any such symbols are removed from the
		// code patterns.

		list_for(symbol1, symbol, class_symbols_in_code_patterns)
		{
			el_pos1 = NIL;
			class_symbol_found = false;
			list_for_el_pos(pattern1, sequence, list_of_patterns, el_pos1)
			{
				if (pattern1->get_origin() == CODE_PATTERN) continue;

				// pattern1 is in the main grammar.

				el_pos2 = NIL;
				list_for_el_pos(symbol2, symbol, pattern1, el_pos2)
				{
					if (symbol1->name_matches(symbol2))
					{
						class_symbol_found = true;
						goto L2;
					}
				}
			}
		L2: if (class_symbol_found) continue;

			// Delete copies of symbol1 wherever they
			// occur in the CODE_PATTERNs.

			el_pos3 = NIL;
			list_for_el_pos(pattern1, sequence, list_of_patterns, el_pos3)
			{
				if (pattern1->get_origin() != CODE_PATTERN) continue;
				while (symbol2 = pattern1->contains_copy_of(symbol1))
					pattern1->delete_child(symbol2);
			}
		}

		class_symbols_in_code_patterns->delete_children();
		delete class_symbols_in_code_patterns;

		initialise();
		while (pattern1 = get_next_pattern())
			pattern1->compute_costs();

		compute_G();
		sum_G_and_E();

		// Write out the grammar in its new form.

		fprintf(output_file, "%s%d%s",
			"GRAMMAR GR",
			this->get_ID(),
			" AFTER MERGING OF PATTERNS\n\n");

		write_grammar(false, false);
	}
	else fprintf(output_file, "%s%d%s",
		"No patterns to be merged in grammar GR",
		this->get_ID(),
		"\n\n");

} // grammar::merge_patterns

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Determines whether or not a given class symbol (from
**	a CODE_PATTERN) is used as an ID symbol in the main grammar.
**
** CALLING SEQUENCE:
**
**	bool grammar::is_ID_symbol(symbol *s1)
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if s1 is an ID symbol within the grammar, false
**					otherwise.
**
**	s1:				The class symbol to be tested.
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

bool grammar::is_ID_symbol(symbol* s1)
{
	sequence* pattern1;
	symbol* symbol1;
	list_element* el_pos1, * el_pos2;

	el_pos1 = NIL;
	list_for_el_pos(pattern1, sequence, list_of_patterns, el_pos1)
	{
		if (pattern1->get_origin() == CODE_PATTERN) continue;
		el_pos2 = NIL;
		list_for_el_pos(symbol1, symbol, pattern1, el_pos2)
		{
			if (symbol1->get_type() == LEFT_BRACKET) continue;
			if (symbol1->get_status() == CONTENTS) break;
			if (symbol1->name_matches(s1)) return(true);
		}
	}

	return(false);
} // grammar::is_ID_symbol

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Determines whether or not a given class symbol is referenced within
**	the grammar patterns.
**
** CALLING SEQUENCE:
**
**	bool grammar::is_referenced(symbol *s1)
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if s1 is referenced within the grammar, false
**					otherwise.
**
**	s1:				The class symbol to be tested.
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

bool grammar::is_referenced(symbol* s1)
{
	sequence* pattern1;
	symbol* symbol1;
	list_element* el_pos1, * el_pos2;

	el_pos1 = NIL;
	list_for_el_pos(pattern1, sequence, list_of_patterns, el_pos1)
	{
		el_pos2 = NIL;
		list_for_el_pos(symbol1, symbol, pattern1, el_pos2)
		{
			if (symbol1->get_type() == LEFT_BRACKET) continue;
			if (symbol1->get_status() != CONTENTS) continue;
			if (symbol1->name_matches(s1)) return(true);
		}
	}

	return(false);
} // grammar::is_referenced

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	This method checks each pattern in the grammar to ensure that
**	it has enough CONTEXT_SYMBOLs to ensure that it can be
**	identified uniquely in the grammar ***but not more than that***.
**	If too many context symbols are found, the excess are removed
**	provided that they are not used as 'references' in any of the
**	patterns in the grammar. Each pattern retains its
**	UNIQUE_ID_SYMBOL.
**
** CALLING SEQUENCE:
**
**	void grammar::clean_up(int original_ID)
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	original_ID		Where a grammar has been copied, this is the ID
**					of the original grammar to be used in writing out.
**					If it is NULL_VALUE, the ID of the current grammar
**					is used.
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

void grammar::clean_up(int original_ID)
{
	sequence* pattern1;
	symbol* symbol1;
	list_element* el_pos1, * el_pos2;
	symbol* scratch_array[MEDIUM_SCRATCH_ARRAY_SIZE];
	int fe_scratch_array, i, number_of_CLASS_ID_symbols;

	el_pos1 = NIL;
	list_for_el_pos(pattern1, sequence, list_of_patterns, el_pos1)
	{
		fe_scratch_array = 0;
		el_pos2 = NIL;
		list_for_el_pos(symbol1, symbol, pattern1, el_pos2)
		{
			if (symbol1->get_type() != CONTEXT_SYMBOL) continue;
			if (symbol1->get_status() != IDENTIFICATION) break;

			// symbol1 is an IDENTIFICATION CONTEXT_SYMBOL.

			if (is_referenced(symbol1)) continue; // This symbol
				// is referenced from within the grammar.

			// Make an entry in scratch_array[].

			scratch_array[fe_scratch_array] = symbol1;
			plus_one(&fe_scratch_array, MEDIUM_SCRATCH_ARRAY_SIZE,
				"Array overflow in grammar::clean_up().");
		}

		// Now delete all the non-referenced IDENTIFICATION CLASS_SYMBOLs 
		// making sure that the pattern contains at least one
		// IDENTIFICATION CONTEXT_SYMBOL.

		for (i = 0; i < fe_scratch_array; i++)
		{
			number_of_CLASS_ID_symbols =
				pattern1->find_number_of_CLASS_ID_symbols();
			if (number_of_CLASS_ID_symbols > 1)
				pattern1->delete_child(scratch_array[i]);
			else break;
		}
	}

	initialise();
	while (pattern1 = get_next_pattern())
		pattern1->compute_costs();

	compute_G();
	sum_G_and_E();

	// Now write out the grammar in its cleaned-up form.

	fprintf(output_file, "CLEANED UP GRAMMAR GR");
	if (original_ID == NULL_VALUE)
		fprintf(output_file, "%d", this->get_ID());
	else fprintf(output_file, "%d", original_ID);
	fprintf(output_file, "\n\n");

	write_grammar(false, false);

} // grammar::clean_up

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Gets the integer position of the original pattern associated
**	with this alignment element.
**
** CALLING SEQUENCE:
**
**	int alignment_element::get_orig_patt_int_pos()
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

int alignment_element::get_orig_patt_int_pos()
{
	symbol* s1 = (symbol*)get_el_obj();
	if (s1 == NIL) return(NULL_VALUE);
	else return(original_pattern->find_int_pos_by_child(s1));
} // alignment_element::get_orig_patt_int_pos

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	For diagnostic purposes.
**
** CALLING SEQUENCE:
**
**	void alignment_element::show_al_structure()
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
**	Printed output
**
** SIDE EFFECTS:
**
**	NONE
**
*/

void alignment_element::show_al_structure()
{
	symbol* symbol1;

	if (el_obj != NIL)
	{
		fprintf(output_file, "Symbol ");
		symbol1 = (symbol*)el_obj;

		fprintf(output_file, "%s", symbol1->get_name());
	}
	else fprintf(output_file, "No symbol");
	fprintf(output_file, "%s%d%s%d%s",
		", same_column_above ", same_column_above,
		", same_column_below ", same_column_below,
		", original_pattern ");
	if (original_pattern != NIL)
		original_pattern->print_ID();
	else fprintf(output_file, "NIL");
	fprintf(output_file, "%s%d%s",
		",\norig_patt_int_pos ", get_orig_patt_int_pos(), "\n");
} // alignment_element::show_al_structure

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	From information in symbol_types_in_old this method assigns
**	a frequency value to 'this' symbol and a bit_cost.
**
** CALLING SEQUENCE:
**
**	void symbol::assign_frequency_and_cost()
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
** IMPLICIT INPUTS:
**
**	symbol_types_in_old
**
** IMPLICIT OUTPUTS:
**
**	NONE
**
** SIDE EFFECTS:
**
**	NONE
*/

void symbol::assign_frequency_and_cost()
{
	symbol* symbol_type;

	list_for(symbol_type, symbol, symbol_types_in_old)
	{
		if (strcmp(symbol_type->get_name(), this->get_name()) != NIL)
			continue;
		set_frequency(symbol_type->get_frequency());
		set_bit_cost(symbol_type->get_bit_cost());
		return;
	}

	// No match was found for 'this' symbol in symbol_types_in_old
	// so it must be given arbitrary values for frequency
	// and bit_cost.

	set_frequency(1);
	if (type == LEFT_BRACKET || type == RIGHT_BRACKET)
		set_bit_cost(BRACKET_BIT_COST);
	else if (type == CONTEXT_SYMBOL || type == UNIQUE_ID_SYMBOL)
		set_bit_cost(average_CONTEXT_SYMBOL_type_cost);
	else set_bit_cost(average_symbol_type_cost);
} // symbol::assign_frequency_and_cost

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Deletion of a symbol
**
** CALLING SEQUENCE:
**
**	symbol::~symbol()
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

symbol::~symbol()
{
	delete[] name;
	delete[] al_el_array; // Notice
		// that deletion of al_el_array does *not* delete
		// any el_obj which may be attached to one or
		// alignment elements in the al_el_array.
} // symbol::~symbol

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Constructor for class symbol.
**
** CALLING SEQUENCE:
**
**	symbol::symbol(char *nm, int al_depth, int hn_ID) : base_object()
**
** FORMAL ARGUMENTS:
**
**	Return value:
**
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

symbol::symbol(const char* nm, int al_depth, int hn_ID) : base_object()
{
	name = NIL;
	if (nm == NIL) { type = NULL_VALUE; }
	else set_name(nm);
	bit_cost = NULL_VALUE;
	status = NULL_VALUE;
	type = NULL_VALUE;
	sequence_depth = al_depth;
	h_node_ID = hn_ID;
	symbol_is_a_hit = false;
	frequency = 1;
	if (al_depth == NULL_VALUE || al_depth == 1) al_el_array = NIL;
	else
	{
		al_el_array = new alignment_element[al_depth];
		for (int i = 0; i < al_depth; i++)
			al_el_array[i].set_el_obj(NIL);
	}
} // symbol::symbol

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Makes a copy of a symbol. If the symbol contains 2 or more rows,
**	it contains a new al_el_array[] but all the pointers within this
**	array are to the original structures pointed to from the symbol
**	which is being copied. Those structures are not themselves copied.
**
** CALLING SEQUENCE:
**
**	symbol::symbol(symbol &s) : base_object(s)
**
** FORMAL ARGUMENTS:
**
**	Return value:	NONE
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

symbol::symbol(symbol& s) : base_object(s)
{
	// The value of el_obj for the copy points to the start of
	// a new array of al_els.

	int row1;

	name = NIL;
	name = new char[strlen(s.get_name()) + 1];
	strcpy(name, s.get_name());
	bit_cost = s.get_bit_cost();
	type = s.get_type();
	status = s.get_status();
	sequence_depth = s.get_sequence_depth();
	h_node_ID = s.get_h_node_ID();
	symbol_is_a_hit = s.is_a_hit();
	frequency = s.get_frequency();

	if (sequence_depth != NULL_VALUE && sequence_depth != 1)
	{
		// Patterns and symbols with a depth of 1
		// do not have any al_el_array[].

		al_el_array = new alignment_element[sequence_depth];

		for (row1 = 0; row1 < sequence_depth; row1++)
			*(al_el_array + row1) = *((s.al_el_array) + row1);
	}
	else al_el_array = NIL;
} // symbol::symbol(&s)

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Checks down the column of an alignment to see whether there
**	are any symbols in the column that have the status CONTENTS. If
**	there is one or more such symbols, the method returns true,
**	otherwise it returns false.
**
** CALLING SEQUENCE:
**
**	bool symbol::contains_contents_symbol()
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if the column contains a CONTENTS symbol,
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

bool symbol::contains_contents_symbol()
{
	symbol* symbol1;
	int i;

	for (i = 0; i < sequence_depth; i++)
	{
		symbol1 = get_row_symbol(i);
		if (symbol1 != NIL)
		{
			if (symbol1->get_status() == CONTENTS) return(true);
		}
	}

	return(false);

} // symbol::contains_contents_symbol

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Makes a copy of 'this' but without any information about rows. It
**	assumes a depth of 1.
**
** CALLING SEQUENCE:
**
**	symbol *symbol::shallow_copy()
**
** FORMAL ARGUMENTS:
**
**	Return value:
**
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

symbol* symbol::shallow_copy()
{
	symbol* copy_symbol = new symbol(this->get_name(), 1, NULL_VALUE);

	copy_symbol->set_bit_cost(this->get_bit_cost());
	copy_symbol->set_type(this->get_type());
	copy_symbol->set_status(this->get_status());
	copy_symbol->set_sequence_depth(1);
	copy_symbol->set_h_node_ID(this->get_h_node_ID());
	copy_symbol->set_is_a_hit(this->is_a_hit());
	copy_symbol->set_al_el_array(NIL);

	return(copy_symbol);

} // symbol::shallow_copy

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	For a column within an alignment, puts in the correct values in
**	for same_column_above and same_column_below. The method also
**	sets symbol_is_a_hit as false if the column contains one symbol
**	and sets it as true if the column contains more than one symbol.
**
** CALLING SEQUENCE:
**
**	void symbol::set_symbol_matches()
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
**	Values for same_column_above and same_column_below in
**	column1.
**
** SIDE EFFECTS:
**
**	NONE
**
*/

void symbol::set_symbol_matches()
{
	int i, j, column_depth = this->get_sequence_depth(),
		symbol_counter = 0;
	alignment_element* al_el1, * al_el2 = NULL;

	for (i = 0; i < column_depth; i++)
	{
		al_el1 = this->get_al_el(i);
		if (al_el1->get_el_obj() == NIL) continue;
		symbol_counter++;
		for (j = i + 1; j < column_depth; j++)
		{
			al_el2 = this->get_al_el(j);
			if (al_el2->get_el_obj() != NIL) break;
		}
		if (j >= column_depth) break;
		al_el1->set_same_column_below(j);
		al_el2->set_same_column_above(i);
	}

	if (symbol_counter == 1) symbol_is_a_hit = false;
	else if (symbol_counter > 1) symbol_is_a_hit = true;
	else abort_run("Invalid value for symbol_count \
		in symbol::set_symbol_matches()");
} // symbol::set_symbol_matches

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	For diagnostic purposes.
**
** CALLING SEQUENCE:
**
**	void symbol::show_al_structure()
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
**	Printout of the name of the column
**
** SIDE EFFECTS:
**
**	NONE
**
*/

void symbol::show_al_structure()
{
	int i;

	fprintf(output_file, "%s%d%s",
		"sequence_depth = ", sequence_depth,
		", hit node ID = ");

	if (h_node_ID == 0) fprintf(output_file, "NIL");
	else if (h_node_ID == NULL_VALUE)
		fprintf(output_file, "NULL_VALUE");
	else fprintf(output_file, "%d",
		h_node_ID);

	fprintf(output_file, ", is_a_hit = ");

	if (symbol_is_a_hit)
		fprintf(output_file, "true");
	else fprintf(output_file, "false");

	fprintf(output_file, "\n");
	for (i = 0; i < sequence_depth; i++)
		al_el_array[i].show_al_structure();
	fprintf(output_file, "\n");
} // symbol::show_al_structure

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Recursive copy constructor for a tree object.
**
** CALLING SEQUENCE:
**
**	tree_object::tree_object(tree_object &tr_obj) : base_object(tr_obj)
**
** FORMAL ARGUMENTS:
**
**	Return value:	NONE
**
**	tr_obj: 	The object to be copied.
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

tree_object::tree_object(tree_object& tr_obj) : base_object(tr_obj)
{
	base_object* child, * new_child;

	first_element = NIL;
	current_el_pos = NIL;
	number_of_children = tr_obj.number_of_children;

	// Now make a 'deep' copy of the structure of sequence columns and
	// sequence elements.

	tr_obj.initialise();
	while (child = tr_obj.get_next_child())
	{
		// The following statement makes a copy of al_col2 including
		// copies of the constituent al_els.

		new_child = child->clone();

		this->add_child(new_child);
	}

	this->mark_parent_and_int_positions_non_recursive(); // take care!
		// This alters the value of the 'parent' field.

} // tree_object::tree_object(tree_object &)

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Sorts the children of a tree_object by their compression_difference
**	scores.
**
**	It is assumed that all the children of 'this' are tree objects
**	(eg patterns).
**
** CALLING SEQUENCE:
**
**	void tree_object::sort_children()
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
**	Children in sort order
**
** SIDE EFFECTS:
**
**	NONE
**
*/

void tree_object::sort_children()
{
	cost_tree_object* child, * best_child;
	group* group_of_children = new group;
	double best_CD, child_CD;

	while (true)
	{
		if (this->is_empty()) break;

		// Find the child with the highest CD.

		best_CD = LOW_VALUE;
		best_child = NIL;
		list_for(child, cost_tree_object, this)
		{
			child_CD = child->get_compression_difference();
			if (child_CD > best_CD)
			{
				best_CD = child_CD;
				best_child = child;
			}
		}

		if (best_child == NIL)
			abort_run("No best child found in tree_object::sort_children()");

		this->extract_child(best_child);
		group_of_children->add_child(best_child);
	}

	// Now put the children back.

	while (child = (cost_tree_object*)group_of_children->
		extract_first_child())
		this->add_child(child);

	delete group_of_children;

} // tree_object::sort_children

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Arranges the symbols in a sequence or a group in alpha-numeric order of
**	their name fields. The method is not valid (and will fail) if
**	the sequence or group is not composed entirely of symbols.
**
** CALLING SEQUENCE:
**
**	void tree_object::sort_by_name()
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
**
*/

void tree_object::sort_by_name()
{
	int i, string_comparison, fe_symbol_array;
	symbol* symbol_array[LARGE_SCRATCH_ARRAY_SIZE], * symbol1, * symbol2;
	const char* s1, * s2;
	base_object* obj1;

	// Transfer children from 'this' to symbol_array[] and release
	// from 'this'.

	this->initialise();
	i = 0;
	while (obj1 = this->get_next_child())
	{
		if (obj1->is_symbol() == false)
			abort_run("Invalid object in sort_by_name()");
		symbol1 = (symbol*)obj1;
		symbol_array[i++] = symbol1;
		if (i >= LARGE_SCRATCH_ARRAY_SIZE)
			abort_run("Array overflow in sort_by_name");
	}
	fe_symbol_array = i;
	this->release_children();

	// Now transfer them back to this, inserting them in
	// alpha-numeric order.

	for (i = 0; i < fe_symbol_array; i++)
	{
		symbol1 = symbol_array[i];
		if (this->is_empty())
		{
			this->add_child(symbol1);
			continue;
		}

		// There is at least one symbol in this ;

		this->initialise();
		while (symbol2 = (symbol*)this->get_next_child())
		{
			s1 = symbol1->get_name();
			s2 = symbol2->get_name();
			string_comparison = strcmp(s1, s2);
			if (string_comparison < 0)
			{
				this->precede(symbol1, symbol2);
				goto L1;
			}
		}

		// If this point is reached, this means that symbol1 comes
		// after the last symbol in this.

		this->add_child(symbol1);

	L1:;
	}
} // tree_object::sort_by_name

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Adds a new object to the end of the message-receiving
**	object. If the new object is NIL, nothing happens.
**
**	Notice that the position of current_el_pos
**	is not changed by adding a new child to an object.
**
** CALLING SEQUENCE:
**
**	void tree_object::add_child(base_object *child)
**
** FORMAL ARGUMENTS:
**
**	Return value:	None.
**
**	child:			The new base_object to be added.
**
** IMPLICIT INPUTS:
**
**	Functions:		new_element
**
** IMPLICIT OUTPUTS:
**
**	NONE
**
** SIDE EFFECTS:
**
**	NONE. Notice, in particular, that the position of current_el_pos
**	is not changed by adding a new child to an object.
*/

void tree_object::add_child(base_object* child)
{
	list_element* temp1, * temp2, * next;

	if (child == NIL) return;
	temp1 = new list_element(child);
	temp2 = first_element;
	if (temp2 == NIL)
	{
		first_element = temp1;
		this->mark_parent_and_int_positions_non_recursive(); // take 
		// care! This alters the value of the 'parent' field.
		return;
	}
	while (next = temp2->get_next()) temp2 = next;
	temp2->set_next(temp1);
	this->mark_parent_and_int_positions_non_recursive(); // take 
	// care! This alters the value of the 'parent' field.
} /* tree_object::add_child */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Adds a new object to the beginning of 'this' object.
**	If the new object is NIL, nothing happens.
**
** CALLING SEQUENCE:
**
**	void tree_object::add_child_at_start(base_object *child)
**
** FORMAL ARGUMENTS:
**
**	Return value:	None.
**
**	child:			The new object to be added.
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

void tree_object::add_child_at_start(base_object* child)
{
	list_element* temp1, * temp2;

	if (child == NIL)
		abort_run("NIL child for add_child_at_start");
	temp1 = new list_element(child);
	temp2 = first_element;
	first_element = temp1;
	temp1->set_next(temp2);
	this->mark_parent_and_int_positions_non_recursive(); // take 
	// care! This alters the value of the 'parent' field.
}; /* tree_object::add_child_at_start */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Extracts (unlinks) an specific object from a composite object.
**
** CALLING SEQUENCE:
**
**	void tree_object::extract_child(base_object *child) ;
**
** FORMAL ARGUMENTS:
**
**	Return value:	void.
**
**	child:			Node to be extracted.
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
**	The object is removed from the parent structure but is not deleted
**	(i.e. it's heap storage is still allocated).
*/

void tree_object::extract_child(base_object* child)
{
	list_element* position, * posx;
	base_object* temp;

	if (child == NIL) abort_run("Invalid parameter to extract_child");
	position = NIL;
	temp = this->get_next_child_by_el_pos(&position);

	if (temp == child)
	{
		first_element = position->get_next();
		delete position;
		this->mark_parent_and_int_positions_non_recursive(); // take 
		// care! This alters the value of the 'parent' field.
		return;
	}

	while (true)
	{
		posx = position;
		temp = this->get_next_child_by_el_pos(&position);
		if (temp == NIL)
			abort_run("Child not found in extract_child");
		if (temp == child) break;
	}

	posx->set_next(position->get_next());
	delete position;
	this->mark_parent_and_int_positions_non_recursive(); // take 
	// care! This alters the value of the 'parent' field.
} /* tree_object::extract_child */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	'Gets' the children of an object in succession, ie it fetches
**	their addresses without disturbing their positions in the object.
**	If the value of *position is NIL, the function
**	gets the first child and sets *position to the position of that
**	child. Otherwise it gets the child following *position and
**	updates *position. It returns NIL when there are no more children
**	to be fetched.
**
** CALLING SEQUENCE:
**
**	base_object *tree_object::get_next_child()
**
** FORMAL ARGUMENTS:
**
**	Return value:	The child being fetched. NIL if there are no
**					more children to be fetched.
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

base_object* tree_object::get_next_child()
{
	if (current_el_pos == NIL)
	{
		/* This is the signal to get the first child of 'this',
		if any. */

		current_el_pos = this->first_element;
		if (current_el_pos == NIL) return(NIL);
		else return((current_el_pos)->get_el_obj());
	}
	else
	{
		/* In this case up-date current_el_pos
		and get the corresponding child. */

		current_el_pos = (current_el_pos)->get_next();
		if (current_el_pos == NIL)
			return(NIL);
		else return((current_el_pos)->get_el_obj());
	}
} /* tree_object::get_next_child */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	'Gets' the children of an object in succession, ie it fetches
**	their addresses without disturbing their positions in the object.
**	If the value of *position is NIL, the function
**	gets the first child and sets *position to the position of that
**	child. Otherwise it gets the child following *position and
**	updates *position. It returns NIL when there are no more children
**	to be fetched.
**
** CALLING SEQUENCE:
**
**	base_object *tree_object::get_next_child_by_el_pos(list_element
**					**position)
**
** FORMAL ARGUMENTS:
**
**	Return value:	The child being fetched. NIL if there are no
**					more children to be fetched.
**
**	position:		The address of the address of the element
**					of the child which was last returned.
**
** IMPLICIT INPUTS:
**
**	position
**
** IMPLICIT OUTPUTS:
**
**	The value of *position in the function which calls this
**	function is up-dated.
**
** SIDE EFFECTS:
**
**	NONE
*/

base_object* tree_object::get_next_child_by_el_pos(list_element** position)
{
	if (*position == NIL)
	{
		/* This is the signal to get the first child of 'this',
		if any. */

		*position = this->first_element;
		if (*position == NIL) return(NIL);
		else return((*position)->get_el_obj());
	}
	else
	{
		/* In this case up-date *position and get the corresponding
		child. */

		*position = (*position)->get_next();
		if (*position == NIL)
			return(NIL);
		else return((*position)->get_el_obj());
	}
} /* tree_object::get_next_child_by_el_pos */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	'Gets' the list elements of an object in (reverse) succession,
**	ie it fetches their addresses without disturbing their positions
**	in the object. If the value of position is NIL, the function
**	gets the last el_pos. Otherwise it gets the el_pos preceding *position.
**	It returns NIL when there are no more list_elements to be fetched.
**
** CALLING SEQUENCE:
**
**	list_element *tree_object::get_preceding_el_pos(list_element *position)
**
** FORMAL ARGUMENTS:
**
**	Return value:	The list_element being fetched. NIL if there are no
**					more list_elements to be fetched.
**
**	position:		The reference position.
**
** IMPLICIT INPUTS:
**
**	position
**
** IMPLICIT OUTPUTS:
**
**	NONE.
**
** SIDE EFFECTS:
**
**	NONE
*/

list_element* tree_object::get_preceding_el_pos(list_element* position)
{
	list_element* pos1, * pos2;

	if (first_element == NIL)
		return(NIL);

	if (position == NIL)
	{
		// This is the signal to get the last list_element of 'this',
		// if any.

		return(get_last_el_pos());
	}

	// position is not NIL.

	pos1 = NIL;
	pos2 = first_element;
	while (pos2 != position)
	{
		pos1 = pos2;
		pos2 = pos2->get_next();
	}

	return(pos1);

} // tree_object::get_preceding_el_pos

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	'Gets' the child of an object preceding a given child. If the
**	given child is the first child, or if there is only one child,
**	the function returns NIL. If the given child is NIL,
**	the function returns an error.
**
** CALLING SEQUENCE:
**
**	base_object *tree_object::get_preceding_child(base_object *child)
**
** FORMAL ARGUMENTS:
**
**	Return value:	The child before child. NIL if there is
**					only one child in 'this'.
**
**	child:			The given child whose predecessor is to be found.
**
** IMPLICIT INPUTS:
**
**	position
**
** IMPLICIT OUTPUTS:
**
**	NONE
**
** SIDE EFFECTS:
**
**	NONE
*/

base_object* tree_object::get_preceding_child(base_object* child)
{
	base_object* preceding_child = NIL, * child1;
	list_element* pos1;

	if (child == NIL) abort_run("NIL parameter for get_preceding_child");

	pos1 = NIL;
	while (child1 = this->get_next_child_by_el_pos(&pos1))
	{
		if (child1 == child) return(preceding_child);
		preceding_child = child1;
	}

	abort_run("child not found in get_preceding_child");

	return(NIL);
} /* tree_object::get_preceding_child */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	'Gets' the children of an object in (reverse) succession, ie it fetches
**	their addresses without disturbing their positions in the object.
**	If the value of position is NIL, the function
**	gets the last child and sets *position to the position of that
**	child. Otherwise it gets the child preceding *position and
**	updates *position. It returns NIL when there are no more children
**	to be fetched.
**
** CALLING SEQUENCE:
**
**	base_object *tree_object::
**		get_preceding_child_by_el_pos(list_element **position)
**
** FORMAL ARGUMENTS:
**
**	Return value:	The child being fetched. NIL if there are no
**					more children to be fetched.
**
**	position:		The address of the address of the element
**					of the child which was last returned.
**
** IMPLICIT INPUTS:
**
**	position
**
** IMPLICIT OUTPUTS:
**
**	The value of *position in the function which calls this
**	function is up-dated.
**
** SIDE EFFECTS:
**
**		NONE
*/

base_object* tree_object::get_preceding_child_by_el_pos(list_element** position)
{
	base_object* child;
	list_element* pos1, * pos2;

	if (first_element == NIL)
	{
		*position = NIL;
		return(NIL);
	}

	if (*position == NIL)
	{
		/* This is the signal to get the last child of 'this',
		if any. */

		*position = this->get_last_el_pos();
		child = this->get_child_by_el_pos(position);
		return(child);
	}

	/* *position is not NIL. Up-date *position and get the
	corresponding child. */

	pos1 = NIL;
	pos2 = first_element;
	while (pos2 != *position)
	{
		pos1 = pos2;
		pos2 = pos2->get_next();
	}

	*position = pos1;
	if (pos1 == NIL) return(NIL);
	else return(pos1->get_el_obj());

} /* tree_object::get_preceding_child_by_el_pos */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Tests whether a child object is the last child of 'this' parent.
**
** CALLING SEQUENCE:
**
**	bool tree_object::this_is_last_child(base_object *child)
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if child is the last child, false otherwise.
**
**	child:			The child to be tested.
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

bool tree_object::this_is_last_child(base_object* child)
{
	if (child == NIL) abort_run("NIL child in this_is_last_child");
	list_element* el_pos1 = this->get_el_pos_by_child(child);
	if (el_pos1->get_next() == NIL) return(true);
	else return(false);
} // tree_object::this_is_last_child

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Recursively marks the 'parent' and 'position' fields of a tree_object.
**	This function also sets the value of 'number_of_children'.
**
** CALLING SEQUENCE:
**
**	void tree_object::mark_parent_and_int_positions_recursive()
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

void tree_object::mark_parent_and_int_positions_recursive()
{
	list_element* pos1;
	int position = 0;
	tree_object* obj;

	pos1 = first_element;
	if (pos1 == NIL) return;
	while (pos1 != NIL)
	{
		pos1->set_parent(this);
		pos1->set_position(position++);
		obj = (tree_object*)pos1->get_el_obj();
		obj->mark_parent_and_int_positions_recursive();
		pos1 = pos1->get_next();
	}
	number_of_children = position;
} /* tree_object::mark_parent_and_int_positions_recursive */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Marks the 'parent' and 'position' fields of a tree_object. This is the
**	non-recursive version of
**	tree_object::mark_parent_and_int_positions_recursive().
**
**	Notice that this method does not disturb the value of
**	current_el_pos.
**
**	But take care! This method alters the value of the 'parent' field.
**
** CALLING SEQUENCE:
**
**	void tree_object::mark_parent_and_int_positions_non_recursive()
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

void tree_object::mark_parent_and_int_positions_non_recursive()
{
	list_element* pos1;
	int position = 0;

	pos1 = first_element;
	if (pos1 == NIL) return;
	while (pos1 != NIL)
	{
		pos1->set_parent(this);
		pos1->set_position(position++);
		pos1 = pos1->get_next();
	}
	number_of_children = position;
} /* tree_object::mark_parent_and_int_positions_non_recursive */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	'Gets' the child of an object by its ordinal position, ie it fetches
**	its address without disturbing its position in the object, using
**	ordinal position to determine which is accessed.
**
** CALLING SEQUENCE:
**
**	base_object *tree_object::get_child_by_int_pos(int driving_patt_int_pos)
**
** FORMAL ARGUMENTS:
**
**	Return value:			The child being fetched. NIL if there is no
**							child at the given position.
**
**	driving_patt_int_pos:	The ordinal position (first position is 0).
**
** IMPLICIT INPUTS:
**
**	NONE
**
** IMPLICIT OUTPUTS:
**
**	The value of current_el_pos in 'this' is set to the
**	value corresponding to the child which is returned.
**
** SIDE EFFECTS:
**
**	NONE
*/

base_object* tree_object::get_child_by_int_pos(int driving_patt_int_pos)
{
	base_object* child;
	int counter = 0;

	this->initialise();
	child = this->get_next_child();
	if (child == NIL) return(NIL);
	while (counter++ < driving_patt_int_pos)
	{
		child = this->get_next_child();
		if (child == NIL) return(NIL);
	}
	return(child);
} /* tree_object ::get_child_by_int_pos */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Return a pointer (and element) to the last object within 'this'
**	object. If there are no children, the function returns
**	NIL.
**
** CALLING SEQUENCE:
**
**	base_object *tree_object::get_last_child() ;
**
** FORMAL ARGUMENTS:
**
**	Return value:	Pointer to the last object within 'this' object.
**
** IMPLICIT INPUTS:
**
**	NONE
**
** IMPLICIT OUTPUTS:
**
**	None, but see `position' above.
**
** SIDE EFFECTS:
**
**	NONE
*/

base_object* tree_object::get_last_child()
{
	list_element* position = first_element;
	if (position == NIL) return(NIL);
	while (position != NIL)
	{
		current_el_pos = position;
		position = position->get_next();
	}
	return(current_el_pos->get_el_obj());
} /* tree_object::get_last_child */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Adds a new child to 'this' node so that it immediately
**	precedes another specified child within the parent object.
**	This is the complementary function to `follow'.
**
** CALLING SEQUENCE:
**
**	void tree_object::precede(base_object *child1, base_object *child2)
**
** FORMAL ARGUMENTS:
**
**	Return value:	None.
**
**	child1: 		The new object to be inserted before child2.
**	child2: 		The child that child1 is to be inserted
**					in front of.
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

void tree_object::precede(base_object* child1, base_object* child2)
{
	list_element* tempx, * temp1, * temp2;

	if (child1 == NIL || child2 == NIL || child1 == child2)
		abort_run("Invalid parameter(s) for precede");
	temp2 = first_element;
	if (temp2 == NIL) abort_run("child2 not found in precede");

	if (temp2->get_el_obj() == child2)
	{
		temp1 = new list_element(child1);
		temp1->set_next(temp2);
		first_element = temp1;

		// Revise position values for list_elements which follow
		// the inserted child1.

		this->mark_parent_and_int_positions_non_recursive(); // take
		// care! This alters the value of the 'parent' field.
		return;
	}

	// child2 is not the first object in the list.

	tempx = temp2;
	temp2 = temp2->get_next();
	while (temp2 != NIL)
	{
		if (temp2->get_el_obj() == child2)
		{
			temp1 = new list_element(child1);
			tempx->set_next(temp1);
			temp1->set_next(temp2);

			// Revise position values for list_elements which
			// follow the inserted child1.

			this->mark_parent_and_int_positions_non_recursive();
			// take care! This alters the value of
			// the 'parent' field.
			return;
		}
		tempx = temp2;
		temp2 = temp2->get_next();
	}

	abort_run("child2 not found in precede");
} /* tree_object::precede */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Adds a new child to 'this' node so that it immediately
**	follows another specified child within the parent object.
**	This is the complementary function to `precede'.
**
** CALLING SEQUENCE:
**
**	void tree_object::follow(base_object *child1, base_object *child2)
**
** FORMAL ARGUMENTS:
**
**	Return value:	None.
**
**	child1: 		child within the parent object that child2
**					is to follow
**	child2:			Object to be inserted after child1.
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

void tree_object::follow(base_object* child1, base_object* child2)
{
	list_element* temp1, * temp2;

	if (child1 == NIL || child2 == NIL || child1 == child2)
		abort_run("Invalid parameter(s) for follow");;
	temp1 = first_element;
	while (temp1 != NIL)
	{
		if (temp1->get_el_obj() == child1)
		{
			temp2 = new list_element(child2);
			temp2->set_next(temp1->get_next());
			temp1->set_next(temp2);

			// Revise position values for list_elements which
			// follow the inserted child.

			this->mark_parent_and_int_positions_non_recursive();
			// take care! This alters the value of
			// the 'parent' field.
			return;
		}
		temp1 = temp1->get_next();
	}

	abort_run("child1 not found in follow");
} /* tree_object::follow */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Returns a stored integer representing the ordinal position of a child
**	within its parent (starting at 0).
**
** CALLING SEQUENCE:
**
**	int tree_object::get_int_pos_by_child(base_object *child) ;
**
** FORMAL ARGUMENTS:
**
**	Return value:	Position of child.
**
**	child:			The child whose position is to be found.
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

int tree_object::get_int_pos_by_child(base_object* child)
{
	list_element* pos1;

	if (child == NIL) abort_run("NIL child in get_int_pos_by_child");
	pos1 = first_element;
	while ((pos1) != NIL)
	{
		if ((pos1)->get_el_obj() == child) break;
		pos1 = (pos1)->get_next();
	}
	if ((pos1) == NIL)
		abort_run("No child found in get_int_pos_by_child");
	return((pos1)->get_position());
} /* tree_object::get_int_pos_by_child */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Returns a stored integer representing the ordinal position of a child
**	within its parent (starting at 0). By contrast, with
**	tree_object::get_int_pos_by_child(), this method obtains the
**	position by counting. It does not set the value in the
**	list element corresponding to the child.
**
** CALLING SEQUENCE:
**
**	int tree_object::find_int_pos_by_child(base_object *child) ;
**
** FORMAL ARGUMENTS:
**
**	Return value:	Position of child.
**
**	child:			The child whose position is to be found.
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

int tree_object::find_int_pos_by_child(base_object* child)
{
	list_element* pos1;
	int counter = 0;

	if (child == NIL)
		abort_run("NIL child in find_int_pos_by_child().");
	pos1 = first_element;
	while ((pos1) != NIL)
	{
		if ((pos1)->get_el_obj() == child) break;
		pos1 = (pos1)->get_next();
		counter++;
	}
	if ((pos1) == NIL)
		abort_run("No child found in find_int_pos_by_child().");
	return(counter);
} /* tree_object::find_int_pos_by_child */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Unlinks the top-level children of 'this' object.  The storage
**	for the children is left allocated but the elements between the
**	'this' and the children are deleted.
**
** CALLING SEQUENCE:
**
**	void tree_object::release_children()
**
** FORMAL ARGUMENTS:
**
**	Return value:	None.
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
**	The tree_object storage for the children is still allocated.
*/

void tree_object::release_children()
{
	base_object* child;
	list_element* position;

	position = NIL;
	child = this->get_next_child_by_el_pos(&position);
	while (child != NIL)
	{
		first_element = position->get_next();
		delete position;
		position = NIL;
		child = this->get_next_child_by_el_pos(&position);
	}
} /* tree_object::release_children */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Completes the deletion of a tree_object or hit_node. This function is
**	called from ~hit_node() or ~tree_object().
**
** CALLING SEQUENCE:
**
**	void tree_object::finish_deletion()
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
**
*/

void tree_object::finish_deletion()
{
	list_element* position, * temp;
	base_object* child;

	if (first_element == NIL) return;

	position = NIL;
	child = this->get_next_child_by_el_pos(&position);
	while (child != NIL)
	{
		delete child;
		temp = position;
		child = this->get_next_child_by_el_pos(&position);
		delete temp;
	}
} // tree_object::finish_deletion.

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Checks whether the pattern contains any ID symbol that is a
**	copy of a given symbol.
**
** CALLING SEQUENCE:
**
**	bool sequence::contains_copy_of_ID_symbol(symbol *symbol1)
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if the pattern contains an ID symbol
**					that is a copy of symbol1, false otherwise.
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

bool sequence::contains_copy_of_ID_symbol(symbol* symbol1)
{
	symbol* symbol2;
	int status;

	/* Run over set until symbol1 is found or CONTENTS symbols
	are reached */

	list_for(symbol2, symbol, this)
	{
		status = symbol2->get_status();
		if (status == CONTENTS) return(false);
		if (status != IDENTIFICATION) continue;

		if (symbol1->name_matches(symbol2))
			return(true);
	}

	return(false);

} // sequence::contains_copy_of_ID_symbol

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Checks first to see whether or not the given pattern has a
**	UNIQUE_ID_SYMBOL. If it has, the method does nothing and exits.
**	If it has not, it creates a unique_id_symbol and adds it to the
**	pattern just before the CONTENTS symbols.
**
** CALLING SEQUENCE:
**
**	void sequence::add_unique_id_symbol()
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

void sequence::add_unique_id_symbol()
{
	if (has_unique_id_symbol()) return;

	symbol* symbol1 = make_unique_id_symbol(), * symbol2;

	list_for(symbol2, symbol, this)
	{
		if (symbol2->get_status() == CONTENTS
			|| (symbol2->get_type() == RIGHT_BRACKET
				&& symbol2->get_status() == BOUNDARY_MARKER))
			break;
	}

	if (symbol2 == NIL)
		abort_run("Anomaly in sequence::add_unique_id_symbol().");

	this->precede(symbol1, symbol2);

	fprintf(output_file, "%s%s%s",
		"Unique ID symbol ",
		symbol1->get_name(),
		" added to pattern ");

	write_pattern(true, false);

} // sequence::add_unique_id_symbol

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Checks that hits with New symbols form a coherent sequence.
**
** CALLING SEQUENCE:
**
**	bool sequence::new_hits_form_coherent_sequence()
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if there are no gaps between hit symbols
**					within the New pattern, false otherwise.
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

bool sequence::new_hits_form_coherent_sequence()
{
	if (sequence_depth == 1) return(false); // This is not
		// an alignment and so there are no hits with New
		// symbols.

	symbol* new_symbol1, * col1;
	int int_pos1, int_pos2 = NULL_VALUE;

	list_for(col1, symbol, this)
	{
		new_symbol1 = col1->get_row_symbol(0);
		if (new_symbol1 == NIL) continue;

		int_pos1 = col1->get_row_orig_int_pos(0);
		if (int_pos2 != NULL_VALUE)
		{
			if (int_pos1 - int_pos2 > 1) return(false);
		}

		int_pos2 = int_pos1;
	}

	return(true);

} // sequence::new_hits_form_coherent_sequence

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Checks whether the CONTENTS symbol of 'this' sequence represent
**	a single reference comprising a LEFT_BRACKET and CONTEXT_SYMBOL
**	and a RIGHT_BRACKET in that order with no other CONTENTS symbols.
**
** CALLING SEQUENCE:
**
**	bool sequence::check_for_single_reference()
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if the sequence is a single reference,
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
*/

bool sequence::check_for_single_reference()
{
	// Check whether 'this' sequence comprises a single reference
	// to another pattern or class, meaning a LEFT_BRACKET followed
	// by a CONTEXT_SYMBOL followed by a RIGHT_BRACKET and nothing
	// else.

	symbol* symbol1, * found_class_symbol;
	int counter = 0;
	bool single_reference_found = false;
	list_for(symbol1, symbol, this)
	{
		counter++;

		if (counter == 1)
		{
			if (symbol1->get_type() == LEFT_BRACKET)
				continue;
			else return(false);
		}

		if (counter == 2)
		{
			if (symbol1->get_type() == CONTEXT_SYMBOL)
			{
				found_class_symbol = symbol1;
				continue;
			}
			else return(false);
		}

		if (counter == 3)
		{
			if (symbol1->get_type() == RIGHT_BRACKET)
			{
				single_reference_found = true;
				continue;
			}
			else return(false);
		}
	}

	if (counter == 3 && single_reference_found)
	{
		set_single_reference(true);
		return(true);
	}
	else return(false);

} // sequence::check_for_single_reference

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Using the values in symbol_types_in_old, this method assigns frequency
**	values and bit_cost values to each symbol in 'this' pattern.
**
** CALLING SEQUENCE:
**
**	void sequence::assign_frequencies_and_costs()
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
** IMPLICIT INPUTS:
**
**	symbol_types_in_old
**
** IMPLICIT OUTPUTS:
**
**	NONE
**
** SIDE EFFECTS:
**
**	NONE
*/

void sequence::assign_frequencies_and_costs()
{
	symbol* symbol1;

	list_for(symbol1, symbol, this)
		symbol1->assign_frequency_and_cost();

} // sequence::assign_frequencies_and_costs

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Tests the alignment to see whether the alignment has more than
**	one LEFT_BRACKET symbols with the status of BOUNDARY_MARKER. If id
**	does, the alignment is marked as a composite alignment.
**
** CALLING SEQUENCE:
**
**	void sequence::check_for_composite_structure()
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if the alignment is legal as described above.
**					false otherwise.
**
** IMPLICIT INPUTS:
**
**	NONE
**
** IMPLICIT OUTPUTS:
**
**	'this' alignment may be marked as composite.
**
** SIDE EFFECTS:
**
**	NONE
*/

void sequence::check_for_composite_structure()
{
	symbol* symbol1;
	int counter = 0;

	list_for(symbol1, symbol, this)
	{
		if (symbol1->get_type() == LEFT_BRACKET)
		{
			if (symbol1->get_status() == BOUNDARY_MARKER)
				counter++;
		}
	}

	if (counter > 1)
		composite_alignment = true;

} // sequence::check_for_composite_structure

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Tests the alignment to see whether there are any LEFT_BRACKET symbols
**	at a depth greater than 1 that have BOUNDARY_MARKER status. Any such
**	alignment is illegal.
**
** CALLING SEQUENCE:
**
**	bool sequence::is_legal()
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if the alignment is legal as described above.
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
*/

bool sequence::is_legal()
{
	symbol* symbol1;
	int depth = 0;

	list_for(symbol1, symbol, this)
	{
		if (symbol1->get_type() == LEFT_BRACKET)
		{
			depth++;
			if (depth > 1)
			{
				if (symbol1->get_status() == BOUNDARY_MARKER)
					return(false);
			}
		}

		if (symbol1->get_type() == RIGHT_BRACKET)
			depth--;
	}

	return(true);

} // sequence::is_legal

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Adds a 'reference' to 'this' sequence, where a reference is a left
**	bracket followed by a class symbol followed by a right bracket, all
**	with status CONTENTS.
**
** CALLING SEQUENCE:
**
**	void sequence::add_reference(int context_number)
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	context_number:	A number from which the name of a CONTEXT_SYMBOL
**					is made for the reference.
**
** IMPLICIT INPUTS:
**
**	NONE
**
** IMPLICIT OUTPUTS:
**
**	Reference symbols are added to 'this' pattern.
**
** SIDE EFFECTS:
**
**	NONE
*/

void sequence::add_reference(int context_number)
{
	symbol* bracket, * cs;
	char symbol_name[STRING_LENGTH];

	bracket = make_bracket_symbol(LEFT_BRACKET, CONTENTS, 1);
	add_child(bracket);

	// Make new context symbol.

	cs = new symbol("", 1, NULL_VALUE);
	sprintf(symbol_name, "%d", context_number);
	cs->set_type(CONTEXT_SYMBOL);
	cs->set_status(CONTENTS);
	cs->set_name(symbol_name);
	cs->assign_frequency_and_cost();
	add_child(cs);

	bracket = make_bracket_symbol(RIGHT_BRACKET, CONTENTS, 1);
	add_child(bracket);

} // sequence::add_reference

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Adds a CONTEXT_SYMBOL at the start of the list of
**	IDENTIFICATION symbols at the start of the pattern. The method
**	checks to see whether a given CONTEXT_SYMBOL is already present
**	before it adds it.
**
** CALLING SEQUENCE:
**
**	bool sequence::add_context_symbol(symbol *context_symbol)
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if the context_symbol is added,
**					false otherwise.
**
**	context_symbol:	The symbol to be added.
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

bool sequence::add_context_symbol(int context_number)
{
	symbol* symbol1, * cs;
	char symbol_name[STRING_LENGTH];

	// Check whether a copy of context_number is already present.
	// If it is, return false, otherwise add the context_symbol
	// and return true.

	sprintf(symbol_name, "%d", context_number);
	list_for(symbol1, symbol, this)
	{
		if (symbol1->get_status() == CONTENTS) break;

		if (symbol1->name_matches_string(symbol_name))
		{
			fprintf(output_file, "%s%s%s",
				"Context number ",
				symbol_name,
				" duplicates symbol in pattern ");
			write_pattern(true, false);
			return(false);
		}
	}

	// Make new context symbol.

	cs = new symbol("", 1, NULL_VALUE);
	cs->set_type(CONTEXT_SYMBOL);
	cs->set_name(symbol_name);
	cs->assign_frequency_and_cost();
	cs->set_status(IDENTIFICATION);

	// Add the context symbol after the BOUNDARY_MARKER if there is
	// one or at the start of the pattern.

	symbol1 = (symbol*)this->get_first_child();
	if (symbol1 != NIL)
	{
		if (symbol1->get_status() == BOUNDARY_MARKER)
			follow(symbol1, cs);
		else this->add_child_at_start(cs);
	}
	else this->add_child_at_start(cs);

	fprintf(output_file, "%s%s%s",
		"Context symbol ",
		symbol_name,
		" added to pattern ");

	write_pattern(true, false);

	return(true);

} // sequence::add_context_symbol

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Using the context symbols in generalisation_list1 (which were
**	created during sequence::create_patterns()), this function
**	generalises the Old patterns by adding context symbols to them
**	according to the following procedure:
**
**	1 For each context symbol in generalisation_list1, compile a list
**	of *other* context symbols that are found in patterns that
**	contain the given context symbol. Makes sure that there are no
**	duplicates in this generalisation_list2.
**
**	2 For each pattern that contains one or more of the context symbols
**	on generalisation_list2, add a copy of the 'given' context symbol
**	under 1. Make sure that there are no duplicates.
**
** CALLING SEQUENCE:
**
**	void generalise_patterns()
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
** IMPLICIT INPUTS:
**
**	generalisation_list1
**
** IMPLICIT OUTPUTS:
**
**	Context symbols added to Old patterns.
**
** SIDE EFFECTS:
**
**	NONE
*/

void generalise_patterns(void)
{
	symbol* symbol1, * symbol2, * symbol3;
	sequence* pattern1;
	int status;

#if DIAGNOSTIC6
	fprintf(output_file, "Diagnostic: generalisation_list1:\n\n");
	list_for(symbol1, symbol, generalisation_list1)
	{
		fprintf(output_file, "%s%s",
			symbol1->get_name(),
			" ");
	}
	fprintf(output_file, "\n\n");
#endif

	list_for(symbol1, symbol, generalisation_list1)
	{
		list_for(pattern1, sequence, old_patterns)
		{
			if (pattern1->contains_copy_of_ID_symbol(symbol1) == false)
				continue;

			// pattern1 contains a copy of symbol1 and symbol1
			// is an ID-symbol in pattern1. Now add copies
			// of *other* ID symbols in the pattern to
			// generalisation_list2, avoiding duplicates.

			list_for(symbol2, symbol, pattern1)
			{
				if (symbol2->get_type() != CONTEXT_SYMBOL)
					continue;
				status = symbol2->get_status();
				if (status == CONTENTS) break;
				if (status != IDENTIFICATION) continue;
				if (symbol2->name_matches(symbol1))
					continue;
				if (generalisation_list2->contains_copy_of(symbol2))
					continue;
				symbol3 = new symbol(*symbol2);
				generalisation_list2->add_child(symbol3);
			}
		}

		// Now that generalisation_list2 has been compiled for
		// symbol1, step through the list and, for each symbol,
		// add a copy of symbol1 to every Old pattern that 
		// contains a copy of the given symbol, avoiding
		// duplicates.

#if DIAGNOSTIC6
		fprintf(output_file, "%s%s%s",
			"Diagnostic: generalisation_list2 for symbol ",
			symbol1->get_name(),
			":\n\n");
		list_for(symbol3, symbol, generalisation_list2)
		{
			fprintf(output_file, "%s%s",
				symbol3->get_name(),
				" ");
		}
		fprintf(output_file, "\n\n");
#endif

		list_for(symbol2, symbol, generalisation_list2)
		{
			list_for(pattern1, sequence, old_patterns)
			{
				if (pattern1->contains_copy_of_ID_symbol(symbol2) == false)
					continue;

				// Add a copy of symbol1 to pattern1 unless the
				// pattern already contains a copy of that symbol.

				if (pattern1->contains_copy_of(symbol1))
					continue;

				symbol3 = new symbol(*symbol1);
				symbol3->set_status(IDENTIFICATION);
				symbol3->set_type(CONTEXT_SYMBOL);
				pattern1->add_context_symbol_at_start(symbol3);
			}
		}

		// Delete the children of generalisation_list2 ready
		// for next symbol1.

		generalisation_list2->delete_children();
	}

	// Now step through the Old patterns renumbering positions.

	list_for(pattern1, sequence, old_patterns)
		pattern1->mark_parent_and_int_positions_non_recursive();

	// Delete the symbols in generalisation_list1 ready for
	// the next invocation of sequence::create_patterns() and
	// resulting generalisations.

	generalisation_list1->delete_children();

} // generalise_patterns

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Derives an encoding of the driving pattern of 'this' alignment
**	when the CONTENTS symbols of the target pattern are not completely
**	matched or the New pattern is not completely matched, directly or
**	indirectly, by a single Old pattern. This method assumes that there
**	is a single target pattern (by contrast with alignments created by
**	combine_alignments() that may contain two or more target patterns).
**
**	The tentative rule embodied in this method is:
**
**	"Make a new pattern from every coherent sequence of two or more
**	matched CONTENTS symbols in the target pattern and
**	likewise for coherent unmatched sequences. Make a new pattern
**	from every coherent sequence of one or more unmatched New symbols.
**	Derive an abstract pattern from the resulting patterns.
**	Include a code pattern for the New pattern." (see sp71_od, %62).
**
**	In SP71, v 7.3, this method has been augmented so that, when there
**	is a null element in the New pattern or the Old pattern, it creates
**	an explicit null element with a corresponding abstract pattern (as now)
**	but it also creates an abstract pattern without a reference to the
**	disjunctive class containing the null element (see sp71_od, %65). In
**	v 7.4, the null patterns have been dropped.
**
**	In SP71, v 7.7, the method is modified so that every alignment is
**	treated as if all the Old patterns had been unified to make a single
**	Old pattern, as described in sp71_od, %79.
**
** CALLING SEQUENCE:
**
**	void sequence::create_patterns()
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

void sequence::create_patterns()
{
	// Print out preliminary details.

	fprintf(output_file, "%s%d%s",
		"Learning from alignment ID",
		ID,
		" (");
	print_pattern_cycle(false, current_new_pattern);
	fprintf(output_file, "):\n\n");

	write_alignment(output_file, write_section_chars_length,
		NULL_VALUE, alignment_format);

	if (composite_alignment == true)
	{
		fprintf(output_file,
			"The alignment is composite. Learning not yet implemented.\n\n");
		return;
	}

	if (degree_of_matching == FULL_A)
	{
		fprintf(output_file,
			"The alignment is complete and there is no learning to be done.\n\n");
		return;
	}

	// Create an array to store details of the alignment. Both the test
	// pattern (== driving pattern) and the target pattern are
	// expanded so that all symbols are shown. Breaks in the matching
	// sequence in the driving pattern or the target pattern or both
	// are marked by a divider column with NIL values.

	struct al_entry
	{
		sequence* new_d_pattern;
		sequence* new_t_pattern;
	} al_array[MEDIUM_SCRATCH_ARRAY_SIZE];

	int fe_al_array = 0, i, bottom_row = sequence_depth - 1;

	sequence* d_pattern = get_row_pattern(0);
	sequence* t_pattern = get_row_pattern(bottom_row);
	sequence* new_d_pattern, * new_t_pattern;

	symbol* symbol1, * d_symbol, * t_symbol;

	for (i = 0; i < MEDIUM_SCRATCH_ARRAY_SIZE; i++)
	{
		new_d_pattern = new_t_pattern = NIL;
	}

	// Traverse 'this' alignment filling in values in al_array[].

	d_pattern->initialise();
	t_pattern->initialise();
	list_for(symbol1, symbol, this)
	{
		d_symbol = symbol1->get_row_symbol(0);
		if (d_symbol != NIL)
		{

		}
		t_symbol = symbol1->get_row_symbol(bottom_row);
	}

} // sequence::create_patterns

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Writes out a pattern or alignment as a flat sequence of symbols.
**
** CALLING SEQUENCE:
**
**	void sequence::write_pattern(bool new_line, bool write_from)
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	new_line:		If true, two line-feed characters are printed at
**					the end, otherwise they are not.
**	write_from:		true if '(from alignment ID)' is to be written,
**					otherwise false.
**
** IMPLICIT INPUTS:
**
**	NONE
**
** IMPLICIT OUTPUTS:
**
**	Output to output_file
**
** SIDE EFFECTS:
**
**	NONE
*/

void sequence::write_pattern(bool new_line, bool write_from)
{
	symbol* symbol1;

	fprintf(output_file, "%s%d", "ID", ID);
	if (write_from && derived_from_parsing != NULL_VALUE)
		fprintf(output_file, "%s%d%s",
			" (from ID",
			derived_from_parsing,
			") (");
	else fprintf(output_file, " (");

	list_for(symbol1, symbol, this)
	{
		fprintf(output_file, "%s", symbol1->get_name());
		if (current_el_pos->get_next() == NIL)
			fprintf(output_file, ")*");
		else fprintf(output_file, " ");
	}

	fprintf(output_file, "%d", frequency);
	if (new_line) fprintf(output_file, "\n\n");
} // sequence::write_pattern

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Makes a 'shallow' copy of 'this' sequence, meaning that there
**	is only one row and the depth is 1.
**
** CALLING SEQUENCE:
**
**	sequence *sequence::shallow_copy()
**
** FORMAL ARGUMENTS:
**
**	Return value:	A copy of 'this'.
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

sequence* sequence::shallow_copy()
{
	symbol* copy_symbol, * symbol1;
	sequence* copy_sequence = new sequence(BASIC_PATTERN);

	list_for(symbol1, symbol, this)
	{
		copy_symbol = symbol1->shallow_copy();
		copy_sequence->add_child(copy_symbol);
	}

	copy_sequence->set_sequence_depth(1);
	copy_sequence->set_origin(origin);
	copy_sequence->set_total_cost(total_cost);

	return(copy_sequence);
} // sequence::shallow_copy

//*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Compares two sequences to see whether they match or not. This alignment
**	assumes that the sequence is only a sequence of simple symbols
**	without rows. If there are rows, the method flags an error. An error
**	is also flagged if the two sequences are in fact the same sequence.
**
** CALLING SEQUENCE:
**
**	bool sequence::is_copy_of(sequence *seq1)
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if the two sequences match, false otherwise.
**
**	seq1:			The second of the two sequences to be compared.
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

bool sequence::is_copy_of(sequence* seq1)
{
	symbol* symbol1, * symbol2;
	int seq1_length, seq1_depth;

	if (seq1 == NIL) abort_run("NIL value for seq1 in \
		sequence::alignment_matches");

	// Check whether the two sequences are, in fact, the same sequence.

	if (seq1 == this)
		abort_run("One sequence compared with itself in is_copy_of().");

	/* Compare the lengths of the sequences. */

	seq1_length = seq1->get_number_of_children();

	if (number_of_children != seq1_length) return(false);

	// Check that sequence depth is not greater than 1 for either sequence.

	seq1_depth = seq1->get_sequence_depth();

	if (sequence_depth != 1 || seq1_depth != 1) return(false);

	// Check for a match in the basic sequence of symbols.

	symbol2 = (symbol*)seq1->get_first_child();
	for (symbol1 = (symbol*)this->get_first_child(); symbol1 != NIL;
		symbol1 = (symbol*)this->get_next_child())
	{
		if (symbol1->name_matches(symbol2) == false) return(false);
		symbol2 = (symbol*)seq1->get_next_child();
		if (symbol2 == NIL)
		{
			symbol1 = (symbol*)this->get_next_child();
			break;
		}
	}

	if (symbol1 != NIL || symbol2 != NIL)
		return(false);

	return(true);
} /* sequence::is_copy_of */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	For each column in 'this' alignment, the method finds the values
**	of frequency and bit_cost for the column (from any symbol within
**	the column) and assigns these values to the column itself.
**
** CALLING SEQUENCE:
**
**	void sequence::correct_column_values()
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
**	Corrected values for frequency and bit_cost for each column in 'this'.
**
** SIDE EFFECTS:
**
**	NONE
**
*/

void sequence::correct_column_values()
{
	symbol* col1;
	symbol* symbol1 = NULL;
	int i;

	list_for(col1, symbol, this)
	{
		for (i = 1; i < sequence_depth; i++)
		{
			symbol1 = col1->get_row_symbol(i);
			if (symbol1 != NIL) break;
		}

		col1->set_frequency(symbol1->get_frequency());
		col1->set_bit_cost(symbol1->get_bit_cost());
	}
} // sequence::correct_column_values

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Tests whether or not all the symbols from cnp
**	are matched in the alignment.
**
** CALLING SEQUENCE:
**
**	bool sequence::all_new_symbols_matched(sequence *cnp)
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if all the symbols in cnp
**					are matched, false otherwise.
**
**	cnp:			The New pattern being processed.
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

bool sequence::all_new_symbols_matched(sequence* cnp)
{
	// Check that this is an alignment and not a flat pattern.

	if (sequence_depth < 2)
		abort_run("Invalid alignment in sequence::all_new_symbols_matched()");

	symbol* col1, * symbol_new1, * symbol_new2 = NULL;
	sequence* pattern_new = get_row_pattern(0);

	this->initialise();
	list_for(symbol_new1, symbol, pattern_new)
	{
		while (col1 = (symbol*)this->get_next_child())
		{
			symbol_new2 = col1->get_row_symbol(0);
			if (symbol_new2 != NIL) break;
		}

		if (col1 == NIL) // The alignment has been exhausted without
			// finding all the symbols in cnp.
			return(false);

		if (col1->is_a_hit() == false) return(false);
		if (symbol_new1 != symbol_new2) return(false);
	}

	return(true);
} // sequence::all_new_symbols_matched

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Adds LEFT_BRACKET and RIGHT_BRACKET symbols with BOUNDARY_MARKER
**	status to the beginning and end of a pattern if they are
**	not already present. Adds a CONTEXT_SYMBOL and a
**	'discrimination symbol' is added if required.
**
**	At the beginning of this method is a check to see whether the
**	CONTENTS symbols of 'this' pattern comprises a single 'reference'
**	to another pattern or class. If it is, the method returns
**	the CONTEXT_SYMBOL of that reference and indicates, via
**	is_single_reference, that a single reference has been found.
**
** CALLING SEQUENCE:
**
**	void sequence::add_ID_symbols(symbol *context_symbol,
**		bool add_uid_symbol)
**
** FORMAL ARGUMENTS:
**
**	Return value:		void.
**
**	context_symbol:		A symbol that provides a name for a context.
**						If the value is NIL, no context symbol is
**						created.
**	add_uid_symbol:		If true, a unique ID symbol is added, otherwise
**						it is not.
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

void sequence::add_ID_symbols(symbol* context_symbol,
	bool add_uid_symbol)
{
	symbol* cs, * first_bracket, * last_bracket, * unique_id_symbol,
		* symbol1;
	char* csn;

	// Create a context symbol, with a name that
	// is either newly-minted or taken from context_symbol.

	if (context_symbol == NIL) cs = NIL;
	else
	{
		csn = context_symbol->get_name();
		cs = new symbol(csn, 1, NULL_VALUE);
		cs->set_status(IDENTIFICATION);
		cs->set_type(CONTEXT_SYMBOL);
		cs->set_bit_cost(context_symbol->get_bit_cost());
	}

	first_bracket =
		make_bracket_symbol(LEFT_BRACKET, BOUNDARY_MARKER, 1);
	this->add_child_at_start(first_bracket);
	last_bracket = make_bracket_symbol(RIGHT_BRACKET, BOUNDARY_MARKER, 1);
	this->add_child(last_bracket);

	// If add_uid_symbol, create a unique id symbol and add
	// it to the pattern.

	if (add_uid_symbol)
	{
		unique_id_symbol = make_unique_id_symbol();
		this->follow(first_bracket, unique_id_symbol);
	}

	// Make a check to see that (a copy of) the cs is not already
	// present in the pattern (see sp70_od, %133). If it is not
	// already present, put the cs after the first bracket.

	if (cs != NIL)
	{
		bool cs_is_already_present = false;
		list_for(symbol1, symbol, this)
		{
			if (symbol1->get_type() == LEFT_BRACKET) continue;
			if (symbol1->get_status() == CONTENTS) break;
			if (symbol1->name_matches(cs))
			{
				cs_is_already_present = true;
				break;
			}
		}

		if (cs_is_already_present == false)
			this->follow(first_bracket, cs);
	}

	this->mark_parent_and_int_positions_non_recursive();
	this->compute_costs();

} // sequence::add_ID_symbols

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Checks whether a newly-formed alignment matches any one formed
**	previously, on this cycle or previous cycles.
**
** CALLING SEQUENCE:
**
**	sequence *sequence::matches_earlier_alignment()
**
** FORMAL ARGUMENTS:
**
**	Return value:	The matching sequence if a match is found,
**					NIL otherwise.
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

sequence* sequence::matches_earlier_alignment()
{
	sequence* al2;
	hit_node* node1;
	int row, leaf_index;

	/* The program searches for matches amongst the sequences
	formed so far in the current cycle. */

	row = 0;
	while (node1 = get_leaf_nodes_in_order(&row, &al2, &leaf_index))
	{
		if (node1->get_compression_difference() == fail_score)
			continue;
		if (al2 == NIL) continue;

		// Because each new sequence is always matched 
		// against sequences formed in the current cycle, it is
		// assumed that there are no other duplicates amongst
		// the new sequences so it is safe to break out of the
		// loop now.

		if (al2->alignment_matches(this)) return(al2);
	}

	// Test whether sequences formed in all previous cycles are
	// to be matched as well.

	// if (!CHECK_ALL_ALIGNMENTS) return(NIL) ;

	// Check whether the newly-formed sequence matches anything
	// produced in previous cycles.

	list_for(al2, sequence, parsing_alignments)
	{
		// Because each new sequence is always matched 
		// against sequences formed in the earlier cycles, it is 
		// assumed that there are no other duplicates amongst the 
		// earlier sequences so it is safe to break out of
		// the loop if a match is found now.

		if (al2->alignment_matches(this)) return(al2);
	}

	return(NIL);
} // matches_earlier_alignment

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Writes out a sequence as a flat pattern without details and as
**	a flat pattern with details. If the pattern is an alignment,
**	the rows of the alignment are written out. The method
**	also writes out any code derived from the alignment or sequence.
**
** CALLING SEQUENCE:
**
**	void sequence::write_out_fully(char *header,
**		hit_node *h_node, int write_section_chars_length,
**		int sections_per_page, bool flat_pattern_and_details,
**		sequence *cnp)
**
** FORMAL ARGUMENTS:
**
**	Return value:					void
**
**	header:							The heading.
**	hit_node:						The hit node in the case of
**									alignments derived from the
**									hit structure. It may be NIL.
**	write_section_chars_length:		The number of character columns
**									that can be written before the
**									alignment must be broken into
**									a new section.
**	sections_per_page:				The number of sections of the
**									alignment that can be written
**									before a new page must be started
**									(shown with a Latex command for
**									use in latex_file).
**									If the value is NULL_VALUE, no
**									page break will be inserted.
**	flat_pattern_and_details:		If true, the alignment is printed
**									as a flat pattern and
**									also with its details. Otherwise,
**									these things are not printed.
**	cnp:							The 'current' New pattern
**
** IMPLICIT INPUTS:
**
**	NONE
**
** IMPLICIT OUTPUTS:
**
**	Output to output_file
**
** SIDE EFFECTS:
**
**	NONE
**
*/

void sequence::write_out_fully(const char* header, hit_node* h_node,
	int write_section_chars_length, int sections_per_page,
	bool flat_pattern_and_details, sequence* cnp)
{
	if (header != NIL)
	{
		fprintf(output_file, "%s%s",
			header,
			" (");
		print_pattern_cycle(true, cnp);
		fprintf(output_file, ")\n\n");
	}

	this->print_ID();

	if (h_node != NIL)
	{
		fprintf(output_file, " : ");
		(h_node->get_driving_pattern())->print_ID();
		fprintf(output_file, " : ");
		(h_node->get_target_pattern())->print_ID();
		fprintf(output_file, " : ");
		h_node->print_ID();
	}

	if (sequence_depth > 1)
	{
		fprintf(output_file,
			"%s%1.2f%s%1.2f%s%1.2f%s%1.2f%s%1.12g%s",
			": NSC = ", this->get_new_symbols_cost(),
			", EC = ", this->get_encoding_cost(),
			", CR = ", this->get_compression_ratio(),
			", CD = ", this->get_compression_difference(),
			",\nAbsolute P = ", this->get_abs_P(),
			"\n\n");

		this->write_alignment(output_file,
			write_section_chars_length, sections_per_page,
			alignment_format);
	}
	else fprintf(output_file, ": ");

	if (flat_pattern_and_details)
	{
		if (sequence_depth > 1)
		{
			fprintf(output_file, "Sequence ");
			this->print_ID();
			fprintf(output_file, " as flat pattern:\n\n");
		}

		this->write_with_details(true);
	}

} // sequence::write_out_fully

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Writes out an alignment of symbols.
**
** CALLING SEQUENCE:
**
**	void sequence::write_alignment_horizontal(FILE *output_file,
**		int write_section_chars_length, int sections_per_page)
**
** FORMAL ARGUMENTS:
**
**	Return value:			void
**
**	output_file:					The output_file or the latex_file.
**	write_section_chars_length:	The number of character columns
**									that can be written before the
**									alignment must be broken into
**									a new section.
**	sections_per_page:				The number of sections of the
**									alignment that can be written
**									before a new page must be started
**									(shown with a Latex command for
**									use in latex_file).
**									If the value is NULL_VALUE, no
**									page break will be inserted.
**
** IMPLICIT INPUTS:
**
**	NONE
**
** IMPLICIT OUTPUTS:
**
**	NONE.
**
** SIDE EFFECTS:
**
**	NONE
*/

void sequence::write_alignment_horizontal(FILE* output_file,
	int write_section_chars_length, int sections_per_page)
{
	symbol* al_col1, * symbol1;
	sequence* pattern1;
	int alignment_depth = this->get_sequence_depth(),
		number_of_children = this->get_number_of_children();
	int s_row1, s_row2, c_row1, c_col1, end_col,
		symbol_length, c_col_for_link, al_char_depth, max_c_col,
		temp_c_col, column_width, s_row_lowest_symbol, al_c_depth;
	char* c, * symbol_name;
	alignment_element* al_el1;
	list_element* temp_el_pos;
	bool highest_symbol_has_been_found, write_char_limit_reached;

	al_c_depth = (alignment_depth * 2) - 1; // Calculate the
	// depth of the alignment in terms of written out characters.

	if (al_c_depth > write_alignment_char_rows)
		abort_run("write_al_chars[][] has too few rows.");

	if (number_of_children >= max_alignment_length)
		abort_run("max_alignment_length has been exceeded");

	/* Clear write_al_chars[][], row_array[] and write_al_vacant_slots[][].
	The first of these arrays will contain the alignment as the
	characters are to be written out. The second one keeps track of
	how far through the alignment (from left to right), the
	process has got. The third one records (with 'x'),
	the character slots which may not contain any non-hit symbol. */

	al_col1 = (symbol*)this->get_first_child();

	if (al_col1 == NIL) abort_run("Anomaly in write_alignment");

	for (s_row1 = 0; s_row1 < alignment_depth; s_row1++)
	{
		row_array[s_row1].row_pattern =
			al_col1->get_row_pattern(s_row1);
		row_array[s_row1].patt_last_el_pos = NIL;
		row_array[s_row1].fe_col_in_write_al_chars = 0;
		row_array[s_row1].pattern_is_finished = false;
		c_row1 = s_row1 * 2;
		for (c_col1 = 0; c_col1 < write_al_chars_length; c_col1++)
		{
			write_al_vacant_slots[s_row1][c_col1] = ' ';
			write_al_chars[c_row1][c_col1] = ' ';
			write_al_chars[c_row1 + 1][c_col1] = ' ';
		}
	}

	// Now fill in characters in write_al_chars[][].

	max_c_col = 0;
	list_for(al_col1, symbol, this)
	{
		// if (!al_col1->is_a_hit()) continue ; // EDIT: line commented out.

		highest_symbol_has_been_found = false;

		// Find the lowest row which contains a symbol.

		s_row_lowest_symbol = NULL_VALUE;

		for (s_row2 = alignment_depth - 1; s_row2 >= 0; s_row2--)
		{
			symbol1 = al_col1->get_row_symbol(s_row2);
			if (symbol1 != NIL)
			{
				s_row_lowest_symbol = s_row2;
				break;
			}
		}

		if (s_row_lowest_symbol == NULL_VALUE)
			abort_run("Anomaly in value of s_row_lowest_symbol");

		// Find the length of the symbol
		// type for this column.

		if (use_abbreviations == LETTERS || use_abbreviations == DIGITS)
			symbol_name = find_symbol_abbreviation(al_col1->
				get_name());
		else symbol_name = al_col1->get_name();

		symbol_length = (int)strlen(symbol_name);
		column_width = symbol_length + 1;

		// For those rows which contain a hit symbol,
		// fill in the non-hit symbols which precede the hit
		// symbol.

		write_char_limit_reached = false;
		for (s_row1 = 0; s_row1 < alignment_depth; s_row1++)
		{
			if (s_row1 > 0) break; // EDIT: line added.
			symbol1 = al_col1->get_row_symbol(s_row1);
			if (symbol1 == NIL) continue;

			// Fill in the non-hit symbols from after
			// the previous hit for this row (or from the
			// beginning of the pattern) up to but
			// not including the current hit_symbol.
			// If necessary, update the value of max_c_col.

			if (fill_in_non_hit_symbols(s_row1, symbol1,
				&temp_c_col))
				write_char_limit_reached = true;

			if (temp_c_col > max_c_col) max_c_col = temp_c_col;
		}

		if (write_char_limit_reached) goto L1;

		// Allowing for the width of the 'hit' column, check that
		// we are still within the bounds of write_al_chars[][].

		if (max_c_col + column_width >= write_al_chars_length)
		{
			write_char_limit_reached = true;
			goto L1;
		}

		// Blank off those sections of write_al_vacant_slots[][]
		// which correspond to a NIL symbol in this column.

		if (al_col1->is_a_hit()) // EDIT: code added.
		{
			end_col = max_c_col + column_width;
			for (s_row1 = 0; s_row1 < alignment_depth; s_row1++)
			{
				symbol1 = al_col1->get_row_symbol(s_row1);
				if (symbol1 == NIL)
				{
					for (c_col1 = max_c_col; c_col1 < end_col; c_col1++)
						write_al_vacant_slots[s_row1][c_col1] = 'x';
				}
			}
		}

		// Now, for each row which contains a hit symbol,
		// fill in the hit symbol. If the hit symbol is the
		// last symbol in that row, mark the row as finished.

		// Calculate the column position of the 'link' symbol ('|')
		// which connects the hit symbols.

		c_col_for_link = max_c_col +
			(column_width - (symbol_length / 2) - 2);

		for (s_row1 = 0; s_row1 <= s_row_lowest_symbol; s_row1++)
		{
			c_row1 = s_row1 * 2;
			al_el1 = al_col1->get_al_el(s_row1);
			symbol1 = (symbol*)al_el1->get_el_obj();
			pattern1 = al_el1->get_original_pattern();

			if (symbol1 == NIL)
			{
				if (!highest_symbol_has_been_found) continue;

				// Fill in a linking symbol for this c_row1
				// and the one below it. We can be sure
				// the lowest symbol has not yet been reached
				// because, if it had, symbol1 would not be
				// equal to NIL.

				write_al_chars[c_row1][c_col_for_link] = '|';
				write_al_chars[c_row1 + 1][c_col_for_link] = '|';
				continue;
			}
			else highest_symbol_has_been_found = true;

			// symbol1 is not NIL. Now fill in the name
			// of the 'hit' symbol for this row. But first,
			// mark this row as finished if the hit symbol
			// is the last symbol in the row.

			temp_el_pos = row_array[s_row1].patt_last_el_pos;
			pattern1->set_current_el_pos(temp_el_pos);
			if (pattern1->this_is_last_child(symbol1))
				row_array[s_row1].pattern_is_finished =
				true;

			c_col1 = max_c_col;
			for (c = symbol_name; *c != '\0'; c++)
				write_al_chars[c_row1][c_col1++] = *c;

			if (s_row1 < s_row_lowest_symbol)
				write_al_chars[c_row1 + 1][c_col_for_link] = '|';

			// Update the value for the first empty column for
			// this row stored in write_al_char_cols[].

			row_array[s_row1].fe_col_in_write_al_chars =
				c_col1 + 1; // Add 1 to allow for the
			// space character following
			// the symbol.
		}

		max_c_col += column_width;
	}

	// Finish off any remaining non-hit symbols in each row of
	// the alignment.

	write_char_limit_reached = false;
	for (s_row1 = 0; s_row1 < alignment_depth; s_row1++)
	{
		if (s_row1 > 0) break; // EDIT: line added.
		if (fill_in_non_hit_symbols(s_row1, NIL, &temp_c_col))
			write_char_limit_reached = true;
		if (temp_c_col > max_c_col) max_c_col = temp_c_col;
	}

L1:;

	if (write_char_limit_reached)
	{
		/* The writing out of the alignment will be
		truncated at this point. The rows which are
		not finished are marked with "...". */

		for (s_row1 = 0; s_row1 < alignment_depth; s_row1++)
		{
			if (row_array[s_row1].pattern_is_finished)
				continue;
			c_col1 = max_c_col;
			c_row1 = s_row1 * 2;
			write_al_chars[c_row1][c_col1++] = '.';
			write_al_chars[c_row1][c_col1++] = '.';
			write_al_chars[c_row1][c_col1++] = '.';
		}
		max_c_col += 3;
	}

	/* Now write out the characters in write_al_chars[][]. */

	// int sections_written = 0 ;
	int first_section_column = 0, last_section_column,
		al_char_length = max_c_col - 1, section_counter = 0;

	// Calculate the number of character rows in the alignment.

	al_char_depth = (alignment_depth * 2) - 1;

	// Adjust the value of write_section_chars_length in the
	// light of the row numbers to be written at the beginning and
	// end of each row. For each number, one or two columns is needed
	// for the number itself, plus one blank column between the
	// number and the alignment.

	int last_row = alignment_depth - 1, cols_for_number;
	if (last_row < 10) cols_for_number = 2;
	else if (last_row >= 10 && last_row < 20) cols_for_number = 3;
	else abort_run("Too many rows in alignment");

	write_section_chars_length -= (cols_for_number * 2);
	if (write_section_chars_length < 10)
		abort_run("write_section_chars_length is too short");

	// Check whether what remains of the alignment will fit across
	// the breadth of the page. If so, set the value of
	// last_section_column to correspond to the last character column
	// of the alignment. If not, find the last blank column
	// in the current section of write_al_chars[] and set the
	// value of last_section_column to point to the column before
	// that blank column.

	while (true)
	{
		last_section_column = first_section_column +
			write_section_chars_length - 1;
		if (last_section_column > al_char_length)
			last_section_column = al_char_length - 1;
		else
		{
			for (c_col1 = first_section_column +
				write_section_chars_length - 1;
				c_col1 >= first_section_column; c_col1--)
			{
				// See whether the column in
				// write_al_chars[] corresponding
				// to c_col1 is blank. If so, break.

				for (c_row1 = 0; c_row1 < al_char_depth;
					c_row1++)
				{
					if (write_al_chars[c_row1][c_col1]
						!= ' ')
						break;
				}

				if (c_row1 >= al_char_depth) break;
			}

			if (c_col1 <= first_section_column)
				abort_run("Anomaly in last_section_column");

			last_section_column = c_col1 - 1;
		}

		// Write out the current section. Write the row number
		// at the beginning and end of each row.

		bool number_required = false;

		s_row1 = 0;

		for (c_row1 = 0; c_row1 < al_char_depth; c_row1++)
		{
			if (number_required == false) number_required = true;
			else number_required = false;

			if (number_required)
			{
				if (cols_for_number == 2)
					fprintf(output_file, "%1d", s_row1);
				else fprintf(output_file, "%2d", s_row1);
				fprintf(output_file, " ");
			}
			else
			{
				if (cols_for_number == 2)
					fprintf(output_file, "  ");
				else fprintf(output_file, "   ");
			}

			for (c_col1 = first_section_column; c_col1 <=
				last_section_column; c_col1++)
			{
				fprintf(output_file, "%c",
					write_al_chars[c_row1][c_col1]);
			}

			if (number_required)
			{
				fprintf(output_file, " ");
				if (cols_for_number == 2)
					fprintf(output_file, "%d%s", s_row1,
						"\n");
				else fprintf(output_file, "%2d%s", s_row1,
					"\n");

				s_row1++;
			}
			else fprintf(output_file, "\n");
		}

		first_section_column = last_section_column + 2;
		if (first_section_column >= al_char_length)
		{
			fprintf(output_file, "\n");
			break;
		}

		if (sections_per_page != NULL_VALUE)
		{
			section_counter++;
			if (section_counter >= sections_per_page)
			{
				fprintf(output_file, "\\newpage\n");
				section_counter = 0;
			}
			else fprintf(output_file, "\n\n");
		}
		else fprintf(output_file, "\n\n");
	}

	fflush(output_file);
} // sequence::write_alignment_horizontal

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Writes out an alignment in 'vertical' format: there is a column
**	for each row in the alignment and symbols are written horizontally.
**
** CALLING SEQUENCE:
**
**	void sequence::write_alignment_vertical(FILE *output_file)
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	output_file:	The file into which the alignment is to be written.
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

void sequence::write_alignment_vertical(FILE* output_file)
{
	struct row_record
	{
		int row_width = { 0 };
		int row_position = { 0 }; // The position of the first character
			// in the column, numbered from the left. The first
			// character is in position 0.
		symbol* next_hit_symbol = NULL;
		list_element* this_el_pos = { NIL };
		list_element* row_el_pos = { NIL };
		sequence* row_pattern = NULL;
		int non_hit_symbols_to_be_written = 0;
		bool new_values_needed = false;
	};

	struct row_record* write_vertical_array;
	char one_line[MEDIUM_SCRATCH_ARRAY_SIZE];

	write_vertical_array = new row_record[sequence_depth];

	// Find the widths and positions of rows. For any row, the width
	// is the length of the longest symbol that will go in the row, plus
	// GAP_BETWEEN_AL_COLS.

	sequence* row_pattern;
	symbol* al_symbol1, * al_symbol2, * row_symbol1, * row_symbol2,
		* next_hit_symbol;
	int longest_name_length = 0, name_length, index1,
		index2, index3, index4, non_hit_symbols_count, size_of_gap;

	write_vertical_array[0].row_position = 0;

	int row, line_length = 0, row_width;
	for (row = 0; row < sequence_depth; row++)
	{
		longest_name_length = 0;
		write_vertical_array[row].row_pattern = row_pattern =
			this->get_row_pattern(row);
		for (row_symbol1 = (symbol*)row_pattern->get_first_child();
			row_symbol1 != NIL; row_symbol1 = (symbol*)row_pattern->
			get_next_child())
		{
			name_length = (int)strlen(row_symbol1->get_name());
			if (name_length > longest_name_length)
				longest_name_length = name_length;
		}

		write_vertical_array[row].row_width = row_width = longest_name_length +
			GAP_BETWEEN_AL_COLS;
		if (row + 1 < sequence_depth)
		{
			write_vertical_array[row + 1].row_position =
				write_vertical_array[row].row_position + row_width;
		}

		line_length += row_width;

		write_vertical_array[row].new_values_needed = true;
		write_vertical_array[row].this_el_pos = NIL;
		write_vertical_array[row].row_el_pos = NIL;
	}

	line_length -= GAP_BETWEEN_AL_COLS; // The last set of blank 
		// characters in the line are not needed.
	if (line_length + 1 >= MEDIUM_SCRATCH_ARRAY_SIZE)
		abort_run("Line is too long in write_alignment_vertical()");

	// Now write out the alignment.

	char* symbol_name, c;
	int max_non_hit_symbols, name_index;
	list_element* this_el_pos1 = NIL, * this_el_pos2, * row_el_pos = NIL;

	list_for_el_pos(al_symbol1, symbol, this, this_el_pos1)
	{
		// Where needed, find the next hit symbol in each row and 
		// the number of non-hit symbols that precede it. If there
		// is no next hit symbol for a given row, count the number
		// of trailing non-hit symbols there are and add that
		// number to write_vertical_array[row].non_hit_symbols_to_be_written.

		for (row = 0; row < sequence_depth; row++)
		{
			if (row > 0) break; // EDIT: line added.
			if (write_vertical_array[row].new_values_needed == false)
				continue;

			next_hit_symbol = NIL;
			this_el_pos2 = write_vertical_array[row].this_el_pos;

			list_for_el_pos(al_symbol2, symbol, this, this_el_pos2)
			{
				if (al_symbol2->is_a_hit() == false) continue;
				next_hit_symbol = al_symbol2->get_row_symbol(row);
				if (next_hit_symbol != NIL) break;
			}

			// We have found the next hit symbol in the row or
			// got to the end of 'this' without finding another
			// hit symbol.

			write_vertical_array[row].next_hit_symbol = next_hit_symbol;
			non_hit_symbols_count = 0;
			row_el_pos = write_vertical_array[row].row_el_pos;
			row_pattern = write_vertical_array[row].row_pattern;
			while (row_symbol1 = (symbol*)row_pattern->
				get_next_child_by_el_pos(&row_el_pos))
			{
				if (row_symbol1 == next_hit_symbol) break;
				non_hit_symbols_count++;
			}

			write_vertical_array[row].non_hit_symbols_to_be_written =
				non_hit_symbols_count;

			write_vertical_array[row].new_values_needed = false;
			write_vertical_array[row].next_hit_symbol = next_hit_symbol;
		}

		// EDIT: next line commented out.
		// if (al_symbol1->is_a_hit() == false) continue ; 

		// Find out the maximum number of non-hit symbols that must
		// be fitted in before the current hit.

		max_non_hit_symbols = 0;
		for (row = 0; row < sequence_depth; row++)
		{
			if (row > 0) break; // EDIT: line added.
			non_hit_symbols_count = 0;
			row_symbol1 = al_symbol1->get_row_symbol(row);
			if (row_symbol1 == NIL) continue;

			// We have a hit symbol in the current row and column.

			row_pattern = write_vertical_array[row].row_pattern;

			row_el_pos = write_vertical_array[row].row_el_pos;
			while (row_symbol2 = (symbol*)row_pattern->
				get_next_child_by_el_pos(&row_el_pos))
			{
				if (row_symbol2 == row_symbol1) break;
				non_hit_symbols_count++;
			}
			write_vertical_array[row].non_hit_symbols_to_be_written =
				non_hit_symbols_count;
			if (non_hit_symbols_count > max_non_hit_symbols)
				max_non_hit_symbols = non_hit_symbols_count;
		}

		// Now write out lines until the available space for non-hit
		// symbols has been used up or there are no more non-hit symbols.

		while (max_non_hit_symbols > 0)
		{
			// Clear one_line[], the array that will hold the
			// characters for one line.

			for (index1 = 0; index1 < line_length; index1++)
				one_line[index1] = ' ';
			one_line[line_length] = '\0';

			// For each row, write out non-hit symbols, provided
			// max_non_hit_symbols has not been reached or available
			// non-hit symbols have been exhausted.

			for (row = 0; row < sequence_depth; row++)
			{
				if (row > 0) break; // EDIT: line added.
				row_el_pos = write_vertical_array[row].row_el_pos;
				row_pattern = write_vertical_array[row].row_pattern;
				row_symbol1 = (symbol*)row_pattern->
					get_next_child_by_el_pos(&row_el_pos);
				if (row_symbol1 == NIL) continue;
				next_hit_symbol = write_vertical_array[row].next_hit_symbol;
				if (write_vertical_array[row].non_hit_symbols_to_be_written <= 0)
					continue;

				// Put characters from row_symbol1 into line1.

				index1 = write_vertical_array[row].row_position;
				name_index = 0;
				symbol_name = row_symbol1->get_name();
				c = symbol_name[name_index++];
				while (c != '\0')
				{
					one_line[index1++] = c;
					c = symbol_name[name_index++];
				}

				(write_vertical_array[row].non_hit_symbols_to_be_written)--;
				write_vertical_array[row].row_el_pos = row_el_pos;
			}

			max_non_hit_symbols--; // Subtract 1 from the 
				// maximum number of non-hit symbols to be written.
			fprintf(output_file, "%s%c", one_line, '\n');
		}

		// Now write out the hit symbols in the current column.

		// Clear one_line[], the array that will hold the
		// characters for one line.

		for (index1 = 0; index1 < line_length; index1++)
			one_line[index1] = ' ';
		one_line[line_length] = '\0';

		for (row = 0; row < sequence_depth; row++)
		{
			row_symbol1 = al_symbol1->get_row_symbol(row);
			if (row_symbol1 == NIL) continue;

			// Put characters from row_symbol1 into line1.

			index1 = write_vertical_array[row].row_position;
			name_index = 0;
			symbol_name = row_symbol1->get_name();
			c = symbol_name[name_index++];
			while (c != '\0')
			{
				one_line[index1++] = c;
				c = symbol_name[name_index++];
			}

			write_vertical_array[row].row_el_pos = row_el_pos;
			row_pattern = write_vertical_array[row].row_pattern;
			write_vertical_array[row].this_el_pos = this_el_pos1;
			write_vertical_array[row].new_values_needed = true;
			write_vertical_array[row].row_el_pos =
				row_pattern->get_el_pos_by_child(row_symbol1);
		}

		// Put in dashes to link hit symbols in the current line.
		// First scan across to the first non-blank character.

		for (index2 = 0; index2 < line_length; index2++)
			if (one_line[index2] != ' ') break;

		if (index2 >= line_length)
			abort_run("Blank line in write_alignment_vertical()");

		for (index1 = index2; index1 < line_length; index1++)
		{
			if (one_line[index1] != ' ') continue;

			// We have a blank character in one_line[].

			index2 = index1;

			// Scan ahead to the next non-blank character, if any.

			size_of_gap = 1;
			index2++;

			while (index2 < line_length)
			{
				if (one_line[index2] != ' ') break;
				index2++;
				size_of_gap++;
			}

			if (index2 >= line_length) break;

			if (size_of_gap == 1) one_line[index1] = '.';
			else if (size_of_gap == 2)
			{
				one_line[index1] = '.';
				one_line[index1 + 1] = '.';
			}
			else
			{
				index3 = index1 + 1;
				index4 = index2 - 1;

				while (index3 < index4)
				{
					one_line[index3] = '-';
					index3++;
				}
			}
			index1 = index2;
		}

		fprintf(output_file, "%s%c", one_line, '\n');
	}

	// Do trailing non-hit symbols (if any). First find out the
	// maximum number of trailing non-hit symbols in any row.

	max_non_hit_symbols = 0;
	for (row = 0; row < sequence_depth; row++)
	{
		if (row > 0) break; // EDIT: line added.
		row_el_pos = write_vertical_array[row].row_el_pos;
		row_pattern = write_vertical_array[row].row_pattern;
		non_hit_symbols_count = 0;
		while (row_symbol1 = (symbol*)row_pattern->
			get_next_child_by_el_pos(&row_el_pos))
			non_hit_symbols_count++;
		write_vertical_array[row].non_hit_symbols_to_be_written =
			non_hit_symbols_count;
		if (non_hit_symbols_count > max_non_hit_symbols)
			max_non_hit_symbols = non_hit_symbols_count;
	}

	while (max_non_hit_symbols > 0)
	{
		// Clear one_line[], the array that will hold the
		// characters for one line.

		for (index1 = 0; index1 < line_length; index1++)
			one_line[index1] = ' ';
		one_line[line_length] = '\0';

		// For each row, write out non-hit symbols if there are
		// any left.

		for (row = 0; row < sequence_depth; row++)
		{
			if (write_vertical_array[row].non_hit_symbols_to_be_written <= 0)
				continue;
			row_el_pos = write_vertical_array[row].row_el_pos;
			row_pattern = write_vertical_array[row].row_pattern;
			row_symbol1 = (symbol*)row_pattern->
				get_next_child_by_el_pos(&row_el_pos);

			// Put characters from row_symbol1 into line1.

			index1 = write_vertical_array[row].row_position;
			name_index = 0;
			symbol_name = row_symbol1->get_name();
			c = symbol_name[name_index++];
			while (c != '\0')
			{
				one_line[index1++] = c;
				c = symbol_name[name_index++];
			}

			(write_vertical_array[row].non_hit_symbols_to_be_written)--;
			write_vertical_array[row].row_el_pos = row_el_pos;
		}
		fprintf(output_file, "%s%c", one_line, '\n');
		max_non_hit_symbols--;
	}

	fprintf(output_file, "\n");

	delete[] write_vertical_array;

} // sequence::write_alignment_vertical

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Writes out the symbols in a pattern with details of
**	the type, status and other information about symbols.
**
** CALLING SEQUENCE:
**
**	void sequence::write_with_details(bool write_from)
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	write_from:		true if '(from alignment ID)' is to be written,
**					otherwise false.
**
** IMPLICIT INPUTS:
**
**	NONE
**
** IMPLICIT OUTPUTS:
**
**	Output of the pattern with details of symbols
**
** SIDE EFFECTS:
**
**	NONE
**
*/

void sequence::write_with_details(bool write_from)
{
	symbol* symbol1;
	int counter = 0, symbol_frequency, tp;
	char type[SMALL_SCRATCH_ARRAY_SIZE],
		status[SMALL_SCRATCH_ARRAY_SIZE];

	write_pattern(true, write_from);

	if (sequence_depth > 1) // The following details are to be written
		// out only if 'this' pattern is an alignment.
	{
		fprintf(output_file,
			"%s%1.2f%s%1.2f%s%1.2f%s%1.2f%s",
			"NSC = ",
			new_symbols_cost,
			", EC = ",
			encoding_cost,
			", CR = ",
			compression_ratio,
			", CD = ",
			compression_difference,
			"\n\n");
	}

	list_for(symbol1, symbol, this)
	{
		counter++;
		if (counter >= 3)
		{
			fprintf(output_file, "\n");
			counter = 1;
		}

		tp = symbol1->get_type();

		if (tp == CONTEXT_SYMBOL) strcpy(type, "CS");
		else if (tp == LEFT_BRACKET) strcpy(type, "LB");
		else if (tp == RIGHT_BRACKET) strcpy(type, "RB");
		else if (tp == UNIQUE_ID_SYMBOL) strcpy(type, "UID");
		else strcpy(type, "DATA");

		if (symbol1->get_status() == IDENTIFICATION)
			strcpy(status, "ID");
		else if (symbol1->get_status() == BOUNDARY_MARKER)
			strcpy(status, "BM");
		else strcpy(status, "CN");

		symbol_frequency = symbol1->get_frequency();

		fprintf(output_file, "%s%s%d%s%1.2f%s%s%s%s",
			symbol1->get_name(),
			" (fr = ", symbol_frequency, ", ",
			symbol1->get_bit_cost(),
			", ", type,
			", ", status);

		if (this->this_is_last_child(symbol1))
			fprintf(output_file, "%s%d%s",
				"), frequency = ",
				frequency,
				".\n\n");
		else fprintf(output_file, "), ");
	}
} // sequence::write_with_details

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Tests whether the CONTENTS symbols in one sequence of symbols
**	match the CONTENTS symbols in another sequence.
**
** CALLING SEQUENCE:
**
**	bool sequence::contents_symbols_match(sequence *pattern1)
**
** FORMAL ARGUMENTS:
**
**	Return value:	If the CONTENTS symbols match, true, otherwise false.
**
**	pattern1:		The sequence to be matched with 'this'.
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

bool sequence::contents_symbols_match(sequence* pattern1)
{
	symbol* symbol1, * symbol2;

	symbol2 = (symbol*)this->get_first_child();
	for (symbol1 = (symbol*)pattern1->get_first_child();
		symbol1 != NIL; symbol1 = (symbol*)pattern1->get_next_child())
	{
		if (symbol1->get_status() != CONTENTS) continue;
		if (symbol2 == NIL) return(false);

		while (symbol2->get_status() != CONTENTS)
		{
			symbol2 = (symbol*)this->get_next_child();
			if (symbol2 == NIL) return(false);
		}

		if (symbol1->name_matches(symbol2))
		{
			symbol2 = (symbol*)this->get_next_child();
			continue;
		}
		else return(false);
	}

	// pattern1 has been completely stepped through. Check to see
	// whether or not there are any CONTENTS symbols left in 'this'.

	if (symbol2 != NIL)
	{
		if (symbol2->get_status() == CONTENTS) return(false);
		while (symbol2 = (symbol*)this->get_next_child())
			if (symbol2->get_status() == CONTENTS) return(false);
	}

	return(true);
} // sequence::contents_symbols_match

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Checks 'this' pattern against pre-existing patterns
**	(in old_patterns) to see whether there is one
**	that has all and only the same CONTENTS symbols in the
**	same order. If there is, that sequence is returned.
**	Otherwise, the method returns NIL.
**
** CALLING SEQUENCE:
**
**	sequence *sequence::check_patterns()
**
** FORMAL ARGUMENTS:
**
**	Return value:	A pre-existing pattern with all and only the
**					same CONTENTS symbols, or NIL if no such pattern
**					exists.
**
** IMPLICIT INPUTS:
**
**	patterns in old_patterns.
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

sequence* sequence::check_patterns()
{
	sequence* pattern1;
	list_element* el_pos1 = NIL;
	bool pattern_found = false;

	for (pattern1 = (sequence*)old_patterns->
		get_next_child_by_el_pos(&el_pos1);
		pattern1 != NIL; pattern1 = (sequence*)old_patterns->
		get_next_child_by_el_pos(&el_pos1))
	{
		if (this->contents_symbols_match(pattern1))
		{
			pattern_found = true;
			break;
		}
	}

	if (pattern_found)
	{
		fprintf(output_file, "Contents-symbol match found between ");
		this->write_pattern(false, true);
		fprintf(output_file, " and ");
		pattern1->write_pattern(true, true);
		return(pattern1);
	}

	return(NIL);
} // sequence::check_patterns

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Calculates the length of an alignment.
**
** CALLING SEQUENCE:
**
**	int sequence::compute_character_length()
**
** FORMAL ARGUMENTS:
**
**	Return value:	The length of the alignment in characters, including
**					The white spaces between symbols.
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

int sequence::compute_character_length()
{
	int char_length = 0;
	symbol* symbol1;
	list_element* el_pos1 = NIL;
	char* symbol_name;

	while (symbol1 = (symbol*)this->get_next_child_by_el_pos(&el_pos1))
	{
		if (use_abbreviations == LETTERS || use_abbreviations == DIGITS)
			symbol_name = find_symbol_abbreviation(symbol1->
				get_name());
		else symbol_name = symbol1->get_name();

		char_length += (int)strlen(symbol_name) + 1; // Add 1
		// to the length of the symbol name for the
		// blank space between one column and the next.
	}

	return(char_length - 1); // Subtract 1 from char_length because
				// the blank character after the last symbol is not needed.
} // sequence::compute_character_length

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Writes a textual description of the sequence specified to
**	the `write file'. This is called from write_tree_object()
**	and wto().
**
** CALLING SEQUENCE:
**
**	void sequence::wto(int print_code) ;
**
** FORMAL ARGUMENTS:
**
**	Return value:	None.
**
**	print_code:		PRINT_ALL_FREQUENCIES if the frequencies
**					of the object and all its constituents
**					are to be printed,
**	print_code:		PRINT_ALL_FREQUENCIES if the frequencies
**					of the object and all its constituents
**					are to be printed,
**					PRINT_SEQUENCE_FREQUENCY if symbol
**					frequencies are to be suppressed.
**					PRINT_SEQUENCE_FREQUENCY otherwise.
**
** IMPLICIT INPUTS:
**
**	NONE
**
** IMPLICIT OUTPUTS:
**
**	output_file
**
** SIDE EFFECTS:
**
**	NONE
*/

void sequence::wto(int print_code)
{
	base_object* child;
	char buffer[STRING_LENGTH];

	write_lines(output_file, "(", CONTINUE);

	this->initialise();
	while (child = this->get_next_child())
	{
		child->wto(print_code);
		if (current_el_pos->get_next() != NIL)
			write_lines(output_file, " ", CONTINUE);
	}
	write_lines(output_file, ")", CONTINUE);
	// if (print_code == PRINT_SEQUENCE_FREQUENCY && frequency != 1)
	if (print_code == PRINT_SEQUENCE_FREQUENCY)
	{
		sprintf(buffer, "*%d", frequency);
		write_lines(output_file, buffer, CONTINUE);
		return;
	}
	// if (print_code == PRINT_ALL_FREQUENCIES && frequency != 1)
	if (print_code == PRINT_ALL_FREQUENCIES)
	{
		sprintf(buffer, "*%d", frequency);
		write_lines(output_file, buffer, CONTINUE);
	}
} /* sequence::wto */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Determines whether or not 'this' ID-symbol has any function within
**	old_patterns, either as a reference from another pattern, or
**	as a discrimination symbol that distinguishes a member of a
**	context class or as a top-level identifier for a pattern.
**
** CALLING SEQUENCE:
**
**	bool symbol::has_no_function()
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if 'this' symbol has no function, false otherwise.
**
** IMPLICIT INPUTS:
**
**	old_patterns
**
** IMPLICIT OUTPUTS:
**
**	NONE
**
** SIDE EFFECTS:
**
**	NONE
*/

bool symbol::has_no_function()
{
	// Look to see whether 'this' symbol is referenced from within
	// the CONTENTS symbols of any Old pattern.

	symbol* symbol1;
	sequence* pattern1;
	list_element* el_pos1, * el_pos2;
	int symbol_type, status, counter;
	bool this_symbol_or_copy_found;

	// Look to see whether 'this' symbol is the only ID-symbol
	// for any one pattern. If so, it is needed as the top-level
	// identifier for the pattern and should not be removed.

	// At the same time, look to see whether 'this' symbol has
	// been referenced from any pattern. If it has, it is needed
	// should not be removed.

	el_pos1 = NIL;
	list_for_el_pos(pattern1, sequence, old_patterns, el_pos1)
	{
		el_pos2 = NIL;
		counter = 0;
		this_symbol_or_copy_found = false;
		list_for_el_pos(symbol1, symbol, pattern1, el_pos2)
		{
			status = symbol1->get_status();
			if (status == IDENTIFICATION)
			{
				if (symbol1 == this || name_matches(symbol1))
					this_symbol_or_copy_found = true;
				counter++;
			}

			if (status != CONTENTS) continue;
			symbol_type = symbol1->get_type();
			if (symbol_type == LEFT_BRACKET
				|| symbol_type == RIGHT_BRACKET)
				continue;

			// symbol1 is a 'reference' symbol.

			if (name_matches(symbol1))
				return(false); // 'this' symbol has been referenced
					// so this->has_no_function() is false.
		}

		if (this_symbol_or_copy_found
			&& counter == 1) // 'this' symbol, or a copy of it,
				// is the only ID-symbol for pattern1
				// so this->has_no_function() is false.
			return(false);
	}

	return(true);

} // symbol::has_no_function

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	This is like add_child but checks that the child to be added
**	does not duplicate any child already added (meaning the identical
**	object, not merely one with a matching name). If the proposed
**	addition is the same as a child already added, the proposed addition
**	is not added.
**
** CALLING SEQUENCE:
**
**	bool group::add_pattern_without_duplicates(sequence *seq1)
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if child added, false otherwise.
**
**	child:			The new base_object to be added.
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
**	NONE. Notice, in particular, that the position of current_el_pos
**	is not changed by adding a new child to an object.
*/

bool group::add_pattern_without_duplicates(sequence* seq1)
{
	list_element* temp1, * temp2, * next;
	base_object* existing_sequence;

	if (seq1 == NIL)
		abort_run("NIL seq1 in \
			group::add_pattern_without_duplicates()");
	temp2 = first_element;
	if (temp2 == NIL)
	{
		temp1 = new list_element(seq1);
		first_element = temp1;
		this->mark_parent_and_int_positions_non_recursive(); // take 
		// care! This alters the value of the 'parent' field.
		return(true);
	}
	else
	{
		existing_sequence = temp2->get_el_obj();
		if (existing_sequence == seq1) return(false);
	}

	while (next = temp2->get_next())
	{
		existing_sequence = next->get_el_obj();
		if (existing_sequence == seq1) return(false);
		temp2 = next;
	}

	temp1 = new list_element(seq1);
	temp2->set_next(temp1);
	this->mark_parent_and_int_positions_non_recursive(); // take 
		// care! This alters the value of the 'parent' field.
	return(true);
} /* group::add_pattern_without_duplicates */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	This is like add_child but checks that the child to be added
**	is not a copy of any child already added. If the proposed
**	addition is a copy of a child already added, the proposed addition
**	is not added. If the proposed addition is identical to a child already
**	added, the proposed addition is not added.
**
** CALLING SEQUENCE:
**
**	bool group::add_pattern_without_copies_or_duplicates(sequence *seq1)
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if child is added, false otherwise.
**
**	child:			The new base_object to be added.
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
**	NONE. Notice, in particular, that the position of current_el_pos
**	is not changed by adding a new child to an object.
*/

bool group::add_pattern_without_copies_or_duplicates(sequence* seq1)
{
	list_element* temp1, * temp2, * next;
	sequence* existing_sequence;

	if (seq1 == NIL)
		abort_run("NIL seq1 in \
			group::add_pattern_without_copies_or_duplicates().");
	temp2 = first_element;
	if (temp2 == NIL)
	{
		temp1 = new list_element(seq1);
		first_element = temp1;
		this->mark_parent_and_int_positions_non_recursive(); // take 
		// care! This alters the value of the 'parent' field.
		return(true);
	}
	else
	{
		existing_sequence = (sequence*)temp2->get_el_obj();
		if (existing_sequence == seq1) return(false);
		if (existing_sequence->is_copy_of(seq1)) return(false); // Notice that this
			// test must be done after the previous one because is_copy_of()
			// flags an error if the two things being compared are the
			// same entity.
	}

	while (next = temp2->get_next())
	{
		existing_sequence = (sequence*)next->get_el_obj();
		if (existing_sequence == seq1) return(false);
		if (existing_sequence->is_copy_of(seq1)) return(false); // Notice that this
			// test must be done after the previous one because is_copy_of()
			// flags an error if the two things being compared are the
			// same entity.
		temp2 = next;
	}

	temp1 = new list_element(seq1);
	temp2->set_next(temp1);
	this->mark_parent_and_int_positions_non_recursive(); // take 
		// care! This alters the value of the 'parent' field.
	return(true);
} /* group::add_pattern_without_copies_or_duplicates */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	This is like add_child but checks that the child to be added
**	is not a copy of any child already added. If the proposed
**	addition is a copy of a child already added, the proposed addition
**	is not added. If the proposed addition is identical to a child already
**	added, the proposed addition is not added.
**
** CALLING SEQUENCE:
**
**	bool group::add_symbol_without_copies_or_duplicates(symbol *symbol1)
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if symbol1 is added, false otherwise.
**
**	symbol1:		The new symbol to be added.
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
**	NONE. Notice, in particular, that the position of current_el_pos
**	is not changed by adding a new child to an object.
*/

bool group::add_symbol_without_copies_or_duplicates(symbol* symbol1)
{
	list_element* temp1, * temp2, * next;
	symbol* existing_symbol;

	if (symbol1 == NIL)
		abort_run("NIL symbol1 in \
			group::add_symbol_without_copies_or_duplicates().");
	temp2 = first_element;
	if (temp2 == NIL)
	{
		temp1 = new list_element(symbol1);
		first_element = temp1;
		this->mark_parent_and_int_positions_non_recursive(); // take 
		// care! This alters the value of the 'parent' field.
		return(true);
	}
	else
	{
		existing_symbol = (symbol*)temp2->get_el_obj();
		if (existing_symbol == symbol1) return(false);
		if (existing_symbol->name_matches(symbol1)) return(false);
	}

	while (next = temp2->get_next())
	{
		existing_symbol = (symbol*)next->get_el_obj();
		if (existing_symbol == symbol1) return(false);
		if (existing_symbol->name_matches(symbol1)) return(false);
		temp2 = next;
	}

	temp1 = new list_element(symbol1);
	temp2->set_next(temp1);
	this->mark_parent_and_int_positions_non_recursive(); // take 
		// care! This alters the value of the 'parent' field.
	return(true);
} /* group::add_symbol_without_copies_or_duplicates */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Checks to see whether the group contains any duplicated references
**	to any object. It is assumed that the objects are sequences.
**
** CALLING SEQUENCE:
**
**	bool group::contains_duplicates()
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if the group contains at least one duplicate,
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
*/

bool group::contains_duplicates()
{
	list_element* el_pos1 = NIL, * el_pos2;
	sequence* child1, * child2;

	list_for_el_pos(child1, sequence, this, el_pos1)
	{
		el_pos2 = el_pos1;
		list_for_el_pos(child2, sequence, this, el_pos2)
		{
			if (child1 == child2)
			{
				fprintf(output_file, "%s%d%s%d%s",
					"In group ID",
					ID,
					" sequence ID",
					child1->get_ID(),
					" is duplicated\n\n");
				return(true);
			}
		}
	}

	return(false);
} // group::contains_duplicates

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Inserts an sequence into group_of_patterns in so that
**	sequences are in order of their CD.
**
** CALLING SEQUENCE:
**
**	void group::insert_sequence_in_order_of_CD(sequence *al1)
**
** FORMAL ARGUMENTS:
**
**	Return value:		void
**
**	al1:				The sequence to be inserted.
**
** IMPLICIT INPUTS:
**
**	NONE
**
** IMPLICIT OUTPUTS:
**
**	'this' with addition of al1.
**
** SIDE EFFECTS:
**
**	NONE
**
*/

void group::insert_sequence_in_order_of_CD(sequence* al1)
{
	sequence* existing_sequence;
	double al_CD = al1->get_compression_difference();

	list_for(existing_sequence, sequence, this)
	{
		if (al_CD < existing_sequence->get_compression_difference())
			continue;

		this->precede(al1, existing_sequence);

		return;
	}

	/* If the program gets this far, the al1 has a larger
	CR than any of the existing sequences so it should be added
	at the end of the set. */

	this->add_child(al1);

} /* group::insert_sequence_in_order_of_CD */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Writes out IDs of members. If a sequence is 'new_this_cycle', it
**	is marked with an asterisk ('*').
**
** CALLING SEQUENCE:
**
**	void group::write_IDs(int selection, sequence *cnp)
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	selection:		Used to choose a subset of the group. NULL_VALUE
**					means that all alignments are selected. Other valid
**					values are FULL_A, FULL_B, FULL_C and PARTIAL.
**	cnp:			The New pattern being processed
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

void group::write_IDs(int selection, sequence* cnp)
{
	int counter1 = 0, counter2 = 0;
	sequence* pattern1;

	// Count the number of IDs to be printed.

	if (selection == NULL_VALUE)
		counter2 = count_number_of_children();
	else
	{
		list_for(pattern1, sequence, this)
		{
			if (selection == pattern1->get_degree_of_matching())
				counter2++;
		}
	}

	// Now print the IDs.

	int counter3 = 0;
	list_for(pattern1, sequence, this)
	{
		if (selection != pattern1->get_degree_of_matching()) continue;
		counter3++;
		fprintf(output_file, "%s%d",
			"ID",
			pattern1->get_ID());

		if (pattern1->is_new_this_cycle())
			fprintf(output_file, "*");

		if (counter3 == counter2)
		{
			fprintf(output_file, "\n\n");
			break;
		}

		if (++counter1 == 10)
		{
			fprintf(output_file, ",\n");
			counter1 = 0;
		}
		else fprintf(output_file, ", ");
	}
} // group::write_IDs

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Checks whether two groups contain the same objects, not
**	necessarily in the same order.
**
** CALLING SEQUENCE:
**
**	bool group::group_matches(group gr1)
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if 'this' contains the same objects as gr1,
**					otherwise false.
**
**	gr1:			The group to be compared with 'this'.
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

bool group::group_matches(group* gr1)
{
	list_element* pos1 = NIL, * pos2;
	base_object* obj1, * obj2;
	bool match_found;

	if (this->count_number_of_children() !=
		gr1->count_number_of_children())
		return(false);

	list_for_el_pos(obj1, base_object, this, pos1)
	{
		pos2 = NIL;
		match_found = false;
		list_for_el_pos(obj2, base_object, gr1, pos2)
			if (obj1 == obj2)
			{
				match_found = true;
				break;
			}
		if (match_found == false) return(false);
	}
	return(true);
} // bool group::group_matches

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Writes out the patterns in a set of patterns, together with
**	an introductory heading.
**
** CALLING SEQUENCE:
**
**	void group::write_patterns(char *heading, int origin, int selection)
**
** FORMAL ARGUMENTS:
**
**	Return value:		void
**
**	heading:			The heading for the set. If the value is NIL,
**						no heading is printed.
**	origin:				Only patterns with the specified origin
**						are to be printed. If origin is NULL_VALUE,
**						all the patterns in the set_of_patterns
**						are to be written.
**	selection:			If NULL_VALUE, all patterns are written, otherwise
**						Other valid values are FULL_A, FULL_B,
**						FULL_C or PARTIAL.
**
** IMPLICIT INPUTS:
**
**	NONE
**
** IMPLICIT OUTPUTS:
**
**	Output to output_file.
**
** SIDE EFFECTS:
**
**	NONE
**
*/

void group::write_patterns(const char* heading, int origin, int selection)
{
	sequence* pattern1;
	list_element* el_pos1 = NIL;

	if (heading != NIL) fprintf(output_file, "%s%s", heading, "\n\n");

	list_for_el_pos(pattern1, sequence, this, el_pos1)
	{
		if (origin != NULL_VALUE && origin != pattern1->get_origin())
			continue;
		if (selection != NULL_VALUE)
		{
			if (selection != pattern1->get_degree_of_matching())
				continue;
		}

		pattern1->print_ID();
		fprintf(output_file, ": ");
		pattern1->write_tree_object(PRINT_SEQUENCE_FREQUENCY);
		fflush(output_file);
	}
} // group::write_patterns

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Write New or Old. Called from write_new_and_old.
**
** CALLING SEQUENCE:
**
**	void group::write_patterns_with_details()
**
** FORMAL ARGUMENTS:
**
**	Return value:		void
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

void group::write_patterns_with_details()
{
	sequence* pattern1;

	this->initialise();
	while (pattern1 = (sequence*)this->get_next_child())
	{
		pattern1->print_ID();

		if (this == old_patterns)
			fprintf(output_file, "%s%1.2f%s",
				" Coding cost: ",
				pattern1->get_encoding_cost(),
				", ");
		else if (this == new_patterns)
			fprintf(output_file, " No coding cost ");
		pattern1->write_tree_object(PRINT_ALL_FREQUENCIES);
		pattern1->write_with_details(true);
	}
} // group::write_patterns_with_details

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Sorts a group of patterns in descending order of their
**	compression_difference scores.
**
** CALLING SEQUENCE:
**
**	void group::sort_by_compression_difference()
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
**
*/

void group::sort_by_compression_difference()
{
	group* temp_group = new group;
	sequence* pattern1;
	sequence* best_pattern = NIL;
	double highest_compression_difference,
		pattern_compression_difference;

	while (pattern1 = (sequence*)this->extract_first_child())
		temp_group->add_child(pattern1);

	while (temp_group->is_empty() == false)
	{
		highest_compression_difference = LOW_VALUE;
		list_for(pattern1, sequence, temp_group)
		{
			pattern_compression_difference =
				pattern1->get_compression_difference();
			if (pattern_compression_difference >
				highest_compression_difference)
			{
				highest_compression_difference =
					pattern_compression_difference;
				best_pattern = pattern1;
			}
		}
		temp_group->extract_child(best_pattern);
		this->add_child(best_pattern);
	}

	delete temp_group;
} // group::sort_by_compression_difference

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Sorts a group of patterns in ascending order of ID number.
**
** CALLING SEQUENCE:
**
**	void group::sort_by_ID()
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
**	The sorted group
**
** SIDE EFFECTS:
**
**	NONE
**
*/

void group::sort_by_ID()
{
	group* temp_group = new group;
	sequence* pattern1;
	sequence* best_pattern = NIL;
	int lowest_ID, pattern_ID;

	while (pattern1 = (sequence*)this->extract_first_child())
		temp_group->add_child(pattern1);

	while (temp_group->is_empty() == false)
	{
		lowest_ID = HIGH_VALUE;
		list_for(pattern1, sequence, temp_group)
		{
			pattern_ID = pattern1->get_ID();
			if (pattern_ID < lowest_ID)
			{
				lowest_ID = pattern_ID;
				best_pattern = pattern1;
			}
		}
		temp_group->extract_child(best_pattern);
		this->add_child(best_pattern);
	}

	delete temp_group;
} // group::sort_by_ID

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Compiles a list of the different symbol types in the patterns
**	in the group 'this' and measures the size of this alphabet.
**
** CALLING SEQUENCE:
**
**	group *group::compile_alphabet(int *alphabet_size)
**
** FORMAL ARGUMENTS:
**
**	Return value:	A group containing the list of symbols.
**
**	alphabet_size:	A pointer to a variable to hold the size of
**					the alphabet.
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

group* group::compile_alphabet(int* alphabet_size)
{
	group* symbol_set = this->symbol_set();
	symbol_set->sort_by_name(); // Puts symbols 
		// in symbol_set in alpha-numeric order of their
		// name fields.
	*alphabet_size = symbol_set->count_number_of_children();
	return(symbol_set);
} // compile_alphabet

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Scans a list of patterns in 'this' and, for each symbol in each
**	pattern, adds the frequency of the pattern to the frequency of
**	the symbol type, stored in symbol_set.
**
** CALLING SEQUENCE:
**
**	void group::compile_frequencies(group *symbol_set,
**		sequence *last_pattern)
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	symbol_set:		The alphabet of symbol types in the group.
**	last_pattern:	The last pattern in the group to be processed. If the
**					value is NIL, all patterns are to be processed.
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

void group::compile_frequencies(group* symbol_set,
	sequence* last_pattern)
{
	sequence* pattern1;
	symbol* symbol1, * symbol_type;

	list_for(pattern1, sequence, this)
	{
		list_for(symbol1, symbol, pattern1)
		{
			/* Scan through symbol_set to find the
			symbol there (representing a symbol type)
			which matches the current symbol. When it
			is found, update the frequency. */

			symbol_set->initialise();
			while (symbol_type = (symbol*)
				symbol_set->get_next_child())
			{
				if (strcmp(symbol1->get_name(),
					symbol_type->get_name()) == 0)
				{
					symbol_type->
						add_to_frequency(pattern1->get_frequency());
					break;
				}
			}
			if (symbol_type == NIL)
				abort_run("symbol_type not found in \
					group::compile_frequencies");
		}
		if (pattern1 == last_pattern) break;
	}
} // group::compile_frequencies

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Writes a textual description of the group specified to
**	the `write file'. This is called from write_tree_object()
**	and wto().
**
** CALLING SEQUENCE:
**
**	void group::wto(int print_code) ;
**
** FORMAL ARGUMENTS:
**
**	Return value:	None.
**
**	print_code:		PRINT_ALL_FREQUENCIES if the frequencies
**					of the object and all its constituents
**					are to be printed,
**					PRINT_SEQUENCE_FREQUENCY if symbol
**					frequencies are to be suppressed.
**					PRINT_SEQUENCE_FREQUENCY otherwise.
**
** IMPLICIT INPUTS:
**
** IMPLICIT OUTPUTS:
**
**	output_file
**
** SIDE EFFECTS:
**
**	NONE
*/

void group::wto(int print_code)
{
	base_object* child;
	char buffer[STRING_LENGTH];

	write_lines(output_file, "[", CONTINUE);

	this->initialise();
	while (child = this->get_next_child())
	{
		child->wto(print_code);
		if (current_el_pos != NIL)
			write_lines(output_file, " ", CONTINUE);
	}
	write_lines(output_file, "]", CONTINUE);
	if (print_code == PRINT_SEQUENCE_FREQUENCY) return;
	if (print_code == PRINT_GROUP_FREQUENCIES && frequency != 1)
	{
		sprintf(buffer, "*%d", frequency);
		write_lines(output_file, buffer, CONTINUE);
		return;
	}
	if (print_code == PRINT_ALL_FREQUENCIES && frequency != 1)
	{
		sprintf(buffer, "*%d", frequency);
		write_lines(output_file, buffer, CONTINUE);
	}
} /* group::wto */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Writes a textual description of the group specified to
**	the `write file'. This is called from write_tree_object()
**	and wto().
**
** CALLING SEQUENCE:
**
**	void symbol::wto(int print_code) ;
**
** FORMAL ARGUMENTS:
**
**	Return value:	None.
**
**	print_code:		PRINT_ALL_FREQUENCIES if the frequencies
**					of the object and all its constituents
**					are to be printed,
**					PRINT_SEQUENCE_FREQUENCY if symbol
**					frequencies are to be suppressed.
**					PRINT_SEQUENCE_FREQUENCY otherwise.
**
** IMPLICIT INPUTS:
**
** IMPLICIT OUTPUTS:
**
**	output_file
**
** SIDE EFFECTS:
**
**	NONE
*/

void symbol::wto(int print_code)
{
	char buffer[STRING_LENGTH];

	write_lines(output_file, name, CONTINUE);

	if (print_code == PRINT_SEQUENCE_FREQUENCY) return;

	if (print_code == PRINT_ALL_FREQUENCIES && frequency != 1)
	{
		sprintf(buffer, "*%d", frequency);
		write_lines(output_file, buffer, CONTINUE);
	}
} /* symbol::wto */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Writes a base_object to a file. Using wto(),
**	it keeps track of the numbers of characters in a line and
**	takes a new line whenever that is needed to avoid splitting
**	a symbol or a frequency value.
**
** CALLING SEQUENCE:
**
**	void tree_object::write_tree_object(int print_code)
**
** FORMAL ARGUMENTS:
**
**	Return value:	NONE
**
**	print_code:		PRINT_ALL_FREQUENCIES if the frequencies
**					of the object and all its constituents
**					are to be printed,
**					PRINT_SEQUENCE_FREQUENCY if SYMBOL frequencies are
**					to be suppressed.
**					PRINT_SEQUENCE_FREQUENCY otherwise.
**
** IMPLICIT INPUTS:
**
**	NONE
**
** IMPLICIT OUTPUTS:
**
**	Text in the write file.
**
** SIDE EFFECTS:
**
**	NONE
*/

void tree_object::write_tree_object(int print_code)
{
	write_lines(output_file, "", START);
	this->wto(print_code);
	fprintf(output_file, "\n\n");
	fflush(output_file);
} /* tree_object::write_tree_object */

/***********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Checks whether a set contains a symbol which
**	matches a parameter symbol.
**
** CALLING SEQUENCE:
**
**	symbol *tree_object::contains_copy_of(symbol *symbol1)
**
** FORMAL ARGUMENTS:
**
**	Return value:	The first item in the set which matches symbol1 or
**					NIL if no matching item is found.
**
**	symbol1:		The data node to be matched to items in the set.
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

symbol* tree_object::contains_copy_of(symbol* symbol1)
{
	symbol* symbol_in_set;

	/* Run over set until symbol1 is found or end of set reached */

	this->initialise();
	while (symbol_in_set = (symbol*)this->get_next_child())
		if (symbol1->name_matches(symbol_in_set))
			return(symbol_in_set);

	return(NIL);
} /* tree_object::contains_copy_of */

/***********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Returns the number of child nodes in a composite data node and
**	set that value in number_of_children.
**
** CALLING SEQUENCE:
**
**	int tree_object::count_number_of_children() ;
**
** FORMAL ARGUMENTS:
**
**	Return value:	The number of child nodes in the given object.
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

int tree_object::count_number_of_children()
{
	register int no_of_children = 0;
	base_object* child;
	list_element* pos1 = NIL;

	list_for_el_pos(child, base_object, this, pos1)
		no_of_children++;

	number_of_children = no_of_children;
	return(no_of_children);
} /* tree_object::count_count_number_of_children */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	In an symbol, finds an al_el by its position.
**
** CALLING SEQUENCE:
**
**	list_element *tree_objectect::get_el_pos_by_int_pos(int position)
**
** FORMAL ARGUMENTS:
**
**	Return value:	The el corresponding to position.
**
**	position:		The position of the el to be found.
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

list_element* tree_object::get_el_pos_by_int_pos(int position)
{
	list_element* el1;

	if (position < -1)
		abort_run("Invalid position in get_el_pos_by_int_pos");

	if (position == -1) return(NIL); // The position -1 corresponds
	// to the position just before the first actual position.

	for (el1 = this->get_first_element(); el1 != NIL;
		el1 = el1->get_next())
		if (el1->get_position() == position) break;

	if (el1 == NIL) abort_run("el1 not found in \
			get_el_pos_by_int_pos");

	return(el1);
} // tree_object::get_el_pos_by_int_pos

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	In an symbol, finds an al_el by its symbol.
**
** CALLING SEQUENCE:
**
**	list_element *tree_object::get_el_pos_by_child(base_object *obj1)
**
** FORMAL ARGUMENTS:
**
**	Return value:	The el corresponding to position.
**
**	obj1:			The object in the el to be found.
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

list_element* tree_object::get_el_pos_by_child(base_object* obj1)
{
	list_element* el1;

	for (el1 = this->get_first_element(); el1 != NIL;
		el1 = el1->get_next())
		if (el1->get_el_obj() == obj1) break;

	if (el1 == NIL) abort_run("el1 not found in \
			get_el_pos_by_child");

	return(el1);
} // tree_object::get_el_pos_by_child

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	A method to create a set of (copies of) the symbols in an
**	object, without any duplicates.
**
** CALLING SEQUENCE:
**
**	group *tree_object::symbol_set()
**
** FORMAL ARGUMENTS:
**
**	Return value:	A group containing copies of symbols in an object,
**					without duplicates.
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

group* tree_object::symbol_set()
{
	group* set;

	set = new group;
	this->symbol_set_recursion(set);
	return(set);
} // tree_object::symbol_set

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Calculates values for EC and total_cost for any pattern. EC is
**	the sum of the bit_cost values of the symbols in the pattern that are
**	marked as IDENTIFICATION symbols. total_cost is the sum of the
**	bit_costs of all the symbols in the pattern.
**
**	The computed value is stored in the pattern.
**
** CALLING SEQUENCE:
**
**	void sequence::compute_costs()
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
**	The computed value is stored in the pattern.
**
** SIDE EFFECTS:
**
**	NONE
**
*/

void sequence::compute_costs()
{
	symbol* symbol1;
	double cumulative_EC = 0, cumulative_cost = 0, symbol_bit_cost;

	list_for(symbol1, symbol, this)
	{
		symbol_bit_cost = symbol1->get_bit_cost();
		cumulative_cost += symbol_bit_cost;
		if (symbol1->get_status() != IDENTIFICATION) continue;
		cumulative_EC += symbol_bit_cost;
	}

	this->set_encoding_cost(cumulative_EC);
	this->set_total_cost(cumulative_cost);
} // sequence::compute_costs

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Makes a code from an alignment ('this'), assuming that patterns
**	in the alignment have code symbols. If they do not have code
**	symbols, then this method does not make a code.
**
**	The method is simply to traverse the alignment, making copies of
**	non-bracket IDENTIFICATION symbols that appear in a column by
**	themselves. The copies exclude the row information in the originals.
**
**	At the same time as making the code, this method computes values
**	for the 'actual cost' of the New hit symbols in the alignment
**	(NSC), the 'minimal cost' of the code symbols in the code (EC),
**	and the 'compression difference' (CD), the 'compression ratio'
**	(CR) and the abs_P of the alignment.
**
**	The code and the calculated values are not printed out - so that
**	the arrangements and formats of these values can be tailored
**	to specific contexts.
**
**	In calculating values for NSC, EC, CR and CD, no attempt is
**	made to correct for gaps in matching.
**
** CALLING SEQUENCE:
**
**	sequence *sequence::make_code(bool return_code)
**
** FORMAL ARGUMENTS:
**
**	Return value:	The new code.
**
**	return_code:	If true, the function returns an explicit code sequence.
**					Otherwise it returns NIL.
**
** IMPLICIT INPUTS:
**
**	NONE
**
** IMPLICIT OUTPUTS:
**
**	NSC, EC, CD, CR and abs_P for the 'this'.
**
** SIDE EFFECTS:
**
**	NONE
**
*/

sequence* sequence::make_code(bool return_code)
{
	symbol* symbol1, * symbol2;
	int i;
	double NSC, EC;

	struct row_record
	{
		sequence* row_pattern; // The pattern in this row.
		int previous_new_int_pos; // Records the position in New
			// of the previous hit with row_pattern. If there has
			// not yet been a hit with New, this variable has a
			// NULL_VALUE.
		int previous_old_int_pos; // Records the position in 
			// row_pattern of the previous hit between another pattern
			// and row_pattern. If there has not yet been a hit on
			// row_pattern, this variable has a NULL_VALUE.
		bool row_pattern_fully_recognised; // true if all the
			// CONTENTS symbols in this row have hits with New and if
			// the corresponding hit symbols in New form a coherent
			// sequence.
	};

	struct row_record* rows_array;

	rows_array = new row_record[sequence_depth];

	// Initialise variables in rows_array.

	for (i = 0; i < sequence_depth; i++)
	{
		rows_array[i].row_pattern = NIL;
		rows_array[i].previous_new_int_pos = NULL_VALUE;
		rows_array[i].previous_old_int_pos = NULL_VALUE;
		rows_array[i].row_pattern_fully_recognised = true;
	}

	NSC = EC = 0;
	symbol* symbol_new;
	list_element* el_pos1 = NIL;

	sequence* new_code = NIL;
	if (return_code) new_code = new sequence(CODE_PATTERN);

	list_for_el_pos(symbol1, symbol, this, el_pos1)
	{
		// Identify columns in the alignment that contain hit
		// symbols from cnp. Add up the 
		// 'actual costs' of these symbols to arrive at the
		// new_symbols_cost for the alignment.

		if (symbol1->is_a_hit())
		{
			symbol_new = symbol1->get_row_symbol(0);
			if (symbol_new != NIL)
			{
				NSC += symbol_new->get_bit_cost();
			}
			continue; // We are now only interested in non-hit
				// CONTEXT_SYMBOLs with status IDENTIFICATION.
		}

		if (symbol1->get_status() != IDENTIFICATION) continue;

		// Make a 'shallow' copy of symbol1 (excluding information about
		// rows ) and add it to the code. At the same time, add the
		// bit_cost of symbol1 to EC.

		if (return_code)
		{
			symbol2 = symbol1->shallow_copy();
			symbol2->set_status(CONTENTS);
			new_code->add_child(symbol2);
		}

		EC += symbol1->get_bit_cost();
	}

	this->set_new_symbols_cost(NSC);
	this->set_encoding_cost(EC);
	this->set_compression_difference(NSC - EC);
	this->set_compression_ratio(NSC / EC);

	double two_power_s = pow(2, EC);
	this->set_abs_P(1 / two_power_s);

	delete[] rows_array;

	if (return_code)
	{
		new_code->set_new_symbols_cost(NSC);
		new_code->set_encoding_cost(EC);
		new_code->set_compression_difference(NSC - EC);
		new_code->set_compression_ratio(NSC / EC);
		new_code->set_derived_from_parsing(ID);
		return(new_code);
	}
	else return(NIL);

} // sequence::make_code

/***********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Recursive part of symbol_set.  Recursively adds all the different
**	symbols in `object' to the object set `set'.  If a SYMBOL is already
**	in `set' then it's frequency is updated every time it is encountered
**	in `object' rather than adding it again.
**
**	In the initial call the `symbol_set_recursion', `set' should be an empty
**	group.
**
** CALLING SEQUENCE:
**
**	void symbol::symbol_set_recursion(group *set)
**
** FORMAL ARGUMENTS:
**
**	Return value:			None.
**
**	set:					The object to receive the set which is
**							built up.
**
** IMPLICIT INPUTS:
**
**	NONE
**
** IMPLICIT OUTPUTS:
**
**	The symbols in `set' become altered by this function.
**
** SIDE EFFECTS:
**
**	NONE
*/

void symbol::symbol_set_recursion(group* set)
{
	symbol* item_in_set, * new_set_item;

	// Check whether set contains a symbol that matches 'this'.

	item_in_set = (symbol*)set->contains_copy_of(this);
	if (item_in_set == NIL)
	{
		// Item is not in the set - add a copy of it.

		new_set_item = new symbol(*this);
		// new_set_item->set_frequency(1) ;
		set->add_child(new_set_item);
	}
	else
	{
		// The symbol is already in the set: 
		// update it's frequency in the set.

		// item_in_set->increment_frequency() ;
	}
} /* symbol::symbol_set_recursion */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Checks a hit sequence to see whether the CONTENTS symbols in the
**	target pattern are all matched. Here, 'this' hit
**	node is the last node in the sequence.
**
** CALLING SEQUENCE:
**
**	bool hit_node::all_contents_symbols_are_matched()
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if all CONTENTS symbols are matched,
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
*/

bool hit_node::all_contents_symbols_are_matched()
{
	hit_node* node1 = this;
	symbol* target_symbol;
	sequence* driving_pattern = node1->get_driving_pattern(),
		* target_pattern = node1->get_target_pattern();
	hit_node* hit_node_array[MEDIUM_SCRATCH_ARRAY_SIZE] = { 0 };
	int counter = 0, fe_hit_node_array, i;

	// Recast hit sequence for easy left-to-right access. First,
	// find the length of the hit sequence.

	while (node1 != hit_root)
	{
		counter++;
		node1 = node1->get_hn_parent();
	}

	if (counter >= MEDIUM_SCRATCH_ARRAY_SIZE)
		abort_run("Array overflow in \
			hit_node::all_data_symbols_are_matched()");

	fe_hit_node_array = counter;

	// Now put the nodes into hit_node_array[].

	node1 = this;
	while (node1 != hit_root)
	{
		hit_node_array[--counter] = node1;
		node1 = node1->get_hn_parent();
	}

	// Work left to right, checking symbols in the hits recorded
	// in hit_node_array[] and symbols between the hits in
	// the target_pattern.

	target_pattern->initialise();

	symbol* symbol1;
	int dr_int_pos2 = NULL_VALUE;

	for (i = 0; i < fe_hit_node_array; i++)
	{
		node1 = hit_node_array[i];
		target_symbol = node1->get_target_symbol();

		// Check the symbols in the target_pattern up to but not 
		// including the target_symbol.

		while (symbol1 = (symbol*)target_pattern->get_next_child())
		{
			if (symbol1 == target_symbol) break;
			if (symbol1->get_status() == CONTENTS)
				return(false); // We have a CONTENTS symbol that is 
					// not part of a hit on this cycle.
		}
	}

	// Now do the trailing parts of the target_pattern, if any.

	while (symbol1 = (symbol*)target_pattern->get_next_child())
	{
		if (symbol1->get_status() == CONTENTS)
			return(false); // We have a CONTENTS symbol that is 
				// not part of a hit on this cycle.
	}

	return(true);

} // hit_node::all_contents_symbols_are_matched

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Consructor for the class hit_node.
**
** CALLING SEQUENCE:
**
**	hit_node::hit_node() : tree_object()
**
** FORMAL ARGUMENTS:
**
**	Return value:	None
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

hit_node::hit_node() : cost_tree_object()
{
	ID = hit_node_ID_number++;
	hn_parent = NIL;
	driving_pattern = NIL;
	driving_symbol = NIL;
	driving_int_pos = NULL_VALUE;
	target_pattern = NIL;
	target_symbol = NIL;
	target_int_pos = NULL_VALUE;
	leaf_entry = NIL;
} // hit_node::hit_node

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Copy constructor for the class hit_node.
**
** CALLING SEQUENCE:
**
**	hit_node::hit_node(hit_node &hn) : tree_object(hn)
**
** FORMAL ARGUMENTS:
**
**	Return value:	None
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

hit_node::hit_node(hit_node& hn) : cost_tree_object(hn)
{
	ID = hit_node_ID_number++;
	hn_parent = hn.get_hn_parent();
	driving_pattern = hn.get_driving_pattern();
	driving_symbol = hn.get_driving_symbol();
	driving_int_pos = hn.get_driving_int_pos();
	target_pattern = hn.get_target_pattern();
	target_symbol = hn.get_target_symbol();
	target_int_pos = hn.get_target_int_pos();
	leaf_entry = hn.get_leaf_entry();
} // hit_node::hit_node

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Compares two alignments to see whether they match or not.
**
** CALLING SEQUENCE:
**
**	bool sequence::alignment_matches(sequence *al1)
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if the two alignments match, false otherwise.
**
**	al1:			The second of the two alignments to be compared.
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

bool sequence::alignment_matches(sequence* al1)
{
	symbol* al_col1, * al_col2, * symbol1, * symbol2;
	alignment_element* al_el1, * al_el2;
	sequence* orig_patt1, * orig_patt2;
	int al1_length, al1_depth, row1, row2;
	bool match_found, al1_rows[MEDIUM_SCRATCH_ARRAY_SIZE];

	if (al1 == NIL) abort_run("NIL value for al1 in \
		sequence::alignment_matches");

	/* Compare the lengths and depths of the sequences. */

	al1_length = al1->get_number_of_children();

	if (number_of_children != al1_length) return(false);

	al1_depth = al1->get_sequence_depth();

	if (sequence_depth != al1_depth) return(false);

	// Check for a match in the basic sequence of columns.

	symbol2 = (symbol*)al1->get_first_child();
	for (symbol1 = (symbol*)this->get_first_child(); symbol1 != NIL;
		symbol1 = (symbol*)this->get_next_child())
	{
		if (symbol1->name_matches(symbol2) == false) return(false);
		symbol2 = (symbol*)al1->get_next_child();
		if (symbol2 == NIL)
		{
			symbol1 = (symbol*)this->get_next_child();
			break;
		}
	}

	if (symbol1 != NIL || symbol2 != NIL)
		return(false);

	/* Now check if actual sequences match. Two sequences can
	be the same even though the rows are in a different order.
	However, if they are to be recognised as the same, for every
	row in one sequence, there must be a row in the other sequence
	which matches it exactly. */

	/* Initialise al1_rows[] which is used to avoid unnecessary
	re-checking of rows in al1 which have already been found to
	match a row in *this. */

	for (row2 = 0; row2 < al1_depth; row2++) al1_rows[row2] = false;

	for (row1 = 0; row1 < sequence_depth; row1++)
	{
		match_found = false;
		for (row2 = 0; row2 < al1_depth; row2++)
		{
			if (al1_rows[row2] == true) continue;

			al_col1 = (symbol*)this->get_first_child();
			this->set_current_el_pos(this->get_first_el_pos());
			al_el1 = al_col1->get_al_el(row1);
			orig_patt1 = al_el1->get_original_pattern();

			al_col2 = (symbol*)al1->get_first_child();
			al1->set_current_el_pos(al1->get_first_el_pos());
			al_el2 = al_col2->get_al_el(row2);
			orig_patt2 = al_el2->get_original_pattern();

			if (orig_patt2 != orig_patt1) continue;

			symbol1 = (symbol*)al_el1->get_el_obj();
			symbol2 = (symbol*)al_el2->get_el_obj();
			if (symbol1 != symbol2) continue;

			while (al_col1 = (symbol*)this->get_next_child())
			{
				symbol1 = al_col1->get_row_symbol(row1);
				al_col2 = (symbol*)al1->get_next_child();
				symbol2 = al_col2->get_row_symbol(row2);
				if (symbol1 != symbol2) goto L1;
			}
			match_found = true;
			al1_rows[row2] = true;
			break;
		L1:;
		}
		if (!match_found) return(false);
	}

	return(true);
} /* sequence::alignment_matches */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	For diagnostic purposes.
**
** CALLING SEQUENCE:
**
**	void sequence::show_al_structure(hit_node *h_node)
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	h_node:			The leaf node for the given alignment. The
**					value may be NIL, in which case, information
**					about driving pattern, target_pattern and
**					h_node itself are not printed out.
**
** IMPLICIT INPUTS:
**
**	NONE
**
** IMPLICIT OUTPUTS:
**
**	Printed output
**
** SIDE EFFECTS:
**
**	NONE
**
*/

void sequence::show_al_structure(hit_node* h_node)
{
	symbol* al_col1;
	int i;

	fprintf(output_file, "Alignment ");
	this->print_ID();
	fprintf(output_file, " : ");

	if (h_node != NIL)
	{
		(h_node->get_driving_pattern())->print_ID();
		fprintf(output_file, " : ");
		(h_node->get_target_pattern())->print_ID();
		fprintf(output_file, " : ");
		h_node->print_ID();
	}
	else fprintf(output_file, "NIL hit_node");

	fprintf(output_file, "%s%1.2f%s%1.2f%s%1.2f%s%d%s%d%s",
		", NSC = ", new_symbols_cost,
		", EC = ", encoding_cost,
		", CR = ", compression_ratio,
		",\nsequence_depth = ", sequence_depth,
		", number of columns = ", number_of_children,
		"\n\n");
	this->initialise();
	i = 0;
	while (al_col1 = (symbol*)this->get_next_child())
	{
		fprintf(output_file, "%s%d%s", "Column ", i, ":\n");
		al_col1->show_al_structure();
		i++;
	}
	fflush(output_file);

	fprintf(output_file, "Alignment ");
	this->print_ID();
	fprintf(output_file, "\n\n");
	this->write_alignment(output_file, write_section_chars_length,
		NULL_VALUE, alignment_format);
	fprintf(output_file, "End of alignment ");
	this->print_ID();
	fprintf(output_file, "\n\n");
	fflush(output_file);
} // sequence::show_al_structure

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Fetches terminal nodes of the hit structure in sorted order and
**	sets the value of an external pointer to the corresponding sequence.
**
** CALLING SEQUENCE:
**
**	hit_node *get_leaf_nodes_in_order(int *row, sequence **alignm,
**		int *leaf_index)
**
** FORMAL ARGUMENTS:
**
**	Return value:	The next node to be fetched. NIL when there are
**					no more nodes.
**
**	row:			A pointer to an integer referencing the relevant row
**					in sort_array[]. Start *row with 0.
**	alignm: 		A pointer to an external pointer to the sequence
**					corresponding to row.
**	leaf_index:		A pointer to an external integer indexing leaf_array[]
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

hit_node* get_leaf_nodes_in_order(int* row, sequence** alignm, int* leaf_index)
{
	if (*row >= fe_sort) return(NIL);
	*leaf_index = sort_array[*row];
	(*row)++;
	*alignm = alignments_array[*leaf_index];
	return(leaf_array[*leaf_index]);
} // get_leaf_nodes_in_order

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Cleans up memory allocated by new_patterns and
**	then exits. If this memory is not cleaned up it may clog
**	up the machine.
**
** CALLING SEQUENCE:
**
**	void exit_routine(int status)
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	status: 		0 for successful termination, 1 otherwise.
**
** IMPLICIT INPUTS:
**
**	corpus, original_symbols_in_corpus, boundary_symbol,
**	duplicate_unifications, hn_master, hit_root
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

void exit_routine(int status)
{
	delete created_patterns;
	parsing_alignments->delete_children();
	parsing_alignments->release_children();
	delete parsing_alignments;
	selected_cumulative_parsing_alignments->release_children();
	delete selected_cumulative_parsing_alignments;
	delete cumulative_parsing_alignments;
	delete set_of_grammars;
	delete naive_grammar;
	set_of_combinations->delete_children();
	delete set_of_combinations;
	delete best_combinations;
	delete generalisation_list1;
	delete generalisation_list2;

	delete hn_master;

	delete brackets_list;

	// delete corpus ;

	delete symbols_in_new;
	delete original_symbols_in_corpus;
	delete symbol_types_in_old;
	delete new_patterns;
	delete old_patterns;

	delete[] write_al_chars;
	delete[] write_al_vacant_slots;

	fclose(output_file);
	fclose(input_file);
	fclose(parameters_file);
	fclose(latex_file);
	exit(status);
} /* exit_routine */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**		Aborts the program after displaying an error message.
**
** CALLING SEQUENCE:
**
**	void abort_run(char *message)
**
** FORMAL ARGUMENTS:
**
**	Return value:	None; this function will never return.
**
**	message:		The string containing the error message.
**
** IMPLICIT INPUTS:
**
**	Functions: write_message
**
** IMPLICIT OUTPUTS:
**
**	NONE
**
** SIDE EFFECTS:
**
**	NONE
*/

void abort_run(const char* message)
{
	write_message(message);
	exit_routine(1);
} /* abort_run */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Writes a string to the logfile.
**
** CALLING SEQUENCE:
**
**	void write_message(char *s)
**
** FORMAL ARGUMENTS:
**
**	Return value:	None
**
**	s:				The string to be written.
**
** IMPLICIT INPUTS:
**
**	NONE
**
** IMPLICIT OUTPUTS:
**
**	logfile: Written to.
**
** SIDE EFFECTS:
**
**	Logfile is updated (written to).  No checks are made
**	as to whether an error occurred during writing.
*/

void write_message(const char* s)
{
	fprintf(output_file, "%s%s", s, "\n\n");
} /* write_message */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Writes to a given output stream, counting the number of characters
**	in each line and taking a new line when necessary.
**
** CALLING SEQUENCE:
**
**	void void write_lines(FILE *stream, char *string, int status)
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	stream:			The file to be written to.
**	string: 		The string to be written.
**	status: 		START for starting, CONTINUE for carrying on.
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

void write_lines(FILE* stream, const char* string, int status)
{
	static size_t column_counter = 0;

	if (status == START) column_counter = 0;

	column_counter += strlen(string);
	if (column_counter > MAX_WRITE_LINE)
	{
		fprintf(stream, "\n");
		column_counter = 0;
	}

	fprintf(stream, "%s", string);
} /* write_lines */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Scans over spaces and commas in the `syntax buffer'.
**
** CALLING SEQUENCE:
**
**	void scan(void) ;
**
** FORMAL ARGUMENTS:
**
**	Return value:	None.
**
** IMPLICIT INPUTS:
**
**	Variables:	syn_buf_ptr
**
** IMPLICIT OUTPUTS:
**
**	syn_buf_ptr
**
** SIDE EFFECTS:
**
**	None, but see IMPLICIT OUTPUTS above.
*/

void scan(void) /* Scans over spaces, commas etc between units in the input */
{
	char c;

	c = *syn_buf_ptr;
	while (c == ' ' || c == ',')
		c = *(++syn_buf_ptr);
} /* scan */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Reads a frequency value for an object (if any) recorded as
**	'*<integer_string>' immediately after the object in the
**	input file. If there is a value, this overwrites the default
**	value (1) in internal representation of the object.
**
** CALLING SEQUENCE:
**
**	void read_frequency(base_object *obj) ;
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	obj:			The object whose frequency value is to be read.
**
** IMPLICIT INPUTS:
**
**	syn_buf_ptr
**
** IMPLICIT OUTPUTS:
**
**	syn_buf_ptr
**
** SIDE EFFECTS:
**
**	None, but see IMPLICIT OUTPUTS above.
*/

void read_frequency(base_object* obj)
{
	char* cpx, digit_array[STRING_LENGTH];
	int i;

	cpx = syn_buf_ptr;
	if (*syn_buf_ptr == '*') syn_buf_ptr++;
	else return;
	if (!isdigit(*syn_buf_ptr))
	{
		syn_buf_ptr = cpx;
		return;
	}
	i = 0;
	while (isdigit(*syn_buf_ptr))
	{
		digit_array[i] = *syn_buf_ptr;
		if (++i >= STRING_LENGTH)
			abort_run("Frequency digits in input too long");
		syn_buf_ptr++;
	}
	digit_array[i] = '\0';

	obj->set_frequency(atoi(digit_array));

	scan();
} /* read_frequency */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Make a symbol out of a string of characters in the `syntax buffer'.
**	true is returned if a valid symbol was constructed, false otherwise.
**	A valid symbol includes "". If the
**	symbol is too long, an error is raised and the program aborts.
**
** CALLING SEQUENCE:
**
**	bool rec_symbol(char *nme, bool *symbol_is_identifier) ;
**
** FORMAL ARGUMENTS:
**
**	Return value:			true if `nme' is valid, false otherwise.
**
**	nme:					String to receive the symbol.
**							NB: Call by reference - symbol value is
**							passed back through this parameter.
**	symbol_is_identifier:	Pointer to an external variable which is set in
**							the function to true if the given
**							symbol is IDENTIFICATION, false otherwise.
**
** IMPLICIT INPUTS:
**
**	NONE
**
** IMPLICIT OUTPUTS:
**
**	syn_buf_ptr
**
** SIDE EFFECTS:
**
**	The string `nme' must be declared such that it is of sufficient size
**	to accept the symbol created.  Also note that `syn_buf_ptr' is
**	updated.
*/

bool rec_symbol(char* nme, bool* symbol_is_identifier) /* Makes a
	symbol out of a string of characters */
{
	char c;
	int i = 0;

	c = *syn_buf_ptr;
	if (c == '\"') /* A quoted string in the input */
	{
		c = *(++syn_buf_ptr);
		while (c != '\"')
		{
			if (i >= (LONG_STRING_LENGTH - 2))
				abort_run("Symbol too long in 'symbol'");
			*(nme + i++) = c;
			c = *(++syn_buf_ptr);
		}
		syn_buf_ptr++;
	}
	else
	{
		if (c == '!')
		{
			identification_symbols_marked = true;
			*symbol_is_identifier = true;
			c = *(++syn_buf_ptr);
		}

		while (c != ' ' && c != ',' && c != '('
			&& c != ')' && c != '[' && c != ']')
		{
			if (i >= (LONG_STRING_LENGTH - 2))
				abort_run("Symbol too long in 'symbol'");
			nme[i++] = c;
			c = *(++syn_buf_ptr);
		}
	}
	if (i == 0) return false;
	nme[i] = '\0';
	scan();
	return(true);
} /* rec_symbol */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Returns true if the string given matches the
**	`syntax buffer'.  The current place in the syntax buffer is advanced
**	past the matched string if the string matches and is left unchanged
**	if it doesn't.
**
** CALLING SEQUENCE:
**
**	bool characters(char *charsx)
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if a match occurred, false otherwise.
**
**	charsx: 		String to match `syntax buffer' with.
**
** IMPLICIT INPUTS:
**
**	Functions:	scan
**
**	Variables:	syn_buf_ptr
**
** IMPLICIT OUTPUTS:
**
**	syn_buf_ptr
**
** SIDE EFFECTS:
**
**	None, but see IMPLICIT OUTPUTS above.
*/

bool characters(const char* charsx) /* Recognises a string of characters
in the syntax buffer */
{
	char* cpx;

	cpx = syn_buf_ptr;
	while (*charsx != '\0')
		if (*syn_buf_ptr++ != *charsx++)
		{
			syn_buf_ptr = cpx;
			return(false);
		}
	scan();
	return(true);
} /* characters */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Returns false if the next character in the `syntax buffer' isn't an SP
**	opening bracket, otherwise the object type associated with the bracket
**	is returned.
**
** CALLING SEQUENCE:
**
**	int left_bracket(void) ;
**
** FORMAL ARGUMENTS:
**
**	Return value:	false if not an SP opening bracket, corresponding object
**					type (GROUP, SEQUENCE) otherwise.
**
** IMPLICIT INPUTS:
**
**	Functions: characters
**
** IMPLICIT OUTPUTS:
**
**	NONE
**
** SIDE EFFECTS:
**
**	NONE
*/

int left_bracket()
{
	int b1 = false;

	if (characters("(")) b1 = SEQUENCE;
	else if (characters("[")) b1 = GROUP;
	return(b1);
} /* left_bracket */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Returns true if the character in the `syntax buffer' is the closing
**	bracket of the object type specified, false otherwise.
**
** CALLING SEQUENCE:
**
**	bool right_bracket(int b1)
**
** FORMAL ARGUMENTS:
**
**	Return value:	true if corresponding closing bracket, false otherwise.
**
**	b1: 		Object type.
**
** IMPLICIT INPUTS:
**
**	Functions: characters
**
** IMPLICIT OUTPUTS:
**
**	NONE
**
** SIDE EFFECTS:
**
**	NONE
*/

bool right_bracket(int b1)
{
	if (b1 == SEQUENCE && characters(")")) return(true);
	else if (b1 == GROUP && characters("]")) return(true);
	return(false);
} /* right_bracket */

/*********************************************************************/

tree_object* comp_object();

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Recognises an `object'.
**
** CALLING SEQUENCE:
**
**	base_object *find_node() ;
**
** FORMAL ARGUMENTS:
**
**	Return value:	A new object if object recognised, NIL otherwise.
**
** IMPLICIT INPUTS:
**
**	Functions:	comp_object, symbol, scan
**
**	Variables:	syn_buf_ptr
**
** IMPLICIT OUTPUTS:
**
**	syn_buf_ptr
**
** SIDE EFFECTS:
**
**	NONE?
*/

/* object

  syntax -> object | bad_syntax ;
  object -> comp_object | simple_object ;
  comp_object -> SEQUENCE | GROUP ;
  SEQUENCE -> '(' , body , ')' ;
  GROUP -> '[' , body , ']' ;
  body -> b | NIL ;
  b -> object , body ;
  simple_object -> SYMBOL ;
  SYMBOL -> alphanumeric(+) ;

*/

base_object* find_node()
{
	char nme[LONG_STRING_LENGTH];
	base_object* node;
	symbol* symbol1;
	bool symbol_is_identifier = false;

	if (node = comp_object())
	{
		read_frequency(node);
		return(node);
	}
	else // Object is a symbol 
		if (rec_symbol(nme, &symbol_is_identifier))
		{
			symbol1 = new symbol(nme, 1, NULL_VALUE);
			if (symbol_is_identifier) symbol1->set_status(IDENTIFICATION);
			else symbol1->set_status(CONTENTS);

			read_frequency(symbol1);
			return(symbol1);
		}
		else return(NIL);

} /* find_node */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Recognises a tree_object.
**
** CALLING SEQUENCE:
**
**	tree_object *comp_object() ;
**
** FORMAL ARGUMENTS:
**
**	Return value:	A new node if composite object, NIL otherwise.
**
** IMPLICIT INPUTS:
**
**	syntax_buffer
**
** IMPLICIT OUTPUTS:
**
**	NONE
**
** SIDE EFFECTS:
**
**	NONE
*/

tree_object* comp_object()
{
	int b1;
	tree_object* new_node;
	base_object* child;

	if (b1 = left_bracket())
	{
		if (b1 == SEQUENCE) new_node = new sequence(BASIC_PATTERN);
		else new_node = new group;
		while (child = find_node())
			new_node->add_child(child);
		if (!right_bracket(b1))
		{
			delete new_node;
			return(NIL);
		}
		new_node->mark_parent_and_int_positions_non_recursive();
		// take care! This alters the value of
		// the 'parent' field.
		new_node->set_number_of_children(new_node->
			count_number_of_children());
		return(new_node);
	}
	else return(NIL);
} /* comp_object */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Input the SP corpus.
**
** CALLING SEQUENCE:
**
**	tree_object *top_object() ;
**
** FORMAL ARGUMENTS:
**
**	Return value:	node if input is correct, false otherwise.
**
** IMPLICIT INPUTS:
**
**	NONE
**
** IMPLICIT OUTPUTS:
**
**	syn_buf_ptr
**
** SIDE EFFECTS:
**
**	None, except see IMPLICIT OUTPUTS above.
*/

tree_object* top_object()
{
	tree_object* node;

	syn_buf_ptr = syntax_buffer;
	scan();
	if (node = (tree_object*)find_node()) return(node);
	else return(NIL);
} /* top_object */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Error non-terminal for SP parser.  Writes `informative' error message
**	and returns.
**
** CALLING SEQUENCE:
**
**	void bad_syntax(void) ;
**
** FORMAL ARGUMENTS:
**
**	Return value:	None.
**
** IMPLICIT INPUTS:
**
**	Functions:	write_message
**
**	Variables:	syn_buf_ptr
**
** IMPLICIT OUTPUTS:
**
**	NONE
**
** SIDE EFFECTS:
**
**	NONE
*/

void bad_syntax()
{
	char* cpx;
	cpx = syn_buf_ptr - 20;

	if (cpx < syntax_buffer) cpx = syntax_buffer;

	write_message("Error_in_syntax:");
	while (cpx <= syn_buf_ptr) write_message(cpx++);
	write_message("\n");
} /* bad_syntax */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Opens file specified using buffer size specified and reads
**	the entire file into the `syntax buffer'. The file is
**	closed after reading.
**
** CALLING SEQUENCE:
**
**	void in_syntax() ;
**
** FORMAL ARGUMENTS:
**
**	Return value:	None.
**
** IMPLICIT INPUTS:
**
**	Functions:	abort_run
**
**	Variables:	syntax_buffer
**
** IMPLICIT OUTPUTS:
**
**	syntax_buffer
**
** SIDE EFFECTS:
**
**	NONE
*/

void in_syntax()
{
	int c;
	int i = 0;

	/* range check required */
	while (c = getc(input_file))
	{
		if (c == EOF || c == '~') break;
		if (i >= BUFSIZE)
			abort_run("Array overflow in 'in_syntax'");
		if (c == '\n' || c == '\t')
			syntax_buffer[i++] = ' ';
		else syntax_buffer[i++] = c;
	}
} /* in_syntax */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Writes out the key to the abbreviations of symbols used in
**	the corpus.
**
** CALLING SEQUENCE:
**
**	void write_key()
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
** IMPLICIT INPUTS:
**
**	key_array[]
**
** IMPLICIT OUTPUTS:
**
**	Output file
**
** SIDE EFFECTS:
**
**	NONE
**
*/

void write_key()
{
	int key_index;

	fprintf(output_file, "KEY\n\n");
	for (key_index = 0; key_index < fe_key_array; key_index++)
	{
		fprintf(output_file, "%s%c",
			key_array[key_index].abbreviated_symbol, ' ');
		fprintf(output_file, "%s%c",
			key_array[key_index].complete_symbol, '\n');
	}
	fprintf(output_file, "\nEND KEY\n\n");
} // write_key

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Finds a unique abbreviation for a symbol. Called from
**	make_letter_abbreviations.
**
** CALLING SEQUENCE:
**
**	void find_abbreviation(int i)
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
**	i:				The last position in key_array[] that needs to be
**					checked in checking for the uniqueness of
**					an abbreviation.
**
** IMPLICIT INPUTS:
**
**	complete_symbol[]
**
** IMPLICIT OUTPUTS:
**
**	new_abbreviation[]
**
** SIDE EFFECTS:
**
**	NONE
**
*/

void find_abbreviation(int i)
{
	int early_char_pos, late_char_pos, last_char_pos, j, m;

	early_char_pos = late_char_pos = 0;
	while (complete_symbol[late_char_pos + 1] != '\0')
		late_char_pos++;
	last_char_pos = late_char_pos;
	int k = 0; // Index into new_abbreviation[].
	new_abbreviation[k] = complete_symbol[early_char_pos];
	if (late_char_pos > early_char_pos)
	{
		new_abbreviation[++k] = complete_symbol[late_char_pos];
		new_abbreviation[++k] = '\0';
	}
	else new_abbreviation[++k] = '\0';

	// Check whether the newly-constructed abbreviation is
	// unique.

	bool abbreviation_is_unique = true;
	for (j = 0; j < i; j++)
	{
		if (key_array[j].abbreviated_symbol == NIL) continue;
		if (strcmp(key_array[j].abbreviated_symbol,
			new_abbreviation) == 0)
		{
			abbreviation_is_unique = false;
			break;
		}
	}

	// If it is not unique, keep adding characters until it is.

	bool next_char_is_early = true;
	char c;
	while (!abbreviation_is_unique)
	{
		// Add the next character.

		if (next_char_is_early)
		{
			next_char_is_early = false;
			c = complete_symbol[++early_char_pos];
			while (c == 'a' || c == 'e' || c == 'i' ||
				c == 'o' || c == 'u')
				c = complete_symbol[++early_char_pos];
			if (early_char_pos >= late_char_pos)
			{
				fprintf(output_file, "%s%s",
					complete_symbol,
					"\n\n");
				abort_run("Non-unique abbreviation");
			}
		}
		else
		{
			next_char_is_early = true;
			c = complete_symbol[--late_char_pos];
			while (c == 'a' || c == 'e' || c == 'i' ||
				c == 'o' || c == 'u')
				c = complete_symbol[--late_char_pos];
			if (early_char_pos >= late_char_pos)
			{
				fprintf(output_file, "%s%s",
					complete_symbol,
					"\n\n");
				abort_run("Non-unique abbreviation");
			}
		}

		// Make the new abbreviation. Do the leading
		// characters first.

		k = 0; // Index into new_abbreviation.
		new_abbreviation[k++] = complete_symbol[0]; // The 
		// first character can be a vowel.
		for (m = 1; m <= early_char_pos; m++)
		{
			c = complete_symbol[m];
			while (c == 'a' || c == 'e' || c == 'i' ||
				c == 'o' || c == 'u')
				c = complete_symbol[++m];
			new_abbreviation[k++] = c;
		}

		// Now do the trailing characters.

		new_abbreviation[k++] = complete_symbol[late_char_pos]; // The 
		// last character can be a vowel.
		for (m = late_char_pos + 1; m < last_char_pos; m++)
		{
			c = complete_symbol[m];
			while (c == 'a' || c == 'e' || c == 'i' ||
				c == 'o' || c == 'u')
				c = complete_symbol[++m];
			new_abbreviation[k++] = c;
		}

		// The last character may be a consonant or a vowel.

		new_abbreviation[k++] = complete_symbol[m];
		new_abbreviation[k] = '\0';

		// Check whether the newly-constructed abbreviation is
		// unique.

		abbreviation_is_unique = true;
		for (j = 0; j < i; j++)
		{
			if (key_array[j].abbreviated_symbol == NIL)
				continue;
			if (strcmp(key_array[j].abbreviated_symbol,
				new_abbreviation) == 0)
			{
				abbreviation_is_unique = false;
				break;
			}
		}
	}
} // find_abbreviation

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Creates a key to the abbreviations of symbols to be used in
**	writing out alignments.
**
**	Abbreviations are only made for symbols which do not begin with
**	a digit. Apart from the first and last characters (which can be
**	a consonant or a vowel) the abbreviation for a symbol is made
**	from consonants in the symbol, starting with the first, followed
**	by the last, followed by the consonant after the first, followed
**	by the consonant before the last, and so on. Addition of
**	consonants stops when the abbreviation is unique in the set
**	of abbreviations.
**
** CALLING SEQUENCE:
**
**	void make_letter_abbreviations()
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
** IMPLICIT INPUTS:
**
**	original_symbols_in_corpus
**
** IMPLICIT OUTPUTS:
**
**	Values for key_array[]
**
** SIDE EFFECTS:
**
**	NONE
**
*/

void make_letter_abbreviations()
{
	symbol* symbol1;
	int i, j, symbol_length;

	// Copy the name of the symbols in the corpus into
	// the 'complete_symbol' field of key_array[] and initialise
	// the abbreviation fields at the same time.

	fe_key_array = 0;
	original_symbols_in_corpus->initialise();
	while (symbol1 = (symbol*)original_symbols_in_corpus->get_next_child())
	{
		symbol_length = (int)strlen(symbol1->get_name()) + 1;
		if (symbol_length > MEDIUM_SCRATCH_ARRAY_SIZE)
			abort_run("Symbol is too long in \
		make_letter_abbreviations");
		key_array[fe_key_array].complete_symbol =
			new char[symbol_length];
		strcpy(key_array[fe_key_array].complete_symbol,
			symbol1->get_name());
		key_array[fe_key_array].abbreviated_symbol = NIL;
		plus_one(&fe_key_array, KEY_ARRAY_SIZE,
			"Overflow of key_array[] in read_key");
	}

	// Now step through key_array[] finding an abbreviation for each
	// entry that does not begin with '#'.

	for (i = 0; i < fe_key_array; i++)
	{
		strcpy(complete_symbol, key_array[i].complete_symbol);
		if (complete_symbol[0] == '#') continue;

		if (isdigit(complete_symbol[0]))
		{
			key_array[i].abbreviated_symbol =
				new char[strlen(complete_symbol) + 1];
			strcpy(key_array[i].abbreviated_symbol,
				complete_symbol);
			continue;
		}

		find_abbreviation(i);

		// Add the newly-constructed abbreviation to the key_array[].

		key_array[i].abbreviated_symbol =
			new char[strlen(new_abbreviation) + 1];
		strcpy(key_array[i].abbreviated_symbol, new_abbreviation);
	}

	// For each symbol in key_array[] that does begin with '#', try
	// to find the corresponding symbol which does not begin with '#'.
	// If one is found, create an abbreviation for the second symbol
	// by appending the abbreviation of the second symbol to '#'.
	// If one is not found, create an abbreviation for it.

	for (i = 0; i < fe_key_array; i++)
	{

		if (*(key_array[i].complete_symbol) != '#') continue;
		strcpy(complete_symbol, key_array[i].complete_symbol);

		// Try to find a complete_symbol in key_array that matches
		// the current symbol after the '#'.

		for (j = 0; j < fe_key_array; j++)
		{
			if (*(key_array[j].complete_symbol) == '#')
				continue;

			if (strcmp((complete_symbol + 1),
				key_array[j].complete_symbol) == 0)
				break;
		}

		if (j >= fe_key_array)
		{
			// No complete symbol has been found which matches
			// key_array[i].complete_symbol after
			// the initial '#'.

			find_abbreviation(fe_key_array);

			key_array[i].abbreviated_symbol =
				new char[strlen(new_abbreviation) + 1];
			strcpy(key_array[i].abbreviated_symbol,
				new_abbreviation);
		}
		else
		{
			// Make an abbreviation by appending the abbreviation
			// that has been found to '#'.

			new_abbreviation[0] = '#';
			strcpy((new_abbreviation + 1),
				key_array[j].abbreviated_symbol);
			key_array[i].abbreviated_symbol =
				new char[strlen(new_abbreviation) + 1];
			strcpy(key_array[i].abbreviated_symbol,
				new_abbreviation);
		}
	}
} // make_letter_abbreviations

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Creates digit abbreviations of symbols in the corpus.
**
** CALLING SEQUENCE:
**
**	void make_digit_abbreviations()
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
**
*/

#if MAKE_DIGIT_ABBREVIATIONS

void make_digit_abbreviations()
{
	symbol* symbol1;
	int i, j, k, m, symbol_length,
		early_char_pos, late_char_pos,
		last_char_pos;
	char complete_symbol[MEDIUM_SCRATCH_ARRAY_SIZE],
		new_abbreviation[MEDIUM_SCRATCH_ARRAY_SIZE], c;
	bool abbreviation_is_unique;
	bool next_char_is_early;

	// First, copy the name of the symbols in the corpus into
	// the 'complete_symbol' field of key_array[].

	fe_key_array = 0;
	original_symbols_in_corpus->initialise();
	while (symbol1 = (symbol*)original_symbols_in_corpus->get_next_child())
	{
		symbol_length = strlen(symbol1->get_name()) + 1;
		if (symbol_length > MEDIUM_SCRATCH_ARRAY_SIZE)
			abort_run("Symbol is too long in \
		make_letter_abbreviations");
		key_array[fe_key_array].complete_symbol =
			new char[symbol_length];
		strcpy(key_array[fe_key_array].complete_symbol,
			symbol1->get_name());
		plus_one(&fe_key_array, KEY_ARRAY_SIZE,
			"Overflow of key_array[] in read_key");
	}

	// Now step through key_array[] finding an abbreviation for each
	// entry. To do this, symbols are processed to remove all characters
	// which are not alphanumeric and these symbols are replaced in
	// the same relative position in the digit abbreviation.
	// This means that '(symbol' and 'symbol)'
	// will both contain the same digits, eg '(23' and '23)'.

	for (i = 0; i < fe_key_array; i++)
	{
	}
} // make_digit_abbreviations

#endif

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Reads in the key to abbreviations of symbols used in the corpus.
**
** CALLING SEQUENCE:
**
**	void read_key()
**
** FORMAL ARGUMENTS:
**
**	Return value:	void
**
** IMPLICIT INPUTS:
**
**	input file
**
** IMPLICIT OUTPUTS:
**
**	Values for the key stored in key_array[]
**
** SIDE EFFECTS:
**
**	NONE
**
*/

void read_key()
{
	char input_string[LONG_STRING_LENGTH];
	int i;

	fe_key_array = 0;
	fscanf(input_file, "%s", input_string);
	while (strcmp(input_string, "KEY") != 0)
		fscanf(input_file, "%s", input_string);
	fscanf(input_file, "%s", input_string);
	while (strcmp(input_string, "END_KEY") != 0)
	{
		key_array[fe_key_array].abbreviated_symbol =
			new char[strlen(input_string) + 1];
		strcpy(key_array[fe_key_array].abbreviated_symbol,
			input_string);

		fscanf(input_file, "%s", input_string);
		if (strcmp(input_string, "END_KEY") == 0) break;

		// Get rid of any comma at end of string.

		for (i = 0; i < LONG_STRING_LENGTH; i++)
		{
			if (input_string[i] == '\0')
			{
				fprintf(output_file, "%s%s",
					input_string, "\n\n");
				abort_run("Error in input_string in read_key");
			}
			if (input_string[i] == ',' ||
				input_string[i] == '.')
			{
				input_string[i] = '\0';
				break;
			}
		}
		key_array[fe_key_array].complete_symbol =
			new char[strlen(input_string) + 1];
		strcpy(key_array[fe_key_array].complete_symbol, input_string);

		plus_one(&fe_key_array, KEY_ARRAY_SIZE,
			"Overflow of key_array[] in read_key");

		fscanf(input_file, "%s", input_string);
	}
} // read_key

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	Reads in parameters for the program.
**
** CALLING SEQUENCE:
**
**	void read_parameters()
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
**
*/


void read_parameters(void)
{
	char input_string[LONG_STRING_LENGTH] = { 0 };
	const char* parameters[] =
	{
		"/*",
		"PROBABILITIES",
		"SHOW_AL_STRUCTURE",
		"SHOW_HIT_STRUCTURE",
		"SHOW_UNSELECTED_ALIGNMENTS",
		"VERBOSE",
		"USE_ABBREVIATIONS",
		"SHOW_ALL_PARSING_ALIGNMENTS",
		"ID_C_SYMBOL_CONSTRAINT",
		"ALIGNMENT_FORMAT",
		"HIT_STRUCTURE_ROWS",
		"FAIL_SCORE",
		"COST_FACTOR",
		"KEEP_ROWS",
		"DRIVING_KEEP_ROWS",
		"FULL_ALIGNMENT_LIMIT",
		"GRAMMAR_LIMIT",
		"EXTRACTION_LIMIT",
		"FIGURE_ID",
		"COMBINATION_LIMIT",
		"MAX_OLD_GAP",
		"MAX_DRIVING_GAP",
		"MAX_TARGET_GAP",
		"MAX_UNSUPPORTED_CYCLES",
		"MAX_ALIGNMENTS_IN_ONE_CYCLE",
		"ORIENTATION",
		"WRITE_AL_CHARS_LENGTH",
		"WRITE_SECTION_CHARS_LENGTH",
		"MINIMUM_FONT_HEIGHT",
		"MAXIMUM_FONT_HEIGHT",
		"N_GRAMMARS",
		"CREATE_PATTERNS_DIAGNOSTIC",
		"LAST_ITEM"
	};

	int i, j, parameter_index;

	fscanf(parameters_file, "PARAMETERS%s", input_string);
	while (strcmp(input_string, "END_PARAMETERS") != 0)
	{
		parameter_index = 0;
		while (true)
		{
			if (strcmp(parameters[parameter_index],
				input_string) == 0)
				break;
			if (strcmp(parameters[parameter_index],
				"LAST_ITEM") == 0)
			{
				fprintf(output_file, "%s%s", input_string,
					"\n\n");
				abort_run("Parameter not identified");
			}
			parameter_index++;
		}

		switch (parameter_index)
		{
		case 0: /* Opening characters for a comment */
		{
			fscanf(parameters_file, "%s", input_string);
			while (strcmp(input_string, "*/") != 0)
				fscanf(parameters_file, "%s",
					input_string);
			goto L1;
		}

		case 1: /* "PROBABILITIES" */
		{
			fscanf(parameters_file, "%s", input_string);
			if (strcmp(input_string, "ON") == 0)
				probabilities = ON;
			else if (strcmp(input_string, "OFF") == 0)
				probabilities = OFF;
			else
			{
				fprintf(output_file, "%s%s",
					input_string, "\n\n");
				abort_run("Invalid parameter");
			}
			goto L1;
		}

		case 2: /* "SHOW_AL_STRUCTURE" */
		{
			fscanf(parameters_file, "%s", input_string);
			if (strcmp(input_string, "ON") == 0)
				show_al_structure = ON;
			else if (strcmp(input_string, "OFF") == 0)
				show_al_structure = OFF;
			else
			{
				fprintf(output_file, "%s%s",
					input_string, "\n\n");
				abort_run("Invalid parameter");
			}
			goto L1;
		}

		case 3: /* "SHOW_HIT_STRUCTURE" */
		{
			fscanf(parameters_file, "%s", input_string);
			if (strcmp(input_string, "ON") == 0)
				show_hit_structure = ON;
			else if (strcmp(input_string, "OFF") == 0)
				show_hit_structure = OFF;
			else
			{
				fprintf(output_file, "%s%s",
					input_string, "\n\n");
				abort_run("Invalid parameter");
			}
			goto L1;
		}

		case 4: /* "SHOW_UNSELECTED_ALIGNMENTS" */
		{
			fscanf(parameters_file, "%s", input_string);
			if (strcmp(input_string, "ON") == 0)
				show_unselected_alignments = ON;
			else if (strcmp(input_string, "OFF") == 0)
				show_unselected_alignments = OFF;
			else
			{
				fprintf(output_file, "%s%s",
					input_string, "\n\n");
				abort_run("Invalid parameter");
			}
			goto L1;
		}

		case 5: /* "VERBOSE" */
		{
			fscanf(parameters_file, "%s", input_string);
			if (strcmp(input_string, "ON") == 0)
				verbose = ON;
			else if (strcmp(input_string, "OFF") == 0)
				verbose = OFF;
			else
			{
				fprintf(output_file, "%s%s",
					input_string, "\n\n");
				abort_run("Invalid parameter");
			}
			goto L1;
		}

		case 6: /* "USE_ABBREVIATIONS" */
		{
			fscanf(parameters_file, "%s", input_string);
			if (strcmp(input_string, "LETTERS") == 0)
				use_abbreviations = LETTERS;
			else if (strcmp(input_string, "DIGITS") == 0)
				use_abbreviations = DIGITS;
			else if (strcmp(input_string, "OFF") == 0)
				use_abbreviations = OFF;
			else
			{
				fprintf(output_file, "%s%s",
					input_string, "\n\n");
				abort_run("Invalid parameter");
			}
			goto L1;
		}

		case 7: /* "SHOW_ALL_PARSING_ALIGNMENTS" */
		{
			fscanf(parameters_file, "%s", input_string);
			if (strcmp(input_string, "ON") == 0)
				show_all_parsing_alignments = ON;
			else if (strcmp(input_string, "OFF") == 0)
				show_all_parsing_alignments = OFF;
			else
			{
				fprintf(output_file, "%s%s",
					input_string, "\n\n");
				abort_run("Invalid parameter");
			}
			goto L1;
		}

		case 8: /* "ID_C_SYMBOL_CONSTRAINT" */
		{
			fscanf(parameters_file, "%s", input_string);
			if (strcmp(input_string, "ON") == 0)
				id_c_symbol_constraint = ON;
			else if (strcmp(input_string, "OFF") == 0)
				id_c_symbol_constraint = OFF;
			else
			{
				fprintf(output_file, "%s%s",
					input_string, "\n\n");
				abort_run("Invalid parameter");
			}
			goto L1;
		}

		case 9: /* "ALIGNMENT_FORMAT" */
		{
			fscanf(parameters_file, "%s", input_string);
			if (strcmp(input_string, "H") == 0)
				alignment_format = 'H';
			else if (strcmp(input_string, "V") == 0)
				alignment_format = 'V';
			else
			{
				fprintf(output_file, "%s%s",
					input_string, "\n\n");
				abort_run("Invalid parameter");
			}
			goto L1;
		}

		case 10: /* "HIT_STRUCTURE_ROWS" */
		{
			fscanf(parameters_file, "%d",
				&hit_structure_rows);
			goto L1;
		}

		case 11: /* "FAIL_SCORE" */
		{
			fscanf(parameters_file, "%d", &fail_score);
			goto L1;
		}

		case 12: /* "COST_FACTOR" */
		{
			fscanf(parameters_file, "%lg", &cost_factor);
			goto L1;
		}

		case 13: /* "KEEP_ROWS" */
		{
			fscanf(parameters_file, "%d",
				&keep_rows);
			if (keep_rows > MAX_NEW_ARRAY_ROWS)
				abort_run("keep_rows too large");
			goto L1;
		}

		case 14: /* "DRIVING_KEEP_ROWS" */
		{
			fscanf(parameters_file, "%d",
				&driving_keep_rows);
			goto L1;
		}

		case 15: /* "FULL_ALIGNMENT_LIMIT" */
		{
			fscanf(parameters_file, "%d",
				&full_alignment_limit);
			goto L1;
		}

		case 16: /* "GRAMMAR_LIMIT" */
		{
			fscanf(parameters_file, "%d",
				&grammar_limit);
			goto L1;
		}

		case 17: /* "EXTRACTION_LIMIT" */
		{
			fscanf(parameters_file, "%d",
				&extraction_limit);
			goto L1;
		}

		case 18: /* "FIGURE_ID" */
		{
			fscanf(parameters_file, "%s", figure_ID);
			goto L1;
		}

		case 19: /* "COMBINATION_LIMIT" */
		{
			fscanf(parameters_file, "%d", &combination_limit);
			goto L1;
		}

		case 20: /* "MAX_OLD_GAP" */
		{
			fscanf(parameters_file, "%d", &max_old_gap);
			goto L1;
		}

		case 21: /* "MAX_DRIVING_GAP" */
		{
			fscanf(parameters_file, "%d",
				&max_driving_gap);
			goto L1;
		}

		case 22: /* "MAX_TARGET_GAP" */
		{
			fscanf(parameters_file, "%d", &max_target_gap);
			goto L1;
		}

		case 23: /* "MAX_UNSUPPORTED_CYCLES" */
		{
			fscanf(parameters_file, "%d",
				&max_unsupported_cycles);
			goto L1;
		}

		case 24: /* "MAX_ALIGNMENTS_IN_ONE_CYCLE" */
		{
			fscanf(parameters_file, "%d",
				&max_alignments_in_one_cycle);
			goto L1;
		}

		case 25: /* "ORIENTATION" */
		{
			fscanf(parameters_file, "%s", input_string);
			if (strcmp(input_string, "LANDSCAPE") == 0)
				orientation = LANDSCAPE;
			else if (strcmp(input_string, "PORTRAIT") == 0)
				orientation = PORTRAIT;
			else
			{
				fprintf(output_file, "%s%s",
					input_string, "\n\n");
				abort_run("Invalid parameter");
			}
			goto L1;
		}

		case 26: /* "WRITE_AL_CHARS_LENGTH" */
		{
			fscanf(parameters_file, "%d",
				&write_al_chars_length);

			goto L1;
		}

		case 27: /* "WRITE_SECTION_CHARS_LENGTH" */
		{
			fscanf(parameters_file, "%d",
				&write_section_chars_length);

			goto L1;
		}

		case 28: /* "MINIMUM_FONT_HEIGHT" */
		{
			fscanf(parameters_file, "%lg",
				&minimum_font_height);

			// Find the smallest available font size
			// which is larger than or equal to the 
			// minimum_font_height and re-set the value
			// of minimum font height to this value.

			for (i = 0; i < FONT_SET_SIZE; i++)
			{
				if (font_heights[i] >=
					minimum_font_height)
				{
					j = i;
					break;
				}
			}

			if (i >= FONT_SET_SIZE)
				abort_run("MINIMUM_FONT_HEIGHT is \
													larger than the largest \
													available font size");

			if (minimum_font_height == font_heights[j])
			{
				fprintf(output_file, "%s%1.2f%s",
					"MINIMUM_FONT_HEIGHT is ",
					minimum_font_height,
					" pt\n\n");
			}
			else
			{
				fprintf(output_file, "%s%1.2f%s",
					"MINIMUM_FONT_HEIGHT changed from ",
					minimum_font_height,
					" pt to ");
				minimum_font_height = font_heights[j];
				fprintf(output_file, "%1.2f%s",
					minimum_font_height,
					" pt\n\n");
			}

			goto L1;
		}

		case 29: /* "MAXIMUM_FONT_HEIGHT" */
		{
			fscanf(parameters_file, "%lg",
				&maximum_font_height);

			// Find the largest available font size
			// which is smaller than or equal to the 
			// maximum_font_height and re-set the value
			// of maximum font size to this value.

			for (i = FONT_SET_SIZE - 1; i >= 0; i--)
			{
				if (font_heights[i] <=
					maximum_font_height)
				{
					j = i;
					break;
				}
			}

			if (i < 0) abort_run("MAXIMUM_FONT_HEIGHT is \
															   smaller than the smallest \
															   available font size");

			if (maximum_font_height == font_heights[j])
			{
				fprintf(output_file, "%s%1.2f%s",
					"MAXIMUM_FONT_HEIGHT is ",
					maximum_font_height,
					" pt\n\n");
			}
			else
			{
				fprintf(output_file, "%s%1.2f%s",
					"MAXIMUM_FONT_HEIGHT changed from ",
					maximum_font_height,
					" pt to ");
				maximum_font_height = font_heights[j];
				fprintf(output_file, "%1.2f%s",
					maximum_font_height,
					" pt\n\n");
			}

			goto L1;
		}

		case 30: /* "N_GRAMMARS" */
		{
			fscanf(parameters_file, "%d",
				&n_grammars);

			goto L1;
		}

		case 31: /* "CREATE_PATTERNS_DIAGNOSTIC" */
		{
			fscanf(parameters_file, "%s", input_string);
			if (strcmp(input_string, "ON") == 0)
				create_patterns_diagnostic = ON;
			else if (strcmp(input_string, "OFF") == 0)
				create_patterns_diagnostic = OFF;
			else
			{
				fprintf(output_file, "%s%s",
					input_string, "\n\n");
				abort_run("Invalid parameter");
			}
			goto L1;
		}

		default:
		{
			fprintf(output_file, "%d%s",
				parameter_index, "\n\n");
			abort_run("Default reached in read_parameters");
		}
		}
	L1: fscanf(parameters_file, "%s", input_string);
	}

	if (maximum_font_height < minimum_font_height)
		abort_run("MAXIMUM_FONT_HEIGHT is smaller \
		than MINIMUM_FONT_HEIGHT");
} /* read_parameters */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**		Load the SP corpus from the text file specified (using
**	the `syntax buffer' size specified), converting it to
**	it's internal format during the process.
**
** CALLING SEQUENCE:
**
**	void load() ;
**
** FORMAL ARGUMENTS:
**
**	Return value:	None.
**
** IMPLICIT INPUTS:
**
**	Input to load() from parameters file.
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

void load()
{
	read_key();

	/* Read in syntax. */

	syntax_buffer = new char[BUFSIZE];
	if (syntax_buffer == NIL)
		abort_run("Out of memory in 'load'");

	in_syntax();

	identification_symbols_marked = false;

	if (corpus = (group*)top_object())
		fprintf(output_file, "Corpus loaded successfully.\n\n");
	else
	{
		bad_syntax();
		abort_run("Error in syntax in 'load'");
	}

	delete[] syntax_buffer;

} /* load */

/*********************************************************************/

/*
** FUNCTIONAL DESCRIPTION:
**
**	A function for incrementing an integer value by 1 and, at
**	the same time, checking whether it equals or exceeds a specified limit.
**	If it does exceed the limit, the program is aborted with a
**	specified message.
**
** CALLING SEQUENCE:
**
**	void plus_one(int *var1, int limit, char *message)
**
** FORMAL ARGUMENTS:
**
**	Return value:	void.
**
**	var1:			A pointer to an external variable holding
**					the input value.
**	limit:			The ceiling which must not be equalled or exceeded.
**	message:		The message to go to write file if the ceiling is
**					breached.
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

void plus_one(int* var1, int limit, const char* message)
{
	(*var1)++;
	if (*var1 >= limit) abort_run(message);
} // plus_one

/*********************************************************************/
