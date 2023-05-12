// Fill out your copyright notice in the Description page of Project Settings.


#include "MassComponent.h"

class MyComponent : public UPrimitiveComponent
{
public:
    float GetMass()
    {
        return Mass;
    }

private:
    float Mass;
};
