// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "commands/TankTurn.h"

TankTurn::TankTurn(Drivetrain& drivetrain, double angle):
    m_drivetrain(&drivetrain),
    m_angle(angle) {

    SetName("TankTurn");
    AddRequirements({m_drivetrain});
}

// Called just before this Command runs the first time
void TankTurn::Initialize() {
    m_drivetrain->m_navX.Reset();//SetAngleAdjustment(0);
    //m_drivetrain->m_turnPid.Reset();
    m_drivetrain->m_turnPid.SetSetpoint(m_angle);
    m_drivetrain->Drive(0, 0);
}


// Called repeatedly when this Command is scheduled to run
void TankTurn::Execute() {
    double angle = m_drivetrain->m_navX.GetAngle();
    if (m_angle >= 0 && angle < -90){
        angle += 360;
    } else if(m_angle < 0 && angle > 90){
        angle -= 360;
    }
    double correction = m_drivetrain->m_turnPid.Calculate(angle);
    if(correction > 0.4){
        correction = 0.4;
    } else if(correction < -0.4){
        correction = -0.4;
    }
    if (m_angle >= 0){
        m_drivetrain->Drive(-0.3-correction, 0.3+correction);
    }
    else {
        m_drivetrain->Drive(0.3-correction, -0.3+correction);
    };
}

// Make this return true when this Command no longer needs to run execute()
bool TankTurn::IsFinished() {
    double angle = m_drivetrain->m_navX.GetAngle();
    if (m_angle >= 0){
        return(angle >= m_angle);
    }
    else{
        return(angle <= m_angle);
    }
}

// Called once after isFinished returns true
void TankTurn::End(bool) {
    m_drivetrain->Drive(0, 0);
}


