##########################################################################
# 
# master makefile for the real-time fMRI system
#
# Oliver Hinds <ohinds@mit.edu> 2007-08-14
#
##########################################################################

# os we build for
OS=linux

# project name
export PROJECT = real

# whether to compile with debug, optimize flags
export DEBUG = 0
export PROF = 0
export OPTIM = 1
export STRIP = 1

export MTRACE = 0

# ace log levels
export ACE_NTRACE   = 1
export ACE_NDEBUG   = 0
export ACE_NLOGGING = 0

export SRC_DIR = $(PWD)/src
export BIN_DIR = $(PWD)/bin
export OBJ_DIR = $(PWD)/obj

################################ APPS ################################

export RM = /bin/rm -v
export ECHO = /bin/echo
export CC = /usr/bin/g++
export PKGCONFIG = /usr/bin/pkg-config

################################ LIBS ################################

export SIGC_HOME=/usr/include/sigc++-2.0
export CCPP_LIBS=-lccgnu2 -lccext2 -ldl -lpthread

################################ FLAG ################################

# debug flag
ifeq ($(DEBUG),1)
	DEBUG_FLAG = -g
endif

# profile flag
ifeq ($(PROF),1)
	PROF_FLAG = -pg
endif

# optimize flag
ifeq ($(OPTIM),1)
	OPTIM_FLAG = -O2
endif

# optimize flag
ifeq ($(STRIP),1)
	STRIP_FLAG = -s
endif

# mtrace flag
ifeq ($(MTRACE),1)
	MTRACE_FLAG = -DMTRACE
endif

# ace log flags
ifeq ($(ACE_NTRACE),1)
	ACE_NTRACE_FLAG = -DACE_NTRACE=1
else
	ACE_NTRACE_FLAG = -DACE_NTRACE=0
endif

ifeq ($(ACE_NDEBUG),1)
	ACE_NDEBUG_FLAG = -DACE_NDEBUG=1
else
	ACE_NDEBUG_FLAG = -DACE_NDEBUG=0
endif

ifeq ($(ACE_NLOGGING),1)
	ACE_NLOGGING_FLAG = -DACE_NLOGGING=1
else
	ACE_NLOGGING_FLAG = -DACE_NLOGGING=0
endif

SUB_DIRS = -I$(SRC_DIR)/executive -I$(SRC_DIR)/data -I$(SRC_DIR)/io -I$(SRC_DIR)/stream -I$(SRC_DIR)/stream/analysis -I$(SRC_DIR)/stream/preprocess -I$(SRC_DIR)/stream/postprocess -I$(SRC_DIR)/display -I$(SRC_DIR)/util

# library flags

# math

MATH_LIB = -lm 

GSL_INCS=-I/usr/include/gsl
GSL_LIB=-lgsl -lgslcblas

#DATETIME_LIB = -lboost_date_time
#THREAD_LIB = -lboost_thread
#SIGNALS_LIB = -lboost_signals -lsigc-2.0 
#SOCKETS_LIB = $(CCPP_LIBS)

#ACE_HOME=/usr/local/ACE_wrappers
#ACE_INCS=-I$(ACE_HOME)/ace
#ACE_LIB=-L$(ACE_HOME)/lib -lACE
ACE_FLAGS=$(ACE_NLOGGING_FLAG) $(ACE_NDEBUG_FLAG) $(ACE_NTRACE_FLAG)
ACE_LIB=-lACE

TINYXML_FLAGS=-DTIXML_USE_STL

GLUT_LIB=-lglut

VXL_INCS=-I/usr/local/include/vxl/core -I/usr/local/include/vxl/vcl
VXL_LIBS=-lvnl -lvcl -lvnl_algo

GNUPLOT_LIBS=-lgnuplot_i_vxl -L/usr/local/lib

# build compiler flags

export C_INCS = -I$(SRC_DIR) $(SUB_DIRS) $(GSL_INCS) $(ACE_INCS) $(ACE_FLAGS) $(TINYXML_FLAGS) $(VXL_INCS)

export C_FLAGS = -Werror -Wall \
	$(MTRACE_FLAG) $(PROF_FLAG) $(DEBUG_FLAG) $(OPTIM_FLAG) $(STRIP_FLAG) \
	$(C_INCS) 
#`$(PKGCONFIG_CMD)`

export C_LIBS = $(MATH_LIB) $(GSL_LIB) $(ACE_LIB) $(GLUT_LIB) $(VXL_LIBS) $(GNUPLOT_LIBS)

export PKGCONFIG_FLAGS = --cflags --libs sigc++-2.0
export PKGCONFIG_CMD = pkg-config $(PKGCONFIG_FLAGS)

export LDFLAGS = $(PROF_FLAG) $(C_LIBS)


############################### SUFFIXES ##############################

OBJ_FILES = $(wildcard $(OBJ_DIR)/*.o)

############################## TARGETS ###############################

default: $(PROJECT)
all:     $(PROJECT)
debug:	 
	$(MAKE) DEBUG=1 OPTIM=0 STRIP=0 $(PROJECT)
profile:	 
	$(MAKE) DEBUG=1 PROF=1 OPTIM=0 STRIP=0 $(PROJECT)

setdebug:
	export DEBUG = 1
	export OPTIM 0
	export STRIP 0

$(PROJECT): $(OBJ_FILES)
	@$(ECHO) 'make: building $@ for $(OS)...'
	cd $(SRC_DIR) && $(MAKE)
	$(CC) $(CFLAGS) $(OBJ_DIR)/*.o -o $(BIN_DIR)/$(PROJECT) $(LDFLAGS) 
	@$(ECHO) '############################################'
	@$(ECHO) 'make: built [$@] successfully!'
	@$(ECHO) '############################################'

############################### CLEAN ################################

clean:
	@$(ECHO) 'make: removing object and autosave files'
	cd $(SRC_DIR) && $(MAKE) clean
	-cd $(OBJ_DIR) && $(RM) -f *.o *~

######################################################################
### $Source$
### Local Variables:
### mode: makefile
### fill-column: 76
### comment-column: 0
### End: 
