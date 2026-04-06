#ifndef UNIT_DEFINED
#define UNIT_DEFINED
#include "Defines.h"
#include "EnumerableT.h"
#include "DoublyLinkedListT.h"
#include "QueueT.h"
#include "StackT.h"

typedef struct unit_s {
	int id;
	int availability;
	char type;
} Unit_t, *Unit;

typedef struct incident_s {
	int id;
	char priority[7];
	char status[11];
	string description;
} Incident_t, *Incident;

typedef struct intervention_s {
	Incident incident;
	Unit unit;
} Intervention_t, *Intervention;

ENUMERABLE_DEFINE(Unit)
ENUMERABLE_DEFINE(Incident)
ENUMERABLE_DEFINE(Intervention)

DOUBLY_LINKED_LIST_DEFINE(Unit)
DOUBLY_LINKED_LIST_DEFINE(Incident)
DOUBLY_LINKED_LIST_DEFINE(Intervention)

QUEUE_DEFINE(Unit)
QUEUE_DEFINE(Incident)
STACK_DEFINE(Intervention)

typedef struct system_s {
	DoublyLinkedList_Unit units;
	DoublyLinkedList_Incident incidents;
	DoublyLinkedList_Intervention interventions;
	Queue_Unit queue_available_units;
	Queue_Incident queue_low;
	Queue_Incident queue_medium;
	Queue_Incident queue_high;
	Stack_Intervention intervention_history;
	FILE* fin;
	FILE* fout;
} System_t, *System;

// Creates a new system with the given input and output files.
System new(System)(string input_file_name, string output_file_name);

// Frees all memory managed by a System.
void System_Destroy(System* source);

enum cmd_type {
	UNKNOWN,
	ADD_INCIDENT,
	CHECK_UNITS_AVAILABILITY,
	DISPATCH,
	UNDO_LAST_DISPATCH,
	SOLVED_INCIDENT,
	SHOW_UNIT,
	SHOW_INCIDENT,
	SHOW_INTERVENTIONS
};

typedef struct CmdArgs_s {
	enum cmd_type cmd;
	struct args_s {
		int id;
		char priority[7];
		char description[256];
	} args;
} CmdArgs;

// Parse a command for the given system.
CmdArgs parse_cmd(System sys);

// Add a new incident to be solved
void add_incident(System sys, int id, string priority, string description);

// Check availability of units
void check_availability(System sys);

// Dispatch a unit to the highest priority incident
void dispatch(System sys);

// Undo the most recent dispatch
void undo_dispatch(System sys);

// Mark an incident as solved and free the unit
void finish_dispatch(System sys, int id);

// Format: Unit <unitID> is type <[ABC]> and is <(un)available>
void unit_details(System sys, int id);

// Format: Incident <incidentID> has <low|medium|high> priority,
// the following description: "<description>" and is <queued|intervened|solved>
void incident_details(System sys, int id);

// Format: Incident <incidentID> was assigned to unit <unitID>,
// and has the following status: <queued|intervened|solved>
void intervention_details(System sys);

#endif
