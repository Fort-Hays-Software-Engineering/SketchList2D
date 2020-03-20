#pragma once
#ifndef TESTWINDOW_H
#define TESTWINDOW_H

class SketchList2D;

class TestWindow : public FXMainWindow {
	FXDECLARE(TestWindow)

protected:
	FXMenuPane *filemenu;
	FXMenuBar *menubar;
	FXText *editor;
	FXDirList *dirlist;
	FXString filename;

private:
	TestWindow() {}
	TestWindow(const TestWindow&);
	TestWindow(SketchList2D* a, const FXString* file);
	//TestWindow& operator=(const TestWindow&);

protected:
	TestWindow *findWindow(const FXString& file) const;
	FXString unique() const;

public:
	long onCmdNew(FXObject*, FXSelector, void*);
	long onCmdOpen(FXObject*, FXSelector, void*);

public:
	enum {
		ID_NEW,
		ID_OPEN,
		ID_WINDOW,
		ID_LAST
	};

public:
	TestWindow(SketchList2D* a, const FXString& file);

	virtual void create();
	virtual void detach();
	virtual FXbool close(FXbool notify = FALSE);
	SketchList2D* getApp() const { return (SketchList2D*)FXMainWindow::getApp(); }
	const FXString& getFilename() const { return filename; }

	virtual ~TestWindow();
};


typedef FXObjectListOf<TestWindow> TestWindowList;

#endif