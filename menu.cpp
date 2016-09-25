#include "menu.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#define NULL 0

MenuTemplate::MenuTemplate(MenuTemplate *parent) {
  this->parentPtr = parent;
  subMenuPtr = NULL;
  returnToParentMenuAsked = false;
  lastChar = -1;
  filterTimer = millis();
  printMenu();
}

MenuTemplate::MenuTemplate() {
  this->parentPtr = NULL;
  subMenuPtr = NULL;
  returnToParentMenuAsked = false;
  lastChar = -1;
  filterTimer = millis();
  printMenu();
}



MenuTemplate::~MenuTemplate() {
  deleteSubMenu();
}

void MenuTemplate::printMenuFiltered() {
  if (abs(millis() - filterTimer) < 1000) return;
  filterTimer = millis();
  printMenu();
}

int MenuTemplate::readSerialChar() {
  return Serial.read();
}

void MenuTemplate::run() {
  if (returnToParentMenuAsked) {
    returnToParentMenuAsked = false;
    filterTimer = millis();
    deleteSubMenu();
    printMenu();
  }

  if (subMenuPtr) {
    subMenuPtr->run();
    return;
  }
  int c = readSerialChar();
  if (c == '\n') {
    printMenuFiltered();
  } else {
    if (c != '\r' && c != -1) {
      menuSelect(c);
    }
  }
};


void MenuTemplate::returnToParentMenu() {
  if (parentPtr) {
    parentPtr->returnToParentMenuAsked = true;
  }
}
void MenuTemplate::deleteSubMenu() {
  if (subMenuPtr) {
    delete subMenuPtr;
    subMenuPtr = NULL;
  }
}
void MenuTemplate::subMenu(MenuTemplate *subMenuPtr) {
  this->subMenuPtr = subMenuPtr;
  subMenuPtr->printMenu();
}

/*********************************************************************************/


AskStringMenu::AskStringMenu(MenuTemplate *parent): MenuTemplate(parent) {
  i = 0;

}
void AskStringMenu::printMenu() {
  Serial.printf("%s\n", text);
}

AskStringMenu * AskStringMenu::menuText(const char*str) {
  text = str;
  return this;
}


void AskStringMenu::run() {
  int c = Serial.read();
  if (c != -1) {
    if (c == 8) {
      i--;
      if (i <= 0) {
        i = 0;
        return;
      }
    } if (c == '\n' || c == '\r') {
      if (abs(millis() - filterTimer) < 1000) return;
      buffer[i] = 0;
      ready(buffer);
      returnToParentMenu();
    } else {
      buffer[i] = c;
      i++;
      if (i >= sizeof(buffer) - 1) {
        i -= 1;
        return;
      }
    }
    Serial.print((char)c);
  }

}
