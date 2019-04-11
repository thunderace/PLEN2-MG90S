ARDUINO_VARIANT    = d1_mini
SERIAL_PORT = /dev/ttyUSB1
TAG = V0.1
NODENAME = ArDomo7
FLASH_PARTITION=4M1M
GLOBAL_USER_LIBDIR=../libraries
USER_DEFINE= -DMQTT_KEEPALIVE=120 -DMQTT_MAX_PACKET_SIZE=1024 -D_NODENAME_=\"$(NODENAME)\" -DMAX_SSID=2 -D_SSID1_=\"main\" -D_WIFI_PASSWORD1_=\"Delphine_Marie_Quentin_Arnaud\" -D_SSID2_=\"speedtouch\" -D_WIFI_PASSWORD2_=\"Delphine_Marie_Quentin_Arnaud\" -D_SSID3_=\"jardin\" -D_WIFI_PASSWORD3_=\"Delphine_Marie_Quentin_Arnaud\"
include ../../Esp8266-Arduino-Makefile/espXArduino.mk
myota: $(BUILD_OUT)/$(TARGET).bin
	node $(HOME)/repo.git/domo.mqtt/utils/sendOTA.js -n $(NODENAME)  -b $(BUILD_OUT)/$(TARGET).bin


