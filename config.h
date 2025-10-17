#ifndef   CONFIG_H
# define  CONFIG_H

# define  LED_PIN   IO4
# define  LED_COUNT 300

const char *hostname = "<esp32_room_XX>";
const char *wifiSsid = "<your_wifi_ssid>";
const char *wifiKey  = "<your_wifi_password>";
const char *mqttHost = "<mqtt_broker_ip>";
const char *mqttUser = "<mqtt_username>";
const char *mqttPass = "<mqtt_password>";

const char *ledsTopic = "/room/XX/leds";
const char *stateTopic = "/devices";

#endif    // CONFIG_H