
#ifndef ENTRYLABEL_H
#define ENTRYLABEL_H

#include <gtkmm.h>
#include <string>
#include "config.h"

/*! \brief A class with a Gtk::SpinButton and Gtk::Label.
    \details Class including a Gtk::SpinButton together with a Gtk::Label, used in SettingsDialog. 
	\see SettingsDialog
    \author Stefano Lovato
    \date 2022
*/
class SpinEntryLabel : public Gtk::Box {
public:
	/*! \brief Constructor.
		\details Default constructor.
		\param val The default value of the spin button.
		\param lower The lower bound of the spin button.
		\param upper The upper bound of the spin button.
		\param step The step of the spin button.
		\param lbl The label.
	*/
	SpinEntryLabel(double val, double lower, double upper, double step, Glib::ustring lbl);

	/*! \brief Set the value.
		\details Function to set the value of the spin button.
		\param val The value of the spin button.
	*/
	void set_value(double val);

	/*! \brief Get the value.
		\details Function to get the value of the spin button.
		\return The value of the spin button.
	*/
	double get_value();

protected:
	Gtk::Box box; //!< Gtk::Box object of the widget.
	Gtk::Label label; //!< label string variable.
	Gtk::SpinButton entry; //!< Gtk::SpinButton object.
};


/*! \brief A class with a Gtk::Entry and Gtk::Label.
    \details Class including a Gtk::Entry together with a Gtk::Label, used in SettingsDialog. 
	\see SettingsDialog
    \author Stefano Lovato
    \date 2022
*/
class TextEntryLabel : public Gtk::Box {
public:
	/*! \brief Constructor.
		\details Default constructor.
		\param str The default value of the text entry.
		\param lbl The label.
	*/
	TextEntryLabel(Glib::ustring str, Glib::ustring lbl);

	/*! \brief Set the text.
		\details Function to set the text of the entry.
		\param str The text of the entry.
	*/
	void set_text(Glib::ustring str);

	/*! \brief Get the text.
		\details Function to get the text of the entry.
		\return The text of the entry.
	*/
	Glib::ustring get_text();

protected:
	Gtk::Box box; //!< Gtk::Box object of the widget.
	Gtk::Label label; //!< label string variable.
	Gtk::Entry entry; //!< Gtk::Entry object.
};


/*! \brief A class with a Gtk::ComboBoxText and Gtk::Label.
    \details Class including a Gtk::ComboBoxText together with a Gtk::Label, used in SettingsDialog. 
	\see SettingsDialog
    \author Stefano Lovato
    \date 2022
*/
class ComboEntryLabel : public Gtk::Box {
public:
	/*! \brief Constructor.
		\details Default constructor.
		\param vals The values of the ComboBoxText.
		\param lbl The label.
	*/
	ComboEntryLabel(std::vector<int> vals, Glib::ustring lbl);

	/*! \brief Set the values.
		\details Function to set the values of the ComboBoxText.
		\param vals The values of the ComboBoxText.
	*/
	void set_entries(std::vector<int> vals);

	/*! \brief Get the values.
		\details Function to get the values of the ComboBoxText.
		\return The values of the ComboBoxText.
	*/
	std::vector<int> get_entries();

	/*! \brief Set the selected value.
		\details Function to set the selected value of the ComboBoxText.
		\param val The value to set.
		\return true if success, false othereise.
	*/
	bool set_selected(int val);

	/*! \brief Get the selected value.
		\details Function to get the selected value of the ComboBoxText.
		\return The selected value of the ComboBoxText.
	*/
	int get_selected();
protected:
	Gtk::Box box; //!< Gtk::Box object of the widget.
	Gtk::Label label; //!< label string variable.
	Gtk::ComboBoxText entry; //!< Gtk::ComboBoxText object.
	std::vector<int> entries; //!< Vector to store the values of the ComboBoxText.
};

/*! \brief A class with a Gtk::ComboBoxText and Gtk::Label. Slightly different from ComboEntryLabel.
    \details Class including a Gtk::ComboBoxText together with a Gtk::Label, used in SBBHostApp.
	It slightly differs from ComboEntryLabel for the appearance, usage, and implementation.
	\see SBBHostApp
    \author Stefano Lovato
    \date 2022
*/
class ComboChoiceEntryLabel : public Gtk::Box {
public:
	/*! \brief Constructor.
		\details Default constructor. Sets only the label, not the choices of the ComboBoxText.
		\param lbl The label.
	*/
	ComboChoiceEntryLabel(Glib::ustring lbl);

	/*! \brief Set the choices.
		\details Function to set the choices of the ComboBoxText.
		\param id_vec The vector of IDs of the choices.
		\param str_vec The vector of names of the choices.
	*/
	void set_entries(std::vector<int> id_vec, std::vector<std::string> str_vec);

	/*! \brief Set the selected choice.
		\details Function to set the selected choice of the ComboBoxText by specifying its ID.
		\param id The ID of the selected choice.
		\return true if success, false otherwise.
	*/
	bool set_selected(int id);

	/*! \brief Get the ID of the selected choice.
		\details Function to get the ID of the selected choice of the ComboBoxText.
		\return The ID of the selected choice.
	*/
	int get_selected();

	/*! \brief Get the Gtk::ComboBoxText pointer.
		\details Function to get a pointer to the Gtk::ComboBoxText object.
		\return The pointer to the Gtk::ComboBoxText.
	*/
	Gtk::ComboBoxText* get_comboBoxText();
protected:
	Gtk::Box box; //!< Gtk::Box object of the widget.
	Gtk::Label label; //!< label string variable.
	Gtk::ComboBoxText entry; //!< Gtk::ComboBoxText object.
	std::vector<int> id; //!< Vector of ID for the item of the ComboBoxText.
	std::vector<std::string> str; //!< Vector to store the choice name of the ComboBoxText.
};

#endif

