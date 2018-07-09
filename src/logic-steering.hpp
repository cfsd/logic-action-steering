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

 #ifndef STEERING
 #define STEERING

 #include "opendlv-standard-message-set.hpp"


class Steering {
 public:
  Steering(bool verbose, uint32_t id, cluon::OD4Session &od4, float, float);
  ~Steering();
  void nextContainer(cluon::data::Envelope &);

 private:
   void setUp(u_int32_t);
   void tearDown();
   float calcSteering(float, float);
   float calcRackPosition(float);

 private:
   cluon::OD4Session &m_od4_proxy;
   bool m_verbose;
   cluon::data::TimeStamp m_latestMessage;
   float m_prevPos;
   float m_Ku;
   float m_Kp;
   std::mutex m_speedMutex;
   float m_groundSpeed;
};
#endif
