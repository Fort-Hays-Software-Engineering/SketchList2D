
/********************************************************************************
*                                                                               *
*                         SplashList 2D GUI Code                                *
*                                                                               *
********************************************************************************/
#ifndef SPLASHWINDOW_H

#include "fx.h"

class NewWindow;


// Main Window
class SplashWindow : public FXMainWindow {

	// Macro for class hierarchy declarations
	FXDECLARE(SplashWindow)

private:

	FXHorizontalFrame *contents;                // Content frame
	FXHorizontalFrame *middleSplashFrame;       // Middle pane of the splash screen
	FXVerticalFrame   *createNewProjectFrame;   // Create New Project label and button
	FXVerticalFrame   *loadProjectFrame;        // Splash screen load new project frame
	FXHorizontalFrame *existingProjectList;     // Horizontal frame for exiting project list

protected:
	SplashWindow() {}
	FXMenuPane          *filemenu;                // File menu
	FXMenuBar           *menubar;                 // Menu bar
	FXStatusBar         *statusbar;               // Status bar
	FXDockSite          *topdock;                 // Topmost dock area
	FXToolBarShell      *dragshell1;              // Shell for floating menubar

protected:

	FXIcon *newicon;
public:

	//create a new project
	long onCmdNewProject(FXObject*, FXSelector, void*);
	//Open project
	long onCmdOpen(FXObject*, FXSelector, void*);
	FXApp* getApp() const { return (FXApp*)FXMainWindow::getApp(); }


public:

	// Messages for our class
	enum {
		ID_CANVAS = FXMainWindow::ID_LAST,
		ID_CLEAR,
		ID_NEW,
		ID_OPEN,
		ID_NEWPROJECT,
		ID_LAST
	};

public:

	// SplashWindow's constructor
	SplashWindow(FXApp* a);

	// Initialize
	virtual void create();

	virtual void detach();
	// Close the window, return TRUE if actually closed
	virtual FXbool close(FXbool notify = FALSE);
	//SplashWindow* getApp() const { return (SplashWindow*)FXMainWindow::getApp(); }

	// File Management functions
	FXbool loadFile(const FXString& file);

	virtual ~SplashWindow();
};
#endif // !SPLASHWINDOW_H