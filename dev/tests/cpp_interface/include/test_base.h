#ifndef __test_base_H__
#define __test_base_H__

class test_base
{
public:
	virtual const char* name() = 0;
	virtual void test_global_functions(csp::VmCore* core) = 0;
	virtual void test_static_class_functions(csp::VmCore* core) = 0;
	virtual avmplus::ScriptObject* test_class_construction(csp::VmCore* core) = 0;
	virtual void test_class_member_functions(csp::VmCore* core, avmplus::ScriptObject* obj) = 0;
};

#endif
