
clear all;
close all;
A = csvread('Capture.txt');
t = A(:,1);
servo   = A(:,2);
gyro = -A(:,3);
compass = -(A(:,4) - 135);
t = 1:length(servo);
dt = conv(A(:,1),[1,-1], 'valid');
figure(1), plot(t, servo, 'r-', t, compass, 'g-', t, cumsum(gyro/13), 'b-');
legend('servo', 'magnetometer', 'gyroscope');
ylabel('angle');
xlabel('time');


% Kalman filter variables and constants
Q_angleX = 0.001; % Process noise covariance for the compass - Sw
Q_gyroX = 0.003; % Process noise covariance for the gyro - Sw
R_angleX = 0.2; % Measurement noise covariance - Sv

angleX = 180; % The angle output from the Kalman filter
biasX = 0; % The gyro bias calculated by the Kalman filter
PX_00 = 0;
PX_01 = 0;
PX_10 = 0;
PX_11 = 0;

% KasBot V2  -  Kalman filter module - http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1284738418
% See also http://www.x-firm.com/?page_id=145
% with slightly modifications by Kristian Lauszus
% See http://academic.csuohio.edu/simond/courses/eec644/kalman.pdf and
% http://www.cs.unc.edu/~welch/media/pdf/kalman_intro.pdf for more information

angleX = 0;
combo(1) = 0;
for i=2:length(compass)
    
    newAngle = compass(i);
    newRate  = gyro(i);
    
    dtX = 1 / 13; % Convert from microseconds to seconds
    
    % Discrete Kalman filter time update equations - Time Update ("Predict")
    % Update xhat - Project the state ahead
    
    angleX = angleX + dtX * (newRate - biasX);
    
    % Update estimation error covariance - Project the error covariance ahead
    PX_00 = PX_00 + -dtX * (PX_10 + PX_01) + Q_angleX * dtX;
    PX_01 = PX_01 + -dtX * PX_11;
    PX_10 = PX_10 + -dtX * PX_11;
    PX_11 = PX_11 + +Q_gyroX * dtX;
    
    % Discrete Kalman filter measurement update equations - Measurement Update ("Correct")
    % Calculate Kalman gain - Compute the Kalman gain
    SX = PX_00 + R_angleX;
    KX_0 = PX_00 / SX;
    KX_1 = PX_10 / SX;
    
    % Calculate angle and resting rate - Update estimate with measurement zk
    yX = newAngle - angleX;
    angleX = angleX + KX_0 * yX;
    biasX = biasX + KX_1 * yX;
    
    % Calculate estimation error covariance - Update the error covariance
    PX_00 = PX_00 - KX_0 * PX_00;
    PX_01 = PX_01 - KX_0 * PX_01;
    PX_10 = PX_10 - KX_1 * PX_00;
    PX_11 = PX_11 - KX_1 * PX_01;
    
    combo(i) = angleX;
end

figure(4), plot(t, servo, 'r-', t, combo, 'b-');
legend('servo', 'Kalman');
ylabel('angle');
xlabel('time');
