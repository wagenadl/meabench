// tern.H

#ifndef TERN_H

#define TERN_H

class tern {
public:
  tern(bool x) { known=true; value=x; }
  tern() { known = false; }
  tern &operator=(bool x) { known=true; value=x; return *this; }
  bool istrue() const { return known && value; }
  bool isfalse() const { return known && !value; }
  bool isknown() const { return known; }
public:
  static const class tern undef;
private:
  bool known, value;
};

#endif
