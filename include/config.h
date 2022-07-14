/*! \file config.h
	\brief A configuration file for the source code.
	\details Configuration file with defines and macros for the source code of the 
	SBB Host application. This includes applicaton information (authors, version, etc...), 
	default signal and control names, (default) settings for the SBBHostApp and SBBHostCore, 
	appearance settings for the windows (e.g. see SettingsDialog, HostControl) and graph, 
	default file extensions used, values for the exit flag for warnings and errors, 
	and settings for local environment variables.
	\see SBBHostApp, SBBHostCore, SettingsDialog, HostControl
	\note Building is necessary for changes to take effect.
	\warning Be careful to change values of this file.
	\author Stefano Lovato
	\date 2022
*/

#ifndef CONFIG_H
#define CONFIG_H

#include "dirs.h"

//OS flags
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

//app infos
#define VERSION "0.1" //!< App version.
#define AUTHOR_COPYRIGHT "Stefano Lovato" //!< Author copyright list.
#define APP_COMMENTS "This is an application to stream the input & output data from the self-balancing bike, as well as to read the datalog file" //!< App description.
#define LICTYPE "MIT License" //!< License type.
#define WEBSITE "https://www.dii.unipd.it/" //!< Website.
#define WEBSITE_LABEL "DII UniPd website" //!< Website label.
#define LIST_AUTHOR "Stefano Lovato" //!< App author.

//defaut SIGNALS_NAMES and CONTROL_NAMES - can be changed config.ini before running.
#define SIGNALS_NAMES	"Speed", "Dist", \
						"Acc x", "Acc y", "Acc z", "Gyro x", "Gyro y", "Gyro z", \
						"Mag x", "Mag y", "Mag z", \
						"Lat", "Long", "GPS Speed", \
						"Steer pos", "Steer vel", "Fork disp", "Curr act", "Steer trq", \
						"CPU temp", "Voltage", "Ref input 1", "Ref input 2", "Err state in", \
						"Curr ref", "Throttle", \
						"Roll est", "Yaw est", "Pos est x", "Pos est y", "Err state out", \
						"User data 1", "User data 2", "User data 3", "User data 4" //!< Default signal names (if unable to load them from the configuration file).
#define CONTROL_NAMES	"Curr ref", "Throttle" //!< Default control names (if unable to load them from the configuration file).
#define CONTROL_MIN		-1, -1 //!< Default control minimum values (if unable to load them from the configuration file).
#define CONTROL_MAX		+1, +1 //!< Default control maximum values (if unable to load them from the configuration file).
#define CONTROL_VAL		+0, 0  //!< Default control values (if unable to load them from the configuration file).

//core settings
#if defined(OS_WIN)
#define CONFIG_FILE "sbbhost-config.ini" //!< Name of the configuration file.
#else
#define CONFIG_FILE CONFIG_FILE_LINUX //!< Name of the configuration file.
#endif
#define PACKET_SIZE 64 //!< Size of the data packet (do not change unless necessary.
#define MAX_MISSING_PACKETS 100 //!< Default maximum missing packets (if unable to load it from the configuration file).
#define NULL_PORT -1 //!< Null serial port.
#define MAX_PORT 128 //!< Max port number of serial communication.
#define MAX_LOGGED_SAMPLES 500*60*60 //!< Max number of logged data element.
#define FILENAME_MAXSIZE 128 //!< Max length of the logged-data file name.
#define FILENAME_FORMAT_STRING "log_%04d-%02d-%02d_%02d-%02d-%02d" //!< Default format for the logged-data file name.
#define FILENAME_FORMAT_ENTRIES time.getYear(), time.getMonth(), time.getDay(), time.getHour(), time.getMin(), time.getSec() //!< Entries for the default format of the logged-data file name.
#define NULL_ID -1 //!< Null log ID.
#define NAN_VAL 0xffc00000 //!< NaN val sent when controls not enabled.

//serial settings
#define BAUD 115200 //!< Defalt baudrate of serial communication (if unable to load it from the configuration file).

//socket settings
#define SOCKET_IP "192.168.0.1" //!< Default IP.
#define SOCKET_PORT 1234 //!< Default port.

//GUI settings
#define APP_NAME "SBB Host" //!< App name.
#define RUN_BUTLABEL "Run" //!< Run button label - not used.
#define STOP_BUTLABEL "Stop" //!< Stop button label - not used.
#define RUN_BUTICON "media-playback-start-symbolic" //!< Run button symbol
#define STOP_BUTICON "media-playback-stop-symbolic" //!< Stop button symbol
#define LOG_BUTLABEL "Log" //!< Log button label.
#define STREAM_LABEL "Stream" //!< Stream tab label.
#define PLOT_LABEL "Plot" //!< Plot tab label.
#define STREAM_PAGE 0 //!< Stream tab page id.
#define PLOT_PAGE 1 //!< Plot tab page id.
#define MAINWIN_WIDTH 1080 //!< Default main window width (if unable to load it from the configuration file).
#define MAINWIN_HEIGHT 720 //!< Default main window height (if unable to load it from the configuration file).
#define MARGIN 10 //!< Margin between widgets.
#define MESSDIALOG_WIDTH 400 //!< Default message dialog window width (if unable to load it from the configuration file).
#define MESSDIALOG_HEIGHT 200 //!< Default message dialog window height (if unable to load it from the configuration file).
#define SETTINGWIN_WIDTH 500 //!< Default setting window width (if unable to load it from the configuration file).
#define SETTINGWIN_HEIGHT 400 //!< Default setting window height (if unable to load it from the configuration file).
#define SETTINGWIN_NAME "Settings" //!< Setting window name.
#define STREAMSETTINGS_LABEL "Stream settings" //!< Stream setting tab label
#define PLOTSETTINGS_LABEL "Plot settings" //!< Plot setting tab label
#define HOSTCTRLWIN_NAME "Host control" //!< Host-control window name
#if defined(OS_WIN)			
#define TOOLBAR_XML "sbbhost-toolbar.xml" //!< Toolbar XML file name.
#else
#define TOOLBAR_XML TOOLBAR_XML_LINUX //!< Toolbar XML file name.
#endif
#define SAVE_FILE_LABEL "Please select a *.sbb/*.dat/*.mat file to save to" //!<  File>SaveAs label
#define OPEN_FILE_LABEL "Please select a *.sbb file to open" //!< File>Open label
#define SETFOLDER_LABEL "Please select a folder to use as default" //!< File>SetFolder label
#define FILE_START_FLAG "file://" //!< Header when drag & drop a file into the application.
#define FILE_END_FLAG "\r\n" //!< Terminator when drag & drop a file into the application.
//#define FILE_NAME_HEADER "log_"
#define FILE_EXT ".sbb" //!< Logged-data file extension (SBB stands for self-balancing-bike).
#define FILE_CSV ".dat" //!< File extension of DAT files.
#define FILE_MAT ".mat" //!< File extension of MAT files.
#define FILE_EXT_LABEL "sbb files" //!< Logged-data file label.
#define FILE_CSV_LABEL "dat files" //!< File label for DAT files.
#define FILE_MAT_LABEL "mat files" //!< File label for MAT files.
#ifdef OS_WIN 
#define FILE_SEP '\\' //!< File separator (for Windows).
#else
#define FILE_SEP '/' //!< File separator (for Linux).
#endif
#define STREAMTREE_FRAMELABEL "Signals" //!< Frame label of the stream tree.
#define STREAMTIME_FORMAT "%02d:%02d:%02d" //!< String format of stream time label.
#define STREAMTIME_UPDATE_MS 1000 //!< Update rate of stream time label.
#define UPDATE_FPS 8 //!< Default frame rate of the stream graph (if unable to load it from the configuration file).
#define LABEL_SCALE 0.66 //!< Default graph label scale (if unable to load it from the configuration file).
#define TICK_SCALE 0.66 //!< Default tick scale (if unable to load it from the configuration file).
#define DARKMODE 0 //!< Default darkmode enable (if unable to load it from the configuration file).
#define WINDOW_SAMPLES 1000 //!< Default samples to show in the stream graph (if unable to load it from the configuration file).
#define FORWARD_SAMPES 200 //!< Default foward samples to show in the stream graph (if unable to load it from the configuration file).
#define LINE_WIDTH 0.50 //!< Default line width of graph(if unable to load it from the configuration file).
#define GRAPH_OFFSETX 0.05 //!< Default graph offset x (if unable to load it from the configuration file).
#define GRAPH_OFFSETY 0.05 //!< Default graph offset y (if unable to load it from the configuration file).
#define GRAPH_WIDTH 0.925 //!< Default graph width (if unable to load it from the configuration file).
#define GRAPH_HEIGHT 0.875 //!< Default graph height  (if unable to load it from the configuration file).
#define STREAM_GRAPH_DECIM 5 //!< Default stram graph decimation (if unable to load it from the configuration file).
#define PLOT_GRAPH_DECIM 2 //!< Default plot graph decimation (if unable to load it from the configuration file).
#define SETTING_LABEL_WIDTH 150  //!< Width for labels in the setting window.
#define SETTING_ENTRY_WIDTH 150 //!< Width for entries in the setting window.
#define STREAMTREE_MINWIDTH 75 //!< Minimum width of the stream tree.
#define PLOTTREE_MINWIDTH 75 //!< Minimum width of the plot tree.
#define STREAMGRAPH_MINWIDTH 50 //!< Minimum width f the stream graph.
#define PLOTGRAPH_MINWIDTH 50 //!< Minimum width f the plot graph.
#define STREAMPANED_POS_DEF 200 //!< Default position of the stream paned.
#define PLOTPANED_POS_DEF 300 //!< Default position of the plot paned.
#define INVALID_CHARS '/', '\\', '<', '>', '"', '|', '?', '*', '[', ']', ',', ';', '.', ':' //!< List of invalid char for file names.
#define SAMPLE_PLOTID 0 //!< Plot ID of sample-domain plot.
#define FFT_PLOTID 1 //!< Plot ID of the spectra plot.
#define PSD_PLOTID 2 //!< Plot ID of power-spectra-density plot.
#define FRF_PLOTID 3 //!< Plot ID of frequency-response-function plot.					
#define DOC_FILE "start ..\\..\\docs\\html\\index.html" //!< Documentation main file

//exit flags
#define EXIT_DEFAULT		0x00 //!< Default exit flag (no issues).
#define EXIT_LOSTCONNECTION	0x01 //!< Lost-connection exit flag.
#define EXIT_MISSINGPACKET	0x02 //!< Missing-packet exit flag.
#define EXIT_NOSERIALPORT	0x03 //!< No-serial-port-found exit flag.
#define EXIT_MULTIPLEPORT	0x04 //!< Multiple-serial-ports-found exit flag.
#define EXIT_UNABLECONNECT	0x05 //!< Unable-to-connect exit flag.
#define EXIT_CONNECTIONOK	0x06 //!< Connection-ok exit flag.
#define EXIT_NOLOGDATA		0x07 //!< No-logged-data exit flag.
#define EXIT_SAVEUNKNOWISS  0x08 //!< Unknown-issue-during-save exit flag.
#define EXIT_OPENUNKNOWISS  0x09 //!< Unknown-issue-during-open exit flag.
#define EXIT_ALLSAVED		0x0A //!< All-logged-data-saved exit flag.
#define EXIT_UNEXPECTED		0x0B //!< Unexpected-error exit flag.
#define EXIT_INVALIDHEADER  0x0C //!< Invalid-header exit flag.
#define EXIT_INVALIDTERMIN	0x0D //!< Invalid-terminator exit flag.
#define EXIT_MAXLOGGEDDATA	0x0E //!< Max-logged-data exit flag.
#define EXIT_INVALIFILEEXT	0x0F //!< Invalid-file-extension exit flag.
#define EXIT_INVALIDFILE	0x10 //!< Invalid-file exit flag.
#define EXIT_UNSELECTEDLOG	0x11 //!< Unselected-logged-data exit flag.
#define EXIT_TOOSELECTEDLOG 0x12 //!< Too-selected-logged-data exit flag.

//Beep message
#ifdef OS_WIN
#define WARNING_BEEP MB_ICONWARNING //!< Warning beep in Windows.
#define INFO_BEEP MB_ICONINFORMATION //!< Info beep in Windows.
#define ERROR_BEEP MB_ICONERROR //!< Error beep in Windows.
#else //beep message not used in Linux
#define WARNING_BEEP 0 //!< Not used in Linux.
#define INFO_BEEP 0 //!< Not used in Linux.
#define ERROR_BEEP 0 //!< Not used in Linux.
#endif

//Local env var
#define GSETTINGS_SCHEMA_DIR_KEY "GSETTINGS_SCHEMA_DIR" //!< Key of the GSettings environment variable.
#ifdef OS_WIN
#define GSETTINGS_SCHEMA_DIR_VAL "C:\\src\\vcpkg\\installed\\x64-windows\\share\\glib-2.0\\schemas" //!< Default GSettings path (if unable to load it from the configuration file).
#else
#define GSETTINGS_SCHEMA_DIR_VAL "./" //!< Default GSettings path (not used in Linux).
#endif
#endif