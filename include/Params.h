#ifndef PARAMS_H
#define PARAMS_H

#include <string>
#include <vector>


/*! \brief A class for the application setting parameters.
    \details Class containing the parameters for the SBB Host Application. 
    Most of the parameters are from the configuration file of by default from config.h. 
    This is only a container, no function members exists.
	\see SBBHostApp SBBHostCore SettingsDialog
    \author Stefano Lovato
    \date 2022
*/
class Params {
public:

    /*! \brief Constructor.
        \details Default constructor.
		\param def_port The default value for port.
	*/
    Params(int def_port);

    /*! \brief Destructor.
		\details Default destructor.
	*/
    ~Params();

	static constexpr int SERIAL = 0; //!< USB/Serial connection constant.
	static constexpr int TCP = 1; //!< TCP/IP connection constant.
	static constexpr int UDP = 2; //!< UDP/Broadcast connection constant.

    //functional vars
	int conn_type = SERIAL; //!< Connection type. Default is SERIAL.
    int port; //!< Current serial port used. Default is NULL_PORT.
	int baud; //!< Current baudrate used.
	int socket_port; //!< Socket port. Default is 9876.
	std::string socket_ip; //!< Socket IP address. Default is "192.168.1.0"
	unsigned int header; //!< Header used for the host communication.
	unsigned int terminator; //!< Terminator used for the host communication.
	int timeout; //!< Timeout used for the host communication.
	int max_missing_packets; //!< Max missing packets during signal streaming.
	int window_samples; //!< Samples shown in the stream graph.
	int forward_samples; //!< Foward samples shown in the stream graph.
	int stream_graph_decim; //!< Stream graph decimation.
	int plot_graph_decim; //!< Plot graph decimation.
	int fps; //!< Frame rate of the stream graph.
	int num_of_signals; //!< Number of signals
	int num_of_controls; //!< Number of controls
	std::vector<std::string> signals; //!< Name of signals
	std::vector<std::string> controls; //!< Name of controls
	std::vector<double> control_min; //!< Minimum values of controls
	std::vector<double> control_max; //!< Maximum values of controls
	std::vector<double> control_val; //!< Default values of controls

	//appearance vars
	std::string gsettings_dir; //!< GSetting directory.
	int use_darkmode; //1< Flag to use the darkmode or not.
	int mainWin_width; //!< Main window width.
	int mainWin_height; //!< Main window height.
	int messageDialog_width; //!< Message dialog width..
	int messageDialog_height; //!< Message dialog height
	int settingsDialog_width; //!< Setting dialog width.
	int settingsDialog_height; //!< Setting dialog height.
	double label_scale; //!< Graph label scale.
	double tick_scale; //!< Graph tick scale.
	double line_width; //!< Graph line width.
	double graph_width; //!< Graph width.
	double graph_height; //!< Graph height.
	double graph_offsetx; //!< Graph x offset.
	double graph_offsety; //!< Graph y offset.
};

#endif