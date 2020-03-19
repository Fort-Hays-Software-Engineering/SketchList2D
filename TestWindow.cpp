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
#include "TestWindow.h"
#include "SketchList2D.h"

FXDEFMAP(TestWindow) TestWindowMap[] = {
  FXMAPFUNC(SEL_UPDATE,            0,                              TestWindow::onUpdate),
  FXMAPFUNC(SEL_FOCUSIN,           0,                              TestWindow::onFocusIn),
  FXMAPFUNC(SEL_COMMAND,           TestWindow::ID_NEW,             TestWindow::onCmdNew),
  FXMAPFUNC(SEL_COMMAND,           TestWindow::ID_OPEN,            TestWindow::onCmdOpen),
};

FXIMPLEMENT(TestWindow, FXMainWindow, TestWindowMap, ARRAYNUMBER(TestWindowMap))

TestWindow::TestWindow(SketchList2D* a, const FXString* file) {}: FXMainWindow(a, "SketchList2D", NULL, NULL, DECOR_ALL, 0, 0, 800, 600, 0, 0), mrufiles(a) {
	getApp()->windowlist.append(this);
	
	filemenu = new FXMenuPane(this);
	new FXMenuTitle(menubar, ("&File"), NULL, filemenu);

	new FXMenuCommand(filemenu, ("&New...\tCtl-N\tCreate new document."), NULL, this, ID_NEW);
	new FXMenuCommand(filemenu, ("&Open...\tCtl-O\tOpen document file."), NULL, this, ID_OPEN);

}

// Create and show window
void TestWindow::create() {
	FXMainWindow::create();
	if (!urilistType) { urilistType = getApp()->registerDragType(urilistTypeName); }
	show(PLACEMENT_DEFAULT);
	editor->setFocus();

	// Jump to current directory
	dirlist->setCurrentFile(FXSystem::getCurrentDirectory());
}


// Detach window
void TestWindow::detach() {
	FXMainWindow::detach();
	urilistType = 0;
}

FXString TestWindow::unique() const {
	FXString name = "untitled";
	for (FXint i = 1; i < 2147483647; i++) {
		if (!findWindow(name)) break;
		name.format("untitled%d", i);
	}
	return name;
}

// Find window, if any, currently editing the given file
TestWindow *TestWindow::findWindow(const FXString& file) const {
	for (FXint w = 0; w < getApp()->windowlist.no(); w++) {
		if (getApp()->windowlist[w]->getFilename() == file) return getApp()->windowlist[w];
	}
	return NULL;
}

long TestWindow::onCmdNew(FXObject*, FXSelector, void*) {
	TestWindow *window = new TestWindow(getApp(), unique());
	window->create();
	window->raise();
	window->setFocus();
	return 1;
}

// Open
long TestWindow::onCmdOpen(FXObject*, FXSelector, void*) {
	FXFileDialog opendialog(this, tr("Open Document"));
	if (opendialog.execute()) {
		FXString file = opendialog.getFilename();
		TestWindow *window = findWindow(file);
		if (!window) {
			window = findUnused();
			if (!window) {
				window = new TestWindow(getApp(), unique());
				window->create();
			}
		}
		window->raise();
		window->setFocus();
	}
	return 1;
}


FXbool TestWindow::close(FXbool notify) {
	return FXMainWindow::close(notify);
}

TestWindow::~TestWindow() {
	getApp()->windowlist.remove(this);
	delete filemenu;
}