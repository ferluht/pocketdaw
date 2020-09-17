//
// Created by Admin on 09.06.2019.
//

#include "Module.h"

Parameter::Parameter()
{
    mapping = nullptr;
    value = 0;
}

Parameter::Parameter(std::string name_)
{
    name = std::move(name_);
    mapping = nullptr;
    value = 0;
}

void Parameter::map(float * param)
{
    mapping = param;
}

void Parameter::unmap()
{
    mapping = nullptr;
}

void Parameter::set(float value_)
{
    value = value_;
    range_guard();
}

void Parameter::setdelta(float delta)
{
    value += delta;
    range_guard();
}

float Parameter::get()
{
    if (mapping){
        return *mapping + value;
    }
    return value;
}

bool Parameter::ismapped()
{
    return (mapping != nullptr);
}

void Parameter::range_guard()
{

}


ParameterMinMax::ParameterMinMax(char * name_, float min_, float max_)
        : Parameter(name_)
{
    min = min_;
    max = max_;
}

void ParameterMinMax::range_guard()
{
    if (value > max) value = max;
    if (value < min) value = min;
}

ParameterPeriodic::ParameterPeriodic(char * name_, float min_, float max_)
        : ParameterMinMax(name_, min_, max_)
{

}

void ParameterPeriodic::range_guard()
{
    // if (value > max) value = fmod(value - min, max - min) + min;
    // if (value < min) value = fmod(value - max, max - min) + max;
}


Module::Module(std::string name_)
{
    name = std::move(name_);
}

void Module::step() {

}