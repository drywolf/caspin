package flash.geom
{
	[native(cls="::flash::geom::ColorTransformClass", instance="::flash::geom::ColorTransform", methods="auto")]
	public class ColorTransform
	{
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
		

			public function test()
			{
				trace("lolo")
			}
		
		public native function get color():int
		public native function set color(value:int):void
	}
}
