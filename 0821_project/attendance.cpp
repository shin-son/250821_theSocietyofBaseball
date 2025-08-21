#include "attendance.h"

#ifdef _DEBUG
#include "gmock/gmock.h"

using namespace testing;

int main(int argc, char** argv) {
	InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

#else 
int main() {
	auto& svc = AttendanceSystem::instance();
	svc.load();
	svc.compute();
}
#endif