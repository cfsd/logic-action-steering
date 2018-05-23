/**
* Copyright (C) 2017 Chalmers Revere
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
* USA.
*/

#include <iostream>
#include "cluon-complete.hpp"
#include "logic-steering.hpp"

#define PI 3.14159265359f



Steering::Steering(bool verbose, uint32_t id, cluon::OD4Session &od4)
  : m_od4(od4)
{
  setUp();
  (void)verbose;
  (void)od4;
  (void)id;
}

Steering::~Steering()
{
  tearDown();
}


void Steering::nextContainer(cluon::data::Envelope &a_container)
{
  std::cout << "I recieved a container!" << std::endl;
  if (a_container.dataType() == opendlv::logic::action::AimPoint::ID()) {
    auto steering = cluon::extractMessage<opendlv::logic::action::AimPoint>(std::move(a_container));
    float azimuth = steering.azimuthAngle();
    float distance = steering.distance();
    float delta = calcSteering(azimuth, distance);
    float rackPosition = calcRackPosition(delta);
    (void) rackPosition; // Ask Dan if we send rack pos or delta
    opendlv::proxy::GroundAccelerationRequest out1;
    out1.groundSteering(delta);

    std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
    cluon::data::TimeStamp sampleTime = cluon::time::convert(tp);

    m_od4.send(out1,sampleTime,1);
  }
}

void Steering::setUp()
{
  // std::string const exampleConfig =
  std::cout << "Setting up steering" << std::endl;
}

void Steering::tearDown()
{
}

float Steering::calcRackPosition(float delta) {
  const float conversionConst = 46.76;
  float rackPosition = delta*conversionConst; // rack position in mm
  return rackPosition;
}

float Steering::calcSteering(float azimuth, float distance) {
  float Kp = 2.0f;
  float delta = Kp*azimuth*distance;

  return delta;
}
