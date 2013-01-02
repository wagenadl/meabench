// flexraster.C



#include <common/MEAB.H>
#include <base/Error.H>
#include <stdio.h>
#include <unistd.h>
#include <qapplication.h>
#include "ControlPanel.H"
#include "Storage.H"

int main(int argc, char **argv) {
  MEAB::announce("flexraster");
  //#ifdef TEST
  //    switchdbx(1);

  try {
    Storage storage;
    QApplication qapp(argc,argv);
    ControlPanel ctrlp(&storage);
    ctrlp.update_source("spike");
    qapp.exec();
    return 0;
  } catch (Error const &e) {
    dbx("Top level exception");
    e.report();
    fprintf(stderr,"Unexpected exception at top level - quitting\n");
    return 1;
  } catch (...) {
    fprintf(stderr,"Unknown exception\n");
    fprintf(stderr,"Unexpected exception at top level - quitting\n");
    return 2;
  }
}

