#ifndef MENU__H_
#define MENU__H_
#include <ESP8266WiFi.h>

class MenuTemplate {
  public:
    MenuTemplate(MenuTemplate *parent);
    MenuTemplate();
    ~MenuTemplate();
    virtual void run();
    virtual void printMenu() {};
    virtual void menuSelect(int c) {};
    virtual int readSerialChar();
    void returnToParentMenu();
    void deleteSubMenu();
    void subMenu(MenuTemplate *subMenuPtr);
  private:
    void printMenuFiltered();
    MenuTemplate *parentPtr;
    MenuTemplate *subMenuPtr;
    bool returnToParentMenuAsked;
    int lastChar;
  protected:
    unsigned long filterTimer;
};

class AskStringMenu: public MenuTemplate {
  public:
    AskStringMenu(MenuTemplate *parent);
    ~AskStringMenu() {}
    void printMenu();

    AskStringMenu * menuText(const char*str);

    virtual void ready(String &str) {};
    void run();
  private:
    const char *text;
    int i;
    String buffer;
};

class Split{
	public:
    Split(String &str, char delimiter){
		this->str=str;
		this->delimiter = delimiter;
	}

	String next() {
		String ret = "";
		int i;
		int l = str.length();
		for(i=0;i<l;i++) {
			if(str[i]==delimiter) break;
			ret+=str[i];			
		}
		String tmp="";
		for(int x=i+1;x<l;x++) {
			tmp+=str[x];	
		}
		str = tmp;
		return ret;
	}

	private:
	String str;
	char delimiter;
};
#endif
