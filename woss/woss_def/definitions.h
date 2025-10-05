/* WOSS - World Ocean Simulation System -
 * 
 * Copyright (C) 2009 2025 Federico Guerra
 * and regents of the SIGNET lab, University of Padova
 *
 * Author: Federico Guerra - WOSS@guerra-tlc.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */ 


/**
 * @file   definitions.h
 * @author Federico Guerra
 *
 * \brief  Generic functions and variables  
 *
 * Collects all generic functions and variables 
 */


#ifndef WOSS_DEFINITIONS_H
#define WOSS_DEFINITIONS_H


#include <iostream>
#include <cmath>
#include <tuple>


namespace woss {

  using Bathymetry = double;

  /** 
  * precision used for write-buffer spacing purposes 
  **/
  static constexpr inline int WOSS_DECIMAL_PRECISION = 17;

  static constexpr inline int WOSS_STREAM_TAB_SPACE = 25;



  /**
  * Pause the execution of the program. Restart it by pressing any key.
  **/
  void debugWaitForUser();

  constexpr double normCart( double x, double y, double z );

  constexpr std::tuple< double, double, double> convSpher2Cart( double rho, double theta, double phi );

  constexpr std::tuple<double, double, double> convCart2Spher( double x, double y, double z );
  
  constexpr std::tuple<double, double> rotCartXY( double rot_angle, double x_init, double y_init );
  
  constexpr std::tuple<double, double> rotCartXZ( double rot_angle, double x_init, double z_init );
  
  constexpr std::tuple<double, double> rotCartYZ( double rot_angle, double y_init, double z_init );
  
  constexpr std::tuple<double, double, double> crossProd( double left_x, double left_y, double left_z, double right_x, double right_y, double right_z );
  
  constexpr double scalarProd( double left_x, double left_y, double left_z, double right_x, double right_y, double right_z);

  constexpr double linInterp( double start_var, double end_var, double start_val, double end_val, double x_var );

  /////////////////////////////
  inline void debugWaitForUser()
  {
    std::string response;
    std::cout << "Press Enter to continue";
    std::getline( std::cin, response);
  }

  inline constexpr double normCart( double x, double y, double z ) {
    return ( sqrt(x*x + y*y + z*z) );
  }

  inline constexpr std::tuple<double, double, double> convSpher2Cart( double rho, double theta, double phi ) {
    auto x = rho*std::sin(theta) * std::cos(phi);
    auto y = rho*std::sin(theta) * std::sin(phi);
    auto z = rho*std::cos(theta); 
    return {x, y, z};
  }

  inline constexpr std::tuple<double, double, double> convCart2Spher( double x, double y, double z ) {
    auto rho = std::sqrt( x*x + y*y + z* z);
    auto theta = std::acos( z / rho );
    auto phi = std::atan2( y, x );
    return {rho, theta, phi};
  }

  inline constexpr std::tuple<double, double> rotCartXY( double rot_angle, double x_init, double y_init ) {
    if (rot_angle == 0) {
      auto x = x_init;
      auto y = y_init;
      return {x, y};
    }
    auto x = std::cos(rot_angle)*x_init - std::sin(rot_angle)*y_init;
    auto y = std::sin(rot_angle)*x_init + std::cos(rot_angle)*y_init;
    return {x, y};
  }

  inline constexpr std::tuple<double, double> rotCartXZ( double rot_angle, double x_init, double z_init ) {
    if (rot_angle == 0) {
      auto x = x_init;
      auto z = z_init;
      return {x, z};
    }  
    auto x = std::cos(rot_angle)*x_init - std::sin(rot_angle)*z_init;
    auto z = std::sin(rot_angle)*x_init + std::cos(rot_angle)*z_init;
    return {x, z};
  }

  inline constexpr std::tuple<double, double> rotCartYZ( double rot_angle, double y_init, double z_init ) {
    if (rot_angle == 0) {
      auto y = y_init;
      auto z = z_init;
      return {y, z};
    }  
    auto y = std::cos(rot_angle)*y_init - std::sin(rot_angle)*z_init;
    auto z = std::sin(rot_angle)*y_init + std::cos(rot_angle)*z_init;
    return {y, z};
  }

  inline constexpr std::tuple<double, double, double> crossProd( double left_x, double left_y, double left_z, double right_x, double right_y, double right_z ) {
    auto res_x = left_y*right_z - left_z*right_y;
    auto res_y = left_z*right_x - left_x*right_z;
    auto res_z = left_x*right_y - left_y*right_x;
    return {res_x, res_y, res_z};
  }

  inline constexpr double scalarProd( double left_x, double left_y, double left_z, double right_x, double right_y, double right_z ) {
    return( left_x*right_x + left_y*right_y + left_z*right_z );
  }

  inline constexpr double linInterp( double start_var, double end_var, double start_val, double end_val, double x_var ) {
    return (end_val-start_val) / (end_var-start_var) * (x_var-start_var) + start_val;
  }

}

#endif /* WOSS_DEFINITIONS_H */

