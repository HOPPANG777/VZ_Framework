#pragma once

#include "Enum/AxeState.h"

class BezierCurveAxe : public Component
{
public:
    BezierCurveAxe(const string& compName = "BezierCurveAxe") : Component(compName) {}
    virtual ~BezierCurveAxe() override = default;

    virtual void Update() override;  
    virtual void Render() override;  

private:



};

