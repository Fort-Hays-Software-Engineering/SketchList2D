#include "SplashWindow.h"
#include "Project.h"
#include "fx.h"

int main(int argc, char *argv[]) {


	// Make application
	FXApp application("SketchList", "SketchListRD");

	// Start app
	application.init(argc, argv);

	// SketchList window
	SplashWindow* mainWindow = new SplashWindow(&application);
	//SketchWindow* projWindow = new SketchWindow(&application, 0);


	// Create the application's windows
	application.create();
	mainWindow->show();
	//projWindow->show();

	// Run the application
	return application.run();
}