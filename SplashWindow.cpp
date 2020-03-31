/********************************************************************************
*                                                                               *
*                         SplashList 2D GUI Code                                *
*                                                                               *
********************************************************************************/
#include "SplashWindow.h"
#include "ProjectWindow.h"
#include "fx.h"

// Message Map for the SplashList Window class
FXDEFMAP(SplashWindow) SplashWindowMap[] = {

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_COMMAND,           SplashWindow::ID_NEWPROJECT,    SplashWindow::onCmdNewProject),
};



// Macro for the SplashListApp class hierarchy implementation
FXIMPLEMENT(SplashWindow, FXMainWindow, SplashWindowMap, ARRAYNUMBER(SplashWindowMap))


// Construct a SplashWindow
SplashWindow::SplashWindow(FXApp *a) :FXMainWindow(a, "SketchList 2D Room Designer", NULL, NULL, DECOR_ALL, 0, 0, 800, 600) {
	// Add to list of windows
	//getApp()->windowlist.append(this);

	topdock = new FXDockSite(this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X);
	// Make menu bar
	menubar = new FXMenuBar(topdock, LAYOUT_DOCK_NEXT | LAYOUT_SIDE_TOP | LAYOUT_FILL_X | FRAME_RAISED);

	contents = new FXHorizontalFrame(this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y, 90, 90, 0, 90, 90, 90, 90, 90);
	middleSplashFrame = new FXHorizontalFrame(contents, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 90, 0, 0, 0, 0, 0);

	// Lets Get Started Create New Project Label and Button
	createNewProjectFrame = new FXVerticalFrame(middleSplashFrame, FRAME_SUNKEN | LAYOUT_FILL_Y | LAYOUT_TOP | LAYOUT_LEFT, 0, 0, 0, 0, 10, 10, 10, 10);
	new FXLabel(createNewProjectFrame, "Let's Get Started", NULL, JUSTIFY_CENTER_X | LAYOUT_FILL_X);
	new FXButton(createNewProjectFrame, "&Create New Project", NULL, this, ID_NEWPROJECT, FRAME_THICK | FRAME_RAISED | LAYOUT_FILL_X | LAYOUT_TOP | LAYOUT_LEFT, 0, 0, 0, 0, 10, 10, 5, 5);

	//Pick Up Where you Left Off, load project frame
	loadProjectFrame = new FXVerticalFrame(middleSplashFrame, FRAME_SUNKEN | LAYOUT_FILL_Y | LAYOUT_TOP | LAYOUT_RIGHT, 0, 0, 0, 0, 10, 10, 10, 10);
	new FXLabel(loadProjectFrame, "Pick Up Where You Left Off", NULL, JUSTIFY_CENTER_X | LAYOUT_FILL_X);
	existingProjectList = new FXHorizontalFrame(loadProjectFrame, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 90, 0, 0, 0, 0, 0);
	new FXButton(loadProjectFrame, "&Load", NULL, this, ID_CLEAR, FRAME_THICK | FRAME_RAISED | LAYOUT_FILL_X | LAYOUT_TOP | LAYOUT_LEFT, 0, 0, 0, 0, 10, 10, 5, 5);



	// Status bar
	statusbar = new FXStatusBar(this, LAYOUT_SIDE_BOTTOM | LAYOUT_FILL_X | STATUSBAR_WITH_DRAGCORNER | FRAME_RAISED);

	// File menu
	filemenu = new FXMenuPane(this);
	new FXMenuTitle(menubar, tr("&File"), NULL, filemenu);

	// File Menu entries
	new FXMenuCommand(filemenu, tr("&New...\tCtl-N\tCreate new document."), NULL, this, ID_NEW);
	new FXMenuCommand(filemenu, tr("&Open...\tCtl-O\tOpen document file."), NULL, this, ID_OPEN);

}


SplashWindow::~SplashWindow() {
}


// Create and initialize
void SplashWindow::create() {

	// Create the windows
	FXMainWindow::create();
	filemenu->create();
	// Make the main window appear
	show(PLACEMENT_SCREEN);
}


long SplashWindow::onCmdNewProject(FXObject*, FXSelector, void*) {
	//redraw screen with isSplashScreen = 0
	ProjectWindow *window = new ProjectWindow(getApp());
	window->create();
	window->raise();
	window->setFocus();


	return 1;
}


FXbool SplashWindow::close(FXbool notify) {
	// Perform normal close stuff
	return FXMainWindow::close(notify);
}

// Detach window
void SplashWindow::detach() {
	FXMainWindow::detach();
	dragshell1->detach();
	urilistType = 0;
}
