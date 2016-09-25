#include"wifiSetupMenu.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "menu.h"
#include <EEPROM.h>

void updateEPPROM();

  const char* encryptionTypeStr(uint8_t authmode) {
    switch(authmode) {
        case ENC_TYPE_NONE:
            return "NONE";
        case ENC_TYPE_WEP:
            return "WEP";
        case ENC_TYPE_TKIP:
            return "TKIP";
        case ENC_TYPE_CCMP:
            return "WPA";
        case ENC_TYPE_AUTO:
            return "AUTO";
        default:
            return "?";
    }
  }

  int scanWifis() {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    int n = WiFi.scanNetworks();
    Serial.printf("Found %d wifi networks...\n", n);
    for(int i=0;i<n;i++) {
        Serial.printf("%d. ", i);

        if(WiFi.isHidden(i)) {
          Serial.print("[Hidden]");
        } else {
          Serial.print(WiFi.SSID(i));
        }

        Serial.printf(" %s %d\n", encryptionTypeStr(WiFi.encryptionType(i)), WiFi.RSSI(i));
    }
    return n;
  }

struct networkSetup {
  bool dhcp;
  char ip[4];
  char gateway[4];
  char subnet[4];
  char dns[4];

  char ssid[20];
  char pass[50];
};
struct networkSetup networkSetup;


class WifiScanMenu: public MenuTemplate {
  public:
    WifiScanMenu(MenuTemplate *parent): MenuTemplate(parent){}
    void printMenu() {
      Serial.println("Scan wifi networks...\n");
      scanWifis();
      returnToParentMenu();
    }

};


class SetIpMenu: public AskStringMenu {
  public:
    SetIpMenu(MenuTemplate *parent, char *to): AskStringMenu(parent){
      this->to = to;
    }
    void ready(char *str) {
      char* command = strtok(str, ".");
      int i=3;
      while (command != 0)
      {
          Serial.println(command);
          if(i>=0) {
            to[i]=atoi(command);
            i--;
          }
          command = strtok(0, ".");
      }
    }
    SetIpMenu *toBuffer(char *ptr) {
      to = ptr;
      return this;
    }
  private:
    char *to;
};

class AskStrMenu: public AskStringMenu {
  public:
    AskStrMenu(MenuTemplate *parent, char *to): AskStringMenu(parent){
      this->to = to;
    }
    void ready(char *str) {
      strcpy(to,str);
    }
    AskStrMenu *toBuffer(char *ptr) {
      to = ptr;
      return this;
    }
  private:
    char *to;
};



class NetworkMenu: public MenuTemplate {
  public:
    NetworkMenu(MenuTemplate *parent): MenuTemplate(parent){}
    void printMenu() {
      Serial.println("Network setup\n");

      Serial.printf(" DHCP is used: %s\n", networkSetup.dhcp?"true":"false");

      Serial.println(" Static ip address setup:");
      Serial.printf("    ip: %u.%u.%u.%u\n", networkSetup.ip[3],networkSetup.ip[2],networkSetup.ip[1],networkSetup.ip[0]);
      Serial.printf("    subnet: %u.%u.%u.%u\n", networkSetup.subnet[3],networkSetup.subnet[2],networkSetup.subnet[1],networkSetup.subnet[0]);
      Serial.printf("    gateway: %u.%u.%u.%u\n", networkSetup.gateway[3],networkSetup.gateway[2],networkSetup.gateway[1],networkSetup.gateway[0]);
      Serial.printf("    dns: %u.%u.%u.%u\n", networkSetup.dns[3],networkSetup.dns[2],networkSetup.dns[1],networkSetup.dns[0]);
      Serial.printf(" Wifi setup:\n");
      Serial.printf("    ssid: %s\n", networkSetup.ssid);
      Serial.printf("    password: %s\n\n", "******Hidden******");

      Serial.println(" 1. Switch between DHCP/Static ip");
      Serial.println(" 2. Set ip");
      Serial.println(" 3. Set subnet");
      Serial.println(" 4. Set gateway");
      Serial.println(" 5. Set dns");
      Serial.println(" 6. Set wifi ssid");
      Serial.println(" 7. Set wifi password");
      Serial.println(" 9. Set exit/save");
    }
    void menuSelect(int c){

      switch(c) {
        case '1': networkSetup.dhcp = !networkSetup.dhcp; return;
        case '2': subMenu((new SetIpMenu(this,networkSetup.ip))->menuText(" ip: ")); return;
        case '3': subMenu((new SetIpMenu(this,networkSetup.subnet))->menuText(" subnet: ")); return;
        case '4': subMenu((new SetIpMenu(this,networkSetup.gateway))->menuText(" gateway: ")); return;
        case '5': subMenu((new SetIpMenu(this,networkSetup.dns))->menuText(" dns: ")); return;

        case '6': subMenu((new AskStrMenu(this,networkSetup.ssid))->menuText(" ssid: ")); return;
        case '7': subMenu((new AskStrMenu(this,networkSetup.pass))->menuText(" password: ")); return;

        case '9': updateEPPROM(); returnToParentMenu(); return;
      }
    }
};

IPAddress toIpAddress(char *t) {
  return IPAddress(t[3],t[2],t[1],t[0]);
}

class WifiConnectMenu: public MenuTemplate {
  public:
    WifiConnectMenu(MenuTemplate *parent): MenuTemplate(parent){

      if(networkSetup.dhcp) {
        WiFi.config(IPAddress(0,0,0,0),IPAddress(0,0,0,0),IPAddress(0,0,0,0),IPAddress(0,0,0,0),IPAddress(0,0,0,0));
      } else {
        WiFi.config(toIpAddress(networkSetup.ip), toIpAddress(networkSetup.dns), toIpAddress(networkSetup.gateway), toIpAddress(networkSetup.subnet));
      }
      WiFi.mode(WIFI_STA);
      WiFi.disconnect();
      WiFi.scanNetworks();
      WiFi.begin(networkSetup.ssid, networkSetup.pass);
    }

    void run(){
      if(abs(millis()-filterTimer)>3000) {
        filterTimer = millis();
        int status = WiFi.status();
        if(status ==  WL_CONNECTED) {
          Serial.print("Connected. ip=");
          Serial.println(WiFi.localIP());
          returnToParentMenu();
        } else {
          Serial.printf("Polling... (stop pressing x) -->%d\n",  status);
        }
      }
      if(Serial.read()=='x') {
        returnToParentMenu();
      }

    }
  };


class MainMenu: public MenuTemplate {
  public:
    void printMenu() {
      if(WiFi.status() ==  WL_CONNECTED) {
        Serial.printf("\nConnected to %s [", networkSetup.ssid);
        Serial.print(WiFi.localIP());
        Serial.println("]");
      } else {
        Serial.println("Not Connected..");
      }

      Serial.println("\nMain Menu\n");
      Serial.println(" 1.  Scan");
      Serial.println(" 2.  Network config");
      Serial.println(" 4.  Restart");
    }
    void menuSelect(int c){
      switch(c) {
        case '1': subMenu(new WifiScanMenu(this)); return;
        case '2': subMenu(new NetworkMenu(this)); return;
        case '4': ESP.restart(); return;//subMenu(new WifiConnectMenu(this)); return;
      }
    }
};

MainMenu menu;

int eppAddr;
void updateEPPROM() {
  bool commmitNeeded = false;
  EEPROM.put(eppAddr,networkSetup);
  EEPROM.commit(); //note here the commit!
}

void initWifiSetupMenu(int eppRomAddress){
  eppAddr = eppRomAddress;
  //struct networkSetup networkSetup;
  char *ptr = (char*)&networkSetup;
  EEPROM.begin(512);
  EEPROM.get(eppAddr,networkSetup);
  WiFi.begin(networkSetup.ssid, networkSetup.pass);
}

void runWifiSetupMenu() {
  menu.run();
}
