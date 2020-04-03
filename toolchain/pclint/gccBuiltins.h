/*
 * gccBuiltins.h
 *
 *  Created on: 02.03.2020
 *      Author: Florian Kaup
 */

#ifndef CEEDLING_PCLINT_GCCBUILTINS_H_
#define CEEDLING_PCLINT_GCCBUILTINS_H_

#include <stdint.h>

//lint -esym(526, __builtin_clz) This is a compiler builtin, it has not to be defined
int __builtin_clz(unsigned int x);

#endif /* CEEDLING_PCLINT_GCCBUILTINS_H_ */
