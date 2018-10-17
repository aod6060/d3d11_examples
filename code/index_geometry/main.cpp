#include "sys.h"
#include "test.h"


int main(int argc, char** argv)
{
	AppTest test;
	app_init("Index Geometry", 1280, 720, &test);
	app_update();
	app_release();
	return 0;
}