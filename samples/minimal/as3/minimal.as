package
{
	import flash.utils.*;
	
	public class MinimalSample
	{
		private var mX:int;
		private var mY:int;
		
		public function MinimalSample(x:int, y:int)
		{
			trace("ctor", x, y);
			mX = x;
			mY = y;
		}
		
		public function add():int
		{
			return mX + mY;
		}
		
		public function subtract():int
		{
			return mX - mY;
		}
		
		public function add2(x:int, y:int):int
		{
			return x + y;
		}
	}
	
	var minSam:MinimalSample = new MinimalSample(1, 2);
	trace("add:", minSam.add());
	trace("subtract:", minSam.subtract());
	
	/*
	var dict = new Dictionary();
	var obj = new Object();
	var key:Object = new Object();
	key.toString = function() { return "key" }
	
	dict[key] = "Letters";
	obj["key"] = "Letters";
	
	trace(dict[key] == "Letters", true); // true
	trace(obj["key"] == "Letters", true); // true
	trace(obj[key] == "Letters", true); // true because key == "key" is true b/c key.toString == "key"
	trace(dict["key"] == "Letters", false); // false because "key" === key is false
	delete dict[key]; //removes the key
	*/
}
