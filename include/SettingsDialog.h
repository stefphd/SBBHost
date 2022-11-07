
#ifndef SETTINGWIN_H
#define SETTINGWIN_H

#include <gtkmm.h>
#include "Serial.h"
#include "Params.h"
#include "EntryLabel.h"
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

/*! \brief A class for the setting dialog.
    \details Class for setting dialog by extending the Gtk::Dialog class, used in SBBHostApp.
	\see SBBHostApp
    \author Stefano Lovato
    \date 2022
*/
class SettingsDialog : public Gtk::Dialog {
public:
	/*! \brief Constructor.
		\details Default constructor. 
	*/
	SettingsDialog();

	/*! \brief Show the dialog.
		\details Function to show the dialog. This overrides, and uses, that in Gtk::Dialog.
		\param p_params The pointer to the parameter struct to use for the values in the dialog window.
		\param page The tab page to be shown when showing the dialog. See STREAM_PAGE and PLOT_PAGE in config.h.
	*/
	void show(Params* p_params, int page);

	/*! \brief Set the parameters.
		\details Function to set the parameters with those of the dialog window.
		\param p_params The pointer to the parameter struct where save the values in the dialog window.
		\return An exit flag (see config.h for details).
	*/
	int set_params(Params* p_params);

	/*! \brief Get the selected ip.
		\details Function to get the selected ip.
		\return The IP address.
	*/
	std::string get_selected_ip();

protected:
	//objects
	Gtk::Notebook noteBook; //!< Gtk::Notebook object for the stream, plot setting tabs.
	Gtk::Grid streamGrid; //!< Gtk::Grid object for the stream setting page.
	Gtk::Grid  plotGrid; //!< Gtk::Grid object for the plot setting page.
	ComboChoiceEntryLabel conntype; //!< ComboChoiceEntryLabel object for the choiche of the connection type.
	ComboEntryLabel port; //!< ComboEntryLabel object for the choice of the serial port.
	SpinEntryLabel baud; //!< SpinEntryLabel object for the baudrate.
	SpinEntryLabel socket_port; //!< SpinEntryLabel object for the socket port.
	TextEntryLabel socket_ip; //!< TextEntryLabel object for the socket ip.
	SpinEntryLabel timeout; //!< SpinEntryLabel object for the timeout.
	TextEntryLabel header; //!< TextEntryLabel object for the header.
	TextEntryLabel terminator; //!< TextEntryLabel object for the terminator.
	SpinEntryLabel maxmissings; //!< SpinEntryLabel object for the timeout.
	SpinEntryLabel winsamples; //!< SpinEntryLabel object for the shown samples in the stream-graph.
	SpinEntryLabel prevsamples; //!< SpinEntryLabel object for the foward samples in the stream-graph.
	SpinEntryLabel fps; //!< SpinEntryLabel object for the stream-graph frame rate.
	SpinEntryLabel stream_decim; //!< SpinEntryLabel object for the stream-graph decimation.
	//Gtk::Separator hsep0, hsep1, hsep2, hsep3, hsep4;
	//Gtk::Separator vsep0;

	//pointers to Gtk objects
	Gtk::Button* p_buttonOK; //!< Pointer to the OK Gtk::Button
	Gtk::Button* p_buttonCancel; //!< Pointer to the Cancel Gtk::Button
	Gtk::Button* p_buttonApply; //!< Pointer to the Apply Gtk::Button

private:
	//void on_ok_button_pressed();
	//void on_apply_button_pressed();
	//void on_cancel_button_pressed();
	void on_conn_type_changed();
};

#endif