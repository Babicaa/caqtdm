//******************************************************************************
// Copyright (c) 2012 Paul Scherrer Institut PSI), Villigen, Switzerland
// Disclaimer: neither  PSI, nor any of their employees makes any warranty
// or assumes any legal liability or responsibility for the use of this software
//******************************************************************************
//******************************************************************************
//
//     Author : Anton Chr. Mezger
//
//******************************************************************************


#ifndef CAQTDM_LIB_H
#define CAQTDM_LIB_H

#include "caQtDM_Lib_global.h"

#include "dbrString.h"
#include <QMainWindow>
#include <QTableWidget>
#include <QVector>
#include <QMutex>
#include <QFile>
#include <QMap>
#include <QtGui>
#include <QtUiTools>

#include <QWidget>
#include <QWaitCondition>
#include <QMessageBox>
#include <QPrinter>
#include <QPrintDialog>

#include "mutexKnobData.h"
#include "mutexKnobDataWrapper.h"
#include "MessageWindow.h"

#include <QtControls>

namespace Ui {
    class CaQtDM_Lib;
}

class CaQtDM_Lib : public QMainWindow
{
    Q_OBJECT

public:
    explicit CaQtDM_Lib(QWidget *parent = 0, QString="", QString="", MutexKnobData *mutexKnobData = 0, MessageWindow *msgWindow = 0);
    ~CaQtDM_Lib();

    bool bitState(int value, int bitNr);
    QString treatMacro(QMap<QString, QString> map, const QString& pv, bool *doNothing);
    int addMonitor(QWidget *thisW, knobData *data, QString pv, QWidget *w, int *specData, QMap<QString, QString> map, QString *pvRep);
    void HandleWidget(QWidget *w, QString macro);
    void closeEvent(QCloseEvent* ce);
    bool CalcVisibility(QWidget *w);
    int ComputeAlarm(QWidget *w);
    int setObjectVisibility(QWidget *w, double value);
    bool reaffectText(QMap<QString, QString> map, QString *text);
    void InitVisibility(QWidget* widget, knobData *kData, QMap<QString, QString> map,  int *specData);
    void ComputeNumericMaxMinPrec(QWidget* widget, knobData* data);
    void postMessage(QtMsgType type, char *msg);

protected:

    virtual void mouseReleaseEvent(QMouseEvent *event);

signals:
    void clicked(QString);
    void Signal_QLineEdit(const QString&, const QString&);
    void Signal_OpenNewWFile(const QString&, const QString&);
    void Signal_TextEntryChanged(const QString&);
    void Signal_ContextMenu(QWidget*);
    void clicked();

private:

    QWidget *myWidget;
    QList<QWidget*> includeWidgetList;
    QwtPlotCurve curve[3];
    QVector<double> xx, yy;
    int level;
    QString savedMacro[50];
    QProcess *proc;
    QMap<QString, QString> createMap(const QString&);

private slots:

    void processError(QProcess::ProcessError err);
    void Callback_EApplyNumeric(double value);
    void Callback_ENumeric(double value);
    void Callback_Slider(double value);
    void Callback_MessageButton(int type);

    void Callback_UpdateWidget(int, QWidget *w, const QString& units,const QString& fec, const QString& statusString,
                               knobData data,const QVector<double>& y);
    void Callback_UpdateLine(const QString&, const QString&);
    void Callback_MenuClicked(const QString&);
    void Callback_ChoiceClicked(const QString&);
    void Callback_RelatedDisplayClicked(int);
    void Callback_ShellCommandClicked(int);

    void ShowContextMenu(const QPoint&);
    void DisplayContextMenu(QWidget* w);
    void Callback_TextEntryChanged(const QString &);

    void print()
    {
        QPrinter *printer = new QPrinter;
        QPrintDialog *printDialog = new QPrintDialog(printer, this);
        if (printDialog->exec() == QDialog::Accepted) {

            QPainter painter(printer);
            printer->setOrientation(QPrinter::Landscape);
            double xscale = printer->pageRect().width()/double(this->width());
            double yscale = printer->pageRect().height()/double(this->height());
            double scale = qMin(xscale, yscale);
            painter.translate(printer->paperRect().x() + printer->pageRect().width()/2,
                               printer->paperRect().y() + printer->pageRect().height()/2);
            painter.scale(scale, scale);
            painter.translate(-width()/2, -height()/2);
            QPixmap pm = QPixmap::grabWidget(this);
            painter.drawPixmap(0, 0, pm);
        }
    }
};

#endif // CaQtDM_Lib_H


