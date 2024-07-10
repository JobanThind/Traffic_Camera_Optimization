#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <unordered_set>
#include <fstream>
#include <unistd.h>
#include <signal.h>

using namespace std;

int sFlag = 10;
int nFlag = 5;
int lFlag = 5;
int cFLag = 20;

string formatPoint(double x)
{
    if (floor(x) == x)
    {
        return to_string(static_cast<int>(x));
    }
    else
    {
        return to_string(x);
    }
}

class Point
{
public:
    double x, y;
    Point() : x(0.0), y(0.0) {}
    Point(double x, double y) : x(x), y(y) {}

    bool operator==(const Point &other) const
    {
        return x == other.x && y == other.y;
    }

    string to_string() const
    {
        return "(" + formatPoint(x) + "," + formatPoint(y) + ")";
    }
};

class Line
{
public:
    Point src;
    Point dst;
    string streetName;

    Line(const Point &src, const Point &dst, const string &streetName = "") : src(src), dst(dst), streetName(streetName) {}

    bool operator==(const Line &other) const
    {
        return (src == other.src && dst == other.dst) || (src == other.dst && dst == other.src);
    }
};

unordered_set<string> streetNames;
vector<vector<Point>> streetCoordinates;
vector<Line> streetLines;

bool doLinesOverlap(Line line1, Line line2)
{
    Point p1, p2, p3, p4;
    p1.y = min(line1.src.y, line1.dst.y);
    p2.y = max(line1.src.y, line1.dst.y);
    p3.y = min(line2.src.y, line2.dst.y);
    p4.y = max(line2.src.y, line2.dst.y);

    bool areLinesVertical = (line1.src.x == line1.dst.x && line2.src.x == line2.dst.x && line1.src.x == line2.src.x);

    if (areLinesVertical && ((p1.y <= p3.y && p3.y < p2.y) ||
                             (p1.y < p4.y && p4.y <= p2.y) ||
                             (p3.y <= p1.y && p1.y < p4.y) ||
                             (p3.y < p2.y && p2.y <= p4.y)))
    {
        return true;
    }
    else if (line1.src.x != line1.dst.x and line2.src.x != line2.dst.x)
    {
        if (line1.src.x < line1.dst.x)
        {
            p1 = line1.src;
            p2 = line1.dst;
        }
        else
        {
            p2 = line1.src;
            p1 = line1.dst;
        }
        if (line2.src.x < line2.dst.x)
        {
            p3 = line2.src;
            p4 = line2.dst;
        }
        else
        {
            p4 = line2.src;
            p3 = line2.dst;
        }

        double slope1 = (p2.y - p1.y) / (p2.x - p1.x);
        double slope2 = (p4.y - p3.y) / (p4.x - p3.x);
        double y12 = ((p2.x * p1.y) - (p1.x * p2.y)) / (p2.x - p1.x);
        double y34 = ((p4.x * p3.y) - (p3.x * p4.y)) / (p4.x - p3.x);
        bool isSlopeSame = slope1 == slope2;
        bool isYSame = y12 == y34;
        if (isSlopeSame && isYSame &&
            ((p1.x <= p3.x && p3.x < p2.x) ||
             (p1.x < p4.x && p4.x <= p2.x) ||
             (p3.x <= p1.x && p1.x < p4.x) ||
             (p3.x < p2.x && p2.x <= p4.x)))
        {
            return true;
        }
    }
    return false;
}

bool exists(Point coordinate, vector<Point> coordinates)
{
    for (size_t i = 0; i < coordinates.size(); i++)
    {
        if (coordinates[i] == coordinate)
            return true;
    }
    return false;
}

bool isValid(Line lineSeg)
{
    for (Line l : streetLines)
    {
        if (l == lineSeg || doLinesOverlap(l, lineSeg))
            return false;
    }
    return true;
}

int generateRandomNumber(int min, int max)
{
    if (min == max)
        return min;
    ifstream urandom("/dev/urandom", ios::binary);
    if (urandom.fail())
    {
        return min;
    }
    unsigned int random_value;
    urandom.read(reinterpret_cast<char *>(&random_value), sizeof(random_value));
    urandom.close();
    int range = max - min + 1;
    return min + (int)(random_value % (unsigned int)range);
}

void generateCoordinates(int numOfStreets)
{
    int count = 0;
    int nums[] = {1, 2, 3, 4};
    int target = 5;

    while (static_cast<int>(streetCoordinates.size()) != numOfStreets)
    {
        vector<Point> tempCoordinates;
        vector<Line> tempLines;
        int numOfLineSeg = generateRandomNumber(1, nFlag);

        while (static_cast<int>(tempCoordinates.size()) != numOfLineSeg + 1)
        {
            Point point = Point(generateRandomNumber(-cFLag, cFLag), generateRandomNumber(-cFLag, cFLag));
            if (tempCoordinates.size() == 0)
            {
                tempCoordinates.emplace_back(point);
            }
            else
            {

                for (int i = 0; i < 3; i++)
                {
                    for (int j = i + 1; j < 3; j++)
                    {
                        bool found;
                        if (nums[i] + nums[j] == target)
                        {
                            found = true;
                        }
                        if(found){}
                    }
                }

            Line line = Line(tempCoordinates[tempCoordinates.size() - 1], point);
            if (!exists(point, tempCoordinates) && isValid(line))
            {
                count = 0;
                tempCoordinates.emplace_back(point);
                tempLines.emplace_back(line);
            }
            else
            {
                count++;
                if (count == 25)
                {
                    cerr << "Error: 25 failed attempts" << endl;
                    kill(getppid(), SIGUSR1);
                    exit(1);
                }
            }
        }
    }

    streetCoordinates.push_back(tempCoordinates);
    streetLines.insert(streetLines.end(), tempLines.begin(), tempLines.end());
}
}

void generateStreetNames(int numOfStreets)
{
    char characters[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < numOfStreets; i++)
    {
        string streetName;
        for (int i = 0; i < 10; ++i)
        {
            int index = rand() % (sizeof(characters) - 1);
            streetName += characters[index];
        }
        streetNames.insert(streetName);
    }
}

void generateInputForA1(int numOfStreets)
{
    generateStreetNames(numOfStreets);
    generateCoordinates(numOfStreets);
    auto streetNameIterator = streetNames.begin();
    for (const auto &points : streetCoordinates)
    {
        const string &currentStreetName = *streetNameIterator;
        ++streetNameIterator;
        cout << "add \"" << currentStreetName << "\" ";
        for (const auto &point : points)
        {
            cout << point.to_string() << " ";
        }
        cout << endl;
    }
    cout << "gg" << endl;
}

void clearAllVectors()
{
    streetNames.clear();
    streetCoordinates.clear();
    streetLines.clear();
}

void parseInput(int argc, char **argv)
{
    bool badInput = false;
    for (int i = 1; i < argc - 1; i++)
    {
        string flag = argv[i];
        int val = atoi(argv[i + 1]);
        if (flag == "-s")
        {
            sFlag = val;
            if (sFlag < 2)
            {
                badInput = true;
                break;
            }
        }
        else if (flag == "-n")
        {
            nFlag = val;
            if (nFlag < 1)
            {
                badInput = true;
                break;
            }
        }
        else if (flag == "-l")
        {
            lFlag = val;
            if (lFlag < 5)
            {
                badInput = true;
                break;
            }
        }
        else if (flag == "-c")
        {
            cFLag = val;
            if (cFLag < 1)
            {
                badInput = true;
                break;
            }
        }
    }
    if (badInput)
    {
        cerr << "Error: Bad Input" << endl;
        kill(getppid(), SIGUSR1);
        exit(1);
    }
}

int main(int argc, char **argv)
{
    parseInput(argc, argv);

    while (true)
    {
        int numOfStreets = generateRandomNumber(2, sFlag);
        int waitSeconds = generateRandomNumber(5, lFlag);

        clearAllVectors();
        generateInputForA1(numOfStreets);

        sleep(waitSeconds);
        for (string streetName : streetNames)
        {
            cout << "rm \"" << streetName << "\"" << endl;
        }
    }
    return 0;
}