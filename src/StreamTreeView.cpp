#include "StreamTreeView.h"

StreamTreeView::StreamTreeView(Gtk::ApplicationWindow* win) : Gtk::TreeView() {
	//assign objects to pointers
	p_win = win;
	p_treeColumns = new TreeColumns();
	p_TreeModel = Gtk::ListStore::create(*p_treeColumns);
	p_TreeSelection = get_selection();
	set_model(p_TreeModel);

	//create click gesture
	auto refGesture = Gtk::GestureClick::create();
	refGesture->set_button(GDK_BUTTON_SECONDARY);
	refGesture->signal_pressed().connect(sigc::mem_fun(*this, &StreamTreeView::on_popover_button_pressed));
	add_controller(refGesture);

	//popover menu & submenu
	p_builder = Gtk::Builder::create();
	try {
		p_builder->add_from_file(TOOLBAR_XML);
	}
	catch (const Glib::Error& ex) {
		printf_s("Building menus failed: %s\n", ex.what());
	}
	auto gmenu = p_builder->get_object<Gio::Menu>("signaltreeopoover");
	p_popOverMenu = std::make_unique<Gtk::PopoverMenu>();
	p_popOverMenu->set_parent(*this);
	p_popOverMenu->set_menu_model(gmenu);
	p_popOverMenu->set_has_arrow(false);
	
	//create actions
	p_actionGroup = Gio::SimpleActionGroup::create();
	p_actionGroup->add_action("set_color",
		sigc::mem_fun(*this, &StreamTreeView::on_menu_set_color));
	p_lineTypeAction = p_actionGroup->add_action_radio_integer("set_linetype",
		sigc::mem_fun(*this, &StreamTreeView::on_menu_set_linetype), (int) CairoGraphLineStyle::SOLID_LINE);
	insert_action_group("signalpopover", p_actionGroup);

	//other set
	set_activate_on_single_click(true);
	p_TreeSelection->set_mode(Gtk::SelectionMode::MULTIPLE);
	p_TreeSelection->signal_changed().connect(sigc::mem_fun(*this, &StreamTreeView::on_stream_tree_selected));
}

void StreamTreeView::set_graph(CGraph::CairoGraph* graph) {
	p_graph = graph;
}

void StreamTreeView::append_row(int id, Glib::ustring name, Gdk::RGBA rgba, CairoGraphLineStyle linetype) {
	Gtk::TreeRow row = *(p_TreeModel->append());
	int linetype_int = (int) linetype;
	row[p_treeColumns->col_signal_id] = id;
	row[p_treeColumns->col_name] = name;
	row[p_treeColumns->col_type] = ColumnType::SIGNAL;
	row[p_treeColumns->col_color] = rgba;
	row[p_treeColumns->col_linetype] = linetype_int;
}

void StreamTreeView::create_column() {
	//append column
	append_column("Signals", p_treeColumns->col_name);
}

std::vector<int> StreamTreeView::get_col_id_selected() {
	std::vector<Gtk::TreeModel::Path> sels = p_TreeSelection->get_selected_rows();
	std::vector<int> ids(sels.size());
	for (int i = 0; i < sels.size(); i++) {
		Gtk::TreeModel::iterator iter = p_TreeModel->get_iter(sels[i]);
		ids[i] = iter->get_value(p_treeColumns->col_signal_id);
	}
	return ids;
}


std::vector<Gdk::RGBA> StreamTreeView::get_col_rgba_selected() {
	std::vector<Gtk::TreeModel::iterator> iters = get_selected();
	std::vector<Gdk::RGBA> rgba(iters.size());
	for (int i = 0; i < iters.size(); i++) rgba[i] = iters[i]->get_value(p_treeColumns->col_color);
	return rgba;
}


std::vector<CairoGraphLineStyle> StreamTreeView::get_col_linetype_selected() {
	std::vector<Gtk::TreeModel::iterator> iters = get_selected();
	std::vector<CairoGraphLineStyle> linetype(iters.size());
	for (int i = 0; i < iters.size(); i++) linetype[i] = (CairoGraphLineStyle)iters[i]->get_value(p_treeColumns->col_linetype);
	return linetype;
}

void StreamTreeView::set_popover_sensitive(bool sensitive) {
	p_popOverMenu->set_sensitive(sensitive);
}

void StreamTreeView::on_popover_button_pressed(int /* n_press */, double x, double y) {
	std::vector<Gtk::TreeModel::iterator> iters = get_selected();
	for (int i = 0; i < iters.size(); ++i) p_lineTypeAction->change_state((int)iters[i]->get_value(p_treeColumns->col_linetype));
	const Gdk::Rectangle rect(x, y, 20, 20);
	p_popOverMenu->set_pointing_to(rect);
	p_popOverMenu->popup();
	
}

std::vector<Gtk::TreeModel::iterator> StreamTreeView::get_selected() {
	std::vector<Gtk::TreeModel::Path> sels = p_TreeSelection->get_selected_rows();
	std::vector<Gtk::TreeModel::iterator> iters(sels.size());
	for (int i = 0; i < sels.size(); ++i) iters[i] = p_TreeModel->get_iter(sels[i]);
	return iters;
}

void StreamTreeView::on_menu_set_color() {
	p_colorChooser.reset(new Gtk::ColorChooserDialog("Please choose a color", *p_win));
	p_colorChooser->set_modal(true);
	p_colorChooser->set_hide_on_close(true);
	p_colorChooser->signal_response().connect(
		sigc::mem_fun(*this, &StreamTreeView::on_color_chooser_response));
	std::vector<Gdk::RGBA> color = get_col_rgba_selected();
	p_colorChooser->set_rgba(color[0]);
	p_colorChooser->show();
}

void StreamTreeView::on_color_chooser_response(int id) {
	p_colorChooser->hide();
	if (id == Gtk::ResponseType::OK) {
		std::vector<Gtk::TreeModel::iterator> iters = get_selected();
		for (int i = 0; i < iters.size(); i++) {
			iters[i]->set_value(p_treeColumns->col_color, p_colorChooser->get_rgba());
			p_graph->set_line_colour(iters[i]->get_value(p_treeColumns->col_signal_id), iters[i]->get_value(p_treeColumns->col_color));
		}
		p_graph->update_graph();
	}
}

void StreamTreeView::on_menu_set_linetype(int type) {
	std::vector<Gtk::TreeModel::iterator> iters = get_selected();
	for (int i = 0; i < iters.size(); i++) {
		iters[i]->set_value(p_treeColumns->col_linetype, type);
		p_graph->set_line_style(iters[i]->get_value(p_treeColumns->col_signal_id), (CairoGraphLineStyle)type);
	}
	p_graph->update_graph();
}

void StreamTreeView::on_stream_tree_selected() {
	//if (!p_buttonRun->get_active()) { return; }
	for (int i = 0; i < p_graph->get_num_series(); i++) p_graph->clear_series(i);
	p_graph->update_ylim(-1, +1);
}

StreamTreeView::~StreamTreeView() {
}