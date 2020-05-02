/********************************************************************************
*                                                                               *
*                         ProjectList 2D GUI Code                                *
*                                                                               *
********************************************************************************/
#ifndef ProjectWindow_H
#define ProjectWindow_H

#include "fx.h"
#include "Project.h";
#include "Placeable.h"
#include "BomWindow.h"


class NewWindow;


// Main Window
class ProjectWindow : public FXMainWindow {

	// Macro for class hierarchy declarations
	FXDECLARE(ProjectWindow)

private:

	FXHorizontalFrame *contents;                // Content frame

	FXVerticalFrame   *LeftPanel;               // Left control panel
	FXComboBox        *placeableTypeComboBox;   // List of placables the user chooses from

	FXButton          *createNewPlaceableButton;// Button to create a new placable

	FXVerticalFrame   *placeableDataPanel;      // The panel where data about the currently selected placeable is displayed

	FXHorizontalFrame *widthFrame;              // Width frame of the data panel
	FXTextField       *widthText;
	FXComboBox		  *widthFraction;

	FXVerticalFrame  *cabinetFrame;
	FXTextField       *cabinet;                 //cabinet test

	FXHorizontalFrame *unitsFrame;              // units frame of the data panel
	FXRadioButton     *feetRadio;
	FXRadioButton     *inchRadio;

	FXHorizontalFrame *heightFrame;             // height frame of the data panel
	FXTextField       *heightText;
	FXComboBox		  *heightFraction;

	FXSlider		  *gridSizeSlider;			// Slider selector for adjustable grid Size
	FXTextField		  *gridSizeDisplay;
	FXDataTarget	   gridSizeTarget;

	FXVerticalFrame   *createNewProjectFrame;   // Create New Project label and button
	FXVerticalFrame   *loadProjectFrame;        // Splash screen load new project frame
	FXHorizontalFrame *existingProjectList;     // Horizontal frame for exiting project list
	FXVerticalFrame   *canvasFrame;             // Canvas frame
	FXVerticalFrame   *buttonFrame;             // Button frame
	FXCanvas          *canvas;                  // Canvas to draw into
	int                mdflag;                  // Mouse button down?
	int                dirty;                   // Canvas has been painted?
	int                isSplashScreen;      // Is this the opening splash screen?
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
	void drawControlHandles();
	bool checkResizeArea(int x, int y);
	void updateCursor(int x, int y);
	void resize(int lastX, int curX, int lastY, int curY);
	FXPoint rotateClick(FXPoint click, FXPoint center, int angle);
	// Scale coordinate to grid Size


protected:
	ProjectWindow() {}
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
	ProjectWindow *findWindow(const FXString& file) const;
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
	long onCmdViewBom(FXObject*, FXSelector, void*);

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

	Project* getProject();

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

	// ProjectWindow's constructor
	ProjectWindow(FXApp* a);

	// Initialize
	virtual void create();

	virtual void detach();
	// Close the window, return TRUE if actually closed
	virtual FXbool close(FXbool notify = FALSE);
	//ProjectWindow* getApp() const { return (ProjectWindow*)FXMainWindow::getApp(); }

	// Save text to file
	FXbool saveFile(const FXString& file);
	FXbool loadFile(const FXString& file);
	virtual ~ProjectWindow();
};

void configurePlaceableComboBox(FXComboBox *comboBox);
void configureFractionComboBox(FXComboBox *box);
#endif // !ProjectWindow_H