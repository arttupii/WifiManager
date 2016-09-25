# WiFiManager
Text based WiFi Manager for ESP8266

Using
```
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include"wifiSetupMenu.h"

void setup(void)
{
  Serial.begin(115200);
  initWifiSetupMenu(); //Initialize library
  Serial.println("Started");
}


void loop(void)
{
  runWifiSetupMenu(); //Run menu loop in loop()
}
```

How It Looks
```
Main Menu

 1.  Scan
 2.  Network config
 4.  Restart/activate
```

```
Network setup

 DHCP is used: true
 Static ip address setup:
    ip: 192.168.1.51
    subnet: 255.255.255.0
    gateway: 192.168.1.1
    dns: 52.53.52.50
 Wifi setup:
    ssid: MiniMe
    password: ******Hidden******

 1. Switch between DHCP/Static ip
 2. Set ip
 3. Set subnet
 4. Set gateway
 5. Set dns
 6. Set wifi ssid
 7. Set wifi password
 9. Set exit/save
```

```
Scan wifi networks...

Found 3 wifi networks...
0. MiniMe WPA -55
1. DonaldDuck WPA -55
2. 655ce8 AUTO -90
```
