#ifndef ConstParameters_h
#define ConstParameters_h



// Dot Pitch
//static const double DotPitch = 0.000258; // (for 23-inch Cinema Display : 1920 x 1200)
//static const double DotPitch = 0.000233; // (for 27-inch iMac Display : 2560 x 1440)
//static const double DotPitch = 0.000240;	// (for ThinkPad X60)
//static const double DotPitch = 0.000224;	// (for Sony VGN-SZ)
//static const double DotPitch[2] = {0.0002805, 0.0002805};	// (for Eizo 18-inch FlexScan L685 : 1280 x 1024)
//static const double DotPitch[2] = {0.000265, 0.000265}; // (for Princeton PTFBGF-23W)
static const double DotPitch[2] = {0.000858, 0.000808}; // (for Pioneer 50-inch PDP-503CMX : 1280 x 768) 
//static const double DotPitch[2] = {0.0001974, 0.0001974};
//static const double DotPitch[2] = {0.000294, 0.000294}; // (for Samsung 19-inch SyncMaster 910N : 1280 x 1024) 
//static const double DotPitch[2] = {0.000276, 0.000276};


// GameMode Display Setting
// static const char *gamemodestr = "1280x1024:16@75";	// for Sony VAIO VGN-SZ (external display) and Eizo 18-inch  
// static const char *gamemodestr = "2560x1440:32@10"; // for Apple 27-inch iMac
//static const char *gamemodestr = "1680x1050:32@60"; // for Princeton 23-inch and IBM ThinkPad x60
//static const char *gamemodestr = "1280x768:16@75"; // for Sony VAIO VGN-SZ (external display) and Pioneer 50-inch
//static const char *gamemodestr = "1280x1024:16@60"; // for MacBookPro and Samsung SyncMaster910N
static const char *gamemodestr = "1280x768:16@60"; // for MacBookPro and Pioneer 50-inch
//static const char *gamemodestr = "1920x1080:16@60";


// 2D vector from the center of display to the origine of the world 
//static const double OriginVec[] = {0.0, -0.1};	// [m]
static const double OriginVec[] = {0.0, -0.40};	// [m]
//static const double OriginVec[] = {0.0, 0.0}; // [m]

// Rotation Angle of Display
static const float RotationAngle = -90;


// Time interval [ms] between trials
static double DurationBeforeTrial = 1000;  // [ms]
static double DurationAfterTrial = 1000;  // [ms]

// Timer interval [ms] for update
static const unsigned int TimerT_Main_ms = 1;	// [ms]
static const unsigned int TimerT_Draw_ms  = 4;	// [ms]
static const unsigned int TimerT_EMG_ms  = 1;	// [ms]

// Background screen color during
static const float CLR_screen[] = {0.0f, 0.3f, 0.0f, 1.0f};
static const float CLR_screen_decision[] = {0.0f, 0.3f, 0.3f, 1.0f};

// Position and Color of Lights
static const float POS_light0[] = {0.0f, 2.0f, 2.0f, 1.0f};
static const float POS_light1[] = {0.0f, 0.0f, -3.0f, 1.0f};
static const float CLR_light1[] = {1.0f, 1.0f, 1.0f, 1.0f};

// Radius and Color of ball
static const double Radius_ball = 0.02;  // [m]
static const float CLR_ball[]  = {1.0f, 0.0f, 0.0f, 1.0f};

// Size and Color of column
static const double Base_Radius_column = 0.02;	// [m]
static const double Height_column = 0.04;	// [m]
static const float CLR_column[]  = {1.0f, 0.0f, 0.0f, 1.0f};

// Size and Color of base line
static const double Length_baseline = 1.0; // [m]
static const double Width_baseline  = 0.03; // [m]
static const float CLR_baseline[]  = {0.0, 0.0, 1.0, 1.0};

// Size and Color of hand pointer line
static const double Length_handline = 0.1; // [m]
static const double Width_handline  = 0.02; // [m]
static const float CLR_handline[] = {0.0f, 0.0f, 0.0f, 1.0f};

// Interval [ms] before force on during "Base Trial"
static const int Interval_force = 1000;

// Sound
static const char* StartBallCatchWAV = "sound0.wav";
static const char* StartPerturbationWAV = "sound1.wav";
static const char* Decision1WAV = "startdrag.wav";
static const char* Decision2WAV = "enddrag.wav";

// EMG Window
//const double PI = 3.14159265358979323846;
const int EMGGRAPH_WIDTH_PER_2KSAMP = 150;
const int EMGGRAPH_WIDTH = 300;
const int EMGGRAPH_HEIGHT_PER_1V = 100;
const int EMGGRAPH_HEIGHT_PER_1CH = (int)(EMGGRAPH_HEIGHT_PER_1V * 0.5 * 2);
const int EMGGRAPH_SIZE_XYPLOT = 300;
const int EMGGRAPH_POS_X = 0;
const int EMGGRAPH_POS_Y = 580;

#endif
