# My makefile
SKETCH = $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))espRGB.ino

ESP_ROOT = /opt/esp32/arduino-esp32/
CHIP = esp32

LIBS = $(HOME)/Arduino/libraries/Adafruit_NeoPixel

include $(HOME)/Arduino/makeEspArduino/makeEspArduino.mk