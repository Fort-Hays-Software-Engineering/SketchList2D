/********************************************************************************
*                                                                               *
*                         SketchList 2D GUI Code                                *
*                                                                               *
********************************************************************************/
#include "SketchList.H"
#include "fx.h"


// Message Map for the SketchList Window class
FXDEFMAP(SketchWindow) SketchWindowMap[] = {

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_PAINT,             SketchWindow::ID_CANVAS, SketchWindow::onPaint),
	FXMAPFUNC(SEL_LEFTBUTTONPRESS,   SketchWindow::ID_CANVAS, SketchWindow::onMouseDown),
	FXMAPFUNC(SEL_LEFTBUTTONRELEASE, SketchWindow::ID_CANVAS, SketchWindow::onMouseUp),
	FXMAPFUNC(SEL_MOTION,            SketchWindow::ID_CANVAS, SketchWindow::onMouseMove),
	FXMAPFUNC(SEL_COMMAND,           SketchWindow::ID_CLEAR,  SketchWindow::onCmdClear),
	FXMAPFUNC(SEL_UPDATE,            SketchWindow::ID_CLEAR,  SketchWindow::onUpdClear),
	//FXMAPFUNC(SEL_COMMAND,           SketchWindow::ID_NEW,    SketchWindow::onCmdNew),
    //FXMAPFUNC(SEL_COMMAND,           SketchWindow::ID_OPEN,   SketchWindow::onCmdOpen),
};



// Macro for the SketchListApp class hierarchy implementation
FXIMPLEMENT(SketchWindow, FXMainWindow, SketchWindowMap, ARRAYNUMBER(SketchWindowMap))



// Construct a SketchWindow
SketchWindow::SketchWindow(FXApp *a) :FXMainWindow(a, "SketchList 2D", NULL, NULL, DECOR_ALL, 0, 0, 800, 600) {
	// Add to list of windows
	//getApp()->windowlist.append(this);

	topdock = new FXDockSite(this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X);
	// Make menu bar
	menubar = new FXMenuBar(topdock, LAYOUT_DOCK_NEXT | LAYOUT_SIDE_TOP | LAYOUT_FILL_X | FRAME_RAISED);

	contents = new FXHorizontalFrame(this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
	
	// Status bar
	statusbar = new FXStatusBar(this, LAYOUT_SIDE_BOTTOM | LAYOUT_FILL_X | STATUSBAR_WITH_DRAGCORNER | FRAME_RAISED);

	// LEFT pane for the buttons
	buttonFrame = new FXVerticalFrame(contents, FRAME_SUNKEN | LAYOUT_FILL_Y | LAYOUT_TOP | LAYOUT_LEFT, 0, 0, 0, 0, 10, 10, 10, 10);

	// Label above the buttons
	new FXLabel(buttonFrame, "Button Frame", NULL, JUSTIFY_CENTER_X | LAYOUT_FILL_X);

	// Horizontal divider line
	new FXHorizontalSeparator(buttonFrame, SEPARATOR_RIDGE | LAYOUT_FILL_X);

	// Button to clear
	new FXButton(buttonFrame, "&Clear", NULL, this, ID_CLEAR, FRAME_THICK | FRAME_RAISED | LAYOUT_FILL_X | LAYOUT_TOP | LAYOUT_LEFT, 0, 0, 0, 0, 10, 10, 5, 5);

	// Exit button
	new FXButton(buttonFrame, "&Exit", NULL, getApp(), FXApp::ID_QUIT, FRAME_THICK | FRAME_RAISED | LAYOUT_FILL_X | LAYOUT_TOP | LAYOUT_LEFT, 0, 0, 0, 0, 10, 10, 5, 5);

	// RIGHT pane to contain the canvas
	canvasFrame = new FXVerticalFrame(contents, FRAME_SUNKEN | LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_TOP | LAYOUT_LEFT, 0, 0, 0, 0, 10, 10, 10, 10);

	// Label above the canvas
	new FXLabel(canvasFrame, "Room Grid", NULL, JUSTIFY_CENTER_X | LAYOUT_FILL_X);

	// Horizontal divider line
	new FXHorizontalSeparator(canvasFrame, SEPARATOR_GROOVE | LAYOUT_FILL_X);


	// Drawing canvas
	canvas = new FXCanvas(canvasFrame, this, ID_CANVAS, FRAME_SUNKEN | FRAME_THICK | LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_FILL_ROW | LAYOUT_FILL_COLUMN);

	// File menu
	filemenu = new FXMenuPane(this);
	new FXMenuTitle(menubar, tr("&File"), NULL, filemenu);

	// File Menu entries
	new FXMenuCommand(filemenu, tr("&New...\tCtl-N\tCreate new document."), NULL, this, ID_NEW);
	new FXMenuCommand(filemenu, tr("&Open...\tCtl-O\tOpen document file."), NULL, this, ID_OPEN);


	// Initialize private variables
	drawColor = FXRGB(255, 0, 0);
	mdflag = 0;
	dirty = 0;
}


SketchWindow::~SketchWindow() {
}


// Create and initialize
void SketchWindow::create() {

	// Create the windows
	FXMainWindow::create();
	filemenu->create();
	// Make the main window appear
	show(PLACEMENT_SCREEN);
}



// Mouse button was pressed somewhere
long SketchWindow::onMouseDown(FXObject*, FXSelector, void*) {
	canvas->grab();

	// While the mouse is down, we'll draw lines
	mdflag = 1;

	return 1;
}



// The mouse has moved, draw a line
long SketchWindow::onMouseMove(FXObject*, FXSelector, void* ptr) {
	FXEvent *ev = (FXEvent*)ptr;

	// Draw
	if (mdflag) {

		// Get DC for the canvas
		FXDCWindow dc(canvas);

		// Set foreground color
		dc.setForeground(drawColor);

		// Draw line
		dc.drawLine(ev->last_x, ev->last_y, ev->win_x, ev->win_y);

		// We have drawn something, so now the canvas is dirty
		dirty = 1;
	}
	return 1;
}



// The mouse button was released again
long SketchWindow::onMouseUp(FXObject*, FXSelector, void* ptr) {
	FXEvent *ev = (FXEvent*)ptr;
	canvas->ungrab();
	if (mdflag) {
		FXDCWindow dc(canvas);

		dc.setForeground(drawColor);
		dc.drawLine(ev->last_x, ev->last_y, ev->win_x, ev->win_y);

		// We have drawn something, so now the canvas is dirty
		dirty = 1;

		// Mouse no longer down
		mdflag = 0;
	}
	return 1;
}


// Paint the canvas
long SketchWindow::onPaint(FXObject*, FXSelector, void* ptr) {
	FXEvent *ev = (FXEvent*)ptr;
	FXDCWindow dc(canvas, ev);
	dc.setForeground(canvas->getBackColor());
	dc.fillRectangle(ev->rect.x, ev->rect.y, ev->rect.w, ev->rect.h);
	return 1;
}


// Handle the clear message
long SketchWindow::onCmdClear(FXObject*, FXSelector, void*) {
	FXDCWindow dc(canvas);
	dc.setForeground(canvas->getBackColor());
	dc.fillRectangle(0, 0, canvas->getWidth(), canvas->getHeight());
	dirty = 0;
	return 1;
}

// ------------------- New File Code --------------------- // 
//    Taken From Adie, still missing getapp->windowlist    //

/*// Generate unique name for a new window
FXString SketchWindow::unique() const {
	FXString name = "untitled";
	for (FXint i = 1; i < 2147483647; i++) {
		if (!findWindow(name)) break;
		name.format("untitled%d", i);
	}
	return name;
}

// Find window, if any, currently editing the given file
SketchWindow *SketchWindow::findWindow(const FXString& file) const {
	for (FXint w = 0; w < getApp()->windowlist.no(); w++) {
		if (getApp()->windowlist[w]->getFilename() == file) return getApp()->windowlist[w];
	}
	return NULL;
}

// New
long SketchWindow::onCmdNew(FXObject*, FXSelector, void*) {
	SketchWindow *window = new SketchWindow(getApp(), unique());
	window->create();
	window->raise();
	window->setFocus();
	return 1;
}*/


// Open file code from adie ---- not yet adapted

/*long SketchWindow::onCmdOpen(FXObject*, FXSelector, void*) {
	FXFileDialog opendialog(this, tr("Open Document"));
	opendialog.setSelectMode(SELECTFILE_EXISTING);
	opendialog.setPatternList(getPatterns());
	opendialog.setCurrentPattern(getCurrentPattern());
	opendialog.setDirectory(FXPath::directory(filename));
	if (opendialog.execute()) {
		setCurrentPattern(opendialog.getCurrentPattern());
		FXString file = opendialog.getFilename();
		SketchWindow *window = findWindow(file);
		if (!window) {
			window = findUnused();
			if (!window) {
				window = new TextWindow(getApp(), unique());
				window->create();
			}
			window->loadFile(file);
			window->readBookmarks(file);
			window->readView(file);
		}
		window->raise();
		window->setFocus();
	}
	return 1;
}*/



// Update the clear button:- each gui element (widget) in FOX
// receives a message during idle processing asking it to be updated.
// For example, buttons can be sensitized or desensitized when the
// state of the application changes.
// In this case, we desensitize the sender (the clear button) when
// the canvas has already been cleared, and sensitize it when it has
// been painted (as indicated by the dirty flag).
long SketchWindow::onUpdClear(FXObject* sender, FXSelector, void*) {

	if (dirty)
		sender->handle(this, FXSEL(SEL_COMMAND, FXWindow::ID_ENABLE), NULL);
	else
		sender->handle(this, FXSEL(SEL_COMMAND, FXWindow::ID_DISABLE), NULL);

	return 1;
}


// Here we begin
int main(int argc, char *argv[]) {

	// Make application
	FXApp application("SketchList", "FoxTest");

	// Start app
	application.init(argc, argv);

	// SketchList window
	new SketchWindow(&application);

	// Create the application's windows
	application.create();

	// Run the application
	return application.run();
}




