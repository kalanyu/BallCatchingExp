#pragma once

/**
GLUT�̃E�C���h�E�Ɋւ���ϐ��E�֐����܂Ƃ߂�N���X�i��{�N���X�j
*/
class GlutWindow {
public:
	/** �E�C���h�E���i�s�N�Z�����j */
	int width;
	/** �E�C���h�E�����i�s�N�Z�����j */
	int height;
	/** �E�C���h�EX���W�i�s�N�Z�����j */
	int pos_x;
	/** �E�C���h�EY���W�i�s�N�Z�����j */
	int pos_y;
	/** �E�C���h�E�̖��O */
	char *name;
	/** �R���X�g���N�^ */
	GlutWindow() {width = height = pos_x = pos_y = 0; name = "";};
	/** �`��̏��� */
	virtual void display()=0;
	/** ���T�C�Y���̏��� */
	virtual void reshape(int width_, int height_)=0;
	/** glutCreateWindow�ł��炦��E�C���h�EID */
	int window_id;
};
