#pragma warning(disable : 4819) 
//
//  World.h
//
//  Created by Hiroyuki KAMBARA on 12/03/01.
//  Copyright 2012年 Tokyo Institute of Techonology. All rights reserved.
//


#ifndef World_h
#define World_h

#include <vector>
#include "ConstParameters.h"
#include "ControlContec.h"
using namespace std;
#include <math.h>

class World;

class WorldState {
public:
    virtual void step(World *world, double dtms, ControlContec *contec) = 0;
    virtual void determineState(World *world) = 0;
};


class WorldState_Waiting : public WorldState {
public:
    WorldState_Waiting();
    void step(World *world, double dtms, ControlContec *contec);
    void determineState(World *world);
};


class WorldState_Falling : public WorldState {
public:
    WorldState_Falling();
    void step(World *world, double dtms, ControlContec *contec);
    void determineState(World *world);
};


class WorldState_Fallen : public WorldState {
public:	
	/*new*/
	int flag_fallen;
    WorldState_Fallen();
    void step(World *world, double dtms, ControlContec *contec);
    void determineState(World *world);
};

/*
class WorldState_CountingForcePerturbation : public WorldState {
public:
	WorldState_CountingForcePerturbation();
	void step(World *world, double dtms, SPIDAR_HANDLE hSpidar);
	void determineState(World *world);
};
*/


class WorldState_ConstIntervalForcePerturbation : public WorldState {
public:
	WorldState_ConstIntervalForcePerturbation();
	void step(World *world, double dtms, ControlContec *contec);
	void determineState(World *world);
};




class WorldState_Null : public WorldState {
public:
    WorldState_Null();
    void step(World *world, double dtms, ControlContec *contec);
    void determineState(World *world);
};

class World {    
private:
    WorldState *state;
    double ball_acc;            // acceleration of ball     [m/s^2]
    double ball_weight;         // weight of ball           [kg]
    double ball_radius;         // radius of ball           [m]
    double hand_thick;          // thickness of hand        [m]
    int Tms_loading;            // duration of force loading                 [ms]
    double tms;                 // current time                              [ms]
    double tms_fall_onset;      // time when ball start falling              [ms]
    double tms_load_onset;     // time when force start exerted on the hand [ms]
    double tms_contact;         // time when ball hit the hand               [ms]
    double height0_ball;        // initial height of center of ball   [m]
    double height_ball;         // current height of center of ball   [m]
    double height_hand;         // current height of center of hand   [m]
    double height_hand_base;         // 20140601 追記　current height of center of hand base   [m]
	double height_hand_shift;	// shift height of hand from center of hand	[m]
	double height_ball_disappear;	// ボールが消える高さ [m]
    double Tms_TTC0;            // total falling duration (time to contact from initial position to current hand position) [ms]
    double loadforce;			// force exerted on the hand [N]

	// 筋活性度を見る指標
	double torque;
	double eqPos;
	double stiffness;
	double emg[4];

	bool flag_loading;
	bool flag_ball_display;

    bool flag_finished;
	bool flag_trial_ballcatch;
	bool flag_hand_display;
    vector< vector<double> > temporal_data;
    
public:
    World();
    void setStateWaiting();
    void update(double dtms, ControlContec *contec);
    void setState(WorldState *newstate);
    void setAccBall(double newacc);
    void setWeightBall(double newweight);
    void setRadiusBall(double newradius);
    void setThickHand(double newthick);
    void setTimeCurrent(double newtms);
    void setTimeFallOnset(double newtms);
    void setTimeLoadOnset(double newtms);
    void setTimeContact(double newtms);
    void setHeight0Ball(double newheight);
    void setHeightBall(double newheight);
    void setHeightHand(double newheight);
    void setHeightHandBase(double newheight);			// 20140601追加
	void setHeightHand_by_angle(double wrist_angle);	// 20140529追加
	void setHeightHandShift(double newheight);
	void setHeightBallDisappear(double newheight);
	void setLoadForce(double newforce);
    void setFlagLoading(bool newflag);
	void setFlagBallDisplay(bool newflag);
    void setFlagFinished(bool newflag);
	void setFlagTrialBallCatch(bool newflag);
	void setFlagHandDisplay(bool newflag);
    double getAccBall();
    double getWeightBall();
    double getRadiusBall();
    double getThickHand();
    double getTimeCurrent();
    double getTimeFallOnset();
    double getTimeLoadOnset();
    double getTimeContact();
    double getHeight0Ball();
    double getHeightBall();
    double getHeightHand();
	double getHeightHandShift();
	double getHeightBallDisappear();
	double getLoadForce();
    bool isLoading();
	bool isBallDisplay();
    bool isFinished();
	bool isTrialBallCatch();
	bool isHandDisplay();

    void onForce();
    void offForce();
    void calTTC0();
    double getTTC0();

	// 筋活性度を見る指標の更新
	void updateMuscle(ControlContec *contec);

	void storeTemporalData();    
    vector< vector<double> > getTemporalData();
};
    

#endif
