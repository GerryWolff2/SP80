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
** TITLE:		SP71_head.h
**
** VERSION: 	SP71, v 9.0
**
** ABSTRACT:	The header file to be inherited by all modules in
**				the SP system. It includes the class structure for the
**				program.
**
** ENVIRONMENT: This file should be placed on the `include' path of
**				the C++ compiler so that it may be located correctly.
**				This file produces no executable code.
**
** AUTHOR:		Gerry Wolff
**
** MODIFIED BY:
**
** SP71:
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

#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#pragma warning(disable : 4996)		// unsafe CRT functions (_CRT_SECURE_NO_WARNINGS).

// Definitions.

#define DIAGNOSTIC1 FALSE
#define DIAGNOSTIC2 FALSE
#define DIAGNOSTIC3 FALSE
#define DIAGNOSTIC4 FALSE
#define DIAGNOSTIC5 FALSE
#define DIAGNOSTIC6 FALSE

#define TRUE 1
#define FALSE 0
#define A_DIFFERENT_FROM_B 2
#define A_SUBSET_B -1
#define A_SAME_AS_B 0
#define B_SUBSET_A 1
#define FULL_A 1 // The New pattern and all Old patterns are fully matched.
#define FULL_B 2 // The Old patterns are fully matched but the New pattern
	// is not fully matched. There are no unmatched New symbols within
	// the scope of any one Old pattern.
#define FULL_C 3 // The Old patterns are fully matched but the New pattern
	// is not fully matched. There may be unmatched New symbols within
	// the scope of any one Old pattern.
#define PARTIAL 4 // What is left after FULL_A, FULL_B and FULL_C have been removed.
#define BEST_ALIGNMENT_PRINT_LIMIT 10
#define BASIC_PATTERN 1
#define ABSTRACT_PATTERN 2
#define CODE_PATTERN 3
#define SUB_ALIGNMENT 4
#define COMPOSITE_ALIGNMENT 5
#define ALIGNMENT_FROM_PARSING 6
#define OUTPUT_COUNTER_1 5
#define OUTPUT_COUNTER_2 10
#define GAP_BETWEEN_AL_COLS 3
#define H 1
#define V 2
#define HIT_NODE_ARRAY_SIZE 10000
#define CODE_BIT_COST 4.0
#define BRACKET_BIT_COST 2.0
#define VERY_LARGE_NUMBER 1000000000
#define ALIGNMENTS_FOR_LEARNING 3 // Controls the
	// number of alignments fed into 'learning' at the end
	// of the 'recognition' phase for each pattern from New.
#define PRINTING_POINTS_PER_MM 2.8452756
#define CHARACTER_HEIGHT_WIDTH_RATIO 1.95
#define FONT_SET_SIZE 10
#define PRINTING_SPACE_LONG_SIDE 247 // The length (in mm) of the long
	// side of the space available for printing on an A4 sheet.
#define PRINTING_SPACE_SHORT_SIDE 160 // The length (in mm) of the short
	// side of the space available for printing on an A4 sheet.
#define LANDSCAPE_CHARS_PER_LINE 116 // Characters (12 pt) in one line of
	// an A4 page in landscape orientation (25 mm margins).
#define LANDSCAPE_LINES_PER_PAGE 21 // Lines in one A4 page in
	// landscape orientation (12 pt characters, 25 mm margins).
#define KEY_ARRAY_SIZE 1000
#define ELLIPSIS_MARGIN 5 // write_al_chars_limit must be at least
	// ELLIPSIS_MARGIN smaller than WRITE_ALIGNMENT_CHAR_LENGTH
	// to allow space for inserting "..." at the end of truncated
	// alignments.
#define HIGH_VALUE 1000000000 // An arbitarily high integer value used
	// in unify_best and elsewhere.
#define LOW_VALUE -1000000000 // An arbitarily low integer value used
	// in unify_best and tree_object::sort_children().
#define CONTEXT_SYMBOL 1 // Value for the type of a symbol.
#define DATA_SYMBOL 2 // Value for the type of a symbol.
#define LEFT_BRACKET 3 // Value for the type of a symbol.
#define RIGHT_BRACKET 4 // Value for the type of a symbol.
#define UNIQUE_ID_SYMBOL 5 // Value for the type of a symbol.
#define IDENTIFICATION 1 // Value for the status of a symbol.
	// See sp70_od, %47.
#define CONTENTS 2 // Value for the status of a symbol.
	// See sp70_od, %47.
#define BOUNDARY_MARKER 3 // Value for the status of a symbol.
	// Only applies to brackets at the extreme ends of a pattern. 
	// Brackets that are internal to a pattern have the status CONTENTS. 
	// See sp62_od, %19.
#define ORIGINAL 1
#define COPY 2
#define MAX_ALIGNMENT_DEPTH 50
#define MAX_NEW_ARRAY_ROWS 200
#define SCALING_FACTORS_SIZE 100
#define SMALL_SCRATCH_ARRAY_SIZE 10
#define MEDIUM_SCRATCH_ARRAY_SIZE 100
#define LARGE_SCRATCH_ARRAY_SIZE 1000
#define AL_CONTENTS_ARRAY_WIDTH 50
#define WRITE_ALIGNMENT_C_COLS 20
#define MCOLS 200 /* The number of columns in the best_hit_structure[] */
#define START 0
#define CONTINUE 1
#define SEQUENCE 1
#define GROUP 2
#define ON 1
#define OFF 0
#define LETTERS 2
#define DIGITS 3
#define LANDSCAPE 0
#define PORTRAIT 1
#define ALREADY_VISITED_1 1
#define ALREADY_VISITED_2 2
#define UNKNOWN 2
#define BUFSIZE 5000
#define NIL 0
#define NULL_VALUE -1
#define STRING_LENGTH 20
#define LONG_STRING_LENGTH 100
#define ESCAPE 033
#define DEL '\177'
#define PRINT_ALL_FREQUENCIES 3
#define PRINT_GROUP_FREQUENCIES 2
#define PRINT_SEQUENCE_FREQUENCY 1
#define PRINT_NO_FREQUENCIES 0
#define MAX_WRITE_LINE 75

// Macro definitions

#define list_for(X, Y, Z) for (X = (Y *)Z->get_first_child();X != NIL ; \
	X = (Y *)Z->get_next_child())

#define list_for_el_pos(X, Y, Z, P) for (X = (Y *)Z->get_next_child_by_el_pos(&P); \
	X != NIL; X = (Y *)Z->get_next_child_by_el_pos(&P))

// Definitions of classes and declarations of methods.

// CLASS STRUCTURE

//	base_element
//		list_element
//		alignment_element

//	base_object
//		symbol
//		combination
//		grammar
//		tree_object
//			group
//			cost_tree_object
//				sequence
//				hit_node

class base_element;
class list_element;
class alignment_element;

class base_object;
class symbol;
class combination;
class grammar;
class tree_object;
class group;
class cost_tree_object;
class sequence;
class hit_node;

struct key_entry
{
	char* abbreviated_symbol;
	char* complete_symbol;
};

struct row_entry
{
	sequence* row_pattern;
	list_element* patt_last_el_pos; // This is used to record the
		// last (ie, most recent) el_pos for row_pattern where the
		// corresponding symbol has been written into
		// write_al_chars[][].
	int fe_col_in_write_al_chars; // Keeps track of the first empty
		// column ***for this row*** in write_al_chars[][].
	bool pattern_is_finished; // true when all the symbols in
		// the row_pattern have been written, false otherwise.
};

struct hn_entry
{
	hit_node* new_hit_node;
	hit_node* hn_parent;
};

// Definitions of global variables

extern group* generalisation_list1;
extern group* generalisation_list2;
extern int new_patterns_counter;
extern double average_symbol_type_cost;
extern double average_CONTEXT_SYMBOL_type_cost;
extern int id_c_symbol_constraint;
extern group* created_patterns;
extern group* set_of_combinations;
extern group* best_combinations;
extern group* brackets_list;
extern int cycle;
extern int phase;
extern bool right_bracket_expected;
extern int number_of_patterns_in_new;
extern int original_number_of_patterns_in_old;
extern int final_number_of_patterns_in_old;
extern int number_of_symbols_in_new;
extern int number_of_symbols_in_old;
extern int fe_hit_node_array;
extern struct hn_entry hit_node_array[];
extern sequence* receptacle_pattern;
extern int half_hit_structure_rows;
extern int total_number_of_alignments;
extern sequence* symbol_selection_array[][MCOLS];
extern clock_t time1;
extern clock_t time2;
extern int original_alphabet_size;
extern int current_alphabet_size;
extern char* in_filename;
extern char* parameters_filename;
extern char* out_filename;
extern char* latex_filename;
extern char* plot_filename;
extern FILE* input_file;
extern FILE* parameters_file;
extern FILE* output_file;
extern FILE* latex_file;
extern FILE* plot_file;
extern int context_number;
extern int unique_id_number;
extern bool identification_symbols_marked;
extern hit_node** half_list;
extern char** write_al_chars;
extern char** write_al_vacant_slots;
extern double font_heights[];
extern int fe_key_array;
extern struct key_entry key_array[];
extern struct row_entry row_array[];
extern const int max_alignment_depth; /* The maximum depth of an sequence
	that can be written (symbols). */
extern const int max_alignment_length;
extern int write_al_chars_length;
extern int write_section_chars_length;
extern double minimum_font_height;
extern double maximum_font_height;
extern int n_grammars;
extern int create_patterns_diagnostic;
extern int show_all_parsing_alignments;
extern char alignment_format;
extern int hit_structure_rows;
extern int patterns_to_keep;
extern double cost_factor;
extern hit_node** leaf_array; /* leaf_array[] is used for recording the
	leaf nodes of the hit structure. */
extern int* sort_array; /* sort_array[] is used for sorting the
	entries in leaf_array[]. */
extern hit_node* hit_root;
extern hit_node* hn_master;
extern int fe_sort; /* The first empty entry in sort_array[]. */
extern group* corpus;
extern group* old_patterns;
extern group* symbols_in_new;
extern group* new_patterns;
extern group* set_of_sequences;
extern sequence* current_new_pattern;
extern group* original_symbols_in_corpus;
extern group* symbol_types_in_old;
extern group* parsing_alignments;
extern group* cumulative_parsing_alignments;
extern group* selected_cumulative_parsing_alignments;
extern group* set_of_grammars;
extern grammar* naive_grammar;
extern bool probabilities;
extern bool show_al_structure;
extern bool show_hit_structure;
extern bool show_unselected_alignments;
extern bool verbose;
extern int use_abbreviations;
extern int keep_rows;
extern int driving_keep_rows;
extern int full_alignment_limit;
extern int grammar_limit;
extern int extraction_limit;
extern int not_used;
extern int fail_score;
extern char figure_ID[];
extern int combination_limit;
extern int max_old_gap;
extern int max_driving_gap;
extern int max_target_gap;
extern int max_unsupported_cycles;
extern int max_alignments_in_one_cycle;
extern int orientation;
extern int group_ID_number;
extern int sequence_ID_number;
extern int hit_node_ID_number;
extern int combination_ID_number;
extern int grammar_ID_number;
extern sequence** alignments_array;

/* Declarations of global functions. */

extern void generalise_patterns(void);
extern void add_symbol_to_generalisation_list1(int);
extern bool isnumeric(char*);
extern void purge_old_patterns(int n_grammars, sequence* cnp);
extern void extract_patterns_1();
extern void extract_patterns_2();
extern symbol* make_bracket_symbol(int, int, int);
extern void increment_index(int*, int, char*);
extern sequence* recognise_select_and_write();
extern void probabilities_of_inferences(sequence*);
extern void write_new_and_old();
extern void assign_symbol_frequencies_and_costs(group*);
extern void find_symbol_frequencies(group*, bool, bool, sequence*);
extern void assign_type_to_symbols();
extern void assign_status_to_symbols();
extern bool recognise(sequence*);
extern void calculate_and_assign_frequencies_and_costs(group* symbol_types, bool ref_is_corpus);
extern void modified_sizes_of_sfe_codes(group*);
extern bool is_unique(sequence*, symbol*,
	sequence**);
extern sequence* unify_best(int, sequence*);
extern void set_up_hit_structure();
extern sequence* make_one_alignment(hit_node*, sequence*);
extern bool mismatch_found(hit_node*, sequence*);
extern void prepare_hit_sequence(hit_node*);
extern symbol* make_column(int, sequence*, symbol*,
	sequence*, symbol*, int, int, bool);
extern void print_pattern_cycle(bool, sequence*);
extern void SP_main(void);
extern void time_now();
extern void year();
extern void output_alignment(sequence*);
extern void sort_leaf_array(int, int);
extern bool is_better_than(double, double);
extern void make_letter_abbreviations();
extern void make_digit_abbreviations();
extern void write_key();
extern double log_2(double);
extern void plus_one(int*, int, const char*);
extern void write_hit_structure(hit_node*, int);
extern base_object* get_child(list_element**);
extern void exit_routine(int);
extern void clear_nodes(hit_node*);
extern void read_parameters();
extern void write_lines(FILE*, const char*, int);
extern void abort_run(const char*);
extern void write_message(const char*);
extern void load();
extern hit_node* get_leaf_nodes_in_order(int*, sequence**, int*);

// CLASSES

class base_element
{
public:
	base_element(base_object* obj1 = NIL) { el_obj = obj1; }
	base_element(base_element& el) { el_obj = el.el_obj; }
	~base_element() {}

	void set_el_obj(base_object* obj1) { el_obj = obj1; }
	base_object* get_el_obj() { return(el_obj); }
protected:
	base_object* el_obj; // Points to a symbol when used
		// within a basic pattern. Points to an object 
		// containing an array of alignment_elements
		// when used within an alignment.
}; // class base_element

class alignment_element : public base_element
{
public:
	alignment_element(base_object* obj1 = NIL) : base_element(obj1)
	{
		same_column_above = same_column_below = NULL_VALUE;
		original_pattern = NIL;
	}
	alignment_element(alignment_element& al_el1) :
		base_element(al_el1)
	{
		same_column_above = al_el1.get_same_column_above();
		same_column_below = al_el1.get_same_column_below();
		original_pattern = al_el1.get_original_pattern();
	}
	~alignment_element() {} // Notice that
		// any el_obj which attached to the alignment
		// element is *not* deleted.

	void set_same_column_above(int row) { same_column_above = row; }
	int get_same_column_above() { return(same_column_above); }
	void set_same_column_below(int row) { same_column_below = row; }
	int get_same_column_below() { return(same_column_below); }
	void set_original_pattern(sequence* orig_patt)
	{
		original_pattern = orig_patt;
	}
	sequence* get_original_pattern() { return(original_pattern); }
	int get_orig_patt_int_pos();
	void show_al_structure();
protected:

	// The el_obj field (at the level of base_element) is used
	// to enter the address of the symbol for this al_el.

	int same_column_above; // An index to to an al_el in a row
		// above which contains a matching symbol.
	int same_column_below; // An pointer to to an al_el in a row 
		// below which contains a matching symbol.
	sequence* original_pattern; // The pattern from which
		// this row of the sequence and this al_el
		// was derived.

	// Note: the column position of each al_el is the 'position'
	// field of the column in which the al_el appears.
	// The row position of each al_el is the index value
	// for the al_el within the column in which the al_el
	// appears.
}; // class alignment_element

class list_element : public base_element
{
public:
	list_element(base_object* obj1 = NIL) : base_element(obj1)
	{
		parent = NIL; position = NULL_VALUE; next = NIL;
	}
	list_element(list_element& el1) : base_element(el1)
	{
		parent = NIL; position = NULL_VALUE; next = NIL;
	}
	~list_element() {}

	bool this_is_last_position()
	{
		if (next == NIL) return(true); else return(false);
	}
	void set_parent(base_object* parent1) { parent = parent1; }
	base_object* get_parent() { return(parent); }
	void set_position(int pos) { position = pos; }
	int get_position() { return(position); }
	void set_next(list_element* el1) { next = el1; }
	list_element* get_next() { return(next); }
protected:
	base_object* parent; // The object containing a pointer
		// to the start of the list.
	int position; // The position in the list.
	list_element* next; // The next list_element in the list.
}; // class list_element

class base_object
{
public:
	base_object() { frequency = 1; }
	base_object(base_object& obj) { frequency = obj.frequency; }
	virtual ~base_object() {}
	virtual base_object* clone() { return(NIL); }

	virtual bool is_symbol() { return(false); }
	void increment_frequency() { frequency++; }
	void add_to_frequency(int fr) { frequency += fr; }
	void set_frequency(int fr) { frequency = fr; }
	int get_frequency() { return(frequency); }
	virtual void mark_parent_and_int_positions_recursive() {}
	virtual int count_number_of_symbols() { return(0); }
	virtual double total_of_symbol_sizes() { return(0); }
	virtual void invert_symbol_status() {}
	virtual void symbol_set_recursion(group*) {}
	virtual void wto(int) {}
	virtual void set_encoding_cost(double) {}
protected:
	int frequency;
}; // class base_object

class symbol : public base_object // This is a class for simple 
	// symbols and for columns of an alignment.
{
public:
	symbol(const char* nm = NIL, int al_depth = 1, int hn_ID = NULL_VALUE);
	symbol(symbol& s);
	~symbol();

	base_object* clone() {
		symbol* result = new symbol(*this);
		return(result);
	}
	symbol* shallow_copy(); // Makes a copy of a symbol 
		// without any information about rows (assumes a depth of 1).

	void assign_frequency_and_cost();
	bool is_symbol() { return(true); }
	bool name_matches(symbol* s)
	{
		if (strcmp(name, s->get_name()) == 0)
			return(true);
		else return(false);
	}
	bool name_matches_string(char* str)
	{
		if (strcmp(name, str) == 0)
			return(true);
		else return(false);
	}
	bool name_is(const char* nm)
	{
		if (strcmp(nm, name) == 0) return(true);
		else return(false);
	}
	void set_name(const char* nm)
	{
		delete name; name = new char[strlen(nm) + 1];
		strcpy(name, nm);
	}
	char* get_name() { return(name); }
	int count_number_of_symbols() { return(1); }
	double total_of_symbol_sizes() { return(bit_cost); }
	void wto(int);
	void set_encoding_cost(double e_cost) { bit_cost = e_cost; }
	void set_bit_cost(double x) { bit_cost = x; }
	double get_bit_cost() { return(bit_cost); }
	void set_type(int tp) {
		if (tp != NULL_VALUE
			&& tp != CONTEXT_SYMBOL && tp != DATA_SYMBOL
			&& tp != LEFT_BRACKET && tp != RIGHT_BRACKET
			&& tp != UNIQUE_ID_SYMBOL)
			abort_run("Invalid type for symbol::set_type");
		type = tp;
	}
	int get_type() { return(type); }
	void set_status(int st) {
		if (st != NULL_VALUE
			&& st != IDENTIFICATION && st != CONTENTS
			&& st != BOUNDARY_MARKER)
			abort_run("Invalid status for symbol::set_status().");
		status = st;
	}
	int get_status() { return(status); }
	void mark_parent_and_int_positions_recursive() {}
	bool has_no_function();

	// Alignment methods

	symbol* find_unmatched_symbol()
	{
		if (is_a_hit() == true)
			return(NIL); if (sequence_depth == 1) return(NIL);
		int i; symbol* s1; for (i = 0; i < sequence_depth; i++)
		{
			s1 = get_row_symbol(i); if (s1 != NIL) return(s1);
		}
		return(NIL);
	}
	bool contains_contents_symbol();
	void set_symbol_matches();

	symbol* get_row_symbol(int row) {
		if (sequence_depth == 1 ||
			row > sequence_depth || row < 0)
			abort_run("Invalid value for row in get_row_symbol");
		alignment_element* al_el1 = this->get_al_el(row);
		return((symbol*)al_el1->get_el_obj());
	}

	int get_row_orig_int_pos(int row) {
		if (sequence_depth == 1 ||
			row > sequence_depth || row < 0)
			abort_run("Invalid value for row in get_row_symbol");
		alignment_element* al_el1 = this->get_al_el(row);
		return(al_el1->get_orig_patt_int_pos());
	}

	sequence* get_row_pattern(int row) {
		if (sequence_depth == 1 ||
			row > sequence_depth || row < 0)
			abort_run("Invalid value for row in get_row_symbol");
		alignment_element* al_el1 = this->get_al_el(row);
		return(al_el1->get_original_pattern());
	}

	void set_row_pattern(int row, sequence* pattern1)
	{
		if (sequence_depth == 1 ||
			row > sequence_depth || row < 0)
			abort_run("Invalid value for row in set_row_pattern");
		alignment_element* al_el1 = this->get_al_el(row);
		al_el1->set_original_pattern(pattern1);
	}

	void set_row_symbol(int row, symbol* symbol1)
	{
		if (sequence_depth == 1 ||
			row > sequence_depth || row < 0)
			abort_run("Invalid value for row in set_row_symbol");
		alignment_element* al_el1 = this->get_al_el(row);
		al_el1->set_el_obj(symbol1);
	}
	void show_al_structure();

	alignment_element* get_al_el(int row)
	{
		if (sequence_depth == 1 ||
			row > sequence_depth || row < 0)
			abort_run("Invalid value for row in get_al_el");
		return(al_el_array + row);
	}

	void set_sequence_depth(int al_depth)
	{
		sequence_depth = al_depth;
	}

	int get_sequence_depth() { return(sequence_depth); }

	void make_al_el_array(int sequence_depth)
	{
		al_el_array = new
			alignment_element[sequence_depth];
	}

	void set_al_el_array(alignment_element* al_el)
	{
		al_el_array = al_el;
	}

	alignment_element* get_al_el_array() { return(al_el_array); }

	void set_h_node_ID(int ID)
	{
		h_node_ID = ID;
	}
	int get_h_node_ID() { return(h_node_ID); }

	void set_is_a_hit(bool is_hit)
	{
		symbol_is_a_hit = is_hit;
	}

	bool is_a_hit() { return(symbol_is_a_hit); }

	symbol* get_first_original_symbol()
	{
		symbol* s; for (int i = 0; i < sequence_depth; i++)
		{
			s = get_row_symbol(i); if (s != NIL) return(s);
			if (i >= sequence_depth) {
				abort_run("Error in get_first_original_symbol()");
				return(NIL);
			}
		}
	}

	bool contains_matching_symbols()
	{
		symbol* s; int counter = 0;
		for (int i = 0; i < sequence_depth; i++)
		{
			s = get_row_symbol(i); if (s == NIL) continue;
			counter++;
		} if (counter > 1) return(true);
		else return(false);
	}

protected:
	void symbol_set_recursion(group* set);
	char* name;
	double bit_cost;
	int type; // Records whether a given symbol is
		// CONTEXT_SYMBOL, LEFT_BRACKET, RIGHT_BRACKET or DATA_SYMBOL.
	int status; // Records whether a given symbol is IDENTIFICATION
		// or CONTENTS or BOUNDARY_MARKER for the parent pattern.

	// Alignment fields

	int sequence_depth;
	alignment_element* al_el_array;
	int h_node_ID; // The ID number of the hit node (if any)
		// which corresponds to this symbol. The ID is
		// stored rather than the hit node itself so that
		// the hit structure can be properly cleared at the
		// end of each cycle and memory is not cluttered
		// with hit nodes from previous cycles.
	bool symbol_is_a_hit; // If a column/symbol contains two or
		// more symbols then it is a 'hit' column.
}; // class symbol

class tree_object : public base_object
{
public:
	tree_object() : base_object() {
		first_element = NIL;
		current_el_pos = NIL;
		number_of_children = 0;
	}
	tree_object(tree_object& tn);

	bool has_member(base_object* obj1)
	{
		list_element* pos1 = NIL; base_object* obj2;
		list_for_el_pos(obj2, base_object, this, pos1)
			if (obj1 == obj2) return(true);
		return(false);
	}
	void transfer_children(tree_object* recipient)
	{
		recipient->set_first_element(first_element);
		first_element = NIL;
	}
	void sort_children(); // Sorts the children of a node by
		// compression_difference.
	void delete_children() {
		if (first_element == NIL) return;
		base_object* child;
		while (child = this->extract_first_child())
			delete child;
	}
	void delete_child(base_object* child) {
		extract_child(child);
		delete child;
	}
	void sort_by_name();
	bool is_symbol() { return(false); }
	base_object* get_child_by_el_pos(list_element** position)
	{
		if ((*position) == NIL)
			abort_run("Invalid parameter in get_child_by_el_pos()");
		return((*position)->get_el_obj());
	}
	base_object* get_child_by_int_pos(int position);
	base_object* get_next_child();
	base_object* get_next_child_by_el_pos(list_element** position);
	base_object* get_preceding_child_by_el_pos(list_element** position);
	base_object* get_preceding_child(base_object* child);
	list_element* get_preceding_el_pos(list_element* position);
	list_element* get_next_el_pos(list_element** position)
	{
		return((*position)->get_next());
	}
	base_object* get_first_child() {
		if (first_element == NIL)
			return(NIL); current_el_pos = first_element;
		return(first_element->get_el_obj());
	}
	base_object* get_last_child();
	list_element* get_el_pos_by_int_pos(int position);
	list_element* get_el_pos_by_child(base_object* obj);
	list_element* get_first_el_pos() { return(first_element); }
	list_element* get_last_el_pos()
	{
		list_element* el_pos1 = first_element, * el_pos2 = NIL;
		while (el_pos1 != NIL) {
			el_pos2 = el_pos1;
			el_pos1 = el_pos1->get_next();
		} return(el_pos2);
	}
	list_element* get_next_el_pos()
	{
		if (current_el_pos == NIL) return(first_element);
		return(current_el_pos->get_next());
	}
	int get_first_int_pos()
	{
		return(first_element->get_position());
	}
	int get_last_int_pos()
	{
		list_element* el_pos1 = get_last_el_pos();
		return(el_pos1->get_position());
	}
	void initialise() { current_el_pos = NIL; }
	void set_current_el_pos(list_element* el_pos)
	{
		current_el_pos = el_pos;
	}
	list_element* get_current_el_pos()
	{
		return(current_el_pos);
	}
	int get_current_int_pos()
	{
		if (current_el_pos == NIL) return(NULL_VALUE);
		else return(current_el_pos->get_position());
	}
	int get_int_pos_by_child(base_object* child); // Obtain integer
		// position of child from the value stored in its list element.
	int find_int_pos_by_child(base_object* child); // Obtain integer
		// position of child by counting.
	void add_child(base_object* child);
	void add_child_at_start(base_object* child);
	void extract_child(base_object* child);
	base_object* extract_first_child()
	{
		if (base_object* child = this->get_first_child())
		{
			this->extract_child(child); return(child);
		}
		else return(NIL);
	}
	void precede(base_object* child1, base_object* child2);
	void follow(base_object* child1, base_object* child2);
	void release_children();
	bool this_is_last_child(base_object* child);
	bool is_empty() {
		if (first_element == NIL) return(true);
		else return(false);
	}
	symbol* contains_copy_of(symbol* symbol1);
	void mark_parent_and_int_positions_recursive();
	void mark_parent_and_int_positions_non_recursive();
	void set_first_element(list_element* el1)
	{
		first_element = el1;
	}
	list_element* get_first_element() { return(first_element); }
	void write_tree_object(int print_code);
	int count_number_of_children();
	void increment_child_count() { number_of_children++; }
	void set_number_of_children(int no_of_children)
	{
		number_of_children = no_of_children;
	}
	int get_number_of_children() { return(number_of_children); }
	int count_number_of_symbols()
	{
		int no_of_symbols = 0; base_object* child;
		list_for(child, base_object, this)
			no_of_symbols += child->count_number_of_symbols();
		return(no_of_symbols);
	}
	double total_of_symbol_sizes()
	{
		double cumulative_size = 0; base_object* child;
		list_for(child, base_object, this)
			cumulative_size += child->total_of_symbol_sizes();
		return(cumulative_size);
	}
	void invert_symbol_status()
	{
		base_object* child; list_for(child, base_object, this)
			child->invert_symbol_status();
	}
	void symbol_set_recursion(group* set) // Called from 
				// symbol_set()
	{
		base_object* child; this->initialise();
		while (child = this->get_next_child())
			child->symbol_set_recursion(set);
	}
	virtual void print_ID() = 0;
	int get_ID() { return(ID); }
	group* symbol_set();
	bool contains(base_object* obj1)
	{
		list_element* el_pos1 = NIL; base_object* obj2;
		list_for_el_pos(obj2, base_object, this, el_pos1)
			if (obj1 == obj2) return(true);
		return(false);
	}
protected:
	list_element* first_element;
	list_element* current_el_pos;
	int number_of_children; // Number of children of this node.
	void finish_deletion();
	int ID;
}; // class tree_object

class group : public tree_object
{
public:
	group() : tree_object() { ID = group_ID_number++; }
	group(group& gr) : tree_object(gr) { ID = group_ID_number++; }
	~group() { this->finish_deletion(); }
	base_object* clone() {
		group* result = new group(*this);
		return(result);
	}

	void add_or_merge_patterns(sequence*);
	bool group_matches(group* gr1);
	void write_patterns(const char* heading, int origin, int selection);
	void write_patterns_with_details();
	void print_ID() { fprintf(output_file, "%s%d", "ID", ID); }
	void wto(int print_code);
	void compile_frequencies(group* symbol_set,
		sequence* last_pattern);
	group* compile_alphabet(int* alphabet_size);
	void sort_by_ID();
	void sort_by_compression_difference();
	void write_IDs(int, sequence* cnp);
	void insert_sequence_in_order_of_CD(sequence* al1);
	bool contains_duplicates();
	bool add_pattern_without_duplicates(sequence* seq1);
	bool add_pattern_without_copies_or_duplicates(sequence* seq1);
	bool add_symbol_without_copies_or_duplicates(symbol* symbol1);
}; // class group

class cost_tree_object : public tree_object
{
public:
	cost_tree_object() : tree_object()
	{
		new_symbols_cost = encoding_cost = compression_ratio =
			compression_difference = NULL_VALUE;
	}
	cost_tree_object(cost_tree_object& cto) : tree_object(cto)
	{
		new_symbols_cost = cto.get_new_symbols_cost();
		encoding_cost = cto.get_encoding_cost();
		compression_ratio = cto.get_compression_ratio();
		compression_difference =
			cto.get_compression_difference();
	}
	~cost_tree_object() {}

	void set_new_symbols_cost(double NSC)
	{
		new_symbols_cost = NSC;
	}
	double get_new_symbols_cost() { return(new_symbols_cost); }
	void set_encoding_cost(double EC)
	{
		encoding_cost = EC;
	}
	double get_encoding_cost() { return(encoding_cost); }
	void set_compression_ratio(double CR)
	{
		compression_ratio = CR;
	}
	double get_compression_ratio() { return(compression_ratio); }
	void set_compression_difference(double CD)
	{
		compression_difference = CD;
	}
	double get_compression_difference()
	{
		return(compression_difference);
	}
protected:
	double new_symbols_cost;
	double encoding_cost;
	double compression_ratio;
	double compression_difference;
}; // class cost_tree_object

class sequence : public cost_tree_object
{
public:
	sequence(int org1 = BASIC_PATTERN) : cost_tree_object()
	{
		ID = sequence_ID_number++; sequence_depth = 1;
		leaf_node_ID = NULL_VALUE; abs_P = NULL_VALUE;
		total_cost = 0; origin = org1; keep = false;
		degree_of_matching = NULL_VALUE;
		new_this_cycle = true;
		driving_ID = NULL_VALUE; target_ID = NULL_VALUE;
		derived_from_parsing = NULL_VALUE;
		single_reference = false;
		composite_alignment = false;
	}
	sequence(sequence& seq) : cost_tree_object(seq)
	{
		ID = sequence_ID_number++;
		sequence_depth = seq.get_sequence_depth();
		leaf_node_ID = seq.get_leaf_node_ID();
		abs_P = seq.get_abs_P(); total_cost = seq.get_total_cost();
		origin = seq.get_origin();
		degree_of_matching = seq.get_degree_of_matching();
		keep = seq.get_keep();
		new_this_cycle = seq.is_new_this_cycle();
		driving_ID = seq.get_driving_ID();
		target_ID = seq.get_target_ID();
		derived_from_parsing = seq.get_derived_from_parsing();
		single_reference = seq.is_single_reference();
		composite_alignment = seq.composite_alignment;
	}
	base_object* clone() {
		sequence* result = new sequence(*this);
		return(result);
	}
	~sequence() { this->finish_deletion(); }

	// Original sequence methods

	bool contains_copy_of_ID_symbol(symbol*);
	void add_unique_id_symbol();
	bool has_unique_id_symbol() {
		symbol* s1;
		list_for(s1, symbol, this)
		{
			if (s1->get_type() == UNIQUE_ID_SYMBOL) return(true);
			if (s1->get_status() == CONTENTS) return(false);
		}
		return(false);
	}
	bool check_for_single_reference();
	bool last_symbol(symbol* s1) {
		symbol* s2 = (symbol*)
			get_last_child(); if (s1 == s2) return(true);
			else return(false);
	}
	void assign_frequencies_and_costs();
	bool is_null() {
		symbol* s1; list_for(s1, symbol, this)
			if (s1->get_status() == CONTENTS) return(false);
		return(true);
	}
	void add_reference(int context_number);
	void set_all_symbols_status(int st) {
		symbol* s1; // Sets all symbols
		if (st != IDENTIFICATION && st != CONTENTS // to the same status.
			&& st != BOUNDARY_MARKER)
			abort_run("Invalid parameter for sequence::set_status().");
		list_for(s1, symbol, this)
			s1->set_status(st);
	}
	int find_number_of_CLASS_ID_symbols() {
		int count = 0;
		symbol* s1; list_for(s1, symbol, this)
		{
			if (s1->get_type() != CONTEXT_SYMBOL) continue;
			if (s1->get_status() != IDENTIFICATION) continue;
			count++;
		} return(count);
	}
	bool is_abstract_pattern() // Contains no DATA_SYMBOLs.
	{
		symbol* s1; list_for(s1, symbol, this)
			if (s1->get_type() == DATA_SYMBOL) return(false);
		return(true);
	}
	bool add_context_symbol(int context_number);
	void add_context_symbol_at_start(symbol* s1)
	{
		symbol* s2 = (symbol*)get_first_child();
		this->follow(s2, s1);
	}
	int count_class_symbols()
	{
		symbol* symbol1; int count = 0;
		list_for(symbol1, symbol, this)
			count++;
		return(count);
	}
	sequence* shallow_copy(); // Makes a copy of a 'this' sequence 
		// without any information about rows (assumes a depth of 1).
	bool is_copy_of(sequence* seq1);
	symbol* get_first_cs()
	{
		symbol* symbol1 = (symbol*)get_first_child();
		if (symbol1->get_type() != LEFT_BRACKET) return(NIL);
		symbol1 = (symbol*)get_next_child();
		if (symbol1->get_type() != CONTEXT_SYMBOL)
			return(NIL);
		else return(symbol1);
	}
	symbol* get_next_cs()
	{
		symbol* symbol1 = (symbol*)get_next_child();
		if (symbol1->get_type() != CONTEXT_SYMBOL)
			return(NIL);
		else return(symbol1);
	}
	bool has_brackets()
	{
		list_element* pos1 = NIL;
		symbol* symbol1 = (symbol*)
			get_next_child_by_el_pos(&pos1);
		if (symbol1 == NIL) return(false);
		else if (symbol1->get_type() == LEFT_BRACKET
			&& symbol1->get_status() == BOUNDARY_MARKER)
			return(true);
		else return(false);
	}
	bool all_new_symbols_matched(sequence* cnp);
	void add_ID_symbols(symbol*, bool);
	sequence* check_patterns(); // Checks 'this' against
		// pre-existing patterns to find one containing all
		// and only the same CONTENTS symbols.
	void print_ID() { fprintf(output_file, "%s%d", "ID", ID); }
	void wto(int print_code);
	bool contents_symbols_match(sequence* pattern1);
	void compute_costs();
	void set_total_cost(double cost) { total_cost = cost; }
	double get_total_cost() { return(total_cost); }
	void set_origin(int org1) {
		if (org1 != ABSTRACT_PATTERN &&
			org1 != BASIC_PATTERN && org1 != CODE_PATTERN
			&& org1 != COMPOSITE_ALIGNMENT && org1 != SUB_ALIGNMENT)
			abort_run("Invalid parameter for sequence::set_origin()");
		origin = org1;
	}
	int get_origin() { return(origin); }
	void write_pattern(bool, bool);
	int get_context_number() {
		char* s_name; symbol* symbol1 =
			(symbol*)get_first_child();
		if (symbol1->get_type() != LEFT_BRACKET) return(NIL);
		symbol1 = (symbol*)get_next_child();
		s_name = symbol1->get_name();
		return(atoi(s_name));
	}
	bool is_single_reference()
	{
		return(single_reference);
	}
	void set_single_reference(bool sr) { single_reference = sr; }

	// Alignment methods

	void check_for_composite_structure();
	bool new_hits_form_coherent_sequence();
	bool is_legal();
	void set_symbol_matches() {
		symbol* col1;
		list_for(col1, symbol, this)
			col1->set_symbol_matches();
	}
	void create_patterns();
	void process_old_pattern(int row);
	int find_degree_of_matching(sequence* cnp);
	bool has_all_data_symbols_matched();
	void correct_column_values();
	void compute_score_with_gaps();
	void write_out_fully(const char*, hit_node*, int, int, bool, sequence*);
	void write_alignment(FILE* of, int wsl, int spp,
		char alignment_format)
	{
		if (alignment_format == 'H')
			write_alignment_horizontal(of, wsl, spp);
		else write_alignment_vertical(of);
	}
	void write_alignment_vertical(FILE*);
	void write_alignment_horizontal(FILE*, int, int);
	void write_with_details(bool);
	sequence* make_code(bool return_code);
	int compute_character_length();
	bool alignment_matches(sequence* al);
	sequence* matches_earlier_alignment();
	void set_sequence_depth(int al_depth)
	{
		sequence_depth = al_depth;
	}
	int get_sequence_depth() { return(sequence_depth); }
	void set_leaf_node_ID(int ID)
	{
		leaf_node_ID = ID;
	}
	int get_leaf_node_ID() { return(leaf_node_ID); }
	void set_abs_P(double a_P) { abs_P = a_P; }
	double get_abs_P() { return(abs_P); }
	void show_al_structure(hit_node* h_node);
	sequence* get_row_pattern(int row)
	{
		if (sequence_depth == 1 ||
			row > sequence_depth || row < 0)
			abort_run("Invalid value for row in get_al_el");
		symbol* al_col1 = (symbol*)this->get_first_child();
		alignment_element* al_el1 = al_col1->get_al_el(row);
		return(al_el1->get_original_pattern());
	}
	void set_degree_of_matching(int dom) {
		if (dom != NULL_VALUE
			&& dom != FULL_A && dom != FULL_B && dom != FULL_C
			&& dom != PARTIAL)
			abort_run("Invalid value for degree of matching in \
				sequence::set_degree_of_matching()");
		degree_of_matching = dom;
	}
	void set_keep(bool k) { keep = k; }
	bool get_keep() { return(keep); }
	int get_degree_of_matching() { return(degree_of_matching); }
	void set_new_this_cycle(bool ntc) { new_this_cycle = ntc; }
	bool is_new_this_cycle() { return(new_this_cycle); }
	void set_driving_ID(int id_value)
	{
		driving_ID = id_value;
	}
	int get_driving_ID() { return(driving_ID); }
	void set_target_ID(int id_value)
	{
		target_ID = id_value;
	}
	int get_target_ID() { return(target_ID); }
	void set_derived_from_parsing(int id_value)
	{
		derived_from_parsing = id_value;
	}
	int get_derived_from_parsing() { return(derived_from_parsing); }
	void set_composite_alignment(bool ca) { composite_alignment = ca; }
	bool is_composite_alignment() { return(composite_alignment); }
protected:
	// Alignment fields

	int sequence_depth; // Records the number of rows in
		// the sequence. An 'alignment' comprising one
		// pattern has a depth of 1.
	int leaf_node_ID; // The ID number of the leaf hit node
		// (if any) which corresponds to this sequence.
		// The ID is stored rather than the hit node itself
		// so that the hit structure can be properly cleared
		// at the end of each cycle and memory is not cluttered
		// with hit nodes from previous cycles.
	double abs_P; // The "absolute probability" of an
		// alignment calculated from its EC (in
		// compute_score_with_gaps).
	double total_cost; // Total of the bit_cost values for all
		// the symbols in the pattern.
	int origin; // Used to mark the origin of a sequence as 'code',
		// sub-alignment, basic pattern etc.
	bool keep; // Records whether an alignment is to be retained
		// on the next cycle.
	int degree_of_matching; // FULL_A, FULL_B, FULL_C or PARTIAL.
	bool new_this_cycle;
	int derived_from_parsing; // If this is a code pattern, this field
		// records the ID of the alignment from which it was derived.
		// Otherwise it has a NULL_VALUE.
	int driving_ID; // Records the ID of the driving pattern for
		// this alignment.
	int target_ID; // Records the ID of the target pattern for
		// this alignment.
	bool single_reference; // Marks alignments that contain a 
		// LEFT_BRACKET, a context symbol and a RIGHT_BRACKET
		// and no other symbols.
	bool composite_alignment;
}; // class sequence

class hit_node : public cost_tree_object	// An element of the hit structure.
{
public:
	hit_node();
	hit_node(hit_node& hn);
	~hit_node() { this->finish_deletion(); }
	base_object* clone() {
		hit_node* result = new hit_node(*this);
		return(result);
	}

	bool all_contents_symbols_are_matched();
	void print_ID() { fprintf(output_file, "%c%d", '#', ID); }
	void set_hn_parent(hit_node* parent) { hn_parent = parent; }
	hit_node* get_hn_parent() { return(hn_parent); }
	void set_driving_pattern(sequence* dr_sequence)
	{
		driving_pattern = dr_sequence;
	}
	sequence* get_driving_pattern() { return(driving_pattern); }
	void set_driving_symbol(symbol* dr_symbol)
	{
		driving_symbol = dr_symbol;
	}
	symbol* get_driving_symbol() { return(driving_symbol); }
	void set_driving_int_pos(int pos)
	{
		driving_int_pos = pos;
	}
	int get_driving_int_pos() { return(driving_int_pos); }
	void set_target_pattern(sequence* al)
	{
		target_pattern = al;
	}
	sequence* get_target_pattern() { return(target_pattern); }
	void set_target_symbol(symbol* symb)
	{
		target_symbol = symb;
	}
	symbol* get_target_symbol() { return(target_symbol); }
	void set_target_int_pos(int int_pos)
	{
		target_int_pos = int_pos;
	}
	int get_target_int_pos() { return(target_int_pos); }
	void set_leaf_entry(hit_node** node) { leaf_entry = node; }
	hit_node** get_leaf_entry() { return(leaf_entry); }
	void wto(int)
	{
		fprintf(output_file, "hit_node ");
		this->print_ID(), fprintf(output_file, "\n\n");
	}
protected:
	hit_node* hn_parent;
	sequence* driving_pattern;
	symbol* driving_symbol;
	int driving_int_pos;
	sequence* target_pattern;
	symbol* target_symbol;
	int target_int_pos;
	hit_node** leaf_entry; // This reverse pointer to an
		// entry in leaf_array[] is needed so that, when
		// a leaf node receives a child (so that it becomes a
		// parent and is no longer a leaf node), the child
		// may use the entry in leaf_array[] that was
		// used by its parent (the old leaf node).
}; // class hit_node

class combination : public base_object
{
public:
	combination(int lfs) : base_object()
	{
		C_ID = combination_ID_number++; len_fit_seq = lfs;
		fitting_sequence = new bool[len_fit_seq];
		int i; for (i = 0; i < len_fit_seq; i++)
			fitting_sequence[i] = false;
		sub_alignment_list = new group;
		combination_score = 0;
	}
	combination(combination& comb1);
	~combination() {
		sub_alignment_list->release_children();
		delete sub_alignment_list;
		delete[] fitting_sequence;
	}

	sequence* make_composite_alignment(sequence*);
	int number_of_alignments()
	{
		return(sub_alignment_list->count_number_of_children());
	}
	bool is_symbol() { return(false); }
	bool is_subset_of(combination* test_comb);
	bool can_accept(int pos_new_first, int pos_new_last);
	void add_sub_alignment(sequence*, int, int, sequence*);
	group* get_sub_alignment_list()
	{
		return(sub_alignment_list);
	}
	double get_combination_score() { return(combination_score); }
	void set_combination_score(double c_score)
	{
		combination_score = c_score;
	}
	void print_combination();
	bool* get_fitting_sequence() { return(fitting_sequence); }
	void set_len_fit_seq(int length) { len_fit_seq = length; }
	int get_len_fit_seq() { return(len_fit_seq); }
	void set_C_ID(int cid) { C_ID = cid; }
	int get_C_ID() { return(C_ID); }
protected:
	bool* fitting_sequence;
	// An array used to mark which symbols 
	// in the current symbol from New
	// are 'covered' by one of the alignments in
	// 'combination' (true) and which are not (false).
	int len_fit_seq;
	group* sub_alignment_list; // A list of basic alignments that
		// fully or partially 'cover' the symbols in New.
	double combination_score;
	int C_ID;
}; // class combination

class grammar : public base_object
{
public:
	grammar() : base_object()
	{
		G = E = score = 0; list_of_patterns = new group;
		ID = grammar_ID_number++; basis_for = NIL;
		derived_from_grammar = NULL_VALUE;
		derived_from_alignment = NIL;
	}
	grammar(grammar& gr);
	~grammar() {
		list_of_patterns->release_children();
		delete list_of_patterns; if (basis_for != NIL)
			delete basis_for;
	}

	void copy_details_and_patterns(grammar*);
	void delete_patterns() { list_of_patterns->delete_children(); }
	bool contains(sequence* p1)
	{
		return(list_of_patterns->contains(p1));
	}
	void renumber_code_symbols(int);
	bool is_ID_symbol(symbol* s1);
	bool is_referenced(symbol* s1);
	void delete_code_patterns() {
		sequence* p1;
		while (p1 = get_first_code_pattern())
			list_of_patterns->delete_child(p1);
	}
	sequence* get_first_pattern() {
		sequence* p1 = (sequence*)
			list_of_patterns->get_first_child(); return(p1);
	}
	sequence* get_next_pattern() {
		sequence* p1 = (sequence*)
			list_of_patterns->get_next_child(); return(p1);
	}
	sequence* get_first_code_pattern() {
		sequence* p1;
		list_for(p1, sequence, list_of_patterns)
			if (p1->get_origin() == CODE_PATTERN) return(p1);
		return(NIL);
	}
	sequence* get_first_non_code_pattern()
	{
		sequence* p1; list_for(p1, sequence, list_of_patterns)
		{
			if (p1->get_origin() != CODE_PATTERN) break;
		}
		return(p1);
	}
	sequence* get_next_non_code_pattern()
	{
		sequence* p1;
		while (p1 = (sequence*)list_of_patterns->get_next_child())
			if (p1->get_origin() != CODE_PATTERN) break;
		return(p1);
	}
	friend sequence* single_reference(symbol* ref1, grammar* gr1);
	void extract_pattern(sequence* p1)
	{
		list_of_patterns->extract_child(p1);
	}
	sequence* extract_first_pattern()
	{
		sequence* p1 = (sequence*)
			list_of_patterns->extract_first_child();
		return(p1);
	}
	void merge_patterns();
	bool matches(grammar* gr1)
	{
		group* patterns1 = gr1->get_list_of_patterns();
		if (patterns1->group_matches(list_of_patterns))
			return(true);
		else return(false);
	}
	void tidy_up_code_symbols(int);
	void clean_up(int);
	bool add_pattern(sequence*);
	void release_patterns() { list_of_patterns->release_children(); }
	void write_grammar(bool, bool);
	void compile_grammar(sequence*);
	void compute_G();
	void set_G(double GG) { G = GG; }
	double get_G() { return(G); }
	void set_E(double EE) { E = EE; }
	double get_E() { return(E); }
	void increment_E(double increment) { E += increment; }
	void increment_G(double increment) { G += increment; }
	void set_score(double score1) { score = score1; }
	void sum_G_and_E() { score = G + E; }
	double get_score() { return(score); }
	int get_ID() { return(ID); }
	group* get_list_of_patterns() { return(list_of_patterns); }
	void initialise() { list_of_patterns->initialise(); }
	void sort_by_ID() { list_of_patterns->sort_by_ID(); }
	void set_derived_from_grammar(int id) { derived_from_grammar = id; }
	int get_derived_from_grammar() { return(derived_from_grammar); }
	void set_derived_from_alignment(sequence* al) { derived_from_alignment = al; }
	sequence* get_derived_from_alignment() { return(derived_from_alignment); }
	void add_derived_grammar(grammar* dg) {
		if (basis_for == NIL)
			basis_for = new group; basis_for->add_child(dg);
	}
	void write_trace(int indendation);

protected:
	double G; // Size of grammar in bits.
	double E; // Size of New after it has been encoded in
		// terms of the grammar.
	double score; // The sum of G and E.
	int ID;
	group* list_of_patterns, // The patterns in the grammar.
		* basis_for; // List of grammars that are derived
			// from the given grammar.
	int derived_from_grammar; // ID of grammar from which the
		// present grammar was derived. 
	sequence* derived_from_alignment; // The most recent
		// alignment from which the present grammar was derived. 
}; // class grammar
