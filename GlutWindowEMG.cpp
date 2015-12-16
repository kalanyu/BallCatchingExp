#include "GlutWindowEMG.h"
#include <cstdlib>
#include <GL/glut.h>
#include "ConstParameters.h"

GlutWindowEMG::GlutWindowEMG(CDaqControl *daq_, int num_channel_)
{
	num_channel = num_channel_;
	daq = daq_;
	width = EMGGRAPH_WIDTH;
	height = EMGGRAPH_HEIGHT_PER_1CH * num_channel + EMGGRAPH_SIZE_XYPLOT;
	pos_x = EMGGRAPH_POS_X;
	pos_y = EMGGRAPH_POS_Y;
	name = "EMG";
}

void GlutWindowEMG::display()
{
	int nCSN,nTSN;
	double **current,**prev;

	int nCQN, nTQN;
	double **qcurrent, **qprev;
	int nGraphLength = width/EMGGRAPH_WIDTH_PER_2KSAMP*2000; //何サンプル分表示するか
	daq->GetEmgPtr(current,prev,nCSN,nTSN);
	daq->GetQtPtr(qcurrent,qprev,nCQN,nTQN);
	/*
	//描画するサンプル数が横幅のピクセル数より多い時は全サンプル描画しなくていい
	//そこで何サンプルごとに描画すればいいかを計算しておく
	int nEmgDrawSkip = nGraphLength / width;
	if(nEmgDrawSkip < 1) nEmgDrawSkip = 1;
	if(nEmgDrawSkip > 5) nEmgDrawSkip = 5;
	int nQtDrawSkip = nGraphLength / width / 10;
	if(nQtDrawSkip < 1) nQtDrawSkip = 1;
	if(nQtDrawSkip > 5) nQtDrawSkip = 5;
	*/
	int nEmgDrawSkip = 1;
	int nQtDrawSkip = 1;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	double width_world = (double)width / EMGGRAPH_WIDTH_PER_2KSAMP;
	double emg1v_world = (double)EMGGRAPH_HEIGHT_PER_1V / EMGGRAPH_HEIGHT_PER_1CH;
	double range1ch = (double)EMGGRAPH_HEIGHT_PER_1CH / 2.0 / (double)EMGGRAPH_HEIGHT_PER_1V;

	for (int ch = 0; ch < num_channel; ch++) {
		double origin_x = 0;
		double origin_y = num_channel - ch - 0.5;
		// 0Vラインの描画
		glColor3d(0.5, 0.5, 0.5);
		glBegin(GL_LINES);
		glVertex2d(origin_x, origin_y);
		glVertex2d(origin_x + width_world, origin_y);
		glEnd();
		// 信号の描画
		glColor3d(0.0, 1.0, 0.0);
		glBegin(GL_LINE_STRIP);
		glVertex2d(origin_x, origin_y);
		int lineend = 0;
		for (int i = (nCSN < nGraphLength ? 0 : nCSN-nGraphLength); i < nCSN; i += nEmgDrawSkip, lineend += nEmgDrawSkip) {
			double emg = current[ch][i];
			if (-range1ch < emg && emg < range1ch) {
				glVertex2d(origin_x + width_world * lineend / nGraphLength, origin_y + emg1v_world * emg);
			}
		}
		glEnd();
		// filted EMGの描画
		glColor3d(1.0, 1.0, 0.0);
		glBegin(GL_LINE_STRIP);
		glVertex2d(origin_x, origin_y);
		int qend = 0;
		for (int i = (nCQN < nGraphLength/10 ? 0 : nCQN-nGraphLength/10); i < nCQN; i += nQtDrawSkip, qend += nQtDrawSkip) {
			double femg = daq->GetNormalizedQt(qcurrent[ch][i], ch);
			if (femg < 1.0) {
				glVertex2d(origin_x + width_world * qend / (nGraphLength/10), origin_y +  femg * 0.5);
			}
		}
		glEnd();
		// 信号終端ラインの描画
		glColor3d(1.0, 1.0, 1.0);
		glBegin(GL_LINES);
		glVertex2d(origin_x + width_world * lineend / nGraphLength, origin_y - 0.5);
		glVertex2d(origin_x + width_world * lineend / nGraphLength, origin_y + 0.5);
		glEnd();
	}
	double unit_x_xyplot = (double)EMGGRAPH_SIZE_XYPLOT / EMGGRAPH_WIDTH_PER_2KSAMP;
	double unit_y_xyplot = (double)EMGGRAPH_SIZE_XYPLOT / EMGGRAPH_HEIGHT_PER_1CH;
	double origin_x_xyplot = 0;
	double origin_y_xyplot = num_channel;
	glColor3d(1.0, 1.0, 1.0);
	glBegin(GL_LINE_LOOP);
	glVertex2d(origin_x_xyplot, origin_y_xyplot);
	glVertex2d(origin_x_xyplot+unit_x_xyplot, origin_y_xyplot);
	glVertex2d(origin_x_xyplot+unit_x_xyplot, origin_y_xyplot+unit_y_xyplot);
	glVertex2d(origin_x_xyplot, origin_y_xyplot+unit_y_xyplot);
	glEnd();
	glColor3d(1.0, 1.0, 0.0);
	glBegin(GL_POINTS);
	for (int i = (nCQN < nGraphLength/10 ? 0 : nCQN-nGraphLength/10); i < nCQN-1; i++) {
		double femg1 = daq->GetNormalizedQt(qcurrent[0][i], 0);
		double femg2 = daq->GetNormalizedQt(qcurrent[1][i], 1);
		glVertex2d(origin_x_xyplot + femg1 * unit_x_xyplot, origin_y_xyplot +  femg2 * unit_y_xyplot);
	}
	glEnd();
	glColor3d(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	{
		double femg1 = daq->GetNormalizedQt(qcurrent[0][nCQN-1], 0);
		double femg2 = daq->GetNormalizedQt(qcurrent[1][nCQN-1], 1);
		double pixel = 1.0/EMGGRAPH_WIDTH_PER_2KSAMP;
		double x = origin_x_xyplot + femg1 * unit_x_xyplot;
		double y = origin_y_xyplot +  femg2 * unit_y_xyplot;
		int num_vertex = 10;
		double radius = 3*pixel;
		for (int i = 0; i < num_vertex; i++) {
			glVertex2d(x+radius*sin(2*PI/num_vertex*i), y+radius*cos(2*PI/num_vertex*i));
		}
	}
	glEnd();

	glutSwapBuffers();
}

void GlutWindowEMG::reshape(int width_, int height_)
{
	width = width_;
	height = height_;
	glViewport(0, 0, width, height);
	glLoadIdentity();
	glOrtho(0.0, (double)width/EMGGRAPH_WIDTH_PER_2KSAMP, 0.0, (double)height/EMGGRAPH_HEIGHT_PER_1CH, -1.0, 1.0);
}
