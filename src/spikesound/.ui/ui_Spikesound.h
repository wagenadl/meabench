/********************************************************************************
** Form generated from reading UI file 'Spikesound.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SPIKESOUND_H
#define UI_SPIKESOUND_H

#include <Qt3Support/Q3MimeSourceFactory>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_spikesound
{
public:
    QHBoxLayout *hboxLayout;
    QComboBox *source;
    QLabel *status;
    QSpacerItem *Spacer2;
    QRadioButton *playbutton;
    QSpacerItem *Spacer3;
    QLabel *TextLabel2;
    QSpinBox *volume;
    QSpacerItem *Spacer5;
    QRadioButton *negativeonly;
    QSpacerItem *Spacer4;
    QRadioButton *rethresh;
    QSpinBox *threshold;
    QLabel *label004;
    QSpacerItem *Spacer6;
    QCheckBox *a1;
    QCheckBox *a2;
    QCheckBox *a3;

    void setupUi(QWidget *spikesound)
    {
        if (spikesound->objectName().isEmpty())
            spikesound->setObjectName(QString::fromUtf8("spikesound"));
        spikesound->resize(880, 42);
        hboxLayout = new QHBoxLayout(spikesound);
        hboxLayout->setSpacing(3);
        hboxLayout->setContentsMargins(7, 7, 7, 7);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        source = new QComboBox(spikesound);
        source->setObjectName(QString::fromUtf8("source"));

        hboxLayout->addWidget(source);

        status = new QLabel(spikesound);
        status->setObjectName(QString::fromUtf8("status"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(status->sizePolicy().hasHeightForWidth());
        status->setSizePolicy(sizePolicy);
        status->setMinimumSize(QSize(80, 28));
        status->setMaximumSize(QSize(80, 28));
        status->setFrameShape(QFrame::Panel);
        status->setFrameShadow(QFrame::Sunken);
        status->setLineWidth(2);
        status->setWordWrap(false);
        status->setMargin(1);

        hboxLayout->addWidget(status);

        Spacer2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(Spacer2);

        playbutton = new QRadioButton(spikesound);
        playbutton->setObjectName(QString::fromUtf8("playbutton"));

        hboxLayout->addWidget(playbutton);

        Spacer3 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(Spacer3);

        TextLabel2 = new QLabel(spikesound);
        TextLabel2->setObjectName(QString::fromUtf8("TextLabel2"));
        TextLabel2->setWordWrap(false);

        hboxLayout->addWidget(TextLabel2);

        volume = new QSpinBox(spikesound);
        volume->setObjectName(QString::fromUtf8("volume"));
        volume->setMinimum(-99);
        volume->setMaximum(10);
        volume->setSingleStep(1);
        volume->setValue(-10);

        hboxLayout->addWidget(volume);

        Spacer5 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(Spacer5);

        negativeonly = new QRadioButton(spikesound);
        negativeonly->setObjectName(QString::fromUtf8("negativeonly"));

        hboxLayout->addWidget(negativeonly);

        Spacer4 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(Spacer4);

        rethresh = new QRadioButton(spikesound);
        rethresh->setObjectName(QString::fromUtf8("rethresh"));

        hboxLayout->addWidget(rethresh);

        threshold = new QSpinBox(spikesound);
        threshold->setObjectName(QString::fromUtf8("threshold"));
        threshold->setEnabled(false);
        threshold->setMinimum(100);
        threshold->setMaximum(999);
        threshold->setSingleStep(10);
        threshold->setValue(100);

        hboxLayout->addWidget(threshold);

        label004 = new QLabel(spikesound);
        label004->setObjectName(QString::fromUtf8("label004"));
        QFont font;
        font.setFamily(QString::fromUtf8("Symbol [Urw]"));
        label004->setFont(font);
        label004->setWordWrap(false);

        hboxLayout->addWidget(label004);

        Spacer6 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(Spacer6);

        a1 = new QCheckBox(spikesound);
        a1->setObjectName(QString::fromUtf8("a1"));

        hboxLayout->addWidget(a1);

        a2 = new QCheckBox(spikesound);
        a2->setObjectName(QString::fromUtf8("a2"));

        hboxLayout->addWidget(a2);

        a3 = new QCheckBox(spikesound);
        a3->setObjectName(QString::fromUtf8("a3"));

        hboxLayout->addWidget(a3);


        retranslateUi(spikesound);
        QObject::connect(rethresh, SIGNAL(toggled(bool)), threshold, SLOT(setEnabled(bool)));
        QObject::connect(volume, SIGNAL(valueChanged(int)), spikesound, SLOT(setVolume(int)));
        QObject::connect(threshold, SIGNAL(valueChanged(int)), spikesound, SLOT(setRethresh(int)));
        QObject::connect(source, SIGNAL(activated(QString)), spikesound, SLOT(setSource(QString)));
        QObject::connect(source, SIGNAL(textChanged(QString)), spikesound, SLOT(setSource(QString)));
        QObject::connect(playbutton, SIGNAL(toggled(bool)), spikesound, SLOT(setPlay(bool)));
        QObject::connect(a1, SIGNAL(toggled(bool)), spikesound, SLOT(setA1(bool)));
        QObject::connect(a2, SIGNAL(toggled(bool)), spikesound, SLOT(setA2(bool)));
        QObject::connect(a3, SIGNAL(toggled(bool)), spikesound, SLOT(setA3(bool)));
        QObject::connect(rethresh, SIGNAL(toggled(bool)), spikesound, SLOT(useRethresh(bool)));

        QMetaObject::connectSlotsByName(spikesound);
    } // setupUi

    void retranslateUi(QWidget *spikesound)
    {
        spikesound->setWindowTitle(QApplication::translate("spikesound", "Spike sound", 0, QApplication::UnicodeUTF8));
        source->clear();
        source->insertItems(0, QStringList()
         << QApplication::translate("spikesound", "spike", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("spikesound", "respike", 0, QApplication::UnicodeUTF8)
        );
        status->setText(QApplication::translate("spikesound", "pre-initial", 0, QApplication::UnicodeUTF8));
        playbutton->setText(QApplication::translate("spikesound", "Play", 0, QApplication::UnicodeUTF8));
        TextLabel2->setText(QApplication::translate("spikesound", "Volume:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        volume->setToolTip(QApplication::translate("spikesound", "Volume (dB)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        negativeonly->setText(QApplication::translate("spikesound", "-ve only", 0, QApplication::UnicodeUTF8));
        rethresh->setText(QApplication::translate("spikesound", "Rethresh:", 0, QApplication::UnicodeUTF8));
        threshold->setSuffix(QString());
        label004->setText(QApplication::translate("spikesound", "\302\264 0.01 q", 0, QApplication::UnicodeUTF8));
        a1->setText(QApplication::translate("spikesound", "A1", 0, QApplication::UnicodeUTF8));
        a2->setText(QApplication::translate("spikesound", "A2", 0, QApplication::UnicodeUTF8));
        a3->setText(QApplication::translate("spikesound", "A3", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class spikesound: public Ui_spikesound {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPIKESOUND_H
