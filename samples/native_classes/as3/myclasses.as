package avmplus 
{
	[native(cls="MyClass", instance="MyClassInstance", methods="auto")]
	public class MyClass
	{
		var test = 0;

		/* A class method, defined in ActionScript 3 */
		public function as3_function():void
		{
			//trace("called as3_function()");
		}

		/* A static class function, defined in ActionScript 3 */
		public static function static_as3_function(a:int, b:int):int
		{
			return a + b;
		}

		/* A class method, defined in C++ */
		public native function native_function(str:String):int;

		/* A static class function, defined in C++ */
		public native static function static_native_function():String;
	}
}

package test
{
	public function hihi()
	{
		trace("hihi");
	}
}
