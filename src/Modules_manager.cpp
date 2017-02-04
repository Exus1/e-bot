#include "includes/Modules_manager.hpp"


bool Modules_manager::module_file::loaded() {
	return (dl_handle != NULL)? true : false;
}



Modules_manager::Modules_manager(string modules_folder_path) :
	modules_folder_path(modules_folder_path)
{

}

int Modules_manager::check_module_compatibility(Module_interface* object) {
	if(object->_module_informations.minimum_api_version < ts3_server->api_version) return 1;
	if(object->_module_informations.minimum_ebot_version < version) return 2;
	return 0;
}

void Modules_manager::set_server_object(Ts3Api::Server& server)  {
	this->ts3_server = &server;
}

bool Modules_manager::folder_is_readable() 
{
	return (access(modules_folder_path.c_str(), R_OK) == -1)? false : true;
}

int Modules_manager::list_modules()
{
	struct dirent* file;
	DIR* path;
	string file_name, module_name;

	path = opendir(modules_folder_path.c_str());
	if(path) 
	{
		while(file = readdir(path))
		{
			file_name = file->d_name;

			if(file_name == ".") continue;
			if(file_name == "..") continue;
			if(file_name.find(".emodule") == string::npos) continue;

			module_name = file_name.substr(0, file_name.length()-8);

			modules_files[module_name].file_name = file_name;
		}
		closedir(path);

		return modules_files.size();
	}
	else 
	{
		return -1;
	}
}

void Modules_manager::load_module(string module_name) 
{
	void* dl_handle = NULL;
	typedef Module_interface* (*module_downloader)();
	Module_interface* object_handle;
	ConfigFile object_config_file("Configs/"+module_name+".cfg");


	if(modules_files.find(module_name) == modules_files.end())
		throw "Module does not exist";

	if(modules_files[module_name].loaded())
		throw "Module is already loaded";

	if(modules_folder_path[modules_folder_path.length()-1] != '/')
		modules_folder_path += "/";

	dl_handle = dlopen((modules_folder_path + modules_files[module_name].file_name).c_str(), RTLD_LAZY);

	if(!dl_handle) 
	{
		dlclose(dl_handle);
		throw "Can't load module file";
	}

	module_downloader downloader = (module_downloader)dlsym(dl_handle, "module_downloader");

	if(!downloader) {
		dlclose(dl_handle);
		throw "Can't link downloader function";
	}

	object_handle = downloader();

	if(!object_handle) 
	{
		dlclose(dl_handle);
		throw "Can't download module object";
	}

	int compatibility_value = check_module_compatibility(object_handle);
	if(compatibility_value != 0) {
		float api_version = object_handle->_module_informations.minimum_api_version;
		float ebot_version = object_handle->_module_informations.minimum_ebot_version;
		delete object_handle;
		dlclose(dl_handle);

		if(compatibility_value == 1)
			throw "Module requires newer API version. "+to_string(api_version);
		else if(compatibility_value == 2)
			throw "Module requires newer e-bot version. "+to_string(ebot_version);
	}

	if(!object_config_file.good()) {
		object_config_file.content = object_handle->default_config;
		object_config_file.save();
	}

	modules_files[module_name].dl_handle = dl_handle;
	object_handle->server = ts3_server;
	modules_files[module_name].object_handle = object_handle;

	modules_files[module_name].thd.set_object(object_handle);
}

void Modules_manager::unload_module(string module_name) 
{
	if(modules_files.find(module_name) == modules_files.end())
		throw "Module does not exist";

	if(!modules_files[module_name].loaded())
		return;

	if(modules_files[module_name].thd.runing())
	{
		if(!modules_files[module_name].thd.stop())
			throw "Can't stop module";
	}

	if(modules_files[module_name].object_handle != NULL)
		delete modules_files[module_name].object_handle;

	dlclose(modules_files[module_name].dl_handle);
}

void Modules_manager::restart_module(string module_name) 
{
	if(modules_files.find(module_name) == modules_files.end())
		throw "Module does not exist";

	if(!modules_files[module_name].loaded())
		throw "Module isn't loaded";

	if(!modules_files[module_name].thd.runing())
		throw "Module isn't runing";

	if(modules_files[module_name].thd.stop()) {
		if(modules_files[module_name].thd.start())
			return;

		throw "Can't start module";
	}

	throw "Can't stop module";
}

bool Modules_manager::load_all_modules() {
	bool error = false;
	bool status = false;

	for(auto it = modules_files.begin(); it != modules_files.end(); ++it) {
		try {
			load_module(it->first);
		}catch(char const* e) {
			cout << COLOR_WARNING;
			cout << e << endl;
			cout << COLOR_RESET;
			error = true;
		}

		if(!error) status = true;
	}

	return status;
}

bool Modules_manager::start_module(string module_name) {
	if(modules_files.find(module_name) == modules_files.end())
		throw "Module does not exist";

	if(modules_files[module_name].thd.runing())
		throw "Module is already runing";

	return modules_files[module_name].thd.start();
}

bool Modules_manager::stop_module(string module_name) {
	if(modules_files.find(module_name) == modules_files.end())
		throw "Module does not exist";

	return modules_files[module_name].thd.stop();
}	