#pragma once

/**
GLUT�̃^�C�}�[�𑀍삷��N���X
*/

class GlutTimer {
protected:
	/** �^�C�}�[�Œ���I�ɌĂяo���֐� */
	static void (*func)();
	/** �^�C�}�[�̊Ԋu[ms] */
	static int interval_ms;
	/** �^�C�}�[�𓮍삳���邩�ǂ��� */
	static bool isOn;
	/** glutTimerFunc�ŌĂяo���悤�w�肷��֐� */
	static void realTimerFunc(int value);
public:
	/** �R���X�g���N�^ */
	GlutTimer(void(*func_)(), double interval_s_);
	/** �^�C�}�[���X�^�[�g������ */
	bool start();
	/** �^�C�}�[���~�߂� */
	bool stop();
};
