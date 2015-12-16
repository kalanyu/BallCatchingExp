#pragma once

#include "MSJointState.h"
#include <vector>
/**
1�֐߂̋؍��i�n�̏�Ԃ��v�Z�E�ێ�����N���X
*/

class MSJointStateShin : public MSJointState {
protected:
	/** �ؓ������烂�f���̃p�����[�^�̐����v�Z���� */
	int calcParamNum(int num_channel_);
	/** �t�B���^��̋ؓd���畽�t�ʒu���v�Z����ieqpoint�̕ύX�͂��Ȃ��j */
	double calcEqPoint(std::vector<double> femg_);
	/** �t�B���^��̋ؓd����X�e�B�t�l�X���v�Z����istiffness�̕ύX�͂��Ȃ��j */
	double calcStiffness(std::vector<double> femg_);
public:
	MSJointStateShin(int num_channel_);
	bool init(std::vector<double> msparam_);
	bool pushFEMG(std::vector<double> femg_);
};
