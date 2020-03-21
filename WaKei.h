#pragma once

/*
	Mouse and keyboard interface
*/
class WaKei {
public:

	/* Start async update thread */
	static void start();
	/* End async update thread */
	static void stop();
	/* Is async thread running */
	static bool isRunning();
	/* Update current input data */
	static void update();
	/* Is the key down since last update */
	static bool isKeyDown(int);
	/* Was key pressed most recent update */
	static bool isKeyPressed(int);
	/* Was key released most recent update */
	static bool isKeyReleased(int);
	/* Callback for key press and release */
	static void setKeyCallback(void(*f)(int key, bool pressed));
	/* Set async update loop sleep time */
	static void setUpdateFrequency(long);
	/* Simulate mouse click */
	static void click(bool left = true);
	/* Simulate key press */
	static void press(int key);

private:

	friend void WaKeiUpdateLoop();

	static const int SIG_BIT;
	static bool isAsync;
	static long SLEEP_TIME;
	static void(*keyCallback)(int, bool);

};