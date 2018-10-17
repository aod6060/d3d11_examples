#include "sys.h"

int main(int argc, char** argv)
{
	app_init("Blank Window", 1280, 720, nullptr);
	app_update();
	app_release();
	return 0;
}