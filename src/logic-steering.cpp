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



Steering::Steering(bool verbose, uint32_t id, cluon::OD4Session &od4_proxy, float Kp)
  : m_od4_proxy(od4_proxy)
  , m_verbose(verbose)
  , m_latestMessage()
  , m_prevPos()
  , m_Kp(Kp)
  {
  setUp(id);
  }

Steering::~Steering()
{
  tearDown();
}


void Steering::nextContainer(cluon::data::Envelope &a_container){
  if (a_container.dataType() == opendlv::logic::action::AimPoint::ID()) {
    if (cluon::time::toMicroseconds(a_container.sampleTimeStamp()) > cluon::time::toMicroseconds(m_latestMessage)) {
      auto steering = cluon::extractMessage<opendlv::logic::action::AimPoint>(std::move(a_container));
      float tol = 0.0f; //Needs to be calibrated
      float azimuth = steering.azimuthAngle();
      float distance = steering.distance();
      float delta = calcSteering(azimuth, distance);
      float rackPosition = calcRackPosition(delta);


      if (std::abs(rackPosition-m_prevPos) > tol){
        opendlv::proxy::GroundSteeringRequest out1;
        out1.groundSteering(rackPosition);

        std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
        cluon::data::TimeStamp sampleTime = cluon::time::convert(tp);

        m_prevPos = rackPosition;
        m_od4_proxy.send(out1,sampleTime,314);
      }
    }
  }
}

void Steering::setUp(u_int32_t id)
{
  if (m_verbose){
    std::cout << "Setting up steering" << std::endl;
    std::cout << "Steering ID: " << id << std::endl;
    std::cout << m_Kp << std::endl;
  }
  std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
  m_latestMessage = cluon::time::convert(tp);
  m_prevPos = 0.0f;
}

void Steering::tearDown()
{
}

float Steering::calcRackPosition(float delta) {
  const float conversionConst = 46.76f;
  float rackPosition = delta*conversionConst; // rack position in mm
  rackPosition = std::max(std::min(rackPosition,20.0f),-20.0f);
  return rackPosition;
}

float Steering::calcSteering(float azimuth, float distance) {
  float delta = m_Kp*azimuth;
  (void) distance;
  return delta;
}
