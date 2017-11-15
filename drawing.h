#ifndef DRAWING_H
#define DRAWING_H

#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_legend.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_picker.h>
#include <qwt_picker_machine.h>

class myCurve:public QwtPlotCurve
{
public:

    std::vector<std::vector<float>> &data;
    QwtPlot* d_plot;
    QwtSymbol *symbol;
    int& ind_c;
    myCurve( std::vector<std::vector<float>> &dataH,QwtPlot* d_plotH,const QString &title,
            const QColor &color,int& ind_ch );
    myCurve( std::vector<std::vector<float>> &dataH,QwtPlot* d_plotH,const QString &title,
            const QColor &color, const QColor &colorSymbol,int& ind_ch );
    void signalDrawing();
    void pointDrawing(float , float);
};



#endif // DRAWING_H
