CXX      := g++ # -g++ to compile everything even with errors
CXXFLAGS := -std=c++14 -pedantic-errors -Wall -Wextra -fmessage-length=0 #-Werror
LDFLAGS  := -lstdc++ -lm -lGL -lGLU -lGLEW -lglfw #-L/usr/lib
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := $(BUILD)/apps
TARGET   := main.out
INCLUDE  := -Iinclude/
SRC      :=                      \
	$(wildcard src/graphics/*.cpp) \
	$(wildcard src/logging/*.cpp)  \
	$(wildcard src/vendor/*/*.cpp)  \
	$(wildcard src/*.cpp)          \

OBJECTS  := $(SRC:%.cpp=$(OBJ_DIR)/%.o)
DEPENDENCIES  \
         := $(OBJECTS:.o=.d)

ARCHIVE  := doom-slayer-source-code
SUBMISSIONS := 									\
	src/ include/	res/						\
	LICENSE README.md Makefile 		\

all: build $(APP_DIR)/$(TARGET)
	-@rm -rf $(TARGET)
	-@ln -s $(APP_DIR)/$(TARGET) $(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -MMD -o $@

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(APP_DIR)/$(TARGET) $^ $(LDFLAGS)

-include $(DEPENDENCIES)

.PHONY: all run build clean debug release archive info

run: all
	$(APP_DIR)/$(TARGET)

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*
	-@rm -vf $(TARGET)

archive:
	@rm -rf $(ARCHIVE)
	@mkdir $(ARCHIVE)
	@cp -r $(SUBMISSIONS) $(ARCHIVE)
	@zip -rq $(ARCHIVE).zip $(ARCHIVE)
	-@rm -rf $(ARCHIVE)
	@echo "Zipped the project to $(ARCHIVE).zip"


info:
	@echo "[*] Application dir: ${APP_DIR}     "
	@echo "[*] Object dir:      ${OBJ_DIR}     "
	@echo "[*] Sources:         ${SRC}         "
	@echo "[*] Objects:         ${OBJECTS}     "
	@echo "[*] Dependencies:    ${DEPENDENCIES}"