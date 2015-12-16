#pragma once

#include "../../EmgTool/DaqControl.h"
#include "MSJointState.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

#include <math.h>
#include <stdio.h>

#include "C:\\SPIDAR\AHS2009\include\SpidarAHS\SpidarAHS.h"

//#define I 0.0081*0.4*(1 + 0.00648)
//#define B 0.05
#define I 0.0020	//����
#define D 0.064	//�S��
#define K 1.28		// �e��
//#define K 0.64		// �e��
//#define K 0		// �e��

#define INITIAL_ANGLE_VEL 0
#define INITIAL_ANGLE_ACC 0
#define GRAVITY 9.8
#define PI 3.14159265
#define MOMENT_ARM 0.1


class ControlContec
{
protected:
	/** Contec board�I�u�W�F�N�g�ւ̃|�C���^*/
	CDaqControl *daq;
	/** �؍��i�n�I�u�W�F�N�g�ւ̃|�C���^ */
	MSJointState *msjoint;
	/** �؍��i�n�I�u�W�F�N�g(stiffness�p)�ւ̃|�C���^ */
	MSJointState *msjoint_stiff;

private:
	// �v�������ۂ�
	bool flag_recording;
	// �L�����u���[�V���������ۂ�
	bool flag_calibrating;
	// �L�����u���[�V�������I�������ۂ�
	bool flag_calib_finished;
	// �_�C�i�~�N�X���v�Z�����ۂ�
	bool flag_calicurating;

	// �v������
	double tms;
	// �ؓd�M��
	double emg[4];
	// �t�B���^�������ؓd�M��
	double femg[4];
	// ���K�������ؓd�M��
	double tmp_nfemg[4];
	// ���t�ʒu
	double eqpoint;
	// 20140531 �ǋL�@�팱�҂������b�N�X���Ă���Ƃ��̎�̃g���N�i�␳�̂��߁j
	double eqpoint_base;
	// �X�e�B�t�l�X
	double stiffness;
	// �팱�҂̎肪�o���Ă���g���N
	double hand_torque;
	// 20140529 �ǋL�@�팱�҂������b�N�X���Ă���Ƃ��̎�̃g���N�i�␳�̂��߁j
	double hand_torque_base;
	// VR��̎��p�x
	double wrist_angle;
	// VR��̎��p���x
	double wrist_angle_vel;
	// VR��̎��p�����x
	double wrist_angle_acc;
	// VR��̎��p�x�̊�ʒu
	double center_line;
	// ��̏d��
	double hand_mass;
	// SPIDAR�̍���
	double spi_height;

	double visco;

	// �o�̓f�[�^
	vector< vector<double> > dynamics_data;
	int data_num;

public:
	// �R���X�g���N�^
	ControlContec(CDaqControl *daq_, MSJointState *msjoint_, MSJointState *msjoint_stiff_);
	// �f�X�g���N�^
	~ControlContec(void);

	// �v�������ۂ�
	void setFlagRecording(bool newflag);
	bool isRecording();
	// �L�����u���[�V���������ۂ�
	void setFlagCalibrating(bool newflag);
	bool isCalibrating();
	// �L�����u���[�V�������I�������ۂ�
	void setFlagCalibFinished(bool newflag);
	bool isCalibFinished();
	// �_�C�i�~�N�X�̌v�Z�����ۂ�
	void setFlagCalicurating(bool newflag);
	bool isCalicurating();
	
	// �ؓd�̍ő�E�ŏ��l���v�����Ď擾
	bool CalibEMGMinmaxStart();
	bool CalibEMGMinmaxStop();
	// �ؓd�̍ő�E�ŏ��l���e�L�X�g�f�[�^�����
	bool CalibEMGMinmaxLoad(std::string path);

	// �v���̊J�n
	bool RecordingStart();
	bool RecordingStop();
	// �_�C�i�~�N�X�̌v�Z
	bool CalculateDynamicsStart();
	bool CalculateDynamicsStop();
	void Caliculartion(double step, double ball_mass, double length, SPIDAR_HANDLE hSpidar_);	//��A�̌v�Z����̊֐��ōs��
	//EMG���擾
	// void getEMG();
	//�^�������������Ԃ��X�V
	void update(double step, double mass, double length);

	// VR��ɕ\��������p�x��Ԃ�
	double getWristAngle();
	// �X�e�B�t�l�X��Ԃ�
	double getStiffness();
	double getEQPos();
	double getTorque();
	double getEMG_ch1();
	double getEMG_ch2();
	double getEMG_ch3();
	double getEMG_ch4();

	// VR��̎��p�x�̊�ʒu�̐ݒ�
	void setCenterLine();
	double getCenterLine();

	// 20140927�ǋL�@���݂̃g���N����̏d���Ƃ��Đݒ肷��
	void setHandMass_as_Torque();

	// �f�[�^�o�͊֐�
	void storeDynamicsData();
	int writeDynamicsData(char *ch_directory);

	void up_hand_mass();
	void down_hand_mass();

};
