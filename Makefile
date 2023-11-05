PROGRAM_NAME := bad_cc
PROGRAM_VERSION := 0.0.0
PROGRAM_EMAIL := mfoley@mfoley.xyz

BUILD_DIR := build
SOURCE_DIR := src
INCLUDE_DIR := include
OUTPUT_FILE := $(PROGRAM_NAME)

CONFIG_HEADER := $(INCLUDE_DIR)/config.h
CONFIG_HEADER_IN := $(INCLUDE_DIR)/config.h.in
CONFIG_REPLACE_VALUES := PROGRAM_NAME PROGRAM_VERSION PROGRAM_EMAIL

SOURCE_FILES := $(shell find $(SOURCE_DIR) -name "*.c")
OBJECT_FILES := $(patsubst $(SOURCE_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCE_FILES))
DEP_FILES := $(patsubst %.o,%.d,$(OBJECT_FILES))

CFLAGS += -I$(INCLUDE_DIR)
CFLAGS += -g -O0

LDFLAGS +=
LDLIBS += -lc

.PHONY: all clean

NODEPS := clean distclean

all: $(OUTPUT_FILE)

clean:
	$(RM) -r $(BUILD_DIR)
	$(RM) $(CONFIG_HEADER)

$(BUILD_DIR):
	mkdir -p $@

$(CONFIG_HEADER): $(CONFIG_HEADER_IN)
	sed $(foreach val,$(CONFIG_REPLACE_VALUES),-e "s/@$(val)@/$($(val))/g") $^ > $@

$(BUILD_DIR)/%.d: $(SOURCE_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -MM -MT$@ $< > $@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c $(CONFIG_HEADER) | $(BUILD_DIR)
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS) $(LDLIBS)

$(OUTPUT_FILE): $(OBJECT_FILES)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

print-%:
	@echo $* = $($*)

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
-include $(DEP_FILES)
endif
