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


/** 
    \class realvec
    \brief Vector of mrs_real values

    Array (vector in the numerical sense) of mrs_real values. Basic 
    arithmetic operations and statistics are supported. 
*/

#include "realvec.h"
#include "NumericLib.h"
#include <algorithm>
#include <limits>

#ifdef MARSYAS_MATLAB
//#define _MATLAB_REALVEC_
#endif

using namespace std;
using namespace Marsyas;


realvec::realvec()
{
  size_ = 0;
	allocatedSize_ = 0;
  data_ = NULL;
  rows_ = 0;  // [!! 0]
  cols_ = size_;
}

realvec::~realvec()
{
  if (size_ == 0)
    data_ = NULL;

  delete [] data_;
  data_ = NULL;
}

realvec::realvec(mrs_natural size)
{
  size_ = size;
	allocatedSize_ = size;
  if (size_ > 0) 
    data_ = new mrs_real[size_];
  rows_ = 1;
  cols_ = size_;
}

realvec::realvec(mrs_natural rows, mrs_natural cols)
{
  
  size_ = rows * cols;
  allocatedSize_ = size_;
  if (size_ > 0) 
    data_ = new mrs_real[size_];
  rows_ = rows;
  cols_ = cols;
}


realvec::realvec(const realvec& a) : size_(a.size_),allocatedSize_(a.size_), 
				     rows_(a.rows_), cols_(a.cols_)
{
  data_ = NULL; 
  if( a.size_ > 0 )
    data_ = new mrs_real[a.size_];
  for (mrs_natural i=0; i<size_; i++)
    data_[i] = a.data_[i];
}

realvec& 
realvec::operator=(const realvec& a)
{
	if (this != &a)
	{
		if (size_ != a.size_)
		{
			//lmartins: [!]
			//Why doesn't this delete all data and creates a new realvec?
			//it would be then easier to use this operator in client code 
			//(i.e. no need to assure that the realvec implicated in the 
			// =  operation are equal sized)
			/*
			MRSERR("realvec::operator= : Different realvec sizes\n");
			MRSERR("realvec left unchanged\n");

			MRSERR("Left size = " + size_);
			MRSERR("Right size = " + a.size_);
			return *this;
			*/
			//Replacing above code by the following one (which still maintains backward compatibility
			// with any previous code in Marsyas that resizes l.h. realvec before using the = operator).

			//			MRSWARN("realvec::operator= : Different realvec sizes! l.h. realvec will be deleted and then recreated during attribution.");
			// [ML] this code is used by stretch on realvec_ctrl, removed warning until sloved issue

			delete [] data_;
			data_ = NULL;
			if( a.size_ > 0 )
				data_ = new mrs_real[a.size_];
			for (mrs_natural i=0; i<a.size_; i++)
				data_[i] = a.data_[i];
			size_ = a.size_;
			allocatedSize_ = a.allocatedSize_;
			rows_ = a.rows_;
			cols_ = a.cols_;
		}
		else
		{
			for (mrs_natural i=0; i < size_; i++)
				data_[i] = a.data_[i];
			rows_ = a.rows_;
			cols_ = a.cols_;
		}
	}

	return *this;
}

mrs_real *
realvec::getData() const
{
  return data_;
}

void
realvec::transpose()
{
  mrs_real *tmp_ = new mrs_real[size_];
  
for (mrs_natural i=0; i < rows_; i++)
for (mrs_natural j=0; j < cols_; j++)
   tmp_[i * cols_ + j] = data_[j * rows_ + i];

  mrs_natural tmp = rows_;
  rows_ = cols_;
  cols_ = tmp;

  delete [] data_;
  data_ = tmp_;
}

mrs_real 
realvec::median() const
{
  realvec tmp(*this);
	mrs_real *tmpData = tmp.data_;
	std::sort(tmpData, tmpData+size_);
	return tmpData[size_/2];
}

mrs_real 
realvec::mean() const
{
  mrs_real sum = 0.0;
  for (mrs_natural i=0; i < size_; i++)
    {
      sum += data_[i];
    }
  if (sum != 0.0) sum /= size_;
  return sum;
}

// [ML] assumes one dimesional vector !!
void
realvec::sort()
{
	std::sort(data_, data_+size_);
}

mrs_real 
realvec::sum() const
{
  mrs_real sum = 0.0;
  for (mrs_natural i=0; i < size_; i++)
    {
      sum += data_[i];
    }
  return sum;
}


mrs_real 
realvec::var() const
{
  mrs_real sum = 0.0;
  mrs_real sum_sq = 0.0;
  mrs_real val;
  mrs_real var;
  
  for (mrs_natural i=0; i < size_; i++)
    {
      val = data_[i];
      sum += val;
      sum_sq += (val * val);
    }
  if (sum != 0.0) sum /= size_;
  if (sum_sq != 0.0) sum_sq /= size_;
  
  var = sum_sq - sum * sum;
  return var;
}



mrs_real 
realvec::std() const
{
  return sqrt(var());
}

 
mrs_natural 
realvec::getRows() const
{
  return rows_;
}


mrs_natural 
realvec::getCols() const
{
  return cols_;
}


mrs_natural
realvec::getSize() const
{
  return size_;
}

void 
realvec::debug_info()
{
  MRSERR("realvec information");
  MRSERR("size = " + size_);
  
}

void 
realvec::create(mrs_natural size)
{
  delete [] data_;
  data_ = NULL;
  size_ = size;
	allocatedSize_ = size;
  if (size_ > 0) 
    data_ = new mrs_real[size_];
  rows_ = 1;
  cols_ = size;
  
    
  for (mrs_natural i=0; i<size_; i++)
    data_[i] = 0.0;
}


/* keep the old data and possibly extend */ 
void 
realvec::stretch(mrs_natural size) 
{
	if (size_ == size) 
		return;

	if(size < allocatedSize_)
	{
		size_ = size;
		rows_ = 1;
		cols_ = size_;
		return;
	}

	mrs_real *ndata = NULL;
	if (size > 0) 
		ndata = new mrs_real[size];
	// zero new data 
	for (mrs_natural i=0; i < size; i++)
	{
		if (i < size_) 
			ndata[i] = data_[i];
		else 
			ndata[i] = 0.0;
	}
	delete [] data_;
	data_ = NULL;
	data_ = ndata;
	size_ = size;
	allocatedSize_ = size;
	rows_ = 1;
	cols_ = size;

}

/* keep the old data and possibly extend */ 
void 
realvec::stretch(mrs_natural rows, mrs_natural cols) 
{
  int size = rows * cols;

	if ((rows == rows_)&&(cols == cols_))
    return;

	/*if(size < size_)  [!] should be improved as the simpler stretch function
	{
		size_ = size;
		rows_ = rows;
		cols_ = cols;
		return;
	}*/
  
  mrs_real *ndata = NULL;
  
  if (size > 0) 
    ndata = new mrs_real[size];
  
	// copy and zero new data 
	for (mrs_natural r=0; r < rows; r++)
		for (mrs_natural c = 0; c < cols; c++) 
		{
			if ((r < rows_)&&(c < cols_)) 
				ndata[c * rows + r] = data_[c * rows_ + r];
			else 
				ndata[c * rows + r] = 0.0;
		}
		if (data_) 
		{
			delete [] data_;
			data_ = NULL;
		}
		data_ = ndata;
		size_ = size;
		allocatedSize_ = size;
		rows_ = rows;
		cols_ = cols;
}

void
realvec::stretchWrite(const mrs_natural pos, const mrs_real val)
{
  // don't forget the zero-indexing position!
  mrs_natural wantSize = pos+1;
  if (wantSize >= size_)
    if ( wantSize < 2*size_ )
      // grow exponentially with sequential access
      stretch( 2*size_ );
    else
      // if we have a sudden large value, don't double it
      stretch( wantSize );
  // FIXME: add a MRSASSERT here for debugging.
  // cout<<"List stretched to "<<size_<<endl;
  data_[pos] = val;
}

void
realvec::stretchWrite(const mrs_natural r, const mrs_natural c, const mrs_real val)
{
 	mrs_natural nextR = rows_;
 	mrs_natural nextC = cols_;
	mrs_natural wantR = r+1;
	mrs_natural wantC = c+1;
	if ( (wantR >= rows_) || (wantC >= cols_) ) {
		if ( wantR >= rows_ )
			if ( wantR < 2*rows_ )
				nextR = 2*rows_;
			else
				nextR = wantR;

		if ( wantC >= cols_ )
			if ( wantC < 2*cols_ )
				nextC = 2*cols_;
			else
				nextC = wantC;

	  stretch( nextR, nextC );
  	// FIXME: add a MRSASSERT here for debugging.
  	// cout<<"List stretched to "<<rows_<<", "<<cols_<<endl;
  }
	data_[c * rows_ + r] = val;
}

void 
realvec::create(mrs_natural rows, mrs_natural cols)
{
  delete [] data_;
  data_ = NULL;
  size_ = rows * cols;
  rows_ = rows;
  cols_ = cols;
  if (size_ > 0) 
    data_ = new mrs_real[size_];
  for (mrs_natural i=0; i<size_; i++)
    data_[i] = 0.0;
}

void 
realvec::create(mrs_real val, mrs_natural rows, mrs_natural cols)
{
  size_ = rows * cols;
  rows_ = rows;
  cols_ = cols;
  delete [] data_;
  data_ = NULL;
  if (size_ > 0) 
    data_ = new mrs_real[size_];
  for (mrs_natural i=0; i<size_; i++)
    data_[i] = val;
}

void 
realvec::allocate(mrs_natural size)
{
  delete [] data_;
  data_ = NULL;
  size_ = size;
	allocatedSize_ = size;
  if (size_ > 0) 
    data_ = new mrs_real[size_];
}

void 
realvec::setval(mrs_natural start, mrs_natural end, mrs_real val)
{
  MRSASSERT(start <= size_);
  MRSASSERT(end <= size_);

  for (mrs_natural i=start; i<end; i++)
    {
      data_[i] = val;
    }
}



void realvec::apply(mrs_real (*func) (mrs_real))
{

  for (mrs_natural i=0; i<size_; i++)
    {
      data_[i] = func(data_[i]);
    }
}

void 
realvec::setval(mrs_real val)
{
  for (mrs_natural i=0; i<size_; i++)
    {
      data_[i] = val;
    }
}

void 
realvec::abs()
{
  for (mrs_natural i=0; i<size_; i++)
    {
      data_[i] = fabs(data_[i]);
    }
  
}

void 
realvec::renorm(mrs_real old_mean, mrs_real old_std, mrs_real new_mean, mrs_real new_std)
{
  mrs_natural i;
  
  for(i=0; i < size_; i++)
    {
      data_[i] = (data_[i] - old_mean) / old_std;
      data_[i] *= new_std;
      data_[i] += new_mean;
    }
}

mrs_natural 
realvec::invert(realvec& res)
{
  if(rows_ != cols_)
    {
      MRSERR("realvec::invert() - matrix should be square!");
      return -1;
    }
	
  mrs_natural rank;
  mrs_natural r,c,i;
  mrs_real temp;

  res.create(rows_, cols_);

  rank = 0;
  for (r = 0; r < rows_; r++)
    for (c=0; c < cols_; c++)
      {
	if (r == c) 
	  res(r,c) = 1.0;
	else 
	  res(r,c) = 0.0;
      }
  for (i = 0; i < rows_; i++)
    {
      if ((*this)(i,i) == 0)
	{
	  for (r = i; r < rows_; r++)
	    for (c = 0; c < cols_; c++)
	      {
		(*this)(i,c) += (*this)(r,c);
		res(i,c) += res(r,c);
	      }
	}
      for (r = i; r < rows_; r++) 
	{
	  temp = (*this)(r,i);
	  if (temp != 0) 
	    for (c =0; c < cols_; c++)
	      {
		(*this)(r,c) /= temp;
		res(r,c) /= temp;
	      }
	}
      if (i != rows_-1)
	{
	  for (r = i+1; r < rows_; r++)
	    {
	      temp = (*this)(r,i);
	      if (temp != 0.0) 
		for (c=0; c < cols_; c++)
		  {
		    (*this)(r,c) -= (*this)(i,c);
		    res(r,c) -= res(i,c);
		  }
	    }
	}
    }
  for (i=1; i < rows_; i++)
    for (r=0; r < i; r++)
      {
	temp = (*this)(r,i);
	for (c=0; c < cols_; c++)
	  {
	    (*this)(r,c) -= (temp * (*this)(i,c));
	    res(r,c) -= (temp * res(i,c));
	  }
      }
  for (r =0; r < rows_; r++)
    for (c=0; c < cols_; c++)
      (*this)(r,c) = res(r,c);
  return rank;
}


void
realvec::sqr()
{
  for (mrs_natural i=0; i<size_; i++)
    {
      data_[i] *= data_[i];
    }
}


mrs_natural
realvec::search(mrs_real val)
{
	mrs_real minDiff = MAXREAL;
  mrs_natural index=-1;
  for (mrs_natural i=0; i<size_; i++)
    if (fabs(data_[i]-val)< minDiff)
		{
minDiff = fabs(data_[i]-val);
index=i;
    }
		return index;
}

void
realvec::sqroot()
{
  for (mrs_natural i=0; i<size_; i++)
    {
      data_[i] = sqrt(data_[i]);
    }
}


realvec 
Marsyas::operator+(const realvec& vec1, const realvec& vec2)
{
  mrs_natural size;
  mrs_natural i;
  if (vec1.size_ != vec2.size_)
    MRSERR("Size of realvecs does not match");
  if (vec1.size_ >= vec2.size_)
    size = vec1.size_;
  else 
    size = vec2.size_;
  realvec sum;
  sum.create(size);    
  
  for (i=0; i<vec1.size_; i++)
    {
      sum.data_[i] = vec1.data_[i];
    }
  for (i=0; i<vec2.size_; i++)
    {
      sum.data_[i] += vec2.data_[i];
    }
      
  return sum;
}


realvec 
Marsyas::operator-(const realvec& vec1, const realvec& vec2)
{
  mrs_natural size;
  mrs_natural i;
  if (vec1.size_ != vec2.size_)
    MRSERR("Size of realvecs does not match");
  if (vec1.size_ >= vec2.size_)
    size = vec1.size_;
  else 
    size = vec2.size_;
  realvec diff;
  diff.create(size);    
  
  for (i=0; i<vec1.size_; i++)
    {
      diff.data_[i] = vec1.data_[i];
    }
  for (i=0; i<vec2.size_; i++)
    {
      diff.data_[i] -= vec2.data_[i];
    }
      
  return diff;
}

bool
Marsyas::operator!=(const realvec& v1, const realvec& v2)
{
	//if vectors have the same dimensions, compare all their values
	if(v1.cols_ == v2.cols_ && v1.rows_ == v2.rows_)
	{
		for(mrs_natural r = 0; r < v1.rows_; ++r)
			for (mrs_natural c = 0; c < v1.cols_; ++c)
				if(v1(r,c) != v2(r,c))
					return true; //at least one value is diferent!
		
		//all values are equal!
		return false;
	}
	//if their dimensions are different... that basically means they are diferent! ;-)
	else 
		return true;
}

void 
realvec::send(Communicator *com)
{ 
  mrs_natural i;
  static char *buf = new char[256];
  string message;
  sprintf(buf, "%i\n", (int)size_);
  message = buf;
  com->send_message(message);
  for (i=0; i<size_; i++)
    {
      sprintf(buf, "%f\n", data_[i]);
      message = buf;
      com->send_message(message);
    }
  MRSERR("realvec::send numbers were sent to the client");
  
}

void
realvec::read(string filename)
{
  ifstream from(filename.c_str());
  from >> (*this);  
}


void 
realvec::shuffle() 
{
  mrs_natural size = cols_;
  mrs_natural rind;
  mrs_real temp;
  
  for (int c=0;  c < cols_; c++)
    {
      rind = (unsigned int)(((mrs_real)rand() / (mrs_real)(RAND_MAX))*size);
      for (int r=0; r < rows_; r++) 
	{
	  temp = data_[c * rows_ + r];
	  data_[c * rows_ + r] = data_[rind * rows_ + r];
	  data_[rind * rows_ + r] = temp;
	}
    }  
}


void 
realvec::write(string filename) const
{
  ofstream os(filename.c_str());
  os << (*this) << endl;
}
void 
realvec::dump()
{
	for(int i =0 ; i< size_ ; i++)
		cout << data_[i] << " " ;
	cout << endl;
}

void
realvec::readText(string filename)
{
	ifstream infile(filename.c_str());
	int i = 0;
	if (size_ == 0)
		allocate(1);
	mrs_real value;
	while (infile >> value) {
		// slower but safer
		stretchWrite(i,value);
		i++;
	}
	stretch(i-1);
	infile.close();
}

void
realvec::writeText(string filename)
{
	if (size_ == 0)
		return ;
	ofstream outfile(filename.c_str());
	for (int i=0; i<size_; i++) {
		outfile << data_[i] <<endl;
	}
	outfile.close();
}

ostream& 
Marsyas::operator<< (ostream& o, const realvec& vec)
{
  o << "# MARSYAS mrs_realvec" << endl;
  o << "# Size = " << vec.size_ << endl << endl;
  o << endl;

  
  o << "# type: matrix" << endl;
  o << "# rows: " << vec.rows_ << endl;
  o << "# columns: " << vec.cols_ << endl;

  for (mrs_natural r=0; r < vec.rows_; r++)
    {
      for (mrs_natural c=0; c < vec.cols_; c++)
	o << vec.data_[c * vec.rows_ + r] << " " ;
      o << endl;
    }
  
  // for (mrs_natural i=0; i<vec.size_; i++)
  // o << vec.data_[i] << endl;
  o << endl;
  o << "# Size = " << vec.size_ << endl;
  o << "# MARSYAS mrs_realvec" << endl;  
  return o;
}

istream& 
Marsyas::operator>>(istream& is, realvec& vec)
{
  if (vec.size_ != 0)
    {
      MRSERR("realvec::operator>>: realvec already allocated cannot read from istream");
      MRSERR("vec.size_ = " + vec.size_);
      
      
      return is;
    }
  string str0,str1,str2;
  mrs_natural size;
  mrs_natural i;
  
  is >> str0;
  is >> str1;
  is >> str2;
  
  if ((str0 != "#") || (str1 != "MARSYAS") || (str2 != "mrs_realvec"))
    {
      MRSERR("realvec::operator>>: Problem1 reading realvec object from istream");
      cout << "-str0 = " << str0 << endl;
      cout << "-str1 = " << str1 << endl;
      cout << "-str2 = " << str2 << endl;
      return is;
    }
  is >> str0;				
  is >> str1;
  is >> str2;
  if ((str0 != "#") || (str1 != "Size") || (str2 != "="))
    {
      MRSERR("realvec::operator>>: Problem2 reading realvec object from istream");
      return is;
    }
  is >> size;

  vec.allocate(size);  
  for (i=0; i<3; i++)
    {
      is >> str0;
    }
  is >> str0 >> str1 >> vec.rows_;
  is >> str0 >> str1 >> vec.cols_;
  

  for (mrs_natural r = 0; r < vec.rows_; r++)
    for (mrs_natural c = 0; c < vec.cols_; c++)
      {
	is >> vec.data_[c * vec.rows_ + r];
      }


  is >> str0;				
  is >> str1;
  is >> str2;
  if ((str0 != "#") || (str1 != "Size") || (str2 != "="))
    {
      MRSERR("realvec::operator>>: Problem3 reading realvec object from istream");
      return is;
    }
  is >> size;
  
  is >> str0;
  is >> str1;
  is >> str2;
  
  if ((str0 != "#") || (str1 != "MARSYAS") || (str2 != "mrs_realvec"))
    {
      MRSERR("realvec::operator>>: Problem4 reading realvec object from istream");

      return is;
    }
  return is;
}

// Jen
realvec
realvec::operator()(std::string r, std::string c)
{
  mrs_natural r_l = r.length();
  mrs_natural c_l = c.length();
  
  mrs_natural r_c = r.find(":");
  mrs_natural c_c = c.find(":");
  
  mrs_natural r_a;
  mrs_natural r_b;
  
  mrs_natural c_a;
  mrs_natural c_b;
  
  char *endptr;
  
  MRSASSERT( (r_c == 0 && r_l == 1) || (r_c == string::npos) || (r_c>0 && r_l-r_c>1) );
  MRSASSERT( (c_c == 0 && c_l == 1) || (c_c == string::npos) || (c_c>0 && c_l-c_c>1) );
  
  if( r_c != string::npos && r_l > 1 )
    {
      r_a = (mrs_natural)strtol( r.substr(0,r_c).c_str() , &endptr , 10  );
      MRSASSERT( *endptr == '\0' );
      r_b = (mrs_natural)strtol( r.substr(r_c+1,r_l-r_c-1).c_str() , &endptr , 10  );
      MRSASSERT( *endptr == '\0' );
    }
  else if( r_c == string::npos )
    {
      r_a = r_b = (mrs_natural)strtol( r.c_str() , &endptr , 10 );
      MRSASSERT( *endptr == '\0' );
    }
  else
    {
      r_a = 0;
      r_b = rows_-1;
    }
  
  MRSASSERT( r_a >= 0 && r_b < rows_ );
  
  if( c_c != string::npos && c_l > 1 )
    {
      c_a = (mrs_natural)strtol( c.substr(0,c_c).c_str() , &endptr , 10  );
      MRSASSERT( *endptr == '\0' );
      c_b = (mrs_natural)strtol( c.substr(c_c+1,c_l-c_c-1).c_str() , &endptr , 10 );      
      MRSASSERT( *endptr == '\0' );
    }
  else if( c_c == string::npos )
    {
      c_a = c_b = (mrs_natural)strtol( c.c_str() , &endptr , 10 );
      MRSASSERT( *endptr == '\0' );
    }
  else
    {
      c_a = 0;
      c_b = cols_-1;
    }
  
  MRSASSERT( c_a >= 0 && c_b < cols_ );
  
  r_l = r_b - r_a + 1;
  c_l = c_b - c_a + 1;
  
  realvec matrix;
  
  matrix.create( r_l , c_l );
  
  for( c_c = c_a ; c_c <= c_b ; c_c++ )
    {
      for( r_c = r_a ; r_c <= r_b ; r_c++ )
	{
	  matrix.data_[(c_c-c_a) * r_l + (r_c-r_a)] = data_[c_c * rows_ + r_c];
	}
    }       
  
  return matrix;
}

realvec
realvec::operator()(std::string c)
{
  mrs_natural c_l = c.length();

  mrs_natural c_c = c.find(":");

  mrs_natural c_a;
  mrs_natural c_b;

  char *endptr;

  MRSASSERT( (c_c == 0 && c_l == 1) || (c_c == string::npos) || (c_c>0 && c_l-c_c>1) );

  if( c_c != string::npos && c_l > 1 )
    {
      c_a = (mrs_natural)strtol( c.substr(0,c_c).c_str() , &endptr , 10  );
      MRSASSERT( *endptr == '\0' );
      c_b = (mrs_natural)strtol( c.substr(c_c+1,c_l-c_c-1).c_str() , &endptr , 10  );      
      MRSASSERT( *endptr == '\0' );
    }
  else if( c_c == string::npos )
    {
      c_a = c_b = (mrs_natural)strtol( c.c_str() , &endptr , 10 );
      MRSASSERT( *endptr == '\0' );
    }
  else
    {
      c_a = 0;
      c_b = (rows_*cols_)-1;
    }

  MRSASSERT( c_a >= 0 && c_b < rows_*cols_ );   

  c_l = c_b - c_a + 1;

  realvec matrix;

  matrix.create( c_l );

  for( c_c = c_a ; c_c <= c_b ; c_c++ )
    {
      matrix.data_[(c_c-c_a)] = data_[c_c];
    }

  return matrix;
}
// Jen end

realvec
realvec::getRow(const mrs_natural r) const
{
  realvec rowvec(cols_);

  if(r >= rows_ )
    {
      MRSERR("realvec::getRow() - row index greater than realvec number of rows! Returning uninitialized row vector.");
      return rowvec;
    }

  for (mrs_natural c=0; c < cols_; c++)
    {
      rowvec(c) = (*this)(r,c);
    }

  return rowvec;
}

realvec
realvec::getCol(const mrs_natural c) const
{
  realvec colvec(rows_,1);

  if(c >= cols_)
    {
      MRSERR("realvec::getCol() - row index greater than realvec number of rows! Returning uninitialized column vector.");
      return colvec;
    }

  for (mrs_natural r=0; r < rows_; r++)
    {
      colvec(r) = (*this)(r,c);
    }

  return colvec;
}

mrs_real 
realvec::maxval(mrs_natural* index) const
{
	mrs_real max = numeric_limits<mrs_real>::max() * -1.0;
	mrs_natural ind = 0;
	for (mrs_natural i=0; i < size_; i++)
	{
		if(data_[i] > max)
		{
			max = data_[i];
			ind = i;
		}
	}
	if(index)
		*index = ind;
	return max;
}

mrs_real 
realvec::minval() const
{
  mrs_real min = numeric_limits<mrs_real>::max();
  for (mrs_natural i=0; i < size_; i++)
    {
      if(data_[i] < min)
	min = data_[i];
    }
  return min;
}

realvec
realvec::meanObs() const
{
  realvec obsrow(cols_); //row vector
  realvec meanobs(rows_, 1); //column vector

  for (mrs_natural r=0; r<rows_; r++)
    {
      //for (mrs_natural c=0; c<cols_; c++)
      //{
      //	obsrow(c) = (*this)(r,c); //get observation row
      //}
      obsrow = getRow(r);
      meanobs(r,0) = obsrow.mean();
    }

  return meanobs;	
}

realvec
realvec::varObs() const
{
  realvec obsrow(cols_); //row vector
  realvec varobs(rows_, 1); //column vector

  for (mrs_natural r=0; r<rows_; r++)
    {
      //for (mrs_natural c=0; c<cols_; c++)
      //{
      //	obsrow(c) = (*this)(r,c); //get observation row
      //}
      obsrow = getRow(r);
      varobs(r,0) = obsrow.var();
    }

  return varobs;
}

realvec
realvec::stdObs() const
{
  realvec obsrow(cols_); //row vector
  realvec stdobs(rows_, 1); //column vector

  for (mrs_natural r=0; r<rows_; r++)
    {
      //for (mrs_natural c=0; c < cols_; c++)
      //{
      //	obsrow(c) = (*this)(r,c); //get observation row
      //}
      obsrow = getRow(r);
      stdobs(r,0) = obsrow.std();
    }

  return stdobs;	
}

void
realvec::normObs()
{
  //normalize (aka standardize) matrix
  //using observations means and standard deviations 

  realvec obsrow(cols_);

  for (mrs_natural r=0; r < rows_; r++)
    {
      //for (mrs_natural c=0; c < cols_; c++)
      //{
      //	obsrow(c) = (*this)(r,c); //get observation row
      //}
      obsrow = getRow(r);
      for (mrs_natural c=0; c < cols_; c++)
	{
	  (*this)(r,c) -= obsrow.mean();
	  (*this)(r,c) /= obsrow.std();
	}
    }
}

void
realvec::normObsMinMax()
{
  //normalize (aka standardize) matrix
  //using observations means and standard deviations 

  realvec obsrow(cols_);

  for (mrs_natural r=0; r < rows_; r++)
    {
      //for (mrs_natural c=0; c < cols_; c++)
      //{
      //	obsrow(c) = (*this)(r,c); //get observation row
      //}
      obsrow = getRow(r);
      for (mrs_natural c=0; c < cols_; c++)
	{
	  (*this)(r,c) -= obsrow.minval();
	  (*this)(r,c) /= obsrow.maxval();
	}
    }
}


void
realvec::normSpl(mrs_natural index)
{
	//normalize (aka standardize) matrix
	//using observations means and standard deviations 
if(!index)
index=cols_;
	for (mrs_natural r=0; r < index; r++)
	{
		//for (mrs_natural c=0; c < cols_; c++)
		//{
		//	obsrow(c) = (*this)(r,c); //get observation row
		//}
		mrs_real mean = getCol(r).mean();
		mrs_real std = getCol(r).std();
		if(std)
			for (mrs_natural c=0; c < rows_; c++)
			{
				(*this)(c, r) -= mean;
				(*this)(c, r) /= std;
			}
	}
}

void
realvec::normSplMinMax(mrs_natural index)
{
	//normalize (aka standardize) matrix
	//using observations means and standard deviations 
if(!index)
index=cols_;
	for (mrs_natural r=0; r < index; r++)
	{
		//for (mrs_natural c=0; c < cols_; c++)
		//{
		//	obsrow(c) = (*this)(r,c); //get observation row
		//}
		mrs_real min = getCol(r).minval();
		mrs_real max = getCol(r).maxval();
		if(max)
			for (mrs_natural c=0; c < rows_; c++)
			{
				(*this)(c, r) -= min;
				(*this)(c, r) /= max;
			}
	}
}


realvec
realvec::correlation() const
{
  //correlation as computed in Marsyas0.1
  //computes the correlation between observations
  //Assumes data points (i.e. examples) in columns and features (i.e. observations) in rows (as usual in Marsyas0.2).

  realvec corr(rows_, rows_);//correlation matrix
	
  if(size_ == 0)
    {
      MRSERR("realvec::correlation() : empty input matrix! returning empty and invalid correlation matrix!")
	return corr;
    }

  realvec temp = (*this);

  // normalize observations (i.e subtract obs. mean, divide by obs. standard dev)
  temp.normObs(); 

  mrs_real sum = 0.0;
  for (mrs_natural r1=0; r1< rows_; r1++)
    {
      for (mrs_natural r2=0; r2 < rows_; r2++)
	{
	  sum = 0.0;

	  for (mrs_natural c=0; c < cols_; c++)
	    sum += (temp(r1,c) * temp(r2,c));

	  sum /= cols_;
	  corr(r1,r2) = sum;
	}
    }
  return corr;
}

realvec 
realvec::covariance() const
{
  //computes the (unbiased estimate) covariance between observations (as in MATLAB cov()).
  //Assumes data points (i.e. examples) in columns and features (i.e. observations) in rows (as usual in Marsyas0.2).
  //This method assumes non-standardized data (typical covariance calculation).

  realvec covar(rows_, rows_); //covariance matrix

  if(size_ == 0)
    {
      MRSERR("realvec::covariance2() : empty input matrix! returning empty and invalid covariance matrix!")
	return covar;
    }

  //check if there are sufficient data points for a good covariance estimation...
  if(cols_ < (rows_ + 1))
    MRSWARN("realvec::covariance() : nr. data points < nr. observations + 1 => covariance matrix is SINGULAR!");
  if( (mrs_real)cols_ < ((mrs_real)rows_*(mrs_real)(rows_-1.0)/2.0))
    MRSWARN("realvec::covariance() : too few data points => ill-calculation of covariance matrix!");

  realvec meanobs = meanObs();//observation means

  mrs_real sum = 0.0;
  for (mrs_natural r1=0; r1< rows_; r1++)
    {
      for (mrs_natural r2=0; r2 < rows_; r2++)
	{
	  sum = 0.0;
			
	  for (mrs_natural c=0; c < cols_; c++)
	    sum += ((data_[c * rows_ + r1] - meanobs(r1)) * (data_[c * rows_ + r2]- meanobs(r2)));
			
	  if(cols_ > 1)
	    sum /= (cols_ - 1);//unbiased covariance estimate
	  else
	    sum /= cols_; //biased covariance estimate

	  covar(r1,r2) = sum;
	}
    }

  return covar;
}

realvec 
realvec::covariance2() const
{
  //covariance matrix as computed in Marsyas0.1
  //computes the covariance between observations.
  //Assumes data points (i.e. examples) in columns and features (i.e. observations) in rows (as usual in Marsyas0.2).
  //This calculation assumes standardized data at its input...
  
  realvec covar(rows_, rows_); //covariance matrix
  
  if(size_ == 0)
    {
      MRSERR("realvec::covariance() : empty input matrix! returning empty and invalid covariance matrix!")
	return covar;
    }

  //check if there are sufficient data points for a good covariance estimation...
  if(cols_ < (rows_ + 1))
    MRSWARN("realvec::covariance() : nr. data points < nr. observations + 1 => covariance matrix is SINGULAR!");
  if( (mrs_real)cols_ < ((mrs_real)rows_*(mrs_real)(rows_-1.0)/2.0))
    MRSWARN("realvec::covariance() : too few data points => ill-calculation of covariance matrix!");

  for (mrs_natural r1=0; r1< rows_; r1++)
    {
      for (mrs_natural r2=0; r2 < rows_; r2++)
	{
	  mrs_real sum = 0.0;

	  for (mrs_natural c=0; c < cols_; c++)
	    sum += (data_[c * rows_ + r1] * data_[c * rows_ + r2]);

	  sum /= cols_;
	  covar(r1,r2) = sum;
	}
    }

  return covar;
}

mrs_real
realvec::trace() const
{
  if (cols_ != rows_)
    {
      MRSWARN("realvec::trace() - matrix is not square!");
    }

  mrs_real res = 0.0;
  for(mrs_natural i = 0; i < size_; )
    {
      res += data_[i];
      i += cols_+1;
    }

  return res;
}

mrs_real
realvec::det() const
{
  NumericLib numlib;
  return numlib.determinant(*this);
}

mrs_real
realvec::divergenceShape(realvec Ci, realvec Cj)
{
  ///matrices should be square and equal sized
  if(Ci.getCols() != Cj.getCols() && Ci.getRows() != Cj.getRows() &&
     Ci.getCols()!= Ci.getRows())
    {
      MRSERR("realvec::divergenceShape() : input matrices should be square and equal sized. Returning invalid value...");
      return -1.0;
    }

  mrs_real res;

  realvec Citemp(Ci.getRows(), Ci.getCols());
  realvec invCi(Ci);

  realvec Cjtemp(Cj.getRows(),Cj.getCols());
  realvec invCj(Cj);

#ifdef _MATLAB_REALVEC_
  MATLAB_PUT(Ci, "Ci");
  MATLAB_PUT(Cj, "Cj");
  MATLAB_PUT(Citemp, "Citemp");
  MATLAB_PUT(Cjtemp, "Cjtemp");
#endif

	
  invCi.invert(Citemp); 
  invCj.invert(Cjtemp);

#ifdef _MATLAB_REALVEC_
  MATLAB_PUT(Citemp, "Citemp2");
  MATLAB_PUT(Cjtemp, "Cjtemp2");
  MATLAB_PUT(invCi, "invCi");
  MATLAB_PUT(invCj, "invCj");
#endif

  Cj *= (-1.0);
  Ci += Cj;

#ifdef _MATLAB_REALVEC_
  MATLAB_PUT(Ci, "Ci_minus_Cj");
#endif

  invCi *= (-1.0);
  invCj += invCi;

#ifdef _MATLAB_REALVEC_
  MATLAB_PUT(invCj, "invCj_minus_invCi");
#endif

  Ci *= invCj;

  res = Ci.trace() / 2.0;

#ifdef _MATLAB_REALVEC_
  MATLAB_PUT(Ci, "divergenceMatrix");
  MATLAB_PUT(res, "divergence");
#endif

  return res;
}

mrs_real
realvec::bhattacharyyaShape(realvec Ci, realvec Cj)
{
  ///matrices should be square and equal sized
  if(Ci.getCols() != Cj.getCols() && Ci.getRows() != Cj.getRows() &&
     Ci.getCols()!= Ci.getRows())
    {
      MRSERR("realvec::bhattacharyyaShape() : input matrices should be square and equal sized. Returning invalid value...");
      return -1.0;
    }
  
  //denominator
  mrs_real sqrtdetCi = sqrt(Ci.det());
  mrs_real sqrtdetCj = sqrt(Cj.det());
  mrs_real den = sqrtdetCi * sqrtdetCj;
#ifdef _MATLAB_REALVEC_
  MATLAB_PUT(Ci, "Ci");
  MATLAB_PUT(Cj, "Cj");
  MATLAB_PUT(sqrtdetCi, "sqrtdetCi");
  MATLAB_PUT(sqrtdetCj, "sqrtdetCj");
  MATLAB_PUT(den, "den");
#endif
    
  //numerator
  Ci += Cj;
  Ci /= 2.0;
  mrs_real num = Ci.det();
  
  //bhattacharyyaShape
  return log(num/den);
}



