#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

#define PI 3.14159265
#define MAX_SPEED 500
#define MAX_FORCE 100

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

    int LengthSqrd() const
    {
        return std::pow(X, 2) + std::pow(Y, 2);
    }

    int Length() const
    {
        return std::sqrt(std::pow(X, 2) + std::pow(Y, 2));
    }

    Vector2 Normalize()
    {
        X /= Length();
        Y /= Length();
        return *this;
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

    void SaveCheckpoint(Vector2 checkpoint)
    {
        CheckpointPositions.push_back(checkpoint);
    }

    int GetMaxDistCheckpoint()
    {
        int maxCheckpointID;
        int maxDist = 0;
        for (int i = 0; i < CheckpointPositions.size() - 1; ++i)
        {
            Vector2 tempCheckpoint = CheckpointPositions[i] - CheckpointPositions[i + 1];
            int tempLength = tempCheckpoint.LengthSqrd();
            if (tempLength > maxDist)
            {
                maxDist = tempLength;
                maxCheckpointID = i + 1;
            }
        }
        Vector2 temp = CheckpointPositions[CheckpointPositions.size() - 1] - CheckpointPositions[0];
        if (temp.LengthSqrd() > maxDist)
        {
            return 0;
        }
        else
        {
            return maxCheckpointID;
        }
    }

    Vector2 GetCheckpoint(int checkpointID)
    {
        return CheckpointPositions[checkpointID];
    }

    Vector2 GetNextCheckpoint(int checkpointID)
    {
        ++checkpointID;
        checkpointID %= CheckpointPositions.size();

        return CheckpointPositions[(checkpointID)];
    }

};


struct Pod
{

    Vector2 Position;
    Vector2 Velocity;
    int Thrust = 0;
    int RotationAngle;
    int TargetCheckpointID;
    int MaxDistCheckpointID;
    bool bBoostUsed = false;

    int GetThrust(int destinationAngle)
    {
        int absdestinationAngle = std::abs(destinationAngle);
        if (absdestinationAngle >= 90)
        {
            Thrust = 0;
        }
        else
        {
            Thrust = 100;
        }
        return Thrust;
    }

    void GetInput()
    {
        cin >> Position.X >> Position.Y >> Velocity.X >> Velocity.Y >>
            RotationAngle >> TargetCheckpointID; cin.ignore();
    }

    float GetAngleToTarget(Vector2 target)
    {
        float dist = (target - Position).Length();
        float distX = (target.X - Position.X) / dist;
        float distY = (target.Y - Position.Y) / dist;

        // Simple trigonometry. We multiply by 180.0 / PI to convert radiants to degrees.
        int angle = acos(distX) * 180.0 / PI;

        // If the point I want is below me, I have to shift the angle for it to be correct
        if (distY < 0)
        {
            angle = 360.0 - angle;
        }

        return angle;
    }

    int GetDistanceToPosition(Vector2 targetPos) const
    {
        return (Position - targetPos).LengthSqrd();
    }

    void output(Vector2 targetCheckpoint, Vector2 nextCheckpoint)
    {
        Vector2 dir = targetCheckpoint - Position;
        int targetCheckpointAngle = GetAngleToTarget(targetCheckpoint) - RotationAngle;
        int targetCheckpointDist = (Position - targetCheckpoint).Length();

        cerr << "Angle: " << targetCheckpointAngle << " Rotation: " << RotationAngle << endl;

        int absCheckpointAngle = std::abs(targetCheckpointAngle);
        if (absCheckpointAngle >= 90)
        {
            Thrust = 0;
        }
        else
        {
            Thrust = 100;
        }

        cerr << "TargetCP: " << targetCheckpoint.X << " nextCP: " << nextCheckpoint.X << endl;
        if (targetCheckpointDist < 1000)
        {
            Vector2 desiredVelocity = (nextCheckpoint - Position).Normalize() * MAX_SPEED;
            Vector2 steering = Vector2(0, 0); desiredVelocity - Velocity;

            cout << (nextCheckpoint + steering).X << " " << (nextCheckpoint + steering).Y << " ";
        }
        else
        {
            Vector2 desiredVelocity = (targetCheckpoint - Position).Normalize() * MAX_SPEED;
            Vector2 steering = desiredVelocity - Velocity;

            cout << (targetCheckpoint + steering).X << " " << (targetCheckpoint + steering).Y << " ";
        }

        if (!bBoostUsed && absCheckpointAngle < 5 && MaxDistCheckpointID == TargetCheckpointID)
        {
            cout << "BOOST" << endl;
            bBoostUsed = true;
        }
        else
        {
            cout << Thrust << endl;
        }
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
    int checkpointRadius = 600;
    CheckpointManager CPManager;
    Vector2 lastCheckpoint;

    int lapCount = 0;
    int checkpointCount = 0;

    cin >> lapCount >> checkpointCount; cin.ignore();
    for (int i = 0; i < checkpointCount; ++i)
    {
        int checkpointX;
        int checkpointY;
        cin >> checkpointX >> checkpointY; cin.ignore();
        CPManager.SaveCheckpoint(std::move(Vector2(checkpointX, checkpointY)));
    }

    int maxCheckpoint;
    maxCheckpoint = CPManager.GetMaxDistCheckpoint();

    Pod allyPod1;
    Pod allyPod2;

    allyPod1.MaxDistCheckpointID = maxCheckpoint;
    allyPod2.MaxDistCheckpointID = maxCheckpoint;
    Pod opponentPod1;
    Pod opponentPod2;


    while (1)
    {


        Vector2 podPosition;
        Vector2 nextCheckpoint;
        Vector2 opponent;
        int thrust = 0;


        allyPod1.GetInput();
        allyPod2.GetInput();
        opponentPod1.GetInput();
        opponentPod2.GetInput();


        allyPod1.output(CPManager.GetCheckpoint(allyPod1.TargetCheckpointID), CPManager.GetNextCheckpoint(allyPod1.TargetCheckpointID));
        allyPod2.output(CPManager.GetCheckpoint(allyPod2.TargetCheckpointID), CPManager.GetNextCheckpoint(allyPod2.TargetCheckpointID));

        //log
        /*
        cerr << "cp dist: " << nextCheckpointDist << " -- cp angle: "
        << nextCheckpointAngle << " -- thrust: " << thrust << " Velocity: " << velocityMag << endl;
        */
    }
}