#include "auto_bind.h"

#include "auto_classes.h"

namespace flash { namespace geom {

	as3_class(ColorTransform, avmplus::ScriptObject, flash.geom)
	//{
		as3_code
		(
			public function ColorTransform(
				redMultiplier:Number = 1.0, 
				greenMultiplier:Number = 1.0, 
				blueMultiplier:Number = 1.0, 
				alphaMultiplier:Number = 1.0, 
				redOffset:Number = 0, 
				greenOffset:Number = 0, 
				blueOffset:Number = 0, 
				alphaOffset:Number = 0)
			{
				var test:Number = 0;
				redMultiplier = test;
			}

			/* attributes */
			public var alphaMultiplier:Number;
			public var alphaOffset:Number;
			public var blueMultiplier:Number;
			public var blueOffset:Number;
			public var greenMultiplier:Number;
			public var greenOffset:Number;
			public var redMultiplier:Number;
			public var redOffset:Number;
		);

		as3_code
		(
			public function test()
			{
				trace("lolo")
			}
		);

		as3_get(public, none, int, color);
		as3_set(public, none, int, color);

		as3_method_1(public, none, void, concat, ColorTransform*);
		as3_method_0(public, none, avmplus::Stringp, customToString);

	private:
		int mColor;
	//}
	as3_class_static(ColorTransform)
	//{
		as3_method_0(public, none, void, test);
	//}
	as3_class_end
}}
