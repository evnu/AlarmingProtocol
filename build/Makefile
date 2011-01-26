RTDS_GEN_DIR=c:\src\rtds\alarmingprotocol\rtds-network-vorlage\build
RTDS_CLASSES_DIR=c:\src\rtds\alarmingprotocol\rtds-network-vorlage
RTDS_TEMPLATES_DIR=c:\programme\rtds\share\ccg\windows

RTDS_CC=mingw32-g++
RTDS_LNK=$(RTDS_CC)
RTDS_CC_INCLUDES=-I "." -I "${RTDS_TEMPLATES_DIR}" -I "${RTDS_HOME}\share\ccg\trace\formattrace" -I "${RTDS_HOME}\share\ccg\common" -I "${RTDS_HOME}\share\ccg\trace\commandinterpreter" -I "$(RTDS_CLASSES_DIR)"
RTDS_CC_OPTIONS=-D RTDS_FORMAT_TRACE -I "." -I "${RTDS_TEMPLATES_DIR}" -I "${RTDS_HOME}\share\ccg\trace\formattrace" -I "${RTDS_HOME}\share\ccg\common" -g -D RTDS_SIMULATOR -D RTDS_CMD_INTERPRETER -I "${RTDS_HOME}\share\ccg\trace\commandinterpreter" -I "$(RTDS_CLASSES_DIR)" -D RTDS_SOCKET_IP_ADDRESS=10.0.2.15 -D RTDS_SOCKET_PORT=49250 
RTDS_LNK_OPTIONS=

RTDS_RM=cmd /c del

RTDS_OBJECTS = \
	rtds_env.o \
	networktopology.o \
	rtds_start.o \
	rtds_encdecmsgdata.o \
	ptransportsender.o \
	blockmember.o \
	papp.o \
	ptransportreceiver.o \
	creator.o \
	admin.o \
	rtds_os.o \
	rtds_tcp_client.o \
	rtds_cmdinterpreter.o \
	rtds_formattrace.o


RTDS_ADDL_OBJECTS = \
	C:/Programme/RTDS/share/3rdparty/MinGW/lib/libws2_32.a

RTDS_EXT_OBJECTS =

RTDS_TARGET_BASE_NAME=network
RTDS_TARGET_EXTENSION=.exe

all: $(RTDS_TARGET_BASE_NAME)$(RTDS_TARGET_EXTENSION)

$(RTDS_TARGET_BASE_NAME)$(RTDS_TARGET_EXTENSION): $(RTDS_OBJECTS) $(RTDS_ADDL_OBJECTS) $(RTDS_EXT_OBJECTS)
	$(RTDS_LNK) $(RTDS_LNK_OPTIONS) -o "$@" $(RTDS_OBJECTS) $(RTDS_ADDL_OBJECTS) $(RTDS_EXT_OBJECTS)

rtds_env.o: $(RTDS_TEMPLATES_DIR)\rtds_env.c
	$(RTDS_CC) $(RTDS_CC_OPTIONS) -c -o "$@" "$<"

networktopology.o: $(RTDS_CLASSES_DIR)\networktopology.cpp
	$(RTDS_CC) $(RTDS_CC_OPTIONS) -c -o "$@" "$<"

rtds_start.o: $(RTDS_GEN_DIR)\rtds_start.c
	$(RTDS_CC) $(RTDS_CC_OPTIONS) -c -o "$@" "$<"

rtds_encdecmsgdata.o: $(RTDS_GEN_DIR)\rtds_encdecmsgdata.c
	$(RTDS_CC) $(RTDS_CC_OPTIONS) -c -o "$@" "$<"

ptransportsender.o: $(RTDS_GEN_DIR)\ptransportsender.c $(RTDS_CLASSES_DIR)\blockmember.h $(RTDS_CLASSES_DIR)\networktopology.h $(RTDS_GEN_DIR)\rtds_gen.h $(RTDS_GEN_DIR)\ptransportsender.h $(RTDS_GEN_DIR)\rtds_messages.h
	$(RTDS_CC) $(RTDS_CC_OPTIONS) -c -o "$@" "$<"

blockmember.o: $(RTDS_CLASSES_DIR)\blockmember.cpp
	$(RTDS_CC) $(RTDS_CC_OPTIONS) -c -o "$@" "$<"

papp.o: $(RTDS_GEN_DIR)\papp.c $(RTDS_CLASSES_DIR)\blockmember.h $(RTDS_CLASSES_DIR)\networktopology.h $(RTDS_GEN_DIR)\rtds_gen.h $(RTDS_GEN_DIR)\papp.h $(RTDS_GEN_DIR)\rtds_messages.h
	$(RTDS_CC) $(RTDS_CC_OPTIONS) -c -o "$@" "$<"

ptransportreceiver.o: $(RTDS_GEN_DIR)\ptransportreceiver.c $(RTDS_CLASSES_DIR)\blockmember.h $(RTDS_CLASSES_DIR)\networktopology.h $(RTDS_GEN_DIR)\rtds_gen.h $(RTDS_GEN_DIR)\ptransportreceiver.h $(RTDS_GEN_DIR)\rtds_messages.h
	$(RTDS_CC) $(RTDS_CC_OPTIONS) -c -o "$@" "$<"

creator.o: $(RTDS_GEN_DIR)\creator.c $(RTDS_GEN_DIR)\rtds_gen.h $(RTDS_GEN_DIR)\creator.h $(RTDS_GEN_DIR)\rtds_messages.h $(RTDS_GEN_DIR)\nodeblockclass.h $(RTDS_CLASSES_DIR)\blockmember.h $(RTDS_CLASSES_DIR)\networktopology.h
	$(RTDS_CC) $(RTDS_CC_OPTIONS) -c -o "$@" "$<"

admin.o: $(RTDS_GEN_DIR)\admin.c $(RTDS_GEN_DIR)\rtds_gen.h $(RTDS_GEN_DIR)\admin.h $(RTDS_GEN_DIR)\rtds_messages.h $(RTDS_GEN_DIR)\network.h $(RTDS_CLASSES_DIR)\blockmember.h $(RTDS_CLASSES_DIR)\networktopology.h
	$(RTDS_CC) $(RTDS_CC_OPTIONS) -c -o "$@" "$<"

rtds_os.o: $(RTDS_TEMPLATES_DIR)\rtds_os.c
	$(RTDS_CC) $(RTDS_CC_OPTIONS) -c -o "$@" "$<"

rtds_tcp_client.o: $(RTDS_TEMPLATES_DIR)\rtds_tcp_client.c
	$(RTDS_CC) $(RTDS_CC_OPTIONS) -c -o "$@" "$<"

rtds_cmdinterpreter.o: $(RTDS_HOME)\share\ccg\trace\commandinterpreter\rtds_cmdinterpreter.c
	$(RTDS_CC) $(RTDS_CC_OPTIONS) -c -o "$@" "$<"

rtds_formattrace.o: $(RTDS_HOME)\share\ccg\trace\formattrace\rtds_formattrace.c
	$(RTDS_CC) $(RTDS_CC_OPTIONS) -c -o "$@" "$<"

RTDS_clean:
	$(RTDS_RM) $(RTDS_TARGET_BASE_NAME)$(RTDS_TARGET_EXTENSION)
	$(RTDS_RM) "rtds_env.o"
	$(RTDS_RM) "networktopology.o"
	$(RTDS_RM) "rtds_start.o"
	$(RTDS_RM) "rtds_encdecmsgdata.o"
	$(RTDS_RM) "ptransportsender.o"
	$(RTDS_RM) "blockmember.o"
	$(RTDS_RM) "papp.o"
	$(RTDS_RM) "ptransportreceiver.o"
	$(RTDS_RM) "creator.o"
	$(RTDS_RM) "admin.o"
	$(RTDS_RM) "rtds_os.o"
	$(RTDS_RM) "rtds_tcp_client.o"
	$(RTDS_RM) "rtds_cmdinterpreter.o"
	$(RTDS_RM) "rtds_formattrace.o"

RTDS_mrproper: RTDS_clean
	$(RTDS_RM) "RTDS_gen.h"
	$(RTDS_RM) "pTransportSender.h"
	$(RTDS_RM) "RTDS_Start.c"
	$(RTDS_RM) "RTDS_messages.h"
	$(RTDS_RM) "RTDS_encdecMsgData.c"
	$(RTDS_RM) "pTransportSender.c"
	$(RTDS_RM) "NodeBlockClass.h"
	$(RTDS_RM) "pTransportReceiver.h"
	$(RTDS_RM) "pApp.h"
	$(RTDS_RM) "Creator.h"
	$(RTDS_RM) "pApp.c"
	$(RTDS_RM) "Admin.h"
	$(RTDS_RM) "pTransportReceiver.c"
	$(RTDS_RM) "network.h"
	$(RTDS_RM) "Creator.c"
	$(RTDS_RM) "Admin.c"
	$(RTDS_RM) RTDS_gen.ini
	$(RTDS_RM) Makefile
