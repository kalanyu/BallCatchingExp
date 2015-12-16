#pragma once

#include <vector>
#include <string>
/**
1関節の筋骨格系の状態を計算・保持するクラス（基本クラス）
*/

class MSJointState {
protected:
	/** 平衡位置 */
	double eqpoint;
	/** スティフネス */
	double stiffness;
	/** 実験プログラムが与えた目標位置 */
	double target;
	/** 使う筋肉の数 */
	int num_channel;
	/** 関節がまっすぐのときの角度 */
	double center_line;
	/**	筋骨格モデルのパラメータを保持する配列 */
	std::vector<double> msparam;
	/** 初期化されているかどうか */
	bool is_init;
	/** 正規化後のフィルタ後筋電 */
	std::vector<double> nfemg;
	/** TCLの目標値 */
	double target_tcl;
public:
	/** 筋肉数と、筋骨格モデルのパラメータをセット */
	virtual bool init(std::vector<double> msparam_)=0;
	/** パラメータをファイルから読み込む */
	virtual bool loadParam(std::string path);
	/** フィルタ後の筋電信号を入れ、平衡位置やスティフネスを更新する */
	virtual bool pushFEMG(std::vector<double> nfemg_)=0;
	/** 平衡位置を得る */
	virtual double getEqPoint() {return eqpoint;}
	/** スティフネスを得る */
	virtual double getStiffness() {return stiffness;}
	/** 目標位置を設定 */
	virtual bool setTarget(double target_) {target = target_; return true;}
	/** 目標位置を得る */
	virtual double getTarget() {return target;}
	/** 関節がまっすぐのときの角度を設定 */
	virtual bool setCenterLine(double center_line_) {center_line = center_line_; return true;}
	/** 関節がまっすぐのときの角度を得る */
	virtual double getCenterLine() {return center_line;}
	/** 筋肉数を得る */
	virtual int getNumChannel() {return num_channel;}
	/** 筋肉数を設定 */
	virtual bool setNumChannel(int num_channel_) {num_channel = num_channel_; return true;}
	/** 初期化されているかどうか */
	virtual bool isInit() {return is_init;}
	/** パラメータを得る */
	virtual std::vector<double> getMSParam();
	/** 筋電を得る */
	virtual std::vector<double> getFEMG() {return nfemg;}
	/** 目標TCLを設定 */
	virtual bool setTargetTCL(double target_tcl_) {target_tcl = target_tcl_; return true;}
	/** 目標TCLを得る */
	virtual double getTargetTCL() {return target_tcl;}
};
