#pragma once

namespace gpx
{
	struct Color
	{
		float r, g, b, a;
		
		Color(){}	
		Color(float red, float green, float blue, float alpha)
			:
				r(red),
				g(green),
				b(blue),
				a(alpha)
		{
			Clamp();
		}

		void Set(const Color& color)
		{
			r = color.r;
			g = color.g;
			b = color.b;
			a = color.a;
		}

		void Set(float red, float green, float blue, float alpha)
		{
			r = red;
			g = green;
			b = blue;
			a = alpha;
		}

		void Clamp()
		{
			if (r < 0.0f)
				r = 0.0f;
			else if (r > 1.0f)
				r = 1.0f;
			if (g < 0.0f)
				g = 0.0f;
			else if (g > 1.0f)
				g = 1.0f;
			if (b < 0.0f)
				b = 0.0f;
			else if (b > 1.0f)
				b = 1.0f;
			if (a < 0.0f)
				a = 0.0f;
			else if (a > 1.0f)
				a = 1.0f;
		
		}

		int ToIntBits()
		{
			int color = ((int)(255 * a) << 24) | ((int)(255 * b) << 16)
							| ((int)(255 * g) << 8) | ((int)(255 * r));
			return color;
		}
	};
}
