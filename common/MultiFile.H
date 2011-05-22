// MultiFile.H

#ifndef MULTIFILE_H

#define MULTIFILE_H

#include <stdio.h>
#include <string>
#include <base/Timeref.H>
#include <base/Error.H>
#include <base/Sprintf.H>

using namespace std;

class MultiFile {
  /*:C MultiFile
   *:D Class to read efficiently from MEABench split files
   */
public:
  MultiFile(string fn) {
  /*:F constructor
   *:D Opens the named file for reading, exception if not found.
   */
    basename = fn;
    fileno = 0;
    offset_at_start_of_file = 0;
    fh = fopen(fn.c_str(), "rb");
    if (!fh)
      throw Error("MultiFile", "File not found");
  }
  MultiFile(FILE *fh) {
  /*:F constructor
   *:D Works on an existing file handle. (For convenience; this does not
       offer true multi-file support.)
  */
    basename = "-";
    fileno = -1;
    offset_at_start_of_file = 0;
  }
  ~MultiFile() {
  /*:F destructor
   *:D Closes the file.
   */
    if (fh)
      fclose(fh);
  }    
  size_t read(void *dest, size_t reclen, size_t nrecs) {
  /*:F read
   *:D Like fread, but works across file boundaries.
   *:N Returns less than nrecs at eof; error conditions throw exception.
   */
    size_t r = fread(dest, reclen, nrecs, fh);
    while (r<nrecs) {
      if (ferror(fh)) 
	throw Error("MultiFile", "Read error");
      offset_at_start_of_file += ftell(fh);
      fclose(fh);
      if (fileno<0)
	return r;
      fileno++;
      fh = fopen((basename+Sprintf("-%i", fileno)).c_str(), "rb");
      if (fh==0)
	return r;
      r += fread(dest, reclen, nrecs-r, fh);
    }
    return r;
  }    
  void seek(timeref_t off) {
  /*:F seek
   *:D Like fseek, but works across file boundaries.
   *:N Only seeking relative to start is supported.
   */
    if (!fh || off<offset_at_start_of_file) {
      if (fh)
	fclose(fh);
      fileno = 0;
      offset_at_start_of_file = 0;
      fh = fopen(basename.c_str(), "rb");
    }
    
    fseek(fh, 0, SEEK_END);
    timeref_t flen = ftell(fh);
    while (off>=offset_at_start_of_file + flen) {
      if (fileno<0) {
	if (off == offset_at_start_of_file + flen)
	  return;
	else {
	  fprintf(stderr, "offset=%Li flen=%Li\n",off, flen);
	  throw Error("MultiFile", "Seek past eof");
	}
      }
      fclose(fh);
      fileno ++;
      offset_at_start_of_file += flen;
      fh = fopen((basename+Sprintf("-%i", fileno)).c_str(), "rb");
      if (fh==0) {
	if (off == offset_at_start_of_file)
	  return;
	else {
	  fprintf(stderr, "mf: offset=%Li flen=%Li fileno=%i offset_at_start=%i\n",off, flen, fileno, offset_at_start_of_file);	  
	  throw Error("MultiFile", "Seek past eof");
	}
      }
      fseek(fh, 0, SEEK_END);
      flen = ftell(fh);
    }
    if (fh)
      fseek(fh, off-offset_at_start_of_file, SEEK_SET);
  }
  timeref_t tell() const {
  /*:F tell
   *:D Like ftell, but works across file boundaries.
   */
    if (fh)
      return ftell(fh) + offset_at_start_of_file;
    else
      return offset_at_start_of_file;
  }
private:
  string basename;
  int fileno;
  timeref_t offset_at_start_of_file;
  FILE *fh;
};

#endif
