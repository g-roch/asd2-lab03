/* 
 * File:   Util.h
 * Author: Olivier Cuisenaire
 *
 * Created on 26. septembre 2014, 15:26
 */

#ifndef UTIL_H
#define	UTIL_H

#include <vector>
#include <string>

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

#endif	/* UTIL_H */

