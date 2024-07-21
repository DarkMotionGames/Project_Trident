#pragma once


// FUNCTION MACROS //


#define DEBUG_SPHERE(Location) if(GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Red, false, 5.f);


#define DEBUG_LINE(StartLocation, EndLocation) if(GetWorld()) DrawDebugLine(World, Location, LineEnd, FColor::Blue, false, -1.f);


#define DEBUG_POINT(Location) if(GetWorld()) DrawDebugPoint(World, Location, 15.f, FColor::Green, false, -1.f);


#define DRAW_VECTOR(StartLocation, EndLocation) if(GetWorld())\
{\
DrawDebugLine(World, StartLocation, EndLocation, FColor::Blue, false, -1.f);\
DrawDebugPoint(World, EndLocation, 15.f, FColor::Green, false, -1.f);\
}