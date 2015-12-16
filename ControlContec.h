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
#define I 0.0020	//慣性
#define D 0.064	//粘性
#define K 1.28		// 弾性
//#define K 0.64		// 弾性
//#define K 0		// 弾性

#define INITIAL_ANGLE_VEL 0
#define INITIAL_ANGLE_ACC 0
#define GRAVITY 9.8
#define PI 3.14159265
#define MOMENT_ARM 0.1


class ControlContec
{
protected:
	/** Contec boardオブジェクトへのポインタ*/
	CDaqControl *daq;
	/** 筋骨格系オブジェクトへのポインタ */
	MSJointState *msjoint;
	/** 筋骨格系オブジェクト(stiffness用)へのポインタ */
	MSJointState *msjoint_stiff;

private:
	// 計測中か否か
	bool flag_recording;
	// キャリブレーション中か否か
	bool flag_calibrating;
	// キャリブレーションを終えたか否か
	bool flag_calib_finished;
	// ダイナミクスを計算中か否か
	bool flag_calicurating;

	// 計測時間
	double tms;
	// 筋電信号
	double emg[4];
	// フィルタかけた筋電信号
	double femg[4];
	// 正規化した筋電信号
	double tmp_nfemg[4];
	// 平衡位置
	double eqpoint;
	// 20140531 追記　被験者がリラックスしているときの手のトルク（補正のため）
	double eqpoint_base;
	// スティフネス
	double stiffness;
	// 被験者の手が出しているトルク
	double hand_torque;
	// 20140529 追記　被験者がリラックスしているときの手のトルク（補正のため）
	double hand_torque_base;
	// VR上の手首角度
	double wrist_angle;
	// VR上の手首角速度
	double wrist_angle_vel;
	// VR上の手首角加速度
	double wrist_angle_acc;
	// VR上の手首角度の基準位置
	double center_line;
	// 手の重さ
	double hand_mass;
	// SPIDARの高さ
	double spi_height;

	double visco;

	// 出力データ
	vector< vector<double> > dynamics_data;
	int data_num;

public:
	// コンストラクタ
	ControlContec(CDaqControl *daq_, MSJointState *msjoint_, MSJointState *msjoint_stiff_);
	// デストラクタ
	~ControlContec(void);

	// 計測中か否か
	void setFlagRecording(bool newflag);
	bool isRecording();
	// キャリブレーション中か否か
	void setFlagCalibrating(bool newflag);
	bool isCalibrating();
	// キャリブレーションを終えたか否か
	void setFlagCalibFinished(bool newflag);
	bool isCalibFinished();
	// ダイナミクスの計算中か否か
	void setFlagCalicurating(bool newflag);
	bool isCalicurating();
	
	// 筋電の最大・最小値を計測して取得
	bool CalibEMGMinmaxStart();
	bool CalibEMGMinmaxStop();
	// 筋電の最大・最小値をテキストデータから取
	bool CalibEMGMinmaxLoad(std::string path);

	// 計測の開始
	bool RecordingStart();
	bool RecordingStop();
	// ダイナミクスの計算
	bool CalculateDynamicsStart();
	bool CalculateDynamicsStop();
	void Caliculartion(double step, double ball_mass, double length, SPIDAR_HANDLE hSpidar_);	//一連の計算を一つの関数で行う
	//EMGを取得
	// void getEMG();
	//運動方程式から状態を更新
	void update(double step, double mass, double length);

	// VR上に表示する手首角度を返す
	double getWristAngle();
	// スティフネスを返す
	double getStiffness();
	double getEQPos();
	double getTorque();
	double getEMG_ch1();
	double getEMG_ch2();
	double getEMG_ch3();
	double getEMG_ch4();

	// VR上の手首角度の基準位置の設定
	void setCenterLine();
	double getCenterLine();

	// 20140927追記　現在のトルクを手の重さとして設定する
	void setHandMass_as_Torque();

	// データ出力関数
	void storeDynamicsData();
	int writeDynamicsData(char *ch_directory);

	void up_hand_mass();
	void down_hand_mass();

};
