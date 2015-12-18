#include "ExperimentCoordinator.h"

#include <iostream>
#include <fstream>
#include <direct.h>
#include <stdlib.h>
#include <errno.h>
#include <string>
using namespace std;


ExperimentCoordinator::ExperimentCoordinator()
{
	LP_ballcatch_wav = (LPCSTR) StartBallCatchWAV;
	LP_perturbation_wav = (LPCSTR) StartPerturbationWAV;

	flag_experiment_finished = false;

	flag_before = false;
	flag_during = false;
	flag_after  = false;
	flag_null = true;
	flag_decision_made = false;
	flag_decision_required = false;

	state = new ExperimentState_Null();
}



int ExperimentCoordinator::readTrialCondition(char *fname)
{
	trialInfo tmp_trialinfo;
	int cnt_Ntrial = 0;
	int idx_d = 1;
	double tmp_d;

	ifstream fin(fname);
	if(!fin) { 
		cout << "Cannot Open " << fname << " file !!" << endl;
		return -1;
	}

	trials.clear();
	while (!fin.eof()) {
		fin >> tmp_d;

		switch (idx_d) {
			case 1:
				idx_d++;
				break;
			case 2:
				tmp_trialinfo.acc = tmp_d;
				idx_d++;
				break;
			case 3:
				tmp_trialinfo.weight = tmp_d;
				idx_d++;
				break;
			case 4:
				tmp_trialinfo.height0 = tmp_d;
				idx_d++;
				break;
			case 5:
				tmp_trialinfo.height_shift = tmp_d;
				idx_d++;
				break;
			case 6:
				tmp_trialinfo.height_disappear = tmp_d;
				trials.push_back(tmp_trialinfo);
				cnt_Ntrial++;
				idx_d = 1;

				cout << "Trial " << cnt_Ntrial;
				cout << ": a=" << tmp_trialinfo.acc << " ";
				cout << "w=" << tmp_trialinfo.weight << " h0=" << tmp_trialinfo.height0 << " ";
				cout << "hs= " << tmp_trialinfo.height_shift << " hd= " << tmp_trialinfo.height_disappear << " " << endl;
				break;
			default:
				break;
		}

	}

	Ntrial = cnt_Ntrial;

	fin.close();
	
	return 0;
}


int ExperimentCoordinator::setDataFileInfo(char *dirname, char *fname_exp, char *fname_trial)
{
	ch_directory = dirname;
	ch_experiment = fname_exp;
	ch_trial = fname_trial;

	//
	// make directory
	//
	if (_mkdir(ch_directory) != 0) {
		if (errno = EEXIST) {
			cout << "Directory '" << dirname << "' already exists !!" << endl;
		} else {
			cout << "Cannot Find Path to Directory '" << dirname << "' !!" << endl;
		}
		return -1;
	}
	
	return 0;
}

void ExperimentCoordinator::setTotalTrialNumber(int newN)
{
	Ntrial = newN;
}

int ExperimentCoordinator::getTotalTrialNumber()
{
	return Ntrial;
}


void ExperimentCoordinator::setIndexCurrentTrial(int newidx)
{
	IDXtrial = newidx;
}

int ExperimentCoordinator::getIndexCurrentTrial()
{
	return IDXtrial;
}

void ExperimentCoordinator::setIndexDecision(int newidx)
{
	IDXdecision = newidx;
}

int ExperimentCoordinator::getIndexDecision()
{
	return IDXdecision;
}

void ExperimentCoordinator::setFlagBeforeTrial(bool newflag)
{
	flag_before = newflag;
}

void ExperimentCoordinator::setFlagDuringTrial(bool newflag)
{
	flag_during = newflag;
}

void ExperimentCoordinator::setFlagAfterTrial(bool newflag)
{
	flag_after = newflag;
}

void ExperimentCoordinator::setFlagNull(bool newflag)
{
	flag_null = newflag;
}

void ExperimentCoordinator::setFlagDecisionMade(bool newflag)
{
	flag_decision_made = newflag;
}

void ExperimentCoordinator::setFlagDecisionRequired(bool newflag)
{
	flag_decision_required = newflag;
}

void ExperimentCoordinator::setFlagExperimentFinished(bool newflag)
{
	flag_experiment_finished = newflag;
}

bool ExperimentCoordinator::isBeforeTrial()
{
	return flag_before;
}

bool ExperimentCoordinator::isDuringTrial()
{
	return flag_during;
}

bool ExperimentCoordinator::isAfterTrial()
{
	return flag_after;
}

bool ExperimentCoordinator::isNull()
{
	return flag_null;
}

bool ExperimentCoordinator::isDecisionMade()
{
	return flag_decision_made;
}

bool ExperimentCoordinator::isDecisionRequired()
{
	return flag_decision_required;
}

bool ExperimentCoordinator::isExperimentFinished()
{
	return flag_experiment_finished;
}

void ExperimentCoordinator::set_tms_before(double newtms)
{
	tms_before = newtms;
}

void ExperimentCoordinator::set_Tms_before(double newTms)
{
	Tms_before = newTms;
}

double ExperimentCoordinator::get_tms_before()
{
	return tms_before;
}

double ExperimentCoordinator::get_Tms_before()
{
	return Tms_before;
}

void ExperimentCoordinator::set_tms_after(double newtms)
{
	tms_after = newtms;
}

void ExperimentCoordinator::set_Tms_after(double newTms)
{
	Tms_after = newTms;
}

double ExperimentCoordinator::get_tms_after()
{
	return tms_after;
}

double ExperimentCoordinator::get_Tms_after()
{
	return Tms_after;
}

void ExperimentCoordinator::startTrial(World *world, ControlContec *contec)
{
	//
	// set trial condition
	//
	trialInfo current_trial = trials[IDXtrial-1];
	if(1){
		world->setFlagTrialBallCatch(true);
	}else{
		world->setFlagTrialBallCatch(false);
	}
	world->setAccBall(current_trial.acc);
	world->setWeightBall(current_trial.weight);
	world->setHeight0Ball(current_trial.height0);
	world->setHeightHandShift(current_trial.height_shift);
	world->setHeightBallDisappear(current_trial.height_disappear);

	//
	// set experiment state to "BeforeTrial" and start state
	//
	this->setState(new ExperimentState_BeforeTrial());
	this->setFlagBeforeTrial(true);
	this->setFlagDuringTrial(false);
	this->setFlagAfterTrial(false);
	this->setFlagNull(false);
	this->setFlagDecisionMade(false);
	this->set_tms_before(0);
	this->set_tms_after(0);
	if(1){
		this->setFlagDecisionRequired(true);
		this->setIndexDecision(0);
	}else{
		this->setFlagDecisionRequired(false);
	}
	this->set_tms_before(0);
	this->set_tms_after(0);

	//
	// start calicurating
	//
	if(!contec->isCalicurating()){
		contec->CalculateDynamicsStart();
	}

	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	cout << "------- Trial : " << IDXtrial << endl;
	// -----------------------------------------------------------------------
}


void ExperimentCoordinator::timerFunc(World *world, double dtms, ControlContec *contec)
{
	state->timerFunc(this,world,dtms,contec);
	state->determineState(this,world,contec);
}


void ExperimentCoordinator::setState(ExperimentState *newstate)
{
	delete state;
	state = newstate;
}

int ExperimentCoordinator::writeTrialData(World *world)
{
	int i,j;
	char tmp_fname[200];
	vector< vector<double> > tmp_data;
	
	tmp_data = world->getTemporalData();

	trialInfo current_trial = trials[IDXtrial-1];
	current_trial.tms_fall = world->getTimeFallOnset();
	current_trial.tms_contact = world->getTimeContact();
	current_trial.tms_load = world->getTimeLoadOnset();

	//
	// store temporal data of current trial
	//
	sprintf_s(tmp_fname,"%s/%s%03d",ch_directory,ch_trial,IDXtrial);
	ofstream fout(tmp_fname);
    if (!fout) {
        cout << "Cannot write trial data file : " << tmp_fname << endl;
        return -1;
    }
    for (i=0; i<(int)tmp_data.size(); i++) {
        for (j=0; j<(int)tmp_data[i].size(); j++) {
            fout << " " << tmp_data[i][j];
        }
        fout << endl;
    }
    fout.close();

	//
	// store trial data 
	//
    sprintf_s(tmp_fname, "%s/%s", ch_directory, ch_experiment);
    ofstream fout_exp(tmp_fname,ios::app);
    if (!fout_exp) {
        cout << "Cannot write trial data file !!!" << tmp_fname << endl;
        return -1;
    }
    fout_exp << IDXtrial << " ";
    fout_exp << current_trial.acc << " ";
    fout_exp << current_trial.weight << " ";
    fout_exp << current_trial.height0 << " ";
	fout_exp << current_trial.height_shift << " ";
	fout_exp << current_trial.height_disappear << " ";
    fout_exp << current_trial.tms_fall << " ";
    fout_exp << current_trial.tms_contact << " ";
    fout_exp << current_trial.tms_load << " ";
    fout_exp << endl;
    
    fout_exp.close();

	return 0;
}


void ExperimentCoordinator::setStateNull()
{
	delete state;
	state = new ExperimentState_Null();
}


LPCSTR ExperimentCoordinator::getLPWavBallCatch()
{
	return LP_ballcatch_wav;
}


LPCSTR ExperimentCoordinator::getLPWavPerturbation()
{
	return LP_perturbation_wav;
}



// --------------------------------------------------------
// ExperimentState_BeforeTrial
// --------------------------------------------------------
ExperimentState_BeforeTrial::ExperimentState_BeforeTrial()
{
}

void ExperimentState_BeforeTrial::timerFunc(ExperimentCoordinator *coordinator, World *world, double dtms, ControlContec *contec)
{
	world->update(dtms, contec);
	coordinator->set_tms_before(coordinator->get_tms_before()+dtms);
}

void ExperimentState_BeforeTrial::determineState(ExperimentCoordinator *coordinator, World *world, ControlContec *contec)
{
	if(coordinator->get_tms_before() >= coordinator->get_Tms_before()){
		coordinator->setFlagBeforeTrial(false);
		coordinator->setFlagDuringTrial(true);
		coordinator->setState(new ExperimentState_DuringTrial());

		//PlaySound(lp_startwav, NULL, SND_FILENAME | SND_ASYNC);
		if(world->isTrialBallCatch()){
			PlaySound(coordinator->getLPWavBallCatch(), NULL, SND_FILENAME | SND_ASYNC);
		}else{
			PlaySound(coordinator->getLPWavPerturbation(), NULL, SND_FILENAME | SND_ASYNC);
		}

		world->setStateWaiting();

//      cout << "Experiment State transer from Before to During Trial" << endl;
	}
}


// --------------------------------------------------------
// ExperimentState_DuringTrial
// --------------------------------------------------------
ExperimentState_DuringTrial::ExperimentState_DuringTrial()
{	
}

void ExperimentState_DuringTrial::timerFunc(ExperimentCoordinator *coordinator, World *world, double dtms, ControlContec *contec)
{
	world->update(dtms, contec);
}

void ExperimentState_DuringTrial::determineState(ExperimentCoordinator *coordinator, World *world, ControlContec *contec)
{
	if(world->isFinished()){
		coordinator->setFlagDuringTrial(false);
		coordinator->setFlagAfterTrial(true);
		coordinator->setState(new ExperimentState_AfterTrial());

//		cout << "Experiment State transfer from During to After Trial" << endl;
	}
}


// --------------------------------------------------------
// ExperimentState_AfterTrial
// --------------------------------------------------------
ExperimentState_AfterTrial::ExperimentState_AfterTrial()
{
	
}

void ExperimentState_AfterTrial::timerFunc(ExperimentCoordinator *coordinator, World *world, double dtms, ControlContec *contec)
{
	world->update(dtms, contec);
	coordinator->set_tms_after(coordinator->get_tms_after()+dtms);
}

void ExperimentState_AfterTrial::determineState(ExperimentCoordinator *coordinator, World *world, ControlContec *contec)
{
	if(coordinator->get_tms_after() >= coordinator->get_Tms_after()){
		if(contec->isCalicurating()){
			contec->CalculateDynamicsStop();
		}
		if( !coordinator->isDecisionRequired() || (coordinator->isDecisionMade() || (coordinator->getIndexCurrentTrial() %2 != 0)) ) {
			coordinator->writeTrialData(world);
			if( coordinator->getIndexCurrentTrial() == coordinator->getTotalTrialNumber() ) {	// end of experiment
				coordinator->setFlagAfterTrial(false);
				coordinator->setFlagExperimentFinished(true);
				world->offForce();
				coordinator->setStateNull();
			}else{	
				/*
				if(coordinator->getIndexCurrentTrial() % 2 == 0) {
					string tmp_str;
					cout << "Trial ended" << endl;
					cin >> tmp_str;
					while (tmp_str.compare("a") != 0) {
						cout << "Trial ended" << endl;
						cin >> tmp_str;
					}
				} 
				*/
				// go to next trial
				coordinator->setIndexCurrentTrial(coordinator->getIndexCurrentTrial()+1);
				coordinator->setFlagAfterTrial(false);
				coordinator->startTrial(world, contec);
				
			
				//sprintf_s(fname_condition, "%s", tmp_str.c_str());
			}
		}
	}
}



// --------------------------------------------------------
// ExperimentState_Null
// --------------------------------------------------------
ExperimentState_Null::ExperimentState_Null()
{
	
}

void ExperimentState_Null::timerFunc(ExperimentCoordinator *coordinator, World *world, double dtms, ControlContec *contec)
{
	world->update(dtms, contec);
}

void ExperimentState_Null::determineState(ExperimentCoordinator *coordinator, World *world, ControlContec *contec)
{
}