//
//  World.cpp
//
//  Created by Hiroyuki KAMBARA on 12/03/01.
//  Copyright 2012年 Tokyo Institute of Techonology. All rights reserved.
//

#include "World.h"
#include "ConstParameters.h"
#include "Triger.h"

#include <stdio.h>
#include <iostream>
#include <cmath>
using namespace std;

#include <windows.h>

static Triger *triger;


World::World()
{
    ball_radius = Radius_ball;
    hand_thick  = Width_handline;

    flag_loading = false;
    flag_finished = false;
	flag_hand_display = true;

	state = new WorldState_Null();
}

void World::storeTemporalData()
{
    vector<double> current_data(10);

	current_data[0] = tms;
	current_data[1] = height_ball;
	current_data[2] = height_hand;
	current_data[3] = emg[0];
	current_data[4] = emg[1];
	current_data[5] = emg[2];
	current_data[6] = emg[3];
	current_data[7] = torque;
	current_data[8] = eqPos;
	current_data[9] = stiffness;


    temporal_data.push_back(current_data);
}

vector< vector<double> > World::getTemporalData()
{
    return temporal_data;
}

void World::calTTC0()
{
	Tms_TTC0 = sqrt(-2*(height0_ball - height_hand - ball_radius - hand_thick/2) / ball_acc) * 1000;
}

double World::getTTC0()
{
    return Tms_TTC0;
}

void World::setStateWaiting()
{
    setState(new WorldState_Waiting());
	setFlagLoading(false);
	setFlagFinished(false);
	//
	// set initial condition
	//
	setTimeCurrent(0);
	setHeightBall(getHeight0Ball());
	setLoadForce(-9.8*getWeightBall());
	//力覚の提示なし
	//setLoadForce(0);
	//ボールを表示
	setFlagBallDisplay(true);

	offForce();

    temporal_data.clear();

	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//	cout << "World : setStateWaiting() : init world and start StateWaiting" << endl;
	// _______________________________

}

void World::update(double dtms, ControlContec *contec)
{
    state->step(this, dtms, contec);
    state->determineState(this);
}

void World::setState(WorldState *newstate)
{
    delete state;
    state = newstate;
}

void World::setAccBall(double newacc)
{
    ball_acc = newacc;
}

void World::setWeightBall(double newweight)
{
    ball_weight = newweight;
}

void World::setRadiusBall(double newradius)
{
    ball_radius = newradius;
}

void World::setThickHand(double newthick)
{
    hand_thick = newthick;
}

void World::setTimeCurrent(double newtms)
{
    tms = newtms;
}

void World::setTimeFallOnset(double newtms)
{
    tms_fall_onset = newtms;
}

void World::setTimeLoadOnset(double newtms)
{
    tms_load_onset = newtms;
}


void World::setTimeContact(double newtms)
{
    tms_contact = newtms;
}

void World::setHeight0Ball(double newheight)
{
    height0_ball = newheight;
}

void World::setHeightBall(double newheight)
{
    height_ball = newheight;
}

void World::setHeightHand(double newheight)
{
    height_hand = newheight;
}

// 20140601　追記
void World::setHeightHandBase(double newheight)
{
	height_hand_base = newheight;
}

// 20140529　追記
void World::setHeightHand_by_angle(double wrist_angle)
{
	if(wrist_angle > PI/2){
		wrist_angle = PI/2;
	}else if(wrist_angle < -PI/2){
		wrist_angle = -PI/2;
	}
	height_hand=sin(wrist_angle)*0.1 - height_hand_base;
}

void World::setHeightBallDisappear(double newheight)
{
	height_ball_disappear = newheight;
}

void World::setHeightHandShift(double newheight)
{
	height_hand_shift = newheight;
}

void World::setLoadForce(double newforce)
{
	loadforce = newforce;
}

void World::setFlagLoading(bool newflag)
{
    flag_loading = newflag;
}

void World::setFlagBallDisplay(bool newflag)
{
	flag_ball_display = newflag;
}

void World::setFlagFinished(bool newflag)
{
    flag_finished = newflag;
}

void World::setFlagTrialBallCatch(bool newflag)
{
	flag_trial_ballcatch = newflag;
}

void World::setFlagHandDisplay(bool newflag)
{
	flag_hand_display = newflag;
}


double World::getAccBall()
{
    return ball_acc;
}

double World::getWeightBall()
{
    return ball_weight;
}

double World::getRadiusBall()
{
    return ball_radius;
}

double World::getThickHand()
{
    return hand_thick;
}

double World::getTimeCurrent()
{
    return tms;
}

double World::getTimeFallOnset()
{
    return tms_fall_onset;
}

double World::getTimeLoadOnset()
{
    return tms_load_onset;
}

double World::getTimeContact()
{
    return tms_contact;
}

double World::getHeight0Ball()
{
    return height0_ball;
}

double World::getHeightBall()
{
    return height_ball;
}

double World::getHeightHand()
{
    return height_hand;
}

double World::getHeightHandShift()
{
	return height_hand_shift;
}

double World::getHeightBallDisappear()
{
	return height_ball_disappear;
}

double World::getLoadForce()
{
	return loadforce;
}

bool World::isLoading()
{
    return flag_loading;
}

bool World::isBallDisplay()
{
	return flag_ball_display;
}

bool World::isFinished()
{
    return flag_finished;
}

bool World::isTrialBallCatch()
{
	return flag_trial_ballcatch;
}

bool World::isHandDisplay()
{
	return flag_hand_display;
}


void World::onForce()
{
    if (!flag_loading) {
		flag_loading = true;
        tms_load_onset = tms;
    }
}


void World::offForce()
{
    if (flag_loading) {
        flag_loading = false;
		//tms_load_offset = tms;
    }
}

void World::updateMuscle(ControlContec *contec)
{
	torque = contec->getTorque();
	eqPos = contec->getEQPos();
	stiffness = contec->getStiffness();
	emg[0] = contec->getEMG_ch1();
	emg[1] = contec->getEMG_ch2();
	emg[2] = contec->getEMG_ch3();
	emg[3] = contec->getEMG_ch4();
}

// ---------------------------------------------
// WorldState_Waiting Class
// ---------------------------------------------

WorldState_Waiting::WorldState_Waiting()
{

}


void WorldState_Waiting::step(World *world, double dtms, ControlContec *contec)
{
	//
	// update current time
	//
    world->setTimeCurrent(world->getTimeCurrent() + dtms);

	//
	// set height of hand
	//

	double wrist_angle;
	if(contec->isCalicurating()){
		wrist_angle = contec->getWristAngle();
	}else{
		wrist_angle = 0;
	}
	//cout << "angle is " << wrist_angle << endl;
	world->setHeightHand_by_angle(wrist_angle);

	//
	// calculate total falling time
	//
	world->calTTC0();

	// 筋活性度の指標を更新
	world->updateMuscle(contec);

    //
	// store data
	//
    world->storeTemporalData();
}


void WorldState_Waiting::determineState(World *world)
{
	if (world->getTimeCurrent() > 1000) {
        world->setTimeFallOnset(world->getTimeCurrent());

		if(world->isTrialBallCatch()){
	        world->setState(new WorldState_Falling());

			// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//			cout << "World transfer From StateWaiting to StateFalling." << endl;
			// _______________________________

		}else{
			world->setState(new WorldState_ConstIntervalForcePerturbation());
			world->setFlagHandDisplay(false);

			// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//			cout << "World transfer From StateWaiting to StateForcePerturbation." << endl;
			// _______________________________

		}

    }
}



// ---------------------------------------------
// WorldState_Falling Class
// ---------------------------------------------

WorldState_Falling::WorldState_Falling()
{

}


void WorldState_Falling::step(World *world, double dtms, ControlContec *contec)
{
	//
	// update current time
	//
	world->setTimeCurrent(world->getTimeCurrent()+dtms);

	//
	// set height of hand
	//
	double wrist_angle;
	if(contec->isCalicurating()){
		wrist_angle = contec->getWristAngle();
	}else{
		wrist_angle = 0;
	}
	//cout << "angle is " << wrist_angle << endl;
	world->setHeightHand_by_angle(wrist_angle);

	//
	// set height of ball
	//
    double tmp_t = (world->getTimeCurrent() - world->getTimeFallOnset()) / 1000;
    double tmp_h = world->getHeight0Ball() + 0.5 * world->getAccBall() * tmp_t * tmp_t;
    world->setHeightBall(tmp_h);

	// ボールが40cmより低くなったら消える
	if(tmp_h < world->getHeightBallDisappear()){
		world->setFlagBallDisplay(false);
	}

	//
	// calculate total falling time
	//
	world->calTTC0();

	// 筋活性度の指標を更新
	world->updateMuscle(contec);
	//
	// load force if condition is satisfied
	//
	if ( !world->isLoading() ) {
	    if ( world->getTimeCurrent() >=
			world->getTimeFallOnset() + world->getTTC0() ) {
		    world->onForce();
			world->setFlagBallDisplay(true);

			// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//			cout << "In World StateFalling : Force Loading Onset !!" << endl;
			// ------------------------------------------------------
		}
	}

	//
	// store data
	//
    world->storeTemporalData();
}


void WorldState_Falling::determineState(World *world)
{
    double tmp_h_ball_bottom = world->getHeightBall() - world->getRadiusBall();
    double tmp_h_hand_top = world->getHeightHand() + world->getThickHand() / 2;
    if (tmp_h_ball_bottom <= tmp_h_hand_top) {
        world->setHeightBall(tmp_h_hand_top + 2);
        world->setTimeContact(world->getTimeCurrent());
		//triger->triger_on();
        world->setState(new WorldState_Fallen());

		// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//		cout << "World transfer From StateFalling to StateFallen." << endl;
		// _______________________________
    }
}



// ---------------------------------------------
// WorldState_Fallen Class
// ---------------------------------------------

WorldState_Fallen::WorldState_Fallen()
{

}


void WorldState_Fallen::step(World *world, double dtms, ControlContec *contec)
{
	//
	// update current time
	//
	world->setTimeCurrent(world->getTimeCurrent()+dtms);

	//
	// set height of hand
	double wrist_angle;
	if(contec->isCalicurating()){
		wrist_angle = contec->getWristAngle();
	}else{
		wrist_angle = 0;
	}
	world->setHeightHand_by_angle(wrist_angle);

	//
	// set height of ball synchronized with height of hand
	//
    double tmp_h = world->getHeightHand() + world->getThickHand()/2 + world->getRadiusBall();
    world->setHeightBall(tmp_h);

	//
	// load force if condition is satisfied
	//
	if ( !world->isLoading() ) {
		if (world->getTimeCurrent() >= world->getTimeContact()) {
			world->onForce();

			// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//			cout << "In World StateFallen : Force Loading Onset !!" << endl;
			// ------------------------------------------------------

		}
	}


	// 筋活性度の指標を更新
	world->updateMuscle(contec);

	//
	// store data
	//
    world->storeTemporalData();
}


void WorldState_Fallen::determineState(World *world)
{
	double tmp_t;
	if(world->isLoading()){
		tmp_t = world->getTimeCurrent() - world->getTimeLoadOnset();
		if (tmp_t>1000) {
				world->setHeightBall(1.0);
				world->offForce();
				world->setFlagFinished(true);
				//triger->triger_off();
				world->setState(new WorldState_Null());


			// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//			cout << "World transfer From StateFallen to StateNull." << endl;
			// _______________________________
		}
    }
}



/*

// ---------------------------------------------
// WorldState_CountingForcePerturbation Class
// ---------------------------------------------

WorldState_CountingForcePerturbation::WorldState_CountingForcePerturbation()
{

}

void WorldState_CountingForcePerturbation::step(World *world, double dtms, SPIDAR_HANDLE hSpidar)
{
	//
	// update current time
	//
	world->setTimeCurrent(world->getTimeCurrent()+dtms);


	//
	// set height of hand (high enough for making them invisible after load is on)
	//
	float spi_pos[3];
	if(!world->isLoading()){
		ahsGetImpPos(hSpidar,spi_pos);
		world->setHeightHand(spi_pos[1]);
	}else{
		world->setHeightHand(3.0);
	}

	//
	// set height of ball (high enough all the time)
	//
	world->setHeightBall(3.0);

	//
	// load force slightly before last beep
	//
	int tmp_idx = world->getIDXbeep();
	double tmp_t = world->getTimeCurrent() - world->getTimeFallOnset();
	if(!world->isLoading() && tmp_idx==Nbeep && tmp_t>=Interval_beep*tmp_idx-TimeMargin_force_beep){
			world->onForce();
	}

	//
	// beep if condition is satisfied
	//
	if(tmp_idx<=Nbeep && tmp_t>=Interval_beep*tmp_idx){
		Beep(Freq_beep[tmp_idx-1],Duration_beep[tmp_idx-1]);
		world->setIDXbeep(tmp_idx+1);
	}

    //
	// store data
	//
    world->storeTemporalData();
}

void WorldState_CountingForcePerturbation::determineState(World *world)
{
	double tmp_t = world->getTimeCurrent() - world->getTimeLoadOnset();

	if(tmp_t > 1000 && world->isLoading()) {
		world->offForce();
		world->setFlagFinished(true);
		world->setState(new WorldState_Null());

		// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//		cout << "World transfer From StateCountingForcePerturbation to StateNull." << endl;
		// _______________________________
	}

}


*/


// ---------------------------------------------
// WorldState_ConstIntervalForcePerturbation
// ---------------------------------------------


WorldState_ConstIntervalForcePerturbation::WorldState_ConstIntervalForcePerturbation()
{

}

void WorldState_ConstIntervalForcePerturbation::step(World *world, double dtms, ControlContec *contec)
{
	//
	// update current time
	//
	world->setTimeCurrent(world->getTimeCurrent()+dtms);

	//
	// set height of hand
	//
	double wrist_angle;
	if(contec->isCalicurating()){
		wrist_angle = contec->getWristAngle();
	}else{
		wrist_angle = 0;
	}
	world->setHeightHand_by_angle(wrist_angle);

	//
	// load force if constant interval has passed from the last second of "Waiting State"
	//
	double tmp_t = world->getTimeCurrent() - world->getTimeFallOnset();
	if(!world->isLoading() && tmp_t>=Interval_force){
			world->onForce();
	}

	// 筋活性度の指標を更新
	world->updateMuscle(contec);

    //
	// store data
	//
    world->storeTemporalData();
}



void WorldState_ConstIntervalForcePerturbation::determineState(World *world)
{
	double tmp_t = world->getTimeCurrent() - world->getTimeLoadOnset();

	if(tmp_t > 1000 && world->isLoading()) {
		world->offForce();
		world->setFlagHandDisplay(true);
		world->setFlagFinished(true);
		world->setState(new WorldState_Null());

		// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//		cout << "World transfer From StateConstIntervalForcePerturbation to StateNull." << endl;
		// _______________________________
	}
}






// ---------------------------------------------
// WorldState_Null Class
// ---------------------------------------------

WorldState_Null::WorldState_Null()
{

}

void WorldState_Null::step(World *world, double dtms, ControlContec *contec)
{
	//
	// set height of hand
	//
	double wrist_angle;
	if(contec->isCalicurating()){
		wrist_angle = contec->getWristAngle();
	}else{
		wrist_angle = 0;
	}
	//cout << "angle is " << wrist_angle << endl;
	world->setHeightHand_by_angle(wrist_angle);

	//world->setHeightHand(0)/1000;

	//cout << s/pi_pos[0] << " " << spi_pos[1] << " " << spi_pos[2] << endl;

	//
	// set height of the ball enough high so that the ball does not appear
	//
    world->setHeightBall(3.0);
}

void WorldState_Null::determineState(World *world)
{
}
