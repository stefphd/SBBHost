#include "HostControl.h"

HostControl::HostControl() : Gtk::Frame(),
grid()
//enableButton("Enable all")
{
	set_label(HOSTCTRLWIN_NAME);

	set_child(grid);

	//set callback
	//enableButton.signal_clicked().connect(sigc::mem_fun(*this, &HostControl::on_enable_button_pressed));

	//set appearance
	//enableButton.set_halign(Gtk::Align::CENTER);
	//enableButton.set_margin_bottom(MARGIN);
	//grid.set_expand();
}

void HostControl::append_slider(Glib::ustring name, double min, double max, double val) {
	sliders.push_back(new Slider(name, min, max, val));
	sliders.back()->set_margin(MARGIN);
}

void HostControl::update() {
	int num_of_sliders = sliders.size();
	int cols = floor(sqrt(num_of_sliders));
	printf("%d\n", cols);
	int ic = 0, ir = 0;
	for (int i = 0; i < num_of_sliders; ++i) {
		grid.attach(*sliders[i], ic, ir);
		printf("c: %d, r: %d\n", ic, ir);
		++ic;
		if (ic >= cols) { ++ir; ic = 0;}
	}
	//++ir;
	//grid.attach(enableButton, 0, ir, cols);
	
}

bool HostControl::get_values(float* buf, size_t len) {
	bool exit = false;
	for (int i = 0; i < len; ++i) {
		if (sliders[i]->get_active()) buf[i] = (float) sliders[i]->get_value(), exit = true;
		else buf[i] = *((float*)&nanVal); //take address of nanVal, cast to float*, and return value pointed by
	}
	return exit;
}

/*
void HostControl::on_enable_button_pressed() {
	for (int i = 0; i < sliders.size(); ++i) sliders[i]->set_active(enableButton.get_active());
}
*/

Slider::Slider(Glib::ustring name, double min, double max, double val) : Gtk::Frame(name),
vbox(Gtk::Orientation::VERTICAL), hminbox(Gtk::Orientation::HORIZONTAL), hmaxbox(Gtk::Orientation::HORIZONTAL),
minEntry(0.1), maxEntry(0.1), minLabel("Min"), maxLabel("Max"),
button("Enable")
{	
	//set childs
	set_child(vbox);
	vbox.append(scrollbar);
	vbox.append(hmaxbox);
	vbox.append(hminbox);
	vbox.append(button);
	hmaxbox.append(maxLabel);
	hmaxbox.append(maxEntry);
	hminbox.append(minLabel);
	hminbox.append(minEntry);

	//set scale
	p_adj = Gtk::Adjustment::create(val, min, max, (max - min) / 1000.0, (max - min) / 100.0);
	scrollbar.set_adjustment(p_adj);
	scrollbar.set_orientation(Gtk::Orientation::VERTICAL);
	scrollbar.set_digits(3);
	scrollbar.set_value_pos(Gtk::PositionType::TOP);
	scrollbar.set_draw_value();
	scrollbar.set_inverted(); // highest value at top

	//set min/max
	p_minadj = Gtk::Adjustment::create(min, -1e6, 1e6, 0.01, 0.1, 0.1);
	p_maxadj = Gtk::Adjustment::create(max, -1e6, 1e6, 0.01, 0.1, 0.1);
	minEntry.set_adjustment(p_minadj);
	maxEntry.set_adjustment(p_maxadj);
	minEntry.set_digits(3);
	maxEntry.set_digits(3);

	//set callbacks
	p_minadj->signal_value_changed().connect(sigc::mem_fun(*this,
		&Slider::on_minadj_changed));
	p_maxadj->signal_value_changed().connect(sigc::mem_fun(*this,
		&Slider::on_maxadj_changed));
	button.signal_toggled().connect(sigc::mem_fun(*this, &Slider::on_enable_button_toggled));

	//set appearance
	scrollbar.set_margin(MARGIN / 2);
	minEntry.set_margin(MARGIN / 2);
	maxEntry.set_margin(MARGIN / 2);
	minLabel.set_margin(MARGIN / 2);
	maxLabel.set_margin(MARGIN / 2);
	scrollbar.set_vexpand();
	button.set_halign(Gtk::Align::CENTER);
	button.set_margin(MARGIN/2);

	//init active
	on_enable_button_toggled();
}

void Slider::on_minadj_changed() {
	if (p_minadj->get_value() > p_maxadj->get_value()) p_minadj->set_value(p_maxadj->get_value());
	p_adj->set_lower(p_minadj->get_value());
	if (p_adj->get_value() < p_adj->get_lower()) p_adj->set_value(p_adj->get_lower());

}

void Slider::on_maxadj_changed() {
	if (p_maxadj->get_value() < p_minadj->get_value()) p_maxadj->set_value(p_minadj->get_value());
	p_adj->set_upper(p_maxadj->get_value());
	if (p_adj->get_value() > p_adj->get_upper()) p_adj->set_value(p_adj->get_upper());
}

void Slider::set_active(bool is_active) {
	button.set_active(is_active);
}

bool Slider::get_active() {
	return button.get_active();
}

double Slider::get_value() {
	return scrollbar.get_value();
}

void Slider::on_enable_button_toggled() {
	bool is_active = button.get_active();
	scrollbar.set_sensitive(is_active);
	minEntry.set_sensitive(is_active);
	maxEntry.set_sensitive(is_active);
}