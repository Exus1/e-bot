#include <Module_interface.hpp>
#include <string>
#include <iostream>

using namespace std;

class New_module : public Module_interface {
public:
	New_module() {
		default_config["connection"]["siema"] = "test";
	}

	void module_loop() {

		auto client = server->getClientByNickname("Exus");

		if(client.good())
			client.poke("TEST");

		sleep(1);
	}
};

extern "C" Module_interface* module_downloader() {
	return new New_module();
}