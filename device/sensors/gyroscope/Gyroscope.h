// Copyright (C) 2012 Mark R. Stevens
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef ardadv_sensors_magnetometer_Magnetometer_h
#define ardadv_sensors_magnetometer_Magnetometer_h

#include <sensors/common/Pin.h>

namespace ardadv
{
  namespace sensors
  {
    namespace magnetometer
    {

      //! @class Magnetometer
      //!
      //! @brief Measure the state of the attached Magnetometer and compute
      //!        heading and tilt.
      //!
      //! Coding information mainly followed from instructions at:
      //!    http://wiring.org.co/learning/basics/magneticfield.html
      //!
      //! From the datasheet
      //!
      //! MOSI (Master Out Slave In)
      //!     The data sent from the master to the MicroMag3. Data is
      //!     transferred most significant bit first. The MOSI line will
      //!     accept data once the SPI is enabled by taking the SSNOT low.
      //!     Valid data must be presented at least 100 nS before the rising
      //!     edge of the clock, and remain valid for 100 nS after the edge.
      //!     New data may be presented to the MOSI pin on the falling edge of
      //!     SCLK.
      //! SSNOT (Slave Select Line)
      //!     Selects the MicroMag3 as the operating slave device. The SSNOT
      //!     line must be low prior to data transfer and must stay low during
      //!     the entire transfer. Once the command byte is received by the
      //!     MicroMag3, and the MicroMag3 begins to execute the command, the
      //!     SSNOT line can be deselected until the next SPI transfer.
      //! SCLK (Serial Clock)
      //!     Used to synchronize both the data in and out through the MISO
      //!     and MOSI lines. SCLK is generated by a master device. SCLK should
      //!     be 1 MHz or less. The MicroMag3 is configured to run as a slave
      //!     device, making it an input. One byte of data is exchanged over
      //!     eight clock cycles. Data is captured by the master device on the
      //!     rising edge of SCLK. Data is shifted out and presented to the
      //!     MicroMag3 on the MOSI pin on the falling edge of SCLK.
      //! MISO (Master In Slave Out)
      //!     The data sent from the MicroMag3 to the master. Data is
      //!     transferred most significant bit first. The MISO line is
      //!     placed in a high impedance state if the slave is not selected
      //!     (SSNOT = 1).
      //! RESET
      //!     RESET us usually low. RESET must be toggled from low-high-low.
      //! DRDY (Data Ready)
      //!     DRDY is low after a RESET; after a command has been received
      //!     and the data is ready, DRDY will be high. It is recommended
      //!     that the DRDY line always be used to ensure that the data is
      //!     clocked out of the MicroMag3 only when it is available. If
      //!     it is determined that the DRDY line cannot be used due to lack
      //!     of I/O lines to the host processor, then the times listed in
      //!     the table below can be used to set open-loop wait times. The
      //!     values listed are the maximum delays from the end of the SCLK
      //!     command until the rise of the DRDY at each period select setting.
      //!     The maximum delay occurs when the sensor being sampled is in a
      //!     zero field
      //!
      class Magnetometer
      {
      public:

        //! @brief Used to reduce ambiguity in pin assignment
        //!
        typedef common::Pin<0> DRDY;
        typedef common::Pin<1> RESET;

        //! @brief Constructor
        //!
        Magnetometer();

        //! @brief Set the internal pin
        //!
        //! @param[in] DRDY  Data ready
        //! @param[in] RESET Reset
        //!
        //! @return true if setup successfully
        //!
        bool setup(const DRDY& drdy, const RESET& reset);

        //! @brief Update the state
        //!
        void update();

        //! @brief Access the raw data
        //!
        //! @return the position
        //!
        inline float x() const
        {
          return mValueX;
        }

        //! @brief Access the raw data
        //!
        //! @return the position
        //!
        inline float y() const
        {
          return mValueY;
        }

        //! @brief Access the raw data
        //!
        //! @return the position
        //!
        inline float z() const
        {
          return mValueZ;
        }

        //! @brief Mark if the reading is valid or not
        //!
        //! @return true if valid
        //!
        inline bool isValid() const
        {
          return mValid;
        }

        //! @brief Return the part number
        //!
        //! @return the part number
        //!
        static inline const char* PartNumber()
        {
          return "MicroMag3 12349";
        }

        //! @brief Return the vendor
        //!
        //! @return the vendor
        //!
        static inline const char* Vendor()
        {
          return "PNI Corporation";
        }

      protected:

        //! @brief Send a command to take a measurement
        //!
        //! @param[in] axis   The requested axis
        //! @param[in] period The time interval
        //!
        //! @return True if command was sent properly
        //!
        bool convert(uint8_t axis, uint8_t period) const;

        //! @brief Request a measurement and convert
        //!
        //! @param[in]  axis    The requested axis
        //! @param[in]  period  The time interval
        //! @param[out] result  The measurement returned
        //! @param[in]  timeout The time to wait on the request
        //!
        //! @return True if a measurement was made
        //!
        bool read(uint8_t axis, uint8_t period, int16_t& result, uint16_t timeout=0) const;

        //! @brief Get the result of the command
        //!
        //! @return The result of the measurement taken
        //!
        int16_t getResult() const;

        //! @brief Pulse the reset
        //!
        void pulseReset() const;

        //! @brief Read the axis
        //!
        //! @param[in] axis The axis to read
        //!
        //! @return The axis value
        //!
        float readAxis(int axis);

     private:

        //! @brief The pins
        //!
        DRDY  mDRDY;
        RESET mRESET;

        //! @brief The pin values
        //!
        float mValueX;
        float mValueY;
        float mValueZ;

        //! @brief Indicate if valid
        //!
        bool mValid;
      };
    }
  }
}
#endif

