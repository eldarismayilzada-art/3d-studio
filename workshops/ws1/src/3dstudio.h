/*
 * Workshop 1
 * Computer Graphics course
 * Dept of Computing Science, Umea University
 * Niclas Borlin, niclas.borlin@cs.umu.se
 *
 * Adapted from code by Stefan Johansson, stefanj@cs.umu.se
 */

#pragma once

// Common definitions and functions
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>

// Macro for index to buffer pointer conversion
#define BUFFER_OFFSET(i) (reinterpret_cast<const char*>(0 + (i)))

// Convert the argument to a string
#define STR(x) #x

// Compute the array size
#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))
typedef float Mat4x4[16];

// Define a 2d vector
struct Vec2 {
    float values[2];
public:
    Vec2(const float x, const float y) {
	this->values[0] = x;
	this->values[1] = y;
    }
    std::string toString(int dec) const {
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(dec)
	    << '('
	    << values[0] << ", "
	    << values[1]
	    << ')';
	return oss.str();
    }
};

// Define a 3d vector
struct Vec3 {
    float values[3];
public:
    Vec3(const float x, const float y, const float z) {
	this->values[0] = x;
	this->values[1] = y;
	this->values[2] = z;
    }
    Vec3(const Vec2& v, const float z) {
	this->values[0] = v.values[0];
	this->values[1] = v.values[1];
	this->values[2] = z;
    }
    // Return the string representation with a specified number of decimals
    std::string toString(int dec) const {
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(dec)
	    << '('
	    << values[0] << ", "
	    << values[1] << ", "
	    << values[2]
	    << ')';
	return oss.str();
    }
};

// Define a 4d vector
struct Vec4 {
    float values[4];
public:
    Vec4(const float x, const float y, const float z, const float w) {
	this->values[0] = x;
	this->values[1] = y;
	this->values[2] = z;
	this->values[3] = w;
    }
    Vec4(const Vec2& v, const float z, const float w) {
	this->values[0] = v.values[0];
	this->values[1] = v.values[1];
	this->values[2] = z;
	this->values[3] = w;
    }
    Vec4(const Vec3& v, const float w) {
	this->values[0] = v.values[0];
	this->values[1] = v.values[1];
	this->values[2] = v.values[2];
	this->values[3] = w;
    }
    // Return the string representation with a specified number of decimals
    std::string toString(int dec) const {
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(dec)
	    << '('
	    << values[0] << ", "
	    << values[1] << ", "
	    << values[2] << ", "
	    << values[3]
	    << ')';
	return oss.str();
    }
};

// Convert from decimal to hexadecimal, including the 0x prefix
template< typename T >
std::string int_to_hex( T i )
{
    std::stringstream stream;
    stream << "0x" << std::setfill('0') << std::setw(sizeof(T)*2) 
	   << std::hex << i;
    return stream.str();
}

// *************
// Debug macros

/*
 * Usage:
 * string s = "foo";
 * DEBUG( "Value of s is '" << s << "'");
 * -> DBG: Value of s is 'foo'
 */
#define DEBUG( msg ) \
    std::cerr << "DBG " << __FILE__ << "(" << __LINE__ << "): " << msg << std::endl

/*
 * Usage:
 * string s = "foo";
 * DEBUGVAR( s );
 * -> DBG: testfile.cpp(20) s = foo
 */
#define DEBUGVAR( var )\
    std::cerr << "DBG " << __FILE__ << "(" << __LINE__ << "): " \
    << #var << " = " << (var) << std::endl

// *************
