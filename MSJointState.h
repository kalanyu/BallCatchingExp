#pragma once

#include <vector>
#include <string>
/**
1�֐߂̋؍��i�n�̏�Ԃ��v�Z�E�ێ�����N���X�i��{�N���X�j
*/

class MSJointState {
protected:
	/** ���t�ʒu */
	double eqpoint;
	/** �X�e�B�t�l�X */
	double stiffness;
	/** �����v���O�������^�����ڕW�ʒu */
	double target;
	/** �g���ؓ��̐� */
	int num_channel;
	/** �֐߂��܂������̂Ƃ��̊p�x */
	double center_line;
	/**	�؍��i���f���̃p�����[�^��ێ�����z�� */
	std::vector<double> msparam;
	/** ����������Ă��邩�ǂ��� */
	bool is_init;
	/** ���K����̃t�B���^��ؓd */
	std::vector<double> nfemg;
	/** TCL�̖ڕW�l */
	double target_tcl;
public:
	/** �ؓ����ƁA�؍��i���f���̃p�����[�^���Z�b�g */
	virtual bool init(std::vector<double> msparam_)=0;
	/** �p�����[�^���t�@�C������ǂݍ��� */
	virtual bool loadParam(std::string path);
	/** �t�B���^��̋ؓd�M�������A���t�ʒu��X�e�B�t�l�X���X�V���� */
	virtual bool pushFEMG(std::vector<double> nfemg_)=0;
	/** ���t�ʒu�𓾂� */
	virtual double getEqPoint() {return eqpoint;}
	/** �X�e�B�t�l�X�𓾂� */
	virtual double getStiffness() {return stiffness;}
	/** �ڕW�ʒu��ݒ� */
	virtual bool setTarget(double target_) {target = target_; return true;}
	/** �ڕW�ʒu�𓾂� */
	virtual double getTarget() {return target;}
	/** �֐߂��܂������̂Ƃ��̊p�x��ݒ� */
	virtual bool setCenterLine(double center_line_) {center_line = center_line_; return true;}
	/** �֐߂��܂������̂Ƃ��̊p�x�𓾂� */
	virtual double getCenterLine() {return center_line;}
	/** �ؓ����𓾂� */
	virtual int getNumChannel() {return num_channel;}
	/** �ؓ�����ݒ� */
	virtual bool setNumChannel(int num_channel_) {num_channel = num_channel_; return true;}
	/** ����������Ă��邩�ǂ��� */
	virtual bool isInit() {return is_init;}
	/** �p�����[�^�𓾂� */
	virtual std::vector<double> getMSParam();
	/** �ؓd�𓾂� */
	virtual std::vector<double> getFEMG() {return nfemg;}
	/** �ڕWTCL��ݒ� */
	virtual bool setTargetTCL(double target_tcl_) {target_tcl = target_tcl_; return true;}
	/** �ڕWTCL�𓾂� */
	virtual double getTargetTCL() {return target_tcl;}
};
