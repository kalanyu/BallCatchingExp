#pragma once

#include "GlutWindow.h"
#include "../../EmgTool/DaqControl.h"
#include <iostream>
using namespace std;

#define PI 3.14159265358979323846

/**
筋電表示ウインドウ
*/
class GlutWindowEMG : public GlutWindow {
protected:
	/** 筋電のチャンネル数 */
	int num_channel;
	/** AD変換ボード操作オブジェクトへのポインタ */
	CDaqControl *daq;
public:
	/** コンストラクタ */
	GlutWindowEMG(CDaqControl *daq_, int num_channel_);
	void display();
	void reshape(int width_, int height_);
};
