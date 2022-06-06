#include "EntryLabel.h"


SpinEntryLabel::SpinEntryLabel(double val, double lower, double upper, double step, Glib::ustring lbl) :
Gtk::Box(Gtk::Orientation::HORIZONTAL),
label(lbl),
entry() {
	Glib::RefPtr<Gtk::Adjustment> adj = Gtk::Adjustment::create(val, lower, upper, step);
	entry.set_adjustment(adj);
	append(label);
	append(entry);
	//label.set_margin(MARGIN);
	entry.set_margin_start(MARGIN);
	label.set_size_request(SETTING_LABEL_WIDTH);
	entry.set_size_request(SETTING_ENTRY_WIDTH);
	label.set_justify(Gtk::Justification::RIGHT);
	label.set_halign(Gtk::Align::START);
	label.set_hexpand(true);
	entry.set_hexpand(true);
}

void SpinEntryLabel::set_value(double val) {
	entry.set_value(val);
}

double SpinEntryLabel::get_value() {
	return entry.get_value();
}

TextEntryLabel::TextEntryLabel(Glib::ustring str, Glib::ustring lbl) :
	Gtk::Box(Gtk::Orientation::HORIZONTAL),
	label(lbl),
	entry() {
	entry.set_text(str);
	append(label);
	append(entry);
	//label.set_margin(MARGIN);
	entry.set_margin_start(MARGIN);
	label.set_size_request(SETTING_LABEL_WIDTH);
	entry.set_size_request(SETTING_ENTRY_WIDTH);
	label.set_justify(Gtk::Justification::RIGHT);
	label.set_halign(Gtk::Align::START);
	label.set_hexpand(true);
	entry.set_hexpand(true);
}

void TextEntryLabel::set_text(Glib::ustring str) {
	entry.set_text(str);
}

Glib::ustring TextEntryLabel::get_text() {
	return entry.get_text();
}

ComboEntryLabel::ComboEntryLabel(std::vector<int> vals, Glib::ustring lbl) :
	Gtk::Box(Gtk::Orientation::HORIZONTAL),
	label(lbl),
	entry(false) {
	set_entries(vals);
	append(label);
	append(entry);
	//label.set_margin(MARGIN);
	entry.set_margin_start(MARGIN);
	label.set_size_request(SETTING_LABEL_WIDTH);
	entry.set_size_request(SETTING_ENTRY_WIDTH);
	label.set_justify(Gtk::Justification::RIGHT);
	label.set_halign(Gtk::Align::START);
	label.set_hexpand(true);
	entry.set_hexpand(true);
}

bool ComboEntryLabel::set_selected(int val) {
	for (int i = 0; i < entries.size(); i++) {
		if (entries[i] == val) {
			entry.set_active(i);
			return true;
		}
	}
	entry.unset_active();
	return false;
}

void ComboEntryLabel::set_entries(std::vector<int> vals) {
	entry.remove_all();
	entries = vals;
	for (int i = 0; i < entries.size(); i++) entry.append(std::to_string(entries[i]));
}

std::vector<int> ComboEntryLabel::get_entries() {
	return entries;
}

int ComboEntryLabel::get_selected() {
	int i = entry.get_active_row_number();
	if (i > -1) { return entries[i]; }
	else { return -1; }
}


ComboChoiceEntryLabel::ComboChoiceEntryLabel(Glib::ustring lbl) :
	Gtk::Box(Gtk::Orientation::HORIZONTAL),
	label(lbl),
	entry(false) {
	append(label);
	append(entry);
	entry.set_margin_start(MARGIN);
	label.set_justify(Gtk::Justification::RIGHT);
	label.set_halign(Gtk::Align::START);
	label.set_hexpand(true);
	entry.set_hexpand(true);
}

void ComboChoiceEntryLabel::set_entries(std::vector<int> choice_id, std::vector<std::string> choice_str) {
	entry.remove_all();
	id = choice_id;
	str = choice_str;
	for (int i = 0; i < str.size(); i++) entry.append(str[i]);
}

int ComboChoiceEntryLabel::get_selected() {
	int i = entry.get_active_row_number();
	if (i > -1) { return id[i]; }
	else { return -1; }
}

bool ComboChoiceEntryLabel::set_selected(int choice_id) {
	for (int i = 0; i < id.size(); i++) {
		if (id[i] == choice_id) {
			entry.set_active(i);
			return true;
		}
	}
	entry.unset_active();
	return false;
}

Gtk::ComboBoxText* ComboChoiceEntryLabel::get_comboBoxText() {
	return &entry;
}