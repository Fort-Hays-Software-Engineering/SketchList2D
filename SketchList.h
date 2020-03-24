
/********************************************************************************
*                                                                               *
*                         SketchList 2D GUI Code                                *
*                                                                               *
********************************************************************************/
#ifndef SKETCHLIST_H

#include "fx.h"



// Main Window
class SketchWindow : public FXMainWindow {

	// Macro for class hierarchy declarations
	FXDECLARE(SketchWindow)

private:

	FXHorizontalFrame *contents;                // Content frame
	FXVerticalFrame   *canvasFrame;             // Canvas frame
	FXVerticalFrame   *buttonFrame;             // Button frame
	FXCanvas          *canvas;                  // Canvas to draw into
	int                mdflag;                  // Mouse button down?
	int                dirty;                   // Canvas has been painted?
	FXColor            drawColor;               // Color for the line

protected:
	SketchWindow() {}
	FXMenuPane          *filemenu;                // File menu
	FXMenuBar           *menubar;                 // Menu bar
	FXStatusBar         *statusbar;               // Status bar
	FXDockSite          *topdock;                 // Topmost dock area
	FXToolBarShell      *dragshell1;              // Shell for floating menubar

protected:
	FXString unique() const;
	SketchWindow *findWindow(const FXString& file) const;
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

public:

	// Messages for our class
	enum {
		ID_CANVAS = FXMainWindow::ID_LAST,
		ID_CLEAR,
		ID_NEW,
		ID_OPEN,
		ID_LAST
	};

public:

	// SketchWindow's constructor
	SketchWindow(FXApp* a);

	// Initialize
	virtual void create();

	virtual ~SketchWindow();
};
#endif // !SKETCHLIST_H