#include "MyClass.h"

#include <string>
#include <sstream>

using namespace avmplus;
#include "myclasses.cpp"

namespace avmplus
{
	//-----------------------------------------------------------------------
	MyClass::MyClass(avmplus::VTable* cvtable) 
		: ClassClosure(cvtable)
	{

	}
	//-----------------------------------------------------------------------
	MyClass::~MyClass()
	{

	}
	//-----------------------------------------------------------------------
	avmplus::ScriptObject* MyClass::createInstance(avmplus::VTable* ivtable, avmplus::ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) MyClassInstance(ivtable, prototype);
	}
	//-----------------------------------------------------------------------
	avmplus::Stringp MyClass::getTestString()
	{
		std::string filename = __FILE__;
		size_t pos = filename.find_last_of('\\');
		if(pos != std::string::npos)
		{
			++pos;
			filename = filename.substr(pos, filename.length()-pos);
		}

		std::stringstream str;
		str << filename << " compiled at " << __TIME__; 
		avmplus::Stringp text = core()->newStringUTF8(str.str().c_str());
		return text;
	}
	//-----------------------------------------------------------------------
	MyClassInstance::MyClassInstance(avmplus::VTable* vtable, avmplus::ScriptObject* prototype) 
		: ScriptObject(vtable, prototype)
	{

	}
	//-----------------------------------------------------------------------
	MyClassInstance::~MyClassInstance()
	{

	}
	//-----------------------------------------------------------------------
	int MyClassInstance::stringTest(avmplus::Stringp str)
	{
		std::stringstream sstr;

		avmplus::StringIndexer str_idx(str);
		for(int i=0, n=str_idx->length(); i<n; i++) 
		{
			sstr << (char)str_idx[n-i-1];
		}

		int temp;
		sstr >> temp;
		return temp;
	}
	//-----------------------------------------------------------------------
}
