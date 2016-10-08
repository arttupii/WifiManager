#ifndef WIFISETUPMENU_H
#define WIFISETUPMENU_H
#include "menu.h"

void runWifiSetupMenu();
void initWifiSetupMenu(int eppRomAddress = 0);

class WifiSetupMenu{
	public:
		void run();
		WifiSetupMenu(int eppRomAddress = 0);
        void setCustomMenuCallback(MenuTemplate *(*cb)(MenuTemplate *));
        int usedEeprom();
};

#endif
