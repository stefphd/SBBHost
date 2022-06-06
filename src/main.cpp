/*! \file main.cpp
	\brief Main file of the application.
	\details Main file of the SBB Host Applicaton. The SBBHostApp object is created as a Glib::RefPtr and 
	run.
*/

#include "SBBHostApp.h"

#ifdef OS_WIN 
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")  //do not show command line when running in Windows
#endif

/*! \brief Main function of the application.
	\details Main file of the SBB Host Applicaton. The SBBHostApp object is created as a Glib::RefPtr and 
	run.
	\param argc The number of input char.
	\param argv Pointer to a char array.
	\return The exit status of the application.
*/
int main(int argc, char* argv[]) {
	Glib::RefPtr<SBBHostApp> app = SBBHostApp::create(); //create object
	return app->run(argc, argv); //run app
}
