#ifndef TREECOLUMNS_H
#define TREECOLUMNS_H

#include <gtkmm.h>

/*! \brief A namespace providing constants for TreeColumns.
    \details Namespace providing constants to use with the TreeColumns class, used 
	in StreamTreeView and PlotTreeView. 
	\see StreamTreeView PlotTreeView TreeColumns
    \author Stefano Lovato
    \date 2022
*/
namespace ColumnType {
	constexpr int SIGNAL = 0; //!< Identifier for signal-type column.
	constexpr int LOG = 1; //!< Identifier for log-type column.
}

/*! \brief A class for column definition in Gtk::TreeView.
    \details Class for defining the column format in Gtk::TreeView objects, used 
	in StreamTreeView and PlotTreeView. 
	\see StreamTreeView PlotTreeView
    \author Stefano Lovato
    \date 2022
*/
class TreeColumns : public Gtk::TreeModel::ColumnRecord {
public:
	/*! \brief Constructor.
		\details Default constractor.
	*/
	TreeColumns() {
		add(col_signal_id), add(col_log_id), add(col_name), add(col_type), add(col_color), add(col_linetype), add(col_average), add(col_rms);
	};

	Gtk::TreeModelColumn<int> col_signal_id;  //!< Signal-ID column (usefull only for signal columns).
	Gtk::TreeModelColumn<int> col_log_id; //!< Log-ID column (in signal columns the corresponding log-id is used).
	Gtk::TreeModelColumn<int> col_type; //!< Column type (see ColumnType::SIGNAL and ColumnType::LOG in the ColumnType namespace).
	Gtk::TreeModelColumn<int> col_linetype; //!< Line type column (usefull only for signal columns).
	Gtk::TreeModelColumn<Glib::ustring> col_name; //!< Name column (signal or log name depending on the column type).
	Gtk::TreeModelColumn<Gdk::RGBA> col_color; //!< Line color column (usefull only for signal columns).
	Gtk::TreeModelColumn<Glib::ustring> col_average; //!< Average column (usefull only for signal columns).
	Gtk::TreeModelColumn<Glib::ustring> col_rms; //!< RMS column (usefull only for signal columns).
};

#endif
