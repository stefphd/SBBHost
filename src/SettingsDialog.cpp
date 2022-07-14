#include "SettingsDialog.h"

SettingsDialog::SettingsDialog() : Gtk::Dialog(),
noteBook(), streamGrid(), plotGrid(),
port({ 0 }, "Serial port"),
baud(9600, 9600, 4e6, 100, "Baudrate"),
socket_port(1234, 0, 65634, 1, "Socket port"),
timeout(0, 0, 1000, 10, "Timeout"),
maxmissings(0, 0, 1000, 10, "Max missings"),
winsamples(100, 100, 10000, 100, "Window samples"),
prevsamples(0, 0, 2000, 100, "Preview samples"),
fps(25, 1, 60, 1, "Graph fps"), 
stream_decim(1, 1, 50, 1, "Decimation"), 
header("0x000000", "Header"),
terminator("0x000000", "Terminator"),
socket_ip("192.168.0.1", "Socket IP")
//hsep0(Gtk::Orientation::HORIZONTAL),
//hsep1(Gtk::Orientation::HORIZONTAL),
//hsep2(Gtk::Orientation::HORIZONTAL),
//hsep3(Gtk::Orientation::HORIZONTAL),
//hsep4(Gtk::Orientation::HORIZONTAL),
//vsep0(Gtk::Orientation::VERTICAL)
{

	//set child
	get_content_area()->append(noteBook);
	noteBook.insert_page(streamGrid, STREAMSETTINGS_LABEL, STREAM_PAGE);
	noteBook.insert_page(plotGrid, PLOTSETTINGS_LABEL, PLOT_PAGE);
	noteBook.get_tab_label(streamGrid)->set_hexpand(true);
	noteBook.get_tab_label(plotGrid)->set_hexpand(true);
	p_buttonCancel = add_button("Cancel", Gtk::ResponseType::CANCEL);
	p_buttonApply = add_button("Apply", Gtk::ResponseType::APPLY);
	p_buttonOK = add_button("OK", Gtk::ResponseType::OK);

	streamGrid.attach(port, 0, 1),
	streamGrid.attach(baud, 0, 2),
	streamGrid.attach(socket_port, 0, 3), 
	streamGrid.attach(socket_ip, 0, 4),
	streamGrid.attach(header, 0, 5),
	streamGrid.attach(terminator, 0, 6),
	streamGrid.attach(timeout, 1, 1),
	streamGrid.attach(maxmissings, 1, 2);
	streamGrid.attach(winsamples, 1, 3);
	streamGrid.attach(prevsamples, 1, 4);
	streamGrid.attach(fps, 1, 5);
	streamGrid.attach(stream_decim, 1, 6);
	streamGrid.property_row_homogeneous() = true;
	streamGrid.property_column_homogeneous() = true;

	//set callbacks
	/*p_buttonOK->signal_clicked().connect(sigc::mem_fun(*this,
		&SettingsDialog::on_ok_button_pressed));
	p_buttonApply->signal_clicked().connect(sigc::mem_fun(*this,
		&SettingsDialog::on_apply_button_pressed));
	p_buttonCancel->signal_clicked().connect(sigc::mem_fun(*this,
		&SettingsDialog::on_cancel_button_pressed));*/
	
	//appearance
	streamGrid.set_expand(true);
	plotGrid.set_expand(true);
	p_buttonCancel->set_margin(MARGIN);
	p_buttonApply->set_margin(MARGIN);
	p_buttonOK->set_margin(MARGIN);

	p_buttonCancel->set_halign(Gtk::Align::START);
	p_buttonApply->set_halign(Gtk::Align::CENTER);
	p_buttonOK->set_halign(Gtk::Align::END);

	port.set_margin(MARGIN);
	baud.set_margin(MARGIN);
	socket_port.set_margin(MARGIN);
	socket_ip.set_margin(MARGIN);
	header.set_margin(MARGIN);
	terminator.set_margin(MARGIN);
	timeout.set_margin(MARGIN);
	maxmissings.set_margin(MARGIN);
	winsamples.set_margin(MARGIN);
	prevsamples.set_margin(MARGIN);
	fps.set_margin(MARGIN);
	stream_decim.set_margin(MARGIN);

}

void SettingsDialog::show(Params* p_params, int page) {
	noteBook.set_current_page(page);

	//update setting parameter
	port.set_entries(Serial::get_availableSerialPorts(MAX_PORT - 1));
	port.set_selected(p_params->port);
	baud.set_value(p_params->baud);
	socket_port.set_value(p_params->socket_port);
	socket_ip.set_text(p_params->socket_ip);
	timeout.set_value(p_params->timeout);
	maxmissings.set_value(p_params->max_missing_packets);
	winsamples.set_value(p_params->window_samples);
	prevsamples.set_value(p_params->forward_samples);
	fps.set_value(p_params->fps);
	stream_decim.set_value(p_params->stream_graph_decim);
	header.set_text(utils::hex2str(p_params->header));
	terminator.set_text(utils::hex2str(p_params->terminator));
	
	//show dialog
	Gtk::Dialog::show();
}

int SettingsDialog::set_params(Params * p_params) {
	int exitFlag = EXIT_DEFAULT;
	p_params->port = port.get_selected();
	p_params->baud = baud.get_value();
	p_params->socket_port = socket_port.get_value();
	p_params->socket_ip = socket_ip.get_text();
	p_params->timeout = timeout.get_value();
	p_params->max_missing_packets = maxmissings.get_value();
	p_params->window_samples = winsamples.get_value();
	p_params->forward_samples = prevsamples.get_value();
	p_params->fps = fps.get_value();
	p_params->stream_graph_decim = stream_decim.get_value();
	if (!utils::str2hex(header.get_text(), &(p_params->header))) exitFlag = EXIT_INVALIDHEADER;
	if (!utils::str2hex(terminator.get_text(), &(p_params->terminator))) exitFlag = EXIT_INVALIDTERMIN;
	return exitFlag;
}

/*
void SettingsDialog::on_ok_button_pressed() {
	set_params();
	close();
}
void SettingsDialog::on_apply_button_pressed() {
	set_params();
}
void SettingsDialog::on_cancel_button_pressed() {
	close();
}
*/