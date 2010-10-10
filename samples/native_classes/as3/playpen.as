package
{
	import my.pkg.MyClass;

	// call the static functions
	trace(MyClass.static_as3_function(2, 3));
	trace(MyClass.static_native_function());

	// create an instance of the class
	var obj:MyClass = new MyClass();

	// call the class methods
	obj.as3_function();
	trace(obj.native_function("123456789"));
}
