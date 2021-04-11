#include "AmsConfiguration.h"

bool AmsConfiguration::getSystemConfig(SystemConfig& config) {
	if(hasConfig()) {
		EEPROM.begin(EEPROM_SIZE);
		EEPROM.get(CONFIG_SYSTEM_START, config);
		EEPROM.end();
		return true;
	} else {
		return false;
	}
}

bool AmsConfiguration::setSystemConfig(SystemConfig& config) {
	EEPROM.begin(EEPROM_SIZE);
	EEPROM.put(CONFIG_SYSTEM_START, config);
	bool ret = EEPROM.commit();
	EEPROM.end();
	return ret;
}

bool AmsConfiguration::getWiFiConfig(WiFiConfig& config) {
	if(hasConfig()) {
		EEPROM.begin(EEPROM_SIZE);
		EEPROM.get(CONFIG_WIFI_START, config);
		EEPROM.end();
		return true;
	} else {
		clearWifi(config);
		return false;
	}
}

bool AmsConfiguration::setWiFiConfig(WiFiConfig& config) {
	WiFiConfig existing;
	if(getWiFiConfig(existing)) {
		wifiChanged |= strcmp(config.ssid, existing.ssid) != 0;
		wifiChanged |= strcmp(config.psk, existing.psk) != 0;
		wifiChanged |= strcmp(config.ip, existing.ip) != 0;
		wifiChanged |= strcmp(config.gateway, existing.gateway) != 0;
		wifiChanged |= strcmp(config.subnet, existing.subnet) != 0;
		wifiChanged |= strcmp(config.dns1, existing.dns1) != 0;
		wifiChanged |= strcmp(config.dns2, existing.dns2) != 0;
		wifiChanged |= strcmp(config.hostname, existing.hostname) != 0;
	} else {
		wifiChanged = true;
	}
	EEPROM.begin(EEPROM_SIZE);
	EEPROM.put(CONFIG_WIFI_START, config);
	bool ret = EEPROM.commit();
	EEPROM.end();
	return ret;
}

void AmsConfiguration::clearWifi(WiFiConfig& config) {
	strcpy(config.ssid, "");
	strcpy(config.psk, "");
	clearWifiIp(config);

	uint16_t chipId;
	#if defined(ESP32)
		chipId = ESP.getEfuseMac();
	#else
		chipId = ESP.getChipId();
	#endif
	strcpy(config.hostname, (String("ams-") + String(chipId, HEX)).c_str());
	config.mdns = true;
}

void AmsConfiguration::clearWifiIp(WiFiConfig& config) {
	strcpy(config.ip, "");
	strcpy(config.gateway, "");
	strcpy(config.subnet, "");
	strcpy(config.dns1, "");
	strcpy(config.dns2, "");
}

bool AmsConfiguration::isWifiChanged() {
	return wifiChanged;
}

void AmsConfiguration::ackWifiChange() {
	wifiChanged = false;
}

bool AmsConfiguration::getMqttConfig(MqttConfig& config) {
	if(hasConfig()) {
		EEPROM.begin(EEPROM_SIZE);
		EEPROM.get(CONFIG_MQTT_START, config);
		EEPROM.end();
		return true;
	} else {
		clearMqtt(config);
		return false;
	}
}

bool AmsConfiguration::setMqttConfig(MqttConfig& config) {
	MqttConfig existing;
	if(getMqttConfig(existing)) {
		mqttChanged |= strcmp(config.host, existing.host) != 0;
		mqttChanged |= config.port != existing.port;
		mqttChanged |= strcmp(config.clientId, existing.clientId) != 0;
		mqttChanged |= strcmp(config.publishTopic, existing.publishTopic) != 0;
		mqttChanged |= strcmp(config.subscribeTopic, existing.subscribeTopic) != 0;
		mqttChanged |= strcmp(config.username, existing.username) != 0;
		mqttChanged |= strcmp(config.password, existing.password) != 0;
		mqttChanged |= config.payloadFormat != existing.payloadFormat;
		mqttChanged |= config.ssl != existing.ssl;
	} else {
		mqttChanged = true;
	}
	EEPROM.begin(EEPROM_SIZE);
	EEPROM.put(CONFIG_MQTT_START, config);
	bool ret = EEPROM.commit();
	EEPROM.end();
	return ret;
}

void AmsConfiguration::clearMqtt(MqttConfig& config) {
	strcpy(config.host, "");
	config.port = 1883;
	strcpy(config.clientId, "");
	strcpy(config.publishTopic, "");
	strcpy(config.subscribeTopic, "");
	strcpy(config.username, "");
	strcpy(config.password, "");
	config.payloadFormat = 0;
	config.ssl = false;
}

void AmsConfiguration::setMqttChanged() {
	mqttChanged = true;
}

bool AmsConfiguration::isMqttChanged() {
	return mqttChanged;
}

void AmsConfiguration::ackMqttChange() {
	mqttChanged = false;
}

bool AmsConfiguration::getWebConfig(WebConfig& config) {
	if(hasConfig()) {
		EEPROM.begin(EEPROM_SIZE);
		EEPROM.get(CONFIG_WEB_START, config);
		EEPROM.end();
		return true;
	} else {
		clearAuth(config);
		return false;
	}
}

bool AmsConfiguration::setWebConfig(WebConfig& config) {
	EEPROM.begin(EEPROM_SIZE);
	EEPROM.put(CONFIG_WEB_START, config);
	bool ret = EEPROM.commit();
	EEPROM.end();
	return ret;
}

void AmsConfiguration::clearAuth(WebConfig& config) {
	config.security = 0;
	strcpy(config.username, "");
	strcpy(config.password, "");
}

bool AmsConfiguration::getMeterConfig(MeterConfig& config) {
	if(hasConfig()) {
		EEPROM.begin(EEPROM_SIZE);
		EEPROM.get(CONFIG_METER_START, config);
		EEPROM.end();
		return true;
	} else {
		clearMeter(config);
		return false;
	}
}

bool AmsConfiguration::setMeterConfig(MeterConfig& config) {
	MeterConfig existing;
	if(getMeterConfig(existing)) {
		meterChanged |= config.type != existing.type;
		meterChanged |= strcmp((char*) config.encryptionKey, (char*) existing.encryptionKey);
		meterChanged |= strcmp((char*) config.authenticationKey, (char*) existing.authenticationKey);
	} else {
		meterChanged = true;
	}
	EEPROM.begin(EEPROM_SIZE);
	EEPROM.put(CONFIG_METER_START, config);
	bool ret = EEPROM.commit();
	EEPROM.end();
	return ret;
}

void AmsConfiguration::clearMeter(MeterConfig& config) {
	config.type = 0;
	config.distributionSystem = 0;
	config.mainFuse = 0;
	config.productionCapacity = 0;
	memset(config.encryptionKey, 0, 16);
	memset(config.authenticationKey, 0, 16);
	config.substituteMissing = false;
}

bool AmsConfiguration::isMeterChanged() {
	return meterChanged;
}

void AmsConfiguration::ackMeterChanged() {
	meterChanged = false;
}

bool AmsConfiguration::getDebugConfig(DebugConfig& config) {
	if(hasConfig()) {
		EEPROM.begin(EEPROM_SIZE);
		EEPROM.get(CONFIG_DEBUG_START, config);
		EEPROM.end();
		return true;
	} else {
		return false;
	}
}

bool AmsConfiguration::setDebugConfig(DebugConfig& config) {
	EEPROM.begin(EEPROM_SIZE);
	EEPROM.put(CONFIG_DEBUG_START, config);
	bool ret = EEPROM.commit();
	EEPROM.end();
	return ret;
}

bool AmsConfiguration::getDomoticzConfig(DomoticzConfig& config) {
	if(hasConfig()) {
		EEPROM.begin(EEPROM_SIZE);
		EEPROM.get(CONFIG_DOMOTICZ_START, config);
		EEPROM.end();
		return true;
	} else {
		clearDomo(config);
		return false;
	}
}

bool AmsConfiguration::setDomoticzConfig(DomoticzConfig& config) {
	DomoticzConfig existing;
	if(getDomoticzConfig(existing)) {
		domoChanged |= config.elidx != existing.elidx;
		domoChanged |= config.vl1idx != existing.vl1idx;
		domoChanged |= config.vl2idx != existing.vl2idx;
		domoChanged |= config.vl3idx != existing.vl3idx;
		domoChanged |= config.cl1idx != existing.cl1idx;
	} else {
		domoChanged = true;
	}
	mqttChanged = domoChanged;
	EEPROM.begin(EEPROM_SIZE);
	EEPROM.put(CONFIG_DOMOTICZ_START, config);
	bool ret = EEPROM.commit();
	EEPROM.end();
	return ret;
}

void AmsConfiguration::clearDomo(DomoticzConfig& config) {
	config.elidx = 0;
	config.vl1idx = 0;
	config.vl2idx = 0;
	config.vl3idx = 0;
	config.cl1idx = 0;
}

bool AmsConfiguration::isDomoChanged() {
	return domoChanged;
}

void AmsConfiguration::ackDomoChange() {
	domoChanged = false;
}

bool AmsConfiguration::pinUsed(uint8_t pin, GpioConfig& config) {
	if(pin == 0xFF)
		return false;
	return 
		pin == config.hanPin ||
		pin == config.apPin ||
		pin == config.ledPin ||
		pin == config.ledPinRed ||
		pin == config.ledPinGreen ||
		pin == config.ledPinBlue ||
		pin == config.tempSensorPin ||
		pin == config.tempAnalogSensorPin ||
		pin == config.vccPin
	;
}

bool AmsConfiguration::getGpioConfig(GpioConfig& config) {
	if(hasConfig()) {
		EEPROM.begin(EEPROM_SIZE);
		EEPROM.get(CONFIG_GPIO_START, config);
		EEPROM.end();
		return true;
	} else {
		clearGpio(config);
		return false;
	}
}

bool AmsConfiguration::setGpioConfig(GpioConfig& config) {
	GpioConfig existing;
	if(getGpioConfig(existing)) {
		meterChanged |= config.hanPin != existing.hanPin;
	}
	/* This currently does not work, as it checks its own pin
	if(pinUsed(config.hanPin, config)) {
		Serial.println("HAN pin already used");
		return false;
	}
	if(pinUsed(config.apPin, config)) {
		Serial.println("AP pin already used");
		return false;
	}
	if(pinUsed(config.ledPin, config)) {
		Serial.println("LED pin already used");
		return false;
	}
	if(pinUsed(config.ledPinRed, config)) {
		Serial.println("LED RED pin already used");
		return false;
	}
	if(pinUsed(config.ledPinGreen, config)) {
		Serial.println("LED GREEN pin already used");
		return false;
	}
	if(pinUsed(config.ledPinBlue, config)) {
		Serial.println("LED BLUE pin already used");
		return false;
	}
	if(pinUsed(config.tempSensorPin, config)) {
		Serial.println("Temp sensor pin already used");
		return false;
	}
	if(pinUsed(config.tempAnalogSensorPin, config)) {
		Serial.println("Analog temp sensor pin already used");
		return false;
	}
	if(pinUsed(config.vccPin, config)) {
		Serial.println("Vcc pin already used");
		return false;
	}
	*/
	if(config.apPin >= 0)
		pinMode(config.apPin, INPUT_PULLUP);

	EEPROM.begin(EEPROM_SIZE);
	EEPROM.put(CONFIG_GPIO_START, config);
	bool ret = EEPROM.commit();
	EEPROM.end();
	return ret;
}

void AmsConfiguration::clearGpio(GpioConfig& config) {
	config.hanPin = 3;
	config.apPin = 0xFF;
	config.ledPin = 0xFF;
	config.ledInverted = true;
	config.ledPinRed = 0xFF;
	config.ledPinGreen = 0xFF;
	config.ledPinBlue = 0xFF;
	config.ledRgbInverted = true;
	config.tempSensorPin = 0xFF;
	config.tempAnalogSensorPin = 0xFF;
	config.vccPin = 0xFF;
	config.vccOffset = 0;
	config.vccMultiplier = 1000;
	config.vccBootLimit = 0;
}

bool AmsConfiguration::getNtpConfig(NtpConfig& config) {
	if(hasConfig()) {
		EEPROM.begin(EEPROM_SIZE);
		EEPROM.get(CONFIG_NTP_START, config);
		EEPROM.end();
		return true;
	} else {
		clearNtp(config);
		return false;
	}
}

bool AmsConfiguration::setNtpConfig(NtpConfig& config) {
	NtpConfig existing;
	if(getNtpConfig(existing)) {
		if(config.enable != existing.enable) {
			if(!existing.enable) {
				wifiChanged = true;
			} else {
				ntpChanged = true;
			}
		}
		ntpChanged |= config.dhcp != existing.dhcp;
		ntpChanged |= config.offset != existing.offset;
		ntpChanged |= config.summerOffset != existing.summerOffset;
		ntpChanged |= strcmp(config.server, existing.server) != 0;
	} else {
		ntpChanged = true;
	}
	EEPROM.begin(EEPROM_SIZE);
	EEPROM.put(CONFIG_NTP_START, config);
	bool ret = EEPROM.commit();
	EEPROM.end();
	return ret;
}

bool AmsConfiguration::isNtpChanged() {
	return ntpChanged;
}

void AmsConfiguration::ackNtpChange() {
	ntpChanged = false;
}

void AmsConfiguration::clearNtp(NtpConfig& config) {
	config.enable = true;
	config.dhcp = true;
	config.offset = 360;
	config.summerOffset = 360;
	strcpy(config.server, "pool.ntp.org");
}

bool AmsConfiguration::getEntsoeConfig(EntsoeConfig& config) {
	if(hasConfig()) {
		EEPROM.begin(EEPROM_SIZE);
		EEPROM.get(CONFIG_ENTSOE_START, config);
		EEPROM.end();
		return true;
	} else {
		return false;
	}
}

bool AmsConfiguration::setEntsoeConfig(EntsoeConfig& config) {
	EntsoeConfig existing;
	if(getEntsoeConfig(existing)) {
		entsoeChanged |= strcmp(config.token, existing.token) != 0;
		entsoeChanged |= strcmp(config.area, existing.area) != 0;
		entsoeChanged |= strcmp(config.currency, existing.currency) != 0;
		entsoeChanged |= config.multiplier != existing.multiplier;
	} else {
		entsoeChanged = true;
	}
	EEPROM.begin(EEPROM_SIZE);
	EEPROM.put(CONFIG_ENTSOE_START, config);
	bool ret = EEPROM.commit();
	EEPROM.end();
	return ret;
}

void AmsConfiguration::clearEntsoe(EntsoeConfig& config) {
	strcpy(config.token, "");
	strcpy(config.area, "");
	strcpy(config.currency, "");
	config.multiplier = 1000;
}

bool AmsConfiguration::isEntsoeChanged() {
	return entsoeChanged;
}

void AmsConfiguration::ackEntsoeChange() {
	entsoeChanged = false;
}

void AmsConfiguration::clear() {
	EEPROM.begin(EEPROM_SIZE);
	MeterConfig meter;
	clearMeter(meter);
	EEPROM.put(CONFIG_METER_START, meter);

	WiFiConfig wifi;
	clearWifi(wifi);
	EEPROM.put(CONFIG_WIFI_START, wifi);

	MqttConfig mqtt;
	clearMqtt(mqtt);
	EEPROM.put(CONFIG_MQTT_START, mqtt);

	WebConfig web;
	clearAuth(web);
	EEPROM.put(CONFIG_WEB_START, web);

	DomoticzConfig domo;
	clearDomo(domo);
	EEPROM.put(CONFIG_DOMOTICZ_START, domo);

	NtpConfig ntp;
	clearNtp(ntp);
	EEPROM.put(CONFIG_NTP_START, domo);

	EntsoeConfig entsoe;
	clearEntsoe(entsoe);
	EEPROM.put(CONFIG_ENTSOE_START, domo);

	EEPROM.put(EEPROM_CONFIG_ADDRESS, -1);
	EEPROM.commit();
	EEPROM.end();
}

bool AmsConfiguration::hasConfig() {
	if(configVersion == 0) {
		EEPROM.begin(EEPROM_SIZE);
		configVersion = EEPROM.read(EEPROM_CONFIG_ADDRESS);
		EEPROM.end();
	}
	switch(configVersion) {
		case 82:
			configVersion = -1; // Prevent loop
			if(loadConfig82(EEPROM_CONFIG_ADDRESS+1)) {
				configVersion = EEPROM_CHECK_SUM;
				return true;
			} else {
				configVersion = 0;
				return false;
			}
			break;
		case 83:
			configVersion = -1; // Prevent loop
			if(loadConfig83(EEPROM_CONFIG_ADDRESS+1)) {
				configVersion = EEPROM_CHECK_SUM;
				return true;
			} else {
				configVersion = 0;
				return false;
			}
			break;
		case EEPROM_CHECK_SUM:
			return true;
		default:
			configVersion = 0;
			return false;
	}
}

int AmsConfiguration::getConfigVersion() {
	return configVersion;
}

void AmsConfiguration::loadTempSensors() {
	EEPROM.begin(EEPROM_SIZE);
	TempSensorConfig* tempSensors[32];
	int address = EEPROM_TEMP_CONFIG_ADDRESS;
	int c = 0;
	int storedCount = EEPROM.read(address++);
	Serial.print("Sensors: ");
	Serial.println(storedCount);
	if(storedCount > 0 && storedCount <= 32) {
		for(int i = 0; i < storedCount; i++) {
			TempSensorConfig* tsc = new TempSensorConfig();
			EEPROM.get(address, *tsc);
			if(tsc->address[0] != 0xFF) {
				tempSensors[c++] = tsc;
			}
			address += sizeof(*tsc);
		}
	}
	this->tempSensors = new TempSensorConfig*[c];
	for(int i = 0; i < c; i++) {
		this->tempSensors[i] = tempSensors[i];
	}
	tempSensorCount = c;
	EEPROM.end();
}

void AmsConfiguration::saveTempSensors() {
	int address = EEPROM_TEMP_CONFIG_ADDRESS;
	EEPROM.put(address++, tempSensorCount);
	for(int i = 0; i < tempSensorCount; i++) {
		TempSensorConfig* tsc = tempSensors[i];
		if(tsc->address[0] != 0xFF) {
			EEPROM.put(address, *tsc);
			address += sizeof(*tsc);
		}
	}
}

bool AmsConfiguration::loadConfig82(int address) {
	ConfigObject82 c;
	EEPROM.begin(EEPROM_SIZE);
	EEPROM.get(address, c);

	EntsoeConfig entsoe;
	clearEntsoe(entsoe);
	EEPROM.put(CONFIG_ENTSOE_START, entsoe);

	NtpConfig ntp;
	clearNtp(ntp);
	EEPROM.put(CONFIG_NTP_START, ntp);

	DomoticzConfig domo {
		c.domoELIDX,
		c.domoVL1IDX,
		c.domoVL2IDX,
		c.domoVL3IDX,
		c.domoCL1IDX
	};
	EEPROM.put(CONFIG_DOMOTICZ_START, domo);

	GpioConfig gpio {
		c.hanPin,
		c.apPin,
		c.ledPin,
		c.ledInverted,
		c.ledPinRed,
		c.ledPinGreen,
		c.ledPinBlue,
		c.ledRgbInverted,
		c.tempSensorPin,
		0xFF,
		c.vccPin,
		0,
		c.vccMultiplier,
		c.vccBootLimit
	};
	EEPROM.put(CONFIG_GPIO_START, gpio);

	DebugConfig debug {
		c.debugTelnet,
		c.debugSerial,
		c.debugLevel
	};
	EEPROM.put(CONFIG_DEBUG_START, debug);

	MeterConfig meter {
		c.meterType,
		c.distributionSystem,
		c.mainFuse,
		c.productionCapacity,
		{0},
		{0},
		c.substituteMissing
	};
	EEPROM.put(CONFIG_METER_START, meter);

	WebConfig web {
		c.authSecurity
	};
	strcpy(web.username, c.authUser);
	strcpy(web.password, c.authPassword);
	EEPROM.put(CONFIG_WEB_START, web);

	MqttConfig mqtt;
	strcpy(mqtt.host, c.mqttHost);
	mqtt.port = c.mqttPort;
	strcpy(mqtt.clientId, c.mqttClientId);
	strcpy(mqtt.publishTopic, c.mqttPublishTopic);
	strcpy(mqtt.subscribeTopic, c.mqttSubscribeTopic);
	strcpy(mqtt.username, c.mqttUser);
	strcpy(mqtt.password, c.mqttPassword);
	mqtt.payloadFormat = c.mqttPayloadFormat;
	mqtt.ssl = c.mqttSsl;
	EEPROM.put(CONFIG_MQTT_START, mqtt);

	WiFiConfig wifi;
	strcpy(wifi.ssid, c.wifiSsid);
	strcpy(wifi.psk, c.wifiPassword);
    strcpy(wifi.ip, c.wifiIp);
    strcpy(wifi.gateway, c.wifiGw);
    strcpy(wifi.subnet, c.wifiSubnet);
	strcpy(wifi.dns1, c.wifiDns1);
	strcpy(wifi.dns2, c.wifiDns2);
	strcpy(wifi.hostname, c.wifiHostname);
	wifi.mdns = true;
	EEPROM.put(CONFIG_WIFI_START, wifi);

	SystemConfig sys  {
		c.boardType
	};
	EEPROM.put(CONFIG_SYSTEM_START, sys);

	EEPROM.put(EEPROM_CONFIG_ADDRESS, EEPROM_CHECK_SUM);
	bool ret = EEPROM.commit();
	EEPROM.end();

	return ret;
}

bool AmsConfiguration::loadConfig83(int address) {
	ConfigObject83 c;
	EEPROM.begin(EEPROM_SIZE);
	EEPROM.get(address, c);

	EntsoeConfig entsoe {"", "", "", 1000};
	EEPROM.put(CONFIG_ENTSOE_START, entsoe);

	NtpConfig ntp {
		c.ntpEnable,
		c.ntpDhcp,
		c.ntpOffset,
		c.ntpSummerOffset
	};
	strcpy(ntp.server, c.ntpServer);
	EEPROM.put(CONFIG_NTP_START, ntp);

	DomoticzConfig domo {
		c.domoELIDX,
		c.domoVL1IDX,
		c.domoVL2IDX,
		c.domoVL3IDX,
		c.domoCL1IDX
	};
	EEPROM.put(CONFIG_DOMOTICZ_START, domo);

	GpioConfig gpio {
		c.hanPin,
		c.apPin,
		c.ledPin,
		c.ledInverted,
		c.ledPinRed,
		c.ledPinGreen,
		c.ledPinBlue,
		c.ledRgbInverted,
		c.tempSensorPin,
		c.tempAnalogSensorPin,
		c.vccPin,
		c.vccOffset,
		c.vccMultiplier,
		c.vccBootLimit
	};
	EEPROM.put(CONFIG_GPIO_START, gpio);

	DebugConfig debug {
		c.debugTelnet,
		c.debugSerial,
		c.debugLevel
	};
	EEPROM.put(CONFIG_DEBUG_START, debug);

	MeterConfig meter {
		c.meterType,
		c.distributionSystem,
		c.mainFuse,
		c.productionCapacity,
		{0},
		{0},
		c.substituteMissing
	};
	memcpy(meter.encryptionKey, c.meterEncryptionKey, 16);
	memcpy(meter.authenticationKey, c.meterAuthenticationKey, 16);
	EEPROM.put(CONFIG_METER_START, meter);

	WebConfig web {
		c.authSecurity
	};
	strcpy(web.username, c.authUser);
	strcpy(web.password, c.authPassword);
	EEPROM.put(CONFIG_WEB_START, web);

	MqttConfig mqtt;
	strcpy(mqtt.host, c.mqttHost);
	mqtt.port = c.mqttPort;
	strcpy(mqtt.clientId, c.mqttClientId);
	strcpy(mqtt.publishTopic, c.mqttPublishTopic);
	strcpy(mqtt.subscribeTopic, c.mqttSubscribeTopic);
	strcpy(mqtt.username, c.mqttUser);
	strcpy(mqtt.password, c.mqttPassword);
	mqtt.payloadFormat = c.mqttPayloadFormat;
	mqtt.ssl = c.mqttSsl;
	EEPROM.put(CONFIG_MQTT_START, mqtt);

	WiFiConfig wifi;
	strcpy(wifi.ssid, c.wifiSsid);
	strcpy(wifi.psk, c.wifiPassword);
    strcpy(wifi.ip, c.wifiIp);
    strcpy(wifi.gateway, c.wifiGw);
    strcpy(wifi.subnet, c.wifiSubnet);
	strcpy(wifi.dns1, c.wifiDns1);
	strcpy(wifi.dns2, c.wifiDns2);
	strcpy(wifi.hostname, c.wifiHostname);
	wifi.mdns = c.mDnsEnable;
	EEPROM.put(CONFIG_WIFI_START, wifi);

	SystemConfig sys  {
		c.boardType
	};
	EEPROM.put(CONFIG_SYSTEM_START, sys);
	
	EEPROM.put(EEPROM_CONFIG_ADDRESS, EEPROM_CHECK_SUM);
	bool ret = EEPROM.commit();
	EEPROM.end();

	return ret;
}

bool AmsConfiguration::save() {
	EEPROM.begin(EEPROM_SIZE);
	EEPROM.put(EEPROM_CONFIG_ADDRESS, EEPROM_CHECK_SUM);
	saveTempSensors();
	bool success = EEPROM.commit();
	EEPROM.end();

	configVersion = EEPROM_CHECK_SUM;
	return success;
}

uint8_t AmsConfiguration::getTempSensorCount() {
	return tempSensorCount;
}

TempSensorConfig* AmsConfiguration::getTempSensorConfig(uint8_t address[8]) {
    for(int x = 0; x < tempSensorCount; x++) {
        TempSensorConfig *conf = tempSensors[x];
        if(isSensorAddressEqual(conf->address, address)) {
			return conf;
		}
	}
	return NULL;
}

void AmsConfiguration::updateTempSensorConfig(uint8_t address[8], const char name[32], bool common) {
    bool found = false;
    for(int x = 0; x < tempSensorCount; x++) {
        TempSensorConfig *data = tempSensors[x];
        if(isSensorAddressEqual(data->address, address)) {
            found = true;
            strcpy(data->name, name);
            data->common = common;
        }
    }
    if(!found) {
		TempSensorConfig** tempSensors = new TempSensorConfig*[tempSensorCount+1];
		if(this->tempSensors != NULL) {
			for(int i = 0;i < tempSensorCount; i++) {
				tempSensors[i] = this->tempSensors[i];
			}
		}
        TempSensorConfig *data = new TempSensorConfig();
        memcpy(data->address, address, 8);
        strcpy(data->name, name);
        data->common = common;
        tempSensors[tempSensorCount++] = data;
		if(this->tempSensors != NULL) {
			delete this->tempSensors;
		}
		this->tempSensors = tempSensors;
    }
}

bool AmsConfiguration::isSensorAddressEqual(uint8_t a[8], uint8_t b[8]) {
    for(int i = 0; i < 8; i++) {
        if(a[i] != b[i]) return false;
    }
    return true;
}


int AmsConfiguration::readString(int pAddress, char* pString[]) {
	int address = 0;
	byte length = EEPROM.read(pAddress + address);
	address++;

	char* buffer = new char[length];
	for (int i = 0; i<length; i++)
	{
		buffer[i] = EEPROM.read(pAddress + address++);
	}
	*pString = buffer;
	return address;
}

int AmsConfiguration::readInt(int address, int *value) {
	int lower = EEPROM.read(address);
	int higher = EEPROM.read(address + 1);
	*value = lower + (higher << 8);
	return 2;
}

int AmsConfiguration::readBool(int address, bool *value) {
	byte y = EEPROM.read(address);
	*value = (bool)y;
	return 1;
}

int AmsConfiguration::readByte(int address, byte *value) {
	*value = EEPROM.read(address);
	return 1;
}

void AmsConfiguration::print(Print* debugger)
{
	debugger->println("-----------------------------------------------");
	WiFiConfig wifi;
	if(getWiFiConfig(wifi)) {
		debugger->println("--WiFi configuration--");
		debugger->printf("SSID:                 '%s'\r\n", wifi.ssid);
		debugger->printf("Psk:                  '%s'\r\n", wifi.psk);
		if(strlen(wifi.ip) > 0) {
			debugger->printf("IP:                   '%s'\r\n", wifi.ip);
			debugger->printf("Gateway:              '%s'\r\n", wifi.gateway);
			debugger->printf("Subnet:               '%s'\r\n", wifi.subnet);
			debugger->printf("DNS1:                 '%s'\r\n", wifi.dns1);
			debugger->printf("DNS2:                 '%s'\r\n", wifi.dns2);
		}
		debugger->printf("Hostname:             '%s'\r\n", wifi.hostname);
		debugger->printf("mDNS:                 '%s'\r\n", wifi.mdns ? "Yes" : "No");
		debugger->println("");
		delay(10);
		Serial.flush();
	}

	MqttConfig mqtt;
	if(getMqttConfig(mqtt)) {
		debugger->println("--MQTT configuration--");
		if(strlen(mqtt.host) > 0) {
			debugger->printf("Enabled:              Yes\r\n");
			debugger->printf("Host:                 '%s'\r\n", mqtt.host);
			debugger->printf("Port:                 %i\r\n", mqtt.port);
			debugger->printf("Client ID:            '%s'\r\n", mqtt.clientId);
			debugger->printf("Publish topic:        '%s'\r\n", mqtt.publishTopic);
			debugger->printf("Subscribe topic:      '%s'\r\n", mqtt.subscribeTopic);
			if (strlen(mqtt.username) > 0) {
				debugger->printf("Username:             '%s'\r\n", mqtt.username);
				debugger->printf("Password:             '%s'\r\n", mqtt.password);
			}
			debugger->printf("Payload format:       %i\r\n", mqtt.payloadFormat);
			debugger->printf("SSL:                  %s\r\n", mqtt.ssl ? "Yes" : "No");
		} else {
			debugger->printf("Enabled:              No\r\n");
		}
		debugger->println("");
		delay(10);
		Serial.flush();
	}

	WebConfig web;
	if(getWebConfig(web)) {
		debugger->println("--Web configuration--");
		debugger->printf("Security:             %i\r\n", web.security);
		if (web.security > 0) {
			debugger->printf("Username:             '%s'\r\n", web.username);
			debugger->printf("Password:             '%s'\r\n", web.password);
		}
		debugger->println("");
		delay(10);
		Serial.flush();
	}

	MeterConfig meter;
	if(getMeterConfig(meter)) {
		debugger->println("--Meter configuration--");
		debugger->printf("Type:                 %i\r\n", meter.type);
		debugger->printf("Distribution system:  %i\r\n", meter.distributionSystem);
		debugger->printf("Main fuse:            %i\r\n", meter.mainFuse);
		debugger->printf("Production Capacity:  %i\r\n", meter.productionCapacity);
		debugger->printf("Substitute missing:   %s\r\n", meter.substituteMissing ? "Yes" : "No");
		debugger->println("");
		delay(10);
		Serial.flush();
	}

	GpioConfig gpio;
	if(getGpioConfig(gpio)) {
		debugger->println("--GPIO configuration--");
		debugger->printf("HAN pin:              %i\r\n", gpio.hanPin);
		debugger->printf("LED pin:              %i\r\n", gpio.ledPin);
		debugger->printf("LED inverted:         %s\r\n", gpio.ledInverted ? "Yes" : "No");
		debugger->printf("LED red pin:          %i\r\n", gpio.ledPinRed);
		debugger->printf("LED green pin:        %i\r\n", gpio.ledPinGreen);
		debugger->printf("LED blue pin:         %i\r\n", gpio.ledPinBlue);
		debugger->printf("LED inverted:         %s\r\n", gpio.ledRgbInverted ? "Yes" : "No");
		debugger->printf("AP pin:               %i\r\n", gpio.apPin);
		debugger->printf("Temperature pin:      %i\r\n", gpio.tempSensorPin);
		debugger->printf("Temp analog pin:      %i\r\n", gpio.tempAnalogSensorPin);
		debugger->printf("Vcc pin:              %i\r\n", gpio.vccPin);
		debugger->printf("Vcc multiplier:       %f\r\n", gpio.vccMultiplier / 1000.0);
		debugger->printf("Vcc offset:           %f\r\n", gpio.vccOffset / 100.0);
		debugger->printf("Vcc boot limit:       %f\r\n", gpio.vccBootLimit / 10.0);
		debugger->println("");
		delay(10);
		Serial.flush();
	}

	DomoticzConfig domo;
	if(getDomoticzConfig(domo)) {
		debugger->println("--Domoticz configuration--");
		if(mqtt.payloadFormat == 3 && domo.elidx > 0) {
			debugger->printf("Enabled:              Yes\r\n");
			debugger->printf("Domoticz ELIDX:       %i\r\n", domo.elidx);
			debugger->printf("Domoticz VL1IDX:      %i\r\n", domo.vl1idx);
			debugger->printf("Domoticz VL2IDX:      %i\r\n", domo.vl2idx);
			debugger->printf("Domoticz VL3IDX:      %i\r\n", domo.vl3idx);
			debugger->printf("Domoticz CL1IDX:      %i\r\n", domo.cl1idx);
		} else {
			debugger->printf("Enabled:              No\r\n");
		}
		debugger->println("");
		delay(10);
		Serial.flush();
	}

	NtpConfig ntp;
	if(getNtpConfig(ntp)) {
		debugger->println("--NTP configuration--");
		debugger->printf("Enabled:              %s\r\n", ntp.enable ? "Yes" : "No");
		if(ntp.enable) {
			debugger->printf("Offset:               %i\r\n", ntp.offset);
			debugger->printf("Summer offset:        %i\r\n", ntp.summerOffset);
			debugger->printf("Server:               %s\r\n", ntp.server);
			debugger->printf("DHCP:                 %s\r\n", ntp.dhcp ? "Yes" : "No");
		}
		debugger->println("");
		delay(10);
		Serial.flush();
	}

	EntsoeConfig entsoe;
	if(getEntsoeConfig(entsoe)) {
		debugger->println("--ENTSO-E configuration--");
		debugger->printf("Token:                %s\r\n", entsoe.token);
		if(strlen(entsoe.token) > 0) {
			debugger->printf("Area:                 %s\r\n", entsoe.area);
			debugger->printf("Currency:             %s\r\n", entsoe.currency);
			debugger->printf("Multiplier:           %f\r\n", entsoe.multiplier / 1000.0);
		}
		debugger->println("");
		delay(10);
		Serial.flush();
	}

	debugger->printf("Temp sensor count:    %i\r\n", this->getTempSensorCount());

	debugger->println("-----------------------------------------------");
}
