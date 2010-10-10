
package GlobalFunctions
{
	public function function0():void
	{
		trace("GlobalFunctions.function0()");
	}

	public function function1(arg0:String):String
	{
		trace("GlobalFunctions.function1(" + arg0 + ")");
		return arg0;
	}
}

package ClassFunctions
{
	public class Class
	{
		public function Class(arg0:String, arg1:String)
		{
			trace("ClassFunctions.Class.Constructor(" + arg0 + ", " + arg1 + ")");
		}

		public static function static_function0():void
		{
			trace("ClassFunctions.Class.static_function0()");
		}

		public static function static_function1(arg0:String):String
		{
			trace("ClassFunctions.Class.static_function1(" + arg0 + ")");
			return arg0;
		}

		public function member_function0():void
		{
			trace("ClassFunctions.Class.member_function0()");
		}

		public function member_function1(arg0:String):String
		{
			trace("ClassFunctions.Class.member_function1(" + arg0 + ")");
			return arg0;
		}
	}
}
