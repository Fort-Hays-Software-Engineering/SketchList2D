#include "BomWindow.h"
#include <FXComboBox.h>
#include <FXText.h>
#include <FXGLCanvas.h>
#include <FXGLVisual.h>
#include <FXRadioButton.h>
#include <FXDCWindow.h>


#pragma comment (lib, "ws2_32")

FXDEFMAP(BomWindow) BomWindowMap[] = {

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_PAINT,             BomWindow::ID_CANVAS,			BomWindow::onPaint),
	FXMAPFUNC(SEL_COMMAND,           BomWindow::ID_SAVE,			BomWindow::onCmdSave),
	FXMAPFUNC(SEL_COMMAND,           BomWindow::ID_SAVEAS,			BomWindow::onCmdSaveAs),
	FXMAPFUNC(SEL_COMMAND,           BomWindow::ID_PRINT,			BomWindow::onCmdPrint),

};



// Macro for the ProjectListApp class hierarchy implementation
FXIMPLEMENT(BomWindow, FXMainWindow, BomWindowMap, ARRAYNUMBER(BomWindowMap))


// Construct a BomWindow
BomWindow::BomWindow(FXApp *a) :FXMainWindow(a, "SketchList 2D Room Designer", NULL, NULL, DECOR_ALL, 0, 0, 800, 600), mrufiles(a) {


	// Add to list of windows
	//getApp()->windowlist.append(this);
	project = new Project;


	topdock = new FXDockSite(this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X);
	// Make menu bar
	menubar = new FXMenuBar(topdock, LAYOUT_DOCK_NEXT | LAYOUT_SIDE_TOP | LAYOUT_FILL_X | FRAME_RAISED);

	contents = new FXHorizontalFrame(this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
	table = new FXTable(contents, NULL, NULL, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
	configureTable();






	// Status bar
	statusbar = new FXStatusBar(this, LAYOUT_SIDE_BOTTOM | LAYOUT_FILL_X | STATUSBAR_WITH_DRAGCORNER | FRAME_RAISED);

	// File menu
	filemenu = new FXMenuPane(this);
	new FXMenuTitle(menubar, tr("&File"), NULL, filemenu);

	// File Menu entries

	new FXMenuCommand(filemenu, tr("&Print...\tCtl-P\tPrint document."), NULL, this, ID_PRINT);


	new FXMenuSeparator(filemenu);
	new FXMenuCommand(filemenu, tr("&Exit\tAlt-F4\tExit Program."), NULL, this, FXApp::ID_QUIT);

	// Initialize private variables
	drawColor = FXRGB(207, 207, 207);
	//mdflag = 0;
	//dirty = 0;
	itemClicked = 0;
	resizeable = 0;
	xDrag = 0;
	yDrag = 0;

	// Recent files
	mrufiles.setTarget(this);
	mrufiles.setSelector(ID_OPEN_RECENT);

}



BomWindow::~BomWindow() {
}


// Create and initialize
void BomWindow::create() {

	// Create the windows
	FXMainWindow::create();
	filemenu->create();
	// Make the main window appear
	show(PLACEMENT_SCREEN);

}

// Paint the canvas
long BomWindow::onPaint(FXObject*, FXSelector, void* ptr) {
	
	ProjectWindow* par = (ProjectWindow*)getParent();
	project = par->getProject();
	
	return 1;
}


long BomWindow::configureTable() {
	table->insertColumns(0, 3);
	table->setColumnWidth(0, 100);
	table->setColumnWidth(1, 200);
	table->setColumnWidth(2, 300);
	table->setColumnText(0, "Item Number");
	table->setColumnText(1, "Item Name");
	table->setColumnText(2, "Find it Online");
	table->insertRows(0, 20);
	table->showHorzGrid();
	table->showVertGrid();
	for (int i = 0; i < 3; i++) {
		
	}
	return 1;
}

long BomWindow::onCmdPrint(FXObject*, FXSelector, void*) {
	FXPrintDialog dlg(this, tr("Print File"));
	FXPrinter printer;
	if (dlg.execute()) {
		dlg.getPrinter(printer);
		FXTRACE((100, "Printer = %s\n", printer.name.text()));
	}
	return 1;
}




// Save
long BomWindow::onCmdSave(FXObject* sender, FXSelector sel, void* ptr) {
	if (!filenameset) return onCmdSaveAs(sender, sel, ptr);

	saveFile(filename);
	return 1;
}

// Save As
long BomWindow::onCmdSaveAs(FXObject*, FXSelector, void*) {
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
FXbool BomWindow::saveFile(const FXString& file) {
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





FXbool BomWindow::close(FXbool notify) {
	// Perform normal close stuff


	return FXMainWindow::close(notify);
}

// Detach window
void BomWindow::detach() {
	FXMainWindow::detach();
	dragshell1->detach();
	urilistType = 0;
}