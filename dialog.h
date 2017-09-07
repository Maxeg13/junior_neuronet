#ifndef DIALOG_H
#define DIALOG_H

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

private:
protected:
     void paintEvent(QPaintEvent *e);
public slots:
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
};

#endif // DIALOG_H
