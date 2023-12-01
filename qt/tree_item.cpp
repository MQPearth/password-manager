#include "tree_item.h"


tree_item_url::tree_item_url(std::string p1, std::string p2)
	: category(p1), url(p2) {}


bool tree_item_url::operator<(const tree_item_url& other) const {
	if (category != other.category) {
		return category < other.category;
	}
	return url < other.url;
}

std::string tree_item_url::get_category() const
{
	return category;
}

std::string tree_item_url::get_url() const
{
	return url;
}

tree_item_login_name::tree_item_login_name(std::string p1, std::string p2, std::string p3) : tree_item_url(p1, p2), login_name(p3) {}

bool tree_item_login_name::operator<(const tree_item_login_name& other) const {
	if (this->get_category() != this->get_category()) {
		return this->get_category() < this->get_category();
	}

	if (this->get_url() != this->get_url()) {
		return this->get_url() < this->get_url();
	}

	return login_name < other.login_name;
}

std::string tree_item_login_name::get_login_name() const
{
	return login_name;
}


tree_item_all::tree_item_all(std::string p1, std::string p2, std::string p3, std::string p4, std::string p5) 
	: tree_item_login_name(p1, p2, p3), password(p4), note(p5) {}



std::string tree_item_all::get_password() const
{
	return password;
}


std::string tree_item_all::get_note() const
{
	return note;
}

bool tree_item_all::operator<(const tree_item_all& other) const {
	if (this->get_category() != other.get_category()) {
		return this->get_category() < other.get_category();
	}

	if (this->get_url() != other.get_url()) {
		return this->get_url() < other.get_url();
	}

	if (this->get_login_name() != other.get_login_name()) {
		return this->get_login_name() < other.get_login_name();
	}

	if (this->get_password() != other.get_password()) {
		return this->get_password() < other.get_password();
	}

	return note < other.note;
}