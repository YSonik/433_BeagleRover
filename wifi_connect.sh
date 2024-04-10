#! /usr/bin/bash

#(1) Obtain root permissions.
echo temppwd | sudo -Sv

#(2) Write the network credentials to the wpa_supplicant.conf file.
W_NAME='ssid="The Thing"'
W_PASS='psk="BananaFritters#1"'
SUPPLICANT="ctrl_interface=/var/run/wpa_supplicant \nctrl_interface_group=0 \nupdate_config=1"
echo -e "$SUPPLICANT\nnetwork={\n$W_NAME\n$W_PASS\n}" | sudo tee /etc/wpa_supplicant/wpa_supplicant.conf >/dev/null

#(3) Add the WiFi interface to the interfaces file.
INTERFACE="auto wlan0 \niface wlan0 inet dhcp \nwpa-driver nl80211 \nwpa-conf /etc/wpa_supplicant/wpa_supplicant.conf"
echo -e "$INTERFACE" | sudo tee /etc/network/interfaces >/dev/null

#(4) Start the wpa_supplicant process.
sudo wpa_supplicant -B -c /etc/wpa_supplicant/wpa_supplicant.conf -i wlan0