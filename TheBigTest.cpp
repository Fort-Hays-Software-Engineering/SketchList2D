#include "fx.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <signal.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include <ctype.h>
#include "FXRex.h"
#include "FXArray.h"
#include "TestWindow.h"
#include "SketchList2D.h"


int main(int argc, char *argv[]) {
	TestWindow *window = NULL;
	FXbool edit = TRUE;
	FXString file;
	FXint arg;
	FXint x, y, w, h, g;

	//make application
	SketchList2D application("SketchList2D");

	//open display
	application.init(argc, argv);

	//make tool tip
	new FXToolTip(&application, 0);
	application.create();

	// Loop over args
	/*for (arg = 1; arg < argc; ++arg) {

		// Parse a few options
		if (compare(argv[arg], "-v") == 0 || compare(argv[arg], "--view") == 0) {
			edit = FALSE;
		}
		else if (compare(argv[arg], "-e") == 0 || compare(argv[arg], "--edit") == 0) {
			edit = TRUE;
		}
		else if (compare(argv[arg], "-h") == 0 || compare(argv[arg], "--help") == 0) {
			printusage();
			exit(0);
		}
		else if (compare(argv[arg], "-l") == 0 || compare(argv[arg], "--line") == 0) {
			if (++arg >= argc) { fprintf(stderr, "Adie: missing line number argument.\n"); exit(1); }
			sscanf(argv[arg], "%d", &line);
		}
		else if (compare(argv[arg], "-t") == 0 || compare(argv[arg], "--tags") == 0) {
			if (++arg >= argc) { fprintf(stderr, "Adie: missing tags file argument.\n"); exit(1); }
			tags = argv[arg];
		}
		else if (compare(argv[arg], "-m") == 0 || compare(argv[arg], "--mode") == 0) {
			if (++arg >= argc) { fprintf(stderr, "Adie: missing language mode argument.\n"); exit(1); }
			language = argv[arg];
		}
		else if (compare(argv[arg], "-g") == 0 || compare(argv[arg], "--geometry") == 0) {
			if (++arg >= argc) { fprintf(stderr, "Adie: missing geometry argument.\n"); exit(1); }
			g = fxparsegeometry(argv[arg], x, y, w, h);
		}
		else if (compare(argv[arg], "-V") == 0 || compare(argv[arg], "--version") == 0) {
			fprintf(stdout, "Adie - ADvanced Interactive Editor %d.%d.%d.\nCopyright (C) 1998,2003 Jeroen van der Zijp.\n\n", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
			exit(0);
		}

		// Load the file
		else {
			file = FXPath::absolute(argv[arg]);
			window = new TestWindow(&application, "untitled");
			window->create();
		}
	}*/

	// Make window, if none opened yet
	if (!window) {
		window = new TestWindow(&application, "untitled");
		window->create();
	}

	// Run
	return application.run();
}