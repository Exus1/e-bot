#ifndef _MODULE_THREAD__HPP
#define _MODULE_THREAD__HPP

#include <Module_interface.hpp>
#include <thread>
#include <string>

using namespace std;

class Module_thread {
	thread thd;

	Module_interface* module_object = NULL;

	time_t status;

	bool runing_status = false;

	bool break_loop = false;

public:

	Module_thread();

	~Module_thread();

	void set_object(Module_interface* module_object);

	bool good();

	bool runing();

	bool start();

	bool stop();

	void loop();

};


#endif