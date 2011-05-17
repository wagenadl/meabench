// Argh.C

#include "Argh.H"
#include <iostream>

Argh::Argh() {
}

void split(std::string const &s, std::deque<std::string> &dst,
	   char const *delim) {
  dst.clear();
  std::string::size_type nextstart = s.find_first_not_of(delim);
  while (nextstart!=std::string::npos) {
    std::string::size_type nextend = s.find_first_of(delim, nextstart);
    if (nextend==std::string::npos) {
      dst.push_back(s.substr(nextstart));
      return;
    } else {
      dst.push_back(s.substr(nextstart, nextend-nextstart));
    }
    nextstart = s.find_first_not_of(delim, nextend);
  }
  return;
}

void join(std::deque<std::string> const &sl, std::string &dst,
	  std::string const &delim) {
  bool first = true;
  dst = "";
  for (std::deque<std::string>::const_iterator k=sl.begin();
       k!=sl.end(); ++k) {
    if (first)
      first = false;
    else
      dst += delim;
    dst += *k;
  }
}      

bool startsWith(std::string const &s, std::string const &t) {
  if (s.size()<t.size())
    return false;
  return s.substr(0,t.size()) == t;
}

void Argh::fillOption(Option &x, std::string defn, bool isflag) {
  x.type = isflag ? otFLAG : otARG;
  std::deque<std::string> bits; split(defn, bits, " \t\n");
  if (startsWith(bits[0], "-") && !startsWith(bits[0], "--")) {
    // short option
    if (bits[0].size()>=2) {
      x.shortName = bits[0].substr(1,1);
      if (bits[0].size()>2) 
	x.defaultValue=bits[0].substr(2);
    }
    bits.pop_front();
  }
  if (startsWith(bits[0], "--")) {
    // long option
    std::deque<std::string> kv; split(bits[0].substr(2), kv, "=");
    if (!kv.empty()) {
      x.longName = kv[0];
      kv.pop_front();
      if (startsWith(x.longName, "with-")) {
	x.type = otWITH;
	x.longName = x.longName.substr(5);
      } else if (startsWith(x.longName, "enable-")) {
	x.type = otENABLE;
	x.longName = x.longName.substr(7);
      }
      if (!kv.empty())
	join(kv, x.defaultValue, "=");
      bits.pop_front();
    }
  }
  join(bits, x.description, " ");
}

void Argh::flag(int id, std::string defn) {
  Option o;
  fillOption(o, defn, true);
  options[id] = o;
}

void Argh::arg(int id, std::string defn) {
  Option o;
  fillOption(o, defn, false);
  options[id] = o;
}

bool Argh::has(int id) {
  return values.find(id) != values.end();
}

std::string Argh::get(int id) {
  return values[id];
}

std::deque<std::string> Argh::helptext() {
  std::deque<std::string> res;
  for (std::map<int, Option>::iterator k=options.begin();
       k!=options.end(); ++k) {
    int id = (*k).first;
    Option const &x = (*k).second;
    std::string line;
    std::string sep="";
    if (!x.shortName.empty()) {
      line += "-" + x.shortName;
      sep=", ";
    }
    if (!x.longName.empty()) {
      line += sep;
      if (x.type==otENABLE)
	line += "--enable-" + x.longName + "/--disable-";
      else if (x.type==otWITH)
	line += "--with-" + x.longName + "/--without-";
      else
	line += "--";
      line += x.longName;
    }
    line += ": " + x.description;
    if (!x.defaultValue.empty())
      line += " (default: " + x.defaultValue + ")";
    res.push_back(line);
  }
  return res;
}

int Argh::parse(int argc, char **argv) {
  values.clear();

  std::map<std::string, int> shortopts;
  std::map<std::string, int> longopts;

  for (std::map<int, Option>::iterator k=options.begin();
       k!=options.end(); ++k) {
    int id = (*k).first;
    if (!options[id].defaultValue.empty())
      values[id] = options[id].defaultValue;
    if (!options[id].shortName.empty())
      shortopts[options[id].shortName] = id;
    if (!options[id].longName.empty())
      longopts[options[id].longName] = id;
  }
  
  int k=1;
  while (k<argc) {
    std::string arg = argv[k];
    if (arg=="--")
      return k+1;
    else if (startsWith(arg, "--enable-")) {
      std::string key = arg.substr(9);
      if (longopts.count(key)>0 && options[longopts[key]].type==otENABLE)
	values[longopts[key]] = "yes";
      else
	unknownOption(arg);
    } else if (startsWith(arg, "--disable-")) {
      std::string key = arg.substr(10);
      if (longopts.count(key)>0 && options[longopts[key]].type==otENABLE)
	values.erase(longopts[key]);
      else
	unknownOption(arg);
    } else if (startsWith(arg, "--with-")) {
      std::string key = arg.substr(7);
      if (longopts.count(key)>0 && options[longopts[key]].type==otWITH)
	values[longopts[key]] = "yes";
      else
	unknownOption(arg);
    } else if (startsWith(arg, "--without-")) {
      std::string key = arg.substr(10);
      if (longopts.count(key)>0 && options[longopts[key]].type==otWITH)
	values.erase(longopts[key]);
      else
	unknownOption(arg);
    } else if (startsWith(arg, "--")) {
      if (arg.find_first_of("=")!=arg.npos) {
	std::deque<std::string> kv; split(arg.substr(2), kv, "=");
	std::string key = kv[0]; kv.pop_front();
	std::string val; join(kv, val, "=");
	if (!longopts.count(key)>0 || options[longopts[key]].type!=otARG) {
	  unknownOption("--" + key);
	  return -1;
	}
	if (val.empty())
	  val="";
	values[longopts[key]] = val;
      } else {
	std::string key = arg.substr(2);
	if (longopts.count(key)==0) {
	  unknownOption(key);
	  return -1;
	}
	int id = longopts[key];
	if (options[id].type==otFLAG)
	  values[id] = "yes";
	else if (k<argc-1)
	  values[id] = argv[++k];
	else {
	  missingArgument(key);
	  return -1;
	}
      }
    } else if (startsWith(arg, "-")) {
      // short option
      if (arg.size()<2)
	return k;
      std::string key = arg.substr(1,1);
      if (shortopts.count(key)==0) {
	unknownOption("-" + key);
	return -1;
      }
      if (options[shortopts[key]].type==otARG) {
	if (arg.size()>2) 
	  values[shortopts[key]] = arg.substr(2);
	else if (k<argc-1)
	  values[shortopts[key]] = argv[++k];
	else {
	  missingArgument(key);
	  return -1;
	}
      } else {
	if (arg.substr(2)=="0")
	  values.erase(shortopts[key]);
	else
	  values[shortopts[key]] = "yes";
      }
    } else {
      break;
    }
    k++;
  }
  return k;
}

void Argh::unknownOption(std::string opt) {
  std::cerr << "Unknown option: " << opt << "\n";
}

void Argh::missingArgument(std::string opt) {
  std::cerr << "Missing argument to: " << opt << "\n";
}

void Argh::help(std::string pre, std::string post) {
  if (!pre.empty())
    std::cerr << pre << "\n";
  std::deque<std::string> s = helptext();
  for (std::deque<std::string>::iterator k=s.begin(); k!=s.end(); k++)
    std::cerr << *k << "\n";
  if (!post.empty())
    std::cerr << post << "\n";
}

void Argh::usage(std::string prog, std::string arg) {
  std::cerr << "Usage: " << prog;
  std::string shortopts;
  for (std::map<int, Option>::iterator k=options.begin();
       k!=options.end(); ++k)
    shortopts += (*k).second.shortName;
  if (!shortopts.empty())
    std::cerr << " -" << shortopts;
  if (!arg.empty())
    std::cerr << " " << arg;
  std::cerr << "\n";
}

#ifdef TEST
int main(int argc, char **argv) {
  std::cout << "Argh Test\n";

  Argh a;
  enum {
    argTESTS,
    argFREQ,
    argGLOBAL,
    argHELP,
    argFLIP,
    
  };
  a.flag(argFLIP, "--with-flip Do a summersault");
  a.flag(argTESTS, "-t --enable-tests Run some tests");
  a.flag(argHELP, "-h	 Show help text");
  a.arg(argFREQ, "-f --freq=25 Set frequency");
  a.arg(argGLOBAL, "-g10 --global Set global value");

  int arg0 = a.parse(argc, argv);
  std::cout << "Have F: " << a.has(argFLIP) << "\n";
  std::cout << "Have t: " << a.has(argTESTS) << "\n";
  std::cout << "Have f: " << a.has(argFREQ) << "\n";
  if (a.has(argFREQ))
    std::cout << "  f: " << a.get(argFREQ) << "\n";
  std::cout << "Have g: " << a.has(argGLOBAL) << "\n";
  if (a.has(argGLOBAL))
    std::cout << "  g: " << a.get(argGLOBAL) << "\n";
  if (a.has(argHELP)) {
    std::string s; join(a.help(), s, "\n");
    std::cout << "Help:\n" << s << "\n";
  }

  std::cout << "arg0: " << arg0 << "\n";
  
  return 0;
}
#endif
