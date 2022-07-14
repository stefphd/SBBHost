
#ifndef SBBHOSTWIN_H
#define SBBHOSTWIN_H

#include <gtkmm.h>
#include <string>
#include <vector>
#include <filesystem>
#include <iomanip>
#include "Params.h"
#include "plotter.hpp"
#include "StreamTreeView.h"
#include "PlotTreeView.h"
#include "EntryLabel.h"
#include "config.h"
#include "SBBHostCore.h"
#include "SettingsDialog.h"
#include "HostControl.h"
#include "IniReader.h"
#include "SBBUtils.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(__WIN32__) || defined(WIN64) || defined(_WIN64) || defined(__WIN64) || defined(__WIN64__)
#ifndef OS_WIN
#define OS_WIN
#endif
#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__GNUC__) || defined(__GNUG__) || defined(unix) || defined(__unix) || defined(__unix__)
#ifndef OS_LINUX
#define OS_LINUX
#endif
#else
#error OS not supported
#endif

/*
* Main class of the GUI of the SBBHost application
*/

/*! \brief A class for SBB Host application GUI.
    \details Main class for the GUI of the SBB Host Application, extending the Gtk::Application class.
	\see main main.cpp
    \author Stefano Lovato
    \date 2022
*/
class SBBHostApp : public Gtk::Application {
public:
	/*! \brief Constructor.
		\details Defualt constructor.
	*/
	SBBHostApp();

	/*! \brief Destructor.
		\details Defualt destructor.
	*/
	virtual ~SBBHostApp(); 

	/*! \brief Create the object.
		\details Function to create a SBBHostApp object, used in main().
		\see main main.cpp
		\return The Glib::RefPtr pointer to the created SBBHostApp object.
	*/
	static Glib::RefPtr<SBBHostApp> create();

protected:

	SBBHostCore core; //!< SBBHostCore object for the core management of the application.

	//conf .ini file
	IniReader* p_cfg; //!< Pointer to IniReader object to read the configuration file when application starts.
	
	//pointer to app builder
	Glib::RefPtr<Gtk::Builder> p_builder; //!< Pointer to Gtk::Builder to read the XML file for the toolbase.
	//gtk objects (pointers not necessary, C++ suggests to avoid using them when unnecessary)
	Gtk::ApplicationWindow mainWin; //!< Gtk::ApplicationWindow object for the main application window.
	//Gtk::Notebook noteBook; //!< Gtk::Notebook object for the stream and plot tabs.
	Gtk::HeaderBar headerbar; //!< Gtk::HeaderBar object for the window header bar
	Gtk::Label headerbarlabel; //!< Gtk::Label object for the label of the window header bar
	Gtk::MenuButton menubutton; //!< Gtk::MenuButton object for the menu button
	Gtk::StackSwitcher switcher; //!< Gtk::StackSwitcher object for the tab switcher
	Gtk::Stack stack; //!< Gtk::Stack object for the stream and plot tabs.
	Gtk::Paned streamPaned; //!< Gtk::Paned object for the adjustable horizontal panes in the stream tab.
	Gtk::Paned plotPaned; //!< Gtk::Paned object for the adjustable horizontal panes in the plot tab.
	Gtk::Box streamBox0; //!< Gtk::Box object for a vertical box in the stream tab.
	Gtk::Box streamBox1; //!< Gtk::Box object for a vertical box in the stream tab.
	Gtk::Box streamBox2; //!< Gtk::Box object for a horizontal box in the stream tab.
	Gtk::Box plotBox1; //!< Gtk::Box object for a vertical box in the plot tab.
	Gtk::Box fftBox; //!< Gtk::Box object for a horizontal box for the fft graph.
	Gtk::Overlay plotGraphOverlay; //!< Gtk::Overlay object to overlay graph in the plot tab.
	Gtk::Frame streamTreeFrame; //!< Gtk::Frame object containing a StreamTreeView object.
	Gtk::Frame plotTreeFrame; //!< Gtk::Frame object containing a PlotTreeView object.
	Gtk::Frame streamGraphFrame; //!< Gtk::Frame object containing the stream graph.
	Gtk::Label streamTimeLabel; //!< Gtk::Label object for the elapsed time during the signal streaming.
	Gtk::ToggleButton buttonRun; //!< Gtk::ToggleButton object for the run/stop button.
	Gtk::ToggleButton buttonLog; //!< Gtk::ToggleButton object for the log button.
	Gtk::AboutDialog aboutDialog; //!< Gtk::AboutDialog object for the about dialog of the application
	Glib::ustring def_dir; //!< Default path where to save the log files.
	SettingsDialog settingsDialog; //!< SettingsDialog object for the setting dialog.
	Gtk::Switch darkModeSwitch; //!< Gtk::Switch object for the switch to enable/disable the dark mode.
	Gtk::Switch streamModeSwitch; //!< Gtk::Switch object to switch the stream mode (serial/socket).
	Gtk::ScrolledWindow scrolledWinStream; //!< Gtk::ScrolledWindow object containing a StreamTreeView object. 
	Gtk::ScrolledWindow scrolledWinPlot; //!< Gtk::ScrolledWindow object containing a PlotTreeView object. 
	StreamTreeView streamTreeView; //!< StreamTreeView object for the tree view in the stream tab.
	PlotTreeView plotTreeView; //!< PlotTreeView object for the tree view in the plot tab.
	Glib::Timer timer; //!< Glib::Timer object to count the elased time.
	HostControl hostControl; //!< HostControl object for the host control.
	ComboChoiceEntryLabel plotTypeComboEntry; //!< ComboChoiceEntryLabel object to choose the plot type in the plot tab.

	//pointer to Gtk objects (initialized to nullptr to ensure being empty) - objects cannot be directly used for them (?)
	Glib::RefPtr<Gio::Menu> p_winmenu = nullptr; //!< Pointer to Gio::Menu object for the menu
	Glib::RefPtr<Gio::Menu> p_filemenu = nullptr; //!< Pointer to Gio::Menu object for the file menu
	Glib::RefPtr<Gio::Menu> p_editmenu = nullptr; //!< Pointer to Gio::Menu object for the edit menu
	Glib::RefPtr<Gio::Menu> p_helpmenu = nullptr; //!< Pointer to Gio::Menu object for the help menu
	Glib::RefPtr<Gio::SimpleAction> p_menuRun = nullptr; //!< Pointer to Gio::SimpleAction object for the toolbar run button.
	Glib::RefPtr<Gio::SimpleAction> p_menuLog = nullptr; //!< Pointer to Gio::SimpleAction object for the toolbar log button.
	std::unique_ptr<Gtk::MessageDialog> p_messageDialog = nullptr; //!< Pointer to Gtk::MessageDialog object for message dialogs.
	Gtk::FileChooserDialog* p_fileSaveAsDialog = nullptr; //!< Pointer to Gtk::FileChooserDialog object for the file saveas dialog.
	Gtk::FileChooserDialog* p_fileOpenDialog = nullptr; //!< Pointer to Gtk::FileChooserDialog object for the file open dialog.
	Gtk::FileChooserDialog* p_setFolderDialog = nullptr; //!< Pointer to Gtk::FileChooserDialog object for the setfolder dialog.
	CGraph::CairoGraph* p_streamGraph = nullptr; //!< Pointer to CGraph::CairoGraph object for the graph in the stream tab.
	CGraph::CairoGraph* p_plotGraph = nullptr; //!< Pointer to CGraph::CairoGraph object for the sample-domain graph in the plot tab.
	CGraph::CairoGraph* p_magfftGraph = nullptr; //!< Pointer to CGraph::CairoGraph object for the magnitude-spectra graph in the plot tab.
	CGraph::CairoGraph* p_phfftGraph = nullptr; //!< Pointer to CGraph::CairoGraph object for the phase-spectra graph in the plot tab.

	//Overrides of default signal handlers

	/*! \brief Signal handler called on sturtup.
		\details Function for signal handler called on startup. It performs basic initializations for the application.
		It overrides the virtual member in Gtk::Application.
	*/
	void on_startup() override;

	/*! \brief Signal handler called on activate.
		\details Function for signal handler called on startup. It performs a full initialization for the application.
		It overrides the virtual member in Gtk::Application.
	*/
	void on_activate() override;

	//Signal handlers

	/*! \brief Signal handler called on plot type changed.
		\details Function for signal handler called when changing the plot type in the plot tab by acting on the ComboChoiceEntryLabels.
		\see ComboChoiceEntryLabels
	*/
	void on_plotType_changed();

	/*! \brief Signal handler called on run button toggled.
		\details Function for signal handler called when toggling the run button, in order to run or stop the signal streaming.
	*/
	void on_buttonRun_toggled();

	/*! \brief Signal handler called on log button toggled.
		\details Function for signal handler called when toggling the log button, in order to enable or disable the log. The button is 
		unsensitive during signal streaming (log must be enabled or disabled before signal streaming starts).
	*/
	void on_buttonLog_toggled();

	/*! \brief Signal handler called on darkmode button switched.
		\details Function for signal handler called when switching the darkmode button, in order to enable or disable the dark mode.
	*/
	void on_darkModeSwitch_switched();

	/*! \brief Signal handler called on streamMode button switched.
		\details Function for signal handler called when switching the streamode button, in order to enable or disable the dark mode.
	*/
	void on_streamModeSwitch_switched();

	/*! \brief Signal handler called on set folder menu button
		\details Function for signal handler called when presseing the 'Set Folder' menu button. It opens a Gtk::FileChooserDialog 
		in order to select the desired folder. 
	*/
	void on_menu_file_setfolder();

	/*! \brief Signal handler called on save menu button
		\details Function for signal handler called when presseing the 'Save' menu button. It save the last log (in the stream-tab mode) 
		or the selected logs (in the plot-tab mode). In the latter case it is equivalent to PlotTreeView::on_menu_save().
		\see PlotTreeView::on_menu_save
	*/
	void on_menu_file_save();

	/*! \brief Signal handler called on saveas menu button
		\details Function for signal handler called when presseing the 'Save As' menu button. It open a Gtk::FileChooserDialog 
		in order to save the last log (in the stream-tab mode) or the selected logs (in the plot-tab mode) with a specified name, 
		path, and extension (*.sbb, *.csv, or *.mat).
	*/
	void on_menu_file_saveas();

	/*! \brief Signal handler called on open menu button
		\details Function for signal handler called when presseing the 'Open' menu button. It opens a Gtk::FileChooserDialog 
		in order to select the desired *.sbb file to open.
	*/
	void on_menu_file_open();

	/*! \brief Signal handler called on quit menu button
		\details Function for signal handler called when presseing the 'Quit' menu button. It exits from the application. The user is
		asked for confirmation if unsaved logs are found.
	*/
	void on_menu_file_quit();

	/*! \brief Signal handler called on run menu button.
		\details Function for signal handler called when toggling the run menu button. It starts of ends the signal streaming.
		\see on_buttonRun_toggled
	*/
	void on_menu_stream_run();

	/*! \brief Signal handler called on log menu button.
		\details Function for signal handler called when toggling the log menu button. It enable or disable the signal logging. 
		The button is unsensitive during signal streaming (signal logging must be enabled or disabled before signal streaming starts).
		\see on_buttonLog_toggled
	*/
	void on_menu_stream_log();

	/*! \brief Signal handler called on connect menu button.
		\details Function for signal handler called when presseing the connect menu button. It try to connect to the selected serial 
		port (if any), or to the serial port found. If more serial ports are found, a warning is shown.
	*/
	void on_menu_stream_connect();

	/*! \brief Signal handler called on settings menu button.
		\details Function for signal handler called when presseing the settings menu button.
	*/
	void on_menu_settings();

	/*! \brief Signal handler called on about menu button.
		\details Function for signal handler called when presseing the about menu button. It opens a Gtk::AboutDialog with information 
		about the application.
	*/
	void on_menu_help_about();

	/*! \brief Signal handler called on help menu button.
		\details Function for signal handler called when presseing the help menu button. It opens the documentation.
		about the application.
	*/
	void on_menu_help_help();

	/*! \brief Signal handler called on setting dialog close.
		\details Function for signal handler called when closing the SettingsDialog. 
		Depending on the reponse, it sets the setting parameters or not.
		\param id_response The response ID.
		\see SettingsDialog
	*/
	void on_setting_dialog(int id_response);

	/*! \brief Signal handler called on setfolder dialog close.
		\details Function for signal handler called when closing the setfolder Gtk::FileChooserDialog. 
		Depending on the reponse, it sets the folder or not.
		\param id_response The response ID.
	*/
	void on_file_setfolder_dialog(int id_response);

	/*! \brief Signal handler called on saveAs dialog close.
		\details Function for signal handler called when closing the saveas Gtk::FileChooserDialog. 
		Depending on the reponse, it saves the log or not.
		\param id_response The response ID.
	*/
	void on_file_saveAs_dialog(int id_response);

	/*! \brief Signal handler called on open dialog close.
		\details Function for signal handler called when closing the open Gtk::FileChooserDialog. 
		Depending on the reponse, it opens the log or not.
		\param id_response The response ID.
	*/
	void on_file_open_dialog(int id_response);

	/*! \brief Signal handler called on quit confirmation.
		\details Function for signal handler called when exiting from the application in order 
		to ask the user for confirmation when one or more unsaved log exists.
		Depending on the reponse, it quits from the application or not.
		\param id_response The response ID.
	*/
	void on_quit_confirmation(int id_response);

	/*! \brief Signal handler called drag-and-drop files into application.
		\details Function for signal handler called when dragging-and-dropping one or more *.sbb files into the application.
		\param value The Value being dropped.
		\param x The x coordinate of the current pointer position.
		\param y The y coordinate of the current pointer position. 
		\return Whether the drop was accepted at the given pointer position.
	*/
	bool on_file_drag_and_drop(const Glib::ValueBase& value, double x, double y);

	/*! \brief Signal handler called during streaming.
		\details Function for signal handler called continously and as soon as possible during signal streaming, in order to read the 
		packet from the host port
		\return Whether the signal streaming should continue or not.
		\see SBBHostCore Packet HostPort Serial
	*/
	bool on_read_serial();

	/*! \brief Signal handler called for close request.
		\details Function for signal handler called when closing the main application window.
		\return true to stop other handlers from being invoked for the signal and continue the window close.
	*/
	bool on_win_close();

	//other
	/*! \brief Show warning dialog.
		\details Function to show a warning dialog with a message.
		\param type The type of the message to be shown (see exit flags in config.h for details).
	*/
	void warning(int type);

	/*! \brief Show information dialog.
		\details Function to show an information dialog with a message.
		\param type The type of the message to be shown (see exit flags in config.h for details).
	*/
	void info(int type);

	/*! \brief Open a file and update the plot tree view.
		\details Function to open the file with SBBHostCore::open and update the PlotTreeView object.
		\param filename The comple file name to open (path+name).
		\return An exit flag (see config.h for details).
		\see SBBHostCore::open PlotTreeView
	*/
	int open(std::string filename);

private:
	//class variables
	int id = NULL_ID; //!< Current log ID (used in save as).
	bool force_close = false; //!< Force the application to close.
	int exitFlag = EXIT_DEFAULT; //!< Exit flag (see config.h for details).
	double streamTimer; //!< Value of the stream timer.
	double streamGraphTimer; //!< Value of the stream-graph timer.
	
	//group params settable in settings in a struct 
	Params params; //!< Params object to save application parameters.

	//class funs
	/*! \brief Create graph.
		\details Function to create the graph.
	*/
	void create_graph();

	/*! \brief Create toolbar.
		\details Function to create the toolbar and set its signal handlers.
	*/
	void create_toolbar();

	/*! \brief Create dialogs.
		\details Function to create and set the dialogs.
	*/
	void create_dialogs();

	/*! \brief Parse the configuration file.
		\details Function to parse and read the configuration file using the IniReader class.
		\see IniReader
	*/
	void parse_ini();

	/*! \brief Set tooltips.
		\details Function to set the tooltips of the widgets.
	*/
	void set_tooltips();

	/*! \brief Set graph.
		\details Function to set the graph.
	*/
	void set_graph();

	/*! \brief Set tree view.
		\details Function to set the stream and plot tree views.
		\see StreamTreeView PlotTreeView
	*/
	void set_tree();

	/*! \brief Set timer.
		\details Function to set the timer label during signal streaming.
	*/
	void set_streamTimer(int secs);

	/*! \brief Clear stream graph.
		\details Function to clear the stream graph.
	*/
	void clear_stream_graph();

	/*! \brief Set drag and drop.
		\details Function to set the drag-and-drop signal handlers.
	*/
	void set_drag_and_drop();

	/*! \brief Set host control.
		\details Function to set the host control widget.
	*/
	void set_host_control();
};

#endif