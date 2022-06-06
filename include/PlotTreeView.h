
#ifndef PLOTTREEVIEW_H
#define PLOTTREEVIEW_H

#include <gtkmm.h>
#include <numeric>
#include <vector>
#include <iomanip>
#include <iostream>
#include "TreeColumns.h"
#include "plotter.hpp"
#include "config.h"
#include "SBBHostCore.h"
#include "SBBUtils.h"
#include "spectra.h"
//#include "FiltFilt.h"

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

#ifdef OS_LINUX
#define printf_s printf
#endif

/*! \brief A class for the plot tree view.
    \details Class for the tree view in the plot tab, extending the Gtk::TreeView class 
	and allowing selection of log-signal combinations to be shown in the graph.
	Class used in SBBHostApp.
	\see SBBHostApp
    \author Stefano Lovato
    \date 2022
*/
class PlotTreeView : public Gtk::TreeView {
public:
	/*! \brief Constructor.
		\details Default constructor. 
		\param win The pointer to the main window of SBBHostApp.
		\param core The pointer to the SBBHostCore object.
		\see SBBHostApp SBBHostCore
	*/
	PlotTreeView(Gtk::ApplicationWindow* win, SBBHostCore *core);

	/*! \brief Destructor.
		\details Default destructor.
	*/
	virtual ~PlotTreeView();

	//functions
	/*! \brief Set the sample-domain graph pointer.
		\details Function to set the sample-domain graph pointer, in order to 
		have full access to the graph.
		\param graph The pointer to the CGraph::CairoGraph graph object.
	*/
	void set_sampleGraph(CGraph::CairoGraph* graph);

	/*! \brief Set the spectra graph pointer.
		\details Function to set the spectra graph pointer, in order to 
		have full access to the graph.
		\param mag The pointer to the CGraph::CairoGraph graph of the magnitude.
		\param ph The pointer to the CGraph::CairoGraph graph of the phase.
	*/
	void set_fftGraph(CGraph::CairoGraph* mag, CGraph::CairoGraph* ph);

	//add more set_graph here, like set_psdGraph, set_frfGraph

	/*! \brief Append rows.
		\details Function to append the row for the last log, as well as the child rows for 
		all the necessary signals. It automatically find the last log from the SBBHostCore object.
		\see SBBHostCore
	*/
	void append_row();

	/*! \brief Create the column(s).
		\details Function to create the column(s) in the tree view.
	*/
	void create_column();

	/*! \brief Update the log name shown.
		\details Function to update the log name in the tree view row.
		\param id The ID of the log.
		\param name The new name.
	*/
	void update_logname(int id, std::string name);
	
	/*! \brief Get the selected log(s) or signal(s).
		\details Function to get the selected log(s) or signal(s) in the tree view.
		\return A std::vector containing the selected log(s) or signal(s), represented by a Gtk::TreeModel::iterator.
	*/
	std::vector<Gtk::TreeModel::iterator> get_selected(int type);
	
	TreeColumns* p_treeColumns = nullptr; //!< TreeColumns pointer for the format of the tree view column(s).

protected:
	Gtk::ApplicationWindow* p_win = nullptr; //!< Gtk::ApplicationWindow pointer to the application main window.
	CGraph::CairoGraph* p_samplegraph = nullptr; //!< CGraph::CairoGraph pointer to the sample-domain graph.
	CGraph::CairoGraph* p_magfftgraph = nullptr; //!< CGraph::CairoGraph pointer to the magnitude-spectra graph.
	CGraph::CairoGraph* p_phfftgraph = nullptr; //!< CGraph::CairoGraph pointer to the phase-spectra graph.
	Glib::RefPtr<Gtk::TreeStore> p_TreeModel = nullptr; //!< Gtk::TreeStore pointer for the tree view model.
	Glib::RefPtr<Gtk::TreeSelection> p_TreeSelection = nullptr; //!< Gtk::TreeSelection pointer for the tree view selection.
	SBBHostCore* p_core = nullptr; //!< SBBHostCore pointer to the SBB Host core object.
	Glib::RefPtr<Gtk::Builder> p_builder; //!< Gtk::Builder pointer to read the popover menu from the XML file.
	Glib::RefPtr<Gio::SimpleActionGroup> p_signalActionGroup; //!< Gio::SimpleActionGroup pointer for simple actions in the signal popover menu.
	Glib::RefPtr<Gio::SimpleActionGroup> p_logActionGroup; //!< Gio::SimpleActionGroup pointer for simple actions in the log popover menu.
	Glib::RefPtr<Gio::SimpleAction> p_lineTypeAction; //!< Gio::SimpleActionGroup pointer for line-type action in the signal popover menu.
	std::unique_ptr<Gtk::PopoverMenu> p_signal_popOverMenu = nullptr; //!< Gtk::PopoverMenu pointer for the signal popover menu.
	std::unique_ptr<Gtk::PopoverMenu> p_log_popOverMenu = nullptr; //!< Gtk::PopoverMenu pointer for the log popover menu.
	std::unique_ptr<Gtk::ColorChooserDialog> p_colorChooser; //!< Gtk::ColorChooserDialog pointer for the color chooser dialog.
	Gtk::TreeView::Column name_col; //!< Gtk::TreeView::Column object for the editable name column.
	Gtk::CellRendererText name_render; //!< Gtk::CellRendererText object for the editable name column.

private:
	/*! \brief Check a new name.
		\details Function to check a new user-defined name for a log. It avoids invalid char (see INVALID_CHARS in config.h).
		\param name The name as a string.
		\return true if the name is ok, false otherwise.
	*/
	bool check_new_name(std::string name);

	/*! \brief Callback for the tree view selected.
		\details Callback function called when selecting a signal or log in the trew view.
	*/
	void on_plot_tree_selected();

	/*! \brief Callback for popover item pressed.
		\details Callback function called when pressing a button of the popover menu.
		\see Glib::SignalProxy<void(int, double, double)> Gtk::GestureClick::signal_pressed()
		\param n_press How many touch/button presses happened with this one.
		\param x The X coordinate, in widget allocation coordinates.
		\param y The Y coordinate, in widget allocation coordinates. 
	*/
	void on_popover_button_pressed(int n_press, double x, double y);

	/*! \brief Callback for delete logs.
		\details Callback function called when selecting the 'Delete' button in the log popover menu.
	*/
	void on_menu_delete();

	/*! \brief Callback for save logs.
		\details Callback function called when selecting the 'Save' button in the log popover menu.
	*/
	void on_menu_save();

	//void on_menu_saveas(); //not used

	/*! \brief Callback for the set color.
		\details Callback function called when selecting the 'Set color' button in the signal popover menu.
	*/
	void on_menu_set_color();

	/*! \brief Callback for the set linetype.
		\details Callback function called when selecting the 'Set linetype' button in the signal popover menu.
	*/
	void on_menu_set_linetype(int type);

	/*! \brief Callback for the color chooser response.
		\details Callback function called when exiting from the color chooser dialog.
		\see Gtk::ColorChooserDialog
		\param id_response The response ID.
	*/
	void on_color_chooser_response(int id_response);

	/*! \brief Callback for setting the name cell.
		\details Callback function called to set the value in the name cell according to its renderer.
		\param renderer The Gtk::CellRenderer pointer to the renderer object.
		\param iter The Gtk::TreeModel::const_iterator.
	*/
	void name_col_on_cell_data(Gtk::CellRenderer* renderer, const Gtk::TreeModel::const_iterator& iter);

	/*! \brief Callback for name column edited.
		\details Callback function called when the editing of the name cell finisched, in order to set the name of the cell. 
		Only for log-type cells (name is editable only from those cells).
		\param path_string The path identifying the edited cell.
		\param new_text The new name used for the edited cell.
	*/
  	void name_render_on_edited(const Glib::ustring& path_string, const Glib::ustring& new_text);

	/*! \brief Callback for name column start editing.
		\details Callback function called when starting to edit a name column cell. It avoids editing in the case of signal-type cells.
		\param cell_editable The pointer to the Gtk::CellEditable object.
		\param path_string The path identifying the edited cell.
	*/
	void name_render_on_editing_started(Gtk::CellEditable* cell_editable, const Glib::ustring& path_string);

};

#endif