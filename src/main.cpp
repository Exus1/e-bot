#include <iostream>

#include <sys/stat.h>

#include <Modules_manager.hpp>
#include <Ts3Api/Server.hpp>
#include <ConfigFile.hpp>
#include <colors.h>


using namespace std;

bool DEBUG_MODE = false;


int main(int argc, char* argv[]) {
	bool error = false;
	map<string, map<string, string>> default_config;

	//
	// Default config file
	//
	default_config["connection"]["query_login"] = "serveradmin";
	default_config["connection"]["query_password"] = "password";
	default_config["connection"]["query_port"] = "10011";
	default_config["connection"]["server_port"] = "9987";
	default_config["connection"]["server_address"] = "127.0.0.1";



	//
	// Creating default directories
	//
	mkdir("Modules", 0777);
	mkdir("Configs", 0777);



	//
	// Debug mode
	//
	if(argc > 1) {
		if(string(argv[1]) == "-debug") {
			DEBUG_MODE = true;
		}
	}



	//
	// Welcome message
	//
	cout << COLOR_INFO;
	cout << "///////////////////////////////////" << endl;
	cout << "//  e-bot - TeamSpeak3 multibot  //" << endl;
	cout << "//  Created by: Exus             //" << endl;
	cout << "//  GitHub: Exus1                //" << endl;
	cout << "///////////////////////////////////" << endl;
	cout << COLOR_RESET << endl;




	//
	// Creating TeamSpeak3 server object
	//
	Ts3Api::Server server;

	//
	// Creating Modules_manager object
	//
	Modules_manager manager("Modules/");



	// 
	// Creating config.cfg
	//
	ConfigFile cfg("config.cfg");

	if(!cfg.good()) {
		cfg.content = default_config;
		cfg.save();

		cout << "Config file was created" << endl;
		return 0;
	}



	// 
	// Opening config file
	//
	if(!cfg.good()) {
		cout << COLOR_ERROR;
		cout << "Can't open config file" << endl;
		return 1;
	}



	//
	// Checking config file
	//
	if(cfg["connection"].empty()) {
		cout << COLOR_ERROR;
		cout << "Config connection section is empty" << endl;
		return 1;
	}



	//
	// Connecting to TeamSpeak3 server
	//
	cout << "Connecting to TeamSpeak3 Server" << endl;
	if(!server.serverConnect("e-bot.eu", "10011")) {
		cout << COLOR_ERROR;
		cout << "Can't connect to server" << endl;
		return 1;
	}



	//
	// Login to TeamSpeak3 server
	cout << "Logging to TeamSpeak3 Server" << endl;
	if(server.login("serveradmin", "bleblesiema").error) {
		cout << COLOR_ERROR;
		cout << "Can't login to server" << endl;
		return 1;
	}



	// 
	// Selecting TeamSpeak3 server
	//
	cout << "Selecting virtualserver" << endl;
	if(server.selectServer("9987").error) {
		cout << COLOR_ERROR;
		cout << "Can't select virtualserver" << endl;
		return 1;
	}



	//
	// Setting nickname
	//
	cout << "Setting nickname" << endl;
	if(server.setNickname("e-bot").error) {
		cout << COLOR_WARNING;
		cout << "Can't set nickname" << endl;
		cout << COLOR_RESET;
	}



	//
	// Setting server object in Modules_manager
	manager.set_server_object(server);



	//
	// Checking whether modules directory is readable
	//
	if(!manager.folder_is_readable()) {
		cout << COLOR_ERROR;
		cout << "E-bot don't have permissions to read files in modules folder" << endl;
		return 1;
	}



	//
	// Listing modules
	//
	int modules_count = manager.list_modules();



	//
	// Message how many modules have been loaded
	cout << COLOR_INFO;
	cout << "Number of modules in Modules directory: " << modules_count << endl;
	cout << COLOR_RESET;



	//
	// End the program if modules count < 1
	//
	if(modules_count < 1) return 0;



	// 
	// Loading all modules
	//
	cout << "Loading all modules" << endl;
	if(manager.load_all_modules()) {
		cout << COLOR_SUCCESS;
		cout << "Succesful loaded all modules" << endl;
		cout << COLOR_RESET;
	}



	try {
		manager.start_module("modul");
	}catch(char* e) {
		cout << e << endl;
	}

	while(true) {
		sleep(1);
	}
	

	return 0;
}