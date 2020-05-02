/********************************************************************************
*                                                                               *
*                         ProjectList 2D GUI Code                                *
*                                                                               *
********************************************************************************/
#ifndef BomWindow_H
#define BomWindow_H
#include "fx.h"
#include "Project.h";
#include "Placeable.h"
#include "ProjectWindow.h"


class NewWindow;


// Main Window
class BomWindow : public FXMainWindow {

	// Macro for class hierarchy declarations
	FXDECLARE(BomWindow)

private:

	FXHorizontalFrame *contents;                // Content frame
	FXTable			  *table;

	FXColor            drawColor;               // Color for the grid lines
	FXColor			   placeableColor;			// Color for placeables
	FXHorizontalFrame *angleFrame;              //frame to hold the angle data text box and label
	FXTextField		  *angleText;               //the angle of the placeable



	int itemClicked;
	int resizeable; //whether mouse movement should resize the current object
	int xDrag; //how far the mouse has been dragged this click
	int yDrag; //how far the mouse has been dragged this click

	Project* project;

	Placeable *currentSelection;
	int currentIndex;

	// Scale coordinate to grid Size


protected:
	BomWindow() {}
	FXMenuPane          *filemenu;                // File menu
	FXMenuBar           *menubar;                 // Menu bar
	FXStatusBar         *statusbar;               // Status bar
	FXDockSite          *topdock;                 // Topmost dock area
	FXToolBarShell      *dragshell1;              // Shell for floating menubar
	FXString             filename;                // File being edited
	FXbool               filenameset;             // Filename is set
	FXTime               filetime;                // Time File was saved
	FXRecentFiles        mrufiles;                // Recent files list

protected:
	FXString unique() const;
	BomWindow *findWindow(const FXString& file) const;
	FXIcon *newicon;

public:

	// Message handlers
	long onPaint(FXObject*, FXSelector, void*);
	long onMouseDown(FXObject*, FXSelector, void*);
	long onMouseUp(FXObject*, FXSelector, void*);
	long onMouseMove(FXObject*, FXSelector, void*);
	long onCmdClear(FXObject*, FXSelector, void*);
	long onUpdClear(FXObject*, FXSelector, void*);

	// File Management Message Handlers
	long onCmdNew(FXObject*, FXSelector, void*);
	long onCmdOpen(FXObject*, FXSelector, void*);
	long onCmdSave(FXObject*, FXSelector, void*);
	long onCmdSaveAs(FXObject*, FXSelector, void*);
	long onCmdOpenRecent(FXObject*, FXSelector, void* ptr);
	long onCmdPrint(FXObject*, FXSelector, void*);
	long configureTable();

	//create a new project
	long onCmdNewProject(FXObject*, FXSelector, void*);

	//make a new placable
	long onCmdNewPlacable(FXObject*, FXSelector, void*);

	void drawScreen(); //draw the canvas

	FXApp* getApp() const { return (FXApp*)FXMainWindow::getApp(); }
	void displayUnits();
	long onCmdGridSize(FXObject*, FXSelector, void*);
	long onCmdUpdateSpecs(FXObject*, FXSelector, void*);
	long deselect();

public:

	// Messages for our class
	enum {
		ID_CANVAS = FXMainWindow::ID_LAST,
		ID_CLEAR,
		ID_NEW,
		ID_OPEN,
		ID_SAVE,
		ID_SAVEAS,
		ID_NEWPROJECT,
		ID_NEWPLACEABLE,
		ID_CABINET,
		ID_VIEWBOM,
		ID_LAST,
		ID_OPEN_RECENT,
		ID_GRIDSIZE,
		ID_UPDATESPECS,
		ID_PRINT
	};

public:

	// BomWindow's constructor
	BomWindow(FXApp* a);

	// Initialize
	virtual void create();

	virtual void detach();
	// Close the window, return TRUE if actually closed
	virtual FXbool close(FXbool notify = FALSE);
	//BomWindow* getApp() const { return (BomWindow*)FXMainWindow::getApp(); }

	// Save text to file
	FXbool saveFile(const FXString& file);
	FXbool loadFile(const FXString& file);
	virtual ~BomWindow();
};

void configurePlaceableComboBox(FXComboBox *comboBox);

#endif // !BomWindow_H