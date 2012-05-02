#ifndef PT_H
#define PT_H

struct pt
	{
	double x;
	double y;
	bool operator==( const pt & rhs )const{return (x==rhs.x && y == rhs.y); }
	bool operator!=( const pt & rhs )const{return !(*this==rhs); }
	};
#endif
