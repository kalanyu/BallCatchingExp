#include "MSJointState.h"
#include <iostream>
#include <fstream>

bool MSJointState::loadParam(std::string path)
{
	std::ifstream ifs(path.c_str());
	if (ifs.fail()) {
		std::cout << "Error in opening file : " << path << std::endl;
		return false;
	}
	std::vector<double> param_;
	std::cout << "Param:" << std::endl;
	int length;
	ifs >> length;
	double num;
	for (int i = 0; i < length; i++) {
		ifs >> num;
		param_.push_back(num);
		std::cout << i + 1 << " " << num << std::endl;
	}
	return init(param_);
}

std::vector<double> MSJointState::getMSParam()
{
	return msparam;
}
