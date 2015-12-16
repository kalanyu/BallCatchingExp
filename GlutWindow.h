#pragma once

/**
GLUTのウインドウに関する変数・関数をまとめるクラス（基本クラス）
*/
class GlutWindow {
public:
	/** ウインドウ幅（ピクセル数） */
	int width;
	/** ウインドウ高さ（ピクセル数） */
	int height;
	/** ウインドウX座標（ピクセル数） */
	int pos_x;
	/** ウインドウY座標（ピクセル数） */
	int pos_y;
	/** ウインドウの名前 */
	char *name;
	/** コンストラクタ */
	GlutWindow() {width = height = pos_x = pos_y = 0; name = "";};
	/** 描画の処理 */
	virtual void display()=0;
	/** リサイズ時の処理 */
	virtual void reshape(int width_, int height_)=0;
	/** glutCreateWindowでもらえるウインドウID */
	int window_id;
};
