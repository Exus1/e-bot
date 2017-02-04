#ifndef _MODULES_MANAGER__HPP
#define _MODULES_MANAGER__HPP

#include <iostream>
#include <unistd.h>
#include <dirent.h>
#include <dlfcn.h>
#include <string>


#include <Module_interface.hpp>
#include <Module_thread.hpp>

#include <Ts3Api/Server.hpp>

#include <colors.h>

using namespace std;

extern bool DEBUG_MODE;
extern bool API_VERSION;


class Modules_manager {
	struct module_file {
		string file_name;
		Module_thread thd;

		void* dl_handle = NULL;
		Module_interface *object_handle = NULL;

		bool loaded();
	};

private:
	float version = 0.1;

	map<string, module_file> modules_files;

	string modules_folder_path;

	Ts3Api::Server *ts3_server = NULL;

	int check_module_compatibility(Module_interface* object);

public:

	Modules_manager(string modules_folder_path);

	void set_server_object(Ts3Api::Server& server);

	int list_modules();

	bool folder_is_readable();

	void load_module(string module_name);

	void unload_module(string module_name);

	void restart_module(string module_name);

	bool load_all_modules();

	bool start_module(string module_name);

	bool stop_module(string module_name);
};



#endif