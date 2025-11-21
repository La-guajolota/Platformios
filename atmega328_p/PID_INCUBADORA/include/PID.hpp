/****************************************************************************************
 * @file PID.hpp
 * @brief This file contains the declaration of the `PIDController` class, which
 *        implements a PID controller for embedded systems. The controller includes
 *        calculations for the proportional, integral, and derivative terms, and
 *        supports dynamic gain updates and anti-windup protection for the integrator.
 *
 * @author Adrian Silva Palafox
 * @company Fourie Embeds
 * @date November 2024
 *
 * @license This code is open source under the MIT license.
 *          It can be modified and distributed for educational or commercial purposes.
 *
 * @note Ensure that the necessary libraries are correctly included and configured
 *       in your development environment.
 ***************************************************************************************/

#ifndef PID_HPP
#define PID_HPP

/**
 * @struct PIDGains
 * @brief A structure to group the PID controller gains.
 * @details This is optional and allows returning all three gains (Kp, Ki, Kd) in a single object.
 */
struct PIDGains
{
    float Kp; // Proportional gain
    float Ki; // Integral gain
    float Kd; // Derivative gain
};

class PIDController
{
public:
    /**
     * @brief Constructor for the PIDController class.
     * @details Initializes the PID parameters, such as gains, output limits, and integrator limits.
     * @param kp Proportional gain.
     * @param ki Integral gain.
     * @param kd Derivative gain.
     * @param tau Low-pass filter time constant for the derivative term.
     * @param limMin Minimum output limit of the controller.
     * @param limMax Maximum output limit of the controller.
     * @param limMinInt Minimum integrator limit to prevent wind-up.
     * @param limMaxInt Maximum integrator limit to prevent wind-up.
     * @param t Sampling time (in seconds).
     */
    PIDController(float kp, float ki, float kd,
                  float tau,                        
                  float limMin, float limMax,       
                  float limMinInt, float limMaxInt, 
                  float t);                         

    // Public methods:

    /**
     * @brief Resets the PID controller to its initial state.
     * @details Clears the integrator, previous error, etc.
     */
    void reset();

    /**
     * @brief Updates the PID controller output based on the setpoint and the current measurement.
     * @param setpoint The desired value.
     * @param measurement The current measured value.
     * @return The calculated output of the controller.
     */
    float update(float setpoint, float measurement);

    /**
     * @brief Updates the PID controller gains (Kp, Ki, Kd) in real-time.
     * @param kp The new proportional gain.
     * @param ki The new integral gain.
     * @param kd The new derivative gain.
     */
    void updateGains(float kp, float ki, float kd);

    // Methods to get the current PID gains:
    float getKp() const; // Returns the proportional gain (Kp)
    float getKi() const; // Returns the integral gain (Ki)
    float getKd() const; // Returns the derivative gain (Kd)

    /**
     * @brief Gets all the gains together as a struct.
     * @details This is useful for sending all gains in a single packet, for example, via serial communication.
     * @return A PIDGains struct containing the current gains.
     */
    PIDGains getGains() const;

private:
    // Internal parameters of the PID controller
    float Kp; // Proportional gain
    float Ki; // Integral gain
    float Kd; // Derivative gain

    float tau; // Low-pass filter time constant for the derivative term

    // Controller output limits
    float limMin; // Minimum output limit
    float limMax; // Maximum output limit

    // Integrator limits (to prevent wind-up)
    float limMinInt; // Minimum integrator limit
    float limMaxInt; // Maximum integrator limit

    // Sampling time (in seconds)
    float T;

    // Internal memory variables of the controller
    float integrator;      // Accumulator for the integral term
    float prevError;       // Error from the previous iteration (needed to calculate the derivative term)
    float differentiator;  // Stores the value of the derivative term
    float prevMeasurement; // Previous measurement (needed to calculate the derivative)

    // Controller output (final result after applying the PID formula)
    float out;
};

#endif // PID_HPP
