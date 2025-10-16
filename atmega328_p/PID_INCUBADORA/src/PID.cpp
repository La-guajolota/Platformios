/****************************************************************************************
 * @file PID.cpp
 * @brief Implementation of a PID controller for embedded systems, used for regulating
 *        temperature or other physical variables. This file contains the `PIDController`
 *        class that manages PID calculations, with support for dynamic gain adjustments
 *        and limitation of the controller output.
 *
 * @author Adrian Silva Palafox
 * @company Fourie Embeds
 * @date November 2024
 *
 * @license This code is open source under the [Your License Here] license.
 *          It can be modified and distributed for educational or commercial purposes.
 *
 * @note It is assumed that the necessary libraries like "PID.hpp" and other dependencies
 *       are already correctly included and configured in the development environment.
 ***************************************************************************************/

// Include the header file for the PID implementation
#include "PID.hpp"

// Constructor of the PIDController class
PIDController::PIDController(float kp, float ki, float kd,
                             float tau,
                             float limMin, float limMax,
                             float limMinInt, float limMaxInt,
                             float t)
    : Kp(kp), Ki(ki), Kd(kd),                     // Initialize PID gains (proportional, integral, derivative)
      tau(tau),                                   // Low-pass filter time constant (for the derivative)
      limMin(limMin), limMax(limMax),             // Limits for the controller output
      limMinInt(limMinInt), limMaxInt(limMaxInt), // Limits for the integrator (anti-windup)
      T(t),                                       // Sampling time (time interval between updates)
      integrator(0.0f),                           // Initialize the integrator (accumulates error)
      prevError(0.0f),                            // Initialize the previous error (for derivative calculation)
      differentiator(0.0f),                       // Initialize the differentiator (for the derivative term)
      prevMeasurement(0.0f),                      // Initialize the previous measurement (for derivative calculation)
      out(0.0f)                                   // Initialize the controller output
{
}

// Method to reset the PID controller
void PIDController::reset()
{
    integrator = 0.0f;      // Reset the integrator
    prevError = 0.0f;       // Reset the previous error
    differentiator = 0.0f;  // Reset the differentiator
    prevMeasurement = 0.0f; // Reset the previous measurement
    out = 0.0f;             // Reset the output
}

// Method that updates the PID controller each time it is called
float PIDController::update(float setpoint, float measurement)
{
    // Calculate the error (difference between the setpoint and the measurement)
    float error = setpoint - measurement;

    // Calculate the proportional term (Kp * error)
    float proportional = Kp * error;

    // Calculate the integral term (integration of the error)
    integrator += 0.5f * Ki * T * (error + prevError); // Trapezoidal integration method (average sum of errors)

    // Anti-windup: Limit the integrator value to prevent excessive error accumulation
    if (integrator > limMaxInt)
    {
        integrator = limMaxInt; // Clamp the integrator to the maximum allowed value
    }
    else if (integrator < limMinInt)
    {
        integrator = limMinInt; // Clamp the integrator to the minimum allowed value
    }

    // Calculate the derivative term (with a low-pass filter to avoid noise)
    differentiator = -(2.0f * Kd * (measurement - prevMeasurement) +
                       (2.0f * tau - T) * differentiator) /
                     (2.0f * tau + T);

    // Calculate the total controller output (Sum of proportional, integral, and derivative)
    out = proportional + integrator + differentiator;

    // Apply limits to the output (the controller output must be within the defined limits)
    if (out > limMax)
    {
        out = limMax; // Limit the output to the maximum
    }
    else if (out < limMin)
    {
        out = limMin; // Limit the output to the minimum
    }

    // Update the controller's memory (previous errors and measurements for the next cycle)
    prevError = error;
    prevMeasurement = measurement;

    // Return the controller output (how much the controlled variable should be adjusted)
    return out;
}

// Method to update the Kp, Ki, and Kd gains at runtime
void PIDController::updateGains(float kp, float ki, float kd)
{
    Kp = kp; // Update the proportional gain
    Ki = ki; // Update the integral gain
    Kd = kd; // Update the derivative gain
}

// Methods to get the Kp, Ki, and Kd gains individually
float PIDController::getKp() const
{
    return Kp; // Return the proportional gain
}

float PIDController::getKi() const
{
    return Ki; // Return the integral gain
}

float PIDController::getKd() const
{
    return Kd; // Return the derivative gain
}

// Method to get all gains together as a struct (useful for sending via serial, etc.)
PIDGains PIDController::getGains() const
{
    return {Kp, Ki, Kd}; // Return the three gains as a struct
}