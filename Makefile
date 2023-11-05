local:
	cp -Rf ~/Documents/Arduino/libraries/WiFiManager ~/Documents/Arduino/libraries/WiFiManagerBackup
	rm -Rf ~/Documents/Arduino/libraries/WiFiManager
	cp -Rf . ~/Documents/Arduino/libraries/WiFiManager

restore:
	rm -Rf ~/Documents/Arduino/libraries/WiFiManager
	cp -Rf ~/Documents/Arduino/libraries/WiFiManagerBackup ~/Documents/Arduino/libraries/WiFiManager
	rm -Rf ~/Documents/Arduino/libraries/WiFiManagerBackup