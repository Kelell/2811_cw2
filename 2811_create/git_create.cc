#include <iostream>
#include "gitpp5.h"

int main()
{
	std::string path=".";
	try{
		GITPP::REPO r(path.c_str());
	}catch(GITPP::EXCEPTION_CANT_FIND const&){
		GITPP::REPO r(GITPP::REPO::_create, path.c_str());
	}

	GITPP::REPO r(path.c_str());

	r.commits().create("test created from git_create.cc");

	for(auto i : r.commits()){
		std::cout << i << " " << i.signature().name() << " " << i.message() << "\n";
	}
}
