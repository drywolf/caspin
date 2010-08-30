package avmplus 
{
	[native(cls="MyClass", instance="MyClassInstance", methods="auto")]
	public class MyClass
	{
		public native function stringTest(par1:String):int;
		public native static function getTestString():String;
	}
}
