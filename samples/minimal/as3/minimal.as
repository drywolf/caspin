package
{
	// a simple test class which is completely written in AS3
	public class MinimalSample
	{
		private var mX:int;
		private var mY:int;

		public function MinimalSample(x:int, y:int)
		{
			trace("constructor", x, y);
			mX = x;
			mY = y;
		}

		public function add():String
		{
			return mX + " + " + mY + " = " + (mX + mY);
		}

		public function subtract():String
		{
			return mX + " - " + mY + " = " + (mX - mY);
		}

		public function add_int(x:int, y:int):int
		{
			return x + y;
		}
	}

	public function main(... args):int
	{
		// print the received arguments
		trace (args);

		// try out the above class
		var minSam:MinimalSample = new MinimalSample(1, 2);
		trace("add:", minSam.add());
		trace("sub:", minSam.subtract());

		// return an interesting value
		return minSam.add_int(3, 4);
	}
}
