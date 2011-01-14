// Scrollbar.H

#ifndef SCROLLBAR_H

#define SCROLLBAR_H

#include <qscrollbar.h>

class Scrollbar: public QScrollBar {
  Q_OBJECT;
public:
  Scrollbar(QWidget *parent, long long t0, long long t1, long long dt);
  void setRange(long long t0, long long t1);
  void setStep(long long dt);
signals:
  void moved(long long t0, long long dt);
public slots:
  virtual void polish();
private slots:
  void moved(int);
private:
  long long t0;
  long long t1;
  long long dt;
};

#endif
