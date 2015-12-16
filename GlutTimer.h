#pragma once

/**
GLUTのタイマーを操作するクラス
*/

class GlutTimer {
protected:
	/** タイマーで定期的に呼び出す関数 */
	static void (*func)();
	/** タイマーの間隔[ms] */
	static int interval_ms;
	/** タイマーを動作させるかどうか */
	static bool isOn;
	/** glutTimerFuncで呼び出すよう指定する関数 */
	static void realTimerFunc(int value);
public:
	/** コンストラクタ */
	GlutTimer(void(*func_)(), double interval_s_);
	/** タイマーをスタートさせる */
	bool start();
	/** タイマーを止める */
	bool stop();
};
