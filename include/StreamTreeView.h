
#ifndef STREAMTREEVIEW_H
#define STREAMTREEVIEW_H

#include <gtkmm.h>
#include "TreeColumns.h"
#include "plotter.hpp"
#include "config.h"

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

/*! \brief A class for the stream tree view.
    \details Class for the tree view in the stream tab, extending the Gtk::TreeView class 
	and allowing selection of the signal to be shown in the graph during signal streaming.
	Class used in SBBHostApp.
	\see SBBHostApp
    \author Stefano Lovato
    \date 2022
*/
class StreamTreeView : public Gtk::TreeView {
public:
	/*! \brief Constructor.
		\details Default constructor. 
		\param win The pointer to the main window of SBBHostApp.
		\see SBBHostApp
	*/
	StreamTreeView(Gtk::ApplicationWindow* win);

	/*! \brief Destructor.
		\details Default destructor.
	*/
	virtual ~StreamTreeView();

	//functions
	/*! \brief Set the graph pointer.
		\details Function to set the stream graph pointer, in order to 
		have full access to the graph.
		\param graph The pointer to the CGraph::CairoGraph graph object.
	*/
	void set_graph(CGraph::CairoGraph* graph);

	/*! \brief Append a row.
		\details Function to append a signal row to the tree view.
		\param id The ID of the signal.
		\param name The name of the signal.
		\param rgba The Gdk::RGBA line color to assign to the graph. Default RGB is (0.5,0.5,0.5).
		\param linetype The CairoGraphLineStyle line type to assign to the graph. Default is solid line.
	*/
	void append_row(int id, Glib::ustring name,
		Gdk::RGBA rgba = Gdk::RGBA(0.5, 0.5, 0.5), CairoGraphLineStyle linetype = CairoGraphLineStyle::SOLID_LINE);
	
	/*! \brief Create the column(s).
		\details Function to create the column(s) in the tree view.
	*/
	void create_column();

	/*! \brief Set the popover menu sensitive.
		\details Function to set the popover menu of the tree view sensitive or not.
		\param sensitive True or false if to set sensitive or nor.
	*/
	void set_popover_sensitive(bool sensitive = true);

	/*! \brief Get the ID of the selected signal(s).
		\details Function to get the ID of the selected signal(s) in the tree view.
		\return A std::vector<int> containing the selected signal ID(s).
	*/
	std::vector<int> get_col_id_selected();

protected:
	//pointers to class objects
	Gtk::ApplicationWindow* p_win = nullptr; //!< Gtk::ApplicationWindow pointer to the application main window.
	CGraph::CairoGraph* p_graph = nullptr; //!< CGraph::CairoGraph pointer to the stream graph.
	TreeColumns* p_treeColumns = nullptr; //!< TreeColumns pointer for the format of the tree view column(s).
	Glib::RefPtr<Gtk::ListStore> p_TreeModel = nullptr; //!< Gtk::ListStore pointer for the tree view model.
	Glib::RefPtr<Gtk::TreeSelection> p_TreeSelection = nullptr; //!< Gtk::TreeSelection pointer for the tree view selection.
	std::unique_ptr<Gtk::PopoverMenu> p_popOverMenu = nullptr; //!< Gtk::PopoverMenu pointer for the popover menu in the tree view.
	Glib::RefPtr<Gio::SimpleActionGroup> p_actionGroup; //!< Gio::SimpleActionGroup pointer for simple actions in the popover menu.
	Glib::RefPtr<Gio::SimpleAction> p_lineTypeAction; //!< Gio::SimpleActionGroup pointer for line-type action in the popover menu.
	std::unique_ptr<Gtk::ColorChooserDialog> p_colorChooser; //!< Gtk::ColorChooserDialog pointer for the color chooser dialog.
	Glib::RefPtr<Gtk::Builder> p_builder; //!< Gtk::Builder pointer to read the popover menu from the XML file.

private:
	/*! \brief Callback for popover item pressed.
		\details Callback function called when pressing a button of the popover menu.
		\see Glib::SignalProxy<void(int, double, double)> Gtk::GestureClick::signal_pressed()
		\param n_press How many touch/button presses happened with this one.
		\param x The X coordinate, in widget allocation coordinates.
		\param y The Y coordinate, in widget allocation coordinates. 
	*/
	void on_popover_button_pressed(int n_press, double x, double y);

	/*! \brief Callback for the set color.
		\details Callback function called when selecting the 'Set color' button in the popover menu.
	*/
	void on_menu_set_color();
	
	/*! \brief Callback for the set linetype.
		\details Callback function called when selecting the 'Set linetype' button in the popover menu.
	*/
	void on_menu_set_linetype(int type);
	
	/*! \brief Callback for the color chooser response.
		\details Callback function called when exiting from the color chooser dialog.
		\see Gtk::ColorChooserDialog
		\param id The response ID.
	*/
	void on_color_chooser_response(int id);
	
	/*! \brief Callback for the tree view selected.
		\details Callback function called when selecting a signal in the trew view.
	*/
	void on_stream_tree_selected();
	
	/*! \brief Get the selected signals(s).
		\details Function to get the selected signal(s) in the tree view.
		\return A std::vector containing the selected signal(s), represented by a Gtk::TreeModel::iterator.
	*/
	std::vector<Gtk::TreeModel::iterator> get_selected();
	
	/*! \brief Get the RGB color of the selected signal(s).
		\details Function to get the Gdk::RGBA color object of the selected signal(s) in the tree view.
		\return A std::vector<Gdk::RGBA> containing the selected signal color(s).
	*/
	std::vector<Gdk::RGBA> get_col_rgba_selected();
	
	/*! \brief Get the linetype of the selected signal(s).
		\details Function to get the CairoGraphLineStyle linetype object of the selected signal(s) in the tree view.
		\return A std::vector<CairoGraphLineStyle> containing the selected signal linetype(s).
	*/
	std::vector<CairoGraphLineStyle> get_col_linetype_selected();

};

#endif
