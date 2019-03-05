#include "interface.h"


#ifndef __MAIN_H__
#define __MAIN_H__


typedef enum TEST_RESULT_ENUM
{
	TEST_RESULT_OK = 0,
	TEST_RESULT_ERROR = 1
} TEST_RESULT_T;


TEST_RESULT_T test_main(const TEST_PARAMETER_T *ptParameter);

#endif  /* __MAIN_H__ */
