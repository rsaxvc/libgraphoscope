#ifndef PT_H
#define PT_H

struct pt
	{
	float x;
	float y;
	bool operator==( const pt & rhs )const{return (x==rhs.x && y == rhs.y); }
	bool operator!=( const pt & rhs )const{return !(*this==rhs); }
	};
#endif
