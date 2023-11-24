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

