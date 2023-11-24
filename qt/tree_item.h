#pragma once
#include "iostream"


class tree_item_url
{
private:
	std::string category;
	std::string url;
public:
	tree_item_url(std::string p1, std::string p2);

	bool operator<(const tree_item_url& other) const;

	std::string get_category() const;

	std::string get_url() const;
};


class tree_item_login_name : public tree_item_url
{
private:
	std::string login_name;

public:
	tree_item_login_name(std::string p1, std::string p2, std::string p3);

	bool operator<(const tree_item_login_name& other) const;
};

