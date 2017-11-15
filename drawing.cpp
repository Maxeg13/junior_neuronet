#include "drawing.h"
#include "dialog.h"
extern double dx;
    myCurve::myCurve(std::vector<std::vector<float>> &dataH,QwtPlot* d_plotH,const QString &title,
            const QColor &color, int& ind_ch ):
        data(dataH),ind_c(ind_ch)
    {
        d_plot=d_plotH;
        setTitle(title);
        setPen(color,2);
//        setBrush(color, 4);
//        setBrush(QBrush(color));

    }

    myCurve::myCurve(std::vector<std::vector<float>> &dataH,QwtPlot* d_plotH,const QString &title,
            const QColor &color, const QColor &colorSymbol,int& ind_ch):data(dataH),ind_c(ind_ch)
    {
        d_plot=d_plotH;
        setTitle(title);
        setPen(color,3);
//        setRenderHint
//                ( QwtPlotItem::RenderAntialiased, true ); // сглаживание
        symbol = new QwtSymbol( QwtSymbol::Rect,
                                QBrush(colorSymbol), QPen( Qt::black, 2 ), QSize( 7, 7 ) );
        setSymbol( symbol );
    }

    void myCurve::signalDrawing()
    {
        // Добавить точки на ранее созданную кривую
        QPolygonF points;

        for (int i=0;i<data[0].size();i++)
        {
            points<<QPointF(data[0][i],data[1][i]);
        }
        setSamples( points ); // ассоциировать набор точек с кривой
        attach( d_plot); // отобразить кривую на графике/
    }

    void myCurve::pointDrawing(float x,float y)
    {
        // Добавить точки на ранее созданную кривую
        QPolygonF points;


            points<<QPointF(x,y);

        setSamples( points ); // ассоциировать набор точек с кривой
        attach( d_plot); // отобразить кривую на графике
    }
