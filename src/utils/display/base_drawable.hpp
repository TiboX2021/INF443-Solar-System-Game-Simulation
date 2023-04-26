#pragma once

// Base virtual class for polymorphism
class BaseDrawable
{
public:
    virtual void initialize(){}; // Bug : ça accepte de compiler si cette méthode n'est pas abstraite (=0)
};