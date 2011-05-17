// Argh.H

#ifndef ARGH_H

#define ARGH_H

#include <string>
#include <deque>
#include <map>

class Argh {
  /*:C Argh
   *:D Simple command line handling
   *:N Argh.C contains a simple example at the end.
   */
public:
  Argh();
  void flag(int id, std::string defn);
  /*:F flag
   *:D Define a new flag option.
   *:A id: integer ID of this option for use with has() and get().
       defn: definition of this option. The definition must take any of
         the forms:
           "-s --long Description"
           "-s --with-long Description"
           "-s --enable-long Description"
   *:N Either the short or the long name may be left out.
       Flags default to false unless they are defined through "-s1",
       "--long=true", or similar. Note that even "-s0" (confusingly)
       makes the flag default to 1.
  */
  void arg(int id, std::string defn);
  /*:F arg
   *:D Define a new non-flag option.
   *:A id: integer ID of this option for use with has() and get().
       defn: definition of this option. The definition must take the form:
         "-s --long Description"
   *:N Either the short or the long name may be left out.
       Default values for non-flags may be given by "-svalue" or
       "--long==value".
  */
  int parse(int argc, char **argv);
  /*:F parse
   *:D Parse command line options
   *:A argc, argv: as passed into main()
   *:R index of first non-option argument or -1 on error
   *:N For flag options, we understand "--with-long", "--without-long", or
       "--enable-long", "--disable-long" if the flag was defined using
       the "--with-long" or "--enable-long" forms. We also understand
       "-s0". Anything else (including "-sno", "-sfalse", etc.) enables
       the flag.
  */
  bool has(int id);
  /*:F has
   *:D For flag options, returns true iff the flag is set.
       For non-flag options, returns true if the option is given.
  */
  std::string get(int id);
  /*:F get
   *:D For non-flag options, returns the value of the option,
       or null if not given.
       Do not use for flag options.
  */
  std::deque<std::string> helptext();
  /*:F help
   *:D Returns list of all definitions
   */
  void help(std::string pre="", std::string post="");
  /*:F help
   *:D Writes help text to stderr.
   */
  void usage(std::string prog, std::string arg="");
protected:
  virtual void unknownOption(std::string opt);
  /*:F unknownOption
   *:D This function is called when an unknown option is given on the
       command line. The default implementation writes a note to stderr.
   */
  virtual void missingArgument(std::string opt);
  /*:F missingArgument
   *:D This function is called when a known non-flag option is given on
       the command line without an argument.
       The default implementation writes a note to stderr.
   */
private:
  enum OptType {
    otARG,
    otFLAG,
    otWITH,
    otENABLE
  };
  struct Option {
    int id;
    std::string shortName;
    std::string longName;
    std::string description;
    std::string defaultValue;
    OptType type;
  };
  std::map<int, Option> options;
  std::map<int, std::string> values;
private:
  static void fillOption(Option &x, std::string defn, bool isFlag);
};

#endif
