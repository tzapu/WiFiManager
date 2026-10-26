cmake_minimum_required(VERSION 3.16)

idf_component_register(
                       SRCS "WiFiManager.cpp" "WiFiManagerStorage.cpp"
                       INCLUDE_DIRS "."
                       REQUIRES arduino
                       PRIV_REQUIRES nvs_flash esp_wifi esp_netif mbedtls
)

project(WiFiManager)
