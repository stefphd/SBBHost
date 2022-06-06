
#ifndef HOSTCONTROL_H
#define HOSTCONTROL_H

#include <gtkmm.h>
#include <vector>
#include "config.h"

/*! \brief A class for slider with Gtk::Label, Gtk::SpinButton and Gtk::ToggleButton
    \details Class for a slider with a Gtk::Label, Gtk::ToggleButton to enable the slider, 
	and two Gtk::SpinButton for the minimum and maximum values, used in HostControl.
	\see HostControl
    \author Stefano Lovato
    \date 2022
*/
class Slider : public Gtk::Frame {
public:
	/*! \brief Constructor.
		\details Default constructor.
		\param str The label.
		\param min The minimum value of the slider.
		\param max The maximum value of the slider.
		\param val The default value of the slider.
	*/
	Slider(Glib::ustring str, double min, double max, double val);

	/*! \brief Enable/disable the slider. 
		\details Function to enable or disable the slider. 
		\param is_active The flag to enable or disable the slider. 
	*/
	void set_active(bool is_active = true);

	/*! \brief Get the slider active.
		\details Function to get the slider state (active or not).
		\return true if the slider is active, false otherwise.
	*/
	bool get_active();

	/*! \brief Get the slider value.
		\details Function to get the slider value.
		\return The slider value.
	*/
	double get_value();

protected:
	Gtk::Scale scrollbar; //!< Gtk::Scale object for the scrollbar.
	Gtk::ToggleButton button; //!< Gtk::Button object for the enable button.
	Gtk::Box vbox; //!< Gtk::Box object for a vertical box.
	Gtk::Box hminbox; //!< Gtk::Box object for the horizontal box of the minimum value.
	Gtk::Box hmaxbox; //!< Gtk::Box object for the horizontal box of the maximum value.
	Gtk::SpinButton minEntry; //!< Gtk::SpinButton object for the minimum value entry.
	Gtk::SpinButton maxEntry; //!< Gtk::SpinButton object for the maximum value entry.
	Gtk::Label minLabel; //!< Gtk::Label object for the minimum value label.
	Gtk::Label maxLabel; //!< Gtk::Label object for the maximum value label.
	Glib::RefPtr<Gtk::Adjustment> p_adj; //!< Gtk::Adjustment pointer for the scrollbar
	Glib::RefPtr<Gtk::Adjustment> p_minadj; //!< Gtk::Adjustment pointer for the minimum value entry
	Glib::RefPtr<Gtk::Adjustment> p_maxadj; //!< Gtk::Adjustment pointer for the minimum value entry

private:
	/*! \brief Callback of minimum value entry.
		\details Callback function of the minimum-value entry changed to update the scrollbar minimum value.
	*/
	void on_minadj_changed();

	/*! \brief Callback of maximum value entry.
		\details Callback function of the maximum-value entry changed to update the scrollbar maximum value.
	*/
	void on_maxadj_changed();

	/*! \brief Callback of enable button.
		\details Callback function of the enable button toggled to enable or disable the scrollbar and the 
		minimum and maximum value entries.
	*/
	void on_enable_button_toggled();
};


/*! \brief A class for host control widget.
    \details Class for host control widget extending the Gtk::Frame class, used in SBBHostApp.
	\see SBBHostApp
    \author Stefano Lovato
    \date 2022
*/
class HostControl : public Gtk::Frame {
public:
	/*! \brief Constructor.
		\details Default constructor.
	*/
	HostControl();

	/*! \brief Append a slider.
		\details Function to append a slider. Does not update the widget.
		\param lbl The label of the slider.
		\param min The minimum value of the slider.
		\param max The maximum value of the slider.
		\param val The default value of the slider.
	*/
	void append_slider(Glib::ustring lbl, double min = -1, double max = +1, double val = 0);

	/*! \brief Update the widget appearance.
		\details Function to update the widget appearance after having added sliders.
	*/
	void update();

	/*! \brief Get the values of the sliders.
		\details Function to get the values of the sliders. Values of sliders not active are set to NaN, see NAN_VAL.
		\param buf Pointer to buffer of floats where store the values.
		\param len Size of the buffer.
		\return true if one or more values has been set.
	*/
	bool get_values(float* buf, size_t len);

protected:
	Gtk::Grid grid; //!< Gtk::Grid object for the sliders.
	//Gtk::ToggleButton  enableButton; //!< Gtk::ToggleButton for the enable-all button.
	std::vector<Slider*> sliders; //!< Vector of pointers to Slider object.

private:

	/* \brief Callback for enable-all button.
		\details Callback function called when the enable-all button is toggled.
	*/
	//void on_enable_button_pressed();
	unsigned int nanVal = NAN_VAL; //!< NaN variable, see NAN_VAL.
};



#endif