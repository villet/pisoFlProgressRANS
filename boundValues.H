/* Bounds calculated values between 0...1 */
Zmean = max(0.0, min(1.0, Zmean));
Zmean.correctBoundaryConditions();

Cmean = max(0.0, min(1.0, Cmean));
Cmean.correctBoundaryConditions();
