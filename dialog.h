#ifndef DIALOG_H
#define DIALOG_H
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_legend.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_picker.h>
#include <qwt_picker_machine.h>
#include "drawing.h"
#include <QDialog>




class Dialog : public QDialog
{
    Q_OBJECT

public:
    void mousePressEvent(QMouseEvent *);
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    //    paintEvent(QPaintEvent*);
    void mainCircle();
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void drawingInit(QwtPlot* ,QString);
private:
protected:
     void paintEvent(QPaintEvent *e);
public slots:
     void changePoisInterval();
     void drawing();
     void neuroGrab();
     void trySlider(int x);
     void trySlider2(int x);
     void spikesStop();
     void weightRadChanged();
     void setMinWeight();
     void setMaxWeight();
     void currentChange(int x);
     void pull_change();
     void setInhPerc();
     void freqChange();
     void change_STDP_speed( );
     void changeWeight();
     void killDelay();
     void savePattern();
     void setPattern(int i=0);
     void startLearning();
     void chooseThePattern();
     void setPhase();
     void changePoisson();
};

#endif // DIALOG_H
