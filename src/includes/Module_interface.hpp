#ifndef _MODULE_INTEFACE__HPP
#define _MODULE_INTEFACE__HPP

#include <iostream>
#include <map>
#include <string>

#include <Ts3Api/Server.hpp>
#include <ConfigFile.hpp>

using namespace std;


class Module_interface {
	friend class Modules_manager;
	friend class Module_thread;
protected:
	struct module_informations {
		string module_name = "New module";
		string module_prefix = "nw";
		string module_version = "0.1a";

		float minimum_api_version = 0.1;
		float minimum_ebot_version = 0.1;
	} _module_informations;

	struct callback_function_return {
	string message = "Error";

	bool success = false;
};

protected:
	Ts3Api::Server *server = NULL;

	ConfigFile* config_file = NULL;

	map<string, map<string, string>> default_config;

	void set_server_object(Ts3Api::Server* server) { this->server = server; };
public:
	callback_function_return _callback(string command, map<string, string> args) { return callback_function_return(); };

	virtual void module_loop()=0;
};


#endif