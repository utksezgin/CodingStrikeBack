#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

struct Vector2
{
    int X;
    int Y;
    Vector2()
    {
    }
    Vector2(Vector2& other) : X(other.X), Y(other.Y)
    {
    }
    Vector2(int x, int y) : X(x), Y(y)
    {
    }

    Vector2 Normalize()
    {
        int magnitude = std::sqrt(std::pow(X, 2) + std::pow(Y, 2));
        return Vector2(X / magnitude, Y / magnitude);
    }

    Vector2 trunc(int max)
    {
        X = std::clamp(X, X, max);
        Y = std::clamp(Y, Y, max);
    }

    Vector2 operator-(Vector2 other)
    {
        return Vector2(other.X - X, other.Y - Y);
    }

    Vector2 operator+(Vector2 other)
    {
        return Vector2(other.X + X, other.Y + Y);
    }

    Vector2 operator*(Vector2 other)
    {
        return Vector2(other.X * X, other.Y * Y);
    }

};


int lerp(int a, int b, float f)
{
    return a + f * (b - a);
}

int main()
{
    bool bBoostUsed = false;
    Vector2 lastPosition(0, 0);
    int maxSpeed = 600;
    int maxForce = 100;

    while (1)
    {

        Vector2 podPosition;
        Vector2 nextCheckpoint;
        int nextCheckpointDist; // distance to the next checkpoint
        int nextCheckpointAngle; // angle between your pod orientation and the direction of the next checkpoint
        cin >> podPosition.X >> podPosition.Y >> nextCheckpoint.X >> nextCheckpoint.Y >> nextCheckpointDist >> nextCheckpointAngle; cin.ignore();
        Vector2 opponent;
        cin >> opponent.X >> opponent.Y; cin.ignore();

        int minBoostDistance = 6000;
        int thrust = 0;
        int slowDownThrust = 50;

        int velocityMag = std::sqrt(std::pow((podPosition.X - lastPosition.X), 2) + std::pow((podPosition.Y - lastPosition.Y), 2));
        Vector2 velocity;
        velocity.X = podPosition.X - lastPosition.X;
        velocity.Y = podPosition.Y - lastPosition.Y;

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;
        int absCheckpointAngle = std::abs(nextCheckpointAngle);
        if (absCheckpointAngle > 90)
        {
            thrust = 0;
        }
        else if (absCheckpointAngle < 10)
        {
            thrust = 100;
        }
        else
        {
            thrust = lerp(10, 100, 1 - std::abs(nextCheckpointAngle) / 90.f);
        }



        if (nextCheckpointDist <= 800)
        {
            thrust = 25;
        }
        else if (nextCheckpointDist <= 1100)
        {
            thrust = 50;
        }
        else if (nextCheckpointDist <= 1300)
        {
            thrust = 75;
        }


        cerr << "cp dist: " << nextCheckpointDist << " -- cp angle: "
            << nextCheckpointAngle << " -- thrust: " << thrust << " Velocity: " << velocityMag <<
            " X: " << velocity.X << " Y: " << velocity.Y << endl;

        // You have to output the target position
        // followed by the power (0 <= thrust <= 100)
        // i.e.: "x y thrust"

        cout << nextCheckpoint.X << " " << nextCheckpoint.Y << " ";
        if (!bBoostUsed && absCheckpointAngle < 5
            && nextCheckpointDist > minBoostDistance)
        {
            cout << "BOOST" << endl;
            bBoostUsed = true;
        }
        else
        {
            cout << thrust << endl;
        }
        lastPosition.X = podPosition.X;
        lastPosition.Y = podPosition.Y;
    }
}