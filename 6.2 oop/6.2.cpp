#include "Route.h"
#include "RoutesArray.h"

using namespace std;

int main() {
    RoutesArray array;

    GPSLocation loc1{ 10.0, 20.0 };
    GPSLocation loc2{ 30.0, 40.0 };

    array.add(new Car(30, 100, loc1, loc2, "example.com"));
    array.add(new Bus(50, 200, loc1, loc2));
    array.add(new Taxi(40, 150, loc1, loc2));

    cout << "Displaying all routes:" << endl;
    array.print();

    array.remove(1);
    cout << "After removing the Bus object:" << endl;
    array.print();

    array.clear();
    cout << "After clearing the array:" << endl;
    array.print();

    return 0;
}