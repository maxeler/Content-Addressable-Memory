#include "cam_test_engine.h"

int main(int argc, char** argv) {
	float frequencyMultiplier = 1;

	printf("--------------------------------\n");

	if (argc >= 2)
		frequencyMultiplier = (float)atof(argv[1]);
	else
		printf("Dynamic clock frequency not provided. Multiplier of 1 is used by default.\n");

	uint64_t NUMBER_OF_TESTS = 3;
#ifdef CAM_MULTIPLE
	NUMBER_OF_TESTS += 2;
#endif
	uint64_t numberOfPassedTests = 0;

	TestEngine** tests = new TestEngine*[NUMBER_OF_TESTS];

	tests[0] = new Test1(frequencyMultiplier);
	tests[1] = new Test2(frequencyMultiplier);
	tests[2] = new Test3(frequencyMultiplier);
#ifdef CAM_MULTIPLE
	tests[3] = new TestM1(frequencyMultiplier);
	tests[4] = new TestM2(frequencyMultiplier);
#endif

	for (size_t i = 0; i < NUMBER_OF_TESTS; i++)
	{
		tests[i]->generateInputValues();
		tests[i]->generateExpectedValues();
		tests[i]->run();
		bool result = tests[i]->assert();
		if (result)
			numberOfPassedTests++;
	}

	printf("--------------------------------\n");
	printf("NUMBER OF TESTS PASSED: %zu/%zu\n", numberOfPassedTests, NUMBER_OF_TESTS);
	printf("--------------------------------\n");

	for (size_t i = 0; i < NUMBER_OF_TESTS; i++)
		delete tests[i];
	delete[] tests;

	return 0;
}
