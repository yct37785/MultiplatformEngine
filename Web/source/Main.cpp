#include "OSEngine.h"

int main()
{
	// test to determine mem. leaks are false positives
	/*char* testChar = new char;
	*testChar = 'a';*/

	OSEngine::instance()->Init();
	OSEngine::instance()->Run();

	// delete all instances
	delete OSEngine::instance();

	// test to determine mem. leaks are false positives
	// notice there's no detected leak in between testChar and testInt
	/*int* testInt = new int;
	*testInt = 8;*/
}