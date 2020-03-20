#pragma once
#define VERSION_MAJOR 0
#define VERSION_MINOR 1
#define VERSION_PATCH 0

class TestWindow;

class SketchList2D : public FXApp {
	FXDECLARE(SketchList2D)
	friend class TestWindow;

protected:
	FX::FXObjectListOf<TestWindow> windowlist;    //Window list
	FXFileDict     *associations;                 // File association table

private:
	SketchList2D() {}
	SketchList2D(const SketchList2D&);
	//SketchList2D& operator=(const SketchList2D&);

public:
	enum {
		ID_CLOSEALL = FXApp::ID_LAST,
		ID_LAST
	};

public:
	long onCmdCloseAll(FXObject*, FXSelector, void*);

public:

	//App object
	SketchList2D(const FXString &name);

	// Initialize application
	virtual void init(int& argc, char** argv, bool connect = TRUE);

	// Exit application
	virtual void exit(FXint code = 0);

	// Delete application object
	virtual ~SketchList2D();
};