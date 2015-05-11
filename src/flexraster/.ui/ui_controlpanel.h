/********************************************************************************
** Form generated from reading UI file 'controlpanel.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTROLPANEL_H
#define UI_CONTROLPANEL_H

#include <Qt3Support/Q3MimeSourceFactory>
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
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_controlpanel
{
public:
    QVBoxLayout *vboxLayout;
    QFrame *frame15;
    QHBoxLayout *hboxLayout;
    QLabel *textLabel3_2_2;
    QSpinBox *t_pre;
    QLabel *textLabel4_2_2;
    QSpinBox *t_post;
    QSpacerItem *spacer2_2;
    QCheckBox *use_down;
    QCheckBox *use_up;
    QSpacerItem *spacer2_2_2;
    QLabel *textLabel7;
    QSpinBox *pixtri;
    QFrame *frame16;
    QHBoxLayout *hboxLayout1;
    QLabel *textLabel1_2;
    QComboBox *source;
    QLabel *source_status;
    QSpacerItem *spacer5;
    QLabel *time;
    QLabel *trigs;
    QFrame *frame17;
    QFrame *frame18;
    QHBoxLayout *hboxLayout2;
    QPushButton *open_spont;
    QPushButton *open_8x8rec;
    QPushButton *open_8x8stim;
    QPushButton *open_vstim;
    QPushButton *open_hstim;
    QFrame *frame7;
    QPushButton *open_cont;

    void setupUi(QWidget *controlpanel)
    {
        if (controlpanel->objectName().isEmpty())
            controlpanel->setObjectName(QString::fromUtf8("controlpanel"));
        controlpanel->resize(606, 125);
        vboxLayout = new QVBoxLayout(controlpanel);
        vboxLayout->setSpacing(8);
        vboxLayout->setContentsMargins(7, 7, 7, 7);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        frame15 = new QFrame(controlpanel);
        frame15->setObjectName(QString::fromUtf8("frame15"));
        frame15->setFrameShape(QFrame::NoFrame);
        frame15->setFrameShadow(QFrame::Raised);
        hboxLayout = new QHBoxLayout(frame15);
        hboxLayout->setSpacing(12);
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        textLabel3_2_2 = new QLabel(frame15);
        textLabel3_2_2->setObjectName(QString::fromUtf8("textLabel3_2_2"));
        textLabel3_2_2->setWordWrap(false);

        hboxLayout->addWidget(textLabel3_2_2);

        t_pre = new QSpinBox(frame15);
        t_pre->setObjectName(QString::fromUtf8("t_pre"));
        t_pre->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        t_pre->setMaximum(100);
        t_pre->setSingleStep(10);

        hboxLayout->addWidget(t_pre);

        textLabel4_2_2 = new QLabel(frame15);
        textLabel4_2_2->setObjectName(QString::fromUtf8("textLabel4_2_2"));
        textLabel4_2_2->setWordWrap(false);

        hboxLayout->addWidget(textLabel4_2_2);

        t_post = new QSpinBox(frame15);
        t_post->setObjectName(QString::fromUtf8("t_post"));
        t_post->setMinimum(20);
        t_post->setMaximum(1000);
        t_post->setSingleStep(10);
        t_post->setValue(50);

        hboxLayout->addWidget(t_post);

        spacer2_2 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacer2_2);

        use_down = new QCheckBox(frame15);
        use_down->setObjectName(QString::fromUtf8("use_down"));
        use_down->setEnabled(true);
        use_down->setChecked(true);

        hboxLayout->addWidget(use_down);

        use_up = new QCheckBox(frame15);
        use_up->setObjectName(QString::fromUtf8("use_up"));
        use_up->setEnabled(true);
        use_up->setChecked(true);

        hboxLayout->addWidget(use_up);

        spacer2_2_2 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacer2_2_2);

        textLabel7 = new QLabel(frame15);
        textLabel7->setObjectName(QString::fromUtf8("textLabel7"));
        textLabel7->setWordWrap(false);

        hboxLayout->addWidget(textLabel7);

        pixtri = new QSpinBox(frame15);
        pixtri->setObjectName(QString::fromUtf8("pixtri"));
        pixtri->setMinimum(1);
        pixtri->setMaximum(10);
        pixtri->setValue(2);

        hboxLayout->addWidget(pixtri);


        vboxLayout->addWidget(frame15);

        frame16 = new QFrame(controlpanel);
        frame16->setObjectName(QString::fromUtf8("frame16"));
        frame16->setFrameShape(QFrame::NoFrame);
        frame16->setFrameShadow(QFrame::Raised);
        hboxLayout1 = new QHBoxLayout(frame16);
        hboxLayout1->setSpacing(12);
        hboxLayout1->setContentsMargins(0, 0, 0, 0);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        textLabel1_2 = new QLabel(frame16);
        textLabel1_2->setObjectName(QString::fromUtf8("textLabel1_2"));
        textLabel1_2->setWordWrap(false);

        hboxLayout1->addWidget(textLabel1_2);

        source = new QComboBox(frame16);
        source->setObjectName(QString::fromUtf8("source"));

        hboxLayout1->addWidget(source);

        source_status = new QLabel(frame16);
        source_status->setObjectName(QString::fromUtf8("source_status"));
        source_status->setMinimumSize(QSize(70, 0));
        source_status->setFrameShape(QFrame::Panel);
        source_status->setFrameShadow(QFrame::Sunken);
        source_status->setAlignment(Qt::AlignCenter);
        source_status->setWordWrap(false);

        hboxLayout1->addWidget(source_status);

        spacer5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(spacer5);

        time = new QLabel(frame16);
        time->setObjectName(QString::fromUtf8("time"));
        time->setMinimumSize(QSize(60, 0));
        time->setFrameShape(QFrame::Panel);
        time->setFrameShadow(QFrame::Sunken);
        time->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        time->setWordWrap(false);

        hboxLayout1->addWidget(time);

        trigs = new QLabel(frame16);
        trigs->setObjectName(QString::fromUtf8("trigs"));
        trigs->setMinimumSize(QSize(40, 0));
        trigs->setFrameShape(QFrame::Panel);
        trigs->setFrameShadow(QFrame::Sunken);
        trigs->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        trigs->setWordWrap(false);

        hboxLayout1->addWidget(trigs);


        vboxLayout->addWidget(frame16);

        frame17 = new QFrame(controlpanel);
        frame17->setObjectName(QString::fromUtf8("frame17"));
        frame17->setFrameShape(QFrame::HLine);
        frame17->setFrameShadow(QFrame::Raised);
        frame17->setLineWidth(2);

        vboxLayout->addWidget(frame17);

        frame18 = new QFrame(controlpanel);
        frame18->setObjectName(QString::fromUtf8("frame18"));
        frame18->setFrameShape(QFrame::NoFrame);
        frame18->setFrameShadow(QFrame::Raised);
        hboxLayout2 = new QHBoxLayout(frame18);
        hboxLayout2->setSpacing(12);
        hboxLayout2->setContentsMargins(0, 0, 0, 0);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        open_spont = new QPushButton(frame18);
        open_spont->setObjectName(QString::fromUtf8("open_spont"));
        open_spont->setCheckable(false);

        hboxLayout2->addWidget(open_spont);

        open_8x8rec = new QPushButton(frame18);
        open_8x8rec->setObjectName(QString::fromUtf8("open_8x8rec"));
        open_8x8rec->setCheckable(false);

        hboxLayout2->addWidget(open_8x8rec);

        open_8x8stim = new QPushButton(frame18);
        open_8x8stim->setObjectName(QString::fromUtf8("open_8x8stim"));
        open_8x8stim->setCheckable(false);

        hboxLayout2->addWidget(open_8x8stim);

        open_vstim = new QPushButton(frame18);
        open_vstim->setObjectName(QString::fromUtf8("open_vstim"));
        open_vstim->setCheckable(false);

        hboxLayout2->addWidget(open_vstim);

        open_hstim = new QPushButton(frame18);
        open_hstim->setObjectName(QString::fromUtf8("open_hstim"));
        open_hstim->setCheckable(false);
        open_hstim->setAutoDefault(false);
        open_hstim->setFlat(false);

        hboxLayout2->addWidget(open_hstim);

        frame7 = new QFrame(frame18);
        frame7->setObjectName(QString::fromUtf8("frame7"));
        frame7->setFrameShape(QFrame::VLine);
        frame7->setFrameShadow(QFrame::Raised);
        frame7->setLineWidth(2);

        hboxLayout2->addWidget(frame7);

        open_cont = new QPushButton(frame18);
        open_cont->setObjectName(QString::fromUtf8("open_cont"));
        open_cont->setEnabled(true);
        open_cont->setCheckable(false);
        open_cont->setAutoDefault(false);
        open_cont->setFlat(false);

        hboxLayout2->addWidget(open_cont);


        vboxLayout->addWidget(frame18);


        retranslateUi(controlpanel);

        QMetaObject::connectSlotsByName(controlpanel);
    } // setupUi

    void retranslateUi(QWidget *controlpanel)
    {
        controlpanel->setWindowTitle(QApplication::translate("controlpanel", "Flex raster", 0, QApplication::UnicodeUTF8));
        textLabel3_2_2->setText(QApplication::translate("controlpanel", "Pre:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        t_pre->setToolTip(QApplication::translate("controlpanel", "Pre-trigger window (ms)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        textLabel4_2_2->setText(QApplication::translate("controlpanel", "Post:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        t_post->setToolTip(QApplication::translate("controlpanel", "Post-trigger window (ms)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        use_down->setToolTip(QApplication::translate("controlpanel", "Show downgoing spikes?", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        use_down->setText(QApplication::translate("controlpanel", "Down", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        use_up->setToolTip(QApplication::translate("controlpanel", "Show upgoing spikes?", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        use_up->setText(QApplication::translate("controlpanel", "Up", 0, QApplication::UnicodeUTF8));
        textLabel7->setText(QApplication::translate("controlpanel", "Pix/trial:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        pixtri->setToolTip(QApplication::translate("controlpanel", "Number of (vertical) pixels per trial", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        textLabel1_2->setText(QApplication::translate("controlpanel", "Source:", 0, QApplication::UnicodeUTF8));
        source->clear();
        source->insertItems(0, QStringList()
         << QApplication::translate("controlpanel", "spike", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("controlpanel", "respike", 0, QApplication::UnicodeUTF8)
        );
        source_status->setText(QApplication::translate("controlpanel", "Not found", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        time->setToolTip(QApplication::translate("controlpanel", "Time since start of run (s)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        time->setText(QApplication::translate("controlpanel", "0.000", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        trigs->setToolTip(QApplication::translate("controlpanel", "Number of triggers", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        trigs->setText(QApplication::translate("controlpanel", "-", 0, QApplication::UnicodeUTF8));
        open_spont->setText(QApplication::translate("controlpanel", "Spont", 0, QApplication::UnicodeUTF8));
        open_8x8rec->setText(QApplication::translate("controlpanel", "8x8 Rec", 0, QApplication::UnicodeUTF8));
        open_8x8stim->setText(QApplication::translate("controlpanel", "8x8 Stim", 0, QApplication::UnicodeUTF8));
        open_vstim->setText(QApplication::translate("controlpanel", "V Stim", 0, QApplication::UnicodeUTF8));
        open_hstim->setText(QApplication::translate("controlpanel", "H Stim", 0, QApplication::UnicodeUTF8));
        open_cont->setText(QApplication::translate("controlpanel", "Cont", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class controlpanel: public Ui_controlpanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTROLPANEL_H
