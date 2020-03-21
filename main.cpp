#include <iostream>
#include <Windows.h>
#include "WaKei.h"

bool enabled = false;

void OnKey(int key, bool press) {

	if (press && (char)key == 'L') {
		enabled = !enabled;
		system("cls");
		std::cout << (enabled ? "Enabled\n" : "Disabled\n");
	}

}

int main() {

	WaKei::setKeyCallback(OnKey);
	WaKei::start();
	while (WaKei::isRunning()) {
		if (enabled) {
			WaKei::press('A');
		}
		Sleep(100);
	}

	return 0;

}