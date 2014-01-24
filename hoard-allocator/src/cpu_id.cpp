#include <unistd.h>
#include <iostream>
#include <string>
#include "cpu_id.h"
#include <string>
#include <thread>

using namespace std;

int get_cores_count() {
	return sysconf( _SC_NPROCESSORS_ONLN );
}