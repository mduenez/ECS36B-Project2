# Define the tools
AR=ar
CC=gcc
CXX=g++

# Define the directories
INC_DIR				= ./proj2/include
SRC_DIR				= ./proj2/src
TESTSRC_DIR			= ./proj2/testsrc
BIN_DIR				= ./bin
OBJ_DIR				= ./obj
LIB_DIR				= ./lib
TESTOBJ_DIR			= ./testobj
TESTBIN_DIR			= ./testbin
TESTCOVER_DIR		= ./htmlconv

# Define the flags for compilation/linking
DEFINES				=
INCLUDE				= -I $(INC_DIR)
ARFLAGS				= rcs
CFLAGS				= -Wall
CPPFLAGS			= --std=c++17
LDFLAGS				=

TEST_CFLAGS			= $(CFLAGS) -O0 -g --coverage
TEST_CPPFLAGS		= $(CPPFLAGS) -fno-inline
TEST_LDFLAGS		= $(LDFLAGS) -lgtest -lgtest_main -lpthread

# Define the object files
SVG_OBJ 			= $(OBJ_DIR)/svg.o
MAIN_OBJ 			= $(OBJ_DIR)/main.o
TEST_SVG_OBJ		= $(TESTOBJ_DIR)/svg.o
TEST_SVG_TEST_OBJ	= $(TESTOBJ_DIR)/SVGTest.o
TEST_OBJ_FILES		= $(TEST_SVG_OBJ) $(TEST_SVG_TEST_OBJ)

# Define the targets
TEST_TARGET				= $(TESTBIN_DIR)/testsvg
TESTSVG             	= $(TESTBIN_DIR)/testsvg
TESTSTRSOURCE       	= $(TESTBIN_DIR)/teststrdatasource
TESTSTRSINK         	= $(TESTBIN_DIR)/teststrdatasink
TESTXML             	= $(TESTBIN_DIR)/testxml
TEST_XML_OBJ 			= $(TESTOBJ_DIR)/XMLTest.o
TEST_STRSOURCE_OBJ   	= $(TESTOBJ_DIR)/StringDataSource.o
TEST_STRSOURCE_TEST_OBJ = $(TESTOBJ_DIR)/StringDataSourceTest.o
TEST_STRSINK_OBJ     	= $(TESTOBJ_DIR)/StringDataSink.o
TEST_STRSINK_TEST_OBJ 	= $(TESTOBJ_DIR)/StringDataSinkTest.o
TEST_SVGWRITER_OBJ 		= $(TESTOBJ_DIR)/SVGWriterTest.o
TESTSVGWRITER       	= $(TESTBIN_DIR)/testsvgwriter
MAIN_BIN				= $(BIN_DIR)/main
LIBSVG					= $(LIB_DIR)/libsvg.a


all: directories runtests $(LIBSVG) $(MAIN_BIN) compare

$(OBJ_DIR)/svg.o: $(SRC_DIR)/svg.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(LIBSVG): $(SVG_OBJ)
	$(AR) $(ARFLAGS) $@ $^

$(MAIN_OBJ): $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(MAIN_BIN): $(MAIN_OBJ) $(LIBSVG)
	$(CC) $^ -o $@

runmain: $(MAIN_BIN)
	./$(MAIN_BIN)

compare: runmain
	xmldiff expected_checkmark.svg checkmark.svg
runtests: $(TESTSVG) $(TESTSTRSOURCE) $(TESTSTRSINK) $(TESTXML) $(TESTSVGWRITER)
	$(TESTSVG)
	$(TESTSTRSOURCE)
	$(TESTSTRSINK)
	$(TESTXML)
	$(TESTSVGWRITER)
	lcov --capture --directory . --output-file $(TESTCOVER_DIR)/coverage.info --ignore-errors inconsistent,source
	lcov --remove $(TESTCOVER_DIR)/coverage.info '/usr/*' '*/testsrc/*' --output-file $(TESTCOVER_DIR)/coverage.info
	genhtml $(TESTCOVER_DIR)/coverage.info --output-directory $(TESTCOVER_DIR)
	
$(TEST_TARGET): $(TEST_OBJ_FILES)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_OBJ_FILES) $(TEST_LDFLAGS) -o $(TEST_TARGET)

$(TEST_SVG_OBJ): $(SRC_DIR)/svg.c | directories
	$(CC) $(TEST_CFLAGS) $(DEFINES) $(INCLUDE) -c $(SRC_DIR)/svg.c -o $(TEST_SVG_OBJ)

$(TEST_SVG_TEST_OBJ): $(TESTSRC_DIR)/SVGTest.cpp
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(DEFINES) $(INCLUDE) -c $(TESTSRC_DIR)/SVGTest.cpp -o $(TEST_SVG_TEST_OBJ)

$(TESTSTRSOURCE): $(TEST_STRSOURCE_OBJ) $(TEST_STRSOURCE_TEST_OBJ)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $^ $(TEST_LDFLAGS) -o $@

$(TEST_STRSOURCE_OBJ): $(SRC_DIR)/StringDataSource.cpp
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(INCLUDE) -c $< -o $@

$(TEST_STRSOURCE_TEST_OBJ): $(TESTSRC_DIR)/StringDataSourceTest.cpp
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(INCLUDE) -c $< -o $@

$(TESTSTRSINK): $(TEST_STRSINK_OBJ) $(TEST_STRSINK_TEST_OBJ)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $^ $(TEST_LDFLAGS) -o $@

$(TEST_STRSINK_OBJ): $(SRC_DIR)/StringDataSink.cpp
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(INCLUDE) -c $< -o $@

$(TEST_STRSINK_TEST_OBJ): $(TESTSRC_DIR)/StringDataSinkTest.cpp
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(INCLUDE) -c $< -o $@

$(TESTXML): $(TEST_XML_OBJ)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $^ $(TEST_LDFLAGS) -o $@

$(TEST_XML_OBJ): $(TESTSRC_DIR)/XMLTest.cpp
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(INCLUDE) -c $< -o $@

$(TESTSVGWRITER): $(TEST_SVGWRITER_OBJ)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $^ $(TEST_LDFLAGS) -o $@

$(TEST_SVGWRITER_OBJ): $(TESTSRC_DIR)/SVGWriterTest.cpp
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(INCLUDE) -c $< -o $@


directories:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)
	mkdir -p $(LIB_DIR)
	mkdir -p $(TESTOBJ_DIR)
	mkdir -p $(TESTBIN_DIR)
	mkdir -p $(TESTCOVER_DIR)

clean:
	rm -rf $(BIN_DIR)
	rm -rf $(OBJ_DIR)
	rm -rf $(LIB_DIR)
	rm -rf $(TESTOBJ_DIR)
	rm -rf $(TESTBIN_DIR)
	rm -rf $(TESTCOVER_DIR)

