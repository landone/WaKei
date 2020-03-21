#include "WaKei.h"
#include <Windows.h>
#include <thread>

static const int MAX_KEYS = VK_OEM_CLEAR + 1;
/* 0-Up,1-Released,2-Down,3-Pressed */
static byte keys[MAX_KEYS] = { 0 };
const int WaKei::SIG_BIT = 0x8000;
static std::thread updateThread;
bool WaKei::isAsync = false;
long WaKei::SLEEP_TIME = 20;
void(*WaKei::keyCallback)(int, bool) = nullptr;
static enum {
	UP = 0,
	RELEASED,
	DOWN,
	PRESSED
};

void WaKei::click(bool left) {

	INPUT input = { 0 };
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | (left ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN);
	POINT mousePos = { 0 };
	GetCursorPos(&mousePos);
	input.mi.dx = mousePos.x;
	input.mi.dy = mousePos.y;
	SendInput(1, &input, sizeof(INPUT));
	input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | (left ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_RIGHTUP);
	SendInput(1, &input, sizeof(INPUT));

}

void WaKei::press(int key) {

	INPUT input = { 0 };
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = key;
	input.ki.wScan = MapVirtualKey(key, MAPVK_VK_TO_VSC);
	SendInput(1, &input, sizeof(INPUT));
	input.mi.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(INPUT));

}

bool WaKei::isRunning() {

	return isAsync;

}

static void WaKeiUpdateLoop() {

	while (WaKei::isAsync) {

		WaKei::update();
		Sleep(WaKei::SLEEP_TIME);

	}

}

void WaKei::setUpdateFrequency(long time) {

	SLEEP_TIME = time;

}

void WaKei::setKeyCallback(void(*f)(int key, bool pressed)) {

	keyCallback = f;

}

void WaKei::start() {

	if (isAsync) {
		return;
	}

	isAsync = true;
	updateThread = std::thread(WaKeiUpdateLoop);

}

void WaKei::stop() {

	if (!isAsync) {
		return;
	}

	isAsync = false;
	if (std::this_thread::get_id() != updateThread.get_id()
		&& !updateThread.joinable()) {
		updateThread.join();
	}

}

void WaKei::update() {

	for (int i = 0; i < MAX_KEYS; i++) {
		if (GetAsyncKeyState(i) & SIG_BIT) {
			if (keys[i] < DOWN) {
				keys[i] = PRESSED;
				if (keyCallback) {
					keyCallback(i, true);
				}
			}
			else {
				keys[i] = DOWN;
			}
		}
		else {
			if (keys[i] > RELEASED) {
				keys[i] = RELEASED;
				if (keyCallback) {
					keyCallback(i, false);
				}
			}
			else {
				keys[i] = UP;
			}
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
