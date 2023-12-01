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

	std::string get_login_name() const;

	bool operator<(const tree_item_login_name& other) const;
};


class tree_item_all : public tree_item_login_name
{
private:
	std::string password;
	std::string note;

public:
	tree_item_all(std::string p1, std::string p2, std::string p3, std::string p4, std::string p5);

	std::string get_password() const;

	std::string get_note() const;

	bool operator<(const tree_item_all& other) const;
};

