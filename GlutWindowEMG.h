#pragma once

#include "GlutWindow.h"
#include "../../EmgTool/DaqControl.h"
#include <iostream>
using namespace std;

#define PI 3.14159265358979323846

/**
�ؓd�\���E�C���h�E
*/
class GlutWindowEMG : public GlutWindow {
protected:
	/** �ؓd�̃`�����l���� */
	int num_channel;
	/** AD�ϊ��{�[�h����I�u�W�F�N�g�ւ̃|�C���^ */
	CDaqControl *daq;
public:
	/** �R���X�g���N�^ */
	GlutWindowEMG(CDaqControl *daq_, int num_channel_);
	void display();
	void reshape(int width_, int height_);
};
