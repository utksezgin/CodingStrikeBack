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
    int X = 0;
    int Y = 0;
    Vector2()
    {
    }
    Vector2(const Vector2& other) : X(other.X), Y(other.Y)
    {
    }
    Vector2(int x, int y) : X(x), Y(y)
    {
    }

    Vector2 Normalize()
    {
        int magnitude = std::sqrt(std::pow(X, 2) + std::pow(Y, 2));
        X /= magnitude;
        Y /= magnitude;
        return *this;
    }

    int Length() const
    {
        return std::pow(X, 2) + std::pow(Y, 2);
    }

    Vector2 trunc(int max)
    {
        X = std::clamp(X, X, max);
        Y = std::clamp(Y, Y, max);
        return *this;
    }

    Vector2 operator-(Vector2 other) const
    {
        return Vector2(X - other.X, Y - other.Y);
    }

    Vector2 operator+(Vector2 other) const
    {
        return Vector2(other.X + X, other.Y + Y);
    }

    Vector2 operator*(int mag) const
    {
        return Vector2(X * mag, Y * mag);
    }

    bool operator==(Vector2 other) const
    {
        return (X == other.X && Y == other.Y);
    }

    bool operator!=(Vector2 other) const
    {
        return !((*this) == other);
    }

};


struct CheckpointManager
{

    std::vector<Vector2> CheckpointPositions;
    bool bCheckpointsFound = false;

    void SaveCheckpoint(Vector2 checkpoint)
    {
        auto it = std::find(CheckpointPositions.begin(), CheckpointPositions.end(), checkpoint);
        if (it == CheckpointPositions.end())
        {
            CheckpointPositions.push_back(checkpoint);
        }
        else
        {
            bCheckpointsFound = true;
        }
    }

    Vector2 GetMaxDistCheckpoint()
    {
        if (CheckpointPositions.size() <= 1)
            return Vector2(0, 0);
        Vector2 maxCheckpoint;
        int maxDist = 0;
        for (int i = 0; i < CheckpointPositions.size() - 1; ++i)
        {
            Vector2 tempCheckpoint = CheckpointPositions[i] - CheckpointPositions[i + 1];
            int tempLength = tempCheckpoint.Length();
            if (tempLength > maxDist)
            {
                maxDist = tempLength;
                maxCheckpoint = CheckpointPositions[i + 1];
            }
        }
        Vector2 temp = CheckpointPositions[CheckpointPositions.size() - 1] - CheckpointPositions[0];
        if (temp.Length() > maxDist)
        {
            return CheckpointPositions[0];
        }
        else
        {
            return maxCheckpoint;
        }
    }

    Vector2 GetNextCheckpoint(Vector2 nextCheckPoint)
    {
        auto it = std::find(CheckpointPositions.begin(), CheckpointPositions.end(), nextCheckPoint);
        return *(it + 1);
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
    int CheckpointRadius = 600;
    CheckpointManager CPManager;
    Vector2 lastCheckpoint;
    while (1)
    {


        Vector2 podPosition;
        Vector2 nextCheckpoint;
        int nextCheckpointDist; // distance to the next checkpoint
        int nextCheckpointAngle; // angle between your pod orientation and the direction of the next checkpoint
        cin >> podPosition.X >> podPosition.Y >> nextCheckpoint.X >> nextCheckpoint.Y >> nextCheckpointDist >> nextCheckpointAngle; cin.ignore();
        Vector2 opponent;
        cin >> opponent.X >> opponent.Y; cin.ignore();
        int thrust = 0;
        int slowDownThrust = 50;

        int velocityMag = std::sqrt(std::pow((podPosition.X - lastPosition.X), 2) + std::pow((podPosition.Y - lastPosition.Y), 2));
        Vector2 velocity;
        velocity.X = podPosition.X - lastPosition.X;
        velocity.Y = podPosition.Y - lastPosition.Y;

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;
        int absCheckpointAngle = std::abs(nextCheckpointAngle);
        if (absCheckpointAngle >= 90)
        {
            thrust = 0;
        }
        else //if (absCheckpointAngle < 10)
        {
            thrust = 100;
        }
        /*else
        {
            thrust = lerp(0, 100, 1 - std::abs(nextCheckpointAngle) / 90.f);
        }*/


        //Damp
        //thrust *= std::clamp(nextCheckpointDist / (CheckpointRadius * 2.5) , 0.0, 1.0);

        cerr << "cp dist: " << nextCheckpointDist << " -- cp angle: "
            << nextCheckpointAngle << " -- thrust: " << thrust << " Velocity: " << velocityMag <<
            " X: " << velocity.X << " Y: " << velocity.Y << endl;


        Vector2 desiredVelocity = (nextCheckpoint - podPosition).Normalize() * maxSpeed;
        Vector2 steering = desiredVelocity - velocity;



        bool bBoostUsable = false;
        Vector2 maxCheckpoint;

        if (nextCheckpoint != lastCheckpoint)
        {
            CPManager.SaveCheckpoint(nextCheckpoint);
        }

        if (CPManager.bCheckpointsFound && maxCheckpoint.X == 0 && maxCheckpoint.Y == 0)
        {
            maxCheckpoint = CPManager.GetMaxDistCheckpoint();
            if (nextCheckpoint == maxCheckpoint)
            {
                bBoostUsable = true;
            }
        }

        if (nextCheckpointDist < 1000 && CPManager.bCheckpointsFound)
        {
            Vector2 nextCP = CPManager.GetNextCheckpoint(nextCheckpoint);
            cout << nextCP.X << " " << nextCP.Y << " ";
        }
        else
        {
            cout << (nextCheckpoint + steering).X << " " << (nextCheckpoint + steering).Y << " ";
        }

        if (!bBoostUsed && absCheckpointAngle < 5 && bBoostUsable)
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
        lastCheckpoint = nextCheckpoint;
    }
}