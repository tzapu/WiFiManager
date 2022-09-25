set(WiFiManager
WiFiManager.cpp
)

set(srcs ${WiFiManager})
# if library is used in library mus be add in requires
# idf_component_register(SRCS ${srcs} INCLUDE_DIRS src REQUIRES arduino ArduinoJson PageBuilder spiffs app_update )
idf_component_register(SRCS ${srcs} INCLUDE_DIRS . REQUIRES arduino )