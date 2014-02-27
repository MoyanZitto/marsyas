/*
** Copyright (C) 1998-2006 George Tzanetakis <gtzan@cs.uvic.ca>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef MARSYAS_SIMPLESTRETCH_H
#define MARSYAS_SIMPLESTRETCH_H

#include <marsyas/system/MarSystem.h>

namespace Marsyas
{
/**
	\ingroup none
	\brief Stretch samples of input observation number 0.

	Controls:
	- *mrs_real/factor* [w]: Factor to stretch by. Default: 1.0.
    WARNING: this MarSystem only works for integer values!
*/


class SimpleStretch: public MarSystem
{
private:
  void addControls();

public:
  SimpleStretch(std::string name);
  ~SimpleStretch();
  MarSystem* clone() const;

  void myProcess(realvec& in, realvec& out);
};

}//namespace Marsyas

#endif


