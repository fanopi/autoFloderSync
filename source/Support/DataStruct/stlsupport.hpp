/*
 * STLsupport.hpp
 *
 *  Created on: 2011-4-23
 *      Author: FanOpi
 */

#ifndef __YF_INCLUDE_SUPPORT_STLSUPPORT_HPP_
#define __YF_INCLUDE_SUPPORT_STLSUPPORT_HPP_

#include <new.h>

namespace YF{

template<typename OBJECTTYPE, typename INITVALUETYPE>
static	OBJECTTYPE* Construct(OBJECTTYPE* p_object_place, INITVALUETYPE object_init_value){
	new (p_object_place) OBJECTTYPE(object_init_value);
	return p_object_place;
}

template<typename OBJECTTYPE>
static void Destroy(OBJECTTYPE* p_object){
	p_object->~ObjectType();
	free(p_object, sizeof(OBJECTTYPE));
}

} /// namespace YF
#endif /* __YF_INCLUDE_SUPPORT_STLSUPPORT_HPP_ */
