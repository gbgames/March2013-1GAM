#ifndef GB_POINT_H
#define GB_POINT_H

class Point
{
	public:
		Point();
		Point(float x, float y, float z = 0);
		~Point();

		float X() const;
		float Y() const;
		float Z() const;

		int intX() const;
		int intY() const;
		int intZ() const;

		int floorX() const;
		int floorY() const;
		int floorZ() const;

		void setX(float value);
		void setY(float value);
		void setZ(float value);
		void setPosition(float x, float y, float z = 0.0f);
		void moveBy(float x, float y, float z = 0.0f);
		void moveBy(Point offset);

		bool operator==(const Point& other) const;
		bool operator!=(const Point& other) const;
		bool operator<(const Point& other) const;

	public:
		float m_xPos;
		float m_yPos;
		float m_zPos;

};

double calculateDistance(Point start, Point target);
double calculateDistanceSquared(Point start, Point target);
double calculateDotProduct(Point a, Point b);

#endif
