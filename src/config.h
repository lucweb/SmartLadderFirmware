#ifndef config_H
#define config_H

#define VERSION F("v1.0.1")

// Define whether I2C functionality should be enabled (1) or disabled (0)
#define USE_I2C 1

// Define whether Wi-Fi functionality should be enabled (1) or disabled (0)
#define USE_WIFI 1

// Define whether the LAN8720 Ethernet driver should be enabled (1) or disabled (0)
#define USE_ETH8720 0

// Ethernet settings section (enabled only if USE_ETH8720 is 1)
#define ETH_ADDR 0
#define ETH_POWER_PIN -1
#define ETH_MDC_PIN 23
#define ETH_MDIO_PIN 18
#define ETH_TYPE ETH_PHY_LAN8720
#define ETH_CLK_MODE ETH_CLOCK_GPIO17_OUT

#endif // config_H