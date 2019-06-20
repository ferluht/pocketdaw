//
// Created by Admin on 09.06.2019.
//

#ifndef PD_MODULE_H
#define PD_MODULE_H

#include <stdlib.h>
#include <string>

class Parameter
{
public:

    float value;
    float * mapping;
    std::string name;

    Parameter();
    Parameter(std::string name_);

    void map(float * param);
    void unmap();
    void set(float value_);
    void setdelta(float delta);
    float get();
    bool ismapped();
    void range_guard();
};

class ParameterMinMax : public Parameter
{
public:
    float min, max;

    ParameterMinMax(char * name_, float min_, float max_);
    void range_guard();
};

class ParameterPeriodic : public ParameterMinMax
{
public:

    ParameterPeriodic(char * name_, float min_, float max_);
    void range_guard();
};

class Module
{
public:
    std::string name;
    Parameter * params;

    Module() = default;
    Module(std::string name_);

    virtual void step();
};


#endif //PD_MODULE_H
