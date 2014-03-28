// g++ -std=c++11 -O2 t.cpp

#include <iostream>
#include <type_traits>
#include <memory>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <unordered_map>
#include <new>
#include <fstream>

struct A;

typedef long int NUMBR;
typedef double NUMBAR;
typedef bool TROOF;
typedef std::string YARN;
typedef std::unordered_map<A, A, A> BUKKIT;
typedef std::shared_ptr<BUKKIT> BUKKIT_PTR;
typedef std::fstream FILEZ;
typedef std::shared_ptr<FILEZ> FILEZ_PTR;

struct A {
	public:
		enum class T : std::int8_t {
			NUMBR, NUMBAR, TROOF, YARN, BUKKIT, FILEZ, NOOB
		};
		
		T type;
		union {
			NUMBR numbr;
			NUMBAR numbar;
			TROOF troof;
			YARN yarn;
			BUKKIT_PTR bukkit;
			FILEZ_PTR filez;
		}; 
		
		A() : type( T::NOOB ) {}
		A(const A &a) : type(a.type) {
			switch( type ) {
				case T::NUMBR:
					numbr = a.numbr;
					break;
				case T::NUMBAR:
					numbar = a.numbar;
					break;
				case T::TROOF:
					troof = a.troof;
					break;
				case T::YARN:
					yarn = a.yarn;
					break;
				case T::BUKKIT:
					bukkit = a.bukkit;
				case T::FILEZ:
					filez = a.filez;
					break;
			}
		}
		~A() {
			switch( type ) {
				case T::BUKKIT:
					bukkit.reset();
					break;
				case T::FILEZ:
					filez.reset();
					break;
			}
		}
		
		A &operator=(const A &a) {
			type = a.type;
			switch( type ) {
				case T::NUMBR:
					numbr = a.numbr;
					break;
				case T::NUMBAR:
					numbar = a.numbar;
					break;
				case T::TROOF:
					troof = a.troof;
					break;
				case T::YARN:
					yarn = a.yarn;
					break;
				case T::BUKKIT:
					bukkit = a.bukkit;
				case T::FILEZ:
					filez = a.filez;
					break;
			}
			return *this;
		}
		bool operator==(const A &a) const {
			if( type != a.type ) {
				return false;
			}
			
			switch( type ) {
				case T::NUMBR:
					return numbr == a.numbr;
				case T::NUMBAR:
					return numbar == a.numbar;
				case T::TROOF:
					return troof == a.troof;
				case T::YARN:
					return yarn == a.yarn;
				case T::BUKKIT:
					return bukkit == a.bukkit;
				case T::FILEZ:
					return filez == a.filez;
				case T::NOOB:
					return true;
			}
		}
		std::size_t operator()(const A &a) const {
			switch( a.type ) {
				case T::NUMBR:
					return std::hash<NUMBR>()(a.numbr);
				case T::NUMBAR:
					return std::hash<NUMBAR>()(a.numbar);
				case T::TROOF:
					return std::hash<TROOF>()(a.troof);
				case T::YARN:
					return std::hash<YARN>()(a.yarn);
				case T::BUKKIT:
					return std::hash<BUKKIT_PTR>()(a.bukkit);
				case T::FILEZ:
					return std::hash<FILEZ_PTR>()(a.filez);
			}
			return std::hash<NUMBR>()(0);
		}
		
		friend std::ostream &operator <<( std::ostream &out, const A &a ) {
			switch( a.type ) {
				case A::T::NUMBR:
					out << "NUMBR: " << a.numbr;
					break;
				case A::T::NUMBAR:
					out << "NUMBAR: " << a.numbar;
					break;
				case A::T::TROOF:
					out << "TROOF: " << a.troof;
					break;
				case A::T::YARN:
					out << "YARN: " << a.yarn;
					break;
				case A::T::BUKKIT:
					out << "BUKKIT: " << a.bukkit;
					break;
				case A::T::FILEZ:
					out << "FILEZ: " << a.filez;
					break;
				case A::T::NOOB:
					out << "NOOB";
			}
			return out;
		}
};

template<class T>
inline typename std::enable_if<std::is_same<T, NUMBR>::value, T>::type
GetData(const A &var) {
	switch( var.type ) {
		case A::T::NUMBR:
			return var.numbr;
		case A::T::NUMBAR:
			return (T)(var.numbar);
		case A::T::TROOF:
			return (T)(var.troof);
	}
}

template<class T>
inline typename std::enable_if<std::is_same<T, NUMBAR>::value, T>::type
GetData(const A &var) {
	switch( var.type ) {
		case A::T::NUMBR:
			return (T)(var.numbr);
		case A::T::NUMBAR:
			return var.numbar;
		case A::T::TROOF:
			return (T)(var.troof);
	}
}

template<class T>
inline typename std::enable_if<std::is_same<T, TROOF>::value, T>::type
GetData(const A &var) {
	switch( var.type ) {
		case A::T::NUMBR:
			return (T)(var.numbr);
		case A::T::NUMBAR:
			return (T)var.numbar;
		case A::T::TROOF:
			return var.troof;
	}
}

inline void GetNumericDataFromYarn( const YARN *yarn, A::T &type, 
									NUMBR *numbr, NUMBAR *numbar ) {
	char *end;
	const char *str = yarn->c_str();
	*numbr = std::strtol(str, &end, 10);
	if (*end == '\0') {
		type = A::T::NUMBR;
		return;
	}
	*numbar = std::strtod(str, &end);
	if (*end == '\0') {
		type = A::T::NUMBAR;
		return;
	}
	type = A::T::NOOB;
}

inline A sum( const A &lhs, const A &rhs ) {
	A ret;
	
	A::T lhs_type = lhs.type;
	A::T rhs_type = rhs.type;
	bool lhs_from_yarn = (lhs_type == A::T::YARN);
	bool rhs_from_yarn = (rhs_type == A::T::YARN);	
	
	NUMBR lhs_numbr, rhs_numbr;
	NUMBAR lhs_numbar, rhs_numbar;
		
	if( lhs_from_yarn || rhs_from_yarn ) {
		if( lhs_from_yarn ) {
			GetNumericDataFromYarn( &lhs.yarn, lhs_type, &lhs_numbr, &lhs_numbar );
		}
		if( rhs_from_yarn ) {
			GetNumericDataFromYarn( &rhs.yarn, rhs_type, &rhs_numbr, &rhs_numbar );
		}
		
		if( lhs_from_yarn && lhs_type == A::T::NUMBR && rhs_type == A::T::NUMBAR) {
			lhs_numbar = (NUMBAR)lhs_numbr;
		}
		if( rhs_from_yarn && rhs_type == A::T::NUMBR && lhs_type == A::T::NUMBAR) {
			rhs_numbar = (NUMBAR)rhs_numbr;
		}
	}
	
	if( lhs_type == A::T::NUMBAR || rhs_type == A::T::NUMBAR ) {
		ret.numbar = (lhs_from_yarn ? lhs_numbar : GetData<NUMBAR>(lhs)) + (rhs_from_yarn ? rhs_numbar : GetData<NUMBAR>(rhs));
		ret.type = A::T::NUMBAR;
		return ret;
	}

	ret.numbr = (lhs_from_yarn ? lhs_numbr : GetData<NUMBR>(lhs)) + (rhs_from_yarn ? rhs_numbr : GetData<NUMBR>(rhs));
	ret.type = A::T::NUMBR;
	return ret;
}

inline A mult( const A &lhs, const A &rhs ) {
	A ret;
	
	A::T lhs_type = lhs.type;
	A::T rhs_type = rhs.type;
	bool lhs_from_yarn = (lhs_type == A::T::YARN);
	bool rhs_from_yarn = (rhs_type == A::T::YARN);	
	
	NUMBR lhs_numbr, rhs_numbr;
	NUMBAR lhs_numbar, rhs_numbar;
		
	if( lhs_from_yarn || rhs_from_yarn ) {
		if( lhs_from_yarn ) {
			GetNumericDataFromYarn( &lhs.yarn, lhs_type, &lhs_numbr, &lhs_numbar );
		}
		if( rhs_from_yarn ) {
			GetNumericDataFromYarn( &rhs.yarn, rhs_type, &rhs_numbr, &rhs_numbar );
		}
		
		if( lhs_from_yarn && lhs_type == A::T::NUMBR && rhs_type == A::T::NUMBAR) {
			lhs_numbar = (NUMBAR)lhs_numbr;
		}
		if( rhs_from_yarn && rhs_type == A::T::NUMBR && lhs_type == A::T::NUMBAR) {
			rhs_numbar = (NUMBAR)rhs_numbr;
		}
	}
	
	if( lhs_type == A::T::NUMBAR || rhs_type == A::T::NUMBAR ) {
		ret.numbar = (lhs_from_yarn ? lhs_numbar : GetData<NUMBAR>(lhs)) * (rhs_from_yarn ? rhs_numbar : GetData<NUMBAR>(rhs));
		ret.type = A::T::NUMBAR;
		return ret;
	}

	ret.numbr = (lhs_from_yarn ? lhs_numbr : GetData<NUMBR>(lhs)) * (rhs_from_yarn ? rhs_numbr : GetData<NUMBR>(rhs));
	ret.type = A::T::NUMBR;
	return ret;
}

#if 0
inline A sum( const A &lhs, const A &rhs ) {
	A ret;
	
	A::T lhs_type = lhs.type;
	A::T rhs_type = rhs.type;
	
	if( lhs_type == A::T::YARN || rhs_type == A::T::YARN ) {
		NUMBR lhs_numbr, rhs_numbr;
		NUMBAR lhs_numbar, rhs_numbar;
		bool lhs_from_yarn, rhs_from_yarn;
		
		lhs_from_yarn = (lhs_type == A::T::YARN);
		rhs_from_yarn = (rhs_type == A::T::YARN);
		
		if( lhs_from_yarn ) {
			GetNumericDataFromYarn( &lhs.yarn, lhs_type, &lhs_numbr, &lhs_numbar );
		}
		if( rhs_from_yarn ) {
			GetNumericDataFromYarn( &rhs.yarn, rhs_type, &rhs_numbr, &rhs_numbar );
		}
		
		if( lhs_from_yarn && lhs_type == A::T::NUMBR && rhs_type == A::T::NUMBAR) {
			lhs_numbar = (NUMBAR)lhs_numbr;
		}
		if( rhs_from_yarn && rhs_type == A::T::NUMBR && lhs_type == A::T::NUMBAR) {
			rhs_numbar = (NUMBAR)rhs_numbr;
		}
		
		if( lhs_type == A::T::NUMBAR || rhs_type == A::T::NUMBAR ) {
			ret.numbar = (lhs_from_yarn ? lhs_numbar : GetData<NUMBAR>(lhs)) + (rhs_from_yarn ? rhs_numbar : GetData<NUMBAR>(rhs));
			ret.type = A::T::NUMBAR;
			return ret;
		}

		ret.numbr = (lhs_from_yarn ? lhs_numbr : GetData<NUMBR>(lhs)) + (rhs_from_yarn ? rhs_numbr : GetData<NUMBR>(rhs));
		ret.type = A::T::NUMBR;
		return ret;
	}
	
	if( lhs_type == A::T::NUMBAR || rhs_type == A::T::NUMBAR ) {
		ret.numbar = GetData<NUMBAR>(lhs) + GetData<NUMBAR>(rhs);
		ret.type = A::T::NUMBAR;
		return ret;
	}
	
	ret.numbr = GetData<NUMBR>(lhs) + GetData<NUMBR>(rhs);
	ret.type = A::T::NUMBR;
	return ret;
}

inline A mult( const A &lhs, const A &rhs ) {
	A ret;
	
	A::T lhs_type = lhs.type;
	A::T rhs_type = rhs.type;
	
	if( lhs_type == A::T::YARN || rhs_type == A::T::YARN ) {
		NUMBR lhs_numbr, rhs_numbr;
		NUMBAR lhs_numbar, rhs_numbar;
		bool lhs_from_yarn, rhs_from_yarn;
		
		lhs_from_yarn = (lhs_type == A::T::YARN);
		rhs_from_yarn = (rhs_type == A::T::YARN);
		
		if( lhs_from_yarn ) {
			GetNumericDataFromYarn( &lhs.yarn, lhs_type, &lhs_numbr, &lhs_numbar );
		}
		if( rhs_from_yarn ) {
			GetNumericDataFromYarn( &rhs.yarn, rhs_type, &rhs_numbr, &rhs_numbar );
		}
		
		if( lhs_from_yarn && rhs_from_yarn ) {
			if( lhs_type == A::T::NUMBAR ) {
				if( rhs_type == A::T::NUMBR ) {
					rhs_numbar = (NUMBAR)rhs_numbr;
				}
			} else {
				if( rhs_type == A::T::NUMBAR ) {
					lhs_numbar = (NUMBAR)lhs_numbr;
				}
			}
		}
		
		if( lhs_type == A::T::NUMBAR || rhs_type == A::T::NUMBAR ) {
			ret.numbar = (lhs_from_yarn ? lhs_numbar : GetData<NUMBAR>(lhs)) * (rhs_from_yarn ? rhs_numbar : GetData<NUMBAR>(rhs));
			ret.type = A::T::NUMBAR;
			return ret;
		}

		ret.numbr = (lhs_from_yarn ? lhs_numbr : GetData<NUMBR>(lhs)) * (rhs_from_yarn ? rhs_numbr : GetData<NUMBR>(rhs));
		ret.type = A::T::NUMBR;
		return ret;
	}
	
	if( lhs_type == A::T::NUMBAR || rhs_type == A::T::NUMBAR ) {
		ret.numbar = GetData<NUMBAR>(lhs) * GetData<NUMBAR>(rhs);
		ret.type = A::T::NUMBAR;
		return ret;
	}
	
	ret.numbr = GetData<NUMBR>(lhs) * GetData<NUMBR>(rhs);
	ret.type = A::T::NUMBR;
	return ret;
}
#endif

NUMBAR a() {
	A a, b, c;
	a.numbar = 4.1;
	a.type = A::T::NUMBAR;
	b.numbr = 5;
	b.type = A::T::NUMBR;
	c.numbr = 3;
	c.type = A::T::NUMBR;
	return GetData<NUMBAR>( mult(a, sum(b, c) ) );
}

double b() {
	double a = 4.2;
	NUMBR b = 2;
	return a + b;
}

NUMBR c() {
	A a, b, c;
	a.type = A::T::NUMBR;
	a.numbr = 5;
	b.type = A::T::NUMBR;
	b.numbr = 2;
	c.type = A::T::NUMBR;
	c.numbr = 3;
	return GetData<NUMBR>( mult(a, sum(b, c)) );
}

NUMBR d() {
	NUMBR a = 4;
	NUMBR b = 2;
	NUMBR c = 3;
	return a + b + c;
}

bool e() {
	A e;
	e.type = A::T::NUMBAR;
	e.numbar = 42.0;
	return GetData<TROOF>( e );
}

NUMBR f(NUMBR x) {
	A f;
	f.type = A::T::NUMBR;
	f.numbr = x;
	return GetData<NUMBR>( sum( f, mult( f, f ) ) );
}

NUMBR f2(NUMBR x) {
	A f, g;
	f.type = A::T::NUMBR;
	f.numbr = x;
	g.type = A::T::NUMBR;
	g.numbr = 2;
	for(int i = 0; i < x; ++i ) {
		f = sum( f, mult( f, g ) );
	}
	return GetData<NUMBR>( f );
}

inline A g2(const A &x, const A &y) {
	return sum( mult( x, x ), mult( y, y ) );
}

inline A g(const A &x, const A &y) {
	return g2(mult(x, y), sum(x, y));
}

NUMBR h() {
	A x, y;
	x.type = A::T::NUMBR;
	x.numbr = 6;
	y.type = A::T::NUMBR;
	y.numbr = 4;
	return GetData<NUMBR>( g( x, y ) );
}

NUMBR m() {
	A x, z;
	x.type = A::T::NUMBR;
	x.numbr = 20;
	x.type = A::T::YARN;
	new (&x.yarn) std::string("10");
	A y = x;
	z.type = A::T::NUMBR;
	z.numbr = 2;
	return GetData<NUMBR>( sum( z, mult( x, y ) ) );
}

NUMBAR n() {
	A x, y, z;
	x.type = A::T::YARN;
	new (&x.yarn) std::string("10");
	y.type = A::T::YARN;
	new (&y.yarn) std::string("10.1");
	z.type = A::T::NUMBR;
	z.numbr = 2;
	return GetData<NUMBAR>( sum( x, mult( z, y ) ) );
}

NUMBR n2() {
	A x, y, z;
	x.type = A::T::NUMBAR;
	x.numbar = 10.1;
	y.type = A::T::NUMBR;
	y.numbr = 10;
	z.type = A::T::NUMBR;
	z.numbr = 2;
	// Note cast to NUMBR from NUMBAR
	return GetData<NUMBR>( sum( z, mult( x, y ) ) );
}

NUMBAR n3() {
	A x, y, z;
	x.type = A::T::YARN;
	new (&x.yarn) std::string("10");
	y.type = A::T::YARN;
	new (&y.yarn) std::string("10.1");
	z.type = A::T::NUMBR;
	z.numbr = 2;
	return GetData<NUMBAR>( sum( z, mult( x, y ) ) );
}

void p() {
	A b, bb, k, kp;
	b.type = A::T::BUKKIT;
	new (&b.bukkit) BUKKIT_PTR(new BUKKIT());
	bb.type = A::T::BUKKIT;
	new (&bb.bukkit) BUKKIT_PTR(new BUKKIT());
	k.type = A::T::NUMBR;
	k.numbr = 1;
	(*bb.bukkit)[k] = b;
	(*b.bukkit)[k] = k;
	std::cout << "at(k) = " << b.bukkit->at(k) << std::endl;
	kp = k;
	k.type = A::T::NUMBAR;
	k.numbar = 1.5;
	(*b.bukkit)[k] = k;
	std::cout << "at(k) = " << b.bukkit->at(k) << std::endl;
	std::cout << "at(kp) = " << b.bukkit->at(kp) << std::endl;
	std::cout << "at(kp)(kp) = " << bb.bukkit->at(kp).bukkit->at(kp) << std::endl;
}

void q() {
	A fin, in;
	fin.type = A::T::FILEZ;
	new (&fin.filez) FILEZ_PTR(new FILEZ("t.out", std::ios::out | std::ios::in));
	new (&in.yarn) std::string();
	//(*fin.filez) << "R";
	(*fin.filez) >> in.yarn;
	in.type = A::T::YARN;
	std::cout << "in.yarn = " << in.yarn << std::endl;
	//std::cout << "in.yarn = " << in.yarn << std::endl;
}

int main() {
	std::cout << "sizeof(A) = " << sizeof(A) << std::endl;
	std::cout << "a() = " << a() << std::endl;
	std::cout << "c() = " << c() << std::endl;
	std::cout << "e() = " << e() << std::endl;
	std::cout << "h() = " << h() << std::endl;
	std::cout << "f(5) = " << f(5) << std::endl;
	std::cout << "f2(5) = " << f2(5) << std::endl;
	std::cout << "m() = " << m() << std::endl;
	std::cout << "n() = " << n() << std::endl;
	std::cout << "n2() = " << n2() << std::endl;
	std::cout << "n3() = " << n3() << std::endl;
	p();
	q();
	return 0;
}