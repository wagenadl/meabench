/********************************************************************************
** Form generated from reading UI file 'Raster.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RASTER_H
#define UI_RASTER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "QMultiRaster.H"

QT_BEGIN_NAMESPACE

class Ui_Raster
{
public:
    QGridLayout *gridLayout;
    QFrame *ctrlpanel;
    QVBoxLayout *vboxLayout;
    QFrame *ctrl1;
    QHBoxLayout *hboxLayout;
    QFrame *rangeframe_2;
    QHBoxLayout *hboxLayout1;
    QLabel *rangelabel_2;
    QComboBox *trigch;
    QLabel *tolabel_2;
    QComboBox *secch;
    QFrame *pretrigframe;
    QHBoxLayout *hboxLayout2;
    QLabel *pretriglabel;
    QSpinBox *pretrig;
    QFrame *posttrigframe;
    QHBoxLayout *hboxLayout3;
    QLabel *posttriglabel;
    QSpinBox *posttrig;
    QFrame *triggersframe;
    QHBoxLayout *hboxLayout4;
    QLabel *triggerslabel;
    QSpinBox *pixpertrig;
    QFrame *symbolsizeframe;
    QHBoxLayout *hboxLayout5;
    QSpacerItem *ctrl1_spacer;
    QCheckBox *dbxon;
    QFrame *ctrl2;
    QHBoxLayout *hboxLayout6;
    QFrame *spikesrcframe_2;
    QHBoxLayout *hboxLayout7;
    QFrame *spikesrcframe1_2;
    QHBoxLayout *hboxLayout8;
    QLabel *spikesrclabel_2;
    QComboBox *spikesrc;
    QLabel *spikestatus;
    QCheckBox *downonly;
    QFrame *posttrigframe_2;
    QHBoxLayout *hboxLayout9;
    QCheckBox *usethresh;
    QSpinBox *threshval;
    QLabel *posttriglabel_2_2;
    QFrame *expander_2;
    QHBoxLayout *hboxLayout10;
    QCheckBox *hex;
    QSpacerItem *Spacer2;
    QCheckBox *freezeflag;
    QLabel *time;
    QLabel *trigger;
    QFrame *Line1;
    QMultiRaster *rasters;

    void setupUi(QWidget *Raster)
    {
        if (Raster->objectName().isEmpty())
            Raster->setObjectName(QString::fromUtf8("Raster"));
        Raster->resize(1147, 927);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Raster->sizePolicy().hasHeightForWidth());
        Raster->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(Raster);
        gridLayout->setSpacing(5);
        gridLayout->setContentsMargins(5, 5, 5, 5);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        ctrlpanel = new QFrame(Raster);
        ctrlpanel->setObjectName(QString::fromUtf8("ctrlpanel"));
        ctrlpanel->setFrameShape(QFrame::NoFrame);
        ctrlpanel->setFrameShadow(QFrame::Raised);
        vboxLayout = new QVBoxLayout(ctrlpanel);
        vboxLayout->setSpacing(5);
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        ctrl1 = new QFrame(ctrlpanel);
        ctrl1->setObjectName(QString::fromUtf8("ctrl1"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(ctrl1->sizePolicy().hasHeightForWidth());
        ctrl1->setSizePolicy(sizePolicy1);
        ctrl1->setMinimumSize(QSize(0, 30));
        ctrl1->setMaximumSize(QSize(32767, 30));
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Active, QPalette::Button, brush);
        palette.setBrush(QPalette::Active, QPalette::Light, brush);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Active, QPalette::Window, brush);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Active, QPalette::Highlight, brush);
        palette.setBrush(QPalette::Active, QPalette::HighlightedText, brush);
        palette.setBrush(QPalette::Active, QPalette::Link, brush);
        palette.setBrush(QPalette::Active, QPalette::LinkVisited, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Highlight, brush);
        palette.setBrush(QPalette::Inactive, QPalette::HighlightedText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Link, brush);
        palette.setBrush(QPalette::Inactive, QPalette::LinkVisited, brush);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Highlight, brush);
        palette.setBrush(QPalette::Disabled, QPalette::HighlightedText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Link, brush);
        palette.setBrush(QPalette::Disabled, QPalette::LinkVisited, brush);
        ctrl1->setPalette(palette);
        ctrl1->setFrameShape(QFrame::NoFrame);
        ctrl1->setFrameShadow(QFrame::Plain);
        hboxLayout = new QHBoxLayout(ctrl1);
        hboxLayout->setSpacing(10);
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        rangeframe_2 = new QFrame(ctrl1);
        rangeframe_2->setObjectName(QString::fromUtf8("rangeframe_2"));
        rangeframe_2->setEnabled(true);
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(rangeframe_2->sizePolicy().hasHeightForWidth());
        rangeframe_2->setSizePolicy(sizePolicy2);
        rangeframe_2->setFrameShape(QFrame::NoFrame);
        rangeframe_2->setFrameShadow(QFrame::Raised);
        hboxLayout1 = new QHBoxLayout(rangeframe_2);
        hboxLayout1->setSpacing(2);
        hboxLayout1->setContentsMargins(0, 0, 0, 0);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        rangelabel_2 = new QLabel(rangeframe_2);
        rangelabel_2->setObjectName(QString::fromUtf8("rangelabel_2"));
        rangelabel_2->setWordWrap(false);

        hboxLayout1->addWidget(rangelabel_2);

        trigch = new QComboBox(rangeframe_2);
        trigch->setObjectName(QString::fromUtf8("trigch"));
        QSizePolicy sizePolicy3(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(trigch->sizePolicy().hasHeightForWidth());
        trigch->setSizePolicy(sizePolicy3);
        trigch->setMinimumSize(QSize(30, 0));
        trigch->setMaximumSize(QSize(50, 32767));

        hboxLayout1->addWidget(trigch);

        tolabel_2 = new QLabel(rangeframe_2);
        tolabel_2->setObjectName(QString::fromUtf8("tolabel_2"));
        tolabel_2->setEnabled(true);
        tolabel_2->setWordWrap(false);

        hboxLayout1->addWidget(tolabel_2);

        secch = new QComboBox(rangeframe_2);
        secch->setObjectName(QString::fromUtf8("secch"));
        sizePolicy3.setHeightForWidth(secch->sizePolicy().hasHeightForWidth());
        secch->setSizePolicy(sizePolicy3);
        secch->setMaximumSize(QSize(50, 32767));

        hboxLayout1->addWidget(secch);


        hboxLayout->addWidget(rangeframe_2);

        pretrigframe = new QFrame(ctrl1);
        pretrigframe->setObjectName(QString::fromUtf8("pretrigframe"));
        sizePolicy2.setHeightForWidth(pretrigframe->sizePolicy().hasHeightForWidth());
        pretrigframe->setSizePolicy(sizePolicy2);
        pretrigframe->setFrameShape(QFrame::NoFrame);
        pretrigframe->setFrameShadow(QFrame::Raised);
        hboxLayout2 = new QHBoxLayout(pretrigframe);
        hboxLayout2->setSpacing(5);
        hboxLayout2->setContentsMargins(0, 0, 0, 0);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        pretriglabel = new QLabel(pretrigframe);
        pretriglabel->setObjectName(QString::fromUtf8("pretriglabel"));
        pretriglabel->setEnabled(true);
        pretriglabel->setWordWrap(false);

        hboxLayout2->addWidget(pretriglabel);

        pretrig = new QSpinBox(pretrigframe);
        pretrig->setObjectName(QString::fromUtf8("pretrig"));
        pretrig->setEnabled(true);
        pretrig->setMinimum(0);
        pretrig->setMaximum(1000);
        pretrig->setSingleStep(10);
        pretrig->setValue(10);

        hboxLayout2->addWidget(pretrig);


        hboxLayout->addWidget(pretrigframe);

        posttrigframe = new QFrame(ctrl1);
        posttrigframe->setObjectName(QString::fromUtf8("posttrigframe"));
        sizePolicy2.setHeightForWidth(posttrigframe->sizePolicy().hasHeightForWidth());
        posttrigframe->setSizePolicy(sizePolicy2);
        posttrigframe->setFrameShape(QFrame::NoFrame);
        posttrigframe->setFrameShadow(QFrame::Raised);
        hboxLayout3 = new QHBoxLayout(posttrigframe);
        hboxLayout3->setSpacing(5);
        hboxLayout3->setContentsMargins(0, 0, 0, 0);
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        posttriglabel = new QLabel(posttrigframe);
        posttriglabel->setObjectName(QString::fromUtf8("posttriglabel"));
        posttriglabel->setWordWrap(false);

        hboxLayout3->addWidget(posttriglabel);

        posttrig = new QSpinBox(posttrigframe);
        posttrig->setObjectName(QString::fromUtf8("posttrig"));
        posttrig->setMinimum(10);
        posttrig->setMaximum(1000);
        posttrig->setSingleStep(10);
        posttrig->setValue(40);

        hboxLayout3->addWidget(posttrig);


        hboxLayout->addWidget(posttrigframe);

        triggersframe = new QFrame(ctrl1);
        triggersframe->setObjectName(QString::fromUtf8("triggersframe"));
        sizePolicy2.setHeightForWidth(triggersframe->sizePolicy().hasHeightForWidth());
        triggersframe->setSizePolicy(sizePolicy2);
        triggersframe->setFrameShape(QFrame::NoFrame);
        triggersframe->setFrameShadow(QFrame::Raised);
        hboxLayout4 = new QHBoxLayout(triggersframe);
        hboxLayout4->setSpacing(5);
        hboxLayout4->setContentsMargins(0, 0, 0, 0);
        hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
        triggerslabel = new QLabel(triggersframe);
        triggerslabel->setObjectName(QString::fromUtf8("triggerslabel"));
        triggerslabel->setEnabled(true);
        triggerslabel->setWordWrap(false);

        hboxLayout4->addWidget(triggerslabel);

        pixpertrig = new QSpinBox(triggersframe);
        pixpertrig->setObjectName(QString::fromUtf8("pixpertrig"));
        pixpertrig->setEnabled(true);
        pixpertrig->setMinimum(1);
        pixpertrig->setMaximum(100);
        pixpertrig->setSingleStep(1);
        pixpertrig->setValue(5);

        hboxLayout4->addWidget(pixpertrig);


        hboxLayout->addWidget(triggersframe);

        symbolsizeframe = new QFrame(ctrl1);
        symbolsizeframe->setObjectName(QString::fromUtf8("symbolsizeframe"));
        sizePolicy2.setHeightForWidth(symbolsizeframe->sizePolicy().hasHeightForWidth());
        symbolsizeframe->setSizePolicy(sizePolicy2);
        symbolsizeframe->setFrameShape(QFrame::NoFrame);
        symbolsizeframe->setFrameShadow(QFrame::Raised);
        hboxLayout5 = new QHBoxLayout(symbolsizeframe);
        hboxLayout5->setSpacing(5);
        hboxLayout5->setContentsMargins(0, 0, 0, 0);
        hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));

        hboxLayout->addWidget(symbolsizeframe);

        ctrl1_spacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(ctrl1_spacer);

        dbxon = new QCheckBox(ctrl1);
        dbxon->setObjectName(QString::fromUtf8("dbxon"));
        dbxon->setChecked(false);

        hboxLayout->addWidget(dbxon);


        vboxLayout->addWidget(ctrl1);

        ctrl2 = new QFrame(ctrlpanel);
        ctrl2->setObjectName(QString::fromUtf8("ctrl2"));
        sizePolicy1.setHeightForWidth(ctrl2->sizePolicy().hasHeightForWidth());
        ctrl2->setSizePolicy(sizePolicy1);
        ctrl2->setMinimumSize(QSize(0, 30));
        ctrl2->setMaximumSize(QSize(32767, 30));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush);
        palette1.setBrush(QPalette::Active, QPalette::Light, brush);
        palette1.setBrush(QPalette::Active, QPalette::Midlight, brush);
        palette1.setBrush(QPalette::Active, QPalette::Dark, brush);
        palette1.setBrush(QPalette::Active, QPalette::Mid, brush);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush);
        palette1.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush);
        palette1.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Active, QPalette::Highlight, brush);
        palette1.setBrush(QPalette::Active, QPalette::HighlightedText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Link, brush);
        palette1.setBrush(QPalette::Active, QPalette::LinkVisited, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Light, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Midlight, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Dark, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Mid, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Highlight, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::HighlightedText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Link, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::LinkVisited, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Light, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Midlight, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Dark, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Mid, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Highlight, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::HighlightedText, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Link, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::LinkVisited, brush);
        ctrl2->setPalette(palette1);
        ctrl2->setFrameShape(QFrame::NoFrame);
        ctrl2->setFrameShadow(QFrame::Plain);
        ctrl2->setProperty("margin", QVariant(0));
        hboxLayout6 = new QHBoxLayout(ctrl2);
        hboxLayout6->setSpacing(10);
        hboxLayout6->setContentsMargins(0, 0, 0, 0);
        hboxLayout6->setObjectName(QString::fromUtf8("hboxLayout6"));
        spikesrcframe_2 = new QFrame(ctrl2);
        spikesrcframe_2->setObjectName(QString::fromUtf8("spikesrcframe_2"));
        spikesrcframe_2->setFrameShape(QFrame::NoFrame);
        spikesrcframe_2->setFrameShadow(QFrame::Raised);
        hboxLayout7 = new QHBoxLayout(spikesrcframe_2);
        hboxLayout7->setSpacing(8);
        hboxLayout7->setContentsMargins(0, 0, 0, 0);
        hboxLayout7->setObjectName(QString::fromUtf8("hboxLayout7"));
        spikesrcframe1_2 = new QFrame(spikesrcframe_2);
        spikesrcframe1_2->setObjectName(QString::fromUtf8("spikesrcframe1_2"));
        sizePolicy2.setHeightForWidth(spikesrcframe1_2->sizePolicy().hasHeightForWidth());
        spikesrcframe1_2->setSizePolicy(sizePolicy2);
        spikesrcframe1_2->setFrameShape(QFrame::NoFrame);
        spikesrcframe1_2->setFrameShadow(QFrame::Raised);
        hboxLayout8 = new QHBoxLayout(spikesrcframe1_2);
        hboxLayout8->setSpacing(5);
        hboxLayout8->setContentsMargins(0, 0, 0, 0);
        hboxLayout8->setObjectName(QString::fromUtf8("hboxLayout8"));
        spikesrclabel_2 = new QLabel(spikesrcframe1_2);
        spikesrclabel_2->setObjectName(QString::fromUtf8("spikesrclabel_2"));
        sizePolicy2.setHeightForWidth(spikesrclabel_2->sizePolicy().hasHeightForWidth());
        spikesrclabel_2->setSizePolicy(sizePolicy2);
        spikesrclabel_2->setWordWrap(false);

        hboxLayout8->addWidget(spikesrclabel_2);

        spikesrc = new QComboBox(spikesrcframe1_2);
        spikesrc->setObjectName(QString::fromUtf8("spikesrc"));
        spikesrc->setEditable(true);
        spikesrc->setAutoCompletion(false);

        hboxLayout8->addWidget(spikesrc);


        hboxLayout7->addWidget(spikesrcframe1_2);

        spikestatus = new QLabel(spikesrcframe_2);
        spikestatus->setObjectName(QString::fromUtf8("spikestatus"));
        sizePolicy2.setHeightForWidth(spikestatus->sizePolicy().hasHeightForWidth());
        spikestatus->setSizePolicy(sizePolicy2);
        spikestatus->setMinimumSize(QSize(80, 20));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Active, QPalette::Button, brush);
        palette2.setBrush(QPalette::Active, QPalette::Light, brush);
        palette2.setBrush(QPalette::Active, QPalette::Midlight, brush);
        palette2.setBrush(QPalette::Active, QPalette::Dark, brush);
        palette2.setBrush(QPalette::Active, QPalette::Mid, brush);
        palette2.setBrush(QPalette::Active, QPalette::Text, brush);
        palette2.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette2.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette2.setBrush(QPalette::Active, QPalette::Base, brush);
        palette2.setBrush(QPalette::Active, QPalette::Window, brush);
        palette2.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette2.setBrush(QPalette::Active, QPalette::Highlight, brush);
        palette2.setBrush(QPalette::Active, QPalette::HighlightedText, brush);
        palette2.setBrush(QPalette::Active, QPalette::Link, brush);
        palette2.setBrush(QPalette::Active, QPalette::LinkVisited, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Light, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Midlight, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Dark, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Mid, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Highlight, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::HighlightedText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Link, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::LinkVisited, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::Light, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::Midlight, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::Dark, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::Mid, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::Window, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::Highlight, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::HighlightedText, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::Link, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::LinkVisited, brush);
        spikestatus->setPalette(palette2);
        spikestatus->setFrameShape(QFrame::Panel);
        spikestatus->setFrameShadow(QFrame::Sunken);
        spikestatus->setAlignment(Qt::AlignCenter);
        spikestatus->setWordWrap(false);

        hboxLayout7->addWidget(spikestatus);


        hboxLayout6->addWidget(spikesrcframe_2);

        downonly = new QCheckBox(ctrl2);
        downonly->setObjectName(QString::fromUtf8("downonly"));
        downonly->setEnabled(true);

        hboxLayout6->addWidget(downonly);

        posttrigframe_2 = new QFrame(ctrl2);
        posttrigframe_2->setObjectName(QString::fromUtf8("posttrigframe_2"));
        sizePolicy2.setHeightForWidth(posttrigframe_2->sizePolicy().hasHeightForWidth());
        posttrigframe_2->setSizePolicy(sizePolicy2);
        posttrigframe_2->setFrameShape(QFrame::NoFrame);
        posttrigframe_2->setFrameShadow(QFrame::Raised);
        hboxLayout9 = new QHBoxLayout(posttrigframe_2);
        hboxLayout9->setSpacing(6);
        hboxLayout9->setContentsMargins(0, 0, 0, 0);
        hboxLayout9->setObjectName(QString::fromUtf8("hboxLayout9"));
        usethresh = new QCheckBox(posttrigframe_2);
        usethresh->setObjectName(QString::fromUtf8("usethresh"));

        hboxLayout9->addWidget(usethresh);

        threshval = new QSpinBox(posttrigframe_2);
        threshval->setObjectName(QString::fromUtf8("threshval"));
        threshval->setEnabled(false);
        threshval->setMinimum(100);
        threshval->setMaximum(1000);
        threshval->setSingleStep(10);
        threshval->setValue(100);

        hboxLayout9->addWidget(threshval);

        posttriglabel_2_2 = new QLabel(posttrigframe_2);
        posttriglabel_2_2->setObjectName(QString::fromUtf8("posttriglabel_2_2"));
        posttriglabel_2_2->setEnabled(false);
        QFont font;
        font.setFamily(QString::fromUtf8("Symbol [Urw]"));
        posttriglabel_2_2->setFont(font);
        posttriglabel_2_2->setWordWrap(false);

        hboxLayout9->addWidget(posttriglabel_2_2);


        hboxLayout6->addWidget(posttrigframe_2);

        expander_2 = new QFrame(ctrl2);
        expander_2->setObjectName(QString::fromUtf8("expander_2"));
        expander_2->setEnabled(false);
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(expander_2->sizePolicy().hasHeightForWidth());
        expander_2->setSizePolicy(sizePolicy4);
        expander_2->setFrameShape(QFrame::NoFrame);
        expander_2->setFrameShadow(QFrame::Plain);
        hboxLayout10 = new QHBoxLayout(expander_2);
        hboxLayout10->setSpacing(2);
        hboxLayout10->setContentsMargins(0, 0, 0, 0);
        hboxLayout10->setObjectName(QString::fromUtf8("hboxLayout10"));
        hex = new QCheckBox(expander_2);
        hex->setObjectName(QString::fromUtf8("hex"));

        hboxLayout10->addWidget(hex);

        Spacer2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout10->addItem(Spacer2);


        hboxLayout6->addWidget(expander_2);

        freezeflag = new QCheckBox(ctrl2);
        freezeflag->setObjectName(QString::fromUtf8("freezeflag"));
        freezeflag->setEnabled(true);
        sizePolicy2.setHeightForWidth(freezeflag->sizePolicy().hasHeightForWidth());
        freezeflag->setSizePolicy(sizePolicy2);
        freezeflag->setMinimumSize(QSize(60, 20));

        hboxLayout6->addWidget(freezeflag);

        time = new QLabel(ctrl2);
        time->setObjectName(QString::fromUtf8("time"));
        sizePolicy2.setHeightForWidth(time->sizePolicy().hasHeightForWidth());
        time->setSizePolicy(sizePolicy2);
        time->setMinimumSize(QSize(80, 20));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Active, QPalette::Button, brush);
        palette3.setBrush(QPalette::Active, QPalette::Light, brush);
        palette3.setBrush(QPalette::Active, QPalette::Midlight, brush);
        palette3.setBrush(QPalette::Active, QPalette::Dark, brush);
        palette3.setBrush(QPalette::Active, QPalette::Mid, brush);
        palette3.setBrush(QPalette::Active, QPalette::Text, brush);
        palette3.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette3.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette3.setBrush(QPalette::Active, QPalette::Base, brush);
        palette3.setBrush(QPalette::Active, QPalette::Window, brush);
        palette3.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette3.setBrush(QPalette::Active, QPalette::Highlight, brush);
        palette3.setBrush(QPalette::Active, QPalette::HighlightedText, brush);
        palette3.setBrush(QPalette::Active, QPalette::Link, brush);
        palette3.setBrush(QPalette::Active, QPalette::LinkVisited, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Light, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Midlight, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Dark, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Mid, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Highlight, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::HighlightedText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Link, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::LinkVisited, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::Light, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::Midlight, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::Dark, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::Mid, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::Window, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::Highlight, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::HighlightedText, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::Link, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::LinkVisited, brush);
        time->setPalette(palette3);
        time->setFrameShape(QFrame::Panel);
        time->setFrameShadow(QFrame::Sunken);
        time->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        time->setWordWrap(false);

        hboxLayout6->addWidget(time);

        trigger = new QLabel(ctrl2);
        trigger->setObjectName(QString::fromUtf8("trigger"));
        sizePolicy2.setHeightForWidth(trigger->sizePolicy().hasHeightForWidth());
        trigger->setSizePolicy(sizePolicy2);
        trigger->setMinimumSize(QSize(50, 20));
        QPalette palette4;
        palette4.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette4.setBrush(QPalette::Active, QPalette::Button, brush);
        palette4.setBrush(QPalette::Active, QPalette::Light, brush);
        palette4.setBrush(QPalette::Active, QPalette::Midlight, brush);
        palette4.setBrush(QPalette::Active, QPalette::Dark, brush);
        palette4.setBrush(QPalette::Active, QPalette::Mid, brush);
        palette4.setBrush(QPalette::Active, QPalette::Text, brush);
        palette4.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette4.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette4.setBrush(QPalette::Active, QPalette::Base, brush);
        palette4.setBrush(QPalette::Active, QPalette::Window, brush);
        palette4.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette4.setBrush(QPalette::Active, QPalette::Highlight, brush);
        palette4.setBrush(QPalette::Active, QPalette::HighlightedText, brush);
        palette4.setBrush(QPalette::Active, QPalette::Link, brush);
        palette4.setBrush(QPalette::Active, QPalette::LinkVisited, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Light, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Midlight, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Dark, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Mid, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Highlight, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::HighlightedText, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Link, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::LinkVisited, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::Light, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::Midlight, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::Dark, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::Mid, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::Window, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::Highlight, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::HighlightedText, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::Link, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::LinkVisited, brush);
        trigger->setPalette(palette4);
        trigger->setFrameShape(QFrame::Panel);
        trigger->setFrameShadow(QFrame::Sunken);
        trigger->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        trigger->setWordWrap(false);

        hboxLayout6->addWidget(trigger);


        vboxLayout->addWidget(ctrl2);


        gridLayout->addWidget(ctrlpanel, 0, 0, 1, 1);

        Line1 = new QFrame(Raster);
        Line1->setObjectName(QString::fromUtf8("Line1"));
        Line1->setMinimumSize(QSize(0, 2));
        Line1->setFrameShape(QFrame::HLine);
        Line1->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(Line1, 1, 0, 1, 1);

        rasters = new QMultiRaster(Raster);
        rasters->setObjectName(QString::fromUtf8("rasters"));
        QSizePolicy sizePolicy5(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(rasters->sizePolicy().hasHeightForWidth());
        rasters->setSizePolicy(sizePolicy5);

        gridLayout->addWidget(rasters, 2, 0, 1, 1);


        retranslateUi(Raster);
        QObject::connect(pretrig, SIGNAL(valueChanged(int)), rasters, SLOT(setPreMS(int)));
        QObject::connect(posttrig, SIGNAL(valueChanged(int)), rasters, SLOT(setPostMS(int)));
        QObject::connect(freezeflag, SIGNAL(toggled(bool)), rasters, SLOT(freeze(bool)));
        QObject::connect(rasters, SIGNAL(reportTime(QString)), time, SLOT(setText(QString)));
        QObject::connect(rasters, SIGNAL(reportTriggers(QString)), trigger, SLOT(setText(QString)));
        QObject::connect(rasters, SIGNAL(reportSourceStatus(QString)), spikestatus, SLOT(setText(QString)));
        QObject::connect(hex, SIGNAL(toggled(bool)), rasters, SLOT(setHex(bool)));
        QObject::connect(pixpertrig, SIGNAL(valueChanged(int)), rasters, SLOT(setPixPerTrig(int)));
        QObject::connect(trigch, SIGNAL(activated(int)), rasters, SLOT(setTrigChannel(int)));
        QObject::connect(secch, SIGNAL(activated(int)), rasters, SLOT(setSecChannel(int)));
        QObject::connect(dbxon, SIGNAL(toggled(bool)), Raster, SLOT(enableDbx(bool)));
        QObject::connect(spikesrc, SIGNAL(activated(QString)), rasters, SLOT(setSpikeSource(QString)));
        QObject::connect(usethresh, SIGNAL(toggled(bool)), threshval, SLOT(setEnabled(bool)));
        QObject::connect(downonly, SIGNAL(toggled(bool)), rasters, SLOT(setDownOnly(bool)));
        QObject::connect(usethresh, SIGNAL(toggled(bool)), rasters, SLOT(setUseThresh(bool)));
        QObject::connect(threshval, SIGNAL(valueChanged(int)), rasters, SLOT(setThreshVal(int)));
        QObject::connect(usethresh, SIGNAL(toggled(bool)), posttriglabel_2_2, SLOT(setEnabled(bool)));

        QMetaObject::connectSlotsByName(Raster);
    } // setupUi

    void retranslateUi(QWidget *Raster)
    {
        Raster->setWindowTitle(QApplication::translate("Raster", "Raster", 0, QApplication::UnicodeUTF8));
        rangelabel_2->setText(QApplication::translate("Raster", "Trigger:", 0, QApplication::UnicodeUTF8));
        trigch->clear();
        trigch->insertItems(0, QStringList()
         << QApplication::translate("Raster", "A1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Raster", "A2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Raster", "A3", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        trigch->setToolTip(QApplication::translate("Raster", "Primary trigger channel", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        tolabel_2->setText(QApplication::translate("Raster", "Secondary:", 0, QApplication::UnicodeUTF8));
        secch->clear();
        secch->insertItems(0, QStringList()
         << QApplication::translate("Raster", "A1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Raster", "A2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Raster", "A3", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        secch->setToolTip(QApplication::translate("Raster", "Secondary trigger channel", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        pretriglabel->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        pretriglabel->setText(QApplication::translate("Raster", "Pre trig:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        pretrig->setToolTip(QApplication::translate("Raster", "Part of window before the trigger, in ms.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pretrig->setPrefix(QString());
        posttriglabel->setText(QApplication::translate("Raster", "Post trig:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        posttrig->setToolTip(QApplication::translate("Raster", "Part of window after the trigger, in ms.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        posttrig->setPrefix(QString());
#ifndef QT_NO_TOOLTIP
        triggerslabel->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        triggerslabel->setText(QApplication::translate("Raster", "Pix/Trial:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        pixpertrig->setToolTip(QApplication::translate("Raster", "Qt::Vertical pixels per trial", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pixpertrig->setPrefix(QString());
#ifndef QT_NO_TOOLTIP
        dbxon->setToolTip(QApplication::translate("Raster", "Enable debug output", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        dbxon->setText(QApplication::translate("Raster", "Dbx", 0, QApplication::UnicodeUTF8));
        spikesrclabel_2->setText(QApplication::translate("Raster", "Spike source:", 0, QApplication::UnicodeUTF8));
        spikesrc->clear();
        spikesrc->insertItems(0, QStringList()
         << QApplication::translate("Raster", "spike", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Raster", "respike", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        spikesrc->setToolTip(QApplication::translate("Raster", "Server to read spike information from", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        spikestatus->setToolTip(QApplication::translate("Raster", "Status of the spike stream", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        spikestatus->setText(QApplication::translate("Raster", "No source", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        downonly->setToolTip(QApplication::translate("Raster", "Show downgoing spikes only", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        downonly->setText(QApplication::translate("Raster", "Down only", 0, QApplication::UnicodeUTF8));
        usethresh->setText(QApplication::translate("Raster", "Rethreshold:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        threshval->setToolTip(QApplication::translate("Raster", "Part of window after the trigger, in ms.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        threshval->setPrefix(QString());
        posttriglabel_2_2->setText(QApplication::translate("Raster", "\302\264 0.01 q", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        hex->setToolTip(QApplication::translate("Raster", "Display graphs in hex mea layout", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        hex->setText(QApplication::translate("Raster", "Hex", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        freezeflag->setToolTip(QApplication::translate("Raster", "Prevent updating when new data comes in", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        freezeflag->setText(QApplication::translate("Raster", " Freeze", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        time->setToolTip(QApplication::translate("Raster", "Time since start of run", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        time->setText(QApplication::translate("Raster", "1.0 s", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        trigger->setToolTip(QApplication::translate("Raster", "Triggers since start of run (primary and secondary combined)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        trigger->setText(QApplication::translate("Raster", "1", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Raster: public Ui_Raster {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RASTER_H
