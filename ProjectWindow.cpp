#include "ProjectWindow.h"
#include <FXComboBox.h>
#include <FXText.h>
#include <FXRadioButton.h>

FXDEFMAP(ProjectWindow) ProjectWindowMap[] = {

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_PAINT,             ProjectWindow::ID_CANVAS, ProjectWindow::onPaint),
	FXMAPFUNC(SEL_LEFTBUTTONPRESS,   ProjectWindow::ID_CANVAS, ProjectWindow::onMouseDown),
	FXMAPFUNC(SEL_LEFTBUTTONRELEASE, ProjectWindow::ID_CANVAS, ProjectWindow::onMouseUp),
	FXMAPFUNC(SEL_MOTION,            ProjectWindow::ID_CANVAS, ProjectWindow::onMouseMove),
	FXMAPFUNC(SEL_COMMAND,           ProjectWindow::ID_CLEAR,  ProjectWindow::onCmdClear),
	FXMAPFUNC(SEL_UPDATE,            ProjectWindow::ID_CLEAR,  ProjectWindow::onUpdClear),
	//FXMAPFUNC(SEL_COMMAND,           ProjectWindow::ID_NEW,    ProjectWindow::onCmdNew),
	//FXMAPFUNC(SEL_COMMAND,           ProjectWindow::ID_OPEN,   ProjectWindow::onCmdOpen),
	FXMAPFUNC(SEL_COMMAND,           ProjectWindow::ID_NEWPROJECT,    ProjectWindow::onCmdNewProject),
};



// Macro for the ProjectListApp class hierarchy implementation
FXIMPLEMENT(ProjectWindow, FXMainWindow, ProjectWindowMap, ARRAYNUMBER(ProjectWindowMap))


// Construct a ProjectWindow
ProjectWindow::ProjectWindow(FXApp *a) :FXMainWindow(a, "SketchList 2D Romm Designer", NULL, NULL, DECOR_ALL, 0, 0, 800, 600) {
	// Add to list of windows
	//getApp()->windowlist.append(this);
	project = new Project();

	topdock = new FXDockSite(this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X);
	// Make menu bar
	menubar = new FXMenuBar(topdock, LAYOUT_DOCK_NEXT | LAYOUT_SIDE_TOP | LAYOUT_FILL_X | FRAME_RAISED);

	contents = new FXHorizontalFrame(this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
	LeftPanel = new FXVerticalFrame(contents, FRAME_SUNKEN | LAYOUT_FILL_Y | LAYOUT_TOP | LAYOUT_LEFT, 0, 0, 0, 0, 10, 10, 10, 10);

	//Create combo box that lists the different placable objects to select from
	placeableTypeComboBox = new FXComboBox(LeftPanel, 1, NULL, 0, COMBOBOX_STATIC, 90, 90, 90, 90, 2, 2, 2, 2);
	configurePlaceableComboBox(placeableTypeComboBox);

	//Frame where data about the currently selected placable type is displayed
	placeableDataPanel = new FXVerticalFrame(LeftPanel, FRAME_SUNKEN | LAYOUT_FILL_Y | LAYOUT_TOP | LAYOUT_LEFT, 0, 0, 0, 0, 10, 10, 10, 10);
	new FXLabel(placeableDataPanel, "Specifications", NULL, JUSTIFY_CENTER_X | LAYOUT_FILL_X);

	widthFrame = new FXHorizontalFrame(placeableDataPanel, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
	new FXLabel(widthFrame, "Width", NULL, JUSTIFY_CENTER_X | LAYOUT_FILL_X);
	widthText = new FXText(widthFrame);

	heightFrame = new FXHorizontalFrame(placeableDataPanel, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
	new FXLabel(heightFrame, "Height", NULL, JUSTIFY_CENTER_X | LAYOUT_FILL_X);
	heightText = new FXText(heightFrame);

	unitsFrame = new FXHorizontalFrame(placeableDataPanel, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
	feetRadio = new FXRadioButton(unitsFrame, "Feet");
	inchRadio = new FXRadioButton(unitsFrame, "inches");
	//set up so these radio buttons are connected to the same data selector


	canvas = new FXCanvas(contents, this, ID_CANVAS, FRAME_SUNKEN | FRAME_THICK | LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_FILL_ROW | LAYOUT_FILL_COLUMN);
	//middleSplashFrame = new FXHorizontalFrame(contents, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 90, 0, 0, 0, 0, 0);

		// LEFT pane for the buttons
		//buttonFrame = new FXVerticalFrame(middleSplashFrame, FRAME_SUNKEN | LAYOUT_FILL_Y | LAYOUT_TOP | LAYOUT_LEFT, 0, 0, 0, 0, 10, 10, 10, 10);

		// Lets Get Started Create New Project Label and Button
		//new FXLabel(buttonFrame, "Let's Get Started", NULL, JUSTIFY_CENTER_X | LAYOUT_FILL_X);
		//new FXButton(buttonFrame, "&Create New Project", NULL, this, ID_CLEAR, FRAME_THICK | FRAME_RAISED | LAYOUT_FILL_X | LAYOUT_TOP | LAYOUT_LEFT, 0, 0, 0, 0, 10, 10, 5, 5);

		// Horizontal divider line
		//new FXHorizontalSeparator(buttonFrame, SEPARATOR_RIDGE | LAYOUT_FILL_X);

		// Button to clear
		//new FXButton(buttonFrame, "&Clear", NULL, this, ID_CLEAR, FRAME_THICK | FRAME_RAISED | LAYOUT_FILL_X | LAYOUT_TOP | LAYOUT_LEFT, 0, 0, 0, 0, 10, 10, 5, 5);

		//new FXButton(buttonFrame, "&New", NULL, this, ID_NEW, FRAME_THICK | FRAME_RAISED | LAYOUT_FILL_X | LAYOUT_TOP | LAYOUT_LEFT, 0, 0, 0, 0, 10, 10, 5, 5);

		// Exit button
		//new FXButton(buttonFrame, "&Exit", NULL, this, FXApp::ID_QUIT, FRAME_THICK | FRAME_RAISED | LAYOUT_FILL_X | LAYOUT_TOP | LAYOUT_LEFT, 0, 0, 0, 0, 10, 10, 5, 5);

		// RIGHT pane to contain the canvas
		

		// Label above the canvas
		//new FXLabel(canvasFrame, "Room Grid", NULL, JUSTIFY_CENTER_X | LAYOUT_FILL_X);

		// Horizontal divider line
		//new FXHorizontalSeparator(canvasFrame, SEPARATOR_GROOVE | LAYOUT_FILL_X);


		// Drawing canvas
		


	// Status bar
	statusbar = new FXStatusBar(this, LAYOUT_SIDE_BOTTOM | LAYOUT_FILL_X | STATUSBAR_WITH_DRAGCORNER | FRAME_RAISED);

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

void configurePlaceableComboBox(FXComboBox *comboBox) {
	comboBox->appendItem("Interior Wall");
	comboBox->appendItem("Exterior Wall");
	comboBox->appendItem("Cabinet");
	comboBox->setNumVisible(3);
}

ProjectWindow::~ProjectWindow() {
}


// Create and initialize
void ProjectWindow::create() {

	// Create the windows
	FXMainWindow::create();
	filemenu->create();
	// Make the main window appear
	show(PLACEMENT_SCREEN);
}



// Mouse button was pressed somewhere
long ProjectWindow::onMouseDown(FXObject*, FXSelector, void*) {
	canvas->grab();

	// While the mouse is down, we'll draw lines
	mdflag = 1;

	return 1;
}



// The mouse has moved, draw a line
long ProjectWindow::onMouseMove(FXObject*, FXSelector, void* ptr) {
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
long ProjectWindow::onMouseUp(FXObject*, FXSelector, void* ptr) {
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
long ProjectWindow::onPaint(FXObject*, FXSelector, void* ptr) {
	FXEvent *ev = (FXEvent*)ptr;
	FXDCWindow dc(canvas, ev);
	dc.setForeground(canvas->getBackColor());
	dc.fillRectangle(ev->rect.x, ev->rect.y, ev->rect.w, ev->rect.h);
	return 1;
}


// Handle the clear message
long ProjectWindow::onCmdClear(FXObject*, FXSelector, void*) {
	FXDCWindow dc(canvas);
	dc.setForeground(canvas->getBackColor());
	dc.fillRectangle(0, 0, canvas->getWidth(), canvas->getHeight());
	dirty = 0;
	return 1;
}

// New
long ProjectWindow::onCmdNew(FXObject*, FXSelector, void*) {
	ProjectWindow *window = new ProjectWindow(getApp());
	window->create();
	window->raise();
	window->setFocus();
	return 1;
}

long ProjectWindow::onCmdNewProject(FXObject*, FXSelector, void*) {
	//redraw screen with isSplashScreen = 0
	isSplashScreen = 0;
	ProjectWindow *window = new ProjectWindow(getApp());
	window->isSplashScreen = 0;
	window->create();
	window->raise();
	window->setFocus();


	return 1;
}

// ------------------- New File Code --------------------- // 
//    Taken From Adie, still missing getapp->windowlist    //

/*// Generate unique name for a new window
FXString ProjectWindow::unique() const {
	FXString name = "untitled";
	for (FXint i = 1; i < 2147483647; i++) {
		if (!findWindow(name)) break;
		name.format("untitled%d", i);
	}
	return name;
}

// Find window, if any, currently editing the given file
ProjectWindow *ProjectWindow::findWindow(const FXString& file) const {
	for (FXint w = 0; w < getApp()->windowlist.no(); w++) {
		if (getApp()->windowlist[w]->getFilename() == file) return getApp()->windowlist[w];
	}
	return NULL;
}

*/


// Open file code from adie ---- not yet adapted

/*long ProjectWindow::onCmdOpen(FXObject*, FXSelector, void*) {
	FXFileDialog opendialog(this, tr("Open Document"));
	opendialog.setSelectMode(SELECTFILE_EXISTING);
	opendialog.setPatternList(getPatterns());
	opendialog.setCurrentPattern(getCurrentPattern());
	opendialog.setDirectory(FXPath::directory(filename));
	if (opendialog.execute()) {
		setCurrentPattern(opendialog.getCurrentPattern());
		FXString file = opendialog.getFilename();
		ProjectWindow *window = findWindow(file);
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
long ProjectWindow::onUpdClear(FXObject* sender, FXSelector, void*) {

	if (dirty)
		sender->handle(this, FXSEL(SEL_COMMAND, FXWindow::ID_ENABLE), NULL);
	else
		sender->handle(this, FXSEL(SEL_COMMAND, FXWindow::ID_DISABLE), NULL);

	return 1;
}

FXbool ProjectWindow::close(FXbool notify) {
	// Perform normal close stuff
	return FXMainWindow::close(notify);
}

// Detach window
void ProjectWindow::detach() {
	FXMainWindow::detach();
	dragshell1->detach();
	urilistType = 0;
}