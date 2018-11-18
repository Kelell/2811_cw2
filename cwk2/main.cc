#include "hci1.h"
#include "gitpp5.h"
#include <string>
using namespace std;


class LIST_PAGE : public HCI_PAGE{
	using HCI_PAGE::HCI_PAGE;
public: // overrides
	void show(){
		out() << "list config\n\n";
		//list all the configuration values
		GITPP::REPO r;
		auto c=r.config();
		GITPP::CONFIG::ITEM N=c["user.name"];
		out() << "Name: " << N.value() << "\n";
		GITPP::CONFIG::ITEM E=c["user.email"];
		out() << "Your E-mail: " << E.value() << "\n\n";
		GITPP::CONFIG::ITEM R=c["core.repositoryformatversion"];
		out() << "Repository Format Version: " << R.value() << "\n";
		GITPP::CONFIG::ITEM B=c["core.bare"];
		out() << "Bare?: " << B.value() << "\n";
		GITPP::CONFIG::ITEM F=c["core.filemode"];
		out() << "Filemode?: " << F.value() << "\n";
		GITPP::CONFIG::ITEM L=c["core.logallrefupdates"];
		out() << "Log all Ref Updates?: " << L.value() << "\n";
	}
};

class CONFIG_PAGE : public HCI_PAGE{
	using HCI_PAGE::HCI_PAGE;
public: // overrides
	void show(){
		GITPP::REPO r;
		string input;
		auto c=r.config();
		out() << "configure repository\n\n";
		//assign a new name
		out() << "Enter the new name\n\n";
		in() >> input;
		c["user.name"]= input;
		GITPP::CONFIG::ITEM N=c["user.name"];
		out() << "New name is:  " << N.value() << "\n";
		//assign a new e-mail
		out() << "Enter the new e-mail\n\n";
		in() >> input;
		c["user.email"]= input;
		GITPP::CONFIG::ITEM E=c["user.email"];
		out() << "New name is:  " << E.value() << "\n";


	}
};

class COMMITS_PAGE : public HCI_PAGE{
	using HCI_PAGE::HCI_PAGE;
public: // overrides
	void show(){
		out() << "list commits\n\n";
		GITPP::REPO r;
		for(auto i : r.commits()){
			std::cout << i << " " << i.signature().name() << "\n";
		}
	}
};

class REPOS : public HCI_APPLICATION{
public:
	REPOS() : HCI_APPLICATION(), _main_menu(*this) {
	}
public:
	void show(){
		try{
			_main_menu.enter();
		}catch(HCI_ESCAPE const&){
			// somebody pressed esc. exiting.
		}
	}

	class SUB_MENU : public HCI_MENU {
	public:
		explicit SUB_MENU(HCI_APPLICATION& ctx)
		    : HCI_MENU(ctx, "submenu"), _page("list config"),
				_page2("configure repository"), _page3("list commits"){
			add(0x1b, &hci_esc);
			add('c', &_page);
			add('e', &_page2);
			add('l', &_page3);
			add('q', &hci_quit);
		}


	public:
		void show(){
			out() << "Your git repository\n\n";
			HCI_MENU::show();
		}
		void enter(){
			while(true){
				try{
					HCI_MENU::enter();
					break;
				}catch( HCI_UP const&){
					continue;
				}
			}
		}
	private:
		LIST_PAGE _page;
		CONFIG_PAGE _page2;
		COMMITS_PAGE _page3;
	};



private:
	SUB_MENU _main_menu;
}; // HELLO

class HCI_MAKE : public HCI_ACTION{
public:
	HCI_MAKE() : HCI_ACTION("yes"){

	}
private:
	void do_it(){
		std::string path=".";
try{
	GITPP::REPO r(path.c_str());
}catch(GITPP::EXCEPTION_CANT_FIND const&){
	GITPP::REPO r(GITPP::REPO::_create, path.c_str());
}

GITPP::REPO r(path.c_str());

r.commits().create("test created from git_create.cc");
REPOS _mainmenu;
		_mainmenu.exec();
	}
};


class REPOSITORY_MENU : public HCI_MENU {
public:
	explicit REPOSITORY_MENU(HCI_APPLICATION& ctx)
	    : HCI_MENU(ctx, "n no,"){
		add(0x1b, &hci_esc);
		add('y', &_make);
		add('n', &hci_quit);
	}


public:
	void show(){
		out() << "... <title line> ...\n\n";
		out() << "create new empty repository?\n\n";
		HCI_MENU::show();
	}
	void enter(){
		while(true){
			try{
				HCI_MENU::enter();
				break;
			}catch( HCI_UP const&){
				continue;
			}
		}
	}
private:
	HCI_MAKE _make;
};




class INITIAL : public HCI_APPLICATION{
public:
	INITIAL() : HCI_APPLICATION(), _main_menu(*this) {
	}
public:
	void show(){
		try{
			_main_menu.enter();
		}catch(HCI_ESCAPE const&){
			// somebody pressed esc. exiting.
		}
	}

private:
	REPOSITORY_MENU _main_menu;
}; // HELLO

// the program starts here.
int main(int argc, char const *argv[])
{

	try{
		GITPP::REPO r;
	}
	catch(GITPP::EXCEPTION const&){
		INITIAL init;
		return init.exec();
	}
	REPOS repos;
	return repos.exec();
}