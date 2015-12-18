
#pragma comment(lib,"C:\\SPIDAR/AHS2009/Release/SpidarAhs.lib")
#pragma comment(lib,"C:\\Program Files/National Instruments/NI-DAQ/DAQmx ANSI C Dev/lib/msvc/NIDAQmx.lib")
#pragma comment(lib,"winmm.lib")

#include <iostream>
#include <string>
#include <ctime>
using namespace std;

#include "ControlContec.h"
#include "World.h"
#include "Drawable_Cylinder.h"
#include "Drawable_Ball.h"
#include "Drawable_Line.h"
#include "ExperimentCoordinator.h"
#include "ConstParameters.h"
#include "Timer.h"

#include "C:\\SPIDAR\AHS2009\include\SpidarAHS\SpidarAHS.h"

#include "MSJointStateShin.h"
#include "SimpleContecControl2.h"
#include "../../EmgTool/SimpleNiControl.h"
#include "../../EmgTool/DaqControl.h"

#include <windows.h>
#include <mmsystem.h>
#include <GL/glut.h>
#include <conio.h>

#define GAMEMODE 1
#define ROTATESCREEN 1
#define DO_CALIBRATION 1

static const char FNAME_SpidarXML[] = "C:\\SPIDAR/AHS2009/data/spidar_gcc_ballcatch004.xml";
SPIDAR_HANDLE g_hSpi;

//static const char FNAME_Path[] = "C:\\ResearchDocs/hama/Exp1/Exp program/BTCwithEMG ver2/BallCatching/BallCatchingExp/parameter/param_eq.txt";
//static const char FNAME_Path_Stiff[] = "C:\\ResearchDocs/hama/Exp1/Exp program/BTCwithEMG ver2/BallCatching/BallCatchingExp/parameter/param_trq.txt";
static const char FNAME_Path[] = "parameter/param_eq.txt";
static const char FNAME_Path_Stiff[] = "parameter/param_trq.txt";


static const char FNAME_ExperimentData[] = "exp_data";
static const char FNAMEHEAD_TrialData[]  = "trial_";
static const char CH_WindowTitle[] = "BallCatchingMainWindow";

static LPCSTR LP_Decision1WAV;
static LPCSTR LP_Decision2WAV;

static const int IDX_LeftButton  = 1;
static const int IDX_RightButton = 2;

static Timer t_obj;
static Drawable_Cylinder *drawobj_cylinder;
static Drawable_Line *drawobj_base;
static Drawable_Line *drawobj_hand;
static World *world;
static ExperimentCoordinator *coordinator;
static ControlContec *contec;

namespace obj{
	CDaqControl *daq = 0;
	MSJointState *msjoint = 0;
	MSJointState *msjoint_stiff = 0;
}

namespace daqs {
	char name[] = "AIO001";
	int num_channel = 4;
	bool isCalibrated = false;
};

static HWND hwMain;
static MMRESULT MMtimerIDMain;
static MMRESULT MMtimerIDDraw;

static bool flag_calib = false;
static bool flag_exp_started;
static char dirname_data[256];
static double t1ms, t2ms, dtms;

static int counter_exitwaiting = 0;
static const int CNT_Exit = 1000;

// Exp window �`���֐�
static void initGL(int argc, char **argv);
static void displayGL();
static void resizeGL(int w, int h);

static void initClassInstances();
static void initDaq();
static int setupExperiment();

static int initSPIDAR();

static void mouse(int button, int state, int x, int y);
static void keyboard(unsigned char key, int x, int y);

static void CALLBACK MMTimerProcMain(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
static void CALLBACK MMTimerProcDraw(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
static void CALLBACK SpidarProc(DWORD dwUser);

static void timerprocMain();
static void timerprocDraw();

static void endProc();


static void initGL(int argc, char **argv)
{
	glutInitWindowSize(1000, 1200);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	if(GAMEMODE) {
		glutGameModeString(gamemodestr);
		glutEnterGameMode();
	}else{
	    glutCreateWindow(CH_WindowTitle);
	}

    glutDisplayFunc(displayGL);
    glutReshapeFunc(resizeGL);
	glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glClearColor(CLR_screen[0], CLR_screen[1], CLR_screen[2], CLR_screen[3]);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, CLR_light1);

	if(GAMEMODE){
		hwMain = FindWindow((LPCSTR)"GLUT",(LPCSTR)"GLUT");
	}else{
		hwMain = FindWindow((LPCSTR)"GLUT",(LPCSTR)CH_WindowTitle);
	}
	SetForegroundWindow(hwMain);
}


static void displayGL()
{
	/*
	if(contec->isCalicurating()){
		double mass;
		double length = 0.1;
		if(world->isLoading()){
			mass = 0.9;
		}else{
			mass = 0.42;
		}
		contec->Caliculartion(dtms, mass, length);
	}
	*/
	vector<float> pos_ball;
    vector<float> pos_hand;

	if ( coordinator->isAfterTrial() && coordinator->isDecisionRequired() ) {
		glClearColor(CLR_screen_decision[0], CLR_screen_decision[1], CLR_screen_decision[2], CLR_screen_decision[3]);
	} else {
	    glClearColor(CLR_screen[0], CLR_screen[1], CLR_screen[2], CLR_screen[3]);
	}

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLightfv(GL_LIGHT0, GL_POSITION, POS_light0);
    glLightfv(GL_LIGHT1, GL_POSITION, POS_light1);

    pos_ball = drawobj_cylinder->getPos();
    pos_hand = drawobj_hand->getPos();
    pos_ball[1] = (float)world->getHeightBall();
    pos_hand[1] = (float)world->getHeightHand();
    drawobj_cylinder->setPos(pos_ball);
    drawobj_hand->setPos(pos_hand);

	if(world->isBallDisplay()){
		drawobj_cylinder->draw();
	}
	if(world->isHandDisplay()){
		drawobj_hand->draw();
	}

	drawobj_base->draw();

	glFlush();
    glutSwapBuffers();
}


static void resizeGL(int w, int h)
{
	double tmp_range_x = w*DotPitch[0]/2;
	double tmp_range_y = h*DotPitch[1]/2;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrtho(-tmp_range_x, tmp_range_x, -tmp_range_y, tmp_range_y, -1.0, 1.0);

	if(ROTATESCREEN){
		glRotated(RotationAngle, 0.0, 0.0, -1.0);
	}

	glTranslated(OriginVec[0], OriginVec[1], 0.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

static void initClassInstances(void)
{
	// �p�����[�^�ǂݍ���
	obj::msjoint = new MSJointStateShin(daqs::num_channel);
	obj::msjoint_stiff = new MSJointStateShin(daqs::num_channel);

	// 20140521�C��
	// �쐣�����̃L�����u���[�V�����ł́A�g���N�덷�ŏ�(param_trq)�Ɗp�x�덷�ŏ�(param_eq)�̓��𐄒�

//	string path = "parameter\\param_eq.txt";
//	string path_stiff = "parameter\\param_trq.txt";


	string path = string(FNAME_Path);
	string path_stiff = string(FNAME_Path_Stiff);


	if (obj::msjoint->loadParam(path) == false || obj::msjoint_stiff->loadParam(path_stiff) == false) {
		cout << "�p�����[�^�̓ǂݍ��݂Ɏ��s" << endl;
		return;
	}
/*
	string path = "parameter\\param_trq.txt";
	string path_stiff = "parameter\\param_trq.txt";
	if (obj::msjoint->loadParam(path) == false || obj::msjoint_stiff->loadParam(path_stiff) == false) {
		cout << "�p�����[�^�̓ǂݍ��݂Ɏ��s" << endl;
		return;
	}
*/
	world = new World();

	drawobj_cylinder = new Drawable_Cylinder();
	drawobj_cylinder->setColor(CLR_ball[0],CLR_ball[1],CLR_ball[2]);
	drawobj_cylinder->setRadius(Radius_ball);

	drawobj_base = new Drawable_Line();
	drawobj_base->setColor(CLR_baseline[0],CLR_baseline[1],CLR_baseline[2]);
	drawobj_base->setLength(Length_baseline);
	drawobj_base->setWidth(Width_baseline);

	drawobj_hand = new Drawable_Line();
	drawobj_hand->setColor(CLR_handline[0],CLR_handline[1],CLR_handline[2]);
	drawobj_hand->setLength(Length_handline);
	drawobj_hand->setWidth(Width_handline);

	coordinator = new ExperimentCoordinator();
	coordinator->set_Tms_before(DurationBeforeTrial);
	coordinator->set_Tms_after(DurationAfterTrial);

	contec = new ControlContec(obj::daq, obj::msjoint, obj::msjoint_stiff);

}

void initDaq()
{
	// obj::daq = new CSimpleContecControl2();
	obj::daq = new CSimpleNiControl();
	obj::daq->SetBasicParam(daqs::num_channel, 0, 2000, 500, 600000);
	obj::daq->SetInputRange(10.0, -10.0);
	obj::daq->SetDeviceName(daqs::name);
	if(!obj::daq->ConnectDevice()){
		cout << "DAQ setting error" << endl;
		delete obj::daq;
		return;
	}
	obj::daq->EnableEmgProcess(true);

	cout << "DAQ setting success" << endl;
}

static int setupExperiment()
{
	string tmp_str;
	char fname_condition[256];
	int idx_first_trial;

	cout << endl;
	cout << "Enter Trial Condition FileName then Press Return :" << endl;
    cin >> tmp_str;
    sprintf_s(fname_condition, "%s", tmp_str.c_str());
	while (coordinator->readTrialCondition(fname_condition)!=0){
        cout << "Failed to set trial condition !!!";
        cout << "   Check the filename or configuration of the file. " << endl;
        cout << endl;
        cout << "Enter Trial Condition FileName then Press Return :" << endl;
        cin >> tmp_str;
        sprintf_s(fname_condition, "%s", tmp_str.c_str());
    }

	cout << endl;
    cout << "Enter Data stored DirectoryName then Press Return :" << endl;
    cin >> tmp_str;
    sprintf_s(dirname_data, "%s", tmp_str.c_str());
	while (coordinator->setDataFileInfo(dirname_data,(char *)FNAME_ExperimentData, (char *)FNAMEHEAD_TrialData) != 0) {
		cout << "Failed to set data stored directory !!!" << endl;
		cout << endl;
		cout << "Enter Data stored DirectoryName then Press Return :" << endl;
		cin >> tmp_str;
	    sprintf_s(dirname_data, "%s", tmp_str.c_str());
	}

    cout << endl;
    cout << "Enter the number of trial you want to start (from 1 to ";
    cout << coordinator->getTotalTrialNumber() << ")." << endl;
    cin >> idx_first_trial;
    while (idx_first_trial<1 || idx_first_trial>coordinator->getTotalTrialNumber()) {
        cout << "Invalid number !!" << endl;
	    cout << "Enter the number of trial you want to start (from 1 to ";
		cout << coordinator->getTotalTrialNumber() << ")." << endl;
		cin >> idx_first_trial;
    }
	coordinator->setIndexCurrentTrial(idx_first_trial);

	return 0;
}

static int initSPIDAR(void)
{
	if (ahsCreate_Spidar(&g_hSpi,(char *)FNAME_SpidarXML,1,-1) != 0 ) {
		cout << "Cannot Create SPIDAR !!" << endl;
		return -1;
	}
	cout << "SPIDAR created with " << FNAME_SpidarXML << "." << endl;
	ahsSetImpScale(g_hSpi,1.0f,1.0f);
	ahsSetHapticLoopCallBackFunction(g_hSpi,SpidarProc);

	return 0;
}


void mouse(int button, int state, int x, int y)
{
    if (coordinator->isAfterTrial() & coordinator->isDecisionRequired()) {
        switch (state) {

            case GLUT_DOWN:
                cout << "Decsion Made as : ";

                switch (button) {
                    case GLUT_LEFT_BUTTON:
                        cout << "1st is heavier." << endl;
						coordinator->setIndexDecision(IDX_LeftButton);
						coordinator->setFlagDecisionMade(true);
						PlaySound(LP_Decision1WAV, NULL, SND_FILENAME | SND_SYNC);
						Sleep(500);
                        break;
                    case GLUT_RIGHT_BUTTON:
                        cout << "2nd is heavier." << endl;
						coordinator->setIndexDecision(IDX_RightButton);
						coordinator->setFlagDecisionMade(true);
						PlaySound(LP_Decision2WAV, NULL, SND_FILENAME | SND_ASYNC);
						Sleep(500);
                        break;
                    default:
                        break;
                }
                break;

            default:
                break;
        }
    }
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
		case 'c':
			if(DO_CALIBRATION){
				if(!contec->isCalibrating()){
					contec->CalibEMGMinmaxStart();
				}else if(contec->isCalibrating()){
					contec->CalibEMGMinmaxStop();
				}
			}else{
				if(!contec->isCalibrating()){
					string path = "minmax\\minmax_data.txt";
					contec->CalibEMGMinmaxLoad(path);
				}
			}
			break;
        case 's':
			if(!flag_exp_started){
				coordinator->startTrial(world, contec);
				flag_exp_started = true;
			}
            break;
        case 'q':
			contec->CalculateDynamicsStop();
            exit(0);
		case '\015':	//ENTER�L�[�i�W�i���j
			if(!contec->isRecording() && contec->isCalibFinished()){
				ahsActive(g_hSpi,true);
				contec->RecordingStart();
			}else if(contec->isRecording() && contec->isCalibFinished()){
				ahsActive(g_hSpi,false);
				contec->RecordingStop();
				contec->writeDynamicsData(dirname_data);
			}
			break;
		case '\040':	//SPACE�L�[�i�W�i���j
			//contec->setCenterLine();
			contec->setHandMass_as_Torque();
			break;
		case 'u':
			contec->up_hand_mass();
			break;
		case 'd':
			contec->down_hand_mass();
			break;
       case '\033':
		   if(GAMEMODE){
	            glutLeaveGameMode();
		   }

        default:
            break;
    }
}

static void CALLBACK MMTimerProcMain(
	UINT uID,
	UINT uMsg,
	DWORD dwUser,
	DWORD dw1,
	DWORD dw2)
{
	timerprocMain();
}

static void CALLBACK MMTimerProcDraw(
	UINT uID,
	UINT uMsg,
	DWORD dwUser,
	DWORD dw1,
	DWORD dw2)
{
	timerprocDraw();
}

static void CALLBACK SpidarProc(DWORD dwUser)
{
	SPIDAR_HANDLE hSpidar = (SPIDAR_HANDLE)dwUser;

	//
	// set force
	//
	//for(int i=0; i<3; i++){
	//	force[i] = 0;
	//}
	//if (world->isLoading()) {
	//	force[i] = 0;
	//}
	//ahsSetImpForce(hSpidar,force);
}



void timerprocMain()
{
	// A : Time object�Ŏ擾�����^�C�}�[(�}�C�N���܂�)

	//double dtms;
	//t2ms = t_obj.getElapsedTimeInMilliSec();
	//dtms = t2ms-t1ms;
	//t1ms = t2ms;


	// B : timeGetTime()�Ŏ擾�����^�C�}�[(�~���܂�)

	t2ms = timeGetTime();
	dtms = t2ms - t1ms;
	t1ms = t2ms;

	if(contec->isCalicurating()){
		double mass;
		double length = 0.1;
		if(world->isLoading()){
			mass = world->getWeightBall();
		}else{
			mass = 0.0;
		}
		contec->Caliculartion(dtms, mass, length, g_hSpi);
	}

	coordinator->timerFunc(world,dtms,contec);

	if(coordinator->isExperimentFinished()){
		counter_exitwaiting++;
		if(counter_exitwaiting > CNT_Exit){
			exit(0);
		}
	}
}

void timerprocDraw()
{
	RedrawWindow(NULL, NULL, NULL, RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_ALLCHILDREN);
}


void endProc()
{
	//ahsActive(g_hSpi,false);
//	Sleep(2000);
	//ahsStopHapticLoop(g_hSpi);
	//g_hSpi = ahsQuit(g_hSpi);


	delete contec;
	delete obj::daq;
	delete obj::msjoint;
	delete obj::msjoint_stiff;
	delete world;
	delete drawobj_cylinder;
	delete drawobj_base;
	delete drawobj_hand;
	delete coordinator;


	t_obj.stop();

	cout << "tihs is 'endProc'" << endl;
}



int main(int argc, char **argv)
{
	// DAQ�{�[�h�̏�����
	initDaq();
	initClassInstances();
	if(flag_calib){
		// EMG�E�B���h�E�\��
		//gluts::initWindowEMG();
		//MMtimerIDEMGWindow = timeSetEvent(TimerT_Draw_ms,1,MMTimerProcEMGWindow,(DWORD) 0,TIME_PERIODIC);
		//glutMainLoop();
	}else{
		flag_exp_started = false;
		if(setupExperiment()!=0){
			cout << "Failed to Setup Experiment !!" << endl;
			return -1;
		}

		initGL(argc,argv);

		//if(initSPIDAR()!=0){
		//	cout << "Failed to Setup Spidar !!" << endl;
		//	return -1;
		//}

		LP_Decision1WAV = (LPCSTR) Decision1WAV;
		LP_Decision2WAV = (LPCSTR) Decision2WAV;

		atexit(endProc);

		// A : Time object�Ŏ擾�����^�C�}�[(�}�C�N���܂�)
		//t_obj.start();
		//t1ms = t_obj.getElapsedTimeInMilliSec();

		// B : timeGetTime�Ŏ擾�����^�C�}�[(�~���܂�)
		t1ms = timeGetTime();

		MMtimerIDMain = timeSetEvent(TimerT_Main_ms,1,MMTimerProcMain,(DWORD) 0,TIME_PERIODIC);
		MMtimerIDDraw = timeSetEvent(TimerT_Draw_ms,1,MMTimerProcDraw,(DWORD) 0,TIME_PERIODIC);

		//ahsStartHapticLoop(g_hSpi);
		glutMainLoop();
	}




	return 0;

}
