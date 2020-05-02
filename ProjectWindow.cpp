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
	FXMAPFUNC(SEL_COMMAND,           ProjectWindow::ID_VIEWBOM,			ProjectWindow::onCmdViewBom),

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
	widthFraction = new FXComboBox(widthFrame, 1, NULL, 0, COMBOBOX_STATIC, 90, 90, 90, 90, 2, 2, 2, 2);
	configureFractionComboBox(widthFraction);

	heightFrame = new FXHorizontalFrame(placeableDataPanel, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
	new FXLabel(heightFrame, "Height", NULL, JUSTIFY_CENTER_X | LAYOUT_FILL_X);
	heightText = new FXTextField(heightFrame, 5, NULL, 0, TEXTFIELD_INTEGER | FRAME_LINE);
	heightFraction = new FXComboBox(heightFrame, 1, NULL, 0, COMBOBOX_STATIC, 90, 90, 90, 90, 2, 2, 2, 2);
	configureFractionComboBox(heightFraction);

	angleFrame = new FXHorizontalFrame(placeableDataPanel, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
	new FXLabel(angleFrame, "Angle", NULL, JUSTIFY_CENTER_X | LAYOUT_FILL_X);
	angleText = new FXTextField(angleFrame, 5, NULL, 0, TEXTFIELD_INTEGER | FRAME_LINE);
	

	
	unitsFrame = new FXHorizontalFrame(placeableDataPanel, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);

	

	
	//set up so these radio buttons are connected to the same data selector
	//feetRadio = new FXRadioButton(unitsFrame, "Feet");
	//inchRadio = new FXRadioButton(unitsFrame, "inches");

	canvas = new FXCanvas(contents, this, ID_CANVAS, FRAME_SUNKEN | FRAME_THICK | LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_FILL_ROW | LAYOUT_FILL_COLUMN);


	cabinetFrame = new FXVerticalFrame(placeableDataPanel, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
	new FXLabel(cabinetFrame, "Cabinet Name", NULL, JUSTIFY_CENTER_X, LAYOUT_FILL_X);
	cabinet = new FXTextField(cabinetFrame, 20, NULL, 0, TEXTFIELD_NORMAL | FRAME_LINE);
	new FXButton(cabinetFrame, "&Update", NULL, this, ID_UPDATESPECS, FRAME_THICK | FRAME_RAISED | LAYOUT_FILL_X | LAYOUT_TOP | LAYOUT_LEFT, 0, 0, 0, 0, 10, 10, 5, 5);

	

	gridSizeDisplay = new FXTextField(LeftPanel, 5, NULL, 0, TEXTFIELD_READONLY | FRAME_LINE);
	new FXLabel(LeftPanel, "Grid Size", NULL, JUSTIFY_CENTER_X, LAYOUT_FILL_X);
	gridSizeSlider = new FXSlider(LeftPanel, this, ID_GRIDSIZE, LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0);
	gridSizeSlider->setRange(1, 6);
	switch (project->get_gridSize()) {
	case 1:
		gridSizeDisplay->setText("1/16\"");
		gridSizeSlider->setValue(1);
		break;
	case 2:
		gridSizeDisplay->setText("1/8\"");
		gridSizeSlider->setValue(2);
		break;
	case 4:
		gridSizeDisplay->setText("1/4\"");
		gridSizeSlider->setValue(3);
		break;
	case 8:
		gridSizeDisplay->setText("1/2\"");
		gridSizeSlider->setValue(4);
		break;
	case 16:
		gridSizeDisplay->setText("1\"");
		gridSizeSlider->setValue(5);
		break;
	case 32:
		gridSizeDisplay->setText("2\"");
		gridSizeSlider->setValue(6);
		break;
	default:
		gridSizeDisplay->setText("N/A");
	}


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
	resizeable = 0;
	xDrag = 0;
	yDrag = 0;

	// Recent files
	mrufiles.setTarget(this);
	mrufiles.setSelector(ID_OPEN_RECENT);
}

void configureFractionComboBox(FXComboBox *box) {
	box->appendItem("in");        // 0
	box->appendItem("1/16\"");    // 1
	box->appendItem("1/8\"");     // 2
	box->appendItem("3/16\"");    // 3
	box->appendItem("1/4\"");     // 4
	box->appendItem("5/16\"");    // 5
	box->appendItem("3/8\"");     // 6
	box->appendItem("7/16\"");    // 7
	box->appendItem("1/2\"");     // 8 
	box->appendItem("9/16\"");    // 9
	box->appendItem("5/8\"");     // 10
	box->appendItem("11/16\"");   // 11
	box->appendItem("3/4\"");     // 12
	box->appendItem("13/16\"");   // 13
	box->appendItem("7/8\"");     // 14
	box->appendItem("15/16\"");   // 15
	box->setNumVisible(8);
}

void configurePlaceableComboBox(FXComboBox *comboBox) {
	comboBox->appendItem("Interior Wall");
	comboBox->appendItem("Exterior Wall");
	comboBox->appendItem("Cabinet");
	comboBox->appendItem("Prefab: Door - Left");
	comboBox->appendItem("Prefab: Door - Right");
	comboBox->appendItem("Prefab: Door - Sink");
	comboBox->appendItem("Prefab: Door - Stove");
	comboBox->setNumVisible(7);
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
		project->addPlaceable(project->get_gridSize(), project->get_gridSize(), 1920, 64); // 4" x 10' wall
		break;
	case 1:
		project->addPlaceable(project->get_gridSize(), project->get_gridSize(), 64, 1920); // 4" x 10' wall
		break;
	case 2:
		project->addPlaceable(project->get_gridSize(), project->get_gridSize(), 384, 480); // standard 24" by 30" cabinet
		break;
	case 3:
		project->addPlaceable(true, 0, project->get_gridSize(), project->get_gridSize()); // Left-Swing Door
		break;
	case 4:
		project->addPlaceable(true, 1, project->get_gridSize(), project->get_gridSize()); // Right-Swing Door
		break;
	case 5:
		project->addPlaceable(true, 2, project->get_gridSize(), project->get_gridSize()); // Sink Cabinet
		break;
	case 6:
		project->addPlaceable(true, 3, project->get_gridSize(), project->get_gridSize()); // Stove
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

	for (int x = 0; x < canvasWidth; x = x + 10) {
		dc.drawLine(x, 0, x, canvasHeight);
	}
	for (int y = 0; y < canvasHeight; y = y + 10) {
		dc.drawLine(0, y, canvasWidth, y);
	}

	dc.setForeground(FXRGB(0, 0, 0));

	//draw placeables
	for (int i = 0; i < project->get_placeableCount(); i++) {
		if (project->placeables[i]->get_prefab() == false) {
			project->placeables[i]->draw(&dc, project->get_gridSize());
		}
		else {
			project->placeables[i]->drawPrefab(&dc, project->get_gridSize());
		}
		

	}
	if(currentSelection != NULL)
		drawControlHandles();


}

bool ProjectWindow::checkResizeArea(int x, int y) {
	if (currentSelection == NULL)
		return false;
	FXRectangle* currentRect = currentSelection->get_rectangle();
	FXRectangle* focusRect = new FXRectangle(currentRect->x - 5,
		currentRect->y - 5,
		currentRect->w + 10,
		currentRect->h + 10

	);
	if (!currentSelection->get_rectangle()->contains(x, y) && focusRect->contains(x, y)) {
		return true;
	}
	return false;
}

// Mouse button was pressed somewhere
long ProjectWindow::onMouseDown(FXObject*, FXSelector, void* ptr) {
	canvas->grab();
	FXEvent *ev = (FXEvent*)ptr;
	int clickX, clickY;
	//get mouse position
	clickX = ev->click_x;
	clickY = ev->click_y;

	FXPoint rotatedPoint = FXPoint(clickX, clickY);
	if (currentSelection != NULL)
	{
		FXRectangle *r = currentSelection->get_rectangle();
		FXPoint center = FXPoint(r->x + r->w * .5,
			r->y + r->h * .5);
		rotatedPoint = rotateClick(rotatedPoint, center, -currentSelection->get_angle());
	}
	FXDCWindow dc(canvas);
	if (mdflag == 0) {

		if (checkResizeArea(rotatedPoint.x, rotatedPoint.y)) {
			resizeable = 1;
		}
		else {
			resizeable = 0;
			//iterate through placeables seeing if this position is inside one of the rectangles
			for (int i = 0; i < project->get_placeableCount(); i++) {

				if (project->placeables[i]->isClicked(clickX, clickY, &dc)) {
					currentSelection = project->placeables[i];
					currentIndex = i;
					drawScreen();
					project->placeables[i]->isClicked(clickX, clickY, &dc); //DEBUG TO DRAW THE CLICK
					itemClicked = 1;
					mdflag = 1;
					displayUnits();
					return 1;

				}
				else { // No item clicked, Deselect
					deselect();
					project->placeables[i]->isClicked(clickX, clickY, &dc); //DEBUG TO DRAW THE CLICK
				}

			}
		}
	}

	itemClicked = 0;
	mdflag = 1;

	return 1;
}



// The mouse has moved, and a placeable is selected, move it
long ProjectWindow::onMouseMove(FXObject*, FXSelector, void* ptr) {
	FXEvent *ev = (FXEvent*)ptr;
	FXDCWindow dc(canvas);

	//set the default cursor state is a normal arrow cursor
	canvas->setDefaultCursor(getApp()->getDefaultCursor(DEF_ARROW_CURSOR));
	canvas->setDragCursor(getApp()->getDefaultCursor(DEF_ARROW_CURSOR));

	if (currentSelection != NULL) { //if there is a selected placeable
		//rotate cursor against current selection

		FXRectangle *r = currentSelection->get_rectangle();
		FXPoint center = FXPoint(r->x + r->w * .5,
			                     r->y + r->h * .5);
		FXPoint rotatedPoint = rotateClick(FXPoint(ev->win_x, ev->win_y), center, -currentSelection->get_angle());
		
		
		if (checkResizeArea(rotatedPoint.x, rotatedPoint.y)) { //if the cursor is in the area to click and drag resize
			updateCursor(rotatedPoint.x, rotatedPoint.y); //update the cursor to drag arrows
		}
		if (itemClicked == 1 && mdflag == 1) {
			xDrag += ev->win_x - ev->last_x;
			yDrag += ev->win_y - ev->last_y;
			if (xDrag > 10) {
				project->placeables[currentIndex]->set_xPos(project->placeables[currentIndex]->get_xPos() + project->get_gridSize());
				xDrag = 0;
			}
			else if (xDrag < -10) {
				project->placeables[currentIndex]->set_xPos(project->placeables[currentIndex]->get_xPos() - project->get_gridSize());
				xDrag = 0;
			}
			if (yDrag > 10) {
				project->placeables[currentIndex]->set_yPos(project->placeables[currentIndex]->get_yPos() + project->get_gridSize());
				yDrag = 0;
			}
			else if (yDrag < -10) {
				project->placeables[currentIndex]->set_yPos(project->placeables[currentIndex]->get_yPos() - project->get_gridSize());
				yDrag = 0;
			}
			
			
			drawScreen();
		}
		if (resizeable && mdflag) { //if the cursor is in the area to resize, and the mouse is down

			FXPoint rotatedLast = rotateClick(FXPoint(ev->last_x, ev->last_y), center, -currentSelection->get_angle());
			resize(rotatedLast.x, rotatedPoint.x, rotatedLast.y, rotatedPoint.y); //resize the current selection
		}
	}
	
	return 1;
}




// The mouse button was released again
long ProjectWindow::deselect() {
	currentSelection = NULL;
	drawScreen();
	widthText->setText("");
	heightText->setText("");
	angleText->setText("");
	widthFraction->setCurrentItem(0);
	heightFraction->setCurrentItem(0);
	cabinet->setText("");
	return 1;
}

Project* ProjectWindow::getProject()
{
	return project;
}


// The mouse button was released again
long ProjectWindow::onMouseUp(FXObject*, FXSelector, void* ptr) {
	FXEvent *ev = (FXEvent*)ptr;
	canvas->ungrab();
	if (mdflag) {

		// Mouse no longer down
		xDrag = 0;
		yDrag = 0;
		resizeable = 0;
		mdflag = 0;
		itemClicked = 1;
	}
	return 1;
}


void ProjectWindow::drawControlHandles()
{
	FXDCWindow dc(canvas);
	currentSelection->drawControlHandles(&dc);

	/*
	dc.setForeground(FXRGB(209, 0, 209));
	FXRectangle* r = project->placeables[currentIndex]->get_rectangle();
	dc.drawRectangle(r->x - 10,
		r->y - 10,
		r->w + 20,
		10);
	dc.drawRectangle(r->x + r->w,
		r->y - 10,
		10,
		r->h + 20);
	dc.drawRectangle(r->x - 10,
		r->y + r->h,
		r->w + 20,
		10);
	dc.drawRectangle(r->x - 10,
		r->y - 10,
		10,
		r->h + 20);
	*/
	

}
// Display Units of selected placeable
void ProjectWindow::displayUnits() {
	// Set Inches for width
	widthText->setText(FXStringVal(currentSelection->get_width() / 16));
	// Set Fraction of Inches
	widthFraction->setCurrentItem(currentSelection->get_width() % 16);
	// Set Inches for height
	heightText->setText(FXStringVal(currentSelection->get_height() / 16));
	// Set Fraction for Height
	heightFraction->setCurrentItem(currentSelection->get_height() % 16);
	// No changes needed to angle
	angleText->setText(FXStringVal(currentSelection->get_angle()));
	// display Name
	cabinet->setText(currentSelection->get_name());

}


void ProjectWindow::updateCursor(int x, int y) {
	//rectangles to check which region the cursor is in
	FXRectangle* r = project->placeables[currentIndex]->get_rectangle();
	FXRectangle* topRect = new FXRectangle(r->x - 10,
		r->y - 10,
		r->w + 20,
		10);
	FXRectangle* rightRect = new FXRectangle(r->x + r->w,
		r->y - 10,
		10,
		r->h + 20);
	FXRectangle* bottomRect = new FXRectangle(r->x - 10,
		r->y + r->h,
		r->w + 20,
		10);
	FXRectangle* leftRect = new FXRectangle(r->x - 10,
		r->y - 10,
		10,
		r->h + 20);

	if (topRect->contains(x, y)) { //if in top region
		if (rightRect->contains(x, y)) { //and right region
			// top-right arrow
			canvas->setDefaultCursor(getApp()->getDefaultCursor(DEF_DRAGTR_CURSOR));
			canvas->setDragCursor(getApp()->getDefaultCursor(DEF_DRAGTR_CURSOR));
		}
		else if (leftRect->contains(x, y)) { //and left region
			// top-left arrow
			canvas->setDefaultCursor(getApp()->getDefaultCursor(DEF_DRAGTL_CURSOR));
			canvas->setDragCursor(getApp()->getDefaultCursor(DEF_DRAGTL_CURSOR));
		}
		else { //just the top
			// up and down arrow
			canvas->setDefaultCursor(getApp()->getDefaultCursor(DEF_DRAGH_CURSOR));
			canvas->setDragCursor(getApp()->getDefaultCursor(DEF_DRAGH_CURSOR));
		}
	}
	else if (bottomRect->contains(x, y)) { //if in bottom region
		if (rightRect->contains(x, y)) { //and right region
			// bottom right arrow
			canvas->setDefaultCursor(getApp()->getDefaultCursor(DEF_DRAGBR_CURSOR));
			canvas->setDragCursor(getApp()->getDefaultCursor(DEF_DRAGBR_CURSOR));
		}
		else if (leftRect->contains(x, y)) { //and left region
			//bottom left arrow
			canvas->setDefaultCursor(getApp()->getDefaultCursor(DEF_DRAGBL_CURSOR));
			canvas->setDragCursor(getApp()->getDefaultCursor(DEF_DRAGBL_CURSOR));
		}
		else { //just the bottom
			// up and down arrow
			canvas->setDefaultCursor(getApp()->getDefaultCursor(DEF_DRAGH_CURSOR));
			canvas->setDragCursor(getApp()->getDefaultCursor(DEF_DRAGH_CURSOR));
		}

	}
	else if (rightRect->contains(x, y)) { //just the right region
		// left and right arrow
		canvas->setDefaultCursor(getApp()->getDefaultCursor(DEF_DRAGV_CURSOR));
		canvas->setDragCursor(getApp()->getDefaultCursor(DEF_DRAGV_CURSOR));
	}
	else if (leftRect->contains(x, y)) { // just the left region
		// left and right arrow
		canvas->setDefaultCursor(getApp()->getDefaultCursor(DEF_DRAGV_CURSOR));
		canvas->setDragCursor(getApp()->getDefaultCursor(DEF_DRAGV_CURSOR));
	}
	delete rightRect;
	delete leftRect;
	delete topRect;
	delete bottomRect;
}


void ProjectWindow::resize(int lastX, int curX, int lastY, int curY) {
	//rectangles to check which region the cursor is in
	FXRectangle* r = project->placeables[currentIndex]->get_rectangle();
	FXRectangle* topRect = new FXRectangle(r->x - 10,
		r->y - 10,
		r->w + 20,
		10);
	FXRectangle* rightRect = new FXRectangle(r->x + r->w,
		r->y - 10,
		10,
		r->h + 20);
	FXRectangle* bottomRect = new FXRectangle(r->x - 10,
		r->y + r->h,
		r->w + 20,
		10);
	FXRectangle* leftRect = new FXRectangle(r->x - 10,
		r->y - 10,
		10,
		r->h + 20);

	// THE REASON THE WHOLE THING DEFORMS IS THE ANGLE!
	//if in the top region
	int grid = project->get_gridSize();
	int xDir = -1;
	int yDir = -1;
	if (curX > lastX) xDir = 1;
	if (curY > lastY) yDir = 1;
	if (topRect->contains(curX, curY)) {
		currentSelection->set_height(currentSelection->get_height() - yDir*grid); //adjust height based on mouse movement
		currentSelection->set_yPos(currentSelection->get_yPos() + yDir*grid); //since this is the top, we have to adjust the y position
	}
	//check if in right region
	if (rightRect->contains(curX, curY)) {
		currentSelection->set_width(currentSelection->get_width() + xDir * grid); //adjust the width
	}
	//check if in bottom region
	if (bottomRect->contains(curX, curY)) {
		currentSelection->set_height(currentSelection->get_height() + yDir * grid); //adjust the height
	}
	//check if in left region
	if (leftRect->contains(curX, curY)) {
		currentSelection->set_width(currentSelection->get_width() - xDir * grid); //adjust the width
		currentSelection->set_xPos(currentSelection->get_xPos() + xDir * grid); //since this is the left, we have to adjust the x position
	}

	drawScreen(); //update the canvas
	displayUnits();

	delete rightRect;
	delete leftRect;
	delete topRect;
	delete bottomRect;
}

FXPoint ProjectWindow::rotateClick(FXPoint click, FXPoint center, int angle)
{
	int tempX, tempY, rotatedX, rotatedY;

	//translate to origin
	tempX = click.x - center.x;
	tempY = click.y - center.y;

	//rotate
	rotatedX = tempX * cos(angle * PI / 180) - tempY * sin(angle * PI / 180);
	rotatedY = tempX * sin(angle * PI / 180) + tempY * cos(angle * PI / 180);

	//translate back to original position
	click.x = rotatedX + center.x;
	click.y = rotatedY + center.y;
	return FXPoint(click.x, click.y);
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

long ProjectWindow::onCmdViewBom(FXObject*, FXSelector, void*) {
	isSplashScreen = 0;
	BomWindow *window = new BomWindow(getApp());
	window->create();
	window->raise();
	window->setFocus();
	return 1;
}

//Update Cabinet Specifications
// Change Grid Size With Slider
long ProjectWindow::onCmdUpdateSpecs(FXObject*, FXSelector, void*) {
	if (currentSelection != NULL) {
		FXint w, h;
		h = (FXIntVal(heightText->getText()) * 16) + heightFraction->getCurrentItem();
		w = (FXIntVal(widthText->getText()) * 16) + widthFraction->getCurrentItem();
		currentSelection->set_height(h);

		currentSelection->set_width(w);

		currentSelection->set_angle(FXIntVal(angleText->getText()));
		currentSelection->set_name(cabinet->getText());
		drawScreen();
	}
	else {
		deselect();
	}
	return 1;
}
// Change Grid Size With Slider
long ProjectWindow::onCmdGridSize(FXObject*, FXSelector, void*) {
	FXint grid, scale = gridSizeSlider->getValue();
	FXString displayText;

	switch (scale) {
	case 1:
		displayText = "1/16\"";
		grid = 1;
		break;
	case 2:
		displayText = "1/8\"";
		grid = 2;
		break;
	case 3:
		displayText = "1/4\"";
		grid = 4;
		break;
	case 4:
		displayText = "1/2\"";
		grid = 8;
		break;
	case 5:
		displayText = "1\"";
		grid = 16;
		break;
	case 6:
		displayText = "2\"";
		grid = 32;
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