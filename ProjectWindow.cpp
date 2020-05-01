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

	angleFrame = new FXHorizontalFrame(placeableDataPanel, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
	new FXLabel(angleFrame, "Angle", NULL, JUSTIFY_CENTER_X | LAYOUT_FILL_X);
	angleText = new FXTextField(angleFrame, 5, NULL, 0, TEXTFIELD_INTEGER | FRAME_LINE);

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
		project->addPlaceable(project->get_gridSize(), project->get_gridSize(), 1920, 64); // 4" x 10' wall
		break;
	case 1:
		project->addPlaceable(project->get_gridSize(), project->get_gridSize(), 64, 1920); // 4" x 10' wall
		break;
	case 2:
		project->addPlaceable(project->get_gridSize(), project->get_gridSize(), 384, 480); // standard 24" by 30" cabinet
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
		project->placeables[i]->draw(&dc, project->get_gridSize());
	}
	if(currentSelection != NULL)
		drawControlHandles();


}

bool ProjectWindow::checkResizeArea(int x, int y) {
	if (currentSelection == NULL)
		return false;
	FXRectangle* focusRect = new FXRectangle(project->placeables[currentIndex]->get_xPos() - 5,
		project->placeables[currentIndex]->get_yPos() - 5,
		project->placeables[currentIndex]->get_width() + 10,
		project->placeables[currentIndex]->get_height() + 10

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

	int xStart, xEnd, yStart, yEnd;
	int rotatedX, rotatedY;

	FXDCWindow dc(canvas);
	if (mdflag == 0) {

		if (checkResizeArea(clickX, clickY)) {
			resizeable = 1;
		}
		else {
			resizeable = 0;
			//iterate through placeables seeing if this position is inside one of the rectangles
			for (int i = 0; i < project->get_placeableCount(); i++) {

				if (project->placeables[i]->isClicked(clickX, clickY)) {
					currentSelection = project->placeables[i];
					currentIndex = i;
					drawScreen();
					itemClicked = 1;
					mdflag = 1;
					widthText->setText(FXStringVal(project->placeables[i]->get_width()));
					heightText->setText(FXStringVal(project->placeables[i]->get_height()));
					angleText->setText(FXStringVal(project->placeables[i]->get_angle()));
					return 1;

				}
				else { // No item clicked, Deselect
					deselect();
				}

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

	//set the default cursor state is a normal arrow cursor
	canvas->setDefaultCursor(getApp()->getDefaultCursor(DEF_ARROW_CURSOR));
	canvas->setDragCursor(getApp()->getDefaultCursor(DEF_ARROW_CURSOR));

	if (currentSelection != NULL) { //if there is a selected placeable
		//rotate cursor against current selection
		int tempX, tempY, rotatedX, rotatedY;
		FXPoint center = FXPoint(currentSelection->get_xPos() + currentSelection->get_width() * .5,
			                     currentSelection->get_yPos() + currentSelection->get_height() * .5);

		//translate to origin
		tempX = ev->win_x - center.x;
		tempY = ev->win_y - center.y;

		//rotate
		rotatedX = tempX * cos(-currentSelection->get_angle() * PI / 180) - tempY * sin(-currentSelection->get_angle() * PI / 180);
		rotatedY = tempX * sin(-currentSelection->get_angle() * PI / 180) + tempY * cos(-currentSelection->get_angle() * PI / 180);

		//translate back to original position
		rotatedX += center.x;
		rotatedY += center.y;
		if (checkResizeArea(rotatedX, rotatedY)) { //if the cursor is in the area to click and drag resize
			updateCursor(rotatedX, rotatedY); //update the cursor to drag arrows
		}
		if (itemClicked == 1 && mdflag == 1) {
			project->placeables[currentIndex]->set_xPos(project->placeables[currentIndex]->get_xPos() + ev->win_x - ev->last_x);
			project->placeables[currentIndex]->set_yPos(project->placeables[currentIndex]->get_yPos() + ev->win_y - ev->last_y);
			drawScreen();
		}
		if (resizeable && mdflag) { //if the cursor is in the area to resize, and the mouse is down
			int lastX, lastY;
			lastX = ev->last_x;
			lastY = ev->last_y;
			tempX = lastX - center.x;
			tempY = lastY - center.y;

			//rotate
			lastX = tempX * cos(-currentSelection->get_angle() * PI / 180) - tempY * sin(-currentSelection->get_angle() * PI / 180);
			lastY = tempX * sin(-currentSelection->get_angle() * PI / 180) + tempY * cos(-currentSelection->get_angle() * PI / 180);

			//translate back to original position
			lastX += center.x;
			lastY += center.y;
			resize(lastX, rotatedX, lastY, rotatedY); //resize the current selection
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
	currentSelection->drawControlHandles(&dc);

	dc.setForeground(FXRGB(209, 0, 209));
	FXRectangle* r = project->placeables[currentIndex]->get_rectangle();
	dc.drawRectangle(r->x - 5,
		r->y - 5,
		r->w + 10,
		10);
	dc.drawRectangle(r->x + r->w,
		r->y - 5,
		10,
		r->h + 10);
	dc.drawRectangle(r->x - 5,
		r->y + r->h,
		r->w + 10,
		10);
	dc.drawRectangle(r->x - 5,
		r->y - 5,
		10,
		r->h + 10);
	
	

}

void ProjectWindow::updateCursor(int x, int y) {
	//rectangles to check which region the cursor is in
	FXRectangle* r = project->placeables[currentIndex]->get_rectangle();
	FXRectangle* topRect = new FXRectangle(r->x - 5,
		r->y - 5,
		r->w + 10,
		10);
	FXRectangle* rightRect = new FXRectangle(r->x + r->w,
		r->y - 5,
		10,
		r->h + 10);
	FXRectangle* bottomRect = new FXRectangle(r->x - 5,
		r->y + r->h,
		r->w + 10,
		10);
	FXRectangle* leftRect = new FXRectangle(r->x - 5,
		r->y - 5,
		10,
		r->h + 10);

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
	FXRectangle* topRect = new FXRectangle(r->x - 5,
		r->y - 5,
		r->w + 10,
		10);
	FXRectangle* rightRect = new FXRectangle(r->x + r->w,
		r->y - 5,
		10,
		r->h + 10);
	FXRectangle* bottomRect = new FXRectangle(r->x - 5,
		r->y + r->h,
		r->w + 10,
		10);
	FXRectangle* leftRect = new FXRectangle(r->x - 5,
		r->y - 5,
		10,
		r->h + 10);

	//if in the top region
	if (topRect->contains(curX, curY)) {
		currentSelection->set_height(currentSelection->get_height() + lastY - curY); //adjust height based on mouse movement
		currentSelection->set_yPos(currentSelection->get_yPos() + curY - lastY); //since this is the top, we have to adjust the y position
	}
	//check if in right region
	if (rightRect->contains(curX, curY)) {
		currentSelection->set_width(currentSelection->get_width() + curX - lastX); //adjust the width
	}
	//check if in bottom region
	if (bottomRect->contains(curX, curY)) {
		currentSelection->set_height(currentSelection->get_height() + curY - lastY); //adjust the height
	}
	//check if in left region
	if (leftRect->contains(curX, curY)) {
		currentSelection->set_width(currentSelection->get_width() + lastX - curX); //adjust the width
		currentSelection->set_xPos(currentSelection->get_xPos() + curX - lastX); //since this is the left, we have to adjust the x position
	}

	drawScreen(); //update the canvas
	widthText->setText(FXStringVal(project->placeables[currentIndex]->get_width())); //update the width in the text box
	heightText->setText(FXStringVal(project->placeables[currentIndex]->get_height())); //update the height in the text box

	delete rightRect;
	delete leftRect;
	delete topRect;
	delete bottomRect;
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
	currentSelection->set_angle(FXIntVal(angleText->getText()));

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