#include "PlotTreeView.h"

PlotTreeView::PlotTreeView(Gtk::ApplicationWindow* win, SBBHostCore* core) {
	p_win = win;
	p_core = core;
	p_treeColumns = new TreeColumns();
	p_TreeModel = Gtk::TreeStore::create(*p_treeColumns);
	p_TreeSelection = get_selection();
	set_model(p_TreeModel);

	//create click gesture
	auto refGesture = Gtk::GestureClick::create();
	refGesture->set_button(GDK_BUTTON_SECONDARY);
	refGesture->signal_pressed().connect(sigc::mem_fun(*this, &PlotTreeView::on_popover_button_pressed));
	add_controller(refGesture);

	//signal popover menu & submenu
	p_builder = Gtk::Builder::create();
	try {
		p_builder->add_from_file(TOOLBAR_XML);
	}
	catch (const Glib::Error& ex) {
		printf_s("Building menus failed: %s\n", ex.what());
	}
	auto gmenu_signal = p_builder->get_object<Gio::Menu>("signaltreeopoover");
	p_signal_popOverMenu = std::make_unique<Gtk::PopoverMenu>();
	p_signal_popOverMenu->set_parent(*this);
	p_signal_popOverMenu->set_menu_model(gmenu_signal);
	p_signal_popOverMenu->set_has_arrow(false);

	//log popover menu
	auto gmenu_log = p_builder->get_object<Gio::Menu>("logtreeopoover");
	p_log_popOverMenu = std::make_unique<Gtk::PopoverMenu>();
	p_log_popOverMenu->set_parent(*this);
	p_log_popOverMenu->set_menu_model(gmenu_log);
	p_log_popOverMenu->set_has_arrow(false);

	//create signal actions
	p_signalActionGroup = Gio::SimpleActionGroup::create();
	p_signalActionGroup->add_action("set_color",
		sigc::mem_fun(*this, &PlotTreeView::on_menu_set_color));
	p_lineTypeAction = p_signalActionGroup->add_action_radio_integer("set_linetype",
		sigc::mem_fun(*this, &PlotTreeView::on_menu_set_linetype), (int)CairoGraphLineStyle::SOLID_LINE);
		
	insert_action_group("signalpopover", p_signalActionGroup);

	//create log action
	p_logActionGroup = Gio::SimpleActionGroup::create();
	p_logActionGroup->add_action("delete",
		sigc::mem_fun(*this, &PlotTreeView::on_menu_delete));
	p_logActionGroup->add_action("save",
		sigc::mem_fun(*this, &PlotTreeView::on_menu_save));
	/*p_logActionGroup->add_action("saveas",
		sigc::mem_fun(*this, &PlotTreeView::on_menu_saveas));*/
	insert_action_group("logpopover", p_logActionGroup);

	//other set
	set_activate_on_single_click(true);
	p_TreeSelection->set_mode(Gtk::SelectionMode::MULTIPLE);
	p_TreeSelection->signal_changed().connect(sigc::mem_fun(*this, &PlotTreeView::on_plot_tree_selected));
}

void PlotTreeView::set_sampleGraph(CGraph::CairoGraph* graph) {
	p_samplegraph = graph;
}

void PlotTreeView::set_fftGraph(CGraph::CairoGraph* mag, CGraph::CairoGraph* ph) {
	p_magfftgraph = mag;
	p_phfftgraph = ph;
}

void PlotTreeView::on_menu_delete() {
	std::vector<Gtk::TreeModel::iterator> iters = get_selected(ColumnType::LOG);
	for (int i = 0; i < iters.size(); ++i) {
		if (p_core->delete_log(iters[i]->get_value(p_treeColumns->col_log_id))) p_TreeModel->erase(iters[i]);
	}
}

void PlotTreeView::on_menu_save() {
	std::vector<Gtk::TreeModel::iterator> iters = get_selected(ColumnType::LOG);
	int exit = EXIT_DEFAULT;
	for (int i = 0; i < iters.size(); ++i) {
		int id = iters[i]->get_value(p_treeColumns->col_log_id);
		p_core->save(id);
	}	
}

/*
void PlotTreeView::on_menu_saveas() {
	printf("SaveAs\n");
}*/

void PlotTreeView::update_logname(int id, std::string name) {
	for (int i = 0; i < p_TreeModel->children().size(); ++i) {
		Gtk::TreeRow row = p_TreeModel->children()[i];
		if (row[p_treeColumns->col_type] == ColumnType::LOG) {
			if (row[p_treeColumns->col_log_id] == id) {
				row[p_treeColumns->col_name] = name;
			}
		}
	}

}

void PlotTreeView::append_row() {
	//append row
	Gtk::TreeRow row = *(p_TreeModel->append());
	row[p_treeColumns->col_log_id] = p_core->logData.back()->id;
	row[p_treeColumns->col_name] = p_core->logData.back()->name;
	row[p_treeColumns->col_type] = ColumnType::LOG;
	row[p_treeColumns->col_average] = ""; //empty field
	row[p_treeColumns->col_rms] = ""; //empty field
	
	//append row for each signal
	for (int i = 0; i < p_core->p_params->num_of_signals; i++) {
		Gtk::TreeRow row_child = *(p_TreeModel->append(row.children()));
		row_child[p_treeColumns->col_signal_id] = i;
		row_child[p_treeColumns->col_log_id] = p_core->logData.back()->id;
		row_child[p_treeColumns->col_name] = p_core->p_params->signals[i];
		row_child[p_treeColumns->col_type] = ColumnType::SIGNAL;
		int j = i % Colors::size;
		row_child[p_treeColumns->col_color] = Gdk::RGBA(Colors::red[j], Colors::green[j], Colors::blue[j]); //null
		int linetype_int = (int)CairoGraphLineStyle::SOLID_LINE;
		row_child[p_treeColumns->col_linetype] = linetype_int; //null
		row_child[p_treeColumns->col_average] = Glib::ustring::format(std::fixed, std::setprecision(3), utils::average(p_core->logData.back()->data[i]));
		row_child[p_treeColumns->col_rms] = Glib::ustring::format(std::fixed, std::setprecision(3), utils::rms(p_core->logData.back()->data[i]));
	}
}

void PlotTreeView::create_column() {
	//set name column
	name_col.set_title("Name");
	name_col.pack_start(name_render);
	name_render.property_editable() = true;
	name_col.set_cell_data_func(name_render,
          sigc::mem_fun(*this, &PlotTreeView::name_col_on_cell_data) );
	name_render.signal_editing_started().connect(
          sigc::mem_fun(*this, &PlotTreeView::name_render_on_editing_started) ); //slip editing for signal cells
 	name_render.signal_edited().connect( sigc::mem_fun(*this, &PlotTreeView::name_render_on_edited) ); //check name used

	//append columns
	//append_column("Name", p_treeColumns->col_name); //name_col used istead to make the column editble only for log cell
	append_column(name_col);
	append_column("Mean", p_treeColumns->col_average);
	append_column_editable("RMS", p_treeColumns->col_rms);
}

void PlotTreeView::on_plot_tree_selected() {
	std::vector<Gtk::TreeModel::iterator> iters = get_selected(ColumnType::SIGNAL);
	if (iters.size() == 0) { return; };
	if (p_samplegraph != nullptr) p_samplegraph->init_plots(iters.size());
	if (p_magfftgraph != nullptr) p_magfftgraph->init_plots(iters.size());
	if (p_phfftgraph != nullptr) p_phfftgraph->init_plots(iters.size());

	for (int i = 0; i < iters.size(); ++i) {
		if (iters[i]->get_value(p_treeColumns->col_type) != ColumnType::SIGNAL) { continue; } //check if signal type
		int log_id = p_core->find(iters[i]->get_value(p_treeColumns->col_log_id));
		int sig_id = iters[i]->get_value(p_treeColumns->col_signal_id);

		//sample plot
		if (p_samplegraph != nullptr) {
			for (int j = 0; j < p_core->logData[log_id]->num_samples; j += p_core->p_params->plot_graph_decim) p_samplegraph->add_point(i, p_core->logData[log_id]->samples[j], p_core->logData[log_id]->get(sig_id, j), true, true);
			p_samplegraph->set_line_colour(i, iters[i]->get_value(p_treeColumns->col_color));
			p_samplegraph->set_line_style(i, (CairoGraphLineStyle)iters[i]->get_value(p_treeColumns->col_linetype));
		}

		//fft plots
		if (p_magfftgraph != nullptr && p_phfftgraph != nullptr) {
			std::vector<float> float_vec = p_core->logData[log_id]->get(sig_id);
			std::vector<double> in(float_vec.begin(), float_vec.end());
			std::vector<double> freq = spectra::frequency(in.size());
			std::vector<double> mag, phase;
			spectra::polard(spectra::fft(in), &mag, &phase);
			for (int j = 0; j < freq.size(); ++j) {
				p_magfftgraph->add_point(i, freq[j], mag[j], true, true);
				p_phfftgraph->add_point(i, freq[j], phase[j], true, true);
			}
			p_magfftgraph->set_line_colour(i, iters[i]->get_value(p_treeColumns->col_color));
			p_magfftgraph->set_line_style(i, (CairoGraphLineStyle)iters[i]->get_value(p_treeColumns->col_linetype));
			p_phfftgraph->set_line_colour(i, iters[i]->get_value(p_treeColumns->col_color));
			p_phfftgraph->set_line_style(i, (CairoGraphLineStyle)iters[i]->get_value(p_treeColumns->col_linetype));
		}

		//Try to show filtfilt
		/*
		std::vector<float> float_vec = p_core->logData[log_id]->get(sig_id);
		FiltFilt::vectord in_sig(float_vec.begin(), float_vec.end());
		FiltFilt::vectord out_sig;
		FiltFilt::vectord coefs_B = {0.2, 0.8};
		FiltFilt::vectord coefs_A = {0};
		FiltFilt::filtfilt(coefs_B, coefs_A, in_sig, out_sig);
		printf("samples 1: %d\n", p_core->logData[log_id]->num_samples);
		printf("samples 2: %d\n", out_sig.size());
		for (int j = 0; j < p_core->logData[log_id]->num_samples; j += p_core->p_params->plot_graph_decim) p_graph->add_point(i+iters.size(), p_core->logData[log_id]->samples[j], out_sig[j], true, true);
		p_graph->set_line_colour(i+iters.size(), Gdk::RGBA(1,1,1));
		p_graph->set_line_style(i+iters.size(), (CairoGraphLineStyle)iters[i]->get_value(p_treeColumns->col_linetype));*/
	}	
	if (p_samplegraph != nullptr) p_samplegraph->update_graph();
	if (p_magfftgraph != nullptr) p_magfftgraph->update_graph();
	if (p_phfftgraph != nullptr) p_phfftgraph->update_graph();
}

void PlotTreeView::on_popover_button_pressed(int /* n_press */, double x, double y)
{
	if (p_TreeSelection->count_selected_rows()==0) { return; } //no slected rows
	std::vector<Gtk::TreeModel::Path> sels = p_TreeSelection->get_selected_rows(); //selected rows (given as TreeModel::Path)
	Gtk::TreeModel::iterator iter = p_TreeModel->get_iter(sels[0]); //take the first only
	
	//two cases depending on the selected type
	const Gdk::Rectangle rect(x, y, 20, 20);
	switch (iter->get_value(p_treeColumns->col_type)) {
	case ColumnType::LOG:
		p_log_popOverMenu->set_pointing_to(rect);
		p_log_popOverMenu->popup();
		break;
	case ColumnType::SIGNAL:
	{
		std::vector<Gtk::TreeModel::iterator> iters = get_selected(ColumnType::SIGNAL);
		for (int i = 0; i < iters.size(); ++i) p_lineTypeAction->change_state((int)iters[i]->get_value(p_treeColumns->col_linetype));
		p_signal_popOverMenu->set_pointing_to(rect);
		p_signal_popOverMenu->popup();
		break;
	}
	default:
		return;
	}
}

std::vector<Gtk::TreeModel::iterator> PlotTreeView::get_selected(int type) {
	std::vector<Gtk::TreeModel::iterator> iters;
	std::vector<Gtk::TreeModel::Path> sels = p_TreeSelection->get_selected_rows();
	for (int i = 0; i < sels.size(); ++i) {
		Gtk::TreeModel::iterator iter = p_TreeModel->get_iter(sels[i]);
		if (iter->get_value(p_treeColumns->col_type) == type) iters.push_back(iter);
	}
	return iters;
}


void PlotTreeView::on_menu_set_color() {
	std::vector<Gtk::TreeModel::iterator> iters = get_selected(ColumnType::SIGNAL);
	if (iters.size() == 0) { return; };
	p_colorChooser.reset(new Gtk::ColorChooserDialog("Please choose a color", *p_win));
	p_colorChooser->set_modal(true);
	p_colorChooser->set_hide_on_close(true);
	p_colorChooser->signal_response().connect(
		sigc::mem_fun(*this, &PlotTreeView::on_color_chooser_response));
	p_colorChooser->set_rgba(iters[0]->get_value(p_treeColumns->col_color));
	p_colorChooser->show();
}

void PlotTreeView::on_color_chooser_response(int id) {
	p_colorChooser->hide();
	if (id == Gtk::ResponseType::OK) {
		std::vector<Gtk::TreeModel::iterator> iters = get_selected(ColumnType::SIGNAL);
		if (iters.size() == 0) { return; };
		for (int i = 0; i < iters.size(); ++i) iters[i]->set_value(p_treeColumns->col_color, p_colorChooser->get_rgba());
		on_plot_tree_selected();
	}
}

void PlotTreeView::on_menu_set_linetype(int type) {
	std::vector<Gtk::TreeModel::iterator> iters = get_selected(ColumnType::SIGNAL);
	if (iters.size() == 0) { return; };
	for (int i = 0; i < iters.size(); ++i) iters[i]->set_value(p_treeColumns->col_linetype, type);
	on_plot_tree_selected();
}



void PlotTreeView::name_col_on_cell_data(Gtk::CellRenderer* /* renderer */, const Gtk::TreeModel::const_iterator& iter) {
  //Get the value from the model and show it appropriately in the view:
  if(iter) {
    std::string str = iter->get_value(p_treeColumns->col_name);
	name_render.property_text() = str;
  }
}

void PlotTreeView::name_render_on_editing_started(Gtk::CellEditable* cell_editable, const Glib::ustring& path_string) {
	Gtk::TreePath path(path_string);
	Gtk::TreeModel::iterator iter = p_TreeModel->get_iter(path);
	if(iter) {
		if (iter->get_value(p_treeColumns->col_type) == ColumnType::SIGNAL) cell_editable->editing_done(); //finish editing immediately, do not edit signal cells!
	}
}

void PlotTreeView::name_render_on_edited(const Glib::ustring& path_string, const Glib::ustring& new_text){
	Gtk::TreePath path(path_string);
	Gtk::TreeModel::iterator iter = p_TreeModel->get_iter(path);
	if(iter && check_new_name(new_text)) {
		//if (iter->get_value(p_treeColumns->col_type) == ColumnType::LOG) //not necessary b/c name_render_on_editing_started skip signal cell
		iter->set_value(p_treeColumns->col_name, new_text);
		p_core->logData[iter->get_value(p_treeColumns->col_log_id)]->name = new_text;
	}
}

bool PlotTreeView::check_new_name(std::string name) {
	std::vector<char> invalid_chars = { INVALID_CHARS };
	for (int i = 0; i < name.size(); ++i) {
		for (int j = 0; j < invalid_chars.size(); ++j) {
			if (name.at(i) == invalid_chars[j]) { return false; };
		}
	}
	return true;
}

PlotTreeView::~PlotTreeView() { }
