#include "MSJointStateShin.h"
#include <cassert>
#include <iostream>

MSJointStateShin::MSJointStateShin(int num_channel_)
{
	eqpoint = stiffness = target = center_line = 0.0;
	num_channel = num_channel_;
	nfemg.resize(num_channel);
	for (int i = 0; i < num_channel; i++) {
		nfemg[i] = 0.0;
	}
	is_init = false;
}

int MSJointStateShin::calcParamNum(int num_channel_)
{
	//モデルのパラメータ+中位の角度
	return 2 + 3 * num_channel_ + 1;
}

bool MSJointStateShin::init(std::vector<double> msparam_)
{
	if (calcParamNum(num_channel) != msparam_.size()) {
		return false;
	}
	msparam = msparam_;
	msparam.pop_back();
	center_line = msparam_[2+3*num_channel];
	std::cout << "Center Angle:" << center_line << std::endl;
	is_init = true;
	return true;
}

bool MSJointStateShin::pushFEMG(std::vector<double> nfemg_)
{
	if (num_channel != nfemg_.size()) {
		return false;
	}
	nfemg = nfemg_;
	if (is_init) {
		eqpoint = calcEqPoint(nfemg_);
		stiffness = calcStiffness(nfemg_);
	}
	return true;
}

double MSJointStateShin::calcEqPoint(std::vector<double> femg_)
{
	double num, den;
	int idx = 0;
	num = msparam[idx];
	idx++;
	for (int i = 0; i < num_channel; i++, idx++) {
		num += msparam[idx] * femg_[i];
	}
	for (int i = 0; i < num_channel; i++, idx++) {
		num += msparam[idx] * femg_[i] * femg_[i];
	}
	assert(idx == 1 + 2 * num_channel);
	den = msparam[idx];
	idx++;
	for (int i = 0; i < num_channel; i++, idx++) {
		den += msparam[idx] * femg_[i];
	}
	assert(idx == 2 + 3 * num_channel);
	return -num / den;
}

double MSJointStateShin::calcStiffness(std::vector<double> femg_)
{
	double ret;
	int idx = 1 + 2 * num_channel;
	ret = msparam[idx];
	idx++;
	for (int i = 0; i < num_channel; i++, idx++) {
		ret += msparam[idx] * femg_[i];
	}
	assert(idx != msparam.size() - 1);
	return -ret;
}
