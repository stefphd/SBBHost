
#---------------------------------------------------------------------------------
# Makefile for compilation and installation of SBBHost in Linux
#---------------------------------------------------------------------------------

#Main info
NAME		:= SBB Host

#Compiler
CC          := g++

#The Target Binary Program
TARGET      := sbbhost

#The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR      := ./src
LIBDIR	 	:= ./lib
INCDIR      := ./include
BUILDDIR    := ./.build
TARGETDIR   := ./bin/linux
RESOURCES   := ./resource/sbbhost-config.ini ./resource/sbbhost-toolbar.xml
DOCS		:= ./docs
SRCEXT      := cpp
DEPEXT      := d
OBJEXT      := o

#Flags, Libraries and Includes
USERCFLAGS	:= 
CFLAGS      := `pkg-config gtkmm-4.0 --cflags --libs` -lfftw3 -g $(USERCFLAGS)
LIB			:= -I./lib/cairoplotter -I./lib/tinymatwriter -I./lib/sockpp #-I/usr/include/eigen3
INC         := -I$(INCDIR) -I/usr/local/include $(LIB)
INCDEP      := -I$(INCDIR) $(LIB)

#Installer files
RESOURCE_DIR:= ./resource
ICON		:= sbbhost-icon.svg
DESKTOP_FILE:= sbbhost.desktop
SHARE_FILE	:= $(RESOURCES)
DOC_FILE	:= README.md
INSTALLER   := ./installer/linux
INSTALL_DIR := $(shell cat ./installation_path.txt)

#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------

SOURCES     := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS     := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))
LIBSOURCES  := $(shell find $(LIBDIR) -type f -name *.$(SRCEXT))
LIBOBJECTS  := $(patsubst $(LIBDIR)/%,$(BUILDDIR)/%,$(LIBSOURCES:.$(SRCEXT)=.$(OBJEXT)))

#Defauilt Make
all: build doc

#Build
build: resources $(TARGET)

#Documentation
doc: cleandoc
	@doxygen

cleandoc:
	@$(RM) -rf $(DOCS)/html/*
	@$(RM) -rf $(DOCS)/latex/*

#Installer
installer: cleaninstaller
	@mkdir -p $(INSTALLER)$(INSTALL_DIR)/bin
	@mkdir -p $(INSTALLER)$(INSTALL_DIR)/share/pixmaps
	@mkdir -p $(INSTALLER)$(INSTALL_DIR)/share/applications
	@mkdir -p $(INSTALLER)$(INSTALL_DIR)/share/doc/$(TARGET)
	@mkdir -p $(INSTALLER)$(INSTALL_DIR)/share/$(TARGET)
	@cp $(TARGETDIR)/$(TARGET) $(INSTALLER)$(INSTALL_DIR)/bin/
	@cp $(SHARE_FILE) $(INSTALLER)$(INSTALL_DIR)/share/$(TARGET)
	@cp $(RESOURCE_DIR)/$(DESKTOP_FILE) $(INSTALLER)$(INSTALL_DIR)/share/applications
	@cp $(RESOURCE_DIR)/$(ICON) $(INSTALLER)$(INSTALL_DIR)/share/pixmaps
	@cp $(DOC_FILE) $(INSTALLER)$(INSTALL_DIR)/share/doc/$(TARGET)
	
cleaninstaller:
	@$(RM) -rf $(INSTALLER)

#Installation (call Makefile in installer folder)
install: installer
	@cp -r $(INSTALLER)/* /
	@update-desktop-database $(INSTALL_DIR)/share/applications

uninstall:
	@$(RM) -rf $(INSTALL_DIR)/bin/$(TARGET)
	@$(RM) -rf $(INSTALL_DIR)/share/$(TARGET)
	@$(RM) -rf	$(INSTALL_DIR)/share/applications/$(DESKTOP_FILE)
	@$(RM) -rf $(INSTALL_DIR)/share/pixmaps/$(ICON)
	@$(RM) -rf $(INSTALL_DIR)/share/doc/$(TARGET)
	@update-desktop-database $(INSTALL_DIR)/share/applications

#Remake
remake: cleaner build

#Copy Resources from Resources Directory to Target Directory
resources: directories
	@cp $(RESOURCES) $(TARGETDIR)/share/$(TARGET)
		
#Make the Directories
directories:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(TARGETDIR)/share/$(TARGET)
	@mkdir -p $(BUILDDIR)

#Clean only Objecst
clean:
	@$(RM) -rf $(BUILDDIR)

#Full Clean, Objects and Binaries
cleaner: clean
	@$(RM) -rf $(TARGETDIR)

#Run the program
run: all
	cd $(TARGETDIR); ./$(TARGET);

#Pull in dependency info for *existing* .o files
-include $(OBJECTS:.$(OBJEXT)=.$(DEPEXT))

#Link
$(TARGET): $(OBJECTS) $(LIBOBJECTS)
	$(CC) -o $(TARGETDIR)/$(TARGET) $^ $(CFLAGS)
#@./$(TARGETDIR)/$(TARGET)

#Compile
$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(INC) -c -o $@ $< $(CFLAGS)
	@$(CC) $(INCDEP) -MM $(SRCDIR)/$*.$(SRCEXT) > $(BUILDDIR)/$*.$(DEPEXT) $(CFLAGS)
	@cp -f $(BUILDDIR)/$*.$(DEPEXT) $(BUILDDIR)/$*.$(DEPEXT).tmp
	@sed -e 's|.*:|$(BUILDDIR)/$*.$(OBJEXT):|' < $(BUILDDIR)/$*.$(DEPEXT).tmp > $(BUILDDIR)/$*.$(DEPEXT)
	@sed -e 's/.*://' -e 's/\\$$//' < $(BUILDDIR)/$*.$(DEPEXT).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(BUILDDIR)/$*.$(DEPEXT)
	@rm -f $(BUILDDIR)/$*.$(DEPEXT).tmp

#Compile libraries
$(BUILDDIR)/%.$(OBJEXT): $(LIBDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(INC) -c -o $@ $< $(CFLAGS)
	@$(CC) $(INCDEP) -MM $(LIBDIR)/$*.$(SRCEXT) > $(BUILDDIR)/$*.$(DEPEXT) $(CFLAGS)
	@cp -f $(BUILDDIR)/$*.$(DEPEXT) $(BUILDDIR)/$*.$(DEPEXT).tmp
	@sed -e 's|.*:|$(BUILDDIR)/$*.$(OBJEXT):|' < $(BUILDDIR)/$*.$(DEPEXT).tmp > $(BUILDDIR)/$*.$(DEPEXT)
	@sed -e 's/.*://' -e 's/\\$$//' < $(BUILDDIR)/$*.$(DEPEXT).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(BUILDDIR)/$*.$(DEPEXT)
	@rm -f $(BUILDDIR)/$*.$(DEPEXT).tmp

#Non-File Targets
.PHONY: all build remake clean cleaner resources doc cleandoc run install uninstall installer cleaninstaller
