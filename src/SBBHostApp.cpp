#include "SBBHostApp.h"
#include "HostPort.h"

SBBHostApp::SBBHostApp() : Gtk::Application(),
mainWin(), //noteBook(),
streamPaned(Gtk::Orientation::HORIZONTAL),
streamBox0(Gtk::Orientation::HORIZONTAL),
streamBox1(Gtk::Orientation::VERTICAL),
streamBox2(Gtk::Orientation::HORIZONTAL),
plotPaned(Gtk::Orientation::HORIZONTAL),
plotBox1(Gtk::Orientation::VERTICAL),
fftBox(Gtk::Orientation::HORIZONTAL),
streamTreeFrame(), plotTreeFrame(), streamTimeLabel(), plotGraphOverlay(), 
streamGraphFrame("Stream graph"),
buttonRun(RUN_BUTLABEL, true), buttonLog(LOG_BUTLABEL, true),
aboutDialog(), settingsDialog(), hostControl(),
darkModeSwitch(),
scrolledWinStream(), scrolledWinPlot(),
streamTreeView(&mainWin), plotTreeView(&mainWin, &core),
timer(),
plotTypeComboEntry("Plot type"),
params(NULL_PORT)
{

	//set app name
	//Glib::ustring app_name = APP_NAME;
	//app_name += " - v";
	//app_name += VERSION;
	//Glib::set_application_name(app_name);

	//creare startuo notification - unsupported in Windows :(
	#ifdef OS_LINUX
	register_application();
	//Glib::RefPtr<Gio::Notification> startup_note = Gio::Notification::create("SBB Host");
	//startup_note->set_body("Welcome to SBB Host App.");
	//send_notification("SBB Host", startup_note);
	#endif

	//create params
	core.set_params(&params);

	//parse config file
	parse_ini();

	//set env - ncessary only in Windows
	#ifdef OS_WIN 
	utils::set_env_var(GSETTINGS_SCHEMA_DIR_KEY, params.gsettings_dir);
	#endif
}

SBBHostApp::~SBBHostApp() {
	core.~SBBHostCore();
}


Glib::RefPtr<SBBHostApp> SBBHostApp::create() {
	return Glib::make_refptr_for_instance<SBBHostApp>(new SBBHostApp());
}

void SBBHostApp::on_startup() { //defined in the case of adding other stuff in on_startup
	//call the base class's implementation:
	Gtk::Application::on_startup();

	//get defualt directory
	def_dir = utils::get_home_path();
}


void SBBHostApp::on_activate() {

	//create builder
	p_builder = Gtk::Builder::create();

	//assign ptr to objects
	p_streamGraph = Gtk::make_managed<CGraph::CairoGraph>();
	p_plotGraph = Gtk::make_managed<CGraph::CairoGraph>();
	p_magfftGraph = Gtk::make_managed<CGraph::CairoGraph>();
	p_phfftGraph = Gtk::make_managed<CGraph::CairoGraph>();

	//create graph
	create_graph();
	
	//create toolbar
	create_toolbar();

	//init dialogs
	create_dialogs();

	//set callbacks
	buttonRun.signal_toggled().connect(sigc::mem_fun(*this, &SBBHostApp::on_buttonRun_toggled));
	buttonLog.signal_toggled().connect(sigc::mem_fun(*this, &SBBHostApp::on_buttonLog_toggled));
	mainWin.signal_close_request().connect(sigc::mem_fun<bool>(*this, &SBBHostApp::on_win_close), true);
	darkModeSwitch.property_active().signal_changed().connect(sigc::mem_fun(*this, &SBBHostApp::on_darkModeSwitch_switched));
	plotTypeComboEntry.get_comboBoxText()->signal_changed().connect(sigc::mem_fun(*this, &SBBHostApp::on_plotType_changed));

	//set window size (default)
	mainWin.set_default_size(params.mainWin_width, params.mainWin_height);

	//set child
	add_window(mainWin);
	mainWin.set_child(stack);
	mainWin.set_titlebar(headerbar);
	headerbar.set_show_title_buttons(true);
	Glib::ustring app_name = APP_NAME;
	app_name += " - v";
	app_name += VERSION;
	headerbarlabel.set_markup_with_mnemonic(gettext(app_name.c_str()));
	menubutton.set_icon_name("open-menu-symbolic");
	headerbar.set_title_widget(headerbarlabel);
	headerbar.pack_start(switcher);
	headerbar.pack_end(menubutton);
	headerbar.pack_end(darkModeSwitch);
	//noteBook.insert_page(streamBox0, STREAM_LABEL, STREAM_PAGE);
	//noteBook.insert_page(plotPaned, PLOT_LABEL, PLOT_PAGE);
	stack.add(streamBox0, gettext(STREAM_LABEL), STREAM_LABEL);
	stack.add(plotPaned, gettext(PLOT_LABEL), PLOT_LABEL);
	switcher.set_stack(stack);
	
	streamBox0.append(streamPaned);
	streamBox0.append(hostControl);
	//noteBook.set_action_widget(&darkModeSwitch, Gtk::PackType::END);
	streamPaned.set_start_child(streamBox1);
	streamPaned.set_end_child(streamGraphFrame);
	streamGraphFrame.set_child(p_streamGraph->get_graph_grid());
	streamBox1.append(streamBox2);
	streamBox2.append(buttonRun);
	streamBox2.append(buttonLog);
	streamBox1.append(streamTreeFrame);
	streamBox1.append(streamTimeLabel);
	fftBox.append(p_magfftGraph->get_graph_grid());
	fftBox.append(p_phfftGraph->get_graph_grid());
	plotGraphOverlay.set_child(p_plotGraph->get_graph_grid());
	plotGraphOverlay.add_overlay(fftBox);
	plotPaned.set_start_child(plotBox1);
	plotPaned.set_end_child(plotGraphOverlay);
	plotBox1.append(plotTypeComboEntry);
	plotBox1.append(plotTreeFrame);
	streamTreeFrame.set_child(scrolledWinStream);
	scrolledWinStream.set_child(streamTreeView);
	streamTreeView.set_graph(p_streamGraph);
	plotTreeFrame.set_child(scrolledWinPlot);
	scrolledWinPlot.set_child(plotTreeView);
	plotTreeView.set_sampleGraph(p_plotGraph);
	plotTreeView.set_fftGraph(p_magfftGraph,p_phfftGraph);

	//set host control
	set_host_control();

	//set drag and drop
	set_drag_and_drop();

	//set tooltips
	set_tooltips();

	//set tree
	set_tree();

	//set graph
	set_graph();

	//other set
	//plotTypeComboEntry.set_entries({SAMPLE_PLOTID,FFT_PLOTID, PSD_PLOTID, FRF_PLOTID}, {"Sample", "FFT", "PSD", "FRF"});
	plotTypeComboEntry.set_entries({SAMPLE_PLOTID,FFT_PLOTID}, {"Sample", "FFT"});
	plotTypeComboEntry.set_selected(SAMPLE_PLOTID);
	plotTypeComboEntry.set_margin(MARGIN);
	plotTypeComboEntry.set_hexpand(true);
	streamBox1.set_size_request(STREAMTREE_MINWIDTH);
	plotTreeFrame.set_size_request(PLOTTREE_MINWIDTH);
	p_streamGraph->set_size_request(STREAMGRAPH_MINWIDTH);
	p_plotGraph->set_size_request(PLOTGRAPH_MINWIDTH);
	streamPaned.set_position(STREAMPANED_POS_DEF);
	plotPaned.set_position(PLOTPANED_POS_DEF);
	streamPaned.set_shrink_start_child(false);
	streamPaned.set_shrink_end_child(false);
	plotPaned.set_shrink_start_child(false);
	plotPaned.set_shrink_end_child(false);
	buttonRun.set_margin(MARGIN);
	buttonLog.set_margin(MARGIN);
	streamTreeFrame.set_vexpand(true);
	streamTreeFrame.set_margin(MARGIN);
	plotTreeFrame.set_vexpand(true);
	plotTreeFrame.set_margin(MARGIN);
	streamTimeLabel.set_halign(Gtk::Align::START);
	streamTimeLabel.set_margin(MARGIN);
	darkModeSwitch.set_margin_end(MARGIN), darkModeSwitch.set_margin_start(MARGIN);
	darkModeSwitch.set_margin_top(MARGIN/2), darkModeSwitch.set_margin_bottom(MARGIN/2);
	buttonRun.set_icon_name(RUN_BUTICON);
	buttonRun.set_size_request(buttonLog.get_size(Gtk::Orientation::HORIZONTAL));
	darkModeSwitch.set_active(params.use_darkmode > 0);
	scrolledWinStream.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
	scrolledWinStream.set_vexpand();
	scrolledWinStream.set_size_request(100);
	scrolledWinPlot.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
	scrolledWinPlot.set_vexpand();
	scrolledWinPlot.set_size_request(100);
	set_streamTimer(0);
	timer.stop();
	const Glib::ustring& active_theme = Gtk::Settings::get_default()->property_gtk_theme_name().get_value();
	if (active_theme.find("dark") != Glib::ustring::npos || active_theme.find("Dark") != Glib::ustring::npos)
		darkModeSwitch.set_sensitive(false);
	else
	{
		darkModeSwitch.set_sensitive(true);
		darkModeSwitch.set_active(true);
	}

	//set icon - only in Linux
	#ifdef OS_LINUX
	//Gtk::IconTheme::get_for_display(mainWin.get_display())->add_resource_path("./");
	//mainWin.set_icon_name("sbbhost-icon.png");
	#endif

	//show window
	mainWin.set_show_menubar();
	mainWin.show();

}

void SBBHostApp::create_graph() {
	p_streamGraph->create_graph();
	p_plotGraph->create_graph();
	p_magfftGraph->create_graph();
	p_phfftGraph->create_graph();
}

void SBBHostApp::parse_ini() {
	p_cfg = new IniReader(CONFIG_FILE);
	if (!p_cfg->getInt("baud", &params.baud)) params.baud = BAUD;
	if (!p_cfg->getHex("header", &params.header)) params.header = HostPort::HEADER;
	if (!p_cfg->getHex("terminator", &params.terminator)) params.terminator = HostPort::TERMINATOR;
	if (!p_cfg->getInt("timeout", &params.timeout)) params.timeout = HostPort::TIMEOUT;
	if (!p_cfg->getInt("fps", &params.fps)) params.fps = UPDATE_FPS;
	if (!p_cfg->getInt("darkmode", &params.use_darkmode)) params.use_darkmode = DARKMODE;
	if (!p_cfg->getInt("max_missing_packets", &params.max_missing_packets)) params.max_missing_packets = MAX_MISSING_PACKETS;
	if (!p_cfg->getInt("window_width", &params.mainWin_width)) params.mainWin_width = MAINWIN_WIDTH;
	if (!p_cfg->getInt("window_height", &params.mainWin_height)) params.mainWin_height = MAINWIN_HEIGHT;
	if (!p_cfg->getInt("settings_width", &params.settingsDialog_width)) params.settingsDialog_width = SETTINGWIN_WIDTH;
	if (!p_cfg->getInt("settings_height", &params.settingsDialog_height)) params.settingsDialog_height = SETTINGWIN_HEIGHT;
	if (!p_cfg->getInt("dialog_width", &params.messageDialog_width)) params.messageDialog_width = MESSDIALOG_WIDTH;
	if (!p_cfg->getInt("dialog_height", &params.messageDialog_height)) params.messageDialog_height = MESSDIALOG_HEIGHT;
	if (!p_cfg->getInt("window_samples", &params.window_samples)) params.window_samples = WINDOW_SAMPLES;
	if (!p_cfg->getInt("forward_samples", &params.forward_samples)) params.forward_samples = FORWARD_SAMPES;
	if (!p_cfg->getInt("stream_graph_decim", &params.stream_graph_decim)) params.stream_graph_decim = STREAM_GRAPH_DECIM;
	if (!p_cfg->getInt("plot_graph_decim", &params.plot_graph_decim)) params.plot_graph_decim = PLOT_GRAPH_DECIM;
	if (!p_cfg->getStringArray("signals", &params.signals)) params.signals = { SIGNALS_NAMES };
	if (!p_cfg->getStringArray("controls", &params.controls)) params.controls = { CONTROL_NAMES };
	if (!p_cfg->getDoubleArray("control_min", &params.control_min)) params.control_min = { CONTROL_MIN };
	if (!p_cfg->getDoubleArray("control_max", &params.control_max)) params.control_max = { CONTROL_MAX };
	if (!p_cfg->getDoubleArray("control_val", &params.control_val)) params.control_val = { CONTROL_VAL };
	if (!p_cfg->getString("gsettings_dir", &params.gsettings_dir)) params.gsettings_dir = GSETTINGS_SCHEMA_DIR_VAL;
	if (!p_cfg->getDouble("label_scale", &params.label_scale)) params.label_scale = LABEL_SCALE;
	if (!p_cfg->getDouble("tick_scale", &params.tick_scale)) params.tick_scale = TICK_SCALE;
	if (!p_cfg->getDouble("line_width", &params.line_width)) params.line_width = LINE_WIDTH;
	if (!p_cfg->getDouble("graph_width", &params.graph_width)) params.graph_width = GRAPH_WIDTH;
	if (!p_cfg->getDouble("graph_height", &params.graph_height)) params.graph_height = GRAPH_HEIGHT;
	if (!p_cfg->getDouble("graph_offsetx", &params.graph_offsetx)) params.graph_offsetx = GRAPH_OFFSETX;
	if (!p_cfg->getDouble("graph_offsety", &params.graph_offsety)) params.graph_offsety = GRAPH_OFFSETY;
	p_cfg->~IniReader();

	//checks
	if (params.window_samples <= 0) params.window_samples = WINDOW_SAMPLES;
	if (params.forward_samples <= 0) params.forward_samples = FORWARD_SAMPES;
	if (params.label_scale <= 0) params.label_scale = LABEL_SCALE;
	if (params.tick_scale <= 0) params.tick_scale = TICK_SCALE;
	if (params.line_width <= 0) params.line_width = LINE_WIDTH;
	if (params.graph_width <= 0) params.graph_width = GRAPH_WIDTH;
	if (params.graph_height <= 0) params.graph_height = GRAPH_HEIGHT;
	if (params.graph_offsetx <= 0) params.graph_offsetx = GRAPH_OFFSETX;
	if (params.graph_offsety <= 0) params.graph_offsety = GRAPH_OFFSETY;
	params.num_of_signals = params.signals.size();
	params.num_of_controls = params.controls.size();
	if (params.control_min.size() != params.num_of_controls) params.control_min = { CONTROL_MIN };
	if (params.control_max.size() != params.num_of_controls) params.control_max = { CONTROL_MAX };
	if (params.control_val.size() != params.num_of_controls) params.control_val = { CONTROL_VAL };
	for (int i = 0; i < params.num_of_controls; ++i) {
		if (params.control_min[i] > params.control_max[i]) { params.control_max[i] = 1, params.control_min[i] = -1; }
		if (params.control_val[i] > params.control_max[i]) { params.control_val[i] = params.control_max[i]; }
		if (params.control_val[i] < params.control_min[i]) { params.control_val[i] = params.control_min[i]; }
	}
}

void SBBHostApp::set_drag_and_drop() {
	const GType ustring_type = Glib::Value<Glib::ustring>::value_type();
	Glib::RefPtr<Gtk::DropTarget> drag_target = Gtk::DropTarget::create(ustring_type, Gdk::DragAction::COPY);
	drag_target->signal_drop().connect(sigc::mem_fun(*this, &SBBHostApp::on_file_drag_and_drop), false);
	mainWin.add_controller(drag_target);
}

void SBBHostApp::set_tree() {
	//stream tree
	for (int i = 0; i < params.num_of_signals; i++) {
		int j = i % Colors::size;
		streamTreeView.append_row(i, params.signals[i],
			Gdk::RGBA(Colors::red[j], Colors::green[j], Colors::blue[j]), CairoGraphLineStyle::SOLID_LINE);
	}
	streamTreeView.create_column();
	
	//plot tree
	plotTreeView.create_column();
}

void SBBHostApp::set_graph() {
	//stream graph settings
	p_streamGraph->set_graph_size(params.graph_offsetx, params.graph_offsety, params.graph_width, params.graph_height);
	p_streamGraph->set_graph_box_style(CairoGraphBoxStyle::BOX_GRID);
	p_streamGraph->set_margin(MARGIN);
	p_streamGraph->set_label_scale(params.label_scale);
	p_streamGraph->set_tick_scale(params.tick_scale);
	p_streamGraph->set_axes_labels("Sample", "Signal(s)", "Sans");
	p_streamGraph->set_theme("Default");

	//p_streamGraph->use_scientific_notation(false, true);
	p_streamGraph->set_tick_label_format_x(false, 0);
	p_streamGraph->set_tick_label_format_y(false, 2);
	p_streamGraph->get_graph_grid().set_margin_top(MARGIN);
	p_streamGraph->get_graph_grid().set_margin_end(MARGIN);
	p_streamGraph->set_xwindow(int((params.window_samples - params.forward_samples) / params.stream_graph_decim) + 10);
	p_streamGraph->set_line_width(params.line_width);
	p_streamGraph->init_plots(params.num_of_signals);
	p_streamGraph->set_xlim(0, params.window_samples);
	p_streamGraph->update_ylim(-1,+1);
	for (int i = 0; i < params.num_of_signals; i++) {
		p_streamGraph->set_line_style(i, CairoGraphLineStyle::SOLID_LINE);
		int j = i % Colors::size;
		p_streamGraph->set_line_colour(i, Gdk::RGBA(Colors::red[j], Colors::green[j], Colors::blue[j]));
		p_streamGraph->set_line_style(i, CairoGraphLineStyle::SOLID_LINE);
	}
	p_streamGraph->update_graph();

	//plot graph settings
	p_plotGraph->set_graph_size(params.graph_offsetx, params.graph_offsety, params.graph_width, params.graph_height);
	p_plotGraph->set_graph_box_style(CairoGraphBoxStyle::BOX_GRID);
	p_plotGraph->set_margin(MARGIN);
	p_plotGraph->set_label_scale(params.label_scale);
	p_plotGraph->set_tick_scale(params.tick_scale);
	p_plotGraph->set_axes_labels("Sample", "Signal(s)", "Sans");
	p_plotGraph->set_theme("Default");
	p_plotGraph->set_tick_label_format_x(false, 0);
	p_plotGraph->set_tick_label_format_y(false, 2);
	p_plotGraph->get_graph_grid().set_margin_top(MARGIN);
	p_plotGraph->get_graph_grid().set_margin_end(MARGIN);
	p_plotGraph->set_xlim(0, params.window_samples);
	p_plotGraph->set_line_width(params.line_width);
	p_plotGraph->update_ylim(-1, +1);
	p_plotGraph->update_graph();

	//mag fft graph settings
	p_magfftGraph->set_graph_size(params.graph_offsetx, params.graph_offsety, params.graph_width, params.graph_height);
	p_magfftGraph->set_graph_box_style(CairoGraphBoxStyle::BOX_GRID);
	p_magfftGraph->set_margin(MARGIN);
	p_magfftGraph->set_label_scale(params.label_scale);
	p_magfftGraph->set_tick_scale(params.tick_scale);
	p_magfftGraph->set_axes_labels("Frequency (1/sample)", "Magnitude (dB)", "Sans");
	p_magfftGraph->set_theme("Default");
	p_magfftGraph->set_tick_label_format_x(false, 3);
	p_magfftGraph->set_tick_label_format_y(false, 3);
	p_magfftGraph->get_graph_grid().set_margin_top(MARGIN);
	p_magfftGraph->get_graph_grid().set_margin_end(MARGIN);
	p_magfftGraph->set_xlim(0, 1);
	p_magfftGraph->set_line_width(params.line_width);
	p_magfftGraph->update_ylim(0, +1);
	p_magfftGraph->update_graph();

	//mag fft graph settings
	p_phfftGraph->set_graph_size(params.graph_offsetx, params.graph_offsety, params.graph_width, params.graph_height);
	p_phfftGraph->set_graph_box_style(CairoGraphBoxStyle::BOX_GRID);
	p_phfftGraph->set_margin(MARGIN);
	p_phfftGraph->set_label_scale(params.label_scale);
	p_phfftGraph->set_tick_scale(params.tick_scale);
	p_phfftGraph->set_axes_labels("Frequency (1/sample)", "Phase (deg)", "Sans");
	p_phfftGraph->set_theme("Default");
	p_phfftGraph->set_tick_label_format_x(false, 3);
	p_phfftGraph->set_tick_label_format_y(false, 1);
	p_phfftGraph->get_graph_grid().set_margin_top(MARGIN);
	p_phfftGraph->get_graph_grid().set_margin_end(MARGIN);
	p_phfftGraph->set_xlim(0, 1);
	p_phfftGraph->set_line_width(params.line_width);
	p_phfftGraph->update_ylim(-180, +180);
	p_phfftGraph->update_graph();

}

void SBBHostApp::set_host_control(){
	for (int i = 0; i < params.controls.size(); ++i) hostControl.append_slider(params.controls[i], params.control_min[i], params.control_max[i], params.control_val[i]);
	hostControl.update();
}

void SBBHostApp::set_tooltips() {
   	menubutton.set_tooltip_text(_("Opens the menu."));
	buttonRun.set_tooltip_text("Run/stop signal streaming.");
	buttonLog.set_tooltip_text("Enable signal logging.");
	streamTimeLabel.set_tooltip_text("Elapsed time of current signal streaming.");
	darkModeSwitch.set_tooltip_text("Enable the darkmode.");
	//p_streamGraph->set_tooltip_text("Stream graph.");
	streamTreeView.set_tooltip_text("Select the signal to plot.");
	plotTreeView.set_tooltip_text("Select the logged signal to plot.");
	hostControl.set_tooltip_text("Bypass the signal in the microcontroller.");
}

void SBBHostApp::create_toolbar() {

	//create menu
	p_filemenu = Gio::Menu::create();
	p_filemenu->append("Save", "app.save");
	p_filemenu->append("SaveAs", "app.saveas");
	p_filemenu->append("Open", "app.open");
	p_filemenu->append("Quit", "app.quit");
	//----//
	p_editmenu = Gio::Menu::create();
	p_editmenu->append("Run", "app.menurun");
	p_editmenu->append("Log", "app.menulog");
	p_editmenu->append("Connect", "app.connect");
	p_editmenu->append("Settings", "app.settings");
	//----//
	p_helpmenu = Gio::Menu::create();
	p_helpmenu->append("About", "app.about");
	//p_helpmenu->append("Help", "app.help");

	p_winmenu = Gio::Menu::create();
	p_winmenu->append_section(p_filemenu);
	p_winmenu->append_section(p_editmenu);
	p_winmenu->append_section(p_helpmenu);
	menubutton.set_menu_model(p_winmenu);

	//set toolbar callback
	add_action("save", sigc::mem_fun(*this,		&SBBHostApp::on_menu_file_save));
	add_action("saveas", sigc::mem_fun(*this,	&SBBHostApp::on_menu_file_saveas));
	add_action("setfolder", sigc::mem_fun(*this,&SBBHostApp::on_menu_file_setfolder));
	add_action("open", sigc::mem_fun(*this,		&SBBHostApp::on_menu_file_open));
	add_action("quit", sigc::mem_fun(*this,		&SBBHostApp::on_menu_file_quit));
	//add_action("hostctrl", sigc::mem_fun(*this, &SBBHostApp::on_menu_hostcontrol));
	add_action("connect", sigc::mem_fun(*this,	&SBBHostApp::on_menu_stream_connect));
	add_action("settings", sigc::mem_fun(*this,&SBBHostApp::on_menu_settings));
	//add_action("plotopt", sigc::mem_fun(*this,	&SBBHostApp::on_menu_plot_opt));
	add_action("about", sigc::mem_fun(*this,	&SBBHostApp::on_menu_help_about));
	//add_action("help", sigc::mem_fun(*this, &SBBHostApp::on_menu_help_help));
	p_menuRun = add_action_bool("menurun", sigc::mem_fun(*this, &SBBHostApp::on_menu_stream_run), false);
	p_menuLog = add_action_bool("menulog", sigc::mem_fun(*this, &SBBHostApp::on_menu_stream_log), false);

	//add accelerators
	set_accel_for_action("app.save", "<Primary>s");
	set_accel_for_action("app.saveas", "<Primary>a");
	set_accel_for_action("app.setfolder", "<Primary>f");
	set_accel_for_action("app.open", "<Primary>o");
	set_accel_for_action("app.quit", "<Primary>q");
	set_accel_for_action("app.connect", "<Primary>c");
	//set_accel_for_action("app.hostctrl", "<Primary>t");
	set_accel_for_action("app.menurun", "<Primary>r");
	set_accel_for_action("app.menulog", "<Primary>l");
	set_accel_for_action("app.settings", "<Primary>d");
	set_accel_for_action("app.about", "<Primary>i");
	//set_accel_for_action("app.help", "<Primary>h");

	//toolbar builder
	/*
	try {
		p_builder->add_from_file(TOOLBAR_XML);
	}
	catch (const Glib::Error& ex) {
		printf_s("Building menus failed: %s\n", ex.what());
	}
	auto object = p_builder->get_object("mainToolbar");
	auto gmenu = std::dynamic_pointer_cast<Gio::Menu>(object);
	if (!gmenu) {
		g_warning("GMenu not found");
	}
	else {
		//set_menubar(gmenu);
		menubutton.set_menu_model(gmenu);
	}*/

}

void SBBHostApp::create_dialogs() {
	//file filter - save sdialog
	Glib::RefPtr<Gtk::FileFilter> filter_sbb = Gtk::FileFilter::create();
	filter_sbb->set_name(FILE_EXT_LABEL);
	filter_sbb->add_pattern("*" + (std::string) FILE_EXT);
	Glib::RefPtr<Gtk::FileFilter> filter_csv = Gtk::FileFilter::create();
	filter_csv->set_name(FILE_CSV_LABEL);
	filter_csv->add_pattern("*" + (std::string) FILE_CSV);
	Glib::RefPtr<Gtk::FileFilter> filter_mat = Gtk::FileFilter::create();
	filter_mat->set_name(FILE_MAT_LABEL);
	filter_mat->add_pattern("*" + (std::string) FILE_MAT);

	//file save dialog
	p_fileSaveAsDialog = new Gtk::FileChooserDialog(SAVE_FILE_LABEL, Gtk::FileChooser::Action::SAVE);
	p_fileSaveAsDialog->set_transient_for(mainWin);
	p_fileSaveAsDialog->set_modal(true);
	p_fileSaveAsDialog->set_hide_on_close();
	p_fileSaveAsDialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
	p_fileSaveAsDialog->add_button("_Save", Gtk::ResponseType::OK);
	p_fileSaveAsDialog->add_filter(filter_sbb);
	p_fileSaveAsDialog->add_filter(filter_csv);
	p_fileSaveAsDialog->add_filter(filter_mat);
	p_fileSaveAsDialog->signal_response().connect(sigc::mem_fun(*this, &SBBHostApp::on_file_saveAs_dialog));

	//file open dialog
	p_fileOpenDialog = new Gtk::FileChooserDialog(OPEN_FILE_LABEL, Gtk::FileChooser::Action::OPEN);
	p_fileOpenDialog->set_transient_for(mainWin);
	p_fileOpenDialog->set_modal(true);
	p_fileOpenDialog->set_hide_on_close();
	p_fileOpenDialog->set_select_multiple(false);
	p_fileOpenDialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
	p_fileOpenDialog->add_button("_Open", Gtk::ResponseType::OK);
	p_fileOpenDialog->add_filter(filter_sbb);
	p_fileOpenDialog->signal_response().connect(sigc::mem_fun(*this, &SBBHostApp::on_file_open_dialog));

	//set folder dialog
	p_setFolderDialog = new Gtk::FileChooserDialog(SETFOLDER_LABEL, Gtk::FileChooser::Action::SELECT_FOLDER);
	p_setFolderDialog->set_transient_for(mainWin);
	p_setFolderDialog->set_modal(true);
	p_setFolderDialog->set_hide_on_close();
	p_setFolderDialog->set_select_multiple(false);
	p_setFolderDialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
	p_setFolderDialog->add_button("Select", Gtk::ResponseType::OK);
	p_setFolderDialog->signal_response().connect(sigc::mem_fun(*this, &SBBHostApp::on_file_setfolder_dialog));

	//about dialogs
	aboutDialog.set_logo_default();
	aboutDialog.set_transient_for(mainWin);
	aboutDialog.set_modal();
	aboutDialog.set_hide_on_close();
	aboutDialog.set_program_name(APP_NAME);
	aboutDialog.set_version(VERSION);
	aboutDialog.set_copyright(AUTHOR_COPYRIGHT);
	aboutDialog.set_comments(APP_COMMENTS);
	aboutDialog.set_license(LICTYPE);
	aboutDialog.set_website(WEBSITE);
	aboutDialog.set_website_label(WEBSITE_LABEL);
	std::vector<Glib::ustring> list_authors = { LIST_AUTHOR };
	aboutDialog.set_authors(list_authors);

	//settings dialog
	settingsDialog.set_title(SETTINGWIN_NAME);
	settingsDialog.set_default_size(params.settingsDialog_width, params.settingsDialog_height);
	settingsDialog.set_resizable(false);
	settingsDialog.set_modal(true);
	settingsDialog.set_hide_on_close(true);
	settingsDialog.set_transient_for(mainWin);
	settingsDialog.signal_response().connect(sigc::mem_fun(*this, &SBBHostApp::on_setting_dialog));
	}

void SBBHostApp::clear_stream_graph() {
	for (int i = 0; i < params.num_of_signals; i++) p_streamGraph->clear_series(i);
	p_streamGraph->update_ylim();
	p_streamGraph->set_xwindow(int((params.window_samples - params.forward_samples) / params.stream_graph_decim) + 10);
	p_streamGraph->set_xlim(0, params.window_samples);
	p_streamGraph->update_graph();
}

void SBBHostApp::on_buttonRun_toggled() {
	if ((exitFlag == EXIT_NOSERIALPORT) | (exitFlag == EXIT_MULTIPLEPORT) | (exitFlag == EXIT_UNABLECONNECT)) {
		exitFlag = EXIT_DEFAULT;
		return;
	}

	//check but state
	if (buttonRun.get_active()) { //if is active start streaming
		exitFlag = core.connect(buttonLog.get_active(), def_dir);
		if ((exitFlag == EXIT_NOSERIALPORT) | (exitFlag == EXIT_MULTIPLEPORT) | (exitFlag == EXIT_UNABLECONNECT)) { //check serial
			warning(exitFlag);
			buttonRun.set_active(false);
			return;
		}
		clear_stream_graph();
		//noteBook.set_show_tabs(false);
		switcher.set_sensitive(false);

		buttonLog.set_sensitive(false);
		p_streamGraph->set_sensitive(false);
		streamTreeView.set_popover_sensitive(false);
		//p_buttonRun->set_label(STOP_BUTLABEL);
		buttonRun.set_icon_name(STOP_BUTICON);
		p_menuRun->change_state(true);
		//streamStartTime = utils::millis(), 
		timer.start();
		streamTimer = 0;
		streamGraphTimer = 0;
		Glib::signal_idle().connect(sigc::mem_fun(*this, &SBBHostApp::on_read_serial), Glib::PRIORITY_HIGH_IDLE+21); //connect isr to be done as quick as possible
	}
	else
	{ //if not active stop streaming
		core.disconnect(); //close host port
		core.reset_stream(); //reset stream
		if (buttonLog.get_active()) plotTreeView.append_row();
		//p_buttonRun->set_label(RUN_BUTLABEL);
		//noteBook.set_show_tabs(true);
		switcher.set_sensitive(true);
		buttonRun.set_icon_name(RUN_BUTICON);
		p_menuRun->change_state(false);
		buttonLog.set_sensitive(true);
		p_streamGraph->set_sensitive(true);
		streamTreeView.set_popover_sensitive(true);
		if (exitFlag == EXIT_LOSTCONNECTION || exitFlag == EXIT_MAXLOGGEDDATA) warning(exitFlag);
		exitFlag = EXIT_DEFAULT;
	}
	return;
}

void SBBHostApp::on_buttonLog_toggled() {
	p_menuLog->change_state(buttonLog.get_active());
}

void SBBHostApp::on_menu_file_setfolder() {
	if (buttonRun.get_active()) { return; }
	p_setFolderDialog->set_current_folder(Gio::File::create_for_parse_name(def_dir));
	p_setFolderDialog->show();
}

void SBBHostApp::on_menu_file_save() {
	if (buttonRun.get_active()) { return; }

	std::string name, path;

	//implementation of cases
	std::string page_id = stack.get_visible_child_name();
	//switch (noteBook.get_current_page()) {
	if (page_id.compare(STREAM_LABEL) == 0) {
	//case STREAM_PAGE:
		exitFlag = core.get_lastUnsaved(&id);
		if (exitFlag != EXIT_DEFAULT) {
			warning(exitFlag);
			exitFlag = EXIT_DEFAULT;
			return;
		}
		//name = core.logData[core.find(id)]->name;
		//path = core.logData[core.find(id)]->path;
		exitFlag = core.save(id);
	//	break;
	}
	//case PLOT_PAGE: {
	else if (page_id.compare(PLOT_LABEL) == 0) {
		std::vector<Gtk::TreeModel::iterator> iters = plotTreeView.get_selected(ColumnType::LOG);
		int exit = EXIT_DEFAULT;
		if (iters.size()==0) {
			warning(EXIT_UNSELECTEDLOG);
			return;
		}
		for (int i = 0; i < iters.size(); ++i) {
			int id = iters[i]->get_value(plotTreeView.p_treeColumns->col_log_id);
			if (core.find(id) > -1) warning(core.save(id));
			else exitFlag = EXIT_SAVEUNKNOWISS;
		}
	//	break;
	}
	//default:
	else {
		warning(EXIT_UNEXPECTED);
		return;
	}

}
void SBBHostApp::on_menu_file_saveas() {
	if (buttonRun.get_active()) { return; }

	//init
	std::string name, path;

	//implementation of cases
	std::string page_id = stack.get_visible_child_name();
	//switch (noteBook.get_current_page()) {
	if (page_id.compare(STREAM_LABEL) == 0) {
	//case STREAM_PAGE:
		exitFlag = core.get_lastUnsaved(&id);
		if (exitFlag != EXIT_DEFAULT) {
			warning(exitFlag);
			exitFlag = EXIT_DEFAULT;
			return;
		}
		name = core.logData[core.find(id)]->name;
		path = core.logData[core.find(id)]->path;
		//str = str + FILE_EXT;
		//str = FILE_NAME_HEADER + str;
		p_fileSaveAsDialog->set_current_folder(Gio::File::create_for_parse_name(path));
		p_fileSaveAsDialog->set_current_name(name);
		p_fileSaveAsDialog->show();
		warning(exitFlag);
		exitFlag = EXIT_DEFAULT;
		//break; 
	} else if (page_id.compare(PLOT_LABEL) == 0) {
	//case PLOT_PAGE: {
		std::vector<Gtk::TreeModel::iterator> iters = plotTreeView.get_selected(ColumnType::LOG);
		int exit = EXIT_DEFAULT;
		if (iters.size()==0) {
			warning(EXIT_UNSELECTEDLOG);
			return;
		}
		if (iters.size()>1) {
			warning(EXIT_TOOSELECTEDLOG);
			return;
		}
		id = iters[0]->get_value(plotTreeView.p_treeColumns->col_log_id);
		if (core.find(id) > -1) {
			name = core.logData[core.find(id)]->name;
			path = core.logData[core.find(id)]->path;
			p_fileSaveAsDialog->set_current_folder(Gio::File::create_for_parse_name(path));
			p_fileSaveAsDialog->set_current_name(name);
			p_fileSaveAsDialog->show();
			warning(exitFlag);
			exitFlag = EXIT_DEFAULT;
		}
		else exitFlag = EXIT_SAVEUNKNOWISS;
	}
		//break;
	//default:
	else {
		warning(EXIT_UNEXPECTED);
		return;
	}
}

bool SBBHostApp::on_file_drag_and_drop(const Glib::ValueBase& value, double x, double y) {
	if (G_VALUE_HOLDS(value.gobj(), Glib::Value<Glib::ustring>::value_type())) {
		Glib::Value<Glib::ustring> files_dragged_value;
		files_dragged_value.init(value.gobj());
		std::string files_dragged = (std::string) files_dragged_value.get();
		//format is: file://path/to/file/file.sbb/r/n 
		std::string file_start_flag = FILE_START_FLAG; 
		std::string file_end_flag = FILE_END_FLAG; 
		std::string file_ext = FILE_EXT;
		std::vector<std::string> files; 
		while (true) {
			size_t n = files_dragged.find(file_end_flag);
			if (n == std::string::npos) break;
			std::string item = files_dragged.substr(0, n);
			files_dragged.erase(0, n + file_end_flag.size());
			if (item.substr(0, file_start_flag.size()) != file_start_flag) { break; }
			item.erase(0, file_start_flag.size());
			if (item.substr(item.size()-file_ext.size(), file_ext.size()) != file_ext) { break; }
			files.push_back(item);
		}
		//check if files found
		if (files.size() == 0) {
			warning(EXIT_INVALIFILEEXT);
			return false;
		}
		//open the files
		for (int i = 0; i < files.size(); ++i) warning(open(files[i]));
		exitFlag = EXIT_DEFAULT;
		return true;
	}
	else {
		warning(EXIT_INVALIFILEEXT);
		return false;
	}
}
void SBBHostApp::on_menu_file_open() {
	if (buttonRun.get_active()) { return; }

	//set default folder to save as dialog
	Glib::RefPtr<Gio::File> folder = Gio::File::create_for_parse_name(def_dir);
	p_fileOpenDialog->set_current_folder(folder);

	p_fileOpenDialog->show();
}
void SBBHostApp::on_menu_file_quit() {
	mainWin.close();
	quit();
}
void SBBHostApp::on_menu_stream_run() {
	buttonRun.set_active(!buttonRun.get_active()); //toggle the run button 
}

void SBBHostApp::on_menu_stream_log() {
	if (buttonRun.get_active()) { return; } //do nothing if streming
	buttonLog.set_active(!buttonLog.get_active()); //toggle the log button 
}

void SBBHostApp::on_menu_stream_connect() {
	exitFlag = core.connect(), core.disconnect();
	if (exitFlag == EXIT_CONNECTIONOK) info(EXIT_CONNECTIONOK);
	else warning(exitFlag);
	exitFlag = EXIT_DEFAULT;
}
void SBBHostApp::on_menu_settings() {
	//implementation of cases
	std::string page_id = stack.get_visible_child_name();
	if (page_id.compare(STREAM_LABEL) == 0) settingsDialog.show(&params, STREAM_PAGE);
	else if (page_id.compare(PLOT_LABEL) == 0) settingsDialog.show(&params, PLOT_PAGE);
	else {
		warning(EXIT_UNEXPECTED);
		return;
	}
}
void SBBHostApp::on_menu_help_about() {
	if (buttonRun.get_active()) { return; }
	aboutDialog.show();
	//Bring it to the front, in case it was already shown:
	aboutDialog.present();
}
void SBBHostApp::on_menu_help_help() {
	if (buttonRun.get_active()) { return; }
	system(DOC_FILE);
}

void SBBHostApp::on_setting_dialog(int exit) {
	switch (exit) {
	case Gtk::ResponseType::OK:
		exitFlag = settingsDialog.set_params(&params);
		clear_stream_graph();
		settingsDialog.close();
		break;
	case Gtk::ResponseType::APPLY:
		exitFlag = settingsDialog.set_params(&params);
		clear_stream_graph();
		break;
	default: //CANCEL or DELETE_EVENT
		exitFlag = EXIT_DEFAULT;
		settingsDialog.close();
		break;
	}
	//warning(exitFlag);
	exitFlag = EXIT_DEFAULT;
}

void SBBHostApp::on_file_saveAs_dialog(int exit) {

	if (exit != Gtk::ResponseType::DELETE_EVENT) def_dir = p_fileSaveAsDialog->get_current_folder()->get_path();
	std::string current_filter = p_fileSaveAsDialog->get_filter()->get_name();
	switch (exit) {
	case Gtk::ResponseType::OK:
		if (current_filter == (std::string) FILE_EXT_LABEL) {
			exitFlag = core.save(id, p_fileSaveAsDialog->get_current_name(), p_fileSaveAsDialog->get_current_folder()->get_path());
			//update plot tree log name according to saved file
			plotTreeView.update_logname(id, p_fileSaveAsDialog->get_current_name());
		}
		else if (current_filter == (std::string) FILE_CSV_LABEL) exitFlag = core.save_dat(id, p_fileSaveAsDialog->get_current_folder()->get_path() + FILE_SEP + p_fileSaveAsDialog->get_current_name() + FILE_CSV); 
		else if (current_filter == (std::string) FILE_MAT_LABEL) exitFlag = core.save_mat(id, p_fileSaveAsDialog->get_current_folder()->get_path() + FILE_SEP + p_fileSaveAsDialog->get_current_name() + FILE_MAT);
		else exitFlag = EXIT_SAVEUNKNOWISS;
		break;
	case Gtk::ResponseType::CANCEL:
		exitFlag = EXIT_DEFAULT;
		break;
	case Gtk::ResponseType::DELETE_EVENT:
		exitFlag = EXIT_DEFAULT;
		break;
	default:
		exitFlag = EXIT_SAVEUNKNOWISS;
		break;
	}
	warning(exitFlag);
	exitFlag = EXIT_DEFAULT;
	p_fileSaveAsDialog->hide();
}

void SBBHostApp::on_file_setfolder_dialog(int exit) {
	if (exit == Gtk::ResponseType::OK) {
		def_dir = p_setFolderDialog->get_current_folder()->get_path();
		core.set_path_logs(def_dir);
	}
	p_setFolderDialog->hide();
}

void SBBHostApp::on_file_open_dialog(int exit) {

	if (exit != Gtk::ResponseType::DELETE_EVENT) def_dir = p_fileOpenDialog->get_current_folder()->get_path();
	switch (exit) {
	case Gtk::ResponseType::OK:
		exitFlag = open(p_fileOpenDialog->get_file()->get_path());
		break;
	case Gtk::ResponseType::CANCEL:
		exitFlag = EXIT_DEFAULT;
		break;
	case Gtk::ResponseType::DELETE_EVENT:
		exitFlag = EXIT_DEFAULT;
		break;
	default:
		exitFlag = EXIT_OPENUNKNOWISS;
		break;
	}
	warning(exitFlag);
	exitFlag = EXIT_DEFAULT;
	p_fileOpenDialog->hide();
}

int SBBHostApp::open(std::string filename) {
	int exit = core.open(filename);
	if (exit == EXIT_DEFAULT) {
		plotTreeView.append_row();
		std::string page_id = stack.get_visible_child_name();
		//switch (noteBook.get_current_page()) {
		//char page_to_use[1] = { PLOT_PAGE };
		if (page_id != PLOT_LABEL) stack.set_visible_child(PLOT_LABEL);
	}
	return exit;
}

void SBBHostApp::on_darkModeSwitch_switched() {
	Gtk::Settings::get_default()->property_gtk_application_prefer_dark_theme().set_value(darkModeSwitch.get_active());
	p_streamGraph->set_theme("Default");
	p_plotGraph->set_theme("Default");
}

bool SBBHostApp::on_win_close() { //return false to confirm win close
	if (force_close) { return false; } //force the app to be closed when force_close set by on_quit_confirmation

	if (core.check_allSaved()) { return false; } //all saved, continue win close

	//sth not saved, ask the user confirmation - close is done in signal responce function (see wait_for_user_responce), therefore returning true
	p_messageDialog.reset(new Gtk::MessageDialog(mainWin, "", false, Gtk::MessageType::QUESTION, Gtk::ButtonsType::OK_CANCEL, true));
	p_messageDialog->set_default_size(params.messageDialog_width, params.messageDialog_height);
	p_messageDialog->set_resizable(false);
	p_messageDialog->set_transient_for(mainWin);
	p_messageDialog->set_hide_on_close(true);
	p_messageDialog->signal_response().connect(sigc::mem_fun(*this, &SBBHostApp::on_quit_confirmation));
	p_messageDialog->set_message("Unsaved data found");
	p_messageDialog->set_secondary_text("Are you sure to close the application?");
	p_messageDialog->show();
	return true;
}

void SBBHostApp::on_quit_confirmation(int exit) {
	if (exit == Gtk::ResponseType::OK) {
		force_close = true;
		on_menu_file_quit();
	} else p_messageDialog->close();
}

bool SBBHostApp::on_read_serial() {
	//check run but
	if (!buttonRun.get_active()) { return false;  }
	
	//read & write (data stored in core.rx_packet if successfull)
	exitFlag = core.stream(hostControl.get_values(core.tx_packet.value, params.num_of_controls));

	//plot data
	if ((exitFlag == EXIT_DEFAULT) && ((core.samples % params.stream_graph_decim) == 0)) {
		std::vector<int> ids = streamTreeView.get_col_id_selected();
		for (int i = 0; i < ids.size(); i++) p_streamGraph->add_point(ids[i], (double)core.samples, (double)core.rx_packet.getValue(ids[i]), false, false);
	}

	double now = 1000*(timer.elapsed());
	//update graph
	if ((now - streamGraphTimer) >= 1000.0 / ((double)params.fps)) {
		streamGraphTimer = now;
		long long xmin = (core.samples > ((long long)params.window_samples - params.forward_samples)) ? core.samples - ((long long)params.window_samples - params.forward_samples) : 0;
		long long xmax = (core.samples > ((long long)params.window_samples - params.forward_samples)) ? core.samples + params.forward_samples : params.window_samples;
		//p_streamGraph->set_xlim(core.samples - (window_samples - forward_samples), core.samples+ forward_samples);
		p_streamGraph->set_xlim(xmin, xmax);
		p_streamGraph->update_ylim(-1, +1);
		p_streamGraph->update_graph();
	}

	if ((now - streamTimer) >= STREAMTIME_UPDATE_MS) {
		streamTimer = now;
		set_streamTimer((int) timer.elapsed());
	}

	//checks
	if (exitFlag == EXIT_LOSTCONNECTION || exitFlag == EXIT_MAXLOGGEDDATA) {
		buttonRun.set_active(false);
		return false;
	}

	return true;
}

void SBBHostApp::set_streamTimer(int secs) {
	utils::hhmmss_t time = utils::sec2hhmmss(secs);
	char c[10] = { 0 };
	sprintf_s(c, 10, STREAMTIME_FORMAT, time.h, time.m, time.s);
	streamTimeLabel.set_text(c);
}

void SBBHostApp::warning(int type) {
	if (type == EXIT_DEFAULT) { return; }

	p_messageDialog.reset(new Gtk::MessageDialog(mainWin, "", false, Gtk::MessageType::WARNING, Gtk::ButtonsType::OK, true));
	p_messageDialog->set_default_size(params.messageDialog_width, params.messageDialog_height);
	p_messageDialog->set_resizable(false);
	p_messageDialog->set_transient_for(mainWin);
	p_messageDialog->set_hide_on_close(true);
	p_messageDialog->signal_response().connect(sigc::hide(sigc::mem_fun(*p_messageDialog, &Gtk::Widget::hide)));

	switch (type) {
	case EXIT_NOSERIALPORT:
		utils::beep(WARNING_BEEP);
		p_messageDialog->set_message("Unable to connect");
		p_messageDialog->set_secondary_text("No serial device found");
		break;
	case EXIT_MULTIPLEPORT:
		utils::beep(WARNING_BEEP);
		p_messageDialog->set_message("Unable to connect");
		p_messageDialog->set_secondary_text("Multiple port found. Please select the port to be used in Stream > Settings");
		break;
	case EXIT_UNABLECONNECT:
		utils::beep(WARNING_BEEP);
		p_messageDialog->set_message("Unable to connect");
		char str[64];
		sprintf_s(str, 64, "Connection to port %d failed", core.p_params->port);
		p_messageDialog->set_secondary_text(str);
		break;
	case EXIT_LOSTCONNECTION:
		utils::beep(WARNING_BEEP);
		p_messageDialog->set_message("Unable to connect");
		p_messageDialog->set_secondary_text("Issues in device connection");
		break;
	case EXIT_MAXLOGGEDDATA:
		utils::beep(WARNING_BEEP);
		p_messageDialog->set_message("Stream stopped");
		p_messageDialog->set_secondary_text("Max logged data reached");
		break;
	case EXIT_NOLOGDATA:
		utils::beep(WARNING_BEEP);
		p_messageDialog->set_message("Unable to save");
		p_messageDialog->set_secondary_text("No logged data found");
		break;
	case EXIT_SAVEUNKNOWISS:
		utils::beep(ERROR_BEEP);
		p_messageDialog->set_message("Unable to save");
		p_messageDialog->set_secondary_text("Unknow issue occured in file saving");
		break;
	case EXIT_ALLSAVED:
		utils::beep(WARNING_BEEP);
		p_messageDialog->set_message("Unable to save");
		p_messageDialog->set_secondary_text("All logged data already saved");
		break;
	case EXIT_OPENUNKNOWISS:
		utils::beep(ERROR_BEEP);
		p_messageDialog->set_message("Unable to open");
		p_messageDialog->set_secondary_text("Unknown issue occured in file opening");
		break;
	case EXIT_UNEXPECTED:
		utils::beep(ERROR_BEEP);
		p_messageDialog->set_message("Error");
		p_messageDialog->set_secondary_text("Unexpected error occured");
		break;
	case EXIT_INVALIDHEADER:
		utils::beep(ERROR_BEEP);
		p_messageDialog->set_message("Error");
		p_messageDialog->set_secondary_text("Invalid header");
		break;
	case EXIT_INVALIDTERMIN:
		utils::beep(ERROR_BEEP);
		p_messageDialog->set_message("Error");
		p_messageDialog->set_secondary_text("Invalid terminator");
		break;
	case EXIT_INVALIFILEEXT:
		utils::beep(WARNING_BEEP);
		p_messageDialog->set_message("Unable to open");
		p_messageDialog->set_secondary_text("Invalid file extension");
		break;
	case EXIT_INVALIDFILE:
		utils::beep(WARNING_BEEP);
		p_messageDialog->set_message("Unable to open");
		p_messageDialog->set_secondary_text("Invalid file");
		break;
	case EXIT_UNSELECTEDLOG:
		utils::beep(WARNING_BEEP);
		p_messageDialog->set_message("Unable to save");
		p_messageDialog->set_secondary_text("No logged data selected");
		break;
	case EXIT_TOOSELECTEDLOG:
		utils::beep(WARNING_BEEP);
		p_messageDialog->set_message("Unable to save");
		p_messageDialog->set_secondary_text("Only one log must be saved at a time");
		break;
	default:
		return;
	}
	p_messageDialog->show();
}

void SBBHostApp::info(int type) {
	if (type == EXIT_DEFAULT) { return; }

	p_messageDialog.reset(new Gtk::MessageDialog(mainWin, "", false, Gtk::MessageType::INFO, Gtk::ButtonsType::OK, true));
	p_messageDialog->set_default_size(params.messageDialog_width, params.messageDialog_height);
	p_messageDialog->set_resizable(false);
	p_messageDialog->set_transient_for(mainWin);
	p_messageDialog->set_hide_on_close(true);
	p_messageDialog->signal_response().connect(sigc::hide(sigc::mem_fun(*p_messageDialog, &Gtk::Widget::hide)));
	
	switch (type) {
	case EXIT_CONNECTIONOK:
		utils::beep(INFO_BEEP);
		p_messageDialog->set_message("Connection established");
		char str[64];
		sprintf_s(str, 64, "Connected to port %d", core.p_params->port);
		p_messageDialog->set_secondary_text(str);
		break;
	}
	p_messageDialog->show();
}

void SBBHostApp::on_plotType_changed() { //TODOOOOO
	switch (plotTypeComboEntry.get_selected()) {
	case SAMPLE_PLOTID:
		fftBox.set_visible(false);
		p_plotGraph->set_visible(true);
		//TODO hide others
		//TODO tell plotTreeView to do make plot for only sample plot
		break;
	case FFT_PLOTID:
		p_plotGraph->set_visible(false);
		fftBox.set_visible(true);
		//TODO hide others
		//TODO tell plotTreeView to do make plot for only sample plot
		break;
	case PSD_PLOTID:
		//TODO hide others
		//TODO tell plotTreeView to do make plot for only sample plot
		break;
	case FRF_PLOTID:
		//TODO hide others
		//TODO tell plotTreeView to do make plot for only sample plot
		break;
	default: //do nothing
		break;
	}
}
