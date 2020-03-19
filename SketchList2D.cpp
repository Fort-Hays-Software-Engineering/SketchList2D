#include "fx.h"
#include "fxkeys.h"
#include <new>
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
#include "SketchList2D.h"
#include "TestWindow.h"

FXDEFMAP(SketchList2D) SketchList2DMap[] = {
	FXMAPFUNC(SEL_SIGNAL, SketchList2D::ID_CLOSEALL, SketchList2D::onCmdCloseAll),
	FXMAPFUNC(SEL_COMMAND, SketchList2D::ID_CLOSEALL, SketchList2D::onCmdCloseAll),
};

FXIMPLEMENT(SketchList2D, FXApp, SketchList2DMap, ARRAYNUMBER(SketchList2DMap))

SketchList2D::SketchList2D(const FXString& name) : FXApp(name, FXString::null) {
#ifndef DEBUG
	// If interrupt happens, quit gracefully; we may want to
	// save edit buffer contents w/o asking if display gets
	// disconnected or if hangup signal is received.
	addSignal(SIGINT, this, ID_CLOSEALL);
#ifndef WIN32
	addSignal(SIGQUIT, this, ID_CLOSEALL);
	addSignal(SIGHUP, this, ID_CLOSEALL);
	addSignal(SIGPIPE, this, ID_CLOSEALL);
#endif
#endif

	// File associations
	associations = new FXFileDict(this);
}

void SketchList2D::init(int& argc, char** argv, bool connect) {

	// After init, the registry has been loaded
	FXApp::init(argc, argv, connect);
}


void SketchList2D::exit(FXint code) {
	FXApp::exit(code);
}

// Close all windows
long SketchList2D::onCmdCloseAll(FXObject*, FXSelector, void*) {
	while (0 < windowlist.no() && windowlist[0]->close(TRUE));
	return 1;
}

SketchList2D::~SketchList2D() {
	FXASSERT(windowlist.no() == 0);
}