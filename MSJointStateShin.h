#pragma once

#include "MSJointState.h"
#include <vector>
/**
1関節の筋骨格系の状態を計算・保持するクラス
*/

class MSJointStateShin : public MSJointState {
protected:
	/** 筋肉数からモデルのパラメータの数を計算する */
	int calcParamNum(int num_channel_);
	/** フィルタ後の筋電から平衡位置を計算する（eqpointの変更はしない） */
	double calcEqPoint(std::vector<double> femg_);
	/** フィルタ後の筋電からスティフネスを計算する（stiffnessの変更はしない） */
	double calcStiffness(std::vector<double> femg_);
public:
	MSJointStateShin(int num_channel_);
	bool init(std::vector<double> msparam_);
	bool pushFEMG(std::vector<double> femg_);
};
