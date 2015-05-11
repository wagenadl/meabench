/********************************************************************************
** Form generated from reading UI file 'Scope.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCOPE_H
#define UI_SCOPE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QScrollBar>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "QMultiScope.H"

QT_BEGIN_NAMESPACE

class Ui_Scope
{
public:
    QVBoxLayout *vboxLayout;
    QFrame *ctrlpanel;
    QVBoxLayout *vboxLayout1;
    QFrame *ctrl1;
    QHBoxLayout *hboxLayout;
    QFrame *Frame6_2_4;
    QHBoxLayout *hboxLayout1;
    QLabel *TextLabel2_2_4;
    QSpinBox *range;
    QFrame *Frame6_2_2_2;
    QHBoxLayout *hboxLayout2;
    QLabel *TextLabel2_2_2_3;
    QSpinBox *divs;
    QFrame *Frame6_4;
    QHBoxLayout *hboxLayout3;
    QLabel *TextLabel2_5;
    QSpinBox *window;
    QCheckBox *trigflag;
    QFrame *Frame6_3_2;
    QHBoxLayout *hboxLayout4;
    QLabel *TextLabel2_3_3;
    QSpinBox *pretrig;
    QComboBox *speedy;
    QPushButton *savebuf;
    QSpacerItem *Spacer4_3;
    QPushButton *center;
    QCheckBox *dbxon;
    QFrame *ctrl2;
    QHBoxLayout *hboxLayout5;
    QFrame *rawsrcframe;
    QHBoxLayout *hboxLayout6;
    QFrame *Frame6_2_3_2_3;
    QHBoxLayout *hboxLayout7;
    QLabel *TextLabel2_2_3_2_3;
    QComboBox *rawsrc;
    QLabel *rawstatus;
    QFrame *spikesrcframe;
    QHBoxLayout *hboxLayout8;
    QFrame *Frame6_2_3_2_2_2;
    QHBoxLayout *hboxLayout9;
    QLabel *TextLabel2_2_3_2_2_2;
    QComboBox *spikesrc;
    QLabel *spikestatus;
    QComboBox *elclayout;
    QFrame *expander;
    QScrollBar *scrollback;
    QCheckBox *freezeflag;
    QLabel *time;
    QFrame *Line1;
    QMultiScope *scopes;

    void setupUi(QWidget *Scope)
    {
        if (Scope->objectName().isEmpty())
            Scope->setObjectName(QString::fromUtf8("Scope"));
        Scope->resize(1390, 659);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Scope->sizePolicy().hasHeightForWidth());
        Scope->setSizePolicy(sizePolicy);
        vboxLayout = new QVBoxLayout(Scope);
        vboxLayout->setSpacing(4);
        vboxLayout->setContentsMargins(4, 4, 4, 4);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        ctrlpanel = new QFrame(Scope);
        ctrlpanel->setObjectName(QString::fromUtf8("ctrlpanel"));
        ctrlpanel->setMaximumSize(QSize(32767, 69));
        ctrlpanel->setFrameShape(QFrame::NoFrame);
        ctrlpanel->setFrameShadow(QFrame::Raised);
        vboxLayout1 = new QVBoxLayout(ctrlpanel);
        vboxLayout1->setSpacing(2);
        vboxLayout1->setContentsMargins(2, 2, 2, 2);
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        ctrl1 = new QFrame(ctrlpanel);
        ctrl1->setObjectName(QString::fromUtf8("ctrl1"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(ctrl1->sizePolicy().hasHeightForWidth());
        ctrl1->setSizePolicy(sizePolicy1);
        ctrl1->setMinimumSize(QSize(0, 30));
        ctrl1->setMaximumSize(QSize(32767, 30));
        ctrl1->setFrameShape(QFrame::NoFrame);
        ctrl1->setFrameShadow(QFrame::Plain);
        hboxLayout = new QHBoxLayout(ctrl1);
        hboxLayout->setSpacing(15);
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        Frame6_2_4 = new QFrame(ctrl1);
        Frame6_2_4->setObjectName(QString::fromUtf8("Frame6_2_4"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(Frame6_2_4->sizePolicy().hasHeightForWidth());
        Frame6_2_4->setSizePolicy(sizePolicy2);
        Frame6_2_4->setFrameShape(QFrame::NoFrame);
        Frame6_2_4->setFrameShadow(QFrame::Raised);
        hboxLayout1 = new QHBoxLayout(Frame6_2_4);
        hboxLayout1->setSpacing(5);
        hboxLayout1->setContentsMargins(0, 0, 0, 0);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        TextLabel2_2_4 = new QLabel(Frame6_2_4);
        TextLabel2_2_4->setObjectName(QString::fromUtf8("TextLabel2_2_4"));
        TextLabel2_2_4->setWordWrap(false);

        hboxLayout1->addWidget(TextLabel2_2_4);

        range = new QSpinBox(Frame6_2_4);
        range->setObjectName(QString::fromUtf8("range"));
        range->setMinimum(5);
        range->setMaximum(5000);
        range->setSingleStep(5);
        range->setValue(20);

        hboxLayout1->addWidget(range);


        hboxLayout->addWidget(Frame6_2_4);

        Frame6_2_2_2 = new QFrame(ctrl1);
        Frame6_2_2_2->setObjectName(QString::fromUtf8("Frame6_2_2_2"));
        sizePolicy2.setHeightForWidth(Frame6_2_2_2->sizePolicy().hasHeightForWidth());
        Frame6_2_2_2->setSizePolicy(sizePolicy2);
        Frame6_2_2_2->setFrameShape(QFrame::NoFrame);
        Frame6_2_2_2->setFrameShadow(QFrame::Raised);
        hboxLayout2 = new QHBoxLayout(Frame6_2_2_2);
        hboxLayout2->setSpacing(5);
        hboxLayout2->setContentsMargins(0, 0, 0, 0);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        TextLabel2_2_2_3 = new QLabel(Frame6_2_2_2);
        TextLabel2_2_2_3->setObjectName(QString::fromUtf8("TextLabel2_2_2_3"));
        TextLabel2_2_2_3->setWordWrap(false);

        hboxLayout2->addWidget(TextLabel2_2_2_3);

        divs = new QSpinBox(Frame6_2_2_2);
        divs->setObjectName(QString::fromUtf8("divs"));
        divs->setMinimum(0);
        divs->setMaximum(1000);
        divs->setSingleStep(5);
        divs->setValue(0);

        hboxLayout2->addWidget(divs);


        hboxLayout->addWidget(Frame6_2_2_2);

        Frame6_4 = new QFrame(ctrl1);
        Frame6_4->setObjectName(QString::fromUtf8("Frame6_4"));
        sizePolicy2.setHeightForWidth(Frame6_4->sizePolicy().hasHeightForWidth());
        Frame6_4->setSizePolicy(sizePolicy2);
        Frame6_4->setFrameShape(QFrame::NoFrame);
        Frame6_4->setFrameShadow(QFrame::Raised);
        hboxLayout3 = new QHBoxLayout(Frame6_4);
        hboxLayout3->setSpacing(5);
        hboxLayout3->setContentsMargins(0, 0, 0, 0);
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        TextLabel2_5 = new QLabel(Frame6_4);
        TextLabel2_5->setObjectName(QString::fromUtf8("TextLabel2_5"));
        TextLabel2_5->setWordWrap(false);

        hboxLayout3->addWidget(TextLabel2_5);

        window = new QSpinBox(Frame6_4);
        window->setObjectName(QString::fromUtf8("window"));
        window->setMinimum(20);
        window->setMaximum(5000);
        window->setSingleStep(50);
        window->setValue(200);

        hboxLayout3->addWidget(window);


        hboxLayout->addWidget(Frame6_4);

        trigflag = new QCheckBox(ctrl1);
        trigflag->setObjectName(QString::fromUtf8("trigflag"));
        trigflag->setChecked(false);

        hboxLayout->addWidget(trigflag);

        Frame6_3_2 = new QFrame(ctrl1);
        Frame6_3_2->setObjectName(QString::fromUtf8("Frame6_3_2"));
        sizePolicy2.setHeightForWidth(Frame6_3_2->sizePolicy().hasHeightForWidth());
        Frame6_3_2->setSizePolicy(sizePolicy2);
        Frame6_3_2->setFrameShape(QFrame::NoFrame);
        Frame6_3_2->setFrameShadow(QFrame::Raised);
        hboxLayout4 = new QHBoxLayout(Frame6_3_2);
        hboxLayout4->setSpacing(5);
        hboxLayout4->setContentsMargins(0, 0, 0, 0);
        hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
        TextLabel2_3_3 = new QLabel(Frame6_3_2);
        TextLabel2_3_3->setObjectName(QString::fromUtf8("TextLabel2_3_3"));
        TextLabel2_3_3->setEnabled(false);
        TextLabel2_3_3->setWordWrap(false);

        hboxLayout4->addWidget(TextLabel2_3_3);

        pretrig = new QSpinBox(Frame6_3_2);
        pretrig->setObjectName(QString::fromUtf8("pretrig"));
        pretrig->setEnabled(false);
        pretrig->setMinimum(0);
        pretrig->setMaximum(1000);
        pretrig->setSingleStep(50);
        pretrig->setValue(0);

        hboxLayout4->addWidget(pretrig);


        hboxLayout->addWidget(Frame6_3_2);

        speedy = new QComboBox(ctrl1);
        speedy->setObjectName(QString::fromUtf8("speedy"));
        speedy->setInsertPolicy(QComboBox::NoInsert);
        speedy->setDuplicatesEnabled(false);

        hboxLayout->addWidget(speedy);

        savebuf = new QPushButton(ctrl1);
        savebuf->setObjectName(QString::fromUtf8("savebuf"));
        savebuf->setEnabled(true);
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(savebuf->sizePolicy().hasHeightForWidth());
        savebuf->setSizePolicy(sizePolicy3);
        savebuf->setMinimumSize(QSize(0, 30));
        savebuf->setMaximumSize(QSize(32767, 30));

        hboxLayout->addWidget(savebuf);

        Spacer4_3 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(Spacer4_3);

        center = new QPushButton(ctrl1);
        center->setObjectName(QString::fromUtf8("center"));
        sizePolicy3.setHeightForWidth(center->sizePolicy().hasHeightForWidth());
        center->setSizePolicy(sizePolicy3);
        center->setMinimumSize(QSize(0, 30));
        center->setMaximumSize(QSize(32767, 30));

        hboxLayout->addWidget(center);

        dbxon = new QCheckBox(ctrl1);
        dbxon->setObjectName(QString::fromUtf8("dbxon"));

        hboxLayout->addWidget(dbxon);


        vboxLayout1->addWidget(ctrl1);

        ctrl2 = new QFrame(ctrlpanel);
        ctrl2->setObjectName(QString::fromUtf8("ctrl2"));
        sizePolicy1.setHeightForWidth(ctrl2->sizePolicy().hasHeightForWidth());
        ctrl2->setSizePolicy(sizePolicy1);
        ctrl2->setMinimumSize(QSize(0, 30));
        ctrl2->setMaximumSize(QSize(32767, 30));
        ctrl2->setFrameShape(QFrame::NoFrame);
        ctrl2->setFrameShadow(QFrame::Plain);
        hboxLayout5 = new QHBoxLayout(ctrl2);
        hboxLayout5->setSpacing(15);
        hboxLayout5->setContentsMargins(0, 0, 0, 0);
        hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
        rawsrcframe = new QFrame(ctrl2);
        rawsrcframe->setObjectName(QString::fromUtf8("rawsrcframe"));
        rawsrcframe->setFrameShape(QFrame::NoFrame);
        rawsrcframe->setFrameShadow(QFrame::Raised);
        hboxLayout6 = new QHBoxLayout(rawsrcframe);
        hboxLayout6->setSpacing(10);
        hboxLayout6->setContentsMargins(0, 0, 0, 0);
        hboxLayout6->setObjectName(QString::fromUtf8("hboxLayout6"));
        Frame6_2_3_2_3 = new QFrame(rawsrcframe);
        Frame6_2_3_2_3->setObjectName(QString::fromUtf8("Frame6_2_3_2_3"));
        sizePolicy2.setHeightForWidth(Frame6_2_3_2_3->sizePolicy().hasHeightForWidth());
        Frame6_2_3_2_3->setSizePolicy(sizePolicy2);
        Frame6_2_3_2_3->setFrameShape(QFrame::NoFrame);
        Frame6_2_3_2_3->setFrameShadow(QFrame::Raised);
        hboxLayout7 = new QHBoxLayout(Frame6_2_3_2_3);
        hboxLayout7->setSpacing(5);
        hboxLayout7->setContentsMargins(0, 0, 0, 0);
        hboxLayout7->setObjectName(QString::fromUtf8("hboxLayout7"));
        TextLabel2_2_3_2_3 = new QLabel(Frame6_2_3_2_3);
        TextLabel2_2_3_2_3->setObjectName(QString::fromUtf8("TextLabel2_2_3_2_3"));
        sizePolicy2.setHeightForWidth(TextLabel2_2_3_2_3->sizePolicy().hasHeightForWidth());
        TextLabel2_2_3_2_3->setSizePolicy(sizePolicy2);
        TextLabel2_2_3_2_3->setWordWrap(false);

        hboxLayout7->addWidget(TextLabel2_2_3_2_3);

        rawsrc = new QComboBox(Frame6_2_3_2_3);
        rawsrc->setObjectName(QString::fromUtf8("rawsrc"));
        rawsrc->setEditable(true);

        hboxLayout7->addWidget(rawsrc);


        hboxLayout6->addWidget(Frame6_2_3_2_3);

        rawstatus = new QLabel(rawsrcframe);
        rawstatus->setObjectName(QString::fromUtf8("rawstatus"));
        sizePolicy2.setHeightForWidth(rawstatus->sizePolicy().hasHeightForWidth());
        rawstatus->setSizePolicy(sizePolicy2);
        rawstatus->setMinimumSize(QSize(80, 20));
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(230, 230, 214, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        QBrush brush2(QColor(255, 255, 255, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush3(QColor(242, 242, 234, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        QBrush brush4(QColor(115, 115, 107, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush4);
        QBrush brush5(QColor(153, 153, 143, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush3);
        QBrush brush6(QColor(255, 255, 220, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        rawstatus->setPalette(palette);
        rawstatus->setAutoFillBackground(true);
        rawstatus->setFrameShape(QFrame::Panel);
        rawstatus->setFrameShadow(QFrame::Sunken);
        rawstatus->setLineWidth(1);
        rawstatus->setAlignment(Qt::AlignCenter);
        rawstatus->setWordWrap(false);

        hboxLayout6->addWidget(rawstatus);


        hboxLayout5->addWidget(rawsrcframe);

        spikesrcframe = new QFrame(ctrl2);
        spikesrcframe->setObjectName(QString::fromUtf8("spikesrcframe"));
        spikesrcframe->setFrameShape(QFrame::NoFrame);
        spikesrcframe->setFrameShadow(QFrame::Raised);
        hboxLayout8 = new QHBoxLayout(spikesrcframe);
        hboxLayout8->setSpacing(10);
        hboxLayout8->setContentsMargins(0, 0, 0, 0);
        hboxLayout8->setObjectName(QString::fromUtf8("hboxLayout8"));
        Frame6_2_3_2_2_2 = new QFrame(spikesrcframe);
        Frame6_2_3_2_2_2->setObjectName(QString::fromUtf8("Frame6_2_3_2_2_2"));
        sizePolicy2.setHeightForWidth(Frame6_2_3_2_2_2->sizePolicy().hasHeightForWidth());
        Frame6_2_3_2_2_2->setSizePolicy(sizePolicy2);
        Frame6_2_3_2_2_2->setFrameShape(QFrame::NoFrame);
        Frame6_2_3_2_2_2->setFrameShadow(QFrame::Raised);
        hboxLayout9 = new QHBoxLayout(Frame6_2_3_2_2_2);
        hboxLayout9->setSpacing(5);
        hboxLayout9->setContentsMargins(0, 0, 0, 0);
        hboxLayout9->setObjectName(QString::fromUtf8("hboxLayout9"));
        TextLabel2_2_3_2_2_2 = new QLabel(Frame6_2_3_2_2_2);
        TextLabel2_2_3_2_2_2->setObjectName(QString::fromUtf8("TextLabel2_2_3_2_2_2"));
        sizePolicy2.setHeightForWidth(TextLabel2_2_3_2_2_2->sizePolicy().hasHeightForWidth());
        TextLabel2_2_3_2_2_2->setSizePolicy(sizePolicy2);
        TextLabel2_2_3_2_2_2->setWordWrap(false);

        hboxLayout9->addWidget(TextLabel2_2_3_2_2_2);

        spikesrc = new QComboBox(Frame6_2_3_2_2_2);
        spikesrc->setObjectName(QString::fromUtf8("spikesrc"));
        spikesrc->setEditable(true);
        spikesrc->setAutoCompletion(false);

        hboxLayout9->addWidget(spikesrc);


        hboxLayout8->addWidget(Frame6_2_3_2_2_2);

        spikestatus = new QLabel(spikesrcframe);
        spikestatus->setObjectName(QString::fromUtf8("spikestatus"));
        sizePolicy2.setHeightForWidth(spikestatus->sizePolicy().hasHeightForWidth());
        spikestatus->setSizePolicy(sizePolicy2);
        spikestatus->setMinimumSize(QSize(80, 20));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        palette1.setBrush(QPalette::Active, QPalette::Dark, brush4);
        palette1.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush);
        palette1.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette1.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Active, QPalette::AlternateBase, brush3);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette1.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette1.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush3);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        spikestatus->setPalette(palette1);
        spikestatus->setAutoFillBackground(true);
        spikestatus->setFrameShape(QFrame::Panel);
        spikestatus->setFrameShadow(QFrame::Sunken);
        spikestatus->setAlignment(Qt::AlignCenter);
        spikestatus->setWordWrap(false);

        hboxLayout8->addWidget(spikestatus);


        hboxLayout5->addWidget(spikesrcframe);

        elclayout = new QComboBox(ctrl2);
        elclayout->setObjectName(QString::fromUtf8("elclayout"));

        hboxLayout5->addWidget(elclayout);

        expander = new QFrame(ctrl2);
        expander->setObjectName(QString::fromUtf8("expander"));
        expander->setEnabled(false);
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(expander->sizePolicy().hasHeightForWidth());
        expander->setSizePolicy(sizePolicy4);
        expander->setFrameShape(QFrame::NoFrame);
        expander->setFrameShadow(QFrame::Plain);

        hboxLayout5->addWidget(expander);

        scrollback = new QScrollBar(ctrl2);
        scrollback->setObjectName(QString::fromUtf8("scrollback"));
        QSizePolicy sizePolicy5(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(scrollback->sizePolicy().hasHeightForWidth());
        scrollback->setSizePolicy(sizePolicy5);
        scrollback->setMinimumSize(QSize(80, 20));
        scrollback->setMaximumSize(QSize(32767, 20));
        scrollback->setAutoFillBackground(true);
        scrollback->setOrientation(Qt::Horizontal);

        hboxLayout5->addWidget(scrollback);

        freezeflag = new QCheckBox(ctrl2);
        freezeflag->setObjectName(QString::fromUtf8("freezeflag"));
        sizePolicy2.setHeightForWidth(freezeflag->sizePolicy().hasHeightForWidth());
        freezeflag->setSizePolicy(sizePolicy2);
        freezeflag->setMinimumSize(QSize(60, 20));

        hboxLayout5->addWidget(freezeflag);

        time = new QLabel(ctrl2);
        time->setObjectName(QString::fromUtf8("time"));
        sizePolicy2.setHeightForWidth(time->sizePolicy().hasHeightForWidth());
        time->setSizePolicy(sizePolicy2);
        time->setMinimumSize(QSize(80, 20));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette2.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette2.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        palette2.setBrush(QPalette::Active, QPalette::Dark, brush4);
        palette2.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette2.setBrush(QPalette::Active, QPalette::Text, brush);
        palette2.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette2.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette2.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette2.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette2.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette2.setBrush(QPalette::Active, QPalette::AlternateBase, brush3);
        palette2.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette2.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette2.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette2.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette2.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette2.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette2.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush3);
        palette2.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette2.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette2.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette2.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette2.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette2.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette2.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette2.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        time->setPalette(palette2);
        time->setAutoFillBackground(true);
        time->setFrameShape(QFrame::Panel);
        time->setFrameShadow(QFrame::Sunken);
        time->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        time->setWordWrap(false);

        hboxLayout5->addWidget(time);


        vboxLayout1->addWidget(ctrl2);


        vboxLayout->addWidget(ctrlpanel);

        Line1 = new QFrame(Scope);
        Line1->setObjectName(QString::fromUtf8("Line1"));
        Line1->setMinimumSize(QSize(0, 2));
        Line1->setFrameShape(QFrame::HLine);
        Line1->setFrameShadow(QFrame::Sunken);

        vboxLayout->addWidget(Line1);

        scopes = new QMultiScope(Scope);
        scopes->setObjectName(QString::fromUtf8("scopes"));

        vboxLayout->addWidget(scopes);


        retranslateUi(Scope);
        QObject::connect(center, SIGNAL(clicked()), scopes, SLOT(setCenter()));
        QObject::connect(pretrig, SIGNAL(valueChanged(int)), scopes, SLOT(setPreTrig(int)));
        QObject::connect(range, SIGNAL(valueChanged(int)), scopes, SLOT(setExtent(int)));
        QObject::connect(divs, SIGNAL(valueChanged(int)), scopes, SLOT(setGuideSpacing(int)));
        QObject::connect(rawsrc, SIGNAL(activated(QString)), Scope, SLOT(setRawSource(QString)));
        QObject::connect(spikesrc, SIGNAL(activated(QString)), Scope, SLOT(setSpikeSource(QString)));
        QObject::connect(dbxon, SIGNAL(toggled(bool)), Scope, SLOT(enableDbx(bool)));
        QObject::connect(freezeflag, SIGNAL(toggled(bool)), Scope, SLOT(freeze(bool)));
        QObject::connect(window, SIGNAL(valueChanged(int)), Scope, SLOT(setLengthMS(int)));
        QObject::connect(trigflag, SIGNAL(toggled(bool)), pretrig, SLOT(setEnabled(bool)));
        QObject::connect(trigflag, SIGNAL(toggled(bool)), TextLabel2_3_3, SLOT(setEnabled(bool)));
        QObject::connect(savebuf, SIGNAL(clicked()), Scope, SLOT(savefrozen()));
        QObject::connect(speedy, SIGNAL(activated(int)), scopes, SLOT(setSpeedy(int)));
        QObject::connect(elclayout, SIGNAL(activated(QString)), scopes, SLOT(setLayout(QString)));

        rawsrc->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Scope);
    } // setupUi

    void retranslateUi(QWidget *Scope)
    {
        Scope->setWindowTitle(QApplication::translate("Scope", "Scope", 0, QApplication::UnicodeUTF8));
        TextLabel2_2_4->setText(QApplication::translate("Scope", "Range:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        range->setToolTip(QApplication::translate("Scope", "Half range of graphs, in uV", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        range->setWhatsThis(QApplication::translate("Scope", "Length of displayed window in ms", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        range->setPrefix(QString());
        TextLabel2_2_2_3->setText(QApplication::translate("Scope", "Divs:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        divs->setToolTip(QApplication::translate("Scope", "Space between division lines, in uV", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        divs->setWhatsThis(QApplication::translate("Scope", "Length of displayed window in ms", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        divs->setPrefix(QString());
        TextLabel2_5->setText(QApplication::translate("Scope", "Window:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        window->setToolTip(QApplication::translate("Scope", "Length of displayed window in ms", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        window->setWhatsThis(QApplication::translate("Scope", "Length of displayed window in ms", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        window->setPrefix(QString());
#ifndef QT_NO_TOOLTIP
        trigflag->setToolTip(QApplication::translate("Scope", "Enable triggered display", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        trigflag->setText(QApplication::translate("Scope", "Triggered", 0, QApplication::UnicodeUTF8));
        TextLabel2_3_3->setText(QApplication::translate("Scope", "Pre trig:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        pretrig->setToolTip(QApplication::translate("Scope", "Part of window before the trigger, in ms. Ignored for non-triggered streams.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        pretrig->setWhatsThis(QApplication::translate("Scope", "Length of displayed window in ms", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        pretrig->setPrefix(QString());
        speedy->clear();
        speedy->insertItems(0, QStringList()
         << QApplication::translate("Scope", "Solid", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Scope", "Min&max", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Scope", "Avg only", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        speedy->setToolTip(QApplication::translate("Scope", "Choose graph drawing method", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        savebuf->setToolTip(QApplication::translate("Scope", "Save the contents of the `Freeze' buffer", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        savebuf->setText(QApplication::translate("Scope", "Save", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        center->setToolTip(QApplication::translate("Scope", "Vertically center all traces around their means", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        center->setText(QApplication::translate("Scope", "Center", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        dbxon->setToolTip(QApplication::translate("Scope", "Enable debug output", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        dbxon->setText(QApplication::translate("Scope", "Dbx", 0, QApplication::UnicodeUTF8));
        TextLabel2_2_3_2_3->setText(QApplication::translate("Scope", "Raw source:", 0, QApplication::UnicodeUTF8));
        rawsrc->clear();
        rawsrc->insertItems(0, QStringList()
         << QApplication::translate("Scope", "raw", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Scope", "raw2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Scope", "reraw", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Scope", "60hz", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Scope", "salpa", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Scope", "spraw", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Scope", "extractwindow", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        rawsrc->setToolTip(QApplication::translate("Scope", "Server to read waveform data from", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        rawstatus->setToolTip(QApplication::translate("Scope", "Status of the raw stream", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        rawstatus->setText(QApplication::translate("Scope", "No source", 0, QApplication::UnicodeUTF8));
        TextLabel2_2_3_2_2_2->setText(QApplication::translate("Scope", "Spike source:", 0, QApplication::UnicodeUTF8));
        spikesrc->clear();
        spikesrc->insertItems(0, QStringList()
         << QApplication::translate("Scope", "spike", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Scope", "respike", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        spikesrc->setToolTip(QApplication::translate("Scope", "Server to read spike information from", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        spikestatus->setToolTip(QApplication::translate("Scope", "Status of the spike stream", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        spikestatus->setText(QApplication::translate("Scope", "No source", 0, QApplication::UnicodeUTF8));
        elclayout->clear();
        elclayout->insertItems(0, QStringList()
         << QApplication::translate("Scope", "8x8", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Scope", "Hex", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Scope", "JN", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        freezeflag->setToolTip(QApplication::translate("Scope", "Prevent updating when new data comes in", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        freezeflag->setText(QApplication::translate("Scope", "Freeze", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        time->setToolTip(QApplication::translate("Scope", "Time or triggers since start of run", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        time->setText(QApplication::translate("Scope", "3400.0 s", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Scope: public Ui_Scope {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCOPE_H
