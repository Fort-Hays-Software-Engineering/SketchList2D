#include "ProjectWindow.h"
#include <FXComboBox.h>
#include <FXText.h>
#include <FXGLCanvas.h>
#include <FXGLVisual.h>
#include <FXRadioButton.h>
#include <FXDCWindow.h>


#pragma comment (lib, "ws2_32")

FXDEFMAP(ProjectWindow) ProjectWindowMap[] = {

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_PAINT,             ProjectWindow::ID_CANVAS,			ProjectWindow::onPaint),
	FXMAPFUNC(SEL_LEFTBUTTONPRESS,   ProjectWindow::ID_CANVAS,			ProjectWindow::onMouseDown),
	FXMAPFUNC(SEL_LEFTBUTTONRELEASE, ProjectWindow::ID_CANVAS,			ProjectWindow::onMouseUp),
	FXMAPFUNC(SEL_MOTION,            ProjectWindow::ID_CANVAS,			ProjectWindow::onMouseMove),
	FXMAPFUNC(SEL_COMMAND,           ProjectWindow::ID_CLEAR,			ProjectWindow::onCmdClear),
	FXMAPFUNC(SEL_UPDATE,            ProjectWindow::ID_CLEAR,			ProjectWindow::onUpdClear),
	FXMAPFUNC(SEL_COMMAND,           ProjectWindow::ID_OPEN,			ProjectWindow::onCmdOpen),
	FXMAPFUNC(SEL_COMMAND,           ProjectWindow::ID_OPEN_RECENT,		ProjectWindow::onCmdOpenRecent),
	FXMAPFUNC(SEL_COMMAND,           ProjectWindow::ID_SAVE,			ProjectWindow::onCmdSave),
	FXMAPFUNC(SEL_COMMAND,           ProjectWindow::ID_SAVEAS,			ProjectWindow::onCmdSaveAs),
	FXMAPFUNC(SEL_COMMAND,           ProjectWindow::ID_NEWPROJECT,		ProjectWindow::onCmdNewProject),
	FXMAPFUNC(SEL_COMMAND,           ProjectWindow::ID_NEWPLACEABLE,	ProjectWindow::onCmdNewPlacable),
	FXMAPFUNC(SEL_COMMAND,			 ProjectWindow::ID_GRIDSIZE,		ProjectWindow::onCmdGridSize),
	FXMAPFUNC(SEL_COMMAND,			 ProjectWindow::ID_UPDATESPECS,		ProjectWindow::onCmdUpdateSpecs),
    FXMAPFUNC(SEL_COMMAND,           ProjectWindow::ID_PRINT,			ProjectWindow::onCmdPrint),

};



// Macro for the ProjectListApp class hierarchy implementation
FXIMPLEMENT(ProjectWindow, FXMainWindow, ProjectWindowMap, ARRAYNUMBER(ProjectWindowMap))


// Construct a ProjectWindow
ProjectWindow::ProjectWindow(FXApp *a) :FXMainWindow(a, "SketchList 2D Room Designer", NULL, NULL, DECOR_ALL, 0, 0, 800, 600), mrufiles(a) {
	

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

	new FXButton(LeftPanel, "&New Item", NULL, this, ID_NEWPLACEABLE, FRAME_THICK | FRAME_RAISED | LAYOUT_FILL_X | LAYOUT_TOP | LAYOUT_LEFT, 0, 0, 0, 0, 10, 10, 5, 5);

	//Frame where data about the currently selected placable type is displayed
	placeableDataPanel = new FXVerticalFrame(LeftPanel, FRAME_SUNKEN | LAYOUT_FILL_Y | LAYOUT_TOP | LAYOUT_LEFT, 0, 0, 0, 0, 10, 10, 10, 10);
	new FXLabel(placeableDataPanel, "Specifications", NULL, JUSTIFY_CENTER_X | LAYOUT_FILL_X);

	widthFrame = new FXHorizontalFrame(placeableDataPanel, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
	new FXLabel(widthFrame, "Width", NULL, JUSTIFY_CENTER_X | LAYOUT_FILL_X);
	widthText = new FXTextField(widthFrame, 5, NULL, 0, TEXTFIELD_INTEGER | FRAME_LINE);

	heightFrame = new FXHorizontalFrame(placeableDataPanel, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
	new FXLabel(heightFrame, "Height", NULL, JUSTIFY_CENTER_X | LAYOUT_FILL_X);
	heightText = new FXTextField(heightFrame, 5, NULL, 0, TEXTFIELD_INTEGER | FRAME_LINE);
	new FXButton(LeftPanel, "&Update", NULL, this, ID_UPDATESPECS, FRAME_THICK | FRAME_RAISED | LAYOUT_FILL_X | LAYOUT_TOP | LAYOUT_LEFT, 0, 0, 0, 0, 10, 10, 5, 5);
	unitsFrame = new FXHorizontalFrame(placeableDataPanel, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
	feetRadio = new FXRadioButton(unitsFrame, "Feet");
	inchRadio = new FXRadioButton(unitsFrame, "inches");
	//set up so these radio buttons are connected to the same data selector


	canvas = new FXCanvas(contents, this, ID_CANVAS, FRAME_SUNKEN | FRAME_THICK | LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_FILL_ROW | LAYOUT_FILL_COLUMN);


	cabinetFrame = new FXHorizontalFrame(placeableDataPanel, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
	new FXLabel(cabinetFrame, "Cabinet", NULL, JUSTIFY_CENTER_X, LAYOUT_FILL_X);
	cabinet = new FXText(cabinetFrame);
	
	gridSizeDisplay = new FXTextField(LeftPanel, 5, NULL, 0, TEXTFIELD_INTEGER | FRAME_LINE);
	new FXLabel(LeftPanel, "Grid Size", NULL, JUSTIFY_CENTER_X, LAYOUT_FILL_X);
	gridSizeSlider = new FXSlider(LeftPanel, this, ID_GRIDSIZE, LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0);
	gridSizeSlider->setRange(1, 6);



	new FXLabel(LeftPanel, "Grid Size", NULL, JUSTIFY_CENTER_X, LAYOUT_FILL_X);
	gridSizeSlider = new FXSlider(LeftPanel, this, ID_GRIDSIZE, LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0);
	gridSizeSlider->setRange(10, 100);
	gridSizeSlider->setIncrement(10);


	// Status bar
	statusbar = new FXStatusBar(this, LAYOUT_SIDE_BOTTOM | LAYOUT_FILL_X | STATUSBAR_WITH_DRAGCORNER | FRAME_RAISED);

	// File menu
	filemenu = new FXMenuPane(this);
	new FXMenuTitle(menubar, tr("&File"), NULL, filemenu);

	// File Menu entries
	new FXMenuCommand(filemenu, tr("&New Project...\tCtl-N\tCreate New Project."), NULL, this, ID_NEWPROJECT);
	new FXMenuCommand(filemenu, tr("&Open Project...\tCtl-O\tOpen Project File."), NULL, this, ID_OPEN);
	new FXMenuCommand(filemenu, tr("&Save Project...\tCtl-S\tSave Project."), NULL, this, ID_SAVE);
	new FXMenuCommand(filemenu, tr("&Save Project As...\tCtl-D\tSave Project As."), NULL, this, ID_SAVEAS);
	new FXMenuCommand(filemenu, tr("&Print...\tCtl-P\tPrint document."), NULL, this, ID_PRINT);


	// Recent file menu; this automatically hides if there are no files
	FXMenuSeparator* sep1 = new FXMenuSeparator(filemenu);
	sep1->setTarget(&mrufiles);
	sep1->setSelector(FXRecentFiles::ID_ANYFILES);
	new FXMenuCommand(filemenu, FXString::null, NULL, &mrufiles, FXRecentFiles::ID_FILE_1);
	new FXMenuCommand(filemenu, FXString::null, NULL, &mrufiles, FXRecentFiles::ID_FILE_2);
	new FXMenuCommand(filemenu, FXString::null, NULL, &mrufiles, FXRecentFiles::ID_FILE_3);
	new FXMenuCommand(filemenu, FXString::null, NULL, &mrufiles, FXRecentFiles::ID_FILE_4);
	new FXMenuCommand(filemenu, FXString::null, NULL, &mrufiles, FXRecentFiles::ID_FILE_5);
	new FXMenuCommand(filemenu, FXString::null, NULL, &mrufiles, FXRecentFiles::ID_FILE_6);
	new FXMenuCommand(filemenu, FXString::null, NULL, &mrufiles, FXRecentFiles::ID_FILE_7);
	new FXMenuCommand(filemenu, FXString::null, NULL, &mrufiles, FXRecentFiles::ID_FILE_8);
	new FXMenuCommand(filemenu, FXString::null, NULL, &mrufiles, FXRecentFiles::ID_FILE_9);
	new FXMenuCommand(filemenu, FXString::null, NULL, &mrufiles, FXRecentFiles::ID_FILE_10);
	new FXMenuCommand(filemenu, tr("&Clear Recent Files"), NULL, &mrufiles, FXRecentFiles::ID_CLEAR);
	FXMenuSeparator* sep2 = new FXMenuSeparator(filemenu);
	sep2->setTarget(&mrufiles);
	sep2->setSelector(FXRecentFiles::ID_ANYFILES);


	new FXMenuCommand(filemenu, tr("&View Materials and Pricing\tCtl-B."), NULL, this, ID_VIEWBOM);
	new FXMenuSeparator(filemenu);
	new FXMenuCommand(filemenu, tr("&Exit\tAlt-F4\tExit Program."), NULL, this, FXApp::ID_QUIT);

	// Initialize private variables
	drawColor = FXRGB(207, 207, 207);
	mdflag = 0;
	dirty = 0;
	itemClicked = 0;

	// Recent files
	mrufiles.setTarget(this);
	mrufiles.setSelector(ID_OPEN_RECENT);
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

long ProjectWindow::onCmdNewPlacable(FXObject*, FXSelector, void*) {
	FXDCWindow dc(canvas); //get the canvas
	switch (placeableTypeComboBox->getCurrentItem()) {
	case 0:
		project->addPlaceable(project->get_gridSize(), project->get_gridSize(), 500, 5);
		break;
	case 1:
		project->addPlaceable(project->get_gridSize(), project->get_gridSize(), 5, 500);
		break;
	case 2:
		project->addPlaceable(project->get_gridSize(), project->get_gridSize(), 50, 50);
		break;
	default:
		break;

	}
	drawScreen(); //redraw the screen
	return 1;
}

void ProjectWindow::drawScreen()
{
	FXDCWindow dc(canvas);
	dc.setForeground(canvas->getBackColor());
	

	//draw grid
	int canvasWidth = canvas->getWidth();
	int canvasHeight = canvas->getHeight();
	dc.fillRectangle(0, 0, canvasWidth, canvasHeight);
	dc.setForeground(drawColor);

	for (int x = 0; x < canvasWidth; x = x + 16) {
		dc.drawLine(x, 0, x, canvasHeight);
	}
	for (int y = 0; y < canvasHeight; y = y + 16) {
		dc.drawLine(0, y, canvasWidth, y);
	}

	dc.setForeground(FXRGB(0, 0, 0));

	//draw placeables
	for (int i = 0; i < project->get_placeableCount(); i++) {
		FXRectangle *rect = project->placeables[i]->get_rectangle();
		dc.drawRectangles(rect, 1);
	}
	if(currentSelection != NULL)
		drawControlHandles();
}

// Mouse button was pressed somewhere
long ProjectWindow::onMouseDown(FXObject*, FXSelector, void* ptr) {
	canvas->grab();
	FXEvent *ev = (FXEvent*)ptr;
	int clickX, clickY;
	//get mouse position
	clickX = ev->click_x;
	clickY = ev->click_y;

	int xStart, xEnd, yStart, yEnd;

	FXDCWindow dc(canvas);
	if (mdflag == 0) {


		//iterate through placeables seeing if this position is inside one of the rectangles
		for (int i = 0; i < project->get_placeableCount(); i++) {
			FXRectangle *rect = project->placeables[i]->get_rectangle();


			if (rect->contains(clickX, clickY)) {
				currentSelection = project->placeables[i];
				currentIndex = i;
				drawScreen();
				itemClicked = 1;
				mdflag = 1;
				widthText->setText(FXStringVal(project->placeables[i]->get_width()));
				heightText->setText(FXStringVal(project->placeables[i]->get_height()));
				return 1;

			}
			else { // No item clicked, Deselect
				currentSelection = NULL;
				drawScreen();
				widthText->setText("");
				heightText->setText("");
			}

		}
	}
	//use data targets to put this placeable's info in onscreen controls


	itemClicked = 0;
	mdflag = 1;

	return 1;
}



// The mouse has moved, and a placeable is selected, move it
long ProjectWindow::onMouseMove(FXObject*, FXSelector, void* ptr) {
	FXEvent *ev = (FXEvent*)ptr;
	/* Experiment in snapping to grid
	int grid = project->get_gridSize();
	if (itemClicked == 1 && mdflag == 1 && currentSelection != NULL) {

		if (ev->win_x - ev->last_x > 0){
			project->placeables[currentIndex]->set_xPos(project->placeables[currentIndex]->get_xPos() + grid);
		}
		else if (ev->win_x - ev->last_x < 0) {
			project->placeables[currentIndex]->set_xPos(project->placeables[currentIndex]->get_xPos() - grid);
		}
		if (ev->win_y - ev->last_y > 0) {
			project->placeables[currentIndex]->set_yPos(project->placeables[currentIndex]->get_yPos() + grid);
		}
		else if (ev->win_y - ev->last_y < 0) {
			project->placeables[currentIndex]->set_yPos(project->placeables[currentIndex]->get_yPos() - grid);
		}
		drawScreen();
	}
	*/
	
	if (currentSelection != NULL) {
		if (itemClicked == 1 && mdflag == 1) {
			project->placeables[currentIndex]->set_xPos(project->placeables[currentIndex]->get_xPos() + ev->win_x - ev->last_x);
			project->placeables[currentIndex]->set_yPos(project->placeables[currentIndex]->get_yPos() + ev->win_y - ev->last_y);
			drawScreen();
		}
	}
	
	return 1;
}



// The mouse button was released again
long ProjectWindow::onMouseUp(FXObject*, FXSelector, void* ptr) {
	FXEvent *ev = (FXEvent*)ptr;
	canvas->ungrab();
	if (mdflag) {

		// Mouse no longer down
		mdflag = 0;
		itemClicked = 1;
	}
	return 1;
}


void ProjectWindow::drawControlHandles()
{
	FXDCWindow dc(canvas);

	dc.drawFocusRectangle(project->placeables[currentIndex]->get_xPos()-5,
						  project->placeables[currentIndex]->get_yPos()-5,
						  project->placeables[currentIndex]->get_width()+10,
						  project->placeables[currentIndex]->get_height()+10);
	
	

}

// Paint the canvas
long ProjectWindow::onPaint(FXObject*, FXSelector, void* ptr) {
	//Draw Grid and placeables
	drawScreen();

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

//Update Cabinet Specifications
// Change Grid Size With Slider
long ProjectWindow::onCmdUpdateSpecs(FXObject*, FXSelector, void*) {
	
	currentSelection->set_height(FXIntVal(heightText->getText()));
	currentSelection->set_width(FXIntVal(widthText->getText()));

	drawScreen();

	return 1;
}
// Change Grid Size With Slider
long ProjectWindow::onCmdGridSize(FXObject*, FXSelector, void*) {
	FXint grid, scale = gridSizeSlider->getValue();
	FXString displayText;

	switch (scale) {
	case 1:
		displayText = "1/16\"";
		grid = 16;
		break;
	case 2:
		displayText = "1/8\"";
		grid = 32;
		break;
	case 3:
		displayText = "1/4\"";
		grid = 64;
		break;
	case 4:
		displayText = "1/2\"";
		grid = 128;
		break;
	case 5:
		displayText = "1\"";
		grid = 256;
		break;
	case 6:
		displayText = "2\"";
		grid = 512;
		break;
	default:
		displayText = "";
	}
	gridSizeDisplay->setText(displayText);

	project->set_gridSize(grid);

	drawScreen();

	return 1;

}

long ProjectWindow::onCmdPrint(FXObject*, FXSelector, void*) {
	FXPrintDialog dlg(this, tr("Print File"));
	FXPrinter printer;
	if (dlg.execute()) {
		dlg.getPrinter(printer);
		FXTRACE((100, "Printer = %s\n", printer.name.text()));
	}
	return 1;
}




// Save
long ProjectWindow::onCmdSave(FXObject* sender, FXSelector sel, void* ptr) {
	if (!filenameset) return onCmdSaveAs(sender, sel, ptr);

	saveFile(filename);
	return 1;
}

// Save As
long ProjectWindow::onCmdSaveAs(FXObject*, FXSelector, void*) {
	FXFileDialog savedialog(getApp(), tr("Save Project"));
	FXString file = filename;
	savedialog.setSelectMode(SELECTFILE_ANY);
	savedialog.setPatternList("Project Files(*.pjt)\nAll Files (*)");
	savedialog.setCurrentPattern(0);
	savedialog.setFilename(file);
	if (savedialog.execute()) {
		file = savedialog.getFilename();
		if (FXStat::exists(file)) {
			if (MBOX_CLICKED_NO == FXMessageBox::question(this, MBOX_YES_NO, tr("Overwrite Document"), tr("Overwrite existing document: %s?"), file.text())) return 1;
		}
		saveFile(file);
	}
	return 1;
}

// Save file
FXbool ProjectWindow::saveFile(const FXString& file) {
	FXFileStream  stream;
	
	// Save stuff to a FILE stream


	FXTRACE((100, "saveFile(%s)\n", file.text()));

	// Opened file?
	if (!stream.open(file, FXStreamSave)) {
		FXMessageBox::error(this, MBOX_OK, tr("Error Saving Project File"), tr("Unable to open file: %s"), file.text());
		return FALSE;
	}

	getApp()->beginWaitCursor();
	// Save project data to the file
	project->save(stream);
	stream.close();
	// Kill wait cursor
	getApp()->endWaitCursor();

	// Set stuff
	mrufiles.appendFile(file);
	filenameset = TRUE;
	filename = file;
	setTitle("SketchList 2D Room Designer - " + file);
	getApp()->reg().write();
	return TRUE;
}

// Open project file
long ProjectWindow::onCmdOpen(FXObject*, FXSelector, void*) {
	FXFileDialog opendialog(this, tr("Open Document"));
	opendialog.setSelectMode(SELECTFILE_EXISTING);
	opendialog.setPatternList("Project Files(*.pjt)\nAll Files (*)");
	opendialog.setCurrentPattern(0);
	opendialog.setDirectory(FXPath::directory(filename));
	if (opendialog.execute()) {
		ProjectWindow *window = new ProjectWindow(getApp());
		FXString file = opendialog.getFilename();
		window->create();
		window->loadFile(file);
		window->raise();
		window->setFocus();
		
	}
	return 1;
}

// Load project file
FXbool ProjectWindow::loadFile(const FXString& file) {
	FXFileStream  stream;
	FXTRACE((100, "loadFile(%s)\n", file.text()));
	// Opened file?
	if (!stream.open(file, FXStreamLoad)) {
		FXMessageBox::error(this, MBOX_OK, tr("Error Saving Project File"), tr("Unable to open file: %s"), file.text());
		return FALSE;
	}
	// Set wait cursor
	getApp()->beginWaitCursor();
	//project->load(stream);
	// Save project data to the file
	project->load(stream);
	drawScreen();
	// Kill wait cursor
	getApp()->endWaitCursor();
	// Set stuff
	mrufiles.appendFile(file);
	filetime = FXStat::modified(file);
	filenameset = TRUE;
	filename = file;
	setTitle("SketchList 2D Room Designer - " + file);
	getApp()->reg().write();
	return TRUE;
}

// Open recent file
long ProjectWindow::onCmdOpenRecent(FXObject*, FXSelector, void* ptr) {
	FXString file = (const char*)ptr;
	ProjectWindow *window = new ProjectWindow(getApp());
	window->create();
	window->loadFile(file);
	window->raise();
	window->setFocus();
	return 1;
}


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