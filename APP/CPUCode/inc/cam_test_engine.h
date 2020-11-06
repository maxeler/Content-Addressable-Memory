#ifndef _MAXELER_CAM_TEST_ENGINE
#define _MAXELER_CAM_TEST_ENGINE

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <algorithm>

#include "Maxfiles.h"

#define SLIC_DATATYPE uint32_t

#define RAM_SIZE (cam_MEMORY_DEPTH)
#define LOOKUP_SIZE (1 << cam_DATA_WIDTH)

class TestEngine
{

protected:
	const size_t SIZE;
	const float frequencyMultiplier;

#ifdef CAM_MULTIPLE
	SLIC_DATATYPE* input_CS;
#endif
	SLIC_DATATYPE* input_WE;
	SLIC_DATATYPE* input_WR_ADDR;
	SLIC_DATATYPE* input_DIN;
	SLIC_DATATYPE* input_EN;

	SLIC_DATATYPE* output_MATCH_ADDR;
    SLIC_DATATYPE* output_MULTIPLE_MATCH;
    SLIC_DATATYPE* output_SINGLE_MATCH;
    SLIC_DATATYPE* output_MATCH;
    SLIC_DATATYPE* output_BUSY;
    SLIC_DATATYPE* output_READ_WARNING;

    SLIC_DATATYPE* expect_ASSERT;
    SLIC_DATATYPE* expect_MATCH_ADDR;
    SLIC_DATATYPE* expect_MULTIPLE_MATCH;
    SLIC_DATATYPE* expect_SINGLE_MATCH;
    SLIC_DATATYPE* expect_MATCH;
    SLIC_DATATYPE* expect_BUSY;
    SLIC_DATATYPE* expect_READ_WARNING;

public:
    TestEngine(size_t SIZE, float frequencyMultiplier) : SIZE(SIZE), frequencyMultiplier(frequencyMultiplier)
	{
    	output_MATCH_ADDR = new SLIC_DATATYPE[SIZE];
    	output_MULTIPLE_MATCH = new SLIC_DATATYPE[SIZE];
    	output_SINGLE_MATCH = new SLIC_DATATYPE[SIZE];
    	output_MATCH = new SLIC_DATATYPE[SIZE];
    	output_BUSY = new SLIC_DATATYPE[SIZE];
    	output_READ_WARNING = new SLIC_DATATYPE[SIZE];
	}

    virtual ~TestEngine();

    virtual void generateInputValues() = 0;
    virtual void generateExpectedValues() = 0;
	void run();
    bool assert();

};

class Test1 : public TestEngine
{
public:
	Test1(float frequencyMultiplier) : TestEngine(96, frequencyMultiplier) {}

    void generateInputValues();
    void generateExpectedValues();
};

class Test2 : public TestEngine
{
public:
	Test2(float frequencyMultiplier) : TestEngine(96, frequencyMultiplier) {}

    void generateInputValues();
    void generateExpectedValues();
};

class Test3 : public TestEngine
{
public:
	Test3(float frequencyMultiplier) : TestEngine(32, frequencyMultiplier) {}

    void generateInputValues();
    void generateExpectedValues();
};

// CAM_MULTIPLE tests
#ifdef CAM_MULTIPLE
class TestM1 : public TestEngine
{
public:
	TestM1(float frequencyMultiplier) : TestEngine(64, frequencyMultiplier) {}

    void generateInputValues();
    void generateExpectedValues();
};

class TestM2 : public TestEngine
{
public:
	TestM2(float frequencyMultiplier) : TestEngine(64, frequencyMultiplier) {}

    void generateInputValues();
    void generateExpectedValues();
};
#endif

#endif
