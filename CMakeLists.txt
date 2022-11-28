cmake_minimum_required(VERSION 3.5)

idf_component_register(
                       SRCS "WiFiManager.cpp"
                       INCLUDE_DIRS "."
                       REQUIRES arduino
)

project(WiFiManager)
