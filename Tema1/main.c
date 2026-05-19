#include "STS.h"

int main(int argc, char** argv)
{
	byte debug = argc > 0 && ValueEquator(2, argv[0], "-d");
	System sys = new(System)("tema1.in", "tema1.out");
	for (CmdArgs last_cmd = parse_cmd(sys); last_cmd.cmd != UNKNOWN; last_cmd = parse_cmd(sys)) {
		switch (last_cmd.cmd) {
		case UNKNOWN:
			fprintf(stderr, "This wasn't in the prophecy...\n");
			exit(1);
			break;
		case ADD_INCIDENT:
			if (debug) printf("Adding incident with id %d...\n", last_cmd.args.id);
			add_incident(sys, last_cmd.args.id, last_cmd.args.priority, last_cmd.args.description);
			break;
		case CHECK_UNITS_AVAILABILITY:
			if (debug) printf("Checking availability...\n");
			check_availability(sys);
			break;
		case DISPATCH:
			if (debug) printf("Dispatching...\n");
			dispatch(sys);
			break;
		case UNDO_LAST_DISPATCH:
			if (debug) printf("Undoing our mistakes...\n");
			undo_dispatch(sys);
			break;
		case SOLVED_INCIDENT:
			if (debug) printf("Solving incident...\n");
			finish_dispatch(sys, last_cmd.args.id);
			break;
		case SHOW_UNIT:
			if (debug) printf("Displaying unit details...\n");
			unit_details(sys, last_cmd.args.id);
			break;
		case SHOW_INCIDENT:
			if (debug) printf("Displaying incident details...\n");
			incident_details(sys, last_cmd.args.id);
			break;
		case SHOW_INTERVENTIONS:
			if (debug) printf("Displaying every detail...\n");
			intervention_details(sys);
			break;
		}
    }
	System_Destroy(&sys);
	return 0;
}
