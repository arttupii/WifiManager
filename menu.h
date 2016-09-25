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

    virtual void ready(char *str) {};
    void run();
  private:
    const char *text;
    int i;
    char buffer[50];
};
