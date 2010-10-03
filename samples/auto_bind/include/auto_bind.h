#ifndef __auto_bind_H__
#define __auto_bind_H__

#include "cspPrerequisites.h"

#define as3_class(clazz, base_class, as3_package) class clazz : public base_class { public: \
	clazz(avmplus::VTable* vtable, avmplus::ScriptObject* prototype) : \
	base_class(vtable, prototype) { } DECLARE_SLOTS_##clazz; public:

#define as3_class_static(clazz) }; class clazz##Class : public avmplus::ClassClosure { DECLARE_SLOTS_##clazz##Class; public: \
	clazz##Class(avmplus::VTable* cvtable) : \
	avmplus::ClassClosure(cvtable) \
	{ \
		AvmAssert(traits()->getSizeOfInstance() == sizeof(clazz##Class)); \
		createVanillaPrototype(); \
	} \
	avmplus::ScriptObject* createInstance(avmplus::VTable* ivtable, avmplus::ScriptObject* prototype) \
	{ \
		return new (core()->GetGC(), ivtable->getExtraSize()) clazz(ivtable, prototype); \
	}


#define as3_class_end };

#define none

#define as3_ctor(a)
#define as3_code(code)
#define as3_auto(clazz) clazz(avmplus::VTable* vtable, avmplus::ScriptObject* prototype);
#define as3_get(visibility, modifiers, type, name) public: modifiers type get_##name()
#define as3_set(visibility, modifiers, type, name) public: modifiers void set_##name(type)
#define as3_get_set(visibility, modifiers, type, name) public: modifiers void set_##name(type); modifiers void type get_##name()
#define as3_method_0(visibility, modifiers, return_type, name) public: modifiers return_type name()
#define as3_method_1(visibility, modifiers, return_type, name, arg0) public: modifiers return_type name(arg0)

#endif
