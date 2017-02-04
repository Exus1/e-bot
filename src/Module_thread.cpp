#include <Module_thread.hpp>

Module_thread::Module_thread() {

}

Module_thread::~Module_thread() {
	if(!stop()) {
		thd.detach();
	}else {
		if(thd.joinable())
			thd.join();
	}

	if(thd.joinable())
		thd.join();
}

void Module_thread::set_object(Module_interface* module_object) {
	this->module_object = module_object;
}

bool Module_thread::good() {
	return (module_object != NULL)? true : false;
}

bool Module_thread::runing() {
	return (status < (time(NULL)+300))? false : true;
}


bool Module_thread::start() {
	if(!good()) return false;

	if(module_object->server == NULL) return false;

	thd = thread([this] { loop(); });

	sleep(1);

	return (runing_status)? true : false;
}

bool Module_thread::stop() {
	if(!good()) return true;


	break_loop = true;

	for(int i = 0; i < 10; i++) {
		if(!runing_status) return true;
		sleep(1);
	}

	return false;
}

void Module_thread::loop() {
	if(!good()) return;

	runing_status = true;

	while(true) {
		module_object->module_loop();

		if(break_loop) {
			runing_status = false;
			return;
		}
	}
}
