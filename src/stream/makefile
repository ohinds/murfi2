##########################################################################
#
# generic src subdir makefile for the real-time fMRI system
# just compiles all cpp files into obj files
#
##########################################################################

############################## SUFFIXES ##############################

.DELETE_ON_ERROR:

.SUFFIXES:
.SUFFIXES:  .o .cpp

# suffix rule for subsidiary source files
# (see http://www.gnu.org/manual/make/html_chapter/make_10.html#SEC111)
$(OBJ_DIR)/%.o: %.cpp %.h
	@$(ECHO) '[make: building $@]'
	$(CC) $(C_FLAGS) -o $@ -c $<

HDR_FILES = $(wildcard *.h)
SRC_FILES = $(wildcard ./*.cpp)
TMP_FILES = $(patsubst ./%,$(OBJ_DIR)/%,$(SRC_FILES))
OBJ_FILES = $(TMP_FILES:.cpp=.o)

default: $(OBJ_FILES)
	cd analysis && $(MAKE)
	cd preprocess && $(MAKE)
	cd postprocess && $(MAKE)

############################### CLEAN ################################

clean:
	@$(ECHO) 'make: removing object and autosave files'
	-$(RM) -f *.o *~
	cd analysis && $(MAKE) clean
	cd preprocess && $(MAKE) clean
	cd postprocess && $(MAKE) clean

######################################################################
### $Source$
### Local Variables:
### mode: makefile
### fill-column: 76
### comment-column: 0
### End:
