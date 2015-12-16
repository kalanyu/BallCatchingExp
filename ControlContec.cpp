#include "ControlContec.h"

ControlContec::ControlContec(CDaqControl *daq_, MSJointState *msjoint_, MSJointState *msjoint_stiff_)
{
	daq = daq_;
	msjoint = msjoint_;
	msjoint_stiff = msjoint_stiff_;
	//hSpidar = hSpidar_;

	tms = 0;
	eqpoint = 0;
	eqpoint_base = 0;
	stiffness = 0;
	hand_torque = 0;
	hand_torque_base = 0;
	wrist_angle = 0;
	wrist_angle_vel = 0;
	wrist_angle_acc = 0;
	center_line = 0;
	hand_mass = 0.0;
	spi_height = 0;
	visco = 0.1;

	flag_recording = false;
	flag_calibrating = false;
	flag_calib_finished = false;
	flag_calicurating = false;

	data_num = 1;

}

ControlContec::~ControlContec(void)
{
}

void ControlContec::setFlagRecording(bool newflag)
{
	flag_recording = newflag;
}

bool ControlContec::isRecording()
{
	return flag_recording;	
}

void ControlContec::setFlagCalibrating(bool newflag)
{
	flag_calibrating = newflag;
}

bool ControlContec::isCalibrating()
{
	return flag_calibrating;
}

void ControlContec::setFlagCalibFinished(bool newflag)
{
	flag_calib_finished = newflag;
}

bool ControlContec::isCalibFinished()
{
	return flag_calib_finished;
}

void ControlContec::setFlagCalicurating(bool newflag)
{
	flag_calicurating = newflag;
}

bool ControlContec::isCalicurating()
{
	return flag_calicurating;
}



bool ControlContec::CalibEMGMinmaxStart()
{
	if(!this->isRecording()){
		if (!daq->StartMeasurement()) {
			return false;
		}	
		cout << "measure start" << endl;
		this->setFlagRecording(true);
		this->setFlagCalibrating(true);
		return true;
	}else{
		cout << "measure start false" << endl;
		return false;
	}
}

bool ControlContec::CalibEMGMinmaxStop()
{
	if(this->isRecording()){
		if (!daq->StopMeasurement()) {
			return false;
		}	

		this->setFlagRecording(false);
		this->setFlagCalibrating(false);
		this->setFlagCalibFinished(true);

		int nCQN, nTQN;
		double **qcurrent, **qprev;
		daq->GetQtPtr(qcurrent,qprev,nCQN,nTQN);
		int samp = 200;

		int num_ch;
		num_ch = daq->getNumInputChannels();
		cout << num_ch << endl;

		if (nCQN < samp) {
			cout << "サンプル数が足りません" << endl;
			for (int i = 0; i < num_ch; i++) {
				daq->dQtMin[i] = 0.0;
				daq->dQtMax[i] = 1.0;
			}
		}
		else {
			double min[4], max[4];
			min[0] = min[1] = min[2] = min[3] = 10000000.0;
			max[0] = max[1] = max[2] = max[3] = 0.0;
			for (int i = samp; i < nCQN; i++) {
				for (int ch = 0; ch < num_ch; ch++) {
					if (min[ch] > qcurrent[ch][i]) {
						min[ch] = qcurrent[ch][i];
					}
					if (max[ch] < qcurrent[ch][i]) {
						max[ch] = qcurrent[ch][i];
					}
				}
			}
			for (int ch = 0; ch < num_ch; ch++) {
				daq->dQtMin[ch] = min[ch];
				daq->dQtMax[ch] = max[ch];
			}
		}
		for (int i = 0; i < num_ch; i++) {
			cout << "Min:" << daq->dQtMin[i] << " Max:" << daq->dQtMax[i] << endl;
		}

		cout << "measure stop" << endl;
		return true;
	}else{
		return false;
	}
}

bool ControlContec::CalibEMGMinmaxLoad(std::string path)
{
	std::ifstream ifs(path.c_str());
	if (ifs.fail()) {
		return false;
	}
	std::cout << "Min Max:" << std::endl;
	int length;
	ifs >> length;
	double num;
	int num_ch;
	for (int i = 0; i < length; i++) {
		ifs >> num;
		if(i%2==0){
			num_ch = i/2;
			daq->dQtMin[num_ch] = num;
			std::cout << "ch" << num_ch+1 << " Min: " << num << std::endl;
		}else{
			num_ch = (int)(i/2);
			daq->dQtMax[num_ch] = num;
			std::cout << "ch" << num_ch+1 << " Max: " << num << std::endl;
		}
	}

	cout << "calib end" << endl;
	this->setFlagCalibFinished(true);	

	return true;
}

bool ControlContec::RecordingStart()
{
	if (!daq->StartMeasurement()) {
		cout << "measure start false" << endl;
		return false;
	}		

	this->setFlagRecording(true);
	this->CalculateDynamicsStart();

	return true;
}

bool ControlContec::RecordingStop()
{
	this->CalculateDynamicsStop();
	
	if (!daq->StopMeasurement()) {
		cout << "measure stop false" << endl;
		return false;
	}

	this->setFlagRecording(false);

	return true;

}

bool ControlContec::CalculateDynamicsStart()
{
	if(this->isRecording()){
		if(!this->isCalicurating()){
			// 角度・角速度・角加速度・基準位置の初期化
			// 手首角度の初期は手の重さと釣り合う位置
			//wrist_angle = eqpoint - hand_mass * GRAVITY * length / stiffness;
			wrist_angle = 0;
			//this->setCenterLine();
			wrist_angle_vel = INITIAL_ANGLE_VEL;
			wrist_angle_acc = INITIAL_ANGLE_ACC;
			tms = 0;

			cout << "calcuration start" << endl;
			this->setFlagCalicurating(true);
			return true;
		}else{
			cout << "calcuration is already started" << endl;
			return false;
		}
	}else{
		cout << "you should start recording first" << endl;
		return false;
	}
}

bool ControlContec::CalculateDynamicsStop()
{
	if(this->isRecording()){
		if(this->isCalicurating()){
			this->setFlagCalicurating(false);
			cout << "calicurate stop" << endl;
			return true;
		}else{
			cout << "calicurate is already stop" << endl;
			return false;
		}
	}else{
		cout << "you should start recording first" << endl;
		return false;
	}
}

void ControlContec::Caliculartion(double step, double ball_mass, double length, SPIDAR_HANDLE hSpidar_)
{	
	if(this->isCalicurating()){
		int num_channel = msjoint->getNumChannel();
		
		// 生波形EMG を取得
		int nCSN,nTSN;
		double **current,**prev;
		daq->GetBufPtr(current,prev,nCSN,nTSN);
		//emg.resize(num_channel);
		for (int i = 0; i < num_channel; i++) {
			emg[i] = current[i][nCSN-1];
		}

		// フィルタリングしたEMG 取得
		int nCQN, nTQN;
		double **qcurrent, **qprev;
		daq->GetQtPtr(qcurrent,qprev,nCQN,nTQN);
		for (int i = 0; i < num_channel; i++) {
			femg[i] = qcurrent[i][nCQN-1];
		}

		// 正規化したfEMG 取得
		std::vector<double> nfemg;
		nfemg.resize(num_channel);
		for (int i = 0; i < num_channel; i++) {
			nfemg[i] = daq->GetNormalizedQt(qcurrent[i][nCQN-1], i);
		}
		
		for (int i = 0; i < num_channel; i++) {
			tmp_nfemg[i] = nfemg[i];
		}
		// 筋骨格オブジェクトへemgを出力
		msjoint->pushFEMG(nfemg);
		msjoint_stiff->pushFEMG(nfemg);	

		// 平衡位置・スティフネス推定値を取得
		double tmp_eqpoint,tmp_stiffness;
		tmp_eqpoint = msjoint->getEqPoint();
		tmp_stiffness = msjoint_stiff->getStiffness();

		eqpoint = tmp_eqpoint;
		//eqpoint = 0;
		stiffness = tmp_stiffness;
		//stiffness = 0.5;

		//状態更新
		this->update(step, hand_mass + ball_mass*2, length);
		
	}else{
		eqpoint = 0;
		stiffness = 0;
		hand_torque = 0;
		wrist_angle = 0;
	}

	// データのストック
	this->storeDynamicsData();
}


void ControlContec::update(double step, double mass, double length)
{
	tms = tms + step;
	step = step*0.001;
	if(tms>1000){
		//トルク計算
		hand_torque = stiffness * (eqpoint - wrist_angle);

		double tmp_angle, tmp_dangle, tmp_ddangle;

		tmp_angle = wrist_angle;
		tmp_dangle = wrist_angle_vel;
		tmp_ddangle = wrist_angle_acc;

		// 20140603追記　±90度を越えると仮想壁から反力が加わる
		double wall_torque;
		if(wrist_angle >= PI/2){
			if (hand_torque > 0){
				wall_torque = -hand_torque;
			}
		}else if (wrist_angle <= -PI/2){
			if (hand_torque < 0){
				wall_torque = -hand_torque;
			}
		}else{
			wall_torque = 0;
		}

		// 筋の粘性を試しに入れてみる
		
		double muscle_vis;
		muscle_vis = -stiffness*0.1;
		wrist_angle_acc = (muscle_vis * tmp_dangle + hand_torque + wall_torque - mass * GRAVITY * length * cos(tmp_angle) - D * tmp_dangle - K * tmp_angle)/I;
		
		wrist_angle = tmp_angle + tmp_dangle * step + tmp_ddangle * step * step / 2;
		wrist_angle_vel = tmp_dangle + tmp_ddangle * step;

	}
	
	return ;
}

double ControlContec::getWristAngle()
{
	if(this->isCalicurating()){
		//return wrist_angle+center_line;		
		return wrist_angle;		
	}else{
		return 0;
	}
}

double ControlContec::getStiffness()
{
	return stiffness;
}

double ControlContec::getEQPos()
{
	return eqpoint;
}

double ControlContec::getTorque()
{
	return hand_torque;
}

double ControlContec::getEMG_ch1()
{
	return emg[0];
}
double ControlContec::getEMG_ch2()
{
	return emg[1];
}
double ControlContec::getEMG_ch3()
{
	return emg[2];
}
double ControlContec::getEMG_ch4()
{
	return emg[3];
}

void ControlContec::setCenterLine()
{
	eqpoint_base = 0-eqpoint;
	//center_line = -wrist_angle;
	//wrist_angle = 0;
}

double ControlContec::getCenterLine()
{
	return center_line;
}

void ControlContec::setHandMass_as_Torque()
{
	hand_mass = hand_torque / (GRAVITY * MOMENT_ARM);
	cout << "handmass is " << hand_mass << endl;
}

void ControlContec::storeDynamicsData()
{
    vector<double> current_data(17);

	current_data[0] = tms;
	current_data[1] = emg[0];
	current_data[2] = emg[1];
    current_data[3] = emg[2];
	current_data[4] = emg[3];
	current_data[5] = femg[0];
	current_data[6] = femg[1];
    current_data[7] = femg[2];
	current_data[8] = femg[3];
	current_data[9] = tmp_nfemg[0];
	current_data[10] = tmp_nfemg[1];
    current_data[11] = tmp_nfemg[2];
	current_data[12] = tmp_nfemg[3];
	current_data[13] = eqpoint;
	current_data[14] = stiffness;
    current_data[15] = wrist_angle;
	current_data[16] = hand_torque;

    dynamics_data.push_back(current_data);

}

int ControlContec::writeDynamicsData(char *ch_directory)
{
	int i,j;
	char tmp_fname[200];
	vector< vector<double> > tmp_data;
	
	tmp_data = dynamics_data;

	//
	// store dynamics data
	//
	sprintf_s(tmp_fname,"%s/dynamics_data%03d",ch_directory,data_num);
	ofstream fout(tmp_fname);
    if (!fout) {
        cout << "Cannot write dynamics data file : " << tmp_fname << endl;
        return -1;
    }
    for (i=0; i<(int)tmp_data.size(); i++) {
        for (j=0; j<(int)tmp_data[i].size(); j++) {
            fout << " " << tmp_data[i][j];
        }
        fout << endl;
    }
    fout.close();	

	data_num++;
	dynamics_data.clear();
	return 1;
}

void ControlContec::up_hand_mass()
{
	hand_mass = hand_mass + 0.01;
	cout << "handmass is " << hand_mass << endl;
}

void ControlContec::down_hand_mass()
{
	if (visco >0){
		hand_mass = hand_mass - 0.01;
	}
	cout << "handmass is " << hand_mass << endl;
}