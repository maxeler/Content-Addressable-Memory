#include "cam_test_engine.h"

TestEngine::~TestEngine()
{
#ifdef CAM_MULTIPLE
	delete[] input_CS;
#endif
	delete[] input_WE;
	delete[] input_WR_ADDR;
	delete[] input_DIN;
	delete[] input_EN;

	delete[] output_MATCH_ADDR;
	delete[] output_MULTIPLE_MATCH;
	delete[] output_SINGLE_MATCH;
	delete[] output_MATCH;
	delete[] output_BUSY;
	delete[] output_READ_WARNING;

	delete[] expect_ASSERT;
    delete[] expect_MATCH_ADDR;
    delete[] expect_MULTIPLE_MATCH;
    delete[] expect_SINGLE_MATCH;
    delete[] expect_MATCH;
    delete[] expect_BUSY;
    delete[] expect_READ_WARNING;
}

void TestEngine::run()
{
    max_file_t* maxfile = cam_init();
    max_actions_t* actions = max_actions_init(maxfile, NULL);
    max_engine_t* engine = max_load(maxfile, "*");

    max_set_ticks(actions, "CAMCPUKernel", SIZE);

#ifdef CAM_MULTIPLE
    max_queue_input(actions, "CS", input_CS, SIZE * sizeof(SLIC_DATATYPE));
#endif
	max_queue_input(actions, "WE", input_WE, SIZE * sizeof(SLIC_DATATYPE));
	max_queue_input(actions, "WR_ADDR", input_WR_ADDR, SIZE * sizeof(SLIC_DATATYPE));
	max_queue_input(actions, "DIN", input_DIN, SIZE * sizeof(SLIC_DATATYPE));
	max_queue_input(actions, "EN", input_EN, SIZE * sizeof(SLIC_DATATYPE));
	// output
	max_queue_output(actions, "MATCH_ADDR", output_MATCH_ADDR, SIZE * sizeof(SLIC_DATATYPE));
	max_queue_output(actions, "MULTIPLE_MATCH", output_MULTIPLE_MATCH, SIZE * sizeof(SLIC_DATATYPE));
	max_queue_output(actions, "SINGLE_MATCH", output_SINGLE_MATCH, SIZE * sizeof(SLIC_DATATYPE));
	max_queue_output(actions, "MATCH", output_MATCH, SIZE * sizeof(SLIC_DATATYPE));
	max_queue_output(actions, "BUSY", output_BUSY, SIZE * sizeof(SLIC_DATATYPE));
	max_queue_output(actions, "READ_WARNING", output_READ_WARNING, SIZE * sizeof(SLIC_DATATYPE));
	// mapped memory
	for (size_t k = 0; k < cam_NUMBER_OF_CAM_ELEMENTS; k++)
	{

#ifdef CAM_MULTIPLE
		char ram_name[50] = "CAM_FSM_";
		sprintf(ram_name + 8 * sizeof(char), "%zu", k);
		strcat(ram_name, ".RAM");

		char lookup_name[50] = "CAM_FSM_";
		sprintf(lookup_name + 8 * sizeof(char), "%zu", k);
		strcat(lookup_name, ".lookup");
#else
		char ram_name[50] = "CAM_FSM.RAM";
		char lookup_name[50] = "CAM_FSM.lookup";
#endif

		for (size_t i = 0; i < RAM_SIZE; i++) // CAMCPUKernel.CAM_FSM.RAM
			max_set_mem_uint64t(actions, "CAMCPUKernel", ram_name, i, 0);
		for (size_t i = 0; i < LOOKUP_SIZE; i++) // CAMCPUKernel.CAM_FSM.RAM
			max_set_mem_uint64t(actions, "CAMCPUKernel", lookup_name, i, 0);
	}

#ifdef cam_DYNAMIC_CLOCK_ENABLED
	char* slicconf = getenv("SLIC_CONF");

	if (slicconf == NULL || strstr(slicconf, "use_simulation") == NULL)
	{
		float f = frequencyMultiplier * cam_DYNAMIC_CLOCK_FREQUENCY;

		printf("Running WITH dynamic clock of %f MHz.\n", f);
		max_set_dynamic_clock(engine, cam_DYNAMIC_CLOCK_NAME, f);
	}
	else
		printf("Running WITHOUT dynamic clock due to simulation mode enabled.\n");
#else
	printf("Running WITHOUT dynamic clock.\n");
#endif

	max_run(engine, actions);
	max_actions_free(actions);
	max_unload(engine);
}

bool TestEngine::assert()
{
    // Printing output
    printf("--------------------------------\nOUTPUT:\n");
    for (size_t i = 0; i < SIZE; i++)
        printf("  %-3zu - MATCH_ADDR = %-3d; MULTIPLE_MATCH = %-1d; SINGLE_MATCH = %-1d; MATCH = %-1d; BUSY = %-1d; READ_WARNING = %-1d\n", i, output_MATCH_ADDR[i], output_MULTIPLE_MATCH[i], output_SINGLE_MATCH[i], output_MATCH[i], output_BUSY[i], output_READ_WARNING[i]);

    uint64_t numberOfFails = 0;

    // Asserting output
    for (size_t i = 0; i < SIZE; i++) { // Ignore edge values
        if (expect_ASSERT[i] &&
        		((expect_MATCH_ADDR[i] != output_MATCH_ADDR[i]) ||
        		 (expect_MULTIPLE_MATCH[i] != output_MULTIPLE_MATCH[i]) ||
        		 (expect_SINGLE_MATCH[i] != output_SINGLE_MATCH[i]) ||
        		 (expect_MATCH[i] != output_MATCH[i]) ||
        		 (expect_BUSY[i] != output_BUSY[i]) ||
        		 (expect_READ_WARNING[i] != output_READ_WARNING[i]))){

            numberOfFails++;
        	printf("\nTest FAILED at %" PRIu64 "!\n", i);
        }
    }

    if (numberOfFails == 0)
    {
    	printf("--------------------------------\n");
    	printf("Test PASSED!\n");
    	printf("--------------------------------\n");

    	return true;
    }

    return false;
}
