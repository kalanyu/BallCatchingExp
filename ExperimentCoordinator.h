#ifndef ExperimentCoordinator_h
#define ExperimentCoordinator_h

#include "World.h"
#include <windows.h>
#include <vector>
using namespace std;


class ExperimentCoordinator;


struct trialInfo {
	double acc;
	double weight;
	double radius;
	double height0;
	double height_shift;
	double height_disappear;
	double tms_fall;
	double tms_contact;
	double tms_load;
	int idx_decision;
};


class ExperimentState {
public:
	virtual void timerFunc(ExperimentCoordinator *coordinator, World *world, double dtms, ControlContec *contec) = 0;
	virtual void determineState(ExperimentCoordinator *coordinator, World *world, ControlContec *contec) = 0;
};

class ExperimentState_BeforeTrial : public ExperimentState {
public:
	ExperimentState_BeforeTrial();
	void timerFunc(ExperimentCoordinator *coordinator, World *world, double dtms, ControlContec *contec);
	void determineState(ExperimentCoordinator *coordinator, World *world, ControlContec *contec);
};

class ExperimentState_DuringTrial : public ExperimentState {
public:
	ExperimentState_DuringTrial();
	void timerFunc(ExperimentCoordinator *coordinator, World *world, double dtms, ControlContec *contec);
	void determineState(ExperimentCoordinator *coordinator, World *world, ControlContec *contec);
};

class ExperimentState_AfterTrial : public ExperimentState {
public:
	ExperimentState_AfterTrial();
	void timerFunc(ExperimentCoordinator *coordinator, World *world, double dtms, ControlContec *contec);
	void determineState(ExperimentCoordinator *coordinator, World *world, ControlContec *contec);
};

class ExperimentState_Null : public ExperimentState {
public:
	ExperimentState_Null();
	void timerFunc(ExperimentCoordinator *coordinator, World *world, double dtms, ControlContec *contec);
	void determineState(ExperimentCoordinator *coordinator, World *world, ControlContec *contec);
};



class ExperimentCoordinator {

private:
	ExperimentState *state;
	int Ntrial;
	int IDXtrial;
	vector<trialInfo> trials;
	char *ch_directory;
	char *ch_experiment;
	char *ch_trial;
	bool flag_before;
	bool flag_during;
	bool flag_after;
	bool flag_null;
	bool flag_decision_made;
	bool flag_decision_required;
	bool flag_experiment_finished;
	double tms_before;
	double Tms_before;
	double tms_after;
	double Tms_after;
	int IDXdecision;
	LPCSTR LP_ballcatch_wav;
	LPCSTR LP_perturbation_wav;

public:
	ExperimentCoordinator();
	int readTrialCondition(char *fname);
	int setDataFileInfo(char *dirname, char *fname_exp, char *fname_trial);
	void setTotalTrialNumber(int newN);
	int  getTotalTrialNumber();
	void setIndexCurrentTrial(int newidx);
	int  getIndexCurrentTrial();
	void setIndexDecision(int newidx);
	int  getIndexDecision();
	void setFlagBeforeTrial(bool newflag);
	void setFlagDuringTrial(bool newflag);
	void setFlagAfterTrial(bool newflag);
	void setFlagNull(bool newflag);
	void setFlagDecisionMade(bool newflag);
	void setFlagDecisionRequired(bool newflag);
	void setFlagExperimentFinished(bool newflag);
	bool isBeforeTrial();
	bool isDuringTrial();
	bool isAfterTrial();
	bool isNull();
	bool isDecisionMade();
	bool isDecisionRequired();
	bool isExperimentFinished();
	void set_tms_before(double newtms);
	void set_Tms_before(double newTms);
	double get_tms_before();
	double get_Tms_before();
	void set_tms_after(double newtms);
	void set_Tms_after(double newTms);
	double get_tms_after();
	double get_Tms_after();

	void startTrial(World *world, ControlContec *contec);
	void timerFunc(World *world, double dtms, ControlContec *contec);
	void setState(ExperimentState *newstate);
	int writeTrialData(World *world);

	void setStateNull();

	LPCSTR getLPWavBallCatch();
	LPCSTR getLPWavPerturbation();
};


#endif