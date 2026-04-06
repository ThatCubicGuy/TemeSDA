#include "STS.h"
#include "Defines.h"
#include "String.h"
#include <stdio.h>
#include <string.h>

#define THROW(stream) do { fprintf(stream, "INVALID OPERATION! ERROR 404\n"); return; } while (0)

Incident new(Incident)(int id, string priority, string description)
{
	Incident result = malloc(sizeof(Incident_t));
	*result = (Incident_t) {
		.id = id,
		.description = new(string)(description)
	};
	strcpy(result->priority, priority);
	strcpy(result->status, "queued");
	return result;
}

Intervention new(Intervention)(Incident incident, Unit unit)
{
	Intervention result = malloc(sizeof(Intervention_t));
	*result = (Intervention_t) {
		.incident = incident,
		.unit = unit
	};
	return result;
}

// Implement the constructor for System.
System new(System)(string input_file_name, string output_file_name)
{
	System sys = (System)malloc(sizeof(System_t));
	*sys = (System_t) {
		.fin = fopen(input_file_name,"rt"),
		.fout = fopen(output_file_name,"wt"),
		.units = new(DoublyLinkedList_Unit)(),
		.incidents = new(DoublyLinkedList_Incident)(),
		.interventions = new(DoublyLinkedList_Intervention)(),
		.queue_available_units = new(Queue_Unit)(),
		.queue_low = new(Queue_Incident)(),
		.queue_medium = new(Queue_Incident)(),
		.queue_high = new(Queue_Incident)(),
		.intervention_history = new(Stack_Intervention)()
	};
	// sys->incidents->_start->Value;
	int unit_count;
	fscanf(sys->fin, "%d\n", &unit_count);
	while (unit_count > 0) {
		Unit unit = malloc(sizeof(Unit_t));
		fscanf(sys->fin, "%d %c\n", &unit->id, &unit->type);
		unit->availability = 1;
		DoublyLinkedList_Unit_Add(sys->units, unit);
		Queue_Unit_Enqueue(sys->queue_available_units, unit);
		unit_count -= 1;
	}
	// Since we're reading from a file, we have EOF, which is a better
	// indicator of stream end than keeping track of a "command count"
	fscanf(sys->fin, "%d\n", &unit_count);
	return sys;
}

// Frees all memory occupied by an entire system.
void System_Destroy(System* source)
{
	System sys = *source;
	*source = NULL;
	fclose(sys->fin);
	fclose(sys->fout);
	// Destroy all the auxiliary queues and stacks
	Queue_Unit_Destroy(&sys->queue_available_units);
	Queue_Incident_Destroy(&sys->queue_low);
	Queue_Incident_Destroy(&sys->queue_medium);
	Queue_Incident_Destroy(&sys->queue_high);
	Stack_Intervention_Destroy(&sys->intervention_history);
	// Free every item in each DLL since they always contain everything
	foreach(Unit, unit, sys->units, free(unit));
	foreach(Incident, inc, sys->incidents, {
		free((void*)inc->description);
		free(inc);
	});
	foreach(Intervention, interv, sys->interventions, free(interv));
	// Destroy the DLLs as well
	DoublyLinkedList_Unit_Destroy(&sys->units);
	DoublyLinkedList_Incident_Destroy(&sys->incidents);
	DoublyLinkedList_Intervention_Destroy(&sys->interventions);
	free(sys);
}

CmdArgs parse_cmd(System sys)
{
	char buffer[256] = {0};
	if (!fscanf(sys->fin, "%s", buffer)) return (CmdArgs){.cmd = UNKNOWN};
	if (!strcmp(buffer, "ADD_INCIDENT")) {
		CmdArgs result = (CmdArgs){.cmd = ADD_INCIDENT};
		fscanf(sys->fin, "%d", &result.args.id);
		fscanf(sys->fin, "%s", result.args.priority);
		// Skip a space and a quote, then read everything
		// up until the next quote and skip it as well.
		fscanf(sys->fin, " \"%[^\"]\"", result.args.description);
		return result;
	}
	if (!strcmp(buffer, "CHECK_UNITS_AVAILABILITY")) {
		return (CmdArgs){.cmd = CHECK_UNITS_AVAILABILITY};
	}
	if (!strcmp(buffer, "DISPATCH")) {
		return (CmdArgs){.cmd = DISPATCH};
	}
	if (!strcmp(buffer, "UNDO_LAST_DISPATCH")) {
		return (CmdArgs){.cmd = UNDO_LAST_DISPATCH};
	}
	if (!strcmp(buffer, "SOLVED_INCIDENT")) {
		CmdArgs result = (CmdArgs){.cmd = SOLVED_INCIDENT};
		fscanf(sys->fin, "%d", &result.args.id);
		return result;
	}
	if (!strcmp(buffer, "SHOW_UNIT")) {
		CmdArgs result = (CmdArgs){.cmd = SHOW_UNIT};
		fscanf(sys->fin, "%d", &result.args.id);
		return result;
	}
	if (!strcmp(buffer, "SHOW_INCIDENT")) {
		CmdArgs result = (CmdArgs){.cmd = SHOW_INCIDENT};
		fscanf(sys->fin, "%d", &result.args.id);
		return result;
	}
	if (!strcmp(buffer, "SHOW_INTERVENTIONS")) {
		return (CmdArgs){.cmd = SHOW_INTERVENTIONS};
	}
	return (CmdArgs){.cmd = UNKNOWN};
}

// Add a new incident to be solved
void add_incident(System sys, int id, string priority, string description)
{
	// Create a new incident and add it to the list
	Incident result = new(Incident)(id, priority, description);
	DoublyLinkedList_Incident_Add(sys->incidents, result);
	// Also append it to its corresponding priority queue.
	switch (priority[0]) {
	case 'h':
		Queue_Incident_Enqueue(sys->queue_high, result);
		return;
	case 'm':
		Queue_Incident_Enqueue(sys->queue_medium, result);
		return;
	default:
		fprintf(stderr, "WARNING: Unknown priority! Defaulting to low.\n");
	case 'l':
		Queue_Incident_Enqueue(sys->queue_low, result);
		return;
	}
}

// Check how many units are available
void check_availability(System sys)
{
	fprintf(sys->fout, "Number of available units: %d\n",
			sys->queue_available_units->Count);
}

// Dispatch a unit to the highest priority incident
void dispatch(System sys)
{
	// Find the most important incident
	Queue_Incident queue = NULL;
	if (sys->queue_low->Count > 0) {
		queue = sys->queue_low;
	}
	if (sys->queue_medium->Count > 0) {
		queue = sys->queue_medium;
	}
	if (sys->queue_high->Count > 0) {
		queue = sys->queue_high;
	}
	// ^ else is not necessary, since lower priorities will be overridden
	// If all queues are empty or if no units are available, throw an error
	if (!queue || sys->queue_available_units->Count == 0) THROW(sys->fout);
	Incident result = Queue_Incident_Dequeue(queue);
	Unit dispatched = Queue_Unit_Dequeue(sys->queue_available_units);
	// Log the intervention in history
	Stack_Intervention_Push(sys->intervention_history, new(Intervention)(result, dispatched));
	// Add the intervention to the main interventions list
	DoublyLinkedList_Intervention_Add(sys->interventions, Stack_Intervention_Peek(sys->intervention_history));
	dispatched->availability = 0;
	// Mark the incident as intervened
	strcpy(result->status, "intervened");
}

// Undo the most recent dispatch
void undo_dispatch(System sys)
{
	Intervention item;
	// While there's still items in the history stack...
	while (Stack_Intervention_TryPop(sys->intervention_history, &item)) {
		// If we found an intervention that isn't solved...
		if (strcmp(item->incident->status, "solved")) {
			// Remove it from the interventions list...
			DoublyLinkedList_Intervention_Remove(sys->interventions, item);
			strcpy(item->incident->status, "queued");
			Queue_Incident queue = NULL;
			switch (item->incident->priority[0]) {
			case 'l':
				queue = sys->queue_low;
				break;
			case 'm':
				queue = sys->queue_medium;
				break;
			case 'h':
				queue = sys->queue_high;
				break;
			}
			// Add it back to its priority queue...
			Queue_Incident_EnqueueFirst(queue, item->incident);
			// Make the corresponding unit available...
			item->unit->availability = 1;
			Queue_Unit_Enqueue(sys->queue_available_units, item->unit);
			// ...and free the intervention pointer.
			free(item);
			return;
		}
	}
	// If we ran out of items in the stack, throw an error.
	THROW(sys->fout);
}

// Mark an incident as solved and free the unit
void finish_dispatch(System sys, int id)
{
	// For each item in the list of interventions...
	foreach (Intervention, item, sys->interventions, {
		// If we find an incident with the given ID...
		if (item->incident->id == id) {
			// So long as its status is "intervened"...
			if (strcmp(item->incident->status, "intervened")) break;
			// Mark the unit as available...
			item->unit->availability = 1;
			Queue_Unit_Enqueue(sys->queue_available_units, item->unit);
			// ...and mark the incident as solved.
			strcpy(item->incident->status, "solved");
			foreach_return();
		}
	});
	// If the incident was not found or was not intervened, throw an error.
	THROW(sys->fout);
}

// Format: Unit <unitID> is type <[ABC]> and is <(un)available>
void unit_details(System sys, int id)
{
	// Find the unit with the matching ID and output its information.
	foreach (Unit, unit, sys->units, {
		if (unit->id == id) {
			fprintf(sys->fout, "Unit %d is type %c and is %savailable\n",
					id, unit->type, unit->availability ? string_Empty : "un");
			foreach_return();
		}
	});
	// Throw an error if not found.
	THROW(sys->fout);
}

// Format: Incident <incidentID> has <low|medium|high> priority,
// the following description: "<description>" and is <queued|intervened|solved>
void incident_details(System sys, int id)
{
	// Find the incident with the matching ID and output its information.
	foreach (Incident, inc, sys->incidents, {
		if (inc->id == id) {
			fprintf(sys->fout, "Incident %d has %s priority, "
					"the following description: \"%s\" and is %s\n", id,
					inc->priority, inc->description, inc->status);
			foreach_return();
		}
	});
	// Throw an error if not found.
	THROW(sys->fout);
}

// Format: Incident <incidentID> was assigned to unit <unitID>,
// and has the following status: <queued|intervened|solved>
void intervention_details(System sys)
{
	if (sys->interventions->Count == 0) {
		fprintf(sys->fout, "No intervention has been initiated\n");
		return;
	}
	foreach (Intervention, interv, sys->interventions, {
		fprintf(sys->fout, "Incident %d was assigned to unit %d, "
				"and has the following status: \"%s\"\n", interv->incident->id,
				interv->unit->id, interv->incident->status);
	});
}
