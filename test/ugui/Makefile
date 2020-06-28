######################################
# target
######################################
TARGET = test
BUILD_DIR = build
UGUI_DIR = ../../3rd/ugui
C_SOURCES  = test.c
C_SOURCES +=$(UGUI_DIR)/ugui.c

# C defines  -DFREERTOS 
C_DEFS =    
 
OPT = -O0
   
C_INCLUDES		=  \
-I. \
-I$(UGUI_DIR)

#######################################
# binaries
#######################################
PREFIX = 
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.

##CC = gcc 
CC = $(PREFIX)clang.exe
CP = $(PREFIX)llvm-objcopy 
 
CFLAGS =  -std=c99 $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections -c -fmessage-length=0
CFLAGS += -Wl,-Map=map.map  
# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)" 

# libraries  
LIBS = -lc  
LIBDIR =    
LDFLAGS =   $(LIBDIR) $(LIBS)  -Wl,--gc-sections -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref 
 
# default action: build all
all: $(TARGET).out

#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS)  $< -o $@

$(TARGET).out: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(CP) -O binary $(TARGET).out $(TARGET).los

$(BUILD_DIR):
	mkdir $@

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
