#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

#define MAX_SPEED 650
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
        int magnitude = std::sqrt(std::pow(X, 2) + std::pow(Y, 2));
        X /= magnitude;
        Y /= magnitude;
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

int GetAngleBetweenVectors(Vector2 v1, Vector2 v2)
{
    int length1 = v1.Length();
    int length2 = v2.Length();
    return std::acos((v1.X * v2.X + v1.Y * v2.Y) / length1 * length2);
}


struct CheckpointManager
{

    std::vector<Vector2> CheckpointPositions;

    void SaveCheckpoint(Vector2 checkpoint)
    {
        CheckpointPositions.push_back(checkpoint);
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
            int tempLength = tempCheckpoint.LengthSqrd();
            if (tempLength > maxDist)
            {
                maxDist = tempLength;
                maxCheckpoint = CheckpointPositions[i + 1];
            }
        }
        Vector2 temp = CheckpointPositions[CheckpointPositions.size() - 1] - CheckpointPositions[0];
        if (temp.LengthSqrd() > maxDist)
        {
            return CheckpointPositions[0];
        }
        else
        {
            return maxCheckpoint;
        }
    }

    Vector2 GetCheckpoint(int checkpointID)
    {
        return CheckpointPositions[checkpointID - 1];
    }

    Vector2 GetNextCheckpoint(int nextCheckpointID)
    {
        //++nextCheckpointID; //Not necessary as checkpoints start from 1 instead of 0
        nextCheckpointID %= CheckpointPositions.size();

        return CheckpointPositions[(nextCheckpointID)];
    }

};


struct Pod
{

    Vector2 Position;
    Vector2 Velocity;
    int Thrust = 0;
    int RotationAngle;
    int TargetCheckpointID;
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

    int GetDistanceToPosition(Vector2 targetPos) const
    {
        return (Position - targetPos).LengthSqrd();
    }

    void output(Vector2 targetCheckpoint, Vector2 nextCheckpoint)
    {
        int targetCheckpointAngle = GetAngleBetweenVectors(Position, targetCheckpoint); //fix
        int targetCheckpointDist = (Position - targetCheckpoint).LengthSqrd();

        int absCheckpointAngle = std::abs(targetCheckpointAngle);
        if (absCheckpointAngle >= 90)
        {
            Thrust = 0;
        }
        else
        {
            Thrust = 100;
        }

        Vector2 desiredVelocity = (nextCheckpoint - Position).Normalize() * MAX_SPEED;
        Vector2 steering = desiredVelocity - Velocity;

        if (targetCheckpointDist < 1000)
        {
            cout << nextCheckpoint.X << " " << nextCheckpoint.Y << " ";
        }
        else
        {
            cout << (nextCheckpoint + steering).X << " " << (nextCheckpoint + steering).Y << " ";
        }

        if (!bBoostUsed && absCheckpointAngle < 5)
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

    Vector2 maxCheckpoint;
    maxCheckpoint = CPManager.GetMaxDistCheckpoint();

    Pod allyPod1;
    Pod allyPod2;
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