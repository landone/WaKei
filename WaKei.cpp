#include "WaKei.h"
#include <Windows.h>

static const int MAX_KEYS = VK_OEM_CLEAR + 1;
/* 0-Up,1-Released,2-Down,3-Pressed */
static byte keys[MAX_KEYS] = { 0 };
static const int SIG_BIT = 0x8000;
static enum {
	UP = 0,
	RELEASED,
	DOWN,
	PRESSED
};

void WaKei::update() {

	for (int i = 0; i < MAX_KEYS; i++) {
		if (GetAsyncKeyState(i) & SIG_BIT) {
			keys[i] = (keys[i] ? DOWN : PRESSED);
		}
		else {
			keys[i] = (!keys[i] ? UP : RELEASED);
		}
	}

}

bool WaKei::isKeyDown(int k) {
	return keys[k] >= DOWN;
}

bool WaKei::isKeyPressed(int k) {
	return keys[k] == PRESSED;
}

bool WaKei::isKeyReleased(int k) {
	return keys[k] == RELEASED;
}