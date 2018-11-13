#include "hci0.h"

class LIST_PAGE : public HCI_PAGE{
	using HCI_PAGE::HCI_PAGE;
public: // overrides
	void show(){
		out() << "list config\n";
	}
};

class CONFIG_PAGE : public HCI_PAGE{
	using HCI_PAGE::HCI_PAGE;
public: // overrides
	void show(){
		out() << "configure repository\n";
	}
};

class COMMITS_PAGE : public HCI_PAGE{
	using HCI_PAGE::HCI_PAGE;
public: // overrides
	void show(){
		out() << "list commits\n";
	}
};

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


class REPOSITORY_MENU : public HCI_MENU {
public:
	explicit REPOSITORY_MENU(HCI_APPLICATION& ctx)
	    : HCI_MENU(ctx, "n no,") , _submenu(ctx){
		add(0x1b, &hci_esc);
		add('y', &_submenu);
		add('n', &hci_quit);
	}


public:
	void show(){
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
	SUB_MENU _submenu;
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

private:
	REPOSITORY_MENU _main_menu;
}; // HELLO

// the program starts here.
int main(int argc, char const *argv[])
{
	REPOS repos;
	return repos.exec();
}
