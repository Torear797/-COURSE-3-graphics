#pragma once
class WorldWindow
{
public:
		float Left;
		float Top;
		float Right;
		float Bottom;
		inline WorldWindow(float left, float top, float right, float bottom) : Left(left), Top(top), Right(right), Bottom(bottom) {}
		inline float Width() const { return (Right - Left); }
		inline float Height() const { return (Top - Bottom); }
	WorldWindow();
	~WorldWindow();
};
