/*
    Copyright (c) 2016 Hubert Jarosz

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
       claim that you wrote the original software. If you use this software
       in a product, an acknowledgement in the product documentation would be
       appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
       misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

bool good_char( char ch ) {
  if( ('A' <= ch && ch <= 'Z') || ('a' <= ch && ch <= 'z') ) {
    return true;
  }
  return false;
}

std::string sanitize( std::string str ) {
  std::string sane_str = str;
  for( size_t i = 0; i < sane_str.length(); i++ ) {
    if( ! good_char(str[i]) ) {
      sane_str[i] = '_';
    }
  }
  return sane_str;
}

int main(int argc, char* argv[]) {

  if( argc != 3 ) {
    std::cerr << "Usage: include-bin INFILE OUTFILE" << argc << std::endl;
    return 1;
  }

  std::ifstream fin( argv[1], std::ios_base::binary );
  std::ofstream fout( argv[2] );

  int b = 0, count = 0;

  std::string name = sanitize(argv[1]);

  fout << "unsigned char " << name << "[] = {\n  ";

  while( (b = fin.get()) != std::istream::traits_type::eof() ) {
    std::stringstream stream;
    stream << std::setfill('0') << std::setw(2) << std::hex << b;
    if( count > 0 ) {
      if( count%12 == 0 ) {
        fout << ",\n  ";
      } else {
        fout << ", ";
      }
    }
    fout << "0x" << stream.str();
    count++;
  }

  fout << "\n};\nunsigned int " << name << "_len = " << count << ";\n";
  fout << std::flush;
  fout.close();

  return 0;
}
